/*
* ============================================================================
*  Name     : CTOPGUNContainer from TOPGUNContainer.h
*  Part of  : TOPGUN
*  Created  : 5/9/2004 by pawel@wapice.com
*  Description:
*     Declares container control for application.
*  Version  :
*  Copyright: IlPadrino group.
* ============================================================================
*/

#ifndef TOPGUNCONTAINER_H
#define TOPGUNCONTAINER_H

// INCLUDES
#include <coecntrl.h>
#include <coemain.h>
#include <mdaaudiooutputstream.h>
#include <mda\common\audio.h>
#include "TSample.h"
   
// FORWARD DECLARATIONS
class CTOPGUNView;
class CSndMixer;
// CLASS DECLARATION

/**
*  CTOPGUNContainer  container control class.
*  
*/
class CTOPGUNContainer : public CCoeControl, MCoeControlObserver
    {
    public: // Constructors and destructor
        
        /**
        * EPOC default constructor.
        * @param aRect Frame rectangle for container.
        */
        void ConstructL(const TRect& aRect,
						CSndMixer*	aSndMixer,
						TSample& aMusicSample,
						TSample& aSelectSample,
						TSample& aButtonSample,
						TInt aAudioVol,
						TInt aSfxVol);

        /**
        * Destructor.
        */
        ~CTOPGUNContainer();

    public: // New functions

	   /**
	    * @function SetView
        *
	    * @discussion Sets valid pointer for the view class.
	    * @param aView pointer to instance of the view class.
	    * @result void.
	    */
		void SetView(CTOPGUNView* aView);

		/// Stops mixer
		//void StopMixer();

		/// Starts mixer
		//void StartMixer();


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

		
		/// Static callback function for timer, 
		/// called periodically
		/// @param aPtr pointer to this class
		//static TInt TimerCallBack( TAny* aPtr );


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

		//void HandleGainingForeground();
		
		//void HandleLosingForeground();

		//TInt Dummy();
        
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
		CTOPGUNView* iView;

		CSndMixer*	iSndMixer;			// Sound mixer
		TSample		iMusicSample;		// sampled music
		TSample		iButtonSample;		// sound effect
		//TSample		iBackSample;		// sound effect
		TSample		iSelectSample;		// sound effect
		//TSample		iMusic;				// sampled music
		//TSample		iShotSound;			// sound effect
	    //TSample		iExplosionSound;	// sound effect
		TInt iAudioVol;
		TInt iSfxVol;
    };

#endif

// End of File
