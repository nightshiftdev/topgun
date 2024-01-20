////////////////////////////////////////////////////////////////////////
//
// EnemyAnim.cpp
//
// Copyright (c) 2004 IlPadrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#include <e32math.h>
#include "RenderableFactory.h"
#include "BitmapAnimFrame.h"
#include "EnemyAnim.h"
#include "EnemyType.h"

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CEnemyAnim::CEnemyAnim() :
CBaseAnim(TSize(0,0))
	{
	}

////////////////////////////////////////////////////////////////////////

CEnemyAnim* CEnemyAnim::NewL(CFbsBitmap& aSourceImage,
							 const TPoint &aScreenPos,
							 TInt aSpeed, 
							 TInt aEnergy,
							 TInt aOrgEnergy,
							 TInt aZigZac,
							 const TSize &aSize,
							 const TEnemy &aType)
	{
	CEnemyAnim* self = new(ELeave) CEnemyAnim();
	CleanupStack::PushL(self);
	self->ConstructL(aSourceImage,
					 aScreenPos,
					 aSpeed,
					 aEnergy,
					 aOrgEnergy,
					 aZigZac,
					 aSize,
					 aType);
	CleanupStack::Pop();
	return self;
	}

////////////////////////////////////////////////////////////////////////

CEnemyAnim::~CEnemyAnim()
	{
	}

////////////////////////////////////////////////////////////////////////

void CEnemyAnim::ConstructL(CFbsBitmap& aSourceImage,
							const TPoint &aScreenPos,
							TInt aSpeed, 
							TInt aEnergy,
							TInt aOrgEnergy,
							TInt aZigZac,
							const TSize &aSize,
							const TEnemy &aType)
    {
	//Initialize Enemy's parameters
	iEnemyScreenPos = aScreenPos;
	iEnemyScreenPosOrg = aScreenPos;
	iEnemySpeed = aSpeed;
	iEnemyEnergy = aEnergy;
	iEnemyOrgEnergy = aOrgEnergy;
	iSize = aSize;
	iEnemyType = aType;
	iZigZac = aZigZac;
	iOrgZigZac = aZigZac;
	iBound  = EFalse;
	AllocRenderablesL(1);

	// Create image, device and context (only needed briefly):
	CFbsBitmap* image = new (ELeave) CFbsBitmap();
	CleanupStack::PushL(image);
	User::LeaveIfError( image->Create(iSize, EColor4K ) );
	CFbsBitmapDevice* device = CFbsBitmapDevice::NewL(image);
	CleanupStack::PushL(device);
	CFbsBitGc* gc;
	User::LeaveIfError(device->CreateContext(gc));
	gc->SetBrushColor(TRgb(0,0,0));

	// Create Enemy frames:
	TInt frame;

	for (frame = 0 ; frame <= 0 ; frame++)
		{
		gc->Clear();
		BuildFrame(aSourceImage,frame,gc);
		AddRenderable(CBitmapAnimFrame::NewL(*image));
		}

	// Destroy context, device and image;
	delete gc;
	CleanupStack::PopAndDestroy(2);
	}

////////////////////////////////////////////////////////////////////////

void CEnemyAnim::BuildFrame(CFbsBitmap& aSourceImage,TInt frame_no,CFbsBitGc* aGc)
{
	// Convert frame to x-coord:
	TInt frame_x = frame_no << 5;

	aGc->BitBlt(TPoint(0,0),&aSourceImage,TRect(TPoint(frame_x,0),iSize));
}

////////////////////////////////////////////////////////////////////////

void CEnemyAnim::SetEnemyScreenPos(const TPoint &aEnemyScreenPos)
	{
	if (iOrgZigZac != 0)
		{
		if ((iZigZac > 0) && (!iBound))
			{
			iZigZac -= 1;
			iEnemyScreenPos = aEnemyScreenPos;
			iEnemyScreenPos.iX = aEnemyScreenPos.iX + 1;
			if (iZigZac == 0)
				{
				iBound  = ETrue;
				}
			}
		
		if ((iZigZac <= iOrgZigZac) && (iBound))
			{
			iZigZac +=1;
			iEnemyScreenPos = aEnemyScreenPos;
			iEnemyScreenPos.iX = aEnemyScreenPos.iX - 1;
		    if (iZigZac == iOrgZigZac)
				{
				iBound = EFalse;
				}
			}
		}
	else
		{
		iEnemyScreenPos = aEnemyScreenPos;
		}
	}

////////////////////////////////////////////////////////////////////////

TPoint CEnemyAnim::GetEnemyScreenPos()
	{
	return iEnemyScreenPos;
	}

////////////////////////////////////////////////////////////////////////

TSize CEnemyAnim::GetEnemySize()
	{
	return iSize;
	}

////////////////////////////////////////////////////////////////////////

TPoint CEnemyAnim::GetEnemyScreenPosOrg()
	{
	return iEnemyScreenPosOrg;
	}

////////////////////////////////////////////////////////////////////////

TEnemy CEnemyAnim::EnemyType()
	{
	return iEnemyType;
	}

////////////////////////////////////////////////////////////////////////

TBool CEnemyAnim::EnemyHit()
	{
	iEnemyEnergy--;
	
	if (iEnemyEnergy <= 0)
		{
		iEnemyEnergy = iEnemyOrgEnergy;//set to start value, change the code later
		return ETrue;
		}
	else
		{
		return EFalse;
		}
	}

////////////////////////////////////////////////////////////////////////

TInt CEnemyAnim::GetEnemySpeed()
	{
	return iEnemySpeed;	
	}

////////////////////////////////////////////////////////////////////////

void CEnemyAnim::SetEnemySpeed(TInt aSpeed)
	{
	iEnemySpeed = aSpeed;
	}

////////////////////////////////////////////////////////////////////////

void CEnemyAnim::ZigZac(TInt aMove)
	{
	iZigZac = aMove;
	}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

