////////////////////////////////////////////////////////////////////////
//
// FireAnim.cpp
//
// Copyright (c) 2003 Nokia Mobile Phones Ltd.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#include <e32math.h>
#include "RenderableFactory.h"
#include "BitmapAnimFrame.h"
#include "FireAnim.h"

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CFireAnim::CFireAnim() :
CBaseAnim(TSize(16,16))
	{
	}

////////////////////////////////////////////////////////////////////////

CFireAnim* CFireAnim::NewL(CFbsBitmap& aSourceImage)
	{
	CFireAnim* self = new(ELeave) CFireAnim();
	CleanupStack::PushL(self);
	self->ConstructL(aSourceImage);
	CleanupStack::Pop();
	return self;
	}

////////////////////////////////////////////////////////////////////////

CFireAnim::~CFireAnim()
	{
	}

////////////////////////////////////////////////////////////////////////

void CFireAnim::ConstructL(CFbsBitmap& aSourceImage)
    {
	AllocRenderablesL(16);

	// Create image, device and context (only needed briefly):
	CFbsBitmap* image = new (ELeave) CFbsBitmap();
	CleanupStack::PushL(image);
	User::LeaveIfError( image->Create(iSize, EColor4K ) );
	CFbsBitmapDevice* device = CFbsBitmapDevice::NewL(image);
	CleanupStack::PushL(device);
	CFbsBitGc* gc;
	User::LeaveIfError(device->CreateContext(gc));
	gc->SetBrushColor(TRgb(0,0,0));

	// Create ship frames:
	TInt frame;
	for (frame = 0 ; frame < 16 ; frame++)
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

void CFireAnim::BuildFrame(CFbsBitmap& aSourceImage,TInt frame_no,CFbsBitGc* aGc)
{
	// Convert frame to x-coord:
	TInt frame_x = frame_no << 4;

	aGc->BitBlt(TPoint(0,0),&aSourceImage,TRect(TPoint(frame_x,0),iSize));
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
