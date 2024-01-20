////////////////////////////////////////////////////////////////////////
//
// BaseAnim.cpp
//
// Copyright (c) 2003 Nokia Mobile Phones Ltd.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#include <e32math.h>
#include "RenderableFactory.h"
#include "BaseAnim.h"

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CBaseAnim::CBaseAnim(TSize aSize) :
iSize(aSize)
{
}

////////////////////////////////////////////////////////////////////////

void CBaseAnim::RenderFrameCentered(TInt aFrameNum,const TPoint& aCenter,const TRect& aScreenRect,CFbsBitGc* aCallerGc)
{
	TPoint topLeftPos(aCenter.iX - (iSize.iWidth >> 1), aCenter.iY - (iSize.iHeight >> 1));
	Renderable(aFrameNum)->Render(-topLeftPos,aScreenRect,aCallerGc);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////