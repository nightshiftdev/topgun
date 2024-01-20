/*
* ============================================================================
*  Name     : CTOPGUNContainer9 from TOPGUNContainer9.h
*  Part of  : TOPGUN
*  Created  : 5/9/2004 by pawel@wapice.com
*  Description:
*     Declares container control for application.
*  Version  :
*  Copyright: IlPadrino group.
* ============================================================================
*/

#ifndef TOPGUNContainer9_H
#define TOPGUNContainer9_H

// INCLUDES
#include <coecntrl.h>
#include <mdaaudiooutputstream.h>
#include <mda\common\audio.h>
#include "TSample.h"
   
// FORWARD DECLARATIONS
class CTOPGUNDocument;
class CTOPGUNView9;
class CSndMixer;
class CLevelLoader;
// CLASS DECLARATION

//CONSTS

/**
*  CTOPGUNContainer9  container control class.
*  
*/
class CTOPGUNContainer9 : public CCoeControl, MCoeControlObserver
    {
    public: // Constructors and destructor
        
		CTOPGUNContainer9(CLevelLoader& aLevelLoader,
					      CTOPGUNDocument& aDocument,
					      CTOPGUNView9& aView);
        /**
        * EPOC default constructor.
        * @param aRect Frame rectangle for container.
        */
        void ConstructL(const TRect& aRect);

        /**
        * Destructor.
        */
        ~CTOPGUNContainer9();

    public: // New functions

		void SetFirstLetter(const TUint aNum);
		void SetSecondLetter(const TUint aNum);
		void SetThirdLetter(const TUint aNum);
		void NextLetter();
		void PreviousLetter();
		void EnterHighScore();

    public: // Functions from base classes

	   /**
	    * @function OfferKeyEventL
        *
	    * @discussion Handle key events.
	    * @param aKeyEvent The key event.
	    * @param aType The type of key event: EEventKey, EEventKeyUp or EEventKeyDown.
	    * @result Was the key event used by this control.
	    */
		TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

	private: // New functions
		
		/**
        * Draws captions: START, OPTIONS, EXIT
        */
		void DrawCaptions() const;

		/**
        * Handles cursor position up
        */
		void MoveUp();

		/**
        * Handles cursor position down
        */
		void MoveDown();

		void SetFirstLetterBuf(TDes& aBuf);

    private: // Functions from base classes

       /**
        * From CoeControl,SizeChanged.
        */
        void SizeChanged();

       /**
        * From CoeControl,CountComponentControls.
        */
        TInt CountComponentControls() const;

       /**
        * From CCoeControl,ComponentControl.
        */
        CCoeControl* ComponentControl(TInt aIndex) const;

       /**
        * From CCoeControl,Draw.
        */
        void Draw(const TRect& aRect) const;

       	
		/**
		* From MCoeControlObserver
		* Acts upon changes in the hosted control's state. 
		*
		* @param	aControl	The control changing its state
		* @param	aEventType	The type of control event 
		*/
        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);
        
    private: //data
        

		/*! @var iBackgroundImage the background bitmap */
		CFbsBitmap* iBackgroundImage;

		/*! @var iOffScreenBitmap the off screen bitmap */
		CFbsBitmap* iOffScreenBitmap;

		/*! @var iOffScreenBitmapDevice the off screen bitmap device */
		CFbsBitmapDevice* iOffScreenBitmapDevice;

		/*! @var iOffScreenBitmapGc the off screen bitmap gc */
		CFbsBitGc* iOffScreenBitmapGc;

		/*! @var iSelectPos responssible for knowing cursor position */
		TInt iSelectPos;
		
		/*! @var iSelectPos responssible for knowing cursor positi */
		CTOPGUNView9& iView;

		TUint iFirstLetter;
		TUint iSecondLetter;
		TUint iThirdLetter;
		TUint iPos;
		TUint iLetter;
		TBool iEnterHighScore;
		CTOPGUNDocument& iDocument;
		CLevelLoader& iLevelLoader;
    };

#endif

// End of File
