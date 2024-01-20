/*
* ============================================================================
*  Name     : CSplashContainer1 from SplashContainer1.h
*  Part of  : SonarBlue
*  Created  : 17/02/2003 by Eric@NewLC adapted to SonarBlue application by Pawel Kijowski 
*  Description: Declares container control for application.
*  $Revision: 1.2 $
*  $Author: pawel $
*  $Date: 2003/12/07 18:50:58 $
* ============================================================================
*/

#ifndef SPLASHCONTAINER1_H
#define SPLASHCONTAINER1_H

// INCLUDES
#include <coecntrl.h>    
// FORWARD DECLARATIONS
class CSplashView1;
// CLASS DECLARATION

/**
*  CSplashContainer1  container control class.
*  
*/
class CSplashContainer1 : public CCoeControl, MCoeControlObserver
    {
    public: // Constructors and destructor
        
        /**
        * EPOC default constructor.
        * @param aRect Frame rectangle for container.
        */
        void ConstructL(const TRect& aRect);

        /**
        * Destructor.
        */
        ~CSplashContainer1();

    public: // New functions
		/**
        * Draw Function
        */
        void DrawSplash(/*CGraphicsContext* bitmapContext, const TRect& aRect*/) const;
        void DrawMain(/*CGraphicsContext* bitmapContext, const TRect& aRect*/) const;
        void DrawExiting(/*CGraphicsContext* bitmapContext, const TRect& aRect*/) const;

		/**
		* Timer management function
		*/
		TBool Tick();
		static TInt Period(TAny* aPtr);
		void DoPeriod();

		void StartTimer();
		void StopTimer();

        void SetTimerTick(TTimeIntervalMicroSeconds32  aTick);

		enum TState 
        {
          ESplash,
          EMain,
          EExiting,
        };
		void SetState(CSplashContainer1::TState aState);
		CSplashContainer1::TState State();

		TState iState;

		void SetView(CSplashView1* aSplashView);

    public: // Functions from base classes

    private: // New Functions
   
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
        * From CCoeControl, Handles control events
        */
        // event handling section
        // e.g Listbox events
        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);
   

        
    private: //data
        
        //TState iState;
		CPeriodic* iPeriodicTimer;
		TTimeIntervalMicroSeconds32  iTick;
		TInt iColorCount;
        TInt iPreCount;
        TInt iPostCount;
		CSplashView1* iSplashView;

		/*! @var iBackgroundImage the background bitmap */
		CFbsBitmap* iBackgroundImage;

		/*! @var iOffScreenBitmap the off screen bitmap */
		CFbsBitmap* iOffScreenBitmap;

		/*! @var iOffScreenBitmapDevice the off screen bitmap device */
		CFbsBitmapDevice* iOffScreenBitmapDevice;

		/*! @var iOffScreenBitmapGc the off screen bitmap gc */
		CFbsBitGc* iOffScreenBitmapGc;
    };

#endif

// End of File
