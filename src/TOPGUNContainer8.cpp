/*
* ============================================================================
*  Name     : CTOPGUNContainer8 from TOPGUNContainer8.h
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
#include "TOPGUNContainer8.h"
#include "TOPGUNView8.h"
#include "TOPGUNAppUi.h"
//level loader
#include "LevelLoader.h"

//CONSTANTS
_LIT (KMultiBitmapFilename,"z:\\resource\\apps\\topgun.mbm"); 
// ================= MEMBER FUNCTIONS =======================

CTOPGUNContainer8::CTOPGUNContainer8(CLevelLoader& aLevelLoader,CTOPGUNAppUi& aAppUi):
	iLevelLoader(aLevelLoader),
	iAppUi(aAppUi)
	{
	}

// ---------------------------------------------------------
// CTOPGUNContainer8::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CTOPGUNContainer8::ConstructL(const TRect& aRect)
    {
	//initial settings of the screen
	iSelectPos = 0;

    CreateWindowL();

	SetRect(aRect);

	iBackgroundImage = iEikonEnv->CreateBitmapL(KMultiBitmapFilename,EMbmTopgunMissionobjective);	
	// Create the off screen bitmap and device / gc
    iOffScreenBitmap       = NBitmapMethods::CreateBitmapL(Rect().Size(),KColourDepth);
    iOffScreenBitmapDevice = NBitmapMethods::CreateBitmapDeviceL(*iOffScreenBitmap);
    iOffScreenBitmapGc     = NBitmapMethods::CreateGraphicsContextL(*iOffScreenBitmapDevice);
    ActivateL();
    }

// Destructor
CTOPGUNContainer8::~CTOPGUNContainer8()
    {
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
// CTOPGUNContainer8::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CTOPGUNContainer8::SizeChanged()
    {
    // TODO: Add here control resize code etc.
    }

// ---------------------------------------------------------
// CTOPGUNContainer8::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CTOPGUNContainer8::CountComponentControls() const
    {
    return 0; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CTOPGUNContainer8::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CTOPGUNContainer8::ComponentControl(TInt /*aIndex*/) const
    {
	return NULL;
    }

// ---------------------------------------------------------
// CTOPGUNContainer8::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CTOPGUNContainer8::Draw(const TRect& /*aRect*/) const
    {
    //SystemGc().Clear(Rect());
	iOffScreenBitmapGc->BitBlt(TPoint(0,0),iBackgroundImage);
	DrawCaptions();
	SystemGc().BitBlt(Rect().iTl,iOffScreenBitmap);
    }

// ---------------------------------------------------------
// CTOPGUNContainer8::::DrawCaptions() const
// ---------------------------------------------------------
//
void CTOPGUNContainer8::DrawCaptions() const
    {
	CFont* font = (CFont*)LatinPlain12(); //font for scale
	iOffScreenBitmapGc->UseFont(font);


	TRgb inactive(111,110,108);

	TPoint objectivePos1(44,116);
	TPoint objectivePos2(82,130);
	TPoint objectivePos3(40,144);
	TPoint objectivePos4(50,160);

	TBuf<32> objectiveText1;
	TBuf<32> objectiveText2;
	TBuf<32> objectiveText3;
	TBuf<32> objectiveText4;

	TInt dl = iLevelLoader.DifficultyLevel();
	objectiveText1.Append(_L("Eliminate at least:"));
	objectiveText2.AppendFormat(_L("%d"), dl);
	objectiveText3.Append(_L("percent of enemies."));
	objectiveText4.Append(_L("Good luck pilot!"));

	iOffScreenBitmapGc->SetPenColor(inactive);
	iOffScreenBitmapGc->SetBrushColor(inactive);
	iOffScreenBitmapGc->DrawText(objectiveText1, objectivePos1);
	iOffScreenBitmapGc->DrawText(objectiveText2, objectivePos2);
	iOffScreenBitmapGc->DrawText(objectiveText3, objectivePos3);
	iOffScreenBitmapGc->DrawText(objectiveText4, objectivePos4);

	iOffScreenBitmapGc->DiscardFont();
    }


// ---------------------------------------------------------
// CTOPGUNContainer8::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CTOPGUNContainer8::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }
// ---------------------------------------------------------
// CTOPGUNContainer8::OfferKeyEventL(
//				     const TKeyEvent& aKeyEvent, TEventCode aType)
// ---------------------------------------------------------
//
TKeyResponse CTOPGUNContainer8::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode /*aType*/)
    {
	switch ( aKeyEvent.iCode )
		{
		case 0x0000f843:
			{
			//iSndMixer->Play( iBackSample, 4, 11025, iSfxVol );
			iAppUi.StopMenuMusic();
			iView->SwitchView(1);
			return EKeyWasConsumed;
			}
			break;
		case 0x0000f842:
			{
			return EKeyWasConsumed;
			}
			break;
		default:
			return EKeyWasNotConsumed;
			break;
		}
    }

void CTOPGUNContainer8::MoveUp()
	{
	if (iSelectPos >= 1)
		{
		iSelectPos--;
		DrawNow();
		}
	}

void CTOPGUNContainer8::MoveDown()
	{
	if (iSelectPos < 3)
		{
		iSelectPos++;
		DrawNow();
		}
	}

void CTOPGUNContainer8::SetView(CTOPGUNView8* aView)
	{
	iView = aView;
	}

// End of File  
