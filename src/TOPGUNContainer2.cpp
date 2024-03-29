/*
* ============================================================================
*  Name     : CTOPGUNContainer2 from TOPGUNContainer2.cpp
*  Part of  : TOPGUN
*  Created  : 5/9/2004 by pawel@wapice.com
*  Implementation notes:
*     Initial content was generated by Series 60 AppWizard.
*  Version  : 1.0
*  Copyright: IlPadrino group.
* ============================================================================
*/

// INCLUDE FILES
#include <eikenv.h>
//commands
#include <avkon.hrh>
#include <TOPGUN.rsg>
#include "topgun.hrh"
#include "TOPGUNContainer2.h"
#include "TOPGUNView2.h" 
#include "ImageFactory.h"
#include "TopGunEngine.h"
#include "LevelLoader.h"
//sound
#include "CSndMixer.h"
#include "CWavLoader.h"    


// Local Constants
_LIT (KImagesFilename,"z:\\resource\\apps\\topgun.mbm");

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CTOPGUNContainer2::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CTOPGUNContainer2::ConstructL(const TRect& aRect,
								   CSndMixer*	aSndMixer,
								   TSample& aMusic,
								   TSample& aShotSound,
								   TSample& aExplosionSound,
								   TInt aAudioVol,
								   TInt aSfxVol,
								   CLevelLoader* aLevelLoader)
    {
	CCoeEnv::Static()->AddForegroundObserverL(*this);
	//initial settings for the container
	iGameOver = EFalse;
	iLevelLoader = aLevelLoader;
	iPausePressed = EFalse;
	iSndMixer = aSndMixer;
	iMusic = aMusic;
	iShotSound = aShotSound;
	iExplosionSound = aExplosionSound;
	iAudioVol = aAudioVol;
	iSfxVol = aSfxVol;
    CreateWindowL();
	SetRect(aRect);
	iImageFactory = CImageFactory::NewL(*iEikonEnv, KImagesFilename, *iLevelLoader);  
	iEngine = CTopGunEngine::NewL(*this,
								 iEikonEnv->WsSession(), 
								 *(CCoeEnv::Static()->ScreenDevice()), 
								 Window(), 
								 iKeyHandler,
								 *iLevelLoader,
								 *iImageFactory, 
								 Rect().Size(),
								 iSndMixer,
								 iMusic,
								 iShotSound,
								 iExplosionSound,
								 iAudioVol,
								 iSfxVol);    
	ActivateL();
	iEngine->StartFirstGameL();
    }

// Destructor
CTOPGUNContainer2::~CTOPGUNContainer2()
    {
	CCoeEnv::Static()->RemoveForegroundObserver(*this);
	delete iImageFactory;
	delete iEngine;
    }

// ---------------------------------------------------------
// CTOPGUNContainer2::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CTOPGUNContainer2::SizeChanged()
    {
    }

// ---------------------------------------------------------
// CTOPGUNContainer2::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CTOPGUNContainer2::Draw(const TRect& /*aRect*/) const
    {
	// Clear the screen
    CWindowGc& gc = SystemGc();
    gc.Clear(Rect());
    }

// ---------------------------------------------------------
// CTOPGUNContainer2::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CTOPGUNContainer2::SwitchView(TInt aViewNumber) const
    {
	if(iView)
		{
		iView->SwitchView(aViewNumber);
		}
    }


// ---------------------------------------------------------
// CTOPGUNContainer2::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CTOPGUNContainer2::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }

void CTOPGUNContainer2::HandleGainingForeground()
	{
	//StartGameL();
	//User::After(100000);
	//User::WaitForAnyRequest();
	//iPausePressed = ETrue;
	//StopGame();
	}

void CTOPGUNContainer2::HandleLosingForeground()
	{
	//StopGame();
	if (iPausePressed)
		{
		iPausePressed = EFalse;
		StartGameL();
		}
	}

