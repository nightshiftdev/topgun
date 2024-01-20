/*
* ============================================================================
*  Name     : CTOPGUNView from TOPGUNView.h
*  Part of  : TOPGUN
*  Created  : 5/9/2004 by pawel@wapice.com
*  Description:
*     Declares view for application.
*  Version  :
*  Copyright: P&P
* ============================================================================
*/

#ifndef TOPGUNVIEW_H
#define TOPGUNVIEW_H

// INCLUDES
#include <aknview.h>
//sound
#include <mdaaudiooutputstream.h>
#include <mda\common\audio.h>
#include "TSample.h"

// CONSTANTS
// UID of view
const TUid KViewId = {1};

// FORWARD DECLARATIONS
class CTOPGUNContainer;
class CTOPGUNAppUi;
class CSndMixer;
// CLASS DECLARATION

/**
*  CTOPGUNView view class.
* 
*/
class CTOPGUNView : public CAknView
    {
    public: // Constructors and destructor

        /**
        * EPOC default constructor.
        */
        void ConstructL(CSndMixer*	aSndMixer,
						TSample& aMusicSample,
						TSample& aSelectSample,
						TSample& aButtonSample,
						TInt aAudioVol,
						TInt aSfxVol);

        /**
        * Destructor.
        */
        ~CTOPGUNView();
	
	public: //New functions

	   /**
        * Sets connection to AppUi class
        * @param aAppUi a valid pointer to AppUi class 
        * @return void
        */
		void SetAppUi(CTOPGUNAppUi* aAppUi);

		/**
        * Switches between views
        * @param aIndex an index of the view to be switched to. 
        * @return void
        */
		void SwitchView(TInt aIndex);

	   /**
        * Sets volume for this view
        * @param aSfxVol sfx volume.
		* @param aAudioVol audio volume.
        * @return void
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
        CTOPGUNContainer* iContainer;
		CTOPGUNAppUi* iAppUi;
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
