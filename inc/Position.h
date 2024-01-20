////////////////////////////////////////////////////////////////////////
//
// Position.h
//
// Copyright (c) 2004 Il Padrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef __POSITION_H__
#define __POSITION_H__

#include <w32std.h>
#include <e32base.h>

class CPosition : public CBase
    {
    public:

        static CPosition* NewL(const TPoint &aPos);
        static CPosition* NewLC(const TPoint &aPos);
        virtual ~CPosition();
		TPoint Position();

	private:
		
		CPosition();
        void ConstructL(const TPoint &aPos);

    private:

		TPoint iPosition;
    };
#endif