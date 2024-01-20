////////////////////////////////////////////////////////////////////////
//
// ExplosionAnim.cpp
//
// Copyright (c) 2004 IlPadrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#include <e32math.h>
#include "RenderableFactory.h"
#include "BitmapAnimFrame.h"
#include "ExplosionAnim.h"

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CExplosionAnim::CExplosionAnim() :
CBaseAnim(TSize(64,64))
	{
	}

////////////////////////////////////////////////////////////////////////

CExplosionAnim* CExplosionAnim::NewL(CFbsBitmap& aSourceImage,const TPoint &aScreenPos)
	{
	CExplosionAnim* self = new(ELeave) CExplosionAnim();
	CleanupStack::PushL(self);
	self->ConstructL(aSourceImage,aScreenPos);
	CleanupStack::Pop();
	return self;
	}

////////////////////////////////////////////////////////////////////////

CExplosionAnim::~CExplosionAnim()
	{
	}

////////////////////////////////////////////////////////////////////////

void CExplosionAnim::ConstructL(CFbsBitmap& aSourceImage,const TPoint &aScreenPos)
    {
	//Initialize Explosion's parameters
	iExplosionScreenPos = aScreenPos;

	AllocRenderablesL(13);

	// Create image, device and context (only needed briefly):
	CFbsBitmap* image = new (ELeave) CFbsBitmap();
	CleanupStack::PushL(image);
	User::LeaveIfError( image->Create(iSize, EColor4K ) );
	CFbsBitmapDevice* device = CFbsBitmapDevice::NewL(image);
	CleanupStack::PushL(device);
	CFbsBitGc* gc;
	User::LeaveIfError(device->CreateContext(gc));
	gc->SetBrushColor(TRgb(0,0,0));

	// Create Explosion frames:
	TInt frame;

	for (frame = 0 ; frame < 13 ; frame++)
		{
		gc->Clear();
		/*
		if (frame%2 == 0)
			{
			BuildFrame(aSourceImage,0,gc);
			}
		else
			{
			BuildFrame(aSourceImage,frame,gc);
			}
		*/
		BuildFrame(aSourceImage,frame,gc);
		//BuildFrame(aSourceImage,0,gc);
		AddRenderable(CBitmapAnimFrame::NewL(*image));
		}

	// Destroy context, device and image;
	delete gc;
	CleanupStack::PopAndDestroy(2);
	}

////////////////////////////////////////////////////////////////////////

void CExplosionAnim::BuildFrame(CFbsBitmap& aSourceImage,TInt frame_no,CFbsBitGc* aGc)
	{
	// Convert frame to x-coord:
	TInt frame_x = frame_no << 6;
	//TInt frame_x = frame_no;

	//aGc->BitBlt(TPoint(0,0),&aSourceImage,TRect(TPoint(frame_x,0),TSize(65,65)));
	aGc->BitBlt(TPoint(0,0),&aSourceImage,TRect(TPoint(frame_x,0),iSize));
	//aGc->Clear();
	//aGc->BitBlt(TPoint(0,0),&aSourceImage);
	}

////////////////////////////////////////////////////////////////////////

void CExplosionAnim::SetExplosionScreenPos(const TPoint &aExplosionScreenPos)
	{
	iExplosionScreenPos = aExplosionScreenPos;
	}

////////////////////////////////////////////////////////////////////////

TPoint CExplosionAnim::GetExplosionScreenPos()
	{
	return iExplosionScreenPos;
	}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

