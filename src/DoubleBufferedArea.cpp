////////////////////////////////////////////////////////////////////////
//
// DoubleBufferedArea.cpp
//
// Copyright (c) 2003 Nokia Mobile Phones Ltd.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#include "DoubleBufferedArea.h"


// System includes
#include <bitdev.h>     // CFbsBitmapDevice
#include <bitstd.h>     // CFbsBitGc
#include <fbs.h>        // CFbsBitmap


CDoubleBufferedArea* CDoubleBufferedArea::NewL(TSize aSize, TDisplayMode aDisplayMode)
	{
	CDoubleBufferedArea* self = new (ELeave) CDoubleBufferedArea();
	CleanupStack::PushL(self);
	self->ConstructL(aSize, aDisplayMode);
	CleanupStack::Pop();
	return self;
	}

CDoubleBufferedArea* CDoubleBufferedArea::NewLC(TSize aSize, TDisplayMode aDisplayMode)
	{
	CDoubleBufferedArea* self = new (ELeave) CDoubleBufferedArea();
	CleanupStack::PushL(self);
	self->ConstructL(aSize, aDisplayMode);
	return self;
	}

CDoubleBufferedArea::CDoubleBufferedArea()
	{
	}


CDoubleBufferedArea::~CDoubleBufferedArea()
	{
	delete iAreaBitmap;
    delete iAreaBitmapDevice;
    delete iAreaBitmapContext;
	}

void CDoubleBufferedArea::ConstructL(TSize aSize, TDisplayMode aDisplayMode)
	{
	//constructing offscreen play area:
	iAreaBitmap = new (ELeave) CFbsBitmap();
    iAreaBitmap->Create(aSize, aDisplayMode);
    iAreaBitmapDevice = CFbsBitmapDevice::NewL(iAreaBitmap);
    iAreaBitmapDevice->CreateContext(iAreaBitmapContext);
	}

CFbsBitGc& CDoubleBufferedArea::GetDoubleBufferedAreaContext() const
    {
     return *iAreaBitmapContext;
    }

const CFbsBitmap& CDoubleBufferedArea::GetDoubleBufferedAreaBitmap() const
	{
	return *iAreaBitmap;
	}

void CDoubleBufferedArea::ClearBufferedArea()
	{
	iAreaBitmapContext->Clear();
	}