/*
* ============================================================================
*  Name     : CTOPGUNView6 from TOPGUNView6.h
*  Part of  : TOPGUN
*  Created  : 5/9/2004 by pawel@wapice.com
*  Description:
*     Declares view for application.
*  Version  :
*  Copyright: P&P
* ============================================================================
*/

#ifndef TOPGUNView6_H
#define TOPGUNView6_H

// INCLUDES
#include <aknview.h>
#include <mdaaudiooutputstream.h>
#include <mda\common\audio.h>
#include "TSample.h"


// CONSTANTS
// UID of view
const TUid KView8Id = {8};

// FORWARD DECLARATIONS
class CTOPGUNContainer6;
class CTOPGUNAppUi;
// CLASS DECLARATION

/**
*  CTOPGUNView6 view class.
* 
*/
class CTOPGUNView6 : public CAknView
    {
    public: // Constructors and destructor

        /**
        * EPOC default constructor.
        */
        void ConstructL();

        /**
        * Destructor.
        */
        ~CTOPGUNView6();

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
        CTOPGUNContainer6* iContainer;
		CTOPGUNAppUi* iAppUi;
    };

#endif

// End of File
