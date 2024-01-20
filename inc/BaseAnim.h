////////////////////////////////////////////////////////////////////////
//
// BaseAnim.h
//
// Copyright (c) 2003 Nokia Mobile Phones Ltd.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef __BASEANIM_H
#define __BASEANIM_H

#include "RenderableFactory.h"

////////////////////////////////////////////////////////////////////////

class CBaseAnim : public CRenderableFactory
	{
public:
	~CBaseAnim(){};

	void RenderFrameCentered(TInt aFrameNum,const TPoint& aCenter,const TRect& aScreenRect,CFbsBitGc* aCallerGc);		

protected:
	CBaseAnim(TSize aSize);

protected:
	TSize iSize;
	};

////////////////////////////////////////////////////////////////////////

#endif