TKeyResponse CTOPGUNContainer2::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
	{
	TKeyResponse returnKey = EKeyWasNotConsumed;
	if (iGameOver)
		{
		iSndMixer->Stop(5);
		iLevelLoader->EnterHighScore(ETrue);
		SwitchView(8); //Mission failed screen
		}
	else
		{
		if(aType == EEventKey)
			{
			switch(aKeyEvent.iCode)
				{
				case 0x00000039: //9
					{
					SfxVolumeUp();
					returnKey = EKeyWasConsumed;
					break;
					}
				case 0x00000023://#
					{
					SfxVolumeDown();
					returnKey = EKeyWasConsumed;
					break;
					}
				case 0x00000038://8
					{
					AudioVolumeUp();
					returnKey = EKeyWasConsumed;
					break;
					}
				case 0x00000030://0
					{
					AudioVolumeDown();
					returnKey = EKeyWasConsumed;
					break;
					}
				case 0x0000f842://left option key
					{
					if (iPausePressed)
						{
						iPausePressed = EFalse;
						StartGameL();
						returnKey = EKeyWasConsumed;
						}
					else
						{
						iPausePressed = ETrue;
						StopGame();
						returnKey = EKeyWasConsumed;
						}
					// we still want the key event to be passed on so don't consume the key
					}
				case 0x0000f852://menu key
					{
					if (aKeyEvent.iScanCode == 0x000000b4)
						{
						/*
						iSndMixer->Pause();
						if (iPausePressed)
							{
							iPausePressed = EFalse;
							StartGameL();
							returnKey = EKeyWasConsumed;
							}
						*/
						}
					}
				}
			// we still want the key event to be passed on so don't consume the key
			}
		else if(aType == EEventKeyDown)
			{
			switch(aKeyEvent.iScanCode)
				{
				case EStdKeyUpArrow:
					iKeyHandler.UpPressed();
					returnKey = EKeyWasConsumed;
					break;
				case EStdKeyDownArrow:
					iKeyHandler.DownPressed();
					returnKey = EKeyWasConsumed;
					break;
				case EStdKeyLeftArrow:
					iKeyHandler.LeftPressed();
					returnKey = EKeyWasConsumed;
					break;
				case EStdKeyRightArrow:
					iKeyHandler.RightPressed();
					returnKey = EKeyWasConsumed;
					break;
				case 53:
				case EStdKeyNkp5:
				case EStdKeyDevice3:
				//case EKeyOK:
				//case EKeyEnter:
					iKeyHandler.PressedFire();
					returnKey = EKeyWasConsumed;
					break;
			default:
				break;
				}
			}
		else if(aType == EEventKeyUp)
			{
			switch(aKeyEvent.iScanCode)
				{
				case EStdKeyUpArrow:
					iKeyHandler.NoDirection();
					returnKey = EKeyWasConsumed;
					break;
				case EStdKeyDownArrow:
					iKeyHandler.NoDirection();
					returnKey = EKeyWasConsumed;
					break;
				case EStdKeyLeftArrow:
					iKeyHandler.NoDirection();
					returnKey = EKeyWasConsumed;
					break;
				case EStdKeyRightArrow:
					iKeyHandler.NoDirection();
					returnKey = EKeyWasConsumed;
					break;
				case 53:
				case EStdKeyNkp5:
				case EStdKeyDevice3:
				//case EKeyOK:
				//case EKeyEnter:
					iKeyHandler.FireReleased();
					returnKey = EKeyWasConsumed;
					break;
			default:
				break;
				}
			}
		}

	return returnKey;

	}

void CTOPGUNContainer2::AudioVolumeUp()
	{
	if (iAudioVol < 256)
		{
		iAudioVol = iAudioVol + 32;
		iSndMixer->Stop(5);
		iSndMixer->Play( iMusic, 5, 11025, iAudioVol );
	    //iEngine->SetVolume(iSfxVol,iAudioVol);
		}
	}

void CTOPGUNContainer2::AudioVolumeDown()
	{
	if (iAudioVol >= 32)
		{
		iAudioVol = iAudioVol - 32;
		iSndMixer->Stop(5);
		iSndMixer->Play( iMusic, 5, 11025, iAudioVol );
		//iEngine->SetVolume(iSfxVol,iAudioVol);
		}
	}

void CTOPGUNContainer2::SfxVolumeUp()
	{
	if (iSfxVol < 256)
		{
		iSfxVol = iSfxVol + 32;
		iEngine->SetVolume(iSfxVol,iAudioVol);
		}
	}

void CTOPGUNContainer2::SfxVolumeDown()
	{
	if (iSfxVol >= 32)
		{
		iSfxVol = iSfxVol - 32;
		iEngine->SetVolume(iSfxVol,iAudioVol);
		}
	}

void CTOPGUNContainer2::StartGameL()
	{
	iEngine->StartGameL();
	}

void CTOPGUNContainer2::StopGame()
	{
	iEngine->StopGame();
	}

TBool CTOPGUNContainer2::IsPlaying()
	{
	return iEngine->Playing();
	}

void CTOPGUNContainer2::SetView(CTOPGUNView2* aView)
	{
	iView = aView;
	}

void CTOPGUNContainer2::SetVolume(TInt aSfxVol,TInt aAudioVol)
	{
	iSfxVol = aSfxVol;
	iAudioVol = aAudioVol;
	iView->SetVolume(aSfxVol,aAudioVol);
	}

void CTOPGUNContainer2::InformAboutGameOver()
	{
	iGameOver = ETrue;
	}

// End of File  
