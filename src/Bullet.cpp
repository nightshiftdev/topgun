////////////////////////////////////////////////////////////////////////
//
// Bullet.cpp
//
// Copyright (c) 2004 Il Padrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////
#include "Bullet.h"

//System includes
#include <e32base.h>
#include <aknutils.h>
//User includes



////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CBullet* CBullet::NewLC(const TPoint &aPos, RWindow& aWindow, const TDesC& aImageFile)
	{
	CBullet* self = new (ELeave) CBullet();
	CleanupStack::PushL(self);
	self->ConstructL(aPos, aWindow, aImageFile);
	return self;
	}

////////////////////////////////////////////////////////////////////////

CBullet* CBullet::NewL(const TPoint &aPos, RWindow& aWindow, const TDesC& aImageFile)
	{
	CBullet* self = CBullet::NewLC(aPos, aWindow, aImageFile);
	CleanupStack::Pop();
	return self;
	}

////////////////////////////////////////////////////////////////////////

CBullet::CBullet()
	{
	}

////////////////////////////////////////////////////////////////////////

CBullet::~CBullet()
	{
	}

////////////////////////////////////////////////////////////////////////
	
void CBullet::ConstructL(const TPoint &aPos, RWindow& aWindow, const TDesC& aImageFile)
	{
	//save last Bullet position
	iBulletPos = aPos;

	// Every second image is a mask.
	//_LIT(KBulletMBM, "Bullet.mbm");
	//TFileName mbmFileName(KBulletMBM);
	TFileName mbmFileName(aImageFile);
	CompleteWithAppPath(mbmFileName);
	
	// Construct my sprite
	iBullet = RWsSprite( CEikonEnv::Static()->WsSession() );
	User::LeaveIfError( iBullet.Construct( aWindow ,aPos, 0 ) );
	
	// Load bitmaps (image+mask) and set properties for each member
	// 1 member in total (1 image + 1 mask)
	for ( TInt i = 0; i < 1; i ++ )
		{
		// Load image
		iBulletMembers[i].iBitmap = new ( ELeave ) CFbsBitmap();
		User::LeaveIfError( iBulletMembers[i].iBitmap->Load(
		mbmFileName, i * 2, EFalse ) );

		// Load mask for the image
		iBulletMembers[i].iMaskBitmap = new ( ELeave ) CFbsBitmap();
		User::LeaveIfError( iBulletMembers[i].iMaskBitmap->Load(
		mbmFileName, i * 2 + 1, EFalse ) );
		
		// Set properties for the member
		iBulletMembers[i].iInvertMask = EFalse;
		iBulletMembers[i].iOffset = TPoint(0,0);
		
		// Change image every 1/10 seconds
		//iBulletMembers[i].iInterval =TTimeIntervalMicroSeconds32(100000);
		iBulletMembers[i].iInterval =TTimeIntervalMicroSeconds32(0);
		
		// Append created member to sprite
		User::LeaveIfError( iBullet.AppendMember(iBulletMembers[i] ) );
		}
	
	// All members added. Activate the sprite; the sprite will be
	// drawn/animated continuously to view until sprite is destroyed
	// with iBullet.Close()
	User::LeaveIfError(iBullet.Activate());
	}

////////////////////////////////////////////////////////////////////////

void CBullet::DestroyBullet()
	{
	iBullet.Close();
	}

////////////////////////////////////////////////////////////////////////

void CBullet::MoveBulletTo(const TPoint &aPos)
	{
	iBulletPos = aPos;
	iBullet.SetPosition(aPos);
	}

////////////////////////////////////////////////////////////////////////

TPoint CBullet::GetBulletPos()
	{
	return iBulletPos;
	}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////