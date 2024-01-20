////////////////////////////////////////////////////////////////////////
//
// BitmapAnimFrame.cpp
//
// Copyright (c) 2003 Nokia Mobile Phones Ltd.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#include <e32math.h>
#include "RenderableFactory.h"
#include "BitmapAnimFrame.h"

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CBitmapAnimFrame* CBitmapAnimFrame::NewL(CFbsBitmap& aImage)
	{
	CBitmapAnimFrame* self = new(ELeave) CBitmapAnimFrame();
	CleanupStack::PushL(self);
	self->ConstructL(aImage);
	CleanupStack::Pop();
	return self;
	}

////////////////////////////////////////////////////////////////////////

CBitmapAnimFrame::~CBitmapAnimFrame()
	{
	delete iImage;
	iImage = NULL;

	delete iMask;
	iMask = NULL;
	}

////////////////////////////////////////////////////////////////////////

void CBitmapAnimFrame::ConstructL(CFbsBitmap& aSourceImage)
    {
	iRect.SetRect(TPoint(0,0),aSourceImage.SizeInPixels());

	// Create image:
	iImage = new (ELeave) CFbsBitmap();
	User::LeaveIfError(iImage->Create(aSourceImage.SizeInPixels(),EColor4K));

	iMask = new (ELeave) CFbsBitmap();
	User::LeaveIfError(iMask->Create(aSourceImage.SizeInPixels(),EGray2));

	//
	// We need to set one pixel of the mask to black and one to white,
	// so that we can read back their raw values with maskUtil below:
	//

	// Create device and context (only needed briefly):
	CFbsBitmapDevice* device = CFbsBitmapDevice::NewL(iMask);
	CleanupStack::PushL(device);
	CFbsBitGc* gc;
	User::LeaveIfError(device->CreateContext(gc));

	// Copy source image:
	gc->SetPenColor(TRgb(0,0,0));
	gc->Plot(TPoint(0,0));
	gc->SetPenColor(TRgb(255,255,255));
	gc->Plot(TPoint(1,0));

	// Destroy context and device;
	delete gc;
	CleanupStack::PopAndDestroy();


	// Now use TBitmapUtils to copy image and create mask:
	TBitmapUtil srcUtil(&aSourceImage);
	TBitmapUtil imageUtil(iImage);
	TBitmapUtil maskUtil(iMask);

	srcUtil.Begin(TPoint(0,0));
	imageUtil.Begin(TPoint(0,0),srcUtil);
	maskUtil.Begin(TPoint(0,0),srcUtil);

	// Fetch top left corner for use as transparent (masked out) value:
	TUint32 srcTransparentValue = srcUtil.GetPixel();

	// Fetch raw black and white values from mask to use in
	// constructing the mask as a whole:
	TUint32 maskTransparentValue = maskUtil.GetPixel();
	maskUtil.IncXPos();
	TUint32 maskOpaqueValue = maskUtil.GetPixel();

	TSize srcSize = aSourceImage.SizeInPixels();

	for (TInt y = 0 ; y < srcSize.iHeight ; y++)
	{
		srcUtil.SetPos(TPoint(0,y));
		imageUtil.SetPos(TPoint(0,y));
		maskUtil.SetPos(TPoint(0,y));

		for (TInt x = 0 ; x < srcSize.iWidth ; x++)
		{
			TUint32 fetchValue = srcUtil.GetPixel();

			imageUtil.SetPixel(fetchValue);

			if (fetchValue == srcTransparentValue)
			{
				maskUtil.SetPixel(maskTransparentValue);
			}
			else
			{
				maskUtil.SetPixel(maskOpaqueValue);
			}

			srcUtil.IncXPos();
			imageUtil.IncXPos();
			maskUtil.IncXPos();
		}
	}

	srcUtil.End();
	imageUtil.End();
	maskUtil.End();
	}

////////////////////////////////////////////////////////////////////////

void CBitmapAnimFrame::Render(const TPoint& aOrigin,const TRect& aScreenRect,CFbsBitGc* aCallerGc) const
	{
	TRect imageRect(aOrigin,aScreenRect.Size());
	imageRect.Intersection(iRect);
	if (imageRect.Size() == iRect.Size())
		{
		aCallerGc->BitBltMasked(aScreenRect.iTl - aOrigin,iImage,iRect,iMask,EFalse);
		}
	else
		{
		aCallerGc->BitBltMasked(aScreenRect.iTl + imageRect.iTl - aOrigin,iImage,imageRect,iMask,EFalse);
		}
	}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
