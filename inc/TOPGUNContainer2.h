/*
* ============================================================================
*  Name     : CTOPGUNContainer2 from TOPGUNContainer2.h
*  Part of  : TOPGUN
*  Created  : 5/9/2004 by pawel@wapice.com
*  Description:
*     Declares container control for application.
*  Version  :
*  Copyright: IlPadrino group.
* ============================================================================
*/

#ifndef TOPGUNCONTAINER2_H
#define TOPGUNCONTAINER2_H

// INCLUDES
#include <coecntrl.h>
#include "KeyHandler.h"
//sound
#include <mdaaudiooutputstream.h>
#include <mda\common\audio.h>
#include "TSample.h"
#include "LevelLoader.h"

// FORWARD DECLARATIONS
class CImageFactory;
class CTopGunEngine; 
class CTOPGUNView2;
class CSndMixer;
class CLevelLoader;

// CLASS DECLARATION

/**
*  CTOPGUNContainer2  container control class.
*  
*/
class CTOPGUNContainer2 : public CCoeControl, MCoeControlObserver , public MCoeForegroundObserver
    {
    public: // Constructors and destructor
        
        /**
        * EPOC default constructor.
        * @param aRect Frame rectangle for container.
        */
        void ConstructL(const TRect& aRect,
						CSndMixer*	aSndMixer,
						TSample& aMusic,
						TSample& aShotSound,
						TSample& aExplosionSound,
						TInt aAudioVol,
						TInt aSfxVol,
						CLevelLoader* aLevelLoader);

        /**
        * Destructor.
        */
        ~CTOPGUNContainer2();

    public: // New functions
		/**
        * Starts the game.
        */
		void StartGameL();
		
		/**
        * Checks if the game is on
        */
		TBool IsPlaying();
		
		/**
        * Stops the game
        */
		void StopGame();

	   /**
	    * @function SetView
        *
	    * @discussion Sets valid pointer for the view class.
	    * @param aView pointer to instance of the view class.
	    * @result void.
	    */
		void SetView(CTOPGUNView2* aView);

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
	    * @function SwitchView
        *
	    * @discussion Switches between application views.
	    * @param aViewNumber view numebr.
	    * @result void.
	    */
		void SwitchView(TInt aViewNumber) const;

		/**
	    * @function InformAboutGameOver
        *
	    * @discussion Informs about game over this container.
	    * @result void.
	    */
		void InformAboutGameOver();

    public: // Functions from base classes

    private: // Functions from base classes

       /**
        * From CoeControl,SizeChanged.
        */
        void SizeChanged();

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

		/**
		* From CoeControl
		* Manages key events during the game. 
		*
		* @param	aKeyEvent	The key event
		* @param	aType	    The type of control event 
		*/
		TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

		// MCoeForegroundObserver
		void HandleGainingForeground();
		void HandleLosingForeground();

		/// Static callback function for timer, 
		/// called periodically
		/// @param aPtr pointer to this class
		//static TInt TimerCallBack( TAny* aPtr );

		/// Timer function called from TimerCallBack
		/// takes care for everything moving in this example
		//TInt DoTimer();

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
        
    private: //data

		TKeyHandler iKeyHandler;
	    
		CImageFactory* iImageFactory;
	    
		CTopGunEngine* iEngine;
		
		/*! @var iSelectPos responssible for knowing cursor positi */
		CTOPGUNView2* iView;

		TBool iPausePressed;

		
		CSndMixer* iSndMixer;			// Sound mixer
		//TSample iMusicSample;		// sampled music
		//TSample iButtonSample;		// sound effect
		//TSample iBackSample;		// sound effect
		//TSample iSelectSample;		// sound effect
		TSample	iMusic;				// sampled music
		TSample	iShotSound;			// sound effect
	    TSample	iExplosionSound;	// sound effect
		TInt iAudioVol;
		TInt iSfxVol;
		CLevelLoader* iLevelLoader;
		TBool iGameOver;
   };

#endif

// End of File
