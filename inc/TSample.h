   /*
    *
============================================================================
    *  Name     : TSample.h
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

#ifndef __TSAMPLE_H__
#define __TSAMPLE_H__

// INCLUDES
#include <e32std.h>

// CLASS DECLARATION
/**
*	This type is a sound sample class for sound mixer.
*	These samples support 16-bit samples with definable loops.
*	If loop is defined, the whole sample is first played and
*	then keeps repeating the repeat area.
*/
class TSample
	{
	public:
		inline TSample()
			: iData( NULL )
			{}

		inline TSample( TInt16* aData, TInt aLength )
			: iData( aData )
			, iLength( aLength )
			, iRepStart( 0 )
			, iRepEnd( 0 )
			{}


		inline TSample( TInt16* aData, TInt aLength, TInt aRepStart, TInt aRepEnd )
			: iData( aData )
			, iLength( aLength )
			, iRepStart( aRepStart )
			, iRepEnd( aRepEnd )
			{}


		TInt16* iData;
		TInt	iLength;
		TInt	iRepStart;
		TInt	iRepEnd;
	};


#endif