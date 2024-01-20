////////////////////////////////////////////////////////////////////////
//
// EnemyAnim.h
//
// Copyright (c) 2004 IlPadrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef _ENEMYANIM_H_
#define _ENEMYANIM_H_

#include "BaseAnim.h"
#include "EnemyType.h"

////////////////////////////////////////////////////////////////////////

class CEnemyAnim : public CBaseAnim
	{
public:
	static CEnemyAnim* NewL(CFbsBitmap& aSourceImageconst, 
							const TPoint &aScreenPos,
							TInt aSpeed, 
							TInt aEnergy,
							TInt aOrgEnergy,
							TInt aZigZac,
							const TSize &aSize,
							const TEnemy &aType);
	CEnemyAnim();
	~CEnemyAnim();
    void ConstructL(CFbsBitmap& aSourceImage,
					const TPoint &aScreenPos,
					TInt aSpeed, 
					TInt aEnergy,
					TInt aOrgEnergy,
					TInt aZigZac,
					const TSize &aSize,
					const TEnemy &aType);
	void SetEnemyScreenPos(const TPoint &aEnemyScreenPos);
	TPoint GetEnemyScreenPos();
	TPoint GetEnemyScreenPosOrg();
	TBool EnemyHit();
	void SetEnemySpeed(TInt aSpeed);
	TInt GetEnemySpeed();
	TEnemy EnemyType();
	void ZigZac(TInt aMove);
	TSize GetEnemySize();

private:

	void BuildFrame(CFbsBitmap& aSourceImage,TInt frame_no,CFbsBitGc* aGc);

	//Enemy's parameters
	TPoint iEnemyScreenPos;
	TPoint iEnemyScreenPosOrg;
	TInt iEnemySpeed;
	TInt iEnemyEnergy;
	TInt iEnemyOrgEnergy;
	TEnemy iEnemyType;
	TInt iZigZac;
	TInt iOrgZigZac;
	TBool iBound;
	};

////////////////////////////////////////////////////////////////////////

#endif

