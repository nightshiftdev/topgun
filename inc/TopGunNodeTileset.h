////////////////////////////////////////////////////////////////////////
//
// TopGunNodeTileset.h
//
// Copyright (c) 2003 ilPadrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef __TOPGUNNODETILESET_H
#define __TOPGUNNODETILESET_H

#include "Tileset.h"

////////////////////////////////////////////////////////////////////////

class CTopGunLeafTileset;

////////////////////////////////////////////////////////////////////////

class CTopGunNodeTileset : public CTileset 
	{
public:
	static CTopGunNodeTileset* NewL(CTopGunLeafTileset& aLeafFactory);
	~CTopGunNodeTileset();

protected:
	CTopGunNodeTileset();
    void ConstructL(CTopGunLeafTileset& aLeafFactory);

	enum { KTopLeftIsForeground		= 1 };
	enum { KTopRightIsForeground	= 2 };
	enum { KBottomLeftIsForeground	= 4 };
	enum { KBottomRightIsForeground	= 8 };

private:
	void CopyData(TInt aSrc[], TInt aDest[]);
	void MergeData(TInt aSrc[], TInt aDest[]);
	};

////////////////////////////////////////////////////////////////////////

#endif

