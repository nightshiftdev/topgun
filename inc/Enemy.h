////////////////////////////////////////////////////////////////////////
//
// Enemy.h
//
// Copyright (c) 2004 Il Padrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <w32std.h>
#include <e32base.h>

class CEnemy : public CBase
    {
    public:

        static CEnemy* NewL(const TDesC& aEnemyFileName, const TPoint &aWorldPos, TPoint aSpeed, TInt aEnergy, RWindow& aWindow);
        static CEnemy* NewLC(const TDesC& aEnemyFileName, const TPoint &aWorldPos, TPoint aSpeed, TInt aEnergy, RWindow& aWindow);
        virtual ~CEnemy();
		void MoveEnemyTo(const TPoint& aPos);
		void DestroyEnemy();
		TPoint GetEnemyPos();
		void UpdateEnemy(TInt aBitmapIndex);

	private:
		
		CEnemy();
        void ConstructL(const TDesC& aEnemyFileName ,const TPoint &aWorldPos, TPoint aSpeed, TInt aEnergy, RWindow& aWindow);

    private:
	
		RWsSprite iEnemy;
		TSpriteMember iEnemyMembers[1];
		TPoint iEnemyWorldPos;
		TPoint iEnemyPos;
		TPoint iEnemySpeed;
		TInt iEnemyEnergy;
    };
#endif