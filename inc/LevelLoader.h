////////////////////////////////////////////////////////////////////////
//
// LevelLoader.h
//
// Copyright (c) 2004 Il Padrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef __LEVELLOADER_H__
#define __LEVELLOADER_H__

#include <e32base.h>

//constants
#define FIRST_LEVEL 0
#define MAX_LEVELS 4

//system wide settings
enum TDifficultyLevel
	{
	EEasy = 1,
	ENormal = 3,
	EHard = 5,
	EMissionImpossible = 10
	};

//forward declarations
class CLevel;

class CLevelLoader : public CBase
    {
    public:

        static CLevelLoader* NewL();
        static CLevelLoader* NewLC();
        virtual ~CLevelLoader();

		TUint LevelNumber();
		void SetFirstLevel();
		TInt NextLevel();
		TInt SetLevelNumber(TUint aLevelNumber);
		void AddScore(TInt aScore);
		TInt GetScore();
		void ResetScore();

		TInt DifficultyLevel();
		void SetDifficultyLevel(TDifficultyLevel aDifficultyLevel);
		void EnterHighScore(TBool aEnterHighScore);
		TBool GetEnterHighScore();
		TBool IsLevelCompleted(TReal aPercentageOfDestroyedShips);
		void IncreaseDifficultyLevel();
		TInt NumberOfLives();
		void SetNumberOfLives(TInt aLives);
		void DecreaseNumberOfLives();
		void IncreaseNumberOfLives();
		void ContinueAfterGameOver(TBool aContinue);

	private:
		
		CLevelLoader();
        void ConstructL();
		//TBool LoadLevel();

    private:

		TUint iLevelNumber;
		TUint iMaxLevelNumber;
		TDifficultyLevel iDifficultyLevel;
		TInt iScores;
		TBool iEnterHighScore;
		TReal iDifficultyLevelPercentage;
		TInt iTIntDifficultyLevelPercentage;
		TInt iLives;

		TUint iContinueLevelNumber;
		TReal iContinueDifficultyLevelPercentage;
		TInt iContinueTIntDifficultyLevelPercentage;
		//most probably not needed
		//CLevel* iCurrentLevel;
    };
#endif