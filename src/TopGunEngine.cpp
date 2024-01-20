////////////////////////////////////////////////////////////////////////
//
// TopGunEngine.cpp
//
// Copyright (c) 2003 IlPadrino group.  All rights reserved.
//
//
////////////////////////////////////////////////////////////////////////

#include "TopGunEngine.h"

//System includes
#include <aknutils.h>
#include <e32math.h>
#include <e32std.h> 
#include <gdi.h>


//User includes
#include "KeyHandler.h"
#include "ImageFactory.h"
#include "DoublebufferedArea.h"
#include "RenderableFactory.h"
#include "BaseAnim.h"         
#include "BitmapAnimFrame.h"  
#include "BitmapTile.h"       
#include "FireAnim.h"         
#include "ParticleSystem.h"   
#include "TopGunLeafTileset.h" 
#include "TopGunNodeTileset.h" 
#include "ShipAnim.h"
#include "EnemyAnim.h"     
#include "ExplosionAnim.h"      
#include "Tilemap.h"          
#include "Tileset.h"          
#include "GameTimer.h"
#include "BulletAnim.h"
#include "Enemy.h"
#include "CSndMixer.h"
#include "CWavLoader.h"
#include "EnemyType.h"
#include "Position.h"
#include "UtilsAnim.h"
#include "TOPGUNContainer2.h"

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CTopGunEngine* CTopGunEngine::NewLC(CTOPGUNContainer2& aContainer,
								  RWsSession& aClient, 
								  CWsScreenDevice& aScreenDevice, 
								  RWindow& aWindow,TKeyHandler& aKeyHandler, 
								  CLevelLoader& aLevelLoader,
								  CImageFactory& aImageFactory, 
								  const TSize& aSize,
								  CSndMixer* aSndMixer,
							      TSample& aMusic,
							      TSample& aShotSound,
							      TSample& aExplosionSound,
							      TInt aAudioVol,
							      TInt aSfxVol)
	{
	CTopGunEngine* self = new (ELeave) CTopGunEngine(aContainer, aClient, aScreenDevice, aWindow, aKeyHandler, aLevelLoader, aImageFactory, aSize);
	CleanupStack::PushL(self);
	self->ConstructL(aSndMixer,
					 aMusic,
					 aShotSound,
					 aExplosionSound,
					 aAudioVol,
					 aSfxVol);
	return self;
	}

////////////////////////////////////////////////////////////////////////

CTopGunEngine* CTopGunEngine::NewL(CTOPGUNContainer2& aContainer,
								 RWsSession& aClient, 
								 CWsScreenDevice& aScreenDevice, 
								 RWindow& aWindow,
								 TKeyHandler& aKeyHandler,
								 CLevelLoader& aLevelLoader,
								 CImageFactory& aImageFactory, 
								 const TSize& aSize,
								 CSndMixer*	aSndMixer,
							     TSample& aMusic,
							     TSample& aShotSound,
							     TSample& aExplosionSound,
							     TInt aAudioVol,
							     TInt aSfxVol)
	{
	CTopGunEngine* self = CTopGunEngine::NewLC(aContainer,
											 aClient, 
											 aScreenDevice, 
											 aWindow, 
											 aKeyHandler,
											 aLevelLoader,
											 aImageFactory, 
											 aSize,
											 aSndMixer,
										     aMusic,
										     aShotSound,
										     aExplosionSound,
										     aAudioVol,
										     aSfxVol);
	CleanupStack::Pop();
	return self;
	}

////////////////////////////////////////////////////////////////////////

CTopGunEngine::CTopGunEngine(CTOPGUNContainer2& aContainer,RWsSession& aClient, CWsScreenDevice& aScreenDevice, RWindow& aWindow,TKeyHandler& aKeyHandler, CLevelLoader& aLevelLoader,CImageFactory& aImageFactory, const TSize& aSize):
	iContainer(aContainer),
	iClient(aClient),
	iScreenDevice(aScreenDevice),
	iWindow(aWindow),
	iKeyHandler(aKeyHandler),
	iLevelLoader(aLevelLoader),
	iImageFactory(aImageFactory),
	iSize(aSize),
	iDropRenderFrames(ETrue), // Set to "EFalse" to demonstrate ViewSrv 11 problem
	iFrameCounter(0),
	iDestroyedShips(0),
	iAllShips(0),
	iPercentageOfDestroyedShips(0)
	{
	}

////////////////////////////////////////////////////////////////////////
	
void CTopGunEngine::ConstructL(CSndMixer*	aSndMixer,
							  TSample& aMusic,
							  TSample& aShotSound,
							  TSample& aExplosionSound,
							  TInt aAudioVol,
							  TInt aSfxVol)
	{
	//Initial game settings
	iFightBoss = EFalse;
	iNumberOfLives = iLevelLoader.NumberOfLives() + 1;
	iDifficultyLevel = iLevelLoader.LevelNumber() ; 
	iScores = iLevelLoader.GetScore();
	iUtilsFrame = 0;
	iBulletScreenPos = TPoint(0,0);
	iEnemyBulletScreenPos = TPoint(0,0);
	iEnemyPlaceOfExplosion = TPoint(0,0);
	iShipPlaceOfExplosion = TPoint(0,0);
	iEnemyExplosion = EFalse;
	iBossExplosion = EFalse;
	iLevelEnd = EFalse;
	iExplosion = EFalse;
	iGameOver = EFalse;
	iBlockKeys = EFalse;
	iLastEnemyIndex = -1;
	iNumberOfEnemies = 4;
	iIndexOfEnemies = 0;
	//Initial speed of the Enemy
	iEnemySpeed = 1;
	//Initial energy of the Enemy
	iEnemyEnergy = 5;
	//Bullet
	iBullet = NULL;
	//iEnemyBullet = NULL;
	//Bullet's speed
	iBulletSpeed = 10;
	//Ship's starting speed
	iShipSpeed = 2;
	//Ship's start positon, maybe it would be better to change to TPoint
	iShipX = 88;
	iShipY = 160;
	iPlaying = EFalse;
	iBoss = ETrue;
	//sound set-up
	iSndMixer = aSndMixer;
	iMusic = aMusic;
	iShotSound = aShotSound;
	iExplosionSound = aExplosionSound;
	iAudioVol = aAudioVol;
	iSfxVol = aSfxVol;
	iSndMixer->Play( iMusic, 5, 11025, iAudioVol );

	iGameTimer = CGameTimer::NewL(*this);
	// Direct screen access
	iDirectScreenAccess = CDirectScreenAccess::NewL(iClient, iScreenDevice, iWindow, *this);

	ConstructSimpleMapL();
//	ConstructCompoundMapL();
	ConstructShipL();
	ConstructEnemyL();

	iDoubleBufferedArea = CDoubleBufferedArea::NewL(iSize, EColor4K);
	iOffscreenContext = &(iDoubleBufferedArea->GetDoubleBufferedAreaContext());

	ConstructExplosionL(TPoint(0,0),TRect(0,0,176,208),iOffscreenContext);
	ConstructUtilsL();

	iSndMixer->Resume();
	}

////////////////////////////////////////////////////////////////////////

CTopGunEngine::~CTopGunEngine()
	{
	delete iGameTimer;
	iGameTimer = NULL;
	
	delete iDirectScreenAccess;
	iDirectScreenAccess = NULL;
	
	delete iDoubleBufferedArea;
	iDoubleBufferedArea = NULL;

	delete iMap;
	iMap = NULL;

	delete iNodeFactory;
	iNodeFactory = NULL;
	
	delete iLeafFactory;
	iLeafFactory = NULL;
	
	delete iExplosionAnim;
	iExplosionAnim = NULL;

	delete iGameOverAnim;
	iGameOverAnim = NULL;

	delete iPauseGameAnim;
	iPauseGameAnim = NULL;

	delete iScoreBarAnim;
	iScoreBarAnim = NULL;

	delete[] iShipVectors;
	iShipVectors = NULL;

	delete iShipAnim;
	iShipAnim = NULL;
	
	delete iFireAnim;
	iFireAnim = NULL;

	//Destroy Bullets;
	//delete iBullet;
	iBullet = NULL;
	//ship bullets
	iBulletArray.ResetAndDestroy();
	iBulletArray.Close();
	//enemy bullets
	iEnemyBulletArray.ResetAndDestroy();
	iEnemyBulletArray.Close();

	iEnemyIndexesArray.Reset();
	iEnemyIndexesArray.Close();

	iEnemyArray.ResetAndDestroy();
	iEnemyArray.Close();

	iPositionArray.ResetAndDestroy();
	iPositionArray.Close();

	//iSndMixer->Stop(5);
	}

