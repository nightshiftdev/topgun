////////////////////////////////////////////////////////////////////////
//
// Enemy.cpp
//
// Copyright (c) 2004 Il Padrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////
#include "Enemy.h"

//System includes
#include <e32base.h>
#include <aknutils.h>
//User includes


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CEnemy* CEnemy::NewLC(const TDesC& aEnemyFileName, 
					  const TPoint &aWorldPos, 
					  TPoint aSpeed, 
					  TInt aEnergy, 
					  RWindow& aWindow)
	{
	CEnemy* self = new (ELeave) CEnemy();
	CleanupStack::PushL(self);
	self->ConstructL(aEnemyFileName, 
					 aWorldPos,
					 aSpeed,
					 aEnergy,
					 aWindow);
	return self;
	}

////////////////////////////////////////////////////////////////////////

CEnemy* CEnemy::NewL(const TDesC& aEnemyFileName, 
					 const TPoint &aWorldPos, 
					 TPoint aSpeed, 
					 TInt aEnergy, 
					 RWindow& aWindow)
	{
	CEnemy* self = CEnemy::NewLC(aEnemyFileName, 
								 aWorldPos,
								 aSpeed,
								 aEnergy,
								 aWindow);
	CleanupStack::Pop();
	return self;
	}

////////////////////////////////////////////////////////////////////////

CEnemy::CEnemy()
	{
	}

////////////////////////////////////////////////////////////////////////

CEnemy::~CEnemy()
	{
	}

////////////////////////////////////////////////////////////////////////
	
void CEnemy::ConstructL(const TDesC& aEnemyFileName, 
						const TPoint &aWorldPos, 
						TPoint aSpeed, 
						TInt aEnergy, 
						RWindow& aWindow)
	{
	//set Enemy's stating speed
	iEnemySpeed = aSpeed;

	//set Enemy's stating energy
	iEnemyEnergy = aEnergy;
	
	//save last Enemy position
	iEnemyWorldPos = aWorldPos;

	// Every second image is a mask.
	TFileName mbmFileName(aEnemyFileName);
	CompleteWithAppPath(mbmFileName);
	
	// Construct my sprite
	iEnemy = RWsSprite( CEikonEnv::Static()->WsSession() );
	//User::LeaveIfError( iEnemy.Construct( aWindow ,aWorldPos, 0 ) );
	User::LeaveIfError( iEnemy.Construct( aWindow ,aWorldPos, ESpriteNoChildClip | ESpriteFlash | ESpriteNoShadows ) );
	
	// Load bitmaps (image+mask) and set properties for each member
	// 1 member in total (1 image + 1 mask)
	for ( TInt i = 0; i < 1; i ++ )
		{
		// Load image
		iEnemyMembers[i].iBitmap = new ( ELeave ) CFbsBitmap();
		User::LeaveIfError( iEnemyMembers[i].iBitmap->Load(mbmFileName, i * 2, EFalse ) );

		// Load mask for the image
		iEnemyMembers[i].iMaskBitmap = new ( ELeave ) CFbsBitmap();
		User::LeaveIfError( iEnemyMembers[i].iMaskBitmap->Load(mbmFileName, i * 2 + 1, EFalse ) );
		
		// Set properties for the member
		//iEnemyMembers[i].iInvertMask = EFalse;
		iEnemyMembers[i].iInvertMask = EFalse;
		iEnemyMembers[i].iOffset = TPoint(0,0);
		
		// Change image every 1/10 seconds
		//iEnemyMembers[i].iInterval =TTimeIntervalMicroSeconds32(100000);
		// Change image every 1/64 seconds
		iEnemyMembers[i].iInterval =TTimeIntervalMicroSeconds32(15625);
		
		// Append created member to sprite
		User::LeaveIfError( iEnemy.AppendMember(iEnemyMembers[i] ) );
		}
	
	// All members added. Activate the sprite; the sprite will be
	// drawn/animated continuously to view until sprite is destroyed
	// with iEnemy.Close()
	User::LeaveIfError(iEnemy.Activate());
	}

////////////////////////////////////////////////////////////////////////

void CEnemy::DestroyEnemy()
	{
	iEnemy.Close();
	}

////////////////////////////////////////////////////////////////////////

void CEnemy::MoveEnemyTo(const TPoint &aPos)
	{
	iEnemyPos = aPos;
	iEnemy.SetPosition(aPos);
	}

////////////////////////////////////////////////////////////////////////

TPoint CEnemy::GetEnemyPos()
	{
	return iEnemyWorldPos;
	}

////////////////////////////////////////////////////////////////////////

void CEnemy::UpdateEnemy(TInt aBitmapIndex)
	{	
	iEnemy.UpdateMember(aBitmapIndex);	
	}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////