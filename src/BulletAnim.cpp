////////////////////////////////////////////////////////////////////////
//
// BulletAnim.cpp
//
// Copyright (c) 2004 IlPadrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#include <e32math.h>
#include "RenderableFactory.h"
#include "BitmapAnimFrame.h"
#include "BulletAnim.h"

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CBulletAnim::CBulletAnim() :
CBaseAnim(TSize(0,0))
	{
	}

////////////////////////////////////////////////////////////////////////

CBulletAnim* CBulletAnim::NewL(CFbsBitmap& aSourceImage,
							 const TPoint &aScreenPos,
							 TInt aSpeed,
							 TSize aSize,
							 const TBulletOrientation &aBulletOrientation)
	{
	CBulletAnim* self = new(ELeave) CBulletAnim();
	CleanupStack::PushL(self);
	self->ConstructL(aSourceImage,
					 aScreenPos,
					 aSpeed,
					 aSize,
					 aBulletOrientation);
	CleanupStack::Pop();
	return self;
	}

////////////////////////////////////////////////////////////////////////

CBulletAnim::~CBulletAnim()
	{
	}

////////////////////////////////////////////////////////////////////////

void CBulletAnim::ConstructL(CFbsBitmap& aSourceImage,
							const TPoint &aScreenPos,
							TInt aSpeed,
							TSize aSize,
							const TBulletOrientation &aBulletOrientation)
    {
	//Initialize Bullet's parameters
	iBulletOrientation = aBulletOrientation;
	iSize = aSize;
	iBulletScreenPos = aScreenPos;
	iBulletScreenPosOrg = aScreenPos;
	iBulletSpeed = aSpeed;

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

	// Create Bullet frames:
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

void CBulletAnim::BuildFrame(CFbsBitmap& aSourceImage,TInt frame_no,CFbsBitGc* aGc)
	{
	// Convert frame to x-coord:
	TInt frame_x = frame_no << 5;

	aGc->BitBlt(TPoint(0,0),&aSourceImage,TRect(TPoint(frame_x,0),iSize));
	}

////////////////////////////////////////////////////////////////////////

void CBulletAnim::SetBulletScreenPos(const TPoint &aBulletScreenPos)
	{
	iBulletScreenPos = aBulletScreenPos;
	}

////////////////////////////////////////////////////////////////////////

TBulletOrientation CBulletAnim::BulletOrientation()
	{
	return iBulletOrientation;
	}

////////////////////////////////////////////////////////////////////////

TPoint CBulletAnim::GetBulletScreenPos()
	{
	return iBulletScreenPos;
	}

////////////////////////////////////////////////////////////////////////

TPoint CBulletAnim::GetBulletScreenPosOrg()
	{
	return iBulletScreenPosOrg;
	}

////////////////////////////////////////////////////////////////////////

TInt CBulletAnim::GetBulletSpeed()
	{
	return iBulletSpeed;	
	}

////////////////////////////////////////////////////////////////////////

void CBulletAnim::SetBulletSpeed(TInt aSpeed)
	{
	iBulletSpeed = aSpeed;
	}

////////////////////////////////////////////////////////////////////////

TSize CBulletAnim::GetBulletSize()
	{
	return iSize;
	}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

