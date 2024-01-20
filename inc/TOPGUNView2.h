/*
* ============================================================================
*  Name     : CTOPGUNView2 from TOPGUNView2.h
*  Part of  : TOPGUN
*  Created  : 5/9/2004 by pawel@wapice.com
*  Description:
*     Declares view for application.
*  Version  : 1.0
*  Copyright: ilPadrino group.
* ============================================================================
*/

#ifndef TOPGUNVIEW2_H
#define TOPGUNVIEW2_H

// INCLUDES
#include <aknview.h>
//sound
#include <mdaaudiooutputstream.h>
#include <mda\common\audio.h>
#include "TSample.h"
#include "LevelLoader.h"

// CONSTANTS
// UID of view
const TUid KView2Id = {2};

// FORWARD DECLARATIONS
class CTOPGUNContainer2;
class CTOPGUNAppUi;
class CSndMixer;
class CLevelLoader;
// CLASS DECLARATION

/**
*  CTOPGUNView2 view class.
* 
*/
class CTOPGUNView2 : public CAknView
    {
    public: // Constructors and destructor

		CTOPGUNView2(CSndMixer& aSndMixer,
					 TSample& aMusic,
					 TSample& aShotSound,
					 TSample& aExplosionSound,
					 TInt aAudioVol,
					 TInt aSfxVol,
					 CLevelLoader& aLevelLoader,
					 CTOPGUNAppUi& aAppUi);
        /**
        * EPOC default constructor.
        */
        void ConstructL(/*
						CSndMixer* aSndMixer,
						TSample& aMusic,
						TSample& aShotSound,
						TSample& aExplosionSound,
						TInt aAudioVol,
						TInt aSfxVol
						*/);

        /**
        * Destructor.
        */
        ~CTOPGUNView2();

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
        * Switches between views
        * @param aIndex an index of the view to be switched to. 
        * @return void
        */
		void SwitchView(TInt aIndex);

	   /**
        * Sets connection to AppUi class
        * @param aAppUi a valid pointer to AppUi class 
        * @return void
        */
		//void SetAppUi(CTOPGUNAppUi* aAppUi);
		
	   /**
	    * @function SetVolume
        *
	    * @discussion Sets current volume options for the engine.
	    * @param aSfxVol sfx volume value.
		* @param aAudioVol audio volume value.
	    * @result void.
	    */
		void SetVolume(TInt aSfxVol, TInt aAudioVol);

    public: // Functions from base classes
                        
		/**
        * From CAknView returns Uid of View
        * @return TUid uid of the view
        */
        TUid Id() const;

        /**
        * From MEikMenuObserver delegate commands from the menu
        * @param aCommand a command emitted by the menu 
        * @return void
        */
        void HandleCommandL(TInt aCommand);

        /**
        * From CAknView reaction if size change
        * @return void
        */
        void HandleClientRectChange();

    private:

        /**
        * From CAknView activate the view
        * @param aPrevViewId 
        * @param aCustomMessageId 
        * @param aCustomMessage 
        * @return void
        */
        void DoActivateL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,
            const TDesC8& aCustomMessage);

        /**
        * From CAknView deactivate the view (free resources)
        * @return void
        */
        void DoDeactivate();

    private: // Data
        CTOPGUNContainer2* iContainer;
		
		CTOPGUNAppUi& iAppUi;

		CSndMixer&	iSndMixer;			// Sound mixer
		//TSample		iMusicSample;		// sampled music
		//TSample		iButtonSample;		// sound effect
		//TSample		iBackSample;		// sound effect
		//TSample		iSelectSample;		// sound effect
		TSample		iMusic;				// sampled music
		TSample		iShotSound;			// sound effect
	    TSample		iExplosionSound;	// sound effect
		TInt iAudioVol;
		TInt iSfxVol;
		CLevelLoader& iLevelLoader;
    };

#endif

// End of File
