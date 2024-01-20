////////////////////////////////////////////////////////////////////////
//
// UtilsAnim.cpp
//
// Copyright (c) 2004 IlPadrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#include <e32math.h>
#include "RenderableFactory.h"
#include "BitmapAnimFrame.h"
#include "UtilsAnim.h"

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CUtilsAnim::CUtilsAnim() :
CBaseAnim(TSize(0,0))
	{
	}

////////////////////////////////////////////////////////////////////////

CUtilsAnim* CUtilsAnim::NewL(CFbsBitmap& aSourceImage,
							 const TPoint &aScreenPos,
							 const TSize &aSize)
	{
	CUtilsAnim* self = new(ELeave) CUtilsAnim();
	CleanupStack::PushL(self);
	self->ConstructL(aSourceImage,
					 aScreenPos,
					 aSize);
	CleanupStack::Pop();
	return self;
	}

////////////////////////////////////////////////////////////////////////

CUtilsAnim::~CUtilsAnim()
	{
	}

////////////////////////////////////////////////////////////////////////

void CUtilsAnim::ConstructL(CFbsBitmap& aSourceImage,
							const TPoint &aScreenPos,
							const TSize &aSize)
    {
	//Initialize Utils's parameters
	iUtilsScreenPos = aScreenPos;
	iSize = aSize;
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

	// Create Utils frames:
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

void CUtilsAnim::BuildFrame(CFbsBitmap& aSourceImage,TInt frame_no,CFbsBitGc* aGc)
{
	// Convert frame to x-coord:
	TInt frame_x = frame_no << 5;

	aGc->BitBlt(TPoint(0,0),&aSourceImage,TRect(TPoint(frame_x,0),iSize));
}

////////////////////////////////////////////////////////////////////////

void CUtilsAnim::SetUtilsScreenPos(const TPoint &aUtilsScreenPos)
	{
	iUtilsScreenPos = aUtilsScreenPos;
	}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

