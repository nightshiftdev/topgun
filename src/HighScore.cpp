/*
* ============================================================================
*  Name     : CHighScoreItem from HighScore.cpp
*  Part of  : TopGun
*  Created  : 12/5/2004 by pawel.kijowski@gmail.com
*  Implementation notes: Holds high score item data.
*  Version  : 1.0 Beta
*  Copyright: ilPadrino Group
* ============================================================================
*/

#include <s32file.h>
#include <e32std.h>
#include "HighScore.h"

CHighScoreItem* CHighScoreItem::NewL()
    {
    CHighScoreItem* self = CHighScoreItem::NewLC();
    CleanupStack::Pop(self);
    return self;
    }

CHighScoreItem* CHighScoreItem::NewLC()
    {
    CHighScoreItem* self = new (ELeave) CHighScoreItem;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CHighScoreItem::CHighScoreItem()
	{
	// No implementation required
	}

CHighScoreItem::~CHighScoreItem()
	{
	delete iPlayerName;
	iPlayerName = NULL;
	}

void CHighScoreItem::ConstructL()
	{
	// No implementation required
	}

void CHighScoreItem::ExternalizeL(RWriteStream& aStream) const
	{
	if (iPlayerName)
		{
		aStream << *iPlayerName;
		}
	else
		{
		aStream << KNullDesC;
		}
	aStream.WriteInt32L(iScore);
	}

void CHighScoreItem::InternalizeL(RReadStream& aStream)
	{
	delete iPlayerName;
	iPlayerName = HBufC::NewL(aStream, 128);
	iScore = aStream.ReadInt32L();
	}

void CHighScoreItem::SetPlayerNameL(HBufC* aPlayerName)
	{
	delete iPlayerName;
	iPlayerName = aPlayerName;
	}

HBufC* CHighScoreItem::PlayerName() const
	{
	return iPlayerName;
	}

TUint CHighScoreItem::HighScore() const
	{
	return iScore;
	}

void CHighScoreItem::SetHighScore(TUint aHighScore)
	{
	iScore = aHighScore;
	}

TInt CHighScoreItem::CompareTop(const CHighScoreItem& aFirst,const CHighScoreItem& aSecond)
	{
	TInt ret = aFirst.iScore - aSecond.iScore;
	
	if (ret > 0)
		{
		return -1;
		}
	if (ret < 0)
		{
		return 1;
		}
	return 0;
	}

// End of file
