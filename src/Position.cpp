////////////////////////////////////////////////////////////////////////
//
// Position.cpp
//
// Copyright (c) 2004 Il Padrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////
#include "Position.h"

//System includes
#include <e32base.h>
#include <aknutils.h>
//User includes



////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CPosition* CPosition::NewLC(const TPoint &aPos)
	{
	CPosition* self = new (ELeave) CPosition();
	CleanupStack::PushL(self);
	self->ConstructL(aPos);
	return self;
	}

////////////////////////////////////////////////////////////////////////

CPosition* CPosition::NewL(const TPoint &aPos)
	{
	CPosition* self = CPosition::NewLC(aPos);
	CleanupStack::Pop();
	return self;
	}

////////////////////////////////////////////////////////////////////////

CPosition::CPosition()
	{
	}

////////////////////////////////////////////////////////////////////////

CPosition::~CPosition()
	{
	}

////////////////////////////////////////////////////////////////////////
	
void CPosition::ConstructL(const TPoint &aPos)
	{
	iPosition = aPos;
	}

////////////////////////////////////////////////////////////////////////

TPoint CPosition::Position()
	{
	return iPosition;
	}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////