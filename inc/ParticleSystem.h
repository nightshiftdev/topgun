////////////////////////////////////////////////////////////////////////
//
// ParticleSystem.h
//
// ilPadrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef __PARTICLESYSTEM_H
#define __PARTICLESYSTEM_H

#include <coeccntx.h>

////////////////////////////////////////////////////////////////////////

class CBaseAnim;

////////////////////////////////////////////////////////////////////////

class TVectorFP
	{
public:
	TVectorFP();
	TVectorFP(const TPoint& aPoint);

	TPoint ScaleToPoint(TInt aFactor) const;
	
public:
	TInt iX;
	TInt iY;
	};

//////////////////////////////////////////////////////////

class TParticle
	{
public:

	TParticle();
	void Reset();
	void Start(TInt iVirtualFrameNum,TInt iDeltaVFN,TInt iMaxVFN,TPoint iPosition,TPoint iVelocity,TInt iDragRatio);
	TBool IsActive() const;

	void Simulate();
	void Render(CBaseAnim& aAnim, const TPoint& aOrigin,const TRect& aScreenRect, CFbsBitGc* aCallerGc) const;

private:

	enum
	{
		dragRatioDownshift = 8,
	};

	TInt iVirtualFrameNum;
	TInt iDeltaVFN;
	TInt iMaxVFN;
	TPoint iPosition;
	TPoint iVelocity;
	TInt iDragRatio;
	};

////////////////////////////////////////////////////////////////////////

class CParticleSystem : public CBase
	{
public:
	static CParticleSystem* NewL(TInt aMaxParticles,CBaseAnim& aAnim);
	~CParticleSystem();

	void Reset();
	void Simulate();
	void Render(const TPoint& aOrigin,const TRect& aScreenRect,CFbsBitGc* aCallerGc) const;
	void Generate(const TPoint& aWorldPos, const TVectorFP& aVector, const TPoint& aGeneratorVelocity, 
					const TInt aRadius, TInt aSpeed);

protected:
	CParticleSystem(CBaseAnim& aAnim);
	void ConstructL(TInt aMaxParticles);

protected:
	TInt64 iSeed;
	TInt iMaxParticles;
	TParticle * iParticles;
	TInt iNextParticleIndex;
	CBaseAnim& iAnim;
	};

////////////////////////////////////////////////////////////////////////

#endif

