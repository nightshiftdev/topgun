/*
* ============================================================================
*  Name     : CGameSettings from GameSettings.cpp
*  Part of  : TopGun
*  Created  : 12/5/2004 by pawel.kijowski@gmail.com
*  Implementation notes: Holds high score item data.
*  Version  : 1.0 Beta
*  Copyright: ilPadrino Group
* ============================================================================
*/

#include <s32file.h>
#include <e32std.h>
#include "GameSettings.h"

CGameSettings* CGameSettings::NewL()
    {
    CGameSettings* self = CGameSettings::NewLC();
    CleanupStack::Pop(self);
    return self;
    }

CGameSettings* CGameSettings::NewLC()
    {
    CGameSettings* self = new (ELeave) CGameSettings;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CGameSettings::CGameSettings()
	{
	// No implementation required
	}

CGameSettings::~CGameSettings()
	{
	// No implementation required
	}

void CGameSettings::ConstructL()
	{
	// No implementation required
	}

void CGameSettings::ExternalizeL(RWriteStream& aStream) const
	{
	aStream.WriteInt32L(iAudioVol);
	aStream.WriteInt32L(iSfxVol);
	}

void CGameSettings::InternalizeL(RReadStream& aStream)
	{
	iAudioVol = aStream.ReadInt32L();
	iSfxVol = aStream.ReadInt32L();
	}

TInt CGameSettings::AudioVol() const
	{
	return iAudioVol;
	}

void CGameSettings::SetAudioVol(TInt aAudioVol)
	{
	iAudioVol = aAudioVol;
	}
	
TInt CGameSettings::SfxVol() const
	{
	return iSfxVol;
	}

void CGameSettings::SetSfxVol(TInt aSfxVol)
	{
	iSfxVol = aSfxVol;
	}	

TInt CGameSettings::CompareTop(const CGameSettings& aFirst,const CGameSettings& aSecond)
	{
	TInt ret = aFirst.iAudioVol - aSecond.iAudioVol;
	
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
