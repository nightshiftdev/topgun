/*
* ============================================================================
*  Name     : CTOPGUNContainer3 from TOPGUNContainer3.h
*  Part of  : TOPGUN
*  Created  : 5/9/2004 by pawel@wapice.com
*  Implementation notes:
*     Initial content was generated by Series 60 AppWizard.
*  Version  :
*  Copyright: P&P
* ============================================================================
*/

// INCLUDE FILES
//system
#include <eikenv.h> 
//bitmaps
#include <topgun.mbg>
// for double screen buffering  
#include <fbs.h>	
#include <bitdev.h>
#include <bitstd.h>
// for fonts
#include <gdi.h> 
#include <aknutils.h>
//commands
#include <TOPGUN.rsg>
#include "topgun.hrh"
//user
#include "bitmapmethods.h"
#include "TOPGUNContainer3.h"
#include "TOPGUNView3.h" 
//sound
#include "CSndMixer.h"
#include "CWavLoader.h" 

//CONSTANTS
_LIT (KMultiBitmapFilename,"z:\\resource\\apps\\topgun.mbm"); 
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CTOPGUNContainer3::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CTOPGUNContainer3::ConstructL(const TRect& aRect,
								   CSndMixer* aSndMixer,
								   TSample& aMusicSample,
								   TSample& aSelectSample,
								   TSample& aButtonSample,
								   TInt aAudioVol,
								   TInt aSfxVol)
    {
	//initial settings of the screen
	iSelectPos = 0;

    CreateWindowL();

	SetRect(aRect);

	iBackgroundImage = iEikonEnv->CreateBitmapL(KMultiBitmapFilename,EMbmTopgunOptions);	
	// Create the off screen bitmap and device / gc
    iOffScreenBitmap       = NBitmapMethods::CreateBitmapL(Rect().Size(),KColourDepth);
    iOffScreenBitmapDevice = NBitmapMethods::CreateBitmapDeviceL(*iOffScreenBitmap);
    iOffScreenBitmapGc     = NBitmapMethods::CreateGraphicsContextL(*iOffScreenBitmapDevice);
    ActivateL();
	
	iSndMixer = aSndMixer;
	iMusicSample = aMusicSample;
	iSelectSample = aSelectSample;
	iButtonSample = aButtonSample;
	iAudioVol = aAudioVol;
	iSfxVol = aSfxVol;
	iSndMixer->Resume();
	//iSndMixer->Play( iMusicSample, 1, 11025, iAudioVol );
    }

// Destructor
CTOPGUNContainer3::~CTOPGUNContainer3()
    {
	//iSndMixer->Stop(1);
	//iSndMixer->Stop(2);
	//iSndMixer->Stop(3);
	//iSndMixer->Stop(4);
	//iSndMixer->Pause();

	if (iOffScreenBitmapGc)
		{
		delete iOffScreenBitmapGc;
		iOffScreenBitmapGc = NULL;
		}

	if (iOffScreenBitmapDevice)
		{
		delete iOffScreenBitmapDevice;
		iOffScreenBitmapDevice = NULL;
		}	
	
	if (iOffScreenBitmap)
		{
		delete iOffScreenBitmap;
		iOffScreenBitmap = NULL;
		}

	if (iBackgroundImage)
		{
		delete iBackgroundImage;
		iBackgroundImage = NULL;
		}
    }

// ---------------------------------------------------------
// CTOPGUNContainer3::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CTOPGUNContainer3::SizeChanged()
    {
    // TODO: Add here control resize code etc.
    }

