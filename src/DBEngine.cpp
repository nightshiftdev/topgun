/*
 * ============================================================================
 *  Name     : TopgunDb from DBEngine.cpp
 *  Part of  : TopgunDb
 *  Created  : 24.07.2004 by IlPadrino group
 *  Version  : 1.0
 *  Copyright: IlPadrino group.
 * ============================================================================
 */

#include <badesca.h>    // CDesCArrayFlat
#include <s32file.h>    // CFileStore & CPermanentFileStore
#include <bautils.h>    // file helpers
#include <eikenv.h>
#include "DBEngine.h"

// Implementation specific constants
const int KCustomSqlMaxLength = 256;
const int KArrayGranularity = 5;     // for CDesCArrayFlat

// ---------------------------------------------------------------------------
// CTopgunDb::NewL()
//
// Create instance of the Topgun database engine.
// ---------------------------------------------------------------------------
CTopgunDb* CTopgunDb::NewL()
    {
    CTopgunDb* tmp = new (ELeave)CTopgunDb();
    CleanupStack::PushL(tmp);
    tmp->ConstructL();
    CleanupStack::Pop();
    return tmp;
    }

// ---------------------------------------------------------------------------
// CTopgunDb::~CTopgunDb()
//
// Destructor of the Topgun database engine. Release resources.
// ---------------------------------------------------------------------------
CTopgunDb::~CTopgunDb()
    {
    Close();  // Just in case, if the user does not close this explicitely
    iFsSession.Close();
    }

// ---------------------------------------------------------------------------
// CTopgunDb::ConstructL()
//
// Second phase construction. Leaves, if RFs session cannot be created.
// ---------------------------------------------------------------------------
void CTopgunDb::ConstructL()
    {
    TInt err = iFsSession.Connect();
    if(err)
		{
        User::Leave(err);
		}
    }

// ---------------------------------------------------------------------------
// CTopgunDb::CTopgunDb()
//
// Constructor
// ---------------------------------------------------------------------------
CTopgunDb::CTopgunDb()
    {
    iOpen = EFalse;
    }

