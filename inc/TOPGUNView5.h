/*
* ============================================================================
*  Name     : CTOPGUNView5 from TOPGUNView5.h
*  Part of  : TOPGUN
*  Created  : 5/9/2004 by pawel@wapice.com
*  Description:
*     Declares view for application.
*  Version  :
*  Copyright: P&P
* ============================================================================
*/

#ifndef TOPGUNVIEW5_H
#define TOPGUNVIEW5_H

// INCLUDES
#include <aknview.h>

// CONSTANTS
// UID of view
const TUid KView7Id = {7};

// FORWARD DECLARATIONS
class CTOPGUNContainer5;
class CTOPGUNAppUi;
// CLASS DECLARATION

/**
*  CTOPGUNView5 view class.
* 
*/
class CTOPGUNView5 : public CAknView
    {
    public: // Constructors and destructor

        /**
        * EPOC default constructor.
        */
        void ConstructL();

        /**
        * Destructor.
        */
        ~CTOPGUNView5();

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
        CTOPGUNContainer5* iContainer;
		CTOPGUNAppUi* iAppUi;
    };

#endif

// End of File
