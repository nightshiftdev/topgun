////////////////////////////////////////////////////////////////////////
//
// TopGunLeafTileset.h
//
// Copyright (c) 2003 Nokia Mobile Phones Ltd.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef __TOPGUNLEAFTILESET_H
#define __TOPGUNLEAFTILESET_H

#include "Tileset.h"

////////////////////////////////////////////////////////////////////////

class CTopGunLeafTileset : public CTileset 
	{
public:
	static CTopGunLeafTileset* NewL(CFbsBitmap& aSourceImage);
	~CTopGunLeafTileset();

protected:
	CTopGunLeafTileset();
    void ConstructL(CFbsBitmap& aSourceImage);
	void BuildTile(CFbsBitmap& aSourceImage,TInt frame_no,CFbsBitGc* aGc);

	enum { KRightIsForeground		= 1 };
	enum { KLeftIsForeground		= 2 };
	enum { KIsTopLeftBottomRight	= 4 };

private:
	TSize iSize;
	};

////////////////////////////////////////////////////////////////////////

#endif

