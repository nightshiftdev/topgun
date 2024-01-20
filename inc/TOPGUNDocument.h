/*
* ============================================================================
*  Name     : CTOPGUNDocument from TOPGUNDocument.h
*  Part of  : TOPGUN
*  Created  : 5/9/2004 by pawel@wapice.com
*  Description:
*     Declares document for application.
*  Version  :
*  Copyright: P&P
* ============================================================================
*/

#ifndef TOPGUNDOCUMENT_H
#define TOPGUNDOCUMENT_H

// INCLUDES
#include <akndoc.h>
   
// CONSTANTS
#define MAX_HS_ENTRIES 5 //5 entries avaiable

// FORWARD DECLARATIONS
class CEikAppUi;
class CHighScoreItem;
class CGameSettings; 

// CLASS DECLARATION

/**
*  CTOPGUNDocument application class.
*/
class CTOPGUNDocument : public CAknDocument
    {
    public: // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CTOPGUNDocument* NewL(CEikApplication& aApp);

        /**
        * Destructor.
        */
        virtual ~CTOPGUNDocument();

    public: // New functions

		TBool CheckHighScore(const TUint aHighScore);

		void InsertHighScoreItemL(CHighScoreItem* aItem, TInt aIndex);

		void RemoveHS(TInt aIndex);

		void SetGameSettingsL(CGameSettings* aSettings, TInt aIndex);

		void SortHighScoreList();

		CHighScoreItem* HighScoreItem(TInt aIndex);

		CGameSettings* GameSettings(TInt aIndex);

    public: // Functions from base classes
        
        /**
        * From CApaDocument: This is called by framework to restore saved data
		* from persistent store to document
        */
		void RestoreL(const CStreamStore& aStore, const CStreamDictionary& aStreamDic);

        /**
        * From CApaDocument: This is called by framework to store data
		* to persistent store
        */
		void StoreL(CStreamStore& aStore, CStreamDictionary& aStreamDic) const;

		// from CEikDocument
		CFileStore* OpenFileL(TBool aDoOpen,const TDesC& aFilename,RFs& aFs);
    protected:  // New functions

    protected:  // Functions from base classes

    private:

        /**
        * EPOC default constructor.
        */ 
        CTOPGUNDocument(CEikApplication& aApp);
        void ConstructL();

    private:

        /**
        * From CEikDocument, create CTOPGUNAppUi "App UI" object.
        */
        CEikAppUi* CreateAppUiL();

		/*! @var iItemList the list of shopping items */
		RPointerArray<CHighScoreItem> iHighScoreList;
		
		/*! @var iItemList the list of shopping items */
		RPointerArray<CGameSettings> iGameSettings;
    };

#endif

// End of File

