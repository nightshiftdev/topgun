////////////////////////////////////////////////////////////////////////
//
// TopGunEngine.h
//
// Copyright (c) 2004 IlPadrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef __TOPGUNENGINE_H__
#define __TOPGUNENGINE_H__

#include <w32std.h>
#include <e32std.h> 
#include <mdaaudiooutputstream.h>
#include <mda\common\audio.h>
#include "GameTimerObserver.h"
#include "TSample.h"
#include "EnemyType.h"
#include "LevelLoader.h"

class TKeyHandler;
class CImageFactory;
class CDoubleBufferedArea;
class CFbsBitmap;
class CGameTimer;

class CTilemap;
class MTileset;
class CShipAnim;
class CEnemyAnim;
class CExplosionAnim; 
class CFireAnim; 
class CTopGunLeafTileset;
class CTopGunNodeTileset;
class CParticleSystem;
class TVectorFP;
class CBulletAnim;
class CEnemy;
class CSndMixer;
class CPosition;
class CUtilsAnim;
class CTOPGUNContainer2;
class CLevelLoader;

class CTopGunEngine : public CBase, public MGameTimerObserver, public MDirectScreenAccess
    {
    public:

        static CTopGunEngine* NewL(CTOPGUNContainer2& aContainer,
								  RWsSession& aClient, 
								  CWsScreenDevice& aScreenDevice, 
								  RWindow& aWindow,
								  TKeyHandler& aKeyHandler,
								  CLevelLoader& aLevelLoader,
								  CImageFactory& aImageFactory, 
								  const TSize& aSize,
								  CSndMixer* aSndMixer,
								  TSample& aMusic,
							      TSample& aShotSound,
							      TSample& aExplosionSound,
							      TInt aAudioVol,
							      TInt aSfxVol);
        static CTopGunEngine* NewLC(CTOPGUNContainer2& aContainer,
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
							       TInt aSfxVol);
        ~CTopGunEngine();

		void StartGameL();
		void StartFirstGameL();
		void StopGame();

		// Are we in play
		inline TBool Playing(){return iPlaying;}

	   /**
	    * @function SetVolume
        *
	    * @discussion Sets current volume options for the engine.
	    * @param aSfxVol sfx volume value.
		* @param aAudioVol audio volume value.
	    * @result void.
	    */
		void SetVolume(TInt aSfxVol, TInt aAudioVol);

	private:
		TInt DoGameFrame();	
		void PauseFrame();
		void LoadMap1();
		void LoadMap2();
		void LoadMap3();
		void LoadMap4();
		void LoadMap5();

	private:
		// MDirectScreenAccess 
		void Restart(RDirectScreenAccess::TTerminationReasons aReason);
		void AbortNow(RDirectScreenAccess::TTerminationReasons aReason);

		void SetupDirectScreenAccessL();
        
    private:
        CTopGunEngine(CTOPGUNContainer2& aContainer, RWsSession& aClient, CWsScreenDevice& aScreenDevice, RWindow& aWindow,TKeyHandler& aKeyHandler, CLevelLoader& aLevelLoader,CImageFactory& aImageFactory, const TSize& aSize);
        void ConstructL(CSndMixer*	aSndMixer,
					    TSample& aMusic,
						TSample& aShotSound,
						TSample& aExplosionSound,
						TInt aAudioVol,
						TInt aSfxVol);

		void ConstructSimpleMapL();
		void ConstructCompoundMapL();
		void ConstructShipL();
		void ConstructEnemyL();
		void CreateEnemiesToFight(const TRect& aRect,CFbsBitGc* aCallerGc) const;
		void ConstructExplosionL(const TPoint &aPlaceOfExplosion,const TRect& aRect,CFbsBitGc* aCallerGc);
		void ConstructUtilsL();
		void DestroyEnemy(TPoint aPos);
		void DestroyEnemy();
		void Shoot(TPoint aPos);
		void UpdateEnemyBullets();

		void Simulate();
		void UpdatePosition();
		void UpdateBullets();
		void Render(const TRect& aRect,CFbsBitGc* aCallerGc) const;
		void RenderScoreBar(const TRect& aRect,CFbsBitGc* aCallerGc) const;
		void RenderGameOver(const TRect& aRect,CFbsBitGc* aCallerGc) const;
		void RenderPauseGame(const TRect& aRect,CFbsBitGc* aCallerGc) const;
		void RenderEnemies(const TRect& aRect,CFbsBitGc* aCallerGc);
		void CheckShipCollision(const TRect& aRect,CFbsBitGc* aCallerGc);
		void DrawGameOver(CFbsBitGc *aCallerGc);
		void DrawNumberOfLives(CFbsBitGc *aCallerGc);
		void DrawPercentageOfDestroyedEnemies(CFbsBitGc *aCallerGc);
		void DrawScores(CFbsBitGc *aCallerGc);
		TInt TempNumberGen(TInt aMaxNumber);

		void RenderBullets(const TRect& aRect,CFbsBitGc* aCallerGc) const;
		//void RenderEnemyBullets(const TRect& aRect,CFbsBitGc* aCallerGc) const;
		void CreateBullets(const TRect& aRect,CFbsBitGc* aCallerGc);
		void CreateEnemyBullets(const TRect& aRect,CFbsBitGc* aCallerGc);
    private:


		//Timer
		CGameTimer* iGameTimer;

		// Window server handling
		RWsSession& iClient;
		CWsScreenDevice& iScreenDevice;
		RWindow& iWindow;

		// Direct Screen Access
		CDirectScreenAccess* iDirectScreenAccess;
		CFbsBitGc* iGc;
		RRegion* iRegion;        
		
		// Offscreen area for double buffering
		CDoubleBufferedArea* iDoubleBufferedArea;
		CFbsBitGc* iOffscreenContext;
		
		//Game stuff
        TKeyHandler& iKeyHandler;
		CImageFactory& iImageFactory;
		
		TPoint iShipScreenPos;
		TPoint iShipScreenPosUp4;
		TPoint iShipWorldPos;
		TPoint iShipVelocity;

		CTilemap* iMap;
		CTopGunLeafTileset* iLeafFactory;
		CTopGunNodeTileset* iNodeFactory;
		TRect  iShipBounds;
		TPoint iMapWindowTopLeft;

		TInt iShipFrame;
		CShipAnim* iShipAnim;
		CFireAnim* iFireAnim;
		//CParticleSystem* iEngineExhaust;
		TVectorFP* iShipVectors;

		TInt iThrust;
		TInt iGravity;
		TInt iDragRatio;
		TInt iDragDownshift;

		TSize iSize;

		TBool iDropRenderFrames;

		TBool iPlaying;
		TBool iPaused;
		TRect iGameDawingArea;
		TInt iFrameCounter;

		//Ship's position
		TInt iShipX;
		TInt iShipY;

		//Ship's speed
		TInt iShipSpeed;

		//Enemy's position
		TInt iEnemyXPos;
		TInt iEnemyYPos;

		TInt iEnemyFrame;
		TVectorFP* iEnemyVectors;
		CEnemyAnim* iEnemyAnim;
		TPoint iEnemyScreenPos;
		TPoint iEnemyScreenPosUp4;
		TPoint iEnemyWorldPos;
		TPoint iEnemyVelocity;
		TInt iEnemySpeed;
		TInt iEnemyEnergy;
		RPointerArray< CEnemyAnim > iEnemyArray;
		RArray <TInt> iEnemyIndexesArray; 
		TInt iWhenCreateNewEnemies;
		TInt iLastEnemyIndex;

		TInt iNumberOfEnemies;
		TInt iIndexOfEnemies;

		//Bullets
		TInt iBulletFrame;
		TInt iEnemyBulletFrame;

		CBulletAnim* iBullet;
		RPointerArray< CBulletAnim > iBulletArray;
		//Bullet's speed
		TInt iBulletSpeed;
		TPoint iBulletScreenPos;

		TPoint iEnemyBulletScreenPos;

		//Enemies Bullets
		//CBulletAnim* iEnemyBullet;
		RPointerArray< CBulletAnim > iEnemyBulletArray;
		//Bullet's speed, maybe not needed for enemies, we can use general iBulletSpeed
		//TInt iBulletSpeed;

		//Enemies
		//CEnemy* iEnemy;

		TBool iGameOver;
		TBool iBlockKeys;

		//Explosion
		TBool iExplosion;
		TBool iEnemyExplosion;
		CExplosionAnim* iExplosionAnim;
		TPoint iExplosionScreenPos;
		TInt iExplosionFrame;
		//CExplosionAnim *iExplosion;
		TPoint iShipPlaceOfExplosion;
		TPoint iEnemyPlaceOfExplosion;

		//Sound
		// Sound mixer
		CSndMixer*	iSndMixer;			
		//sampled music
		TSample		iMusic;
		//shot sound
		TSample		iShotSound;
		//explosion sound
		TSample		iExplosionSound;
		
		/*! @var iAudioVal represents audio volume value */
		TInt iAudioVol;

		/*! @var iSfxVal represents SFX volume value */
		TInt iSfxVol;

		/*! @var iEnemyType represents type of the enemy,
				 variable decides about enemy's behavoiurs*/
		TEnemy iEnemyType;

		/*! @var iPosition help position variable variable */
		CPosition* iPosition;
		
		/*! @var iPositionArray holds possible enemy start positions */
		RPointerArray< CPosition > iPositionArray;

		/*! @var iBoss performs displaying boss on the screen */
		TBool iBoss;

		/*! @var iBossExplosion notyfies about explosion of the boss */
		TBool iBossExplosion;

		/*! @var iLevelEnd notyfies about end of the level */
		TBool iLevelEnd;

		/*! @var iBossExplosion displays Game Over bitmap */
		CUtilsAnim* iGameOverAnim;

		/*! @var iPauseGame displays Pause bitmap */
		CUtilsAnim* iPauseGameAnim;

		/*! @var iScoreBar displays Score Bar bitmap */
		CUtilsAnim* iScoreBarAnim;

		/*! @var iScores counts scores */
		TInt iScores; 
		
		/*! @var iUtilsFrame represents all utils frame */
		TInt iUtilsFrame;

		/*! @var iContainer connection to container class */
		CTOPGUNContainer2& iContainer;
		
		/*! @var iLevelLoader level loader, responssible for level loading */
		CLevelLoader& iLevelLoader;

		/*! @var iDestroyedShips */
		TReal iDestroyedShips;

		/*! @var iAllShips */
		TReal iAllShips;

		/*! @var iPercentageOfDestroyedShips */
		TReal iPercentageOfDestroyedShips;

		/*! @var iDifficultyLevel */
		TInt iDifficultyLevel;

		/*! @var iNumberOfLives */
		TInt iNumberOfLives;

		/*! @var iFightBoss */
		TBool iFightBoss;
    };
#endif