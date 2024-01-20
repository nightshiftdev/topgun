/*
* ============================================================================
*  Name     : CTOPGUNView4 from TOPGUNView4.h
*  Part of  : TOPGUN
*  Created  : 5/9/2004 by pawel@wapice.com
*  Description:
*     Declares view for application.
*  Version  :
*  Copyright: P&P
* ============================================================================
*/

#ifndef TOPGUNVIEW4_H
#define TOPGUNVIEW4_H

// INCLUDES
#include <aknview.h>


// CONSTANTS
// UID of view
const TUid KView6Id = {6};

// FORWARD DECLARATIONS
class CTOPGUNContainer4;
class CTOPGUNAppUi;
// CLASS DECLARATION

/**
*  CTOPGUNView4 view class.
* 
*/
class CTOPGUNView4 : public CAknView
    {
    public: // Constructors and destructor

        /**
        * EPOC default constructor.
        */
        void ConstructL();

        /**
        * Destructor.
        */
        ~CTOPGUNView4();

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
        CTOPGUNContainer4* iContainer;
		CTOPGUNAppUi* iAppUi;
    };

#endif

// End of File
