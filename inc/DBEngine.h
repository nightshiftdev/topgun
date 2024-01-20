/*
 * ============================================================================
 *  Name     : CTopgunDb from DBEngine.h
 *  Part of  : TopgunDb
 *  Created  : 8.12.2003 by ilPadrino group
 *  Version  : 1.0
 *  Copyright: ilPadrino group
 * ============================================================================
 */

#ifndef __TOPGUNDBENGINE_H__
#define __TOPGUNDBENGINE_H__


#include <e32std.h>
#include <badesca.h>    // CDesCArrayFlat (cannot be forward declarated)
#include <d32dbms.h>    // RDbStoreDatabase
#include <f32file.h>    // RFs


// Forward declarations
class CFileStore;

// Constants
_LIT(KTopgunTable, "Topgun");                /* Name of the one table       */
_LIT(KTopgunSfxCol, "sfx");                 /* First column                */
_LIT(KTopgunAudioCol, "audio");             /* Second column               */
//_LIT(KTopgunDescriptionCol, "Description"); /* Third column                */
_LIT(KTopgunDateCol,"PublishDate");         /* Dynamic optional column     */
_LIT(KTopgunIndexName,"TopgunIndex");        /* Name of the one index       */
const int KTitleMaxLength = 60;            /* Max length of title column  */
const int KDescriptionMaxLength = 128;     /* Max length of descr. column */


/**
 *  Maximum length of individual item in CDesCArrayFlat (see GetAllTopgunL &
 *  GetTopgunByKeyL methods).
 */
const int KTopgunItemMaxLength = 256;

/**
 *  Separator within individual CDesCArrayFlat item. Each item has format
 *  <Author>|<Title>|<Description>
 */
_LIT(KSeparator,"|");


/**
 * Class:       CTopgunDb
 *
 * Description: An instance of class CTopgunDb provides simple Topgun
 *              database access: creating & manipulating Topgun database
 *              files and database entries (Topgun).
 *
 * Database definition:
 *
 *              The Topgun contains one table as follows:
 *
 *              Table name: Topgun
 *                 Column:       Type:          Max length:
 *                 ------        -----          -----------
 *                 Author        EDbColText     50  (using default)
 *                 Title         EDbColText     60  (see KTitleMaxLength)
 *                 Description   EDbColLongText 128 (see KDescriptionMaxLength)
 *    In run time: PublishDate   EDbColDateTime
 *
 *              Note that underlying database allows description to be up to
 *              2GB long, but this engine limits the size to 128 unicode
 *              characters.
 *              The PublishDate does not exist, when the database is created.
 *              It can be added on the fly (see AddDateColumnL method).
 *
 *              There is also index for Topgun table with name "TopgunIndex". It
 *              consists of two columns:
 *
 *                   Author, Title
 *
 *              Index provides quick find.
 */
