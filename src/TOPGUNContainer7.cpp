/*
* ============================================================================
*  Name     : CTOPGUNContainer7 from TOPGUNContainer7.h
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
#include "TOPGUNContainer7.h"
#include "TOPGUNView7.h"
#include "TOPGUNAppUi.h"
//level loader
#include "LevelLoader.h"

//CONSTANTS
_LIT (KMultiBitmapFilename,"z:\\resource\\apps\\topgun.mbm"); 
// ================= MEMBER FUNCTIONS =======================

CTOPGUNContainer7::CTOPGUNContainer7(CLevelLoader& aLevelLoader,CTOPGUNAppUi& aAppUi):
	iLevelLoader(aLevelLoader),
	iAppUi(aAppUi)
	{
	}

// ---------------------------------------------------------
// CTOPGUNContainer7::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CTOPGUNContainer7::ConstructL(const TRect& aRect)
    {
	//initial settings of the screen
	iSelectPos = 0;

    CreateWindowL();

	SetRect(aRect);

	iBackgroundImage = iEikonEnv->CreateBitmapL(KMultiBitmapFilename,EMbmTopgunMissionfailed);	
	// Create the off screen bitmap and device / gc
    iOffScreenBitmap       = NBitmapMethods::CreateBitmapL(Rect().Size(),KColourDepth);
    iOffScreenBitmapDevice = NBitmapMethods::CreateBitmapDeviceL(*iOffScreenBitmap);
    iOffScreenBitmapGc     = NBitmapMethods::CreateGraphicsContextL(*iOffScreenBitmapDevice);
    ActivateL();

	iAppUi.StartMenuMusic();
    }

// Destructor
CTOPGUNContainer7::~CTOPGUNContainer7()
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
// CTOPGUNContainer7::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CTOPGUNContainer7::SizeChanged()
    {
    // TODO: Add here control resize code etc.
    }

// ---------------------------------------------------------
// CTOPGUNContainer7::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CTOPGUNContainer7::CountComponentControls() const
    {
    return 0; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CTOPGUNContainer7::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CTOPGUNContainer7::ComponentControl(TInt /*aIndex*/) const
    {
	return NULL;
    }

// ---------------------------------------------------------
// CTOPGUNContainer7::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CTOPGUNContainer7::Draw(const TRect& /*aRect*/) const
    {
    //SystemGc().Clear(Rect());
	iOffScreenBitmapGc->BitBlt(TPoint(0,0),iBackgroundImage);
	//DrawCaptions();
	SystemGc().BitBlt(Rect().iTl,iOffScreenBitmap);
    }

// ---------------------------------------------------------
// CTOPGUNContainer7::::DrawCaptions() const
// ---------------------------------------------------------
//
void CTOPGUNContainer7::DrawCaptions() const
    {
	CFont* font = (CFont*)LatinPlain12(); //font for scale
	iOffScreenBitmapGc->UseFont(font);


	TRgb active(165,2,9);
	TRgb inactive(36,35,36);

	TPoint singlePos(0,46);
	TPoint multiPos(96,63);
	TPoint optionsPos(89,81);
	TPoint exitPos(100,99);


	switch ( iSelectPos )
		{
		case 0:
			iOffScreenBitmapGc->SetPenColor(active);
			iOffScreenBitmapGc->SetBrushColor(active);
			iOffScreenBitmapGc->DrawText(_L(" Container7 "), singlePos);
			iOffScreenBitmapGc->SetPenColor(inactive);
			iOffScreenBitmapGc->SetBrushColor(inactive);
			iOffScreenBitmapGc->DrawText(_L(" m u l t i "), multiPos);
			iOffScreenBitmapGc->DrawText(_L(" o p t i o n s "), optionsPos);
			iOffScreenBitmapGc->DrawText(_L(" e x i t "), exitPos);
			break;
		case 1:
			iOffScreenBitmapGc->SetPenColor(active);
			iOffScreenBitmapGc->SetBrushColor(active);
			iOffScreenBitmapGc->DrawText(_L(" m u l t i "), multiPos);
			iOffScreenBitmapGc->SetPenColor(inactive);
			iOffScreenBitmapGc->SetBrushColor(inactive);
			iOffScreenBitmapGc->DrawText(_L(" s i n g l e "), singlePos);
			iOffScreenBitmapGc->DrawText(_L(" o p t i o n s "), optionsPos);
			iOffScreenBitmapGc->DrawText(_L(" e x i t "), exitPos);
			break;
		case 2:
			iOffScreenBitmapGc->SetPenColor(active);
			iOffScreenBitmapGc->SetBrushColor(active);
			iOffScreenBitmapGc->DrawText(_L(" o p t i o n s "), optionsPos);
			iOffScreenBitmapGc->SetPenColor(inactive);
			iOffScreenBitmapGc->SetBrushColor(inactive);
			iOffScreenBitmapGc->DrawText(_L(" s i n g l e "), singlePos);
			iOffScreenBitmapGc->DrawText(_L(" m u l t i "), multiPos);
			iOffScreenBitmapGc->DrawText(_L(" e x i t "), exitPos);
			break;
		case 3:
			iOffScreenBitmapGc->SetPenColor(active);
			iOffScreenBitmapGc->SetBrushColor(active);
			iOffScreenBitmapGc->DrawText(_L(" e x i t "), exitPos);
			iOffScreenBitmapGc->SetPenColor(inactive);
			iOffScreenBitmapGc->SetBrushColor(inactive);
			iOffScreenBitmapGc->DrawText(_L(" s i n g l e "), singlePos);
			iOffScreenBitmapGc->DrawText(_L(" m u l t i "), multiPos);
			iOffScreenBitmapGc->DrawText(_L(" o p t i o n s "), optionsPos);
			break;
		}
	iOffScreenBitmapGc->DiscardFont();
    }


// ---------------------------------------------------------
// CTOPGUNContainer7::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CTOPGUNContainer7::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }
// ---------------------------------------------------------
// CTOPGUNContainer7::OfferKeyEventL(
//				     const TKeyEvent& aKeyEvent, TEventCode aType)
// ---------------------------------------------------------
//
TKeyResponse CTOPGUNContainer7::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode /*aType*/)
    {
	switch ( aKeyEvent.iCode )
		{
		case 0x0000f843://right option key
			{
			//continue YES
			iLevelLoader.ContinueAfterGameOver(ETrue);
			iView->SwitchView(9); //Mission objectives screen
			return EKeyWasConsumed;
			}
			break;
		case 0x0000f842://left option key
			{
			//continue NO
			iLevelLoader.ContinueAfterGameOver(EFalse);
			iView->SwitchView(10); //go to high scores
			return EKeyWasConsumed;
			}
			break;
		default:
			return EKeyWasNotConsumed;
			break;
		}
    }

void CTOPGUNContainer7::MoveUp()
	{
	if (iSelectPos >= 1)
		{
		iSelectPos--;
		DrawNow();
		}
	}

void CTOPGUNContainer7::MoveDown()
	{
	if (iSelectPos < 3)
		{
		iSelectPos++;
		DrawNow();
		}
	}

void CTOPGUNContainer7::SetView(CTOPGUNView7* aView)
	{
	iView = aView;
	}

// End of File  
