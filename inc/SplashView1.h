/*
* ============================================================================
*  Name     : CSplashView1 from SplashView1.h
*  Part of  : SonarBlue
*  Created  : 12/2/2003 by pawel@wpaice.com
*  Description: Declares view for application.
*  $Revision: 1.2 $
*  $Author: pawel@wapice.com $
*  $Date: 2003/12/07 18:50:58 $ 
* ============================================================================
*/

#ifndef SPLASHVIEW1_H
#define SPLASHVIEW1_H

// INCLUDES
#include <aknview.h>
#include <akntabgrp.h>


// CONSTANTS
// UID of view
const TUid KView4Id = {4};

// FORWARD DECLARATIONS
class CSplashContainer1;
class CTOPGUNAppUi;
// CLASS DECLARATION

/**
*  CSplashView1 view class.
* 
*/
class CSplashView1 : public CAknView
    {
    public: // Constructors and destructor

        /**
        * EPOC default constructor.
        */
        void ConstructL();

        /**
        * Destructor.
        */
        ~CSplashView1();

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
        CSplashContainer1* iContainer;
		CTOPGUNAppUi* iAppUi;
		CAknTabGroup* iTabGroup;
    };

#endif

// End of File
