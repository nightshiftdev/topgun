////////////////////////////////////////////////////////////////////////
//
// ParticleSystem.cpp
//
// ilPadrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#include <e32math.h>
#include "RenderableFactory.h"
#include "BaseAnim.h"
#include "ParticleSystem.h"

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

TVectorFP::TVectorFP()
	{
	iX = 0;
	iY = 0;
	}

////////////////////////////////////////////////////////////////////////

TVectorFP::TVectorFP(const TPoint& aPoint)
	{
	iX = aPoint.iX;
	iY = aPoint.iY;
	}

////////////////////////////////////////////////////////////////////////

TPoint TVectorFP::ScaleToPoint(TInt aFactor) const
	{
	TPoint result;

	result.iX = ( iX * aFactor ) >> 8;
	result.iY = ( iY * aFactor ) >> 8;

	return result;
	}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

TParticle::TParticle()
	{
	Reset();
	}

////////////////////////////////////////////////////////////////////////

void TParticle::Reset()
	{
	iVirtualFrameNum = -1;
	}

////////////////////////////////////////////////////////////////////////

void TParticle::Start(TInt aVirtualFrameNum,TInt aDeltaVFN,TInt aMaxVFN,TPoint aPosition,TPoint aVelocity,TInt aDragRatio)
	{
	iVirtualFrameNum = aVirtualFrameNum;
	iDeltaVFN 		 = aDeltaVFN;        
	iMaxVFN 		 = aMaxVFN;          
	iPosition 		 = aPosition;        
	iVelocity 		 = aVelocity;        
	iDragRatio 		 = aDragRatio;
	}

////////////////////////////////////////////////////////////////////////

void TParticle::Simulate()
	{
	if ( !IsActive() )
		{
		return;
		}
	else
		{
		iVirtualFrameNum += iDeltaVFN;

		// Have we finished yet?
		if (iVirtualFrameNum >= iMaxVFN)
			{
			// Yes, so switch off:
			Reset();
			}

		iPosition += iVelocity;
		iVelocity.iX = ( iVelocity.iX * iDragRatio ) >> dragRatioDownshift;
		iVelocity.iY = ( iVelocity.iY * iDragRatio ) >> dragRatioDownshift;
		}
	}

////////////////////////////////////////////////////////////////////////

TBool TParticle::IsActive() const
	{
	return iVirtualFrameNum >= 0;
	}

////////////////////////////////////////////////////////////////////////

void TParticle::Render(CBaseAnim& aAnim, const TPoint& aOrigin,const TRect& aScreenRect,CFbsBitGc* aCallerGc) const
	{
	if ( IsActive() )
		{
		TInt screenX = ( iPosition.iX >> 4 ) - aOrigin.iX;
		TInt screenY = ( iPosition.iY >> 4 ) - aOrigin.iY;

		TPoint screenPos( screenX, screenY );

		aAnim.RenderFrameCentered(iVirtualFrameNum >> 8,screenPos,aScreenRect,aCallerGc);
		}
	}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CParticleSystem::CParticleSystem(CBaseAnim& aAnim) :
iAnim(aAnim)
	{
	// A "don't care" value:
	iSeed = 3141592;
	}

////////////////////////////////////////////////////////////////////////

CParticleSystem* CParticleSystem::NewL(TInt aMaxParticles,CBaseAnim& aAnim)
	{
	CParticleSystem* self = new(ELeave) CParticleSystem(aAnim);
	CleanupStack::PushL(self);
	self->ConstructL(aMaxParticles);
	CleanupStack::Pop();
	return self;
	}

////////////////////////////////////////////////////////////////////////

CParticleSystem::~CParticleSystem()
	{
	delete[] iParticles;
	iParticles = NULL;
	}

////////////////////////////////////////////////////////////////////////

void CParticleSystem::ConstructL(TInt aMaxParticles)
    {
	iMaxParticles = aMaxParticles;
	iParticles = new TParticle[ aMaxParticles ];
	User::LeaveIfNull(iParticles);
	Reset();
	}

////////////////////////////////////////////////////////////////////////

void CParticleSystem::Reset()
	{
	for ( TInt p = 0 ; p < iMaxParticles ; p++ )
		{
		iParticles[ p ].Reset();
		}

	iNextParticleIndex = 0;
	}

////////////////////////////////////////////////////////////////////////

void CParticleSystem::Simulate()
	{
	for ( TInt p = 0 ; p < iMaxParticles ; p++ )
		{
		iParticles[ p ].Simulate();
		}
	}

////////////////////////////////////////////////////////////////////////

void CParticleSystem::Render(const TPoint& aOrigin,const TRect& aScreenRect,CFbsBitGc* aCallerGc) const
	{
	//
	// We take care to maintain a consistent drawing order,
	// oldest ( == least recently started ) first:
	//
	TInt p;

	for ( p = iNextParticleIndex - 1 ; p >= 0 ; p-- )
		{
		iParticles[ p ].Render(iAnim,aOrigin,aScreenRect,aCallerGc);
		}

	for ( p = iMaxParticles - 1 ; p >= iNextParticleIndex ; p-- )
		{
		iParticles[ p ].Render(iAnim,aOrigin,aScreenRect,aCallerGc);
		}
	}

////////////////////////////////////////////////////////////////////////

void CParticleSystem::Generate(const TPoint& aWorldPos, const TVectorFP& aVector, const TPoint& aGeneratorVelocity,
									const TInt aRadius, const TInt aSpeed)
	{
	TParticle * p_particle = &iParticles[ iNextParticleIndex ];
	iNextParticleIndex++;
	if (iNextParticleIndex >= iMaxParticles)
		{
		iNextParticleIndex = 0;
		}

	// We start the particle downrange from the notional point of origin:
	TPoint position = aWorldPos + aVector.ScaleToPoint(aRadius);

	// The particle's initial velocity includes a component from the generator's own motion:
	TPoint velocity = aGeneratorVelocity + aVector.ScaleToPoint(aSpeed);

	// Generate a (pseudo-)random integer for use in setting 
	// parameters for the new particle. We only want the topmost
	// few bits of the Math::Rand() result, since these have the 
	// least "pattern" to them:
	TInt random = Math::Rand(iSeed) >> 22;

	// Decide how fast to run the animation for the new particle.
	// A deltaVFN of 0x0100 implies an animation rate of one frame
	// per simulation cycle (that is, we will get a rate of between 
	// 1 and 1.5 frames per simulation cycle):
	TInt randDeltaVFN = 0x0100 + ( random & 0x7f );

	// Decide which animation frame to start our particle on.
	// A VFN of 0x0100 corresponds to frame #1 (that is, we skip
	// frame #0 of the animation 50% of the time):
	TInt randVFN = random & 0x0100;

	p_particle->Start(randVFN,randDeltaVFN,0x0fff,position,velocity,0x00e0);
	}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


