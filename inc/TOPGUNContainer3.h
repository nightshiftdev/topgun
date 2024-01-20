/*
* ============================================================================
*  Name     : CTOPGUNContainer3 from TOPGUNContainer3.h
*  Part of  : TOPGUN
*  Created  : 5/9/2004 by pawel@wapice.com
*  Description:
*     Declares container control for application.
*  Version  :
*  Copyright: IlPadrino group.
* ============================================================================
*/

#ifndef TOPGUNCONTAINER3_H
#define TOPGUNCONTAINER3_H

// INCLUDES
#include <coecntrl.h>
//sound
#include <mdaaudiooutputstream.h>
#include <mda\common\audio.h>
#include "TSample.h"
   
// FORWARD DECLARATIONS
class CTOPGUNView3;
class CSndMixer;

// CLASS DECLARATION

/**
*  CTOPGUNContainer3  container control class.
*  
*/
class CTOPGUNContainer3 : public CCoeControl, MCoeControlObserver
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
        ~CTOPGUNContainer3();

    public: // New functions

	   /**
	    * @function SetView
        *
	    * @discussion Sets valid pointer for the view class.
	    * @param aView pointer to instance of the view class.
	    * @result void.
	    */
		void SetView(CTOPGUNView3* aView);

	   /**
	    * @function SetVolume
        *
	    * @discussion Sets current volume options for the engine.
	    * @param aSfxVol sfx volume value.
		* @param aAudioVol audio volume value.
	    * @result void.
	    */
		void SetVolume(TInt aSfxVol, TInt aAudioVol);
		
	   /**
	    * @function SetVolumeForContainer
        *
	    * @discussion Sets stored volume options.
	    * @param aSfxVol sfx volume value.
		* @param aAudioVol audio volume value.
	    * @result void.
	    */
		void SetVolumeForContainer(TInt aSfxVol, TInt aAudioVol);

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

		/**
        * Sets higher audio volume
        */
		void AudioVolumeUp();

		/**
        * Sets lower audio volume
        */
		void AudioVolumeDown();

		/**
        * Sets higher sfx volume
        */
		void SfxVolumeUp();

		/**
        * Sets lower sfx volume
        */
		void SfxVolumeDown();

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
		CTOPGUNView3* iView;

		/*! @var iAudioVol represents audio volume value */
		TInt iAudioVol;

		CSndMixer*	iSndMixer;			// Sound mixer
		TSample		iMusicSample;		// sampled music
		TSample		iButtonSample;		// sound effect
		//TSample		iBackSample;		// sound effect
		TSample		iSelectSample;		// sound effect
		//TSample		iMusic;				// sampled music
		//TSample		iShotSound;			// sound effect
	    //TSample		iExplosionSound;	// sound effect
		
		TInt iSfxVol;
    };

#endif

// End of File
