////////////////////////////////////////////////////////////////////////
//
// Tilemap.h
//
// Copyright (c) 2003 Nokia Mobile Phones Ltd.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef __TILEMAP_H
#define __TILEMAP_H

#include "RenderableFactory.h"

////////////////////////////////////////////////////////////////////////

class CTileset;

////////////////////////////////////////////////////////////////////////

class CTilemap : public CRenderable
	{
public:
	static CTilemap* NewL(TInt aTilesWide,TInt aTilesHigh,const TInt* aMapData,CTileset& aTileset);
	~CTilemap();
    void ConstructL(TInt aTilesWide,TInt aTilesHigh,const TInt* aMapData,CTileset& aTileset);

public:   
	void Render(const TPoint& aOrigin,const TRect& aScreenRect,CFbsBitGc* aCallerGc) const;		

private:
	TInt GetTileIndex(TInt aX,TInt aY) const;

	void AllocTilesL(TInt aTilesWide, TInt aTilesHigh);
	void FreeTiles();

	TPoint PixelToTile(const TPoint& aScreenCoord) const;
	TPoint TileToPixel(const TPoint& aScreenCoord) const;

private:
	TInt iTileLogWidth;
	TInt iTileLogHeight;
	TSize iTileSize;

	TSize iSizeInTiles;

	TInt iTilePosIdx;

	MRenderable** iTiles;
	};

////////////////////////////////////////////////////////////////////////

#endif