void CTopGunEngine::LoadMap1()
	{
	TInt mapData[] =                
	{
	39,40,1,1,1,4,
	45,46,1,2,1,1,
	51,52,1,1,9,10,
	57,58,1,1,12,13,
	24,25,3,1,15,16,
	26,27,1,1,18,19,
	1,2,1,1,21,22,
	1,1,1,1,28,1,
	1,4,1,1,1,1,
	1,1,1,9,10,11,
	33,34,1,12,13,14,
	39,40,1,15,16,17,
	45,46,1,18,19,20,
	51,52,2,21,22,23,
	57,58,1,1,5,6,
	3,60,61,62,7,8,
	1,64,65,66,1,1,
	68,69,70,71,24,25,
	73,74,75,1,26,27,
	77,78,79,1,1,1,
	1,1,1,28,1,1,
	1,2,1,1,1,1,
	1,1,1,1,1,1,
	1,1,4,1,3,1,
	1,1,1,1,1,1,
	1,28,1,9,10,11,
	1,1,1,12,13,14,
	1,1,1,15,16,17,
	5,6,1,18,19,20,
	7,8,1,21,22,23,
	25,1,1,1,2,1,
	27,3,1,1,1,1,
	1,1,1,30,31,32,
	1,1,35,36,37,38,
	4,1,41,42,43,44,
	1,1,47,48,49,50,
	1,1,1,54,55,56,
	1,5,6,1,1,1,
	1,7,8,1,3,1,
	1,2,1,1,5,6,
	1,1,1,1,7,8,
	1,1,4,1,1,24,
	1,1,1,1,1,26,
	33,1,1,2,1,9,
	39,40,1,1,1,12,
	45,46,3,1,1,15,
	51,52,1,1,1,18,
	57,58,1,1,1,21,
	1,1,1,1,28,1,
	1,1,1,1,1,1,
	1,2,1,4,1,1,
	1,1,1,9,10,11,
	3,1,1,12,13,14,
	1,1,1,15,16,17,
	1,24,25,18,19,20,
	1,26,27,21,22,23,
	1,1,3,1,1,1,
	1,1,1,1,1,2,
	32,33,34,1,1,1,
	38,39,40,1,1,1,
	44,45,46,1,4,1,
	50,51,52,1,1,1,
	56,57,1,24,25,1,
	1,1,1,26,27,1,
	1,1,28,1,1,1,
	1,5,6,1,1,1,
	1,7,8,3,1,1,
	1,1,1,1,30,31,
	4,1,1,35,36,37,
	1,1,1,41,42,43,
	1,2,1,47,48,49,
	1,1,1,53,54,55,
	1,1,1,1,1,1,
	24,25,1,1,1,1,
	26,27,1,4,1,1,
	1,1,1,1,1,1,
	5,6,60,61,62,24,
	7,8,64,65,66,26,
	67,68,69,70,71,1,
	72,73,74,75,1,5,
	76,77,78,79,1,7,
	6,1,1,1,5,6,
	8,28,1,1,7,8,
	1,1,1,1,1,1,
	1,1,4,1,1,1,
	1,1,29,30,31,32,
	1,2,35,36,37,38,
	1,1,41,42,43,44,
	1,1,47,48,49,50,
	33,34,53,54,55,56,
	39,40,1,1,1,4,
	45,46,1,2,1,1,
	51,52,1,1,9,10,
	57,58,1,1,12,13,
	24,25,3,1,15,16,
	26,27,1,1,18,19,
	1,2,1,1,21,22,
	1,1,1,1,28,1,
	1,4,1,1,1,1,
	1,1,1,9,10,11,
	33,34,1,12,13,14,
	39,40,1,15,16,17,
	45,46,1,18,19,20,
	51,52,2,21,22,23,
	57,58,1,1,5,6,
	3,60,61,62,7,8,
	1,64,65,66,1,1,
	68,69,70,71,24,25,
	73,74,75,1,26,27,
	77,78,79,1,1,1,
	1,1,1,28,1,1,
	1,2,1,1,1,1,
	1,1,1,1,1,1,
	1,1,4,1,3,1,
	1,1,1,1,1,1,
	1,28,1,9,10,11,
	1,1,1,12,13,14,
	1,1,1,15,16,17,
	5,6,1,18,19,20,
	7,8,1,21,22,23,
	25,1,1,1,2,1,
	27,3,1,1,1,1,
	1,1,1,30,31,32,
	1,1,35,36,37,38,
	4,1,41,42,43,44,
	1,1,47,48,49,50,
	1,1,1,54,55,56,
	1,5,6,1,1,1,
	1,7,8,1,3,1,
	1,2,1,1,5,6,
	1,1,1,1,7,8,
	1,1,4,1,1,24,
	1,1,1,1,1,26,
	33,1,1,2,1,9,
	39,40,1,1,1,12,
	45,46,3,1,1,15,
	51,52,1,1,1,18,
	57,58,1,1,1,21,
	1,1,1,1,28,1,
	1,1,1,1,1,1,
	1,2,1,4,1,1,
	1,1,1,9,10,11,
	3,1,1,12,13,14,
	1,1,1,15,16,17,
	1,24,25,18,19,20,
	1,26,27,21,22,23,
	1,1,3,1,1,1,
	1,1,1,1,1,2,
	32,33,34,1,1,1,
	38,39,40,1,1,1,
	44,45,46,1,4,1,
	50,51,52,1,1,1,
	56,57,1,24,25,1,
	1,1,1,26,27,1,
	1,1,28,1,1,1,
	1,5,6,1,1,1,
	1,7,8,3,1,1,
	1,1,1,1,30,31,
	4,1,1,35,36,37,
	1,1,1,41,42,43,
	1,2,1,47,48,49,
	1,1,1,53,54,55,
	1,1,1,1,1,1,
	24,25,1,1,1,1,
	26,27,1,4,1,1,
	1,1,1,1,1,1
	};
	iShipBounds		= TRect(TPoint(0x00000800,0x00000800),TPoint(0x00001400,0x0000FA00));

	iShipWorldPos	= TPoint(0x00000DF0,0x0000AE00);
	//changed for end of level testing 
	//iShipWorldPos	= TPoint(0x00000DF0,0x00000700);
	
	
	iWhenCreateNewEnemies = iShipWorldPos.iY - 0x0000000f;
	iLeafFactory = CTopGunLeafTileset::NewL(iImageFactory.TileImage());
	iMap = CTilemap::NewL(6,166,&mapData[0],*iLeafFactory);
	}


void CTopGunEngine::LoadMap2()
	{
	TInt mapData[] =                
	{
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6
	};
	iShipBounds		= TRect(TPoint(0x00000800,0x00000800),TPoint(0x00001400,0x0000FA00));

	iShipWorldPos	= TPoint(0x00000DF0,0x0000AE00);
	//changed for end of level testing 
	//iShipWorldPos	= TPoint(0x00000DF0,0x00000700);
	
	
	iWhenCreateNewEnemies = iShipWorldPos.iY - 0x0000000f;
	iLeafFactory = CTopGunLeafTileset::NewL(iImageFactory.TileImage());
	iMap = CTilemap::NewL(6,166,&mapData[0],*iLeafFactory);
	}