// ---------------------------------------------------------
// CTOPGUNContainer3::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CTOPGUNContainer3::CountComponentControls() const
    {
    return 0; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CTOPGUNContainer3::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CTOPGUNContainer3::ComponentControl(TInt /*aIndex*/) const
    {
	return NULL;
    }

// ---------------------------------------------------------
// CTOPGUNContainer3::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CTOPGUNContainer3::Draw(const TRect& /*aRect*/) const
    {
    //SystemGc().Clear(Rect());
	iOffScreenBitmapGc->BitBlt(TPoint(0,0),iBackgroundImage);
	DrawCaptions();
	SystemGc().BitBlt(Rect().iTl,iOffScreenBitmap);
    }

// ---------------------------------------------------------
// CTOPGUNContainer3::::DrawCaptions() const
// ---------------------------------------------------------
//
void CTOPGUNContainer3::DrawCaptions() const
    {
	CFont* font = (CFont*)LatinPlain12(); //font for scale
	iOffScreenBitmapGc->UseFont(font);

	TRgb active(0,0,0);
	TRgb inactive(111,110,108);

	TBuf<64> audioValBuf;
	TBuf<64> sfxValBuf;
	TBuf<64> audio;
	TBuf<64> sfx;
	TBuf<64> controls;
	TBuf<64> credits;

	audioValBuf.Format(_L("%d"),iAudioVol);
	sfxValBuf.Format(_L("%d"),iSfxVol);
	audio =    _L("a u d i o ");
	sfx   =    _L("s f x ");
	controls = _L("c o n t r o l s ");
	credits =  _L("c r e d i t s ");


	TPoint audioPosVal(112,120);
	TPoint sfxPosVal(112,132);
	TPoint audioPos(46,120);
	TPoint sfxPos(54,132);
	TPoint controlsPos(53,146);
	TPoint creditsPos(60,160);


	switch ( iSelectPos )
		{
		case 0:
			iOffScreenBitmapGc->SetPenColor(active);
			iOffScreenBitmapGc->SetBrushColor(active);
			iOffScreenBitmapGc->DrawText(audio,audioPos);
			iOffScreenBitmapGc->DrawText(audioValBuf,audioPosVal);
			iOffScreenBitmapGc->SetPenColor(inactive);
			iOffScreenBitmapGc->SetBrushColor(inactive);
			iOffScreenBitmapGc->DrawText(sfxValBuf,sfxPosVal);
			iOffScreenBitmapGc->DrawText(sfx, sfxPos);
			iOffScreenBitmapGc->DrawText(controls, controlsPos);
			iOffScreenBitmapGc->DrawText(credits, creditsPos);
			break;
		case 1:
			iOffScreenBitmapGc->SetPenColor(active);
			iOffScreenBitmapGc->SetBrushColor(active);
			iOffScreenBitmapGc->DrawText(sfx, sfxPos);
			iOffScreenBitmapGc->DrawText(sfxValBuf,sfxPosVal);
			iOffScreenBitmapGc->SetPenColor(inactive);
			iOffScreenBitmapGc->SetBrushColor(inactive);
			iOffScreenBitmapGc->DrawText(audioValBuf,audioPosVal);
			iOffScreenBitmapGc->DrawText(audio, audioPos);
			iOffScreenBitmapGc->DrawText(controls, controlsPos);
			iOffScreenBitmapGc->DrawText(credits, creditsPos);
			break;
		case 2:
			iOffScreenBitmapGc->SetPenColor(active);
			iOffScreenBitmapGc->SetBrushColor(active);
			iOffScreenBitmapGc->DrawText(controls, controlsPos);
			iOffScreenBitmapGc->SetPenColor(inactive);
			iOffScreenBitmapGc->SetBrushColor(inactive);
			iOffScreenBitmapGc->DrawText(audioValBuf,audioPosVal);
			iOffScreenBitmapGc->DrawText(sfxValBuf,sfxPosVal);
			iOffScreenBitmapGc->DrawText(audio, audioPos);
			iOffScreenBitmapGc->DrawText(sfx, sfxPos);
			iOffScreenBitmapGc->DrawText(credits, creditsPos);
			break;
		case 3:
			iOffScreenBitmapGc->SetPenColor(active);
			iOffScreenBitmapGc->SetBrushColor(active);
			iOffScreenBitmapGc->DrawText(credits, creditsPos);
			iOffScreenBitmapGc->SetPenColor(inactive);
			iOffScreenBitmapGc->SetBrushColor(inactive);
			iOffScreenBitmapGc->DrawText(audioValBuf,audioPosVal);
			iOffScreenBitmapGc->DrawText(sfxValBuf,sfxPosVal);
			iOffScreenBitmapGc->DrawText(audio, audioPos);
			iOffScreenBitmapGc->DrawText(sfx, sfxPos);
			iOffScreenBitmapGc->DrawText(controls, controlsPos);
			break;
		}
	iOffScreenBitmapGc->DiscardFont();
    }


// ---------------------------------------------------------
// CTOPGUNContainer3::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CTOPGUNContainer3::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }
// ---------------------------------------------------------
// CTOPGUNContainer3::OfferKeyEventL(
//				     const TKeyEvent& aKeyEvent, TEventCode aType)
// ---------------------------------------------------------
//
TKeyResponse CTOPGUNContainer3::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode /*aType*/)
    {
	switch ( aKeyEvent.iCode )
		{
		case EKeyLeftArrow:
			{
			switch ( iSelectPos )
				{
				case 0:
					{
					/*
					TReal vol = (TReal)((TReal)iSfxVal * (TReal)2.56);
					TInt  intVol = (TInt)vol;
					*/
					  
					iSndMixer->Play( iButtonSample, 2, 11025, iSfxVol );
					
					AudioVolumeDown();
					return EKeyWasConsumed;
					break;
					}
				case 1:
					{
					/*
					TReal vol = (TReal)((TReal)iSfxVal * (TReal)2.56);
					TInt  intVol = (TInt)vol;
					*/
					  
					iSndMixer->Play( iButtonSample, 2, 11025, iSfxVol );

					SfxVolumeDown();
					return EKeyWasConsumed;
					break;
					}
				default:
					return EKeyWasNotConsumed;
					break;
				}
			}

		case EKeyRightArrow:
			{
			switch ( iSelectPos )
				{
				case 0:
					{
					/*
					TReal vol = (TReal)((TReal)iSfxVal * (TReal)2.56);
					TInt  intVol = (TInt)vol;
					*/
					  
					iSndMixer->Play( iButtonSample, 2, 11025, iSfxVol );

					AudioVolumeUp();
					return EKeyWasConsumed;
					break;
					}
				case 1:
					{
					/*
					TReal vol = (TReal)((TReal)iSfxVal * (TReal)2.56);
					TInt  intVol = (TInt)vol;
					*/
					  
					iSndMixer->Play( iButtonSample, 2, 11025, iSfxVol );
					SfxVolumeUp();
					return EKeyWasConsumed;
					break;
					}
				default:
					return EKeyWasNotConsumed;
					break;
				}
			}
	

		case EKeyUpArrow:
			{
			/*
			TReal vol = (TReal)((TReal)iSfxVal * (TReal)2.56);
			TInt  intVol = (TInt)vol;
			*/
			  
			iSndMixer->Play( iButtonSample, 2, 11025, iSfxVol );

			MoveUp();
			return EKeyWasConsumed;
			break;
			}
		case EKeyDownArrow:
			{
			/*
			TReal vol = (TReal)((TReal)iSfxVal * (TReal)2.56);
			TInt  intVol = (TInt)vol;
			*/
			  
			iSndMixer->Play( iButtonSample, 2, 11025, iSfxVol );

			MoveDown();
			return EKeyWasConsumed;
			break;
			}
		case 0x0000f843:
			iView->SwitchView(0);
			return EKeyWasConsumed;
			break;
		
		case 0x0000f842:
		case EKeyEnter:
		case EKeyOK:
			{
		switch ( iSelectPos )
			{
			case 0:
				iSndMixer->Play( iSelectSample, 3, 11025, iSfxVol );
			    //iView->HandleCommandL(EStartGame);
				break;
			case 1:
				iSndMixer->Play( iSelectSample, 3, 11025, iSfxVol );
				break;
			case 2:
				iSndMixer->Play( iSelectSample, 3, 11025, iSfxVol );
				iView->SwitchView(5);
				break;
			case 3:
				iSndMixer->Play( iSelectSample, 3, 11025, iSfxVol );
				iView->SwitchView(6);
				break;
			default:
				break;
			}
			return EKeyWasConsumed;
			break;
		
		default:
			return EKeyWasNotConsumed;
			break;
			}	
		}
    }

