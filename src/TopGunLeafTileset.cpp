////////////////////////////////////////////////////////////////////////
//
// TopGunLeafTileset.cpp
//
// Copyright (c) 2003 ilPadrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#include <e32math.h>
#include "RenderableFactory.h"
#include "BitmapTile.h"
#include "TopGunLeafTileset.h"

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CTopGunLeafTileset* CTopGunLeafTileset::NewL(CFbsBitmap& aSourceImage)
	{
	CTopGunLeafTileset* self = new(ELeave) CTopGunLeafTileset();
	CleanupStack::PushL(self);
	self->ConstructL(aSourceImage);
	CleanupStack::Pop();
	return self;
	}

////////////////////////////////////////////////////////////////////////

CTopGunLeafTileset::CTopGunLeafTileset()
	{
	}

////////////////////////////////////////////////////////////////////////

CTopGunLeafTileset::~CTopGunLeafTileset()
	{
	}

////////////////////////////////////////////////////////////////////////

void CTopGunLeafTileset::ConstructL(CFbsBitmap& aSourceImage)
    {
	//AllocRenderablesL(8);
	AllocRenderablesL(80);

	iSize.SetSize(32,32);
	iTileLogWidth = 5;
	iTileLogHeight = 5;

	// Create image, device and context (only needed briefly):
	CFbsBitmap* image = new (ELeave) CFbsBitmap();
	CleanupStack::PushL(image);
	User::LeaveIfError( image->Create(iSize, EColor4K ) );
	CFbsBitmapDevice* device = CFbsBitmapDevice::NewL(image);
	CleanupStack::PushL(device);
	CFbsBitGc* gc;
	User::LeaveIfError(device->CreateContext(gc));
	gc->SetBrushColor(TRgb(0,0,0));

	// Create ship frames ???
	//create map frames
	TInt tile;
	//for (tile = 0 ; tile < 8 ; tile++)
	for (tile = 0 ; tile < 80 ; tile++)
	{
		gc->Clear();
		BuildTile(aSourceImage,tile,gc);
		AddRenderable(CBitmapTile::NewL(*image));
	}

	// Destroy context, device and image;
	delete gc;
	CleanupStack::PopAndDestroy(2);
	}

////////////////////////////////////////////////////////////////////////

void CTopGunLeafTileset::BuildTile(CFbsBitmap& aSourceImage,TInt frame_no,CFbsBitGc* aGc)
{
	// Convert frame to x-coord:
	TInt frame_x = frame_no << 5;

	aGc->BitBlt(TPoint(0,0),&aSourceImage,TRect(TPoint(frame_x,0),iSize));
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