// ---------------------------------------------------------------------------
// CTopgunDb::OpenDbL()
//
// Open existing Topgun database for exclusive access.
// ---------------------------------------------------------------------------
TInt CTopgunDb::OpenDbL(const TFileName& aExistingTopgunFile)
    {
    Close();

    if(!BaflUtils::FileExists(iFsSession, aExistingTopgunFile))
        {
            return KErrNotFound;
        }

    iFileStore = CPermanentFileStore::OpenL(iFsSession,
											aExistingTopgunFile, 
											EFileRead|EFileWrite);
    iFileStore->SetTypeL(iFileStore->Layout());       // Set file store type
    iTopgunDb.OpenL(iFileStore,iFileStore->Root());
    iOpen = ETrue;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CTopgunDb::CreateDbL()
//
// Create a new database. The database will be in exclusive access mode.
// ---------------------------------------------------------------------------
TInt CTopgunDb::CreateDbL(const TFileName& aNewTopgunFile)
    {
    Close();

    // Create empty database file.
    iFileStore = CPermanentFileStore::ReplaceL(iFsSession,
											   aNewTopgunFile, 
											   EFileRead|EFileWrite);
    iFileStore->SetTypeL(iFileStore->Layout());       // Set file store type
    TStreamId id = iTopgunDb.CreateL(iFileStore);  // Create stream object
    iFileStore->SetRootL(id);            // Keep database id as root of store
    iFileStore->CommitL();               // Complete creation by commiting

    // Create Topgun tables and indexes
    CreateTopgunTableL();
    CreateTopgunIndexL();
    iOpen = ETrue;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CTopgunDb::RemoveDbL()
//
// First remove the Topgun table. Then remove the database file.
// ---------------------------------------------------------------------------
TInt CTopgunDb::RemoveDbL(const TFileName& aExistingTopgunFile)
    {
    Close();

    if(!BaflUtils::FileExists(iFsSession, aExistingTopgunFile))
        {
        return KErrNotFound;
        }

    // It is enough to delete the database file directly. Because this example
    // demonstrates DDL statements, it first opens and drops the Topgun table.

    OpenDbL(aExistingTopgunFile);
    DropTopgunTableL();
    Close();

    iFsSession.Delete(aExistingTopgunFile);
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CTopgunDb::Close()
//
// Close the database.
// ---------------------------------------------------------------------------
TInt CTopgunDb::Close()
    {
    iTopgunDb.Close();
    if(iFileStore)
        {
        delete iFileStore;
        iFileStore = NULL;
        }
    iOpen = EFalse;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CTopgunDb::IsOpen()
//
// Return open status of the database.
// ---------------------------------------------------------------------------
TBool CTopgunDb::IsOpen() const
    {
    return iOpen;
    }

// ---------------------------------------------------------------------------
// CTopgunDb::CreateTopgunTableL()
//
// Creates Topgun table. Leaves, if the table cannot be created.
// ---------------------------------------------------------------------------
void CTopgunDb::CreateTopgunTableL()
    {

    // Specify columns for Topgun table
    TDbCol sfxCol(KTopgunSfxCol, EDbColText);   // Using default length
	TDbCol audioCol(KTopgunAudioCol, EDbColText);   // Using default length
    /*
	TDbCol Col(KTopgunTitleCol, EDbColText, KTitleMaxLength);
    titleCol.iAttributes = TDbCol::ENotNull;
    TDbCol descriptionCol(KTopgunDescriptionCol, EDbColLongText); // Stream Data
	*/

    // Add the columns to column set
    CDbColSet* TopgunColSet = CDbColSet::NewLC();
    TopgunColSet->AddL(sfxCol);
    TopgunColSet->AddL(audioCol);
    //TopgunColSet->AddL(descriptionCol);

    // Create the Topgun table
    User::LeaveIfError(iTopgunDb.CreateTable(KTopgunTable,*TopgunColSet));
    CleanupStack::PopAndDestroy(TopgunColSet);
    }

// ---------------------------------------------------------------------------
// CTopgunDb::CreateTopgunIndexL()
//
// Creates an index for Topgun table. Leaves, if the index cannot be created.
// ---------------------------------------------------------------------------
void CTopgunDb::CreateTopgunIndexL()
    {
    // Create index consisting of two columns
    TDbKeyCol sfxCol(KTopgunSfxCol);
    TDbKeyCol audioCol(KTopgunAudioCol);

    CDbKey* index = CDbKey::NewLC();   // create index key set
    index->AddL(sfxCol);
    index->AddL(audioCol);
    User::LeaveIfError(iTopgunDb.CreateIndex(KTopgunIndexName, KTopgunTable, *index));
    CleanupStack::PopAndDestroy(index);
    }

// ---------------------------------------------------------------------------
// CTopgunDb::DropTopgunTableL()
//
// ---------------------------------------------------------------------------
void CTopgunDb::DropTopgunTableL()
    {
    _LIT(KDropTable, "DROP TABLE ");

    // Sql: DROP TABLE Topgun
    TBuf<KCustomSqlMaxLength> sqlStr;
    sqlStr.Append(KDropTable);
    sqlStr.Append(KTopgunTable);

    RDbIncremental incOp;
    TInt incStep = 0xFFFF;
    // Initialise Execution
    TInt incStat = incOp.Execute(iTopgunDb, sqlStr, incStep);
    while (incStep>0 && incStat==KErrNone)
        {
        incStat = incOp.Next(incStep); // Do the work
        }
    incOp.Close();
    }

// ---------------------------------------------------------------------------
// CTopgunDb::AddTopgunWithSqlL()
//
// Add a Topgun to database using RDbView and SQL
// ---------------------------------------------------------------------------
TInt CTopgunDb::AddTopgunWithSqlL(const TDesC& aSfx,
								  const TDesC& aAudio)
    {

    if(aSfx.Length()==0 || aAudio.Length()==0)
        {
        return KErrGeneral;
        }

    // Sql: SELECT Sfx, Audio FROM Topgun
    TBuf<KCustomSqlMaxLength> sqlStr;
    sqlStr.Append(_L("SELECT "));
    sqlStr.Append(KTopgunSfxCol);
    sqlStr.Append(_L(", "));
    sqlStr.Append(KTopgunAudioCol);
    sqlStr.Append(_L(" FROM "));
    sqlStr.Append(KTopgunTable);
	sqlStr.Append(_L(" ORDER BY "));
    sqlStr.Append(KTopgunSfxCol);
    sqlStr.Append(_L(", "));
    sqlStr.Append(KTopgunAudioCol);


    RDbView view;    // Create a view on the database
    User::LeaveIfError(view.Prepare(iTopgunDb, 
									TDbQuery(sqlStr, EDbCompareFolded)));
    User::LeaveIfError(view.EvaluateAll());

    view.InsertL();  // Insert a row. Column order matches sql select statement

    view.SetColL(1, aSfx);
    view.SetColL(2, aAudio);
    /*
	RDbColWriteStream writeStream;  // Use stream to insert the description
    writeStream.OpenLC(view, 3);
    writeStream.WriteL(aDescription);
    CleanupStack::Pop();
    writeStream.Close();
	*/

    view.PutL();     // Complete insertion

    view.Close();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CTopgunDb::AddTopgunWithCppApiL()
//
// Add a Topgun to database using RDbTable API
// ---------------------------------------------------------------------------
TInt CTopgunDb::AddTopgunWithCppApiL(const TDesC& aSfx,
								     const TDesC& aAudio)
    {

    if(aSfx.Length()==0 || aAudio.Length()==0)
        {
        return KErrGeneral;
        }

    // Create an updateable database table object
    RDbTable table;
    TInt err = table.Open(iTopgunDb, KTopgunTable, table.EUpdatable);
    User::LeaveIfError(err);

    CDbColSet* TopgunColSet = table.ColSetL();
    CleanupStack::PushL(TopgunColSet);

    table.Reset();
    table.InsertL();
    table.SetColL(TopgunColSet->ColNo(KTopgunSfxCol), aSfx); // col = 1
    table.SetColL(TopgunColSet->ColNo(KTopgunAudioCol), aAudio);   // col = 2

    CleanupStack::PopAndDestroy(TopgunColSet);

    table.PutL();    // Complete changes (the insertion)
    table.Close();

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CTopgunDb::GetAllTopgunL()
//
// Get array of all Topgun in database. Format of each array item is:
//      <sfx>|<audio>
// ---------------------------------------------------------------------------
CDesCArrayFlat* CTopgunDb::GetAllTopgunL()
    {
    TPtrC sfx, audio;
    TBuf<KDescriptionMaxLength> description;
    TBuf<KTopgunItemMaxLength> rowText;

    RDbTable table;
    TInt err = table.Open(iTopgunDb, KTopgunTable, table.EReadOnly);
    User::LeaveIfError(err);

    CDesCArrayFlat* resultArray = new (ELeave)CDesC16ArrayFlat(KArrayGranularity);
    CleanupStack::PushL(resultArray);

    table.Reset();
    CDbColSet* colSet = table.ColSetL();
    CleanupStack::PushL(colSet);

    for (table.FirstL(); table.AtRow(); table.NextL())
        {
        description.Zero();
        rowText.Zero();

        table.GetL();

        sfx.Set(table.ColDes(colSet->ColNo(KTopgunSfxCol)));
        audio.Set(table.ColDes(colSet->ColNo(KTopgunAudioCol)));


        rowText.Append(sfx);
        rowText.Append(KSeparator);
        rowText.Append(audio);

        resultArray->AppendL(rowText); // Copy rowText to resultArray
        }
    CleanupStack::PopAndDestroy(colSet);
    CleanupStack::Pop(resultArray);
    table.Close();

    return resultArray;
    }

// ---------------------------------------------------------------------------
// CTopgunDb::GetATopgunFastL()
//
// Get a Topgun using index. Format of the result is:
//      <sfx>|<audio>
// ---------------------------------------------------------------------------
TInt CTopgunDb::GetATopgunFastL(const TDesC& aTitle, TDes& aResult)
    {
    TInt err = KErrNone;
    TBuf<KDescriptionMaxLength> description; // Only 128 first characters read
    RDbTable rowset;

    TDbSeekKey seekKey(aTitle); // Initialize one-column seek key

    // Open view to "Topgun" table. Use index to browse the table.
    User::LeaveIfError(
        rowset.Open(iTopgunDb, KTopgunTable, rowset.EReadOnly));
    User::LeaveIfError(
        rowset.SetIndex(KTopgunIndexName));

    // Query colum numbers for author, title, and description
    CDbColSet* colSet = rowset.ColSetL();
    CleanupStack::PushL(colSet);
    TInt sfxColumnNo = colSet->ColNo(KTopgunSfxCol);
    TInt audioColumnNo = colSet->ColNo(KTopgunAudioCol);
    CleanupStack::PopAndDestroy(colSet);

    // Search the index for aTitle
    if( rowset.SeekL(seekKey) )
        {
        rowset.GetL();

        aResult.Zero();
        aResult.Append(rowset.ColDes(sfxColumnNo));
        aResult.Append(KSeparator);
        aResult.Append(rowset.ColDes(audioColumnNo));

        err = KErrNone;
        }
    else
        {
        err = KErrNotFound;
        }

    rowset.Close();
    return err;
    }

// ---------------------------------------------------------------------------
// CTopgunDb::GetTopgunByKeyL()
//
// Get array of Topgun from database according to column name and a search
// pattern. Format of each array item is:
//      <sfx>|<audio>
// ---------------------------------------------------------------------------
CDesCArrayFlat* CTopgunDb::GetTopgunByKeyL(const TDesC& aColumnName,
										   const TDesC& aSearchString)
    {

    TPtrC sfx, audio;
    TBuf<KTopgunItemMaxLength> rowText;

    // Sql: SELECT sfx, audio FROM Topgun
    //      WHERE "aColumnName LIKE aSearchString"
    TBuf<KCustomSqlMaxLength> sqlStr;
    sqlStr.Append(_L("SELECT "));
    sqlStr.Append(KTopgunSfxCol);
    sqlStr.Append(_L(", "));
    sqlStr.Append(KTopgunAudioCol);
    sqlStr.Append(_L(" FROM "));
    sqlStr.Append(KTopgunTable);
    sqlStr.Append(_L(" WHERE "));
    sqlStr.Append(aColumnName);
    sqlStr.Append(_L(" LIKE '"));
    sqlStr.Append(aSearchString);
  

    CDesCArrayFlat* resultArray = new (ELeave)CDesC16ArrayFlat(KArrayGranularity);
    CleanupStack::PushL(resultArray);

    // Create a view on the database
    RDbView view;
    User::LeaveIfError(
        view.Prepare(iTopgunDb, TDbQuery(sqlStr), view.EReadOnly));
    User::LeaveIfError(view.EvaluateAll());

    CDbColSet* colSet = view.ColSetL();
    CleanupStack::PushL(colSet);

    // Append each result row to array
    for (view.FirstL(); view.AtRow(); view.NextL())
        {
        view.GetL();

        sfx.Set(view.ColDes(colSet->ColNo(KTopgunSfxCol)));
        audio.Set(view.ColDes(colSet->ColNo(KTopgunAudioCol)));

        rowText.Append(sfx);
        rowText.Append(KSeparator);
        rowText.Append(audio);

        resultArray->AppendL(rowText);
        }
    CleanupStack::PopAndDestroy(colSet);
    view.Close();
    CleanupStack::Pop(resultArray);

    return resultArray;

    }

// ---------------------------------------------------------------------------
// CTopgunDb::RemoveTopgunL()
//
// Delete a Topgun using title pattern and RDbUpdate (DML)
// ---------------------------------------------------------------------------
TInt CTopgunDb::RemoveTopgunL(const TDesC& aTitle, TInt& aResultCount)
    {
    RDbUpdate updOp;

    // Sql: DELETE FROM Topgun WHERE Title LIKE 'aTitle'
    TBuf<KCustomSqlMaxLength> sqlStr;
    sqlStr.Append(_L("DELETE FROM "));
    sqlStr.Append(KTopgunTable);
    sqlStr.Append(_L(" WHERE "));
    sqlStr.Append(KTopgunAudioCol);
    sqlStr.Append(_L(" LIKE '"));
    sqlStr.Append(aTitle);
    sqlStr.Append(_L("'"));

    // Initialize execution and perform the first step.
    // Note: Execute() returns 0 (=KErrNone), but it does not affect database
    //       until Next() is called.
    TInt incStat = updOp.Execute(iTopgunDb, sqlStr, EDbCompareFolded);
    incStat = updOp.Next(); // This will leave, if Execute() failed.

    while( incStat == 1 ) // Just in case, if the operation has more steps
        {
        incStat = updOp.Next();
        }
    aResultCount = updOp.RowCount();
    updOp.Close();
    return incStat; // KErrNone or system wide error code
    }

// ---------------------------------------------------------------------------
// CTopgunDb::RemoveAllTopgunL()
//
// Delete Topgun using asynchronous API. (RDbUpdate and DML)
// This implementation is still synchronous, because it uses
// User::WaitForRequest. Normally asynchronous functionality should be hidden
// into active object and client callback interfaces.
// ---------------------------------------------------------------------------
TInt CTopgunDb::RemoveAllTopgunL(TInt& aResultCount)
    {
    _LIT(KDeleteFrom, "DELETE FROM ");

    // Sql: DELETE FROM Topgun
    TBuf<KCustomSqlMaxLength> sqlStr;
    sqlStr.Append(KDeleteFrom);
    sqlStr.Append(KTopgunTable);

    RDbUpdate updOp;
    TRequestStatus incStat(1);
    TInt updStat = updOp.Execute(iTopgunDb, sqlStr, EDbCompareFolded);
    while (updStat==KErrNone && incStat ==1)
        {
        updOp.Next(incStat);           // Start async operation. It returns
                                       // immediately.
        User::WaitForRequest(incStat); // For simplicity wait completion here.
        }

    aResultCount = updOp.RowCount();
    updOp.Close();

    if(updStat!=KErrNone)
        return updStat;       // System wide error code
    else
        return incStat.Int(); // KErrNone or system wide error code
    }


// ---------------------------------------------------------------------------
// CTopgunDb::UpdateTopgunTitle()
//
// Update Topgun title using SQL UPDATE.
// ---------------------------------------------------------------------------
//
TInt CTopgunDb::UpdateTopgunTitle(const TDesC& aOldTitleKey,
								  const TDesC& aNewTitle)
    {
    _LIT(KSQLUpdateStart, "UPDATE Topgun SET audio = '");
    _LIT(KSQLUpdateMiddle, "' WHERE audio = '");
    _LIT(KSQLUpdateEnd, "'");

    TBuf<KCustomSqlMaxLength> sqlStr;
    sqlStr.Append(KSQLUpdateStart);
    sqlStr.Append(aNewTitle);
    sqlStr.Append(KSQLUpdateMiddle);
    sqlStr.Append(aOldTitleKey);
    sqlStr.Append(KSQLUpdateEnd);

    return iTopgunDb.Execute(sqlStr);
    }


// ---------------------------------------------------------------------------
// CTopgunDb::AddDateColumnL()
//
// Get array of column names and sizes of the Topgun table.
// ---------------------------------------------------------------------------
CDesCArrayFlat* CTopgunDb::ColumnNamesAndSizes()
    {
    RDbTable TopgunTable;
    TBuf<32> columnNameAndSize;
    _LIT(KDelimiter, ": ");
    _LIT(KNoSize,"No size");

    // Open the Topgun table.
    User::LeaveIfError(
        TopgunTable.Open(iTopgunDb, KTopgunTable, TopgunTable.EReadOnly));
    CleanupClosePushL(TopgunTable);  // Remember to pop and close

    CDesCArrayFlat* resultArray =
        new (ELeave)CDesC16ArrayFlat(KArrayGranularity);
    CleanupStack::PushL(resultArray);

    // Iterate through the colums of Topgun table. Extract the column name and
    // column size (size only for text columns).´
    // Note: Description column is long text. Database limits its size
    //       only by hardware. If size is queried, it is -1
    CDbColSet* colSet = TopgunTable.ColSetL();
    CleanupStack::PushL(colSet);
    TDbColSetIter colIter(*colSet);
    while(colIter)
        {
        columnNameAndSize.Zero();
        columnNameAndSize.Append(colIter->iName);
        columnNameAndSize.Append(KDelimiter);
        if(colIter->iType == EDbColText)
            columnNameAndSize.AppendNum(colIter->iMaxLength);
        else
            columnNameAndSize.Append(KNoSize);
        resultArray->AppendL(columnNameAndSize);
        colIter++;
        }
    CleanupStack::PopAndDestroy(colSet);
    CleanupStack::Pop(resultArray);

    // Pop the TopgunTable from cleanup stack and close it.
    CleanupStack::PopAndDestroy();

    return resultArray;
    }

// ---------------------------------------------------------------------------
// CTopgunDb::HasDateColumn()
//
// Tests wheter the Topgun table has date column
// ---------------------------------------------------------------------------
TInt CTopgunDb::HasDateColumn(TBool& aReturnValue)
    {
    RDbTable TopgunTable;
    aReturnValue = EFalse;

    // Open the Topgun table.
    User::LeaveIfError(
        TopgunTable.Open(iTopgunDb, KTopgunTable, TopgunTable.EReadOnly));
    CleanupClosePushL(TopgunTable);  // Remember to pop and close

    // Iterate through the colums of Topgun table. Check whether there is
    // a 'PublishDate' column
    CDbColSet* colSet = TopgunTable.ColSetL();
    CleanupStack::PushL(colSet);
    TDbColSetIter colIter(*colSet);
    while(colIter)
        {
        if( (colIter->iName).Compare(KTopgunDateCol) == 0) // 0 = equal
            {
            aReturnValue = ETrue;
            break;
            }
        colIter++;
        }
    CleanupStack::PopAndDestroy(colSet);

    // Pop the TopgunTable from cleanup stack and close it.
    CleanupStack::PopAndDestroy();

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CTopgunDb::AddDateColumnL()
//
// Adds date column to Topgun table (DDL).
// ---------------------------------------------------------------------------
TInt CTopgunDb::AddDateColumnL()
    {
    _LIT(KSqlAddDate, "ALTER TABLE Topgun ADD PublishDate DATE");
    return iTopgunDb.Execute(KSqlAddDate);
    }

// ---------------------------------------------------------------------------
// CTopgunDb::RemoveDateColumn()
//
// Removes date column from Topgun table (DDL).
// ---------------------------------------------------------------------------
TInt CTopgunDb::RemoveDateColumn()
    {
    _LIT(KSqlRemoveDate, "ALTER TABLE Topgun DROP PublishDate");
    return iTopgunDb.Execute(KSqlRemoveDate);
    }