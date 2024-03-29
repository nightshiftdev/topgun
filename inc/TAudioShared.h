   /*
    *
============================================================================
    *  Name     : TAudioShared.h
    *  Part of  : SoundMixer
    *  Created  : 03/01/2003 by ilPadrino group
    *  Description:
    *     This is the project specification file for SoundMixer.
    *     Initial content was generated by Series 60 AppWizard.
    *
    *  Version  : 1.0.0
    *  Copyright: ilPadrino group
    *
============================================================================
    */

#ifndef __TAUDIOSHARED_H__
#define __TAUDIOSHARED_H__

// INCLUDES
#include <e32std.h>
#include "TSample.h"

// CONSTANTS
const TInt KMaxChannels = 16;
const TInt KAudioShift = 12;

// ENUMERATIONS
enum TMixerCmd
	{
	ECmdStartMixer = 0,
	ECmdStopMixer,
	ECmdDestroyMixer
	};

// CLASS DECLARATION
/**
*	TAudioShared is internal type for sound mixer application.
*	This struct is used for inter-thread data trasfer and
*	communication
*/

class TAudioShared
    {
    public:
        /**
         * For thread death signaling.
         */
        RSemaphore iAliveMutex;
 
        /**
         * For sample attribute change signaling.
         */
        RMutex iMutex;
 
        // Sample attributes.
        TSample iSample[KMaxChannels];
        TInt iVolume[KMaxChannels];
        TInt iFrequency[KMaxChannels];
        TBool iPlayStarted[KMaxChannels];
        TExcType iExc;
        TRequestStatus* iStatusPtr;
 
        /**
         * Main volume.
         */
        TInt iMainVolume;
 
        /**
         * Command parameter.
         */
        TMixerCmd iCmd;
    };

#endif