void CTopGunEngine::LoadMap3()
	{
	TInt mapData[] =                
	{
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,2,1,1,
	1,13,14,1,1,1,
	1,17,18,1,1,1,
	45,46,1,1,1,3,
	47,48,1,1,1,4,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,7,8,9,2,
	1,2,10,11,12,1,
	1,1,1,1,1,1,
	3,1,19,20,1,1,
	4,1,21,22,1,1,
	1,1,1,1,39,40,
	1,1,1,1,41,42,
	20,1,1,1,43,44,
	22,1,1,1,1,1,
	13,14,1,1,1,1,
	15,16,1,1,1,1,
	17,18,1,1,2,1,
	1,1,2,1,1,1,
	1,23,24,25,26,1,
	9,27,28,29,30,1,
	12,31,32,33,34,1,
	1,1,36,37,38,1,
	1,1,1,1,1,1,
	1,1,1,1,5,1,
	1,1,1,1,6,2,
	1,1,1,1,1,1,
	3,1,1,1,1,1,
	4,2,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,13,
	1,1,1,1,1,15,
	1,1,1,13,14,17,
	8,9,1,15,16,1,
	11,12,1,17,18,1,
	1,1,39,40,1,2,
	46,1,41,42,1,1,
	48,1,43,44,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,23,24,25,
	1,2,1,27,28,29,
	1,1,1,31,32,33,
	1,1,1,1,36,37,
	1,1,1,1,1,1,
	1,13,14,1,7,8,
	1,15,16,1,10,11,
	1,17,18,1,1,1,
	1,1,1,1,1,3,
	1,1,1,1,1,4,
	1,39,40,1,1,1,
	1,41,42,1,1,1,
	1,43,44,1,2,1,
	1,1,1,1,1,1,
	1,1,1,19,20,1,
	1,1,1,21,22,1,
	2,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	46,13,14,1,1,5,
	48,15,16,1,1,6,
	3,17,18,1,2,1,
	4,1,1,1,1,1,
	5,1,1,1,1,1,
	6,1,1,1,1,1,
	1,2,1,1,13,14,
	1,1,1,1,15,16,
	40,1,1,1,17,18,
	42,1,1,1,1,1,
	44,1,1,1,1,1,
	1,1,13,14,1,1,
	1,1,15,16,1,1,
	1,1,17,18,2,45,
	8,9,1,1,2,47,
	11,12,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,2,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,2,1,1,
	1,13,14,1,1,1,
	1,17,18,1,1,1,
	45,46,1,1,1,3,
	47,48,1,1,1,4,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,7,8,9,2,
	1,2,10,11,12,1,
	1,1,1,1,1,1,
	3,1,19,20,1,1,
	4,1,21,22,1,1,
	1,1,1,1,39,40,
	1,1,1,1,41,42,
	20,1,1,1,43,44,
	22,1,1,1,1,1,
	13,14,1,1,1,1,
	15,16,1,1,1,1,
	17,18,1,1,2,1,
	1,1,2,1,1,1,
	1,23,24,25,26,1,
	9,27,28,29,30,1,
	12,31,32,33,34,1,
	1,1,36,37,38,1,
	1,1,1,1,1,1,
	1,1,1,1,5,1,
	1,1,1,1,6,2,
	1,1,1,1,1,1,
	3,1,1,1,1,1,
	4,2,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,13,
	1,1,1,1,1,15,
	1,1,1,13,14,17,
	8,9,1,15,16,1,
	11,12,1,17,18,1,
	1,1,39,40,1,2,
	46,1,41,42,1,1,
	48,1,43,44,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,23,24,25,
	1,2,1,27,28,29,
	1,1,1,31,32,33,
	1,1,1,1,36,37,
	1,1,1,1,1,1,
	1,13,14,1,7,8,
	1,15,16,1,10,11,
	1,17,18,1,1,1,
	1,1,1,1,1,3,
	1,1,1,1,1,4,
	1,39,40,1,1,1,
	1,41,42,1,1,1,
	1,43,44,1,2,1,
	1,1,1,1,1,1,
	1,1,1,19,20,1,
	1,1,1,21,22,1,
	2,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	46,13,14,1,1,5,
	48,15,16,1,1,6,
	3,17,18,1,2,1,
	4,1,1,1,1,1,
	5,1,1,1,1,1,
	6,1,1,1,1,1,
	1,2,1,1,13,14,
	1,1,1,1,15,16,
	40,1,1,1,17,18,
	42,1,1,1,1,1,
	44,1,1,1,1,1,
	1,1,13,14,1,1,
	1,1,15,16,1,1,
	1,1,17,18,2,45,
	8,9,1,1,2,47,
	11,12,1,1,1,1,
	1,1,1,1,1,1
	};
	iShipBounds		= TRect(TPoint(0x00000800,0x00000800),TPoint(0x00001400,0x0000FA00));

	iShipWorldPos	= TPoint(0x00000DF0,0x0000AE00);
	//changed for end of level testing 
	//iShipWorldPos	= TPoint(0x00000DF0,0x00000700);
	
	
	iWhenCreateNewEnemies = iShipWorldPos.iY - 0x0000000f;
	iLeafFactory = CTopGunLeafTileset::NewL(iImageFactory.TileImage());
	iMap = CTilemap::NewL(6,166,&mapData[0],*iLeafFactory);
	}

void CTopGunEngine::LoadMap4()
	{
	TInt mapData[] =                
	{
	35,36,37,38,2,23,
	39,40,41,15,16,25,
	15,16,2,17,18,1,
	17,18,1,1,1,2,
	1,1,19,20,1,1,
	27,28,21,22,3,4,
	29,30,23,24,7,8,
	15,16,25,26,11,12,
	17,18,1,1,1,1,
	20,15,16,1,15,16,
	22,17,18,1,17,18,
	24,1,1,35,36,37,
	26,31,32,39,40,41,
	15,33,34,1,1,1,
	17,18,1,19,20,15,
	37,38,1,21,22,17,
	41,2,1,23,24,19,
	2,15,16,25,26,21,
	1,17,18,1,1,23,
	3,4,5,6,1,25,
	7,8,9,10,15,16,
	11,12,13,14,17,18,
	15,16,1,1,1,1,
	17,18,1,15,16,1,
	1,1,2,17,18,2,
	2,31,32,1,1,1,
	2,33,34,19,20,2,
	27,28,1,21,22,2,
	29,30,1,23,24,2,
	19,20,1,25,26,1,
	21,22,35,36,37,38,
	23,24,39,40,41,1,
	25,26,2,27,28,3,
	4,5,6,29,30,7,
	8,9,10,2,1,11,
	12,13,14,15,16,1,
	15,16,1,17,18,15,
	17,18,15,16,1,17,
	15,16,17,18,1,1,
	17,18,2,15,16,19,
	1,1,1,17,18,21,
	35,36,37,38,2,23,
	39,40,41,15,16,25,
	15,16,2,17,18,1,
	17,18,1,1,1,2,
	1,1,19,20,1,1,
	27,28,21,22,3,4,
	29,30,23,24,7,8,
	15,16,25,26,11,12,
	17,18,1,1,1,1,
	20,15,16,1,15,16,
	22,17,18,1,17,18,
	24,1,1,35,36,37,
	26,31,32,39,40,41,
	15,33,34,1,1,1,
	17,18,1,19,20,15,
	37,38,1,21,22,17,
	41,2,1,23,24,19,
	2,15,16,25,26,21,
	1,17,18,1,1,23,
	3,4,5,6,1,25,
	7,8,9,10,15,16,
	11,12,13,14,17,18,
	15,16,2,1,1,1,
	17,18,1,1,1,35,
	1,19,20,1,1,39,
	1,21,22,2,1,1,
	2,23,24,1,1,2,
	1,25,26,1,1,1,
	5,6,1,1,1,1,
	9,10,1,15,16,3,
	13,14,1,17,18,7,
	35,36,37,38,1,11,
	39,40,41,1,1,1,
	1,1,1,1,1,1,
	1,31,32,1,1,1,
	2,33,34,19,20,2,
	27,28,1,21,22,2,
	29,30,1,23,24,2,
	19,20,1,25,26,1,
	21,22,35,36,37,38,
	23,24,39,40,41,1,
	25,26,2,27,28,3,
	4,5,6,29,30,7,
	8,9,10,2,1,11,
	12,13,14,15,16,1,
	15,16,1,17,18,15,
	17,18,15,16,1,17,
	15,16,17,18,1,1,
	17,18,2,15,16,19,
	1,1,1,17,18,21,
	35,36,37,38,2,23,
	39,40,41,15,16,25,
	15,16,2,17,18,1,
	17,18,1,1,1,2,
	1,1,19,20,1,1,
	27,28,21,22,3,4,
	29,30,23,24,7,8,
	15,16,25,26,11,12,
	17,18,1,1,1,1,
	20,15,16,1,15,16,
	22,17,18,1,17,18,
	24,1,1,35,36,37,
	26,31,32,39,40,41,
	15,33,34,1,1,1,
	17,18,1,19,20,15,
	37,38,1,21,22,17,
	41,2,1,23,24,19,
	2,15,16,25,26,21,
	1,17,18,1,1,23,
	3,4,5,6,1,25,
	7,8,9,10,15,16,
	11,12,13,14,17,18,
	15,16,1,1,1,1,
	17,18,1,15,16,1,
	1,1,2,17,18,2,
	2,31,32,1,1,1,
	2,33,34,19,20,2,
	27,28,1,21,22,2,
	29,30,1,23,24,2,
	19,20,1,25,26,1,
	21,22,35,36,37,38,
	23,24,39,40,41,1,
	25,26,2,27,28,3,
	4,5,6,29,30,7,
	8,9,10,2,1,11,
	12,13,14,15,16,1,
	15,16,1,17,18,15,
	17,18,15,16,1,17,
	15,16,17,18,1,1,
	17,18,2,15,16,19,
	1,1,1,17,18,21,
	35,36,37,38,2,23,
	39,40,41,15,16,25,
	15,16,2,17,18,1,
	17,18,1,1,1,2,
	1,1,19,20,1,1,
	27,28,21,22,3,4,
	29,30,23,24,7,8,
	15,16,25,26,11,12,
	17,18,1,1,1,1,
	20,15,16,1,15,16,
	22,17,18,1,17,18,
	24,1,1,35,36,37,
	26,31,32,39,40,41,
	15,33,34,1,1,1,
	17,18,1,19,20,15,
	37,38,1,21,22,17,
	41,2,1,23,24,19,
	2,15,16,25,26,21,
	1,17,18,1,1,23,
	3,4,5,6,1,25,
	7,8,9,10,15,16,
	11,12,13,14,17,18,
	15,16,2,1,1,1,
	17,18,1,1,1,35,
	1,19,20,1,1,39,
	1,21,22,2,1,1,
	2,23,24,1,1,2,
	1,25,26,1,1,1,
	5,6,1,1,1,1,
	9,10,1,15,16,3,
	13,14,1,17,18,7,
	35,36,37,38,1,11,
	39,40,41,1,1,1,
	1,1,1,1,1,1
	};
	iShipBounds		= TRect(TPoint(0x00000800,0x00000800),TPoint(0x00001400,0x0000FA00));

	iShipWorldPos	= TPoint(0x00000DF0,0x0000AE00);
	//changed for end of level testing 
	//iShipWorldPos	= TPoint(0x00000DF0,0x00000700);
	
	
	iWhenCreateNewEnemies = iShipWorldPos.iY - 0x0000000f;
	iLeafFactory = CTopGunLeafTileset::NewL(iImageFactory.TileImage());
	iMap = CTilemap::NewL(6,166,&mapData[0],*iLeafFactory);
	}

void CTopGunEngine::LoadMap5()
	{
	TInt mapData[] =                
	{
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	73,74,75,76,77,78,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	73,74,75,76,77,78,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	73,74,75,76,77,78,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	73,74,75,76,77,78,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	73,74,75,76,77,78,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	73,74,75,76,77,78,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	73,74,75,76,77,78,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	73,74,75,76,77,78,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	73,74,75,76,77,78,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	73,74,75,76,77,78,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	73,74,75,76,77,78,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6,
	73,74,75,76,77,78,
	67,68,69,70,71,72,
	61,62,63,64,65,66,
	55,56,57,58,59,60,
	49,50,51,52,53,54,
	43,44,45,46,47,48,
	37,38,39,40,41,42,
	31,32,33,34,35,36,
	25,26,27,28,29,30,
	19,20,21,22,23,24,
	13,14,15,16,17,18,
	7,8,9,10,11,12,
	1,2,3,4,5,6
	};
	iShipBounds		= TRect(TPoint(0x00000800,0x00000800),TPoint(0x00001400,0x0000FA00));

	iShipWorldPos	= TPoint(0x00000DF0,0x0000AE00);
	//changed for end of level testing 
	//iShipWorldPos	= TPoint(0x00000DF0,0x00000700);
	
	
	iWhenCreateNewEnemies = iShipWorldPos.iY - 0x0000000f;
	iLeafFactory = CTopGunLeafTileset::NewL(iImageFactory.TileImage());
	iMap = CTilemap::NewL(6,166,&mapData[0],*iLeafFactory);
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::ConstructSimpleMapL()
    {
	switch(iLevelLoader.LevelNumber())
		{
	case 0:
		{
		LoadMap1();
		break;
		}

	case 1:
		{
		LoadMap2();
		break;
		}

	case 2:
		{
		LoadMap3();
		break;
		}

	case 3:
		{
		LoadMap4();
		break;
		}

	case 4:
		{
		LoadMap5();
		break;
		}
	default:
		{
		break;
		}
		};

	/*	
	iShipBounds		= TRect(TPoint(0x00000800,0x00000800),TPoint(0x00001400,0x0000FA00));
	
	iShipWorldPos	= TPoint(0x00000DF0,0x0000AE00);
	//changed for end of level testing 
	//iShipWorldPos	= TPoint(0x00000DF0,0x00000700);
	
	
	iWhenCreateNewEnemies = iShipWorldPos.iY - 0x0000000f;
	iLeafFactory = CTopGunLeafTileset::NewL(iImageFactory.TileImage());
	iMap = CTilemap::NewL(6,166,&mapData[0],*iLeafFactory);
	*/
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::ConstructCompoundMapL()
    {
	TInt mapData[] = 
		{
		15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
		15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
		15,15, 7, 3,11,15, 7, 3, 3, 3, 3, 3, 3,11,15,15,
		15,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,15,15,
		15,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,15,15,
		15,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,15,15,
		15,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,15,15,
		15,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,15,15,
		15,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,15,15,
		15,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,15,15,
		15,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,15,15,
		15,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,15,15,
		15,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,15,15,
		15,15,13,12,12,12,12,12,12,12,12,12,12,14,15,15,
		15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
		15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
		};

	iShipBounds		= TRect(TPoint(0x00000800,0x00000800),TPoint(0x00007800,0x00007800));
	
	
	//changed for end of level testing 
	//iShipWorldPos	= TPoint(0x0000900,0x00007600);
	iShipWorldPos	= TPoint(0x0000900,0x00000700);

	iLeafFactory = CTopGunLeafTileset::NewL(iImageFactory.TileImage());
	iNodeFactory = CTopGunNodeTileset::NewL(*iLeafFactory);
	iMap = CTilemap::NewL(16,16,&mapData[0],*iNodeFactory);
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::ConstructShipL()
	{
	//make the velocity slower
	//iShipVelocity	= -TPoint(0x000e,0x0013);
	iShipVelocity	= -TPoint(0x000f,0x000f);

	//the number of frame that we start displaying the ship
	iShipFrame = 16;         
	iShipScreenPos = TPoint(iShipX, iShipY);
	iShipScreenPosUp4 = TPoint(iShipScreenPos.iX << 4, iShipScreenPos.iY << 4);

	iShipAnim = CShipAnim::NewL(iImageFactory.ShipAnim());
	//iFireAnim = CFireAnim::NewL(iImageFactory.FireAnim());
	

	//iEngineExhaust = CParticleSystem::NewL(16,*iFireAnim);
	//iEngineExhaust = CParticleSystem::NewL(16,*iExplosionAnim);

	// Physics parameters for ship motion:
	iDragDownshift = 8;
	//iDragRatio = 0xff;
	iDragRatio = 0x0100;
	//iThrust = 6;
	iThrust = 0;
	//pawel
	//iGravity = 1;
	iGravity = -1;


	// Allocate and initialise ship vectors
	// (each one a vector lying along the central axis of the ship
	// in the corresponding animation frame):
	
	iShipVectors = new TVectorFP[64];
	User::LeaveIfNull(iShipVectors);

	TReal degreesToRadians = 3.141592 / 180.0;

	// Animation frame #0 is at 45 degrees, so:
	TReal angle = 45.0 * degreesToRadians;
	TReal angleStep = ( 360.0 / 64.0 ) * degreesToRadians;

	for ( TInt frame = 0 ; frame < 32 ; frame++ )
		{
		TReal cos;
		TReal sin;

		Math::Cos(cos,angle);
		Math::Sin(sin,angle);

		TInt16 cosFP;
		TInt16 sinFP;

		Math::Int(cosFP,cos * 256.0);
		Math::Int(sinFP,sin * 256.0);

		iShipVectors[frame] = TVectorFP(TPoint(cosFP,sinFP));

		angle += angleStep;
		}
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::ConstructExplosionL(const TPoint &/*aPlaceOfExplosion*/,const TRect& /*aRect*/,CFbsBitGc* /*aCallerGc*/)
	{
	iExplosionAnim = CExplosionAnim::NewL(iImageFactory.ExplosionAnim(),iShipPlaceOfExplosion);
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::ConstructUtilsL()
	{
	iGameOverAnim = CUtilsAnim::NewL(iImageFactory.GameOver(),TPoint(13,61),TSize(150,87));
	iPauseGameAnim = CUtilsAnim::NewL(iImageFactory.Pause(),TPoint(13,61),TSize(150,87));
	iScoreBarAnim = CUtilsAnim::NewL(iImageFactory.ScoreBar(),TPoint(0,0),TSize(178,26));
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::ConstructEnemyL()
	{
	//iPositionArray.Reset();
	
	iPositionArray.Append(CPosition::NewL(TPoint(55,-64)));
	iPositionArray.Append(CPosition::NewL(TPoint(160,-32)));
	iPositionArray.Append(CPosition::NewL(TPoint(25,-128)));
	iPositionArray.Append(CPosition::NewL(TPoint(95,-256)));

	//the number of frame that we start displaying the Enemy
	iEnemyFrame = 0;
	
	//create enemies and add them to the enemy's array
	iEnemyArray.Append(CEnemyAnim::NewL(iImageFactory.EnemyAnim2(),
										iPositionArray[0]->Position(),
										2+iDifficultyLevel, //speed
										7+iDifficultyLevel, //energy
										7+iDifficultyLevel, //org energy
										0+(iDifficultyLevel*10), //zig zac
										TSize(24,32),
										ERaider));
	iEnemyArray.Append(CEnemyAnim::NewL(iImageFactory.EnemyAnim3(),
										iPositionArray[1]->Position(),
										3+iDifficultyLevel,
										4+iDifficultyLevel,
										4+iDifficultyLevel,
										0+(iDifficultyLevel*10),
										TSize(27,48),
										EHelicopter));
	iEnemyArray.Append(CEnemyAnim::NewL(iImageFactory.EnemyAnim4(),
										iPositionArray[2]->Position(),
										4+iDifficultyLevel,
										3+iDifficultyLevel,
										3+iDifficultyLevel,
										20+iDifficultyLevel,
										TSize(20,32),
										EScout));
	iEnemyArray.Append(CEnemyAnim::NewL(iImageFactory.EnemyAnim6(),
										iPositionArray[3]->Position(),
										4+iDifficultyLevel,
										1+iDifficultyLevel,
										1+iDifficultyLevel,
										20+iDifficultyLevel,
										TSize(24,32),
									    ESuicide));
	iEnemyArray.Append(CEnemyAnim::NewL(iImageFactory.EnemyAnim8(),
										TPoint(35,-16),
										2,
										100+(iDifficultyLevel*20),
										100+(iDifficultyLevel*20),
										90+(iDifficultyLevel*2),
										TSize(40,138),
										EBoss));
	
	//minus boss at the end of the level
	iIndexOfEnemies = iEnemyArray.Count() - 1;
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::Simulate()
	{
	iUtilsFrame = 0;
	if (iExplosion || iEnemyExplosion || iBossExplosion)
		{
		iExplosionFrame++;

		if (iExplosionFrame >= 13)
			{
			iExplosion = EFalse;
			iEnemyExplosion = EFalse;
			iBossExplosion = EFalse;
			iExplosionFrame = 0;
			}
		}

	if (!iBlockKeys)
		{
		UpdateBullets();
		UpdateEnemyBullets();
		UpdatePosition();

		if( iKeyHandler.Direction() == TKeyHandler::ELeft )
			{
			iShipFrame--;
			//move the ship to the left
			if (iShipX > 0)
				{
				iShipX = iShipX - iShipSpeed;
				}
			
			}
		else if ( iKeyHandler.Direction() == TKeyHandler::ERight )
			{
			iShipFrame++;
			//move the ship to the right
			if (iShipX < 176)
				{
				iShipX = iShipX + iShipSpeed;
				}
			}
		else if ( iKeyHandler.Direction() == TKeyHandler::EDown )
			{
			//iShipFrame++;
			//move the ship down
			if (iShipY < 208)
				{
				iShipY = iShipY + iShipSpeed;
				}
			}
		else if ( iKeyHandler.Direction() == TKeyHandler::EUp )
			{
			//iShipFrame--;
			//move the ship up
			if (iShipY > 0)
				{
				iShipY = iShipY - iShipSpeed;
				}
			}

		iBulletFrame = 0;
		iEnemyBulletFrame = 0;
		if (iKeyHandler.FirePressed())
			{
			if (iBulletArray.Count() < 10)
				{
				TPoint adjustBulletPos(0x00000000,0x00000005);
				TPoint bulletStartPos = iShipScreenPos - adjustBulletPos;
				iBulletArray.Append(CBulletAnim::NewL(iImageFactory.BulletAnim(),bulletStartPos,iBulletSpeed,TSize(13,13),EMiddleO));
				iSndMixer->Play( iShotSound, 6, 11025, iSfxVol );
				}
			else
				{
				iBulletArray.ResetAndDestroy();
				}
			}
		}

	iShipFrame &= 0x1f;
	iEnemyFrame &= 0x1;
	iExplosionFrame &= 0xf;
	iBulletFrame &= 0x1;
	iUtilsFrame &= 0x1;
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::UpdatePosition()
	{
	TPoint newWorldPos = iShipWorldPos + iShipVelocity;
	if (!iLevelEnd)
		{
		if ( iShipBounds.Contains(newWorldPos) )
			{
			iShipWorldPos = newWorldPos;
			}
		
		else
			{
			//bound from the bounduaries
			if ( ( newWorldPos.iX < iShipBounds.iTl.iX ) || ( newWorldPos.iX >= iShipBounds.iBr.iX ) )
				{
				iShipVelocity.iX = -iShipVelocity.iX;
				}
			//stop moving when end of the level
			if (newWorldPos.iY >= 0x00000800)
				{
				if ( ( newWorldPos.iY < iShipBounds.iTl.iY ) || ( newWorldPos.iY >= iShipBounds.iBr.iY ) )
					{
					iShipVelocity.iY = -iShipVelocity.iY;
					}
				}
			}
		}
	else
		{
		if (iShipY > -32)
			{
			iShipY = iShipY - iShipSpeed;
			}
		else
			{
			iPercentageOfDestroyedShips = ((iDestroyedShips/iAllShips) * 100);


			TInt theEnd = 0;
			if (iLevelLoader.IsLevelCompleted(iPercentageOfDestroyedShips))
				{
				theEnd = iLevelLoader.NextLevel();
				iLevelLoader.IncreaseDifficultyLevel();
				if (theEnd != KErrNotFound)
					{
					iLevelLoader.AddScore(iScores);
					iLevelLoader.IncreaseNumberOfLives();
					iContainer.SwitchView(7); //go to "victory screen"	
					}
				else
					{
					iLevelLoader.EnterHighScore(ETrue);
					iLevelLoader.AddScore(iScores);
					iContainer.SwitchView(10); //High-score screen	
					}
				}
			else
				{
				iLevelLoader.ResetScore();
				iContainer.SwitchView(9); //mission objectives
				}
			}
		}
	
	//to update the screen position of the ship
	iShipScreenPos = TPoint(iShipX, iShipY);
	iShipScreenPosUp4 = TPoint(iShipScreenPos.iX << 4, iShipScreenPos.iY << 4); 
	
	//do not move the map when ship changes onscreen position
	iMapWindowTopLeft.iY = iShipWorldPos.iY - 0x00000020;
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::UpdateBullets()
	{
	TInt index,arraySize;

	arraySize = iBulletArray.Count();

	for (index = 0; index < arraySize; index++)
		{
		for (TInt i = 0; i < iEnemyIndexesArray.Count();i++)
			{	
			iEnemyAnim = iEnemyArray[iEnemyIndexesArray[i]];
			iEnemyScreenPos = iEnemyAnim->GetEnemyScreenPos();
				
			TPoint stCorner(iEnemyScreenPos.iX-16, iEnemyScreenPos.iY);
			//TPoint ndCorner(iEnemyScreenPos.iX + (iEnemyAnim->GetEnemySize()).iWidth , 
			//	            iEnemyScreenPos.iY + (iEnemyAnim->GetEnemySize()).iHeight);
			TPoint ndCorner(iEnemyScreenPos.iX+16 , iEnemyScreenPos.iY+32);
			//TPoint ndCorner = (iEnemyAnim->GetEnemySize()).AsPoint();
			TRect enemyArea (stCorner, ndCorner);
			
			if ((iBulletArray.Count() > 0 ) && (index < iBulletArray.Count()))
				{
				if (iBulletArray[index] != NULL)
					{
					TPoint stBulletCorner(iBulletArray[index]->GetBulletScreenPos().iX-6, iBulletArray[index]->GetBulletScreenPos().iY);
					//TPoint ndBulletCorner( (iBulletArray[index]->GetBulletScreenPos()).iX + (iBulletArray[index]->GetBulletSize()).iWidth, (iBulletArray[index]->GetBulletScreenPos()).iY + (iBulletArray[index]->GetBulletSize()).iHeight );

																						  //13	
					TPoint ndBulletCorner(((iBulletArray[index]->GetBulletScreenPos()).iX + 6), (iBulletArray[index]->GetBulletScreenPos()).iY);

					//check collision
					if (enemyArea.Contains(stBulletCorner) || enemyArea.Contains(ndBulletCorner))
						{
						//destroy coliding enemy
						iScores = iScores + 10;
						if (iEnemyAnim->EnemyHit())
							{
							iDestroyedShips++;
							iSndMixer->Play( iExplosionSound, 7 , 11025, iSfxVol );
							switch (iEnemyAnim->EnemyType())
							{
							case EScout:
							case EHelicopter:
							case ERaider:
							case ESuicide:
								{
								iEnemyExplosion = ETrue;
								break;
								}
							case EBoss:
								{
								iBossExplosion = ETrue;
								iLevelEnd = ETrue;
								break;
								}
							}
							iEnemyPlaceOfExplosion = iEnemyScreenPos;
							DestroyEnemy();
							iEnemyIndexesArray.Remove(i);
							iEnemyIndexesArray.Compress();
							}

						CBulletAnim* bullet = iBulletArray[index];
						delete bullet;
						bullet = NULL;
						iBulletArray.Remove(index);
						//iBulletArray.Compress();
						}
					}
				}
			}

		if ((iBulletArray.Count() > 0 ) && (index < iBulletArray.Count()))
			{
			if (iBulletArray[index] != NULL)
				{
				if (iBulletArray[index]->GetBulletScreenPos().iY > 0) //bug here!!!
					{
					TPoint newBulletPos = iBulletArray[index]->GetBulletScreenPos();
					newBulletPos.iY -= iBulletSpeed;  
					iBulletArray[index]->SetBulletScreenPos(newBulletPos);
					}
				else
					{
					
					CBulletAnim* bullet = iBulletArray[index];
					delete bullet;
					bullet = NULL;
					iBulletArray.Remove(index);
					//iBulletArray.Compress();
					}
				}
			}
		}
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::UpdateEnemyBullets()
	{
	TInt index,arraySize;

	arraySize = iEnemyBulletArray.Count();

	for (index = 0; index < arraySize; index++)
		{
		//if ((iEnemyBulletArray.Count() > 0) && (iEnemyBulletArray[index]))
		if (index < (iEnemyBulletArray.Count() - 1) )
			{
			//TPoint stCorner(iShipScreenPos.iX + 11, iShipScreenPos.iY);
			//TPoint ndCorner(iShipScreenPos.iX + 21 , iShipScreenPos.iY+32);
			TPoint stCorner(iShipScreenPos.iX - 4, iShipScreenPos.iY);
			//TPoint ndCorner(iShipScreenPos.iX + 21 , iShipScreenPos.iY+32);
			TPoint ndCorner(iShipScreenPos.iX + 10 , iShipScreenPos.iY + 32);
			TRect shipArea (stCorner, ndCorner);
			
			if ((iEnemyBulletArray.Count() > 0 ) && (index < iEnemyBulletArray.Count()))
				{
				if (iEnemyBulletArray[index] != NULL)
					{
					TPoint stBulletCorner(iEnemyBulletArray[index]->GetBulletScreenPos());
					TPoint ndBulletCorner(((iEnemyBulletArray[index]->GetBulletScreenPos()).iX + 3), 
										   (iEnemyBulletArray[index]->GetBulletScreenPos()).iY);

					//check collision
					if (shipArea.Contains(stBulletCorner) || shipArea.Contains(ndBulletCorner))
						{
						//iScores = 0; //reset the score
						if(iLevelLoader.NumberOfLives() > 0)
							{
							iLevelLoader.DecreaseNumberOfLives();
							iNumberOfLives = iLevelLoader.NumberOfLives()+1;
							iExplosion = ETrue;
							iShipPlaceOfExplosion = iShipScreenPos;
							}
						else
							{
							iGameOver = ETrue;
							iNumberOfLives = 0;
							iShipVelocity.iY = 0;
							iExplosion = ETrue;
							iShipPlaceOfExplosion = iShipScreenPos;
							//iShipScreenPos = TPoint(-32,-32);
							iSndMixer->Play( iExplosionSound, 7, 11025, iSfxVol );
							}

						//destroy coliding bullet
						CBulletAnim* enemyBullet = iEnemyBulletArray[index];
						delete enemyBullet;
						enemyBullet = NULL;
						iEnemyBulletArray.Remove(index);
						//iEnemyBulletArray.Compress();
						}
					}
				}



			if (iEnemyBulletArray[index]->GetBulletScreenPos().iY < 208)
				{
				if (iEnemyAnim->EnemyType() == EBoss)
					{
					TPoint newBulletPos;
					switch(iBullet->BulletOrientation())
						{
					case ERightO:
						newBulletPos = iEnemyBulletArray[index]->GetBulletScreenPos();
						newBulletPos.iY += (iBulletSpeed - 5);
						newBulletPos.iX += 3;
						iEnemyBulletArray[index]->SetBulletScreenPos(newBulletPos);
						break;
					case ELeftO:
						newBulletPos = iEnemyBulletArray[index]->GetBulletScreenPos();
						newBulletPos.iY += (iBulletSpeed - 5);
						newBulletPos.iX -= 3;
						iEnemyBulletArray[index]->SetBulletScreenPos(newBulletPos);
						break;
					default:
						newBulletPos = iEnemyBulletArray[index]->GetBulletScreenPos();
						newBulletPos.iY += (iBulletSpeed - 5);  
						iEnemyBulletArray[index]->SetBulletScreenPos(newBulletPos);
						break;
						};
					}
				else
					{
					TPoint newBulletPos = iEnemyBulletArray[index]->GetBulletScreenPos();
					newBulletPos.iY += (iBulletSpeed - 5);  
					iEnemyBulletArray[index]->SetBulletScreenPos(newBulletPos);
					}
				}
			else
				{
				//iEnemyBulletArray[index]->DestroyBullet();
				CBulletAnim* enemyBullet = iEnemyBulletArray[index];
				delete enemyBullet;
				enemyBullet = NULL;
				iEnemyBulletArray.Remove(index);
				//iEnemyBulletArray.Compress();
				}
			}
		}
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::Render(const TRect& aRect,CFbsBitGc* aCallerGc) const
	{
	TPoint pixelTopLeft(iMapWindowTopLeft.iX >> 4,iMapWindowTopLeft.iY >> 4);
	iMap->Render(pixelTopLeft,aRect,aCallerGc);
	
	//iEngineExhaust->Render(pixelTopLeft,aRect,aCallerGc);
	iShipAnim->RenderFrameCentered(iShipFrame,iShipScreenPos,aRect,aCallerGc);

	if(iExplosion)
		{
		iExplosionAnim->RenderFrameCentered(iExplosionFrame,iShipPlaceOfExplosion,aRect,aCallerGc);
		}

	if (iEnemyExplosion)
		{
		iExplosionAnim->RenderFrameCentered(iExplosionFrame,iEnemyPlaceOfExplosion,aRect,aCallerGc);
		}
	
	if (iBossExplosion)
		{
		iExplosionAnim->RenderFrameCentered(iExplosionFrame,iEnemyPlaceOfExplosion,aRect,aCallerGc);
		iExplosionAnim->RenderFrameCentered(iExplosionFrame,iEnemyPlaceOfExplosion + TPoint(0,32),aRect,aCallerGc);
		iExplosionAnim->RenderFrameCentered(iExplosionFrame,iEnemyPlaceOfExplosion - TPoint(0,32),aRect,aCallerGc);
		}
	//display score bar
	//iScoreBarAnim->SetUtilsScreenPos(TPoint(10,10));
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::SetupDirectScreenAccessL()
	{
	// Initialise DSA
	iDirectScreenAccess->StartL();

	// Get graphics context for it
	iGc = iDirectScreenAccess->Gc();

	// Get region that DSA can draw in
	iRegion = iDirectScreenAccess->DrawingRegion();

	// Set the display to clip to this region
	iGc->SetClippingRegion(iRegion);
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::StartFirstGameL()
	{
	StartGameL();
	// We only want to play the game if we have the whole screen available so 
	// on first start we record the region available for comparision later (in restart)
	iGameDawingArea = iRegion->BoundingRect();
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::StartGameL()
	{
	iPaused = EFalse;
	if(!iPlaying)
		{
		SetupDirectScreenAccessL();
		iGameTimer->Restart();
		iSndMixer->Resume();
		}
	iPlaying = ETrue;
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::StopGame()
	{
	//stop music
	
	//iSndMixer->Stop(0);
	
	//iSndMixer->Stop(1);
	//iSndMixer->Stop(2);
	//iSndMixer->Stop(3);
	//iSndMixer->Stop(4);
	iPaused = ETrue;
	DoGameFrame();
	//RenderPauseGame(iGameDawingArea, iOffscreenContext);
	//iGc->BitBlt(TPoint(0,0), &(iDoubleBufferedArea->GetDoubleBufferedAreaBitmap()));
	iSndMixer->Pause();
	iPlaying = EFalse;
	iGameTimer->CancelTimer();
	iDirectScreenAccess->Cancel();
	DoGameFrame();
	}

////////////////////////////////////////////////////////////////////////

TInt CTopGunEngine::DoGameFrame()
	{
	// Force screen update:
	iDirectScreenAccess->ScreenDevice()->Update();

	Simulate();

	// Drop 1 render frame in every 128, to avoid ViewSrv 11 problems:
	if (iDropRenderFrames & ((iFrameCounter & 0x007f) != 0x007f))
		{
		Render(iGameDawingArea, iOffscreenContext);
		iExplosion = EFalse;
		RenderEnemies(iGameDawingArea, iOffscreenContext);
		CreateEnemyBullets(iGameDawingArea, iOffscreenContext);
		CreateBullets(iGameDawingArea, iOffscreenContext);
		RenderScoreBar(iGameDawingArea, iOffscreenContext);
		RenderGameOver(iGameDawingArea, iOffscreenContext);
		DrawScores(iOffscreenContext);
		DrawNumberOfLives(iOffscreenContext);
		DrawPercentageOfDestroyedEnemies(iOffscreenContext);
		CheckShipCollision(iGameDawingArea, iOffscreenContext);
		}

	iFrameCounter++;


	//DrawScores(iOffscreenContext);


	if (iGameOver)
		{
		iShipScreenPos = TPoint(-32,-32);
		//DrawGameOver(iOffscreenContext);
		iBlockKeys = ETrue;
		//iScores = 0; //reset the score
		}

	if (iPaused)
		{
		RenderPauseGame(iGameDawingArea, iOffscreenContext);
		}
	// Draw from offscreen bitmap
	iGc->BitBlt(TPoint(0,0), &(iDoubleBufferedArea->GetDoubleBufferedAreaBitmap()));

	iClient.Flush();	

	if(iPaused)
		{
		return CGameTimer::StopTicking;
		}
	else
		{
		return CGameTimer::TickAgain;
		}
	}


////////////////////////////////////////////////////////////////////////
		
void CTopGunEngine::RenderEnemies(const TRect& aRect,CFbsBitGc* aCallerGc)
	{
	TInt i = 0;
	TInt enemyIndex = 0;
	TInt numberOfEnemies = 0;

	if (iWhenCreateNewEnemies == iShipWorldPos.iY)
		{
		iWhenCreateNewEnemies -= 150 * 15;
	
		numberOfEnemies = TempNumberGen( iNumberOfEnemies );
		if (numberOfEnemies == 0)
			{
			numberOfEnemies = 3;
			}

		if (numberOfEnemies == 1)
			{
			numberOfEnemies = 2;
			}

		//add number of enemies
		iAllShips = iAllShips + numberOfEnemies;

		while (numberOfEnemies != iEnemyIndexesArray.Count())
			{
			enemyIndex = TempNumberGen( iIndexOfEnemies );
			
			if (iLastEnemyIndex != enemyIndex)
				{
				iEnemyIndexesArray.Append(enemyIndex);
				iLastEnemyIndex = enemyIndex;
				}
			}
		}
	//create boss at the end of the level
	
	if ((iShipWorldPos.iY <= 2100) && (iBoss))
		{
		iEnemyIndexesArray.Append(4);
		iBoss = EFalse;
		iFightBoss = ETrue;
		//add boss
		iAllShips = iAllShips + 1;
		}
	

	iLastEnemyIndex = -1;

	for (i = 0; i < iEnemyIndexesArray.Count();i++)
		{	
		iEnemyAnim = iEnemyArray[iEnemyIndexesArray[i]];
		if(iEnemyAnim != NULL)
			{
			iEnemyScreenPos = iEnemyAnim->GetEnemyScreenPos();
			
			if (iEnemyScreenPos.iY >= 248)
				{
				DestroyEnemy(iPositionArray[i]->Position());
				iEnemyIndexesArray.Remove(i);
				iEnemyIndexesArray.Compress();
				}
			else
				{
				switch (iEnemyAnim->EnemyType())
					{
					case EScout:
					case EHelicopter:
					case ERaider:
					case ESuicide:
						{
						iEnemyScreenPos.iY = iEnemyScreenPos.iY + iEnemyAnim->GetEnemySpeed();
						
						//test of enemy's shoots
						if (((iEnemyScreenPos.iY % 40) == 0) && (iEnemyScreenPos.iY >= 0))
							{
							Shoot(iEnemyScreenPos);
							}

						iEnemyAnim->SetEnemyScreenPos(iEnemyScreenPos);
						//CreateEnemiesToFight(aRect, aCallerGc);
						break;
						}
					case EBoss:
						{
						if (iEnemyScreenPos.iY <= 60)
							{		
							iEnemyScreenPos.iY = iEnemyScreenPos.iY + iEnemyAnim->GetEnemySpeed();
							}
						else
							{
							iEnemyScreenPos.iY = iEnemyScreenPos.iY;
							}
						//test of enemy's shoots
						if (((iEnemyScreenPos.iX % 5) == 0) && (iEnemyScreenPos.iY >= 0))
							{
							Shoot(iEnemyScreenPos);
							}

						iEnemyAnim->SetEnemyScreenPos(iEnemyScreenPos);
						//CreateEnemiesToFight(aRect, aCallerGc);
						break;
						}
					}

					CreateEnemiesToFight(aRect, aCallerGc);
				}
			}
		}
	}



////////////////////////////////////////////////////////////////////////

void CTopGunEngine::Restart(RDirectScreenAccess::TTerminationReasons /*aReason*/)
	{
	// Restart display
	// Note that this will result in the clipping region being updated
	// so that menus, overlaying dialogs, etc. will not be drawn over
	SetupDirectScreenAccessL();
	if(iPaused)
		{
		if(iGameDawingArea == iRegion->BoundingRect())
			{
			iPaused = EFalse;	
			if(!iGameTimer->IsActive())
				{
				iGameTimer->Restart();
				iSndMixer->Resume();
				}
			}
		else
			{
			PauseFrame();
			}
		}
	else
		{
		if(!iGameTimer->IsActive())
			{
			iGameTimer->Restart();
			iSndMixer->Resume();
			}
		}
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::AbortNow(RDirectScreenAccess::TTerminationReasons /*aReason*/)
	{
	// Cancel timer and display
	iDirectScreenAccess->Cancel();
	iPaused = ETrue;
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::PauseFrame()
	{
	// Force screen update: this is required for WINS, but may
	// not be for all hardware:
	iDirectScreenAccess->ScreenDevice()->Update();

	// and draw from unchanged offscreen bitmap
	iGc->BitBlt(TPoint(0,0), &(iDoubleBufferedArea->GetDoubleBufferedAreaBitmap()));

	iClient.Flush();		
	}

////////////////////////////////////////////////////////////////////////

TInt CTopGunEngine::TempNumberGen(TInt aMaxNumber)
	{
	TTime time; 
	TRequestStatus timerStatus;
	time.HomeTime();
	TInt64 seed=time.Int64();
	TInt random=Math::Rand( seed );
	return random%aMaxNumber;
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::CreateEnemiesToFight(const TRect& aRect,CFbsBitGc* aCallerGc) const
	{
	iEnemyAnim->RenderFrameCentered(iEnemyFrame,iEnemyScreenPos,aRect,aCallerGc);
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::RenderScoreBar(const TRect& aRect,CFbsBitGc* aCallerGc) const
	{
	iScoreBarAnim->RenderFrameCentered(iUtilsFrame,TPoint(89,12),aRect,aCallerGc);
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::RenderGameOver(const TRect& aRect,CFbsBitGc* aCallerGc) const
	{
	//display game over
	if (iGameOver)
		{
		iGameOverAnim->RenderFrameCentered(iUtilsFrame,TPoint(87,109),aRect,aCallerGc);
		iContainer.InformAboutGameOver();
		iLevelLoader.SetFirstLevel();
		iLevelLoader.AddScore(iScores);
		iLevelLoader.EnterHighScore(ETrue);
		}
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::RenderPauseGame(const TRect& aRect,CFbsBitGc* aCallerGc) const
	{
	//display game over
	//if (iPaused)
	//	{
		iPauseGameAnim->RenderFrameCentered(iUtilsFrame,TPoint(87,109),aRect,aCallerGc);
		//iContainer.InformAboutGameOver();
		//iLevelLoader.SetFirstLevel();
	//	}
	}


////////////////////////////////////////////////////////////////////////

void CTopGunEngine::DestroyEnemy(TPoint aPos)
	{
	//iEnemyAnim->SetEnemyScreenPos(iEnemyAnim->GetEnemyScreenPosOrg());
	iEnemyAnim->SetEnemyScreenPos(aPos);
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::DestroyEnemy()
	{
	iEnemyAnim->SetEnemyScreenPos(iEnemyAnim->GetEnemyScreenPosOrg());
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::CheckShipCollision(const TRect& /*aRect*/,CFbsBitGc* /*aCallerGc*/)
	{
	for (TInt i = 0; i < iEnemyIndexesArray.Count();i++)
		{	
		iEnemyAnim = iEnemyArray[iEnemyIndexesArray[i]];
		if(iEnemyAnim != NULL)
			{
			iEnemyScreenPos = iEnemyAnim->GetEnemyScreenPos();
			
			TPoint stCorner(iEnemyScreenPos.iX , iEnemyScreenPos.iY);
			TPoint ndCorner(iEnemyScreenPos.iX+32 , iEnemyScreenPos.iY+32);
			TRect enemyArea (stCorner, ndCorner);

			TPoint stShipCorner(iShipScreenPos.iX + 11, iShipScreenPos.iY);
			TPoint ndShipCorner(iShipScreenPos.iX + 21, iShipScreenPos.iY);

			//check collision
			if (enemyArea.Contains(stShipCorner) || enemyArea.Contains(ndShipCorner))
				{
				
				//destroy the ship
				iSndMixer->Play( iExplosionSound, 7 , 11025, iSfxVol );
				
				if(iLevelLoader.NumberOfLives() > 0)
						{
						iLevelLoader.DecreaseNumberOfLives();
						iNumberOfLives = iLevelLoader.NumberOfLives()+1;
						iExplosion = ETrue;
						iShipPlaceOfExplosion = stShipCorner;
						}
					else
						{
						iGameOver = ETrue;
						iNumberOfLives = 0;
						iShipVelocity.iY = 0;
						iExplosion = ETrue;
						iShipPlaceOfExplosion = stShipCorner;
						iShipScreenPos = TPoint(-32,-32);
						}
	
				//here fight boss
				if (iFightBoss)
					{
					iFightBoss = EFalse;
					
					if (iEnemyAnim->EnemyHit())
						{
						iEnemyExplosion = ETrue;
						iEnemyPlaceOfExplosion = iEnemyScreenPos;
						DestroyEnemy();
						iEnemyIndexesArray.Remove(i);
						iEnemyIndexesArray.Compress();
						}

					if(iLevelLoader.NumberOfLives() > 0)
						{
						iLevelLoader.DecreaseNumberOfLives();
						iNumberOfLives = iLevelLoader.NumberOfLives()+1;
						iExplosion = ETrue;
						iShipPlaceOfExplosion = stShipCorner;
						}
					else
						{
						iGameOver = ETrue;
						iNumberOfLives = 0;
						iShipVelocity.iY = 0;
						iExplosion = ETrue;
						iShipPlaceOfExplosion = stShipCorner;
						iShipScreenPos = TPoint(-32,-32);
						}
					}
				else
					{
					//destroy coliding enemy
					iEnemyExplosion = ETrue;
					iEnemyPlaceOfExplosion = iEnemyScreenPos;
					DestroyEnemy();
					iEnemyIndexesArray.Remove(i);
					iEnemyIndexesArray.Compress();
					}
				}
			}
		}
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::DrawGameOver(CFbsBitGc *aCallerGc)
	{
	//test
	iShipScreenPos = TPoint(-32,-32);

	CFont* font = (CFont*)LatinBold19(); //font for game over
	aCallerGc->UseFont(font);
	aCallerGc->SetPenColor(KRgbRed);
	aCallerGc->SetBrushColor(KRgbRed);
	TRect rect(0,0,176,208);
	TPoint gameOverPos;
	gameOverPos	= rect.Center();
	gameOverPos.iX -= 30;
	aCallerGc->DrawText(_L("Game Over"), gameOverPos);
	aCallerGc->DiscardFont();
	}


////////////////////////////////////////////////////////////////////////

void CTopGunEngine::DrawScores(CFbsBitGc *aCallerGc)
	{
	CFont* font = (CFont*)NumberPlain5(); //font for game over
	aCallerGc->UseFont(font);
	aCallerGc->SetPenColor(KRgbBlack);
	aCallerGc->SetBrushColor(KRgbBlack);
	TRect rect(0,0,176,208);
	TPoint scorePos(56,10);
	TBuf<128> scoresBuf;
	scoresBuf.Format(_L("%d"),iScores);
	aCallerGc->DrawText(scoresBuf, scorePos);
	aCallerGc->DiscardFont();
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::DrawNumberOfLives(CFbsBitGc *aCallerGc)
	{
	CFont* font = (CFont*)NumberPlain5(); //font for game over
	aCallerGc->UseFont(font);
	aCallerGc->SetPenColor(KRgbBlack);
	aCallerGc->SetBrushColor(KRgbBlack);
	TRect rect(0,0,176,208);
	TPoint livesPos(25,10);
	TBuf<128> livesBuf;
	livesBuf.Format(_L("%d"),iNumberOfLives);
	aCallerGc->DrawText(livesBuf, livesPos);
	aCallerGc->DiscardFont();
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::DrawPercentageOfDestroyedEnemies(CFbsBitGc *aCallerGc)
	{
	TRgb bad(147,4,18);
	TRgb good(25,103,10);
	
	CFont* font = (CFont*)NumberPlain5(); //font for game over
	aCallerGc->UseFont(font);
	
	TInt percentageOfDestroyedShips = (TInt)((iDestroyedShips/iAllShips) * 100);

	if (percentageOfDestroyedShips >= iLevelLoader.DifficultyLevel())
		{
		aCallerGc->SetPenColor(good);
		aCallerGc->SetBrushColor(good);
		}
	else
		{
		aCallerGc->SetPenColor(bad);
		aCallerGc->SetBrushColor(bad);
		}
	
	TRect rect(0,0,176,208);
	TPoint podePos(105,10);
	TBuf<128> podeBuf;
	
	podeBuf.Format(_L("%d"),percentageOfDestroyedShips);
	aCallerGc->DrawText(podeBuf, podePos);
	aCallerGc->DiscardFont();
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::Shoot(TPoint aPos)
	{
	if (iBulletArray.Count() < 16)
		{
		if (iEnemyAnim->EnemyType() == EBoss)
			{
			TInt orientation = TempNumberGen(2);
			TPoint adjustBulletPos(0x00000000,0x00000005);
			TPoint bulletStartPos = TPoint(aPos.iX - adjustBulletPos.iX, aPos.iY + adjustBulletPos.iY);
			//iEnemyBulletArray.Append(CBullet::NewL(bulletStartPos, iWindow,_L("EnemyBullet.mbm")));
			iEnemyBulletArray.Append(CBulletAnim::NewL(iImageFactory.EnemyBulletAnim(),bulletStartPos,iBulletSpeed,TSize(3,16),(TBulletOrientation)orientation));
			iSndMixer->Play( iShotSound, 6, 11025, iSfxVol );
			}
		else
			{
			TPoint adjustBulletPos(0x00000000,0x00000005);
			TPoint bulletStartPos = TPoint(aPos.iX - adjustBulletPos.iX, aPos.iY + adjustBulletPos.iY);
			//iEnemyBulletArray.Append(CBullet::NewL(bulletStartPos, iWindow,_L("EnemyBullet.mbm")));
			iEnemyBulletArray.Append(CBulletAnim::NewL(iImageFactory.EnemyBulletAnim(),bulletStartPos,iBulletSpeed,TSize(3,16),EMiddleO));
			iSndMixer->Play( iShotSound, 6, 11025, iSfxVol );
			}
		}
	else
		{
		iEnemyBulletArray.Reset();
		}
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::RenderBullets(const TRect& aRect,CFbsBitGc* aCallerGc) const
	{
	iBullet->RenderFrameCentered(iBulletFrame,iBulletScreenPos,aRect,aCallerGc);
	}

////////////////////////////////////////////////////////////////////////

/*
void CTopGunEngine::RenderEnemyBullets(const TRect& aRect,CFbsBitGc* aCallerGc) const
	{
	iEnemyBullet->RenderFrameCentered(iEnemyBulletFrame,iEnemyBulletScreenPos,aRect,aCallerGc);
	}
*/

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::CreateBullets(const TRect& aRect,CFbsBitGc* aCallerGc)
	{
	TInt i = 0;

	for (i = 0; i < iBulletArray.Count();i++)
		{	
		iBullet = iBulletArray[i];
		if(iBullet != NULL)
			{
			iBulletScreenPos = iBullet->GetBulletScreenPos();
			
			if (iBulletScreenPos.iY >= 248)
				{
				//DestroyBullet();
				
				CBulletAnim* bullet = iBulletArray[i];
				delete bullet;
				bullet = NULL;
				iBulletArray.Remove(i);
				//iBulletArray.Compress();
				}
			else
				{
				iBulletScreenPos.iY = iBulletScreenPos.iY - iBullet->GetBulletSpeed();
				iBullet->SetBulletScreenPos(iBulletScreenPos);
				RenderBullets(aRect, aCallerGc);
				}
			}
		}
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::CreateEnemyBullets(const TRect& aRect,CFbsBitGc* aCallerGc)
	{
	TInt i = 0;

	for (i = 0; i < iEnemyBulletArray.Count();i++)
		{	
		iBullet = iEnemyBulletArray[i];
		if(iBullet != NULL)
			{
			iBulletScreenPos = iBullet->GetBulletScreenPos();
			
			if (iBulletScreenPos.iY >= 248)
				{
				//DestroyBullet();
				CBulletAnim* enemyBullet = iEnemyBulletArray[i];
				delete enemyBullet;
				enemyBullet = NULL;
				iEnemyBulletArray.Remove(i);
				//iEnemyBulletArray.Compress();
				}
			else
				{
				if (iEnemyAnim->EnemyType() == EBoss)
					{
				    switch(iBullet->BulletOrientation())
						{
					case ERightO:
						iBulletScreenPos.iY = iBulletScreenPos.iY + iBullet->GetBulletSpeed();
						iBulletScreenPos.iX = iBulletScreenPos.iX + 3;
						iBullet->SetBulletScreenPos(iBulletScreenPos);
						RenderBullets(aRect, aCallerGc);
						break;
					case ELeftO:
						iBulletScreenPos.iY = iBulletScreenPos.iY + iBullet->GetBulletSpeed();
						iBulletScreenPos.iX = iBulletScreenPos.iX - 3;
						iBullet->SetBulletScreenPos(iBulletScreenPos);
						RenderBullets(aRect, aCallerGc);
						break;
					default:
						iBulletScreenPos.iY = iBulletScreenPos.iY + iBullet->GetBulletSpeed();
						iBullet->SetBulletScreenPos(iBulletScreenPos);
						RenderBullets(aRect, aCallerGc);
						break;
						};
					}
				else
					{
					iBulletScreenPos.iY = iBulletScreenPos.iY + iBullet->GetBulletSpeed();
					iBullet->SetBulletScreenPos(iBulletScreenPos);
					RenderBullets(aRect, aCallerGc);
					}
				}
			}
		}
	/*
	TInt i = 0;

	for (i = 0; i < iEnemyBulletArray.Count();i++)
		{	
		iEnemyBullet = iEnemyBulletArray[i];
		if(iEnemyBullet != NULL)
			{
			iEnemyBulletScreenPos = iEnemyBullet->GetBulletScreenPos();
			
			if (iEnemyBulletScreenPos.iY >= 248)
				{
				//DestroyBullet();
				iEnemyBulletArray.Remove(i);
				iEnemyBulletArray.Compress();
				}
			else
				{
				iEnemyBulletScreenPos.iY = iEnemyBulletScreenPos.iY + iEnemyBullet->GetBulletSpeed();
				iEnemyBullet->SetBulletScreenPos(iEnemyBulletScreenPos);
				RenderEnemyBullets(aRect, aCallerGc);
				}
			//delete iEnemyBullet;
			//iEnemyBullet = NULL;
			}
		}
	*/
	}

////////////////////////////////////////////////////////////////////////

void CTopGunEngine::SetVolume(TInt aSfxVol,TInt aAudioVol)
	{
	 switch ( aSfxVol )
        {
        case 0:
            iSfxVol = 0;
            break;
        case 25:
            iSfxVol = 64;
            break;
		case 75:
            iSfxVol = 192;
            break;
		case 100:
            iSfxVol = 256;
            break;
        default:
            iSfxVol = 32;
            break;
        }
	 switch ( aAudioVol )
        {
        case 0:
            iAudioVol = 0;
            break;
        case 25:
            iAudioVol = 64;
            break;
		case 75:
            iAudioVol = 192;
            break;
		case 100:
            iAudioVol = 256;
            break;
        default:
            iAudioVol = 32;
            break;
        }
	}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

