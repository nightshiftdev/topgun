////////////////////////////////////////////////////////////////////////
//
// ExplosionAnim.h
//
// Copyright (c) 2004 IlPadrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef _EXPLOSIONANIM_H_
#define _EXPLOSIONANIM_H_

#include "BaseAnim.h"

////////////////////////////////////////////////////////////////////////

class CExplosionAnim : public CBaseAnim
	{
public:
	static CExplosionAnim* NewL(CFbsBitmap& aSourceImageconst,const TPoint &aScreenPos);
	CExplosionAnim();
	~CExplosionAnim();
    void ConstructL(CFbsBitmap& aSourceImage,const TPoint &aScreenPos);
	void SetExplosionScreenPos(const TPoint &aExplosionScreenPos);
	TPoint GetExplosionScreenPos();

private:
	void BuildFrame(CFbsBitmap& aSourceImage,TInt frame_no,CFbsBitGc* aGc);

	//Explosion's parameters
	TPoint iExplosionScreenPos;
	TPoint iExplosionScreenPosOrg;
	};

////////////////////////////////////////////////////////////////////////

#endif

