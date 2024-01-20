////////////////////////////////////////////////////////////////////////
//
// KeyHandler.cpp
//
// ilPadrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#include "KeyHandler.h"


TKeyHandler::TKeyHandler() : 
iDirection(ENoDirection),
iFirePressed(EFalse)
	{
	}

TKeyHandler::TDirection TKeyHandler::Direction() const
	{
	return iDirection;
	}

TBool TKeyHandler::FirePressed() const
	{
	return iFirePressed;
	}

void TKeyHandler::UpPressed()
	{
	User::ResetInactivityTime();
	iDirection = EUp;
	}

void TKeyHandler::DownPressed()
	{
	User::ResetInactivityTime();
	iDirection = EDown;
	}

void TKeyHandler::LeftPressed()
	{
	User::ResetInactivityTime();
	iDirection = ELeft;
	}

void TKeyHandler::RightPressed()
	{
	User::ResetInactivityTime();
	iDirection = ERight;
	}

void TKeyHandler::NoDirection()
	{
	User::ResetInactivityTime();
	iDirection = ENoDirection;
	}

void TKeyHandler::PressedFire()
	{
	User::ResetInactivityTime();
	iFirePressed = ETrue;
	}

void TKeyHandler::FireReleased()
	{
	User::ResetInactivityTime();
	iFirePressed = EFalse;
	}

