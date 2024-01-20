////////////////////////////////////////////////////////////////////////
//
// BitmapTile.h
//
// Copyright (c) 2003 Nokia Mobile Phones Ltd.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef __BITMAPTILE_H
#define __BITMAPTILE_H

#include "RenderableFactory.h"

////////////////////////////////////////////////////////////////////////

class CBitmapTile : public CRenderable
	{
public:
	static CBitmapTile* NewL(CFbsBitmap& aImage);
	~CBitmapTile();

public:   
	void Render(const TPoint& aOrigin,const TRect& aScreenRect,CFbsBitGc* aCallerGc) const;

private:
    void ConstructL(CFbsBitmap& aImage);

private:
	CFbsBitmap* iImage;
	TRect iRect;
	TSize iSize;
	};

////////////////////////////////////////////////////////////////////////

#endif

