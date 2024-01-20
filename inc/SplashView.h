/*
* ============================================================================
*  Name     : CSplashView from SplashView2.h
*  Part of  : SonarBlue
*  Created  : 12/2/2003 by Pawel Kijowski
*  Description: Declares view for application.
*  $Revision: 1.2 $
*  $Author: pawel $
*  $Date: 2003/12/07 18:50:58 $ 
* ============================================================================
*/

#ifndef SPLASHVIEW_H
#define SPALSHVIEW_H

// INCLUDES
#include <aknview.h>
#include <akntabgrp.h>

// CONSTANTS
// UID of view
const TUid KView3Id = {3};

// FORWARD DECLARATIONS
class CSplashContainer;
class CTOPGUNAppUi;
// CLASS DECLARATION

/**
*  CSplashView view class.
* 
*/
class CSplashView : public CAknView
    {
    public: // Constructors and destructor

        /**
        * EPOC default constructor.
        */
        void ConstructL();

        /**
        * Destructor.
        */
        ~CSplashView();

    public: // Functions from base classes
        
        /**
        * From ?base_class ?member_description
        */
        TUid Id() const;

        /**
        * From ?base_class ?member_description
        */
        void HandleCommandL(TInt aCommand);

        /**
        * From ?base_class ?member_description
        */
        void HandleClientRectChange();

		void SetAppUi(CTOPGUNAppUi* aAppUi, CAknTabGroup* aTabGroup);

		void SwitchView();

    private:

        /**
        * From AknView, ?member_description
        */
        void DoActivateL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,
            const TDesC8& aCustomMessage);

        /**
        * From AknView, ?member_description
        */
        void DoDeactivate();

    private: // Data
        CSplashContainer* iContainer;
		CTOPGUNAppUi*     iAppUi;
		CAknTabGroup*     iTabGroup;
    };

#endif

// End of File
