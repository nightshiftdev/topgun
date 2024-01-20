/**
 * This class manages the game images
 */

#ifndef CIMAGEFACTORY_H
#define CIMAGEFACTORY_H

#include <e32base.h>

//consts
#define LEVEL_FACTOR 7

class CEikonEnv;
class CFbsBitmap;
class CLevelLoader;

class CImageFactory : public CBase
    {
    public:
        static CImageFactory* NewL(CEikonEnv& aEikonEnv, const TDesC& aImageFile, CLevelLoader& aLevelLoader);
        static CImageFactory* NewLC(CEikonEnv& aEikonEnv,const TDesC& aImageFile, CLevelLoader& aLevelLoader);
        ~CImageFactory();

		CFbsBitmap& EnemyBulletAnim() const;
		CFbsBitmap& BulletAnim() const;
        CFbsBitmap& ShipAnim() const;
		//enemies
		CFbsBitmap& EnemyAnim2() const;
		CFbsBitmap& EnemyAnim3() const;
		CFbsBitmap& EnemyAnim4() const;
		CFbsBitmap& EnemyAnim6() const;
		CFbsBitmap& EnemyAnim8() const;

        CFbsBitmap& FireAnim() const;
		CFbsBitmap& ExplosionAnim() const;
        CFbsBitmap& TileImage() const;
        CFbsBitmap& TileMask() const;

		CFbsBitmap& GameOver() const;
		CFbsBitmap& Pause() const;
        CFbsBitmap& ScoreBar() const;

   private:
        CImageFactory(CEikonEnv& aEikonEnv, CLevelLoader& aLevelLoader);
        void ConstructL(const TDesC& aImageFileName);
        void LoadImagesL();

		CFbsBitmap* iEnemyBulletAnim;
		CFbsBitmap* iBulletAnim;
		//enemies
		CFbsBitmap* iEnemyAnim2;
		CFbsBitmap* iEnemyAnim3;
		CFbsBitmap* iEnemyAnim4;
		CFbsBitmap* iEnemyAnim6;
		CFbsBitmap* iEnemyAnim8;

        CFbsBitmap* iShipAnim;
        CFbsBitmap* iFireAnim;
		CFbsBitmap* iExplosionAnim;
        CFbsBitmap* iTileImage;
        CFbsBitmap* iTileMask;
		//utils
		CFbsBitmap* iGameOver;
        CFbsBitmap* iPause;
		CFbsBitmap* iScoreBar;

        CEikonEnv& iEikEnv;
        HBufC* iMbmFile;
		CLevelLoader& iLevelLoader;
    };
#endif //IMAGEFACTORY_H