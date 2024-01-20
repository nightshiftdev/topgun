////////////////////////////////////////////////////////////////////////
//
// KeyHandler.h
//
// ilPadrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef _KEYHANDLER_H_
#define _KEYHANDLER_H_

#include <e32std.h>

class TKeyHandler
	{
public:
	enum TDirection { ELeft, ERight, EUp, EDown, ENoDirection };

	TKeyHandler();
		
	TDirection Direction() const;
	TBool FirePressed() const;
	
	void DownPressed();
	void UpPressed();
	void LeftPressed();
	void RightPressed();
	void NoDirection();
	void PressedFire();
	void FireReleased();

private:
	TDirection iDirection;
	TBool iFirePressed;
	};

#endif	