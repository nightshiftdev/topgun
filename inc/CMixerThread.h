#ifndef __CMIXERTHREAD_H_
#define __CMIXERTHREAD_H_
 
#include <E32Base.h>
#include <mdaaudiooutputstream.h>
#include <mda\common\audio.h>
 
#include "TAudioShared.h"
 
class MyActive;
 
class CMixerThread : CBase, MMdaAudioOutputStreamCallback
    {
    public:  
        /**
         * Thread entry point.
         */
        static TInt ThreadFunction(TAny* aData);
 
        /**
         * Default destructor.
         */
        ~CMixerThread();
    
        /**
         * Exception handler.
         */
        void HandleExceptionL();
    
    private:  // Constructors
        /**
         * Default constructor.
         * @param aData data from the creator thread
         */
        CMixerThread(TAny* aData);
 
        /**
         * Two-phased constructor.
         * @param aData data from the creator thread
         */
        static CMixerThread* CreateL(TAny* aData);
 
        /**
         * Second phase constructor.
         */
        TInt Construct();
 
        /**
         * Second phase constructor.
         */
        void ConstructL();
 
    private:  // New functions
        /**
         * Starts the mixer.
         */
        void StartMixerL();
 
        /**
         * Stops the mixer.
         */
        void StopMixer();
 
        /**
         * Fill mixing buffer with new data.
         */
        void FillBufferL();
        
    private:  // Methods from base classes
    
        /**
         * From MMdaAudioOutputStreamCallback.
         */
        void MaoscPlayComplete(TInt aError);
        
        /**
         * From MMdaAudioOutputStreamCallback.
         */
        void MaoscBufferCopied(TInt aError, const TDesC8& aBuffer);
 
        /**
         * From MMdaAudioOutputStreamCallback.
         */
        void MaoscOpenComplete(TInt aError);
 
    private:  // Data
        CTrapCleanup* iCleanupStack;
        CActiveScheduler* iActiveScheduler;
        CMdaAudioOutputStream* iStream;
        TMdaAudioDataSettings iSettings;
        
        TInt16* iBuffer;  // Buffer to CMdaAudioOutput
        TInt* iMixBuffer; // 32-bit buffer to mixing
        TPtrC8 iBufferPtr; // Pointer to iBuffer
        
        TAudioShared& iShared;  // Reference to shared data with client
        MyActive* iActive;
        
        // Current sample data pointers
        TInt16* iAudioData[KMaxChannels];
        
        // These are shifted by KAudioShift
        TInt iAudioPos[KMaxChannels];
        TInt iAudioEnd[KMaxChannels];
        TInt iRepStart[KMaxChannels];
        TInt iRepEnd[KMaxChannels];
    };
  
#endif /*__CMIXERTHREAD_H_*/
