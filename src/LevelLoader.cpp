////////////////////////////////////////////////////////////////////////
//
// LevelLoader.cpp
//
// Copyright (c) 2004 Il Padrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////
#include "LevelLoader.h"

//System includes
#include <e32base.h>
//User includes

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CLevelLoader* CLevelLoader::NewLC()
	{
	CLevelLoader* self = new (ELeave) CLevelLoader();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

////////////////////////////////////////////////////////////////////////

CLevelLoader* CLevelLoader::NewL()
	{
	CLevelLoader* self = CLevelLoader::NewLC();
	CleanupStack::Pop();
	return self;
	}

////////////////////////////////////////////////////////////////////////

CLevelLoader::CLevelLoader(): iLevelNumber(FIRST_LEVEL), 
iMaxLevelNumber(MAX_LEVELS), iDifficultyLevel(ENormal), iScores(0), iEnterHighScore(EFalse), iDifficultyLevelPercentage(15), iTIntDifficultyLevelPercentage(15), iLives(2)
	{
	}

////////////////////////////////////////////////////////////////////////

CLevelLoader::~CLevelLoader()
	{
	}

////////////////////////////////////////////////////////////////////////
	
void CLevelLoader::ConstructL()
	{
	}

////////////////////////////////////////////////////////////////////////

TUint CLevelLoader::LevelNumber()
	{
	return iLevelNumber;
	}

////////////////////////////////////////////////////////////////////////

void CLevelLoader::SetFirstLevel()
	{
	iLevelNumber = FIRST_LEVEL;
	}

////////////////////////////////////////////////////////////////////////

TInt CLevelLoader::SetLevelNumber(TUint aLevelNumber)
	{
	TInt err = KErrNone;
	if (iLevelNumber < iMaxLevelNumber)
		{
		iLevelNumber = aLevelNumber;	
		}
	else
		{
		err = KErrNotFound;
		}	
	return err;
	}

////////////////////////////////////////////////////////////////////////

TInt CLevelLoader::NextLevel()
	{
	TInt err = KErrNone;
	if (iLevelNumber < iMaxLevelNumber)
		{
		iLevelNumber++;
		iContinueLevelNumber++;
		}
	else
		{
		iDifficultyLevelPercentage = 15;
		iTIntDifficultyLevelPercentage = 15;
		iLevelNumber = FIRST_LEVEL;
		ResetScore();
		err = KErrNotFound;
		}
	return err;
	}

////////////////////////////////////////////////////////////////////////

void CLevelLoader::IncreaseDifficultyLevel()
	{
	iContinueDifficultyLevelPercentage = iDifficultyLevelPercentage = iDifficultyLevelPercentage + 15;
	iContinueTIntDifficultyLevelPercentage = iTIntDifficultyLevelPercentage = iTIntDifficultyLevelPercentage + 15;
	}

////////////////////////////////////////////////////////////////////////

TBool CLevelLoader::IsLevelCompleted(TReal aPercentageOfDestroyedShips)
	{
	TBool result = EFalse;
	if (aPercentageOfDestroyedShips >= iDifficultyLevelPercentage)
		{
		result = ETrue;
		}
	else
		{
		ResetScore();
		}
	return result;
	}

////////////////////////////////////////////////////////////////////////

TInt CLevelLoader::DifficultyLevel()
	{
	return iTIntDifficultyLevelPercentage;
	}

////////////////////////////////////////////////////////////////////////

void CLevelLoader::SetDifficultyLevel(TDifficultyLevel aDifficultyLevel)
	{
	iDifficultyLevel = aDifficultyLevel;
	}

////////////////////////////////////////////////////////////////////////

void CLevelLoader::AddScore(TInt aScore)
	{
	if (aScore != 0)
		{
 		iScores = aScore;
		}
	}

////////////////////////////////////////////////////////////////////////

TInt CLevelLoader::GetScore()
	{
	return iScores;
	}

////////////////////////////////////////////////////////////////////////

void CLevelLoader::ResetScore()
	{
	iLives = 2;
	iScores = 0;
	iDifficultyLevelPercentage = 15;
	iTIntDifficultyLevelPercentage = 15;
	}

////////////////////////////////////////////////////////////////////////

void CLevelLoader::ContinueAfterGameOver(TBool aContinue)
	{
	if (aContinue)
		{
		if (iContinueLevelNumber == FIRST_LEVEL)
			{
			iLives = 2;
			iScores = 0;
			iDifficultyLevelPercentage = 15;
			iTIntDifficultyLevelPercentage = 15;
			}
		else
			{
			iLives = 1;
			iScores = 0;
			iDifficultyLevelPercentage = iContinueDifficultyLevelPercentage;
			iTIntDifficultyLevelPercentage = iContinueTIntDifficultyLevelPercentage;
			iLevelNumber = iContinueLevelNumber;
			}
		}
	else
		{
		iContinueLevelNumber = FIRST_LEVEL;
		iContinueDifficultyLevelPercentage = 15;
		iContinueTIntDifficultyLevelPercentage = 15;
		}
	}

////////////////////////////////////////////////////////////////////////

void CLevelLoader::EnterHighScore(TBool aEnterHighScore)
	{
	iEnterHighScore = aEnterHighScore;
	}

////////////////////////////////////////////////////////////////////////

TBool CLevelLoader::GetEnterHighScore()
	{
	return iEnterHighScore;
	}

////////////////////////////////////////////////////////////////////////

TInt CLevelLoader::NumberOfLives()
	{
	return iLives;
	}

////////////////////////////////////////////////////////////////////////

void CLevelLoader::SetNumberOfLives(TInt aLives)
	{
	iLives = aLives;
	}

////////////////////////////////////////////////////////////////////////

void CLevelLoader::DecreaseNumberOfLives()
	{
	if(iLives > 0)
		{
		iLives--;
		}
	}

////////////////////////////////////////////////////////////////////////

void CLevelLoader::IncreaseNumberOfLives()
	{
	iLives++;
	}

////////////////////////////////////////////////////////////////////////

//End of file