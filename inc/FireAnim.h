////////////////////////////////////////////////////////////////////////
//
// FireAnim.h
//
// Copyright (c) 2003 Nokia Mobile Phones Ltd.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef __FIREANIM_H
#define __FIREANIM_H

#include "BaseAnim.h"

////////////////////////////////////////////////////////////////////////

class CFireAnim : public CBaseAnim
	{
public:
	static CFireAnim* NewL(CFbsBitmap& aSourceImage);
	CFireAnim();
	~CFireAnim();
    void ConstructL(CFbsBitmap& aSourceImage);

private:
	void BuildFrame(CFbsBitmap& aSourceImage,TInt frame_no,CFbsBitGc* aGc);
	};

////////////////////////////////////////////////////////////////////////

#endif

