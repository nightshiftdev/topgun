////////////////////////////////////////////////////////////////////////
//
// BulletAnim.h
//
// Copyright (c) 2004 IlPadrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef _BULLETANIM_H_
#define _BULLETANIM_H_

#include "BaseAnim.h"
#include "EnemyType.h"

////////////////////////////////////////////////////////////////////////

class CBulletAnim : public CBaseAnim
	{
public:
	static CBulletAnim* NewL(CFbsBitmap& aSourceImageconst, 
							const TPoint &aScreenPos,
							TInt aSpeed,
							TSize aSize,
							const TBulletOrientation &aBulletOrientation);
	CBulletAnim();
	~CBulletAnim();
    void ConstructL(CFbsBitmap& aSourceImage,
					const TPoint &aScreenPos,
					TInt aSpeed,
					TSize aSize,
					const TBulletOrientation &aBulletOrientation);
	void SetBulletScreenPos(const TPoint &aBulletScreenPos);
	TPoint GetBulletScreenPos();
	TPoint GetBulletScreenPosOrg();
	void SetBulletSpeed(TInt aSpeed);
	TInt GetBulletSpeed();
	TSize GetBulletSize();
	TBulletOrientation BulletOrientation();

private:
	void BuildFrame(CFbsBitmap& aSourceImage,TInt frame_no,CFbsBitGc* aGc);
	
	//Bullet's parameters
	TPoint iBulletScreenPos;
	TPoint iBulletScreenPosOrg;
	TInt iBulletSpeed;
	TBulletOrientation iBulletOrientation;
	};

////////////////////////////////////////////////////////////////////////

#endif