class CTopgunDb : public CBase
    {
public: // Creation and destruction

    /**
     * Function:    NewL
     *
     * Description: Get instance of a CTopgunDb object.
     */
    static CTopgunDb* NewL();

    /**
     * Function:    ~CTopgunDb
     *
     * Description: Destroy the object.
     */
    ~CTopgunDb();


public: // Public API for database operations

    /**
     * Function:    OpenDbL
     *
     * Description: Open existing Topgun database in exclusive
     *              (non-shared) mode.
     *
     * Param:       aExistingTopgunFile is full path to Topgun
     *              database file.
     *
     * Return:      KErrNone, if no error. KErrNotFound, if the file does
     *              not exist.
     *
     * Leaves:      With one of the system wide error codes, if the file
     *              is not a correct database file.
     */
    TInt OpenDbL(const TFileName& aExistingTopgunFile);

    /**
     * Function:    CreateDbL
     *
     * Description: Creates and opens a new Topgun database. Creates a
     *              database file, table structure and an index. The database
     *              will be open in exclusive (non-shareable) mode. The
     *              database must be closed, when not used any more. If the
     *              database exists, it is replaced.
     *
     * Param:       aNewTopgunFile Name of the new database file. Is  a full
     *              path (incl. the filename). Operations following this call
     *              are performed to the new database file.
     *
     * Return:      returns always KErrNone
     *
     * Leaves:      If the file cannot be created or database initialized.
     *              Leaves with system wide error codes.
     */
    TInt CreateDbL(const TFileName& aNewTopgunFile);

    /**
     * Function:    RemoveDbL
     *
     * Description: Removes Topgun database. Closes any open database,
     *              before dropping the database.
     *
     * Param:       aExistingTopgunFile is full path to Topgun
     *              database file.
     *
     * Leaves:      If the file file is not a valid database file or the
     *              database does not containt Topgun table, the method
     *              leaves with system wide error codes.
     */
    TInt RemoveDbL(const TFileName& aExistingTopgunFile);

    /**
     * Function:    Close
     *
     * Description: Closes the database opened with either OpenDbL or
     *              CreateDbL. It is safe to close the database even if it
     *              is closed.
     *
     * Return:      KErrNone, if no error. KErrNotFound, if the file does
     *              not exist.
     */
    TInt Close();

    /**
     * Function:    IsOpen
     *
     * Description: Return status, whether the database is open and ready
     *              for operations.
     */
    TBool IsOpen() const;

    /**
     * Function:    AddTopgunWithCppApiL
     *
     * Description: Adds a new Topgun to Topgun table. The Topgun is inserted using
     *              RDbTable API.
     *
     * Param:       aSfx 
     *
     * Param:       aAudio 
     *
     * Param:       aDescription 
     */
    TInt AddTopgunWithSqlL(const TDesC& aSfx,
						   const TDesC& aAudio);

    /**
     * Function:    AddTopgunWithCppApiL
     *
     * Description: Adds a new Topgun to Topgun table. The Topgun is inserted using
     *              RDbTable API.
     *
     * Param:       aSfx 
     *
     * Param:       aAudio 
     *
     * Param:       aDescription 
     */
    TInt AddTopgunWithCppApiL(const TDesC& aSfx,
							  const TDesC& aAudio);

    /**
     * Function:    GetAllTopgunL
     *
     * Description: Retrieve all Topgun from database.
     *
     * Returns:     Array of Topgun. Each array item is represented as follows:
     *                  <Author>|<Title>|<Description>
     *              Maximum length of each item is KTopgunItemMaxLength
     *              Caller takes ownership of the array.
     */
    CDesCArrayFlat* GetAllTopgunL();

    /**
     * Function:    GetTopgunByKeyL
     *
     * Description: Retrieve Topgun from database, which match given column
     *              and search pattern. Implementation uses SQL.
     *
     * Param:       aColumnName Name of the column to apply the search pattern.
     *              Must be either KTopgunAuthorCol or KTopgunTitleCol
     *
     * Param:       aSearchString Search pattern used to restrict results to.
     *
     * Returns:     Array of Topgun. Each array item is represented as follows:
     *                  <Author>|<Title>|<Description>
     *              Maximum length of each item is KTopgunItemMaxLength
     *              Caller takes ownership of the array.
     */
    CDesCArrayFlat* GetTopgunByKeyL(const TDesC& aColumnName,
        const TDesC& aSearchString);

    /**
     * Function:    GetATopgunFastL
     *
     * Description: Retrieves Topgun information for given Topgun name. This method
     *              uses index to find first occurrence of the Topgun.
     *              Implementation uses exact match.
     *
     * Param:       aTitle is name of the Topgun to search for.
     *
     * Param:       aResult If there is a matching Topgun, the complete Topgun info
     *              is written to aResult. It is in the following format:
     *                  <Author>|<Title>|<Description>
     *              Length of the given descriptor must be KTopgunItemMaxLength
     */
    TInt GetATopgunFastL(const TDesC& aTitle, TDes& aResult);

    /**
     * Function:    RemoveTopgunL
     *
     * Description: Deletes Topgun(s) from database.
     *
     * Param:       aTitle is name of the Topgun to delete. It can contain
     *              wildcard characters (% for single char, * for zero or
     *              multiple chars).
     *
     * Param:       aResultCount will contain number of deleted Topgun.
     *
     * Returns:     KErrNone or one of the system wide error codes.
     */
    TInt RemoveTopgunL(const TDesC& aTitle, TInt& aResultCount);

    /**
     * Function:    RemoveAllTopgunL
     *
     * Description: Deletes all Topgun from database.
     *
     * Param:       aResultCount will contain number of deleted Topgun.
     *
     * Returns:     KErrNone or one of the system wide error codes.
     */
    TInt RemoveAllTopgunL(TInt& aResultCount);


    /**
     * Function:    UpdateTopgunTitle
     *
     * Description: Changes the title for a Topgun (or Topgun, if there are
     *              multiple Topgun with the name aOldTitleKey).
     *
     * Param:       aOldTitleKey Topgun title used for getting Topgun for update.
     *
     * Param:       aNewTitle New title for the Topgun(s).
     *
     * Returns:     KErrNone or one of the system wide error codes.
     */
    TInt UpdateTopgunTitle(const TDesC& aOldTitleKey, const TDesC& aNewTitle);



    /**
     * Function:    ColumnNamesAndSizes
     *
     * Description: Get array of colum names in the Topgun table. The result
     *              array includes also the size of the textual columns.
     *              This is here just to demonstrate iterating column names
     *              from a table.
     *
     * Returns:     Array of column names. Caller takes ownership.
     */
    CDesCArrayFlat* ColumnNamesAndSizes();

    /**
     * Function:    HasDateColumn
     *
     * Description: Tests whether the Topgun table has date column.
     *
     * Returns:     KErrNone or one of the system wide error codes.
     */
    TInt HasDateColumn(TBool& aReturnValue);

    /**
     * Function:    AddDateColumnL
     *
     * Description: Adds date column to Topgun table. This here just to
     *              demonstrate how to alter table definition.
     *              This fails, if the date column already exists.
     *
     * Returns:     KErrNone or one of the system wide error codes.
     */
    TInt AddDateColumnL();

    /**
     * Function:    RemoveDateColumn
     *
     * Description: Removes date column from Topgun table. This here just to
     *              demonstrate how to alter table definition.
     *              This fails, if the date column does not exist.
     *
     * Returns:     KErrNone or one of the system wide error codes.
     */
    TInt RemoveDateColumn();

private: // Construction

    /**
     * Function:    ConstructL
     *
     * Description: Perform the second phase construction of a CTopgunDb
     *              object
     */
    void ConstructL();

    /**
     * Function:    CTopgunDb
     *
     * Description: Perform the first phase of two phase construction.
     */
    CTopgunDb();

private: // Helpers

    void CreateTopgunTableL();     /* Create the Topgun table                  */
    void CreateTopgunIndexL();     /* Create the TopgunIndex for Topgun table   */
    void DropTopgunTableL();       /* Drop Topgun table (DDL example)          */

private: // Member data

    RFs              iFsSession;  /* For use of iFileStore                   */
    RDbStoreDatabase iTopgunDb;/* For database operations                 */
    CFileStore*      iFileStore;  /* For creating and opening database files */
    TBool            iOpen;       /* Flag indicating iTopgunDb open status*/
    };


#endif // __TOPGUNDBENGINE_H__