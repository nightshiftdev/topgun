////////////////////////////////////////////////////////////////////////
//
// BitmapAnimFrame.h
//
// Copyright (c) 2003 Nokia Mobile Phones Ltd.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef __BITMAPANIMFRAME_H
#define __BITMAPANIMFRAME_H

#include "RenderableFactory.h"

////////////////////////////////////////////////////////////////////////

class CBitmapAnimFrame : public CRenderable
	{
public:
	static CBitmapAnimFrame* NewL(CFbsBitmap& aImage);
	~CBitmapAnimFrame();

public:   
	void Render(const TPoint& aOrigin,const TRect& aScreenRect,CFbsBitGc* aCallerGc) const;

private:
    void ConstructL(CFbsBitmap& aImage);

private:
	CFbsBitmap* iImage;
	CFbsBitmap* iMask;
	TRect iRect;
	};

////////////////////////////////////////////////////////////////////////

#endif

