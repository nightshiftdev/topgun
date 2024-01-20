   /*
    *
============================================================================
    *  Name     : CWavLoader.cpp
    *  Part of  : SoundMixer
    *  Created  : 03/01/2003 by Forum Nokia
    *  Description:
    *     This is the project specification file for SoundMixer.
    *     Initial content was generated by Series 60 AppWizard.
    *
    *  Version  : 1.0.0
    *  Copyright: Forum Nokia
    *
============================================================================
    */

#include "CWavLoader.h"
#include <e32svr.h>
#include <f32file.h>
#include <aknutils.h>

CWavLoader* CWavLoader::NewL()
	{
	CWavLoader* self = NewLC();
	CleanupStack::Pop( self );
	return self;
	}


CWavLoader* CWavLoader::NewLC()
	{
	CWavLoader* self = new( ELeave )CWavLoader();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

CWavLoader::~CWavLoader()
	{
	}

CWavLoader::CWavLoader()
	{

	}

void CWavLoader::ConstructL()
	{
	}




TSample CWavLoader::LoadL( const TFileName& aFileName )
	{
	TFileName name( aFileName );	
	CompleteWithAppPath( name );
	// windows build wants to install apps on rom drive ( "z:\" )
	// but we want the data from "c:\" instead
#ifdef __WINS__
	name[ 0 ] = 'C';
#endif


	RFs fs;
	User::LeaveIfError( fs.Connect() );
	RFile file;
	CleanupClosePushL( fs );
	CleanupClosePushL( file );
	User::LeaveIfError( file.Open( fs, name, EFileStream | EFileRead ) );
	TInt size;
	User::LeaveIfError( file.Size( size ) );

	TUint8* tbuf = new( ELeave )TUint8[ size ];

	TPtr8 ptr( (TUint8*)tbuf, size );
	User::LeaveIfError( file.Read( ptr ) );

	//pawel NOKIA bug
	file.Close();
	fs.Close();
	
	CleanupStack::PopAndDestroy( 2 ); // fs, file

	TInt16* buf = new( ELeave )TInt16[ size ];
	TInt i;
	for( i=0; i<size; i++ )
		{
		TInt v = tbuf[ i ];
		v -= 128;
		buf[ i ] = ( TInt16 )( v * 256 );
		}
	delete tbuf;
	
	TSample sample;
	sample.iData = buf;
	sample.iLength = size;
	sample.iRepStart = 0;
	sample.iRepEnd = 0;

	return sample;
	}