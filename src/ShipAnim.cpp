////////////////////////////////////////////////////////////////////////
//
// ShipAnim.cpp
//
// Copyright (c) 2003 Nokia Mobile Phones Ltd.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#include <e32math.h>
#include "RenderableFactory.h"
#include "BitmapAnimFrame.h"
#include "ShipAnim.h"

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CShipAnim::CShipAnim() :
CBaseAnim(TSize(32,32))
	{
	}

////////////////////////////////////////////////////////////////////////

CShipAnim* CShipAnim::NewL(CFbsBitmap& aSourceImage)
	{
	CShipAnim* self = new(ELeave) CShipAnim();
	CleanupStack::PushL(self);
	self->ConstructL(aSourceImage);
	CleanupStack::Pop();
	return self;
	}

////////////////////////////////////////////////////////////////////////

CShipAnim::~CShipAnim()
	{
	}

////////////////////////////////////////////////////////////////////////

void CShipAnim::ConstructL(CFbsBitmap& aSourceImage)
    {
	//AllocRenderablesL(64);

	AllocRenderablesL(33);

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

	for (frame = 0 ; frame <= 32 ; frame++)
	{
		gc->Clear();
		BuildFrame(aSourceImage,frame,gc);
		AddRenderable(CBitmapAnimFrame::NewL(*image));
	}
	
	/*
	for (; frame < 64 ; frame++)
	{
		gc->Clear();
		BuildFlippedFrame(aSourceImage,64 - frame,*image);
		AddRenderable(CBitmapAnimFrame::NewL(*image));
	}
	*/

	// Destroy context, device and image;
	delete gc;
	CleanupStack::PopAndDestroy(2);
	}

////////////////////////////////////////////////////////////////////////

void CShipAnim::BuildFrame(CFbsBitmap& aSourceImage,TInt frame_no,CFbsBitGc* aGc)
{
	// Convert frame to x-coord:
	TInt frame_x = frame_no << 5;

	aGc->BitBlt(TPoint(0,0),&aSourceImage,TRect(TPoint(frame_x,0),TSize(32,32)));
}

////////////////////////////////////////////////////////////////////////

void CShipAnim::BuildFlippedFrame(CFbsBitmap& aSourceImage,TInt frame_no,CFbsBitmap& aDestImage)
{
	// Convert frame to x-coord:
	TInt frame_x = frame_no << 5;

	// Now use TBitmapUtils to flip source image
	// about the line x = y:
	TBitmapUtil srcUtil(&aSourceImage);
	TBitmapUtil destUtil(&aDestImage);

	srcUtil.Begin(TPoint(frame_x,0));
	destUtil.Begin(TPoint(0,0),srcUtil);

	for (TInt y = 0 ; y < 32 ; y++)
	{
		srcUtil.SetPos(TPoint(frame_x,y));
		destUtil.SetPos(TPoint(y,0));

		for (TInt x = 0 ; x < 32 ; x++)
		{
			destUtil.SetPixel(srcUtil);
			srcUtil.IncXPos();
			destUtil.IncYPos();
		}
	}

	srcUtil.End();
	destUtil.End();
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

