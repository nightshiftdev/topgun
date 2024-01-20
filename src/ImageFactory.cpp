////////////////////////////////////////////////////////////////////////
//
// LevelLoader.h
//
// Copyright (c) 2004 Il Padrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

//system
#include <fbs.h>
#include <eikenv.h>
#include <topgun.mbg>
//user
#include "ImageFactory.h"
#include "LevelLoader.h"

CImageFactory* CImageFactory::NewL(CEikonEnv& aEikonEnv, const TDesC& aImageFile, CLevelLoader& aLevelLoader)
	{
	CImageFactory* self = new (ELeave) CImageFactory(aEikonEnv, aLevelLoader);
	CleanupStack::PushL(self);
	self->ConstructL(aImageFile);
	CleanupStack::Pop();
	return self;
	}

CImageFactory* CImageFactory::NewLC(CEikonEnv& aEikonEnv, const TDesC& aImageFile, CLevelLoader& aLevelLoader)
	{
	CImageFactory* self = new (ELeave) CImageFactory(aEikonEnv, aLevelLoader);
	CleanupStack::PushL(self);
	self->ConstructL(aImageFile);
	return self;
	}

void CImageFactory::ConstructL(const TDesC& aImageFileName)
	{
	iMbmFile = aImageFileName.AllocL();
	LoadImagesL();
	}

CImageFactory::CImageFactory(CEikonEnv& aEikonEnv, CLevelLoader& aLevelLoader) :
	iEikEnv(aEikonEnv),
	iMbmFile(0),
	iLevelLoader(aLevelLoader)
	{
	}

CImageFactory::~CImageFactory()
	{
	delete iMbmFile;
	delete iEnemyBulletAnim;
	delete iBulletAnim;
	delete iShipAnim;
	delete iEnemyAnim2;
	delete iEnemyAnim3;
	delete iEnemyAnim4;
	delete iEnemyAnim6;
	delete iEnemyAnim8;
	delete iFireAnim;
	delete iExplosionAnim;
	delete iTileImage;
	delete iTileMask;

	delete iGameOver;
	delete iPause;
	delete iScoreBar;
	}

void CImageFactory::LoadImagesL()
	{
	//Utils
	iGameOver = iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunGameover );
	iPause	  = iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunPause ); 
	iScoreBar = iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunScore ); 


	iShipAnim	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunShipanim );
	iExplosionAnim	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunExplosionanim ); //4
	//Bullets
	iBulletAnim	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunBulletanim ); 
	iEnemyBulletAnim = iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunEnemybulletanim ); //6
	
	TUint factor = LEVEL_FACTOR*(iLevelLoader.LevelNumber());
	//Level Data
	iTileImage	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunEnemybulletanim+(factor+1));
	iTileMask	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunEnemybulletanim+(factor+2));
	iEnemyAnim2	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunEnemybulletanim+(factor+3));
	iEnemyAnim3	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunEnemybulletanim+(factor+4));
	iEnemyAnim4	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunEnemybulletanim+(factor+5));
	iEnemyAnim6	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunEnemybulletanim+(factor+6));
	iEnemyAnim8	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunEnemybulletanim+(factor+7));

	/*
	//Level Data
	//Level 1 Bitmaps
	
	//iTileImage	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunTileimage );
	//iTileMask	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunTilemask );
	iTileImage	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunEnemybulletanim+1 );
	iTileMask	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunEnemybulletanim+2 );
	//Level 1 Enemies
	
	//iEnemyAnim2	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunEnemyanim2 );
	//iEnemyAnim3	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunEnemyanim3 );
	//iEnemyAnim4	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunEnemyanim4 );
	//iEnemyAnim6	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunEnemyanim6 );
	//iEnemyAnim8	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunEnemyanim8 );
	iEnemyAnim2	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunEnemybulletanim+3 );
	iEnemyAnim3	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunEnemybulletanim+4 );
	iEnemyAnim4	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunEnemybulletanim+5 );
	iEnemyAnim6	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunEnemybulletanim+6 );
	iEnemyAnim8	= iEikEnv.CreateBitmapL(*iMbmFile, EMbmTopgunEnemybulletanim+7 );
	*/
	}

CFbsBitmap& CImageFactory::BulletAnim() const
	{
	return *iBulletAnim;
	}

CFbsBitmap& CImageFactory::EnemyBulletAnim() const
	{
	return *iEnemyBulletAnim;
	}

CFbsBitmap& CImageFactory::ShipAnim() const
	{
	return *iShipAnim;
	}

CFbsBitmap& CImageFactory::EnemyAnim2() const
	{
	return *iEnemyAnim2;
	}

CFbsBitmap& CImageFactory::EnemyAnim3() const
	{
	return *iEnemyAnim3;
	}

CFbsBitmap& CImageFactory::EnemyAnim4() const
	{
	return *iEnemyAnim4;
	}

CFbsBitmap& CImageFactory::EnemyAnim6() const
	{
	return *iEnemyAnim6;
	}

CFbsBitmap& CImageFactory::EnemyAnim8() const
	{
	return *iEnemyAnim8;
	}

CFbsBitmap& CImageFactory::FireAnim() const
	{
	return *iFireAnim;
	}

CFbsBitmap& CImageFactory::ExplosionAnim() const
	{
	return *iExplosionAnim;
	}

CFbsBitmap& CImageFactory::TileImage() const
	{
	return *iTileImage;
	}

CFbsBitmap& CImageFactory::GameOver() const
	{
	return *iGameOver;
	}

CFbsBitmap& CImageFactory::Pause() const
	{
	return *iPause;
	}

CFbsBitmap& CImageFactory::ScoreBar() const
	{
	return *iScoreBar;
	}

CFbsBitmap& CImageFactory::TileMask() const
	{
	return *iTileMask;
	}



