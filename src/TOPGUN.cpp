// INCLUDE FILES
#include <eikstart.h>
#include "TOPGUNApp.h"

LOCAL_C CApaApplication* NewApplication()
	{
	return new CTOPGUNApp;
	}

GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication( NewApplication );
	}

