/*
* ============================================================================
*  Name     : CTOPGUNApp from TOPGUNApp.h
*  Part of  : TOPGUN
*  Created  : 5/9/2004 by pawel@wapice.com
*  Description:
*     Declares main application class.
*  Version  :
*  Copyright: P&P
* ============================================================================
*/

#ifndef TOPGUNAPP_H
#define TOPGUNAPP_H

// INCLUDES
#include <aknapp.h>

// CONSTANTS
// UID of the application
const TUid KUidTOPGUN = { 0x1027486B };

// CLASS DECLARATION

/**
* CTOPGUNApp application class.
* Provides factory to create concrete document object.
* 
*/
class CTOPGUNApp : public CAknApplication
    {
    
    public: // Functions from base classes
        /**
        * From CApaApplication, overridden to enable INI file support.
        * @return A pointer to the dictionary store
        */
    CDictionaryStore* OpenIniFileLC(RFs& aFs) const;
    private:

        /**
        * From CApaApplication, creates CTOPGUNDocument document object.
        * @return A pointer to the created document object.
        */
        CApaDocument* CreateDocumentL();
        
        /**
        * From CApaApplication, returns application's UID (KUidTOPGUN).
        * @return The value of KUidTOPGUN.
        */
        TUid AppDllUid() const;
    };

#endif

// End of File

