////////////////////////////////////////////////////////////////////////
//
// BitmapTile.cpp
//
// Copyright (c) 2003 Nokia Mobile Phones Ltd.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#include <e32math.h>
#include "RenderableFactory.h"
#include "BitmapTile.h"

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CBitmapTile* CBitmapTile::NewL(CFbsBitmap& aImage)
	{
	CBitmapTile* self = new(ELeave) CBitmapTile();
	CleanupStack::PushL(self);
	self->ConstructL(aImage);
	CleanupStack::Pop();
	return self;
	}

////////////////////////////////////////////////////////////////////////

CBitmapTile::~CBitmapTile()
	{
	delete iImage;
	iImage = NULL;
	}

////////////////////////////////////////////////////////////////////////

void CBitmapTile::ConstructL(CFbsBitmap& aImage)
    {
	// Create image:
	iImage = new (ELeave) CFbsBitmap();
	User::LeaveIfError( iImage->Create(aImage.SizeInPixels(), EColor4K ) );

	// Create device and context (only needed briefly):
	CFbsBitmapDevice* device = CFbsBitmapDevice::NewL( iImage );
	CleanupStack::PushL(device);
	CFbsBitGc* gc;
	User::LeaveIfError( device->CreateContext( gc ) );

	// Copy source image:
	gc->BitBlt(TPoint(0,0),&aImage);
	iSize = aImage.SizeInPixels();
	iRect.SetRect(TPoint(0,0),iSize);

	// Destroy context and device;
	delete gc;
	CleanupStack::PopAndDestroy();
	}

////////////////////////////////////////////////////////////////////////

void CBitmapTile::Render(const TPoint& aOrigin,const TRect& aScreenRect,CFbsBitGc* aCallerGc) const
	{
	aCallerGc->BitBlt(aScreenRect.iTl - aOrigin,iImage);
	}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
