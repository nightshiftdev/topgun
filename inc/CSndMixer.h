   /*
    *
============================================================================
    *  Name     : CSndMixer.h
    *  Part of  : SoundMixer
    *  Created  : 03/01/2003 by Forum Nokia
    *  Description:
    *     This is the project specification file for SoundMixer.
    *
    *  Version  : 1.0.0
    *  Copyright: Forum Nokia
    *
============================================================================
    */


#ifndef __CSNDMIXER_H__
#define __CSNDMIXER_H__

// INCLUDES
#include <E32Base.h>
#include "TSample.h"
#include "TAudioShared.h"

// CLASS DECLARATION

/**
*	CSndMixer is the main class for user to produce sound.
*	this class creates second thread where the main mixer resides.
*	Messages to the second thread are sent via exceptions.
*
*	Usage example:
*	CSndMixer* iMixer;
*	..
*	iMixer = CSndMixer->NewL();
*	// plays sample iSample in channel 0 with frequency 16000Hz
*	// and volume 300
*	iMixer->Play( iSample, 0, 16000, 300 );
*
*/
class CSndMixer
	: public CBase
	{
	public:

		/// Two-phased constructor
		static CSndMixer* NewL();

		/// Default destructor
		~CSndMixer();

	private:

		/// Default constructor
		CSndMixer();

		/// Second phase constructor
		void ConstructL();

	public:

		/// Pauses mixer
		void Pause();

		/// Resumes mixer
		void Resume();

		/// Plays a sample with given parameters
		/// @param aSample sample to play
		/// @param aChannel channel 0-15 to play the sample on
		/// @param aVolume volume to play sample with, default is 256
		void Play( const TSample& aSample, TInt aChannel, TInt aFrequency, TInt aVolume = 256 );

		/// Stops sample from playing in given channel
		/// @param aChannel channel to play sample in
		void Stop( TInt aChannel );

		/// Sets the main volume
		/// @param aVolume new main volume
		void SetVolume( TInt aVolume );

		/// returns main volume
		/// @return main volume
		TInt Volume();

	private:

		void SendCmd( TMixerCmd aCmd );

	private:

		TAudioShared	iShared;		// shared data with mixer thread
		RThread			iMixerThread;	// handle to mixer thread

		TSample			iEmptySample;	// empty sample, used to stop channel

		TBool			iPaused;        // paused flag

	};



#endif
