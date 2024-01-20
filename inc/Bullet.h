////////////////////////////////////////////////////////////////////////
//
// Bullet.h
//
// Copyright (c) 2004 Il Padrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef __BULLET_H__
#define __BULLET_H__

#include <w32std.h>
#include <e32base.h>

class CBullet : public CBase
    {
    public:

        static CBullet* NewL(const TPoint &aPos, RWindow& aWindow, const TDesC& aImageFile);
        static CBullet* NewLC(const TPoint &aPos, RWindow& aWindow, const TDesC& aImageFile);
        virtual ~CBullet();
		void MoveBulletTo(const TPoint& aPos);
		void DestroyBullet();
		TPoint GetBulletPos();

	private:
		
		CBullet();
        void ConstructL(const TPoint &aPos, RWindow& aWindow, const TDesC& aImageFile);

    private:
	
		RWsSprite iBullet;
		TSpriteMember iBulletMembers[1];
		TPoint iBulletPos;

    };
#endif