   /*
    *
============================================================================
    *  Name     : CSndMixer.cpp
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

#include "CSndMixer.h"
#include "CMixerThread.h"
#include <e32svr.h>

_LIT(KMixer, "Mixer");

CSndMixer* CSndMixer::NewL()
	{
	CSndMixer* self = new( ELeave )CSndMixer();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

void CSndMixer::ConstructL()
    {
    iShared.iMainVolume = 256;   // Default main volume
 
    // commented for testing
//    User::LeaveIfError(iShared.iAliveMutex.CreateLocal(1));
//    User::LeaveIfError(iShared.iMutex.CreateLocal());
//    User::LeaveIfError(iMixerThread.Create(KMixer,
//            CMixerThread::ThreadFunction,
//            KDefaultStackSize,
//            KMinHeapSize,
//            KMinHeapSize + 1000000,
//            &iShared));
// 
//    // Give all possible priority to audio
//    iMixerThread.SetPriority(EPriorityRealTime);
//    iMixerThread.Resume();
    }

CSndMixer::CSndMixer()
	{

	}

CSndMixer::~CSndMixer()
    {
    SendCmd(ECmdDestroyMixer);
    iShared.iAliveMutex.Wait();
    iShared.iAliveMutex.Close();
    iShared.iMutex.Close();
    }

void CSndMixer::Pause()
	{
	// check if mutex is already in wait ( pause mode )
	if( iPaused )
		{
		return;
		}

	iPaused = ETrue;

	// commented for testing
//	SendCmd( ECmdStopMixer );
	}

void CSndMixer::Resume()
	{
	//commented for testing
//	if( !iPaused )
//		{
//		return;
//		}
//	iPaused = EFalse;
//
//	SendCmd( ECmdStartMixer );
	}

void CSndMixer::Play(const TSample& aSample, TInt aChannel, TInt aFrequency,
        TInt aVolume)
    {
//    iShared.iMutex.Wait();
//    iShared.iPlayStarted[aChannel] = ETrue;
//    iShared.iSample[aChannel] = aSample;
//    iShared.iFrequency[aChannel] = aFrequency;
//    iShared.iVolume[aChannel] = aVolume;
//    iShared.iMutex.Signal();
// 
//    SendCmd(ECmdStartMixer);
    }


void CSndMixer::Stop( TInt aChannel )
	{
//	iShared.iMutex.Wait();
//	iShared.iPlayStarted[ aChannel ] = ETrue;
//	iShared.iSample[ aChannel ] = iEmptySample;
//	iShared.iMutex.Signal();	
	}


void CSndMixer::SetVolume( TInt aVolume )
	{
//	iShared.iMutex.Wait();
//	iShared.iMainVolume = aVolume;
//	iShared.iMutex.Signal();	
	}


TInt CSndMixer::Volume()
	{
	return iShared.iMainVolume;
	}
 
void CSndMixer::SendCmd(TMixerCmd aCmd)
    {
    iShared.iMutex.Wait();
    iShared.iCmd = aCmd;
    iShared.iMutex.Signal();
    iShared.iExc = EExcUserInterrupt;
 
    TRequestStatus* status = iShared.iStatusPtr;
    if (status->Int() == KRequestPending)
        {
        iMixerThread.RequestComplete(status, KErrNone);
        }
    }

// End of file