void CTOPGUNContainer3::MoveUp()
	{
	if (iSelectPos >= 1)
		{
		iSelectPos--;
		DrawNow();
		}
	}

void CTOPGUNContainer3::MoveDown()
	{
	if (iSelectPos < 3)
		{
		iSelectPos++;
		DrawNow();
		}
	}


void CTOPGUNContainer3::AudioVolumeUp()
	{
	if (iAudioVol < 256)
		{
		iAudioVol = iAudioVol + 32;
		iSndMixer->Stop(1);
		iSndMixer->Play( iMusicSample, 1, 22050, iAudioVol );
		SetVolume(iSfxVol,iAudioVol);
		DrawNow();
		}
	}

void CTOPGUNContainer3::AudioVolumeDown()
	{
	if (iAudioVol >= 32)
		{
		iAudioVol = iAudioVol - 32;
		iSndMixer->Stop(1);
		iSndMixer->Play( iMusicSample, 1, 22050, iAudioVol );
		SetVolume(iSfxVol,iAudioVol);
		DrawNow();
		}
	}

void CTOPGUNContainer3::SfxVolumeUp()
	{
	if (iSfxVol < 256)
		{
		iSfxVol = iSfxVol + 32;
		SetVolume(iSfxVol,iAudioVol);
		DrawNow();
		}
	}

void CTOPGUNContainer3::SfxVolumeDown()
	{
	if (iSfxVol >= 32)
		{
		iSfxVol = iSfxVol - 32;
		SetVolume(iSfxVol,iAudioVol);
		DrawNow();
		}
	}

void CTOPGUNContainer3::SetView(CTOPGUNView3* aView)
	{
	iView = aView;
	}

void CTOPGUNContainer3::SetVolume(TInt aSfxVol, TInt aAudioVol)
	{
	iView->SetVolume(aSfxVol,aAudioVol);
	}

void CTOPGUNContainer3::SetVolumeForContainer(TInt aSfxVol, TInt aAudioVol)
	{
	iSfxVol = aSfxVol;
	iAudioVol = aAudioVol;
	DrawNow();
	}

// End of File  
