/*
* ============================================================================
*  Name     : CTOPGUNAppUi from TOPGUNAppUi.h
*  Part of  : TOPGUN
*  Created  : 5/9/2004 by pawel@wapice.com
*  Description:
*     Declares UI class for application.
*  Version  : 1.0
*  Copyright: P&P
* ============================================================================
*/

#ifndef TOPGUNAPPUI_H
#define TOPGUNAPPUI_H

// INCLUDES
#include <eikapp.h>
#include <eikdoc.h>
#include <e32std.h>
#include <coeccntx.h>
#include <aknviewappui.h>
#include <akntabgrp.h>
#include <aknnavide.h>
#include "TOPGUNView2.h"
#include "TOPGUNView.h"
//sound
#include <mdaaudiooutputstream.h>
#include <mda\common\audio.h>
#include "TSample.h"


// FORWARD DECLARATIONS
class CTOPGUNContainer;
class CTOPGUNView3;
class CTOPGUNView2;
class CTOPGUNView;
class CSndMixer;
class CTopgunDb;
class CTOPGUNDocument;
class CLevelLoader;


//CONSTANTS
// Name of database file in the application directory
//_LIT(KSoundDatabaseFile,"sound.dat");
//_LIT(KHSDatabaseFile,"hs.dat");
#define DEFAULT_SFX 64
#define DEFAULT_AUDIO 160



// CLASS DECLARATION

/**
* Application UI class.
* Provides support for the following features:
* - EIKON control architecture
* - view architecture
* - status pane
* 
*/
class CTOPGUNAppUi : public CAknViewAppUi
    {
    public: // // Constructors and destructor

        /**
        * EPOC default constructor.
        */      
        void ConstructL();

        /**
        * Destructor.
        */      
        ~CTOPGUNAppUi();
        
    public: // New functions
		/**
        * Sets iView class member variable.
        * @param aView2 pointer to an instance of CTOPGUNView2 class 
        */
		void SetView2(CTOPGUNView2* aView2);
		
	   /**
        * Changes view.
        */
		void SwitchView(TInt aIndex);

	   /**
	    * @function SetVolume
        *
	    * @discussion Sets current volume options for the engine.
	    * @param aSfxVol sfx volume value.
		* @param aAudioVol audio volume value.
	    * @result void.
	    */
		void SetVolume(TInt aSfxVol, TInt aAudioVol, const TBool aCommandFromEngine);

	   /**
	    * @function CreateDataBase
		* @discussion Creates path to database files.
	    * @result void.
	    */
		//void CreateDataBase();

		void SaveNowL();

		void StopMenuMusic();

		void StartMenuMusic();

		CTOPGUNDocument* GetDocument();

		TBool CheckHighScore(const TUint aHighScore);

		void InsertHighScoreL(const TUint aPos, const TUint aHighScore, HBufC* aPlayerName);
		void RunGameFirstTimeL();
		//void SetLevelLoader(CLevelLoader* aLevelLoader);
		//CLevelLoader* GetLevelLoader();

    public: // Functions from base classes

    private:
        // From MEikMenuObserver
        void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);

    private:
		void InitSoundL();
		void AddDummyHighScoreL();
		void AddDefaultVolSettingsL();
        /**
        * From CEikAppUi, takes care of command handling.
        * @param aCommand command to be handled
        */
        void HandleCommandL(TInt aCommand);

        /**
        * From CEikAppUi, handles key events.
        * @param aKeyEvent Event to handled.
        * @param aType Type of the key event. 
        * @return Response code (EKeyWasConsumed, EKeyWasNotConsumed). 
        */
        virtual TKeyResponse HandleKeyEventL(
            const TKeyEvent& aKeyEvent,TEventCode aType);

		/**
        * @discussion  Opens database files.
        * @return void. 
        */
		//void OpenDatabaseL();

	    /**
        * @discussion  Creates database files.
        * @return void. 
        */
		//void CreateDatabaseL();

		
	    /**
        * @discussion  Closes database files.
        * @return void. 
        */
		//void CloseDatabaseL();

	   /**
        * @discussion  Appends complete database drive and path.
        * @return Full database file drive + path + file name . 
        */
		TFileName ApplicationDriveAndPath() const;

	   /**
        * @discussion  Adds sound settings to database.
		* @param aUseSql specyfies if we want use SQL or Symbian database SQL API
		* @param aSfx sfx setting.
		* @param aAudio audio setting.
        * @return void . 
        */
		//void AddSoundSettingsL(TBool aUseSql, TDes& aSfx, TDes& aAudio);

	   /**
        * @discussion  Removes all sound settings from database.
        * @return void . 
        */
		//void RemoveSettingsL();
		
	   /**
        * @discussion  Gets sound settings from database.
        * @return aSfx and aAudio. 
        */
		//TInt GetSettingsL(TInt& aSfx, TInt& aAudio);
	
	   /**
        * @discussion  Saves settings to database.
		* @param aSfx sfx setting.
		* @param aAudio audio setting.
        * @return void . 
        */
		void SaveVolL(TInt aSfx, TInt aAudio);

	   /**
        * @discussion  Loads settings from database.
		* @param aSfx sfx setting.
		* @param aAudio audio setting.
        * @return void . 
        */
		void LoadL(TInt& aSfx, TInt& aAudio);

    private: //Data
        CAknNavigationControlContainer* iNaviPane;
        CAknTabGroup*                   iTabGroup;
        CAknNavigationDecorator*        iDecoratedTabGroup;
		CTOPGUNView*					iView;
		CTOPGUNView2*					iView2;
		CTOPGUNView3*					iView3;
		CSndMixer*	iSndMixer;			// Sound mixer
		TSample		iEnterSample;
		TSample		iMusicSample;		// sampled music
		TSample		iButtonSample;		// sound effect
		TSample		iBackSample;		// sound effect
		TSample		iSelectSample;		// sound effect
		TSample		iMusic;				// sampled music
		TSample		iShotSound;			// sound effect
	    TSample		iExplosionSound;	// sound effect
		TInt iAudioVol;
		TInt iSfxVol;
		CLevelLoader* iLevelLoader;

		/*! @var iTopgunDb provides connection to database managment class */
		//CTopgunDb* iTopgunDb;

		/*! @var iSoundDatabaseFile Full path to sound database file */
		//TFileName iSoundDatabaseFile;

		/*! @var iHSDatabaseFile Full path to highscores database file */
		//TFileName iHSDatabaseFile;
    };

#endif

// End of File
