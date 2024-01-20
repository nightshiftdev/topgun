////////////////////////////////////////////////////////////////////////
//
// ShipAnim.h
//
// Copyright (c) 2003 Nokia Mobile Phones Ltd.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef __SHIPANIM_H
#define __SHIPANIM_H

#include "BaseAnim.h"

////////////////////////////////////////////////////////////////////////

class CShipAnim : public CBaseAnim
	{
public:
	static CShipAnim* NewL(CFbsBitmap& aSourceImage);
	CShipAnim();
	~CShipAnim();
    void ConstructL(CFbsBitmap& aSourceImage);

private:
	void BuildFrame(CFbsBitmap& aSourceImage,TInt frame_no,CFbsBitGc* aGc);
	void BuildFlippedFrame(CFbsBitmap& aSourceImage,TInt frame_no,CFbsBitmap& aDestImage);
	};

////////////////////////////////////////////////////////////////////////

#endif

