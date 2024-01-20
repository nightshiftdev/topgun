////////////////////////////////////////////////////////////////////////
//
// Level.h
//
// Copyright (c) 2004 Il Padrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <e32base.h>

class CLevel : public CBase
    {
    public:

        static CLevel* NewL();
        static CLevel* NewLC();
        virtual ~CLevel();

	private:
		
		CLevel();
        void ConstructL();

    private:
		//bitmaps
		//level array
		//setings
    };
#endif