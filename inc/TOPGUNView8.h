/*
* ============================================================================
*  Name     : CTOPGUNView8 from TOPGUNView8.h
*  Part of  : TOPGUN
*  Created  : 5/9/2004 by pawel@wapice.com
*  Description:
*     Declares view for application.
*  Version  :
*  Copyright: P&P
* ============================================================================
*/

#ifndef TOPGUNVIEW8_H
#define TOPGUNVIEW8_H

// INCLUDES
#include <aknview.h>
#include <mdaaudiooutputstream.h>
#include <mda\common\audio.h>
#include "TSample.h"


// CONSTANTS
// UID of view
const TUid KView10Id = {10};

// FORWARD DECLARATIONS
class CTOPGUNContainer8;
class CTOPGUNAppUi;
class CLevelLoader;
// CLASS DECLARATION

/**
*  CTOPGUNView8 view class.
* 
*/
class CTOPGUNView8 : public CAknView
    {
    public: // Constructors and destructor

		CTOPGUNView8(CLevelLoader& aLevelLoader,
					 CTOPGUNAppUi& aAppUi);

        /**
        * Destructor.
        */
        ~CTOPGUNView8();

	public: //New functions

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
        CTOPGUNContainer8* iContainer;
		CLevelLoader& iLevelLoader;
		CTOPGUNAppUi& iAppUi;
    };

#endif

// End of File
