#include <e32svr.h>
 
#include "CMixerThread.h"
#include "MyActive.h"
#include "TSample.h"
#include "TAudioShared.h"
 
const TInt KSampleRate = 16000;             // sample rate used
const TInt KBufferSize = KSampleRate / 20;  // 20 buffers per second
 
TInt CMixerThread::ThreadFunction(TAny* aData)
    {
    TAudioShared& shared = *((TAudioShared*)aData);
 
    // Tell the client we're alive
    shared.iAliveMutex.Wait();
    CMixerThread* mixerThread = NULL;
 
    TRAPD(error, mixerThread = CMixerThread::CreateL(aData));
    // TODO: Error handling
    
    shared.iStatusPtr = &(mixerThread->iActive->iStatus);
 
    // If we're still here, the active scheduler has been constructed.
    // Start the wait loop which runs until it's time to end the thread.
    CActiveScheduler::Start();
    delete mixerThread;
 
    // Tell the owning thread that it's safe to exit
    shared.iAliveMutex.Signal();
 
    return KErrNone;
    }
 
CMixerThread* CMixerThread::CreateL(TAny* aData)
    {
    CMixerThread* self = new (ELeave) CMixerThread(aData);
    // TODO: Error handling (self may be NULL)
    
    TInt constructErr = self->Construct();
    // TODO: Error handling (constructErr)
 
    return self;
    }
 
TInt CMixerThread::Construct()
    {
    iCleanupStack = CTrapCleanup::New();
 
    TInt err = KErrNone;
    TRAP(err, ConstructL());
    return err;
    }
 
void CMixerThread::ConstructL()
    {
    // Create the active scheduler
    iActiveScheduler = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(iActiveScheduler);
 
    // Sound inits
    iSettings.iChannels = TMdaAudioDataSettings::EChannelsMono;
    iSettings.iSampleRate = TMdaAudioDataSettings::ESampleRate16000Hz;
    iSettings.iVolume = 1;
 
    iMixBuffer = new (ELeave) TInt[KBufferSize];
    iBuffer = new (ELeave) TInt16[KBufferSize];
 
    iBufferPtr.Set(TPtrC8((TUint8*)iBuffer, KBufferSize * 2));
 
    iActive = new (ELeave) MyActive(this);
    iActive->Request();
    }
 
CMixerThread::CMixerThread(TAny* aData) :
    iShared(*((TAudioShared*)aData))
    {
    }
 
CMixerThread::~CMixerThread()
    {
    delete iStream;
    delete iBuffer;
    delete iMixBuffer;
    delete iActiveScheduler;
    delete iCleanupStack;
    if (iActive)
        {
        iActive->Cancel();
        }
    delete iActive;
    }
 
void CMixerThread::HandleExceptionL()
    {
    switch (iShared.iExc)
        {
        case EExcUserInterrupt: // Command from client
            {
            switch (iShared.iCmd)
                {
                case ECmdStartMixer:
                    {
                    StartMixerL();
                    break;
                    }
                case ECmdStopMixer:
                    {
                    StopMixer();
                    break;
                    }
                case ECmdDestroyMixer:
                    {
                    CActiveScheduler::Stop(); // Exit
                    break;
                    }
                }
            break;
            }
        default:
            {
            // On unknown exception, just exit this thread
            CActiveScheduler::Stop(); // Exit
            break;
            }
        }
    }
 
void CMixerThread::StartMixerL()
    {
    iStream = CMdaAudioOutputStream::NewL(*this);
    iStream->Open(&iSettings);
    }
 
void CMixerThread::StopMixer()
    {
    iStream->Stop();
    delete iStream;
    iStream = NULL;
    }
 
void CMixerThread::FillBufferL()
    {
    // Wait for access to shared data
    iShared.iMutex.Wait();
 
    TInt volume = iShared.iMainVolume;
 
    // Gather new sample information
    for (TInt i = 0; i < KMaxChannels; i++)
        {
        if (iShared.iPlayStarted[i])
            {
            iShared.iPlayStarted[i] = EFalse;
            TSample& sample = iShared.iSample[i];
 
            iAudioData[i] = sample.iData;
            iAudioPos[i] = 0;
            iAudioEnd[i] = sample.iLength << KAudioShift;
            iRepStart[i] = sample.iRepStart << KAudioShift;
            iRepEnd[i] = sample.iRepEnd << KAudioShift;
            }
        }
    // Give access to shared data
    iShared.iMutex.Signal();
 
    // Clear the buffer. This has to be done because channels are mixed by
    // adding their values to each other
    Mem::FillZ(iMixBuffer, KBufferSize * 4);
 
    // Mix active channels
    for (TInt i = 0; i < KMaxChannels; i++)
        {
        if (iAudioData[i] != NULL)
            {
            TInt* buf = iMixBuffer;
            TInt* bufEnd = buf + KBufferSize;
 
            TInt16* src = iAudioData[i];
 
            TInt pos = iAudioPos[i];
            TInt posEnd = iAudioEnd[i];
            TInt repStart = iRepStart[i];
            TInt repEnd = iRepEnd[i];
            TInt posAdd = (iShared.iFrequency[i] << KAudioShift) / KSampleRate;
            TInt volume = iShared.iVolume[i];
 
            while (buf < bufEnd)
                {
                TInt sample = (src[pos >> KAudioShift] * volume);
                pos += posAdd;
                if (pos >= posEnd)
                    {
                    if (repEnd == 0)
                        {
                        iAudioData[i] = NULL;
                        break;
                        }
                    else
                        {
                        pos = repStart;
                        posEnd = repEnd;
                        }
                    }
                *buf++ += sample;
                }
            iAudioPos[i] = pos;
            iAudioEnd[i] = posEnd;
            }
        }
 
    // Convert 32-bit mixing buffer to 16-bit output buffer
    TInt* buf = iMixBuffer;
    TInt* bufEnd = buf + KBufferSize;
    TInt16* buf2 = iBuffer;
    while (buf < bufEnd)
        {
        // 32-bit mixer buffer contents are multiplied by main volume.
        // Shifts are in two phases to prevent overflow and to maintain
        // quality.
        TInt value = ((*buf++ >> 8) * volume) >> 8;
 
        // Prevent sound from trashing on overboost volume
        if (value < -0x7FFF)
            value = -0x7FFF;
        if (value > 0x7FFF)
            value = 0x7FFF;
 
        // and write to buffer
        *buf2++ = (TInt16)value;
        }
 
    // Write 16-bit buffer to CMdaAudioOutputStream
    iStream->WriteL(iBufferPtr);
    }
 
void CMixerThread::MaoscBufferCopied(TInt aError, const TDesC8& aBuffer)
    {
    // TODO: Error handling (aError)
    
    TRAPD(error, FillBufferL());
    // TODO: Error handling (error)
    }
 
void CMixerThread::MaoscOpenComplete(TInt aError)
    {
    // TODO: Error handling (aError)
    
    iStream->SetVolume(iStream->MaxVolume());
    TRAPD(error, FillBufferL());
    // TODO: Error handling (error)
    }
 
void CMixerThread::MaoscPlayComplete(TInt aError)
    {
    // TODO: Error handling (aError)
    
    iStream->SetVolume(iStream->MaxVolume());
    TRAPD(error, FillBufferL());
    // TODO: Error handling (error)
    }

// End of file
