////////////////////////////////////////////////////////////////////////
//
// Tilemap.cpp
//
// Copyright (c) 2003 Nokia Mobile Phones Ltd.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#include <e32math.h>
#include "RenderableFactory.h"
#include "Tileset.h"
#include "Tilemap.h"

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CTilemap* CTilemap::NewL(TInt aTilesWide,TInt aTilesHigh,const TInt* aMapData,CTileset& aTileset)
	{
	CTilemap* self = new(ELeave) CTilemap();
	CleanupStack::PushL(self);
	self->ConstructL(aTilesWide,aTilesHigh,aMapData,aTileset);
	CleanupStack::Pop();
	return self;
	}

////////////////////////////////////////////////////////////////////////

CTilemap::~CTilemap()
	{
	FreeTiles();
	}

////////////////////////////////////////////////////////////////////////

void CTilemap::ConstructL(TInt aTilesWide,TInt aTilesHigh,const TInt* aMapData,CTileset& aTileset)
    {
	iTileLogWidth = aTileset.TileLogWidth();
	iTileLogHeight = aTileset.TileLogHeight();

	iTileSize.SetSize(1 << iTileLogWidth, 1 << iTileLogHeight);

	AllocTilesL(aTilesWide,aTilesHigh);

	const TInt* mapData = aMapData;

	for ( TInt y = 0 ; y < aTilesHigh ; y++ )
		{
		TInt mapTileIndex = GetTileIndex(0,y);
		for ( TInt x = 0 ; x < aTilesWide ; x++ )
			{
			TInt factoryTileIndex = *mapData;
			mapData++;
			iTiles[mapTileIndex] = aTileset.Renderable(factoryTileIndex);
			mapTileIndex++;
			}
		}
	}

////////////////////////////////////////////////////////////////////////

void CTilemap::AllocTilesL(TInt aTilesWide, TInt aTilesHigh)
{
	TInt tileCount = aTilesWide * aTilesHigh;
	iTiles = new MRenderable*[tileCount];
	User::LeaveIfNull(iTiles);
	
	iSizeInTiles.SetSize(aTilesWide, aTilesHigh);
}

////////////////////////////////////////////////////////////////////////

void CTilemap::FreeTiles()
{
	delete[] iTiles;
	iTiles = NULL;
}

////////////////////////////////////////////////////////////////////////

TInt CTilemap::GetTileIndex(TInt aX, TInt aY) const
{
	return aX + (iSizeInTiles.iWidth * aY);
}

////////////////////////////////////////////////////////////////////////

TPoint CTilemap::PixelToTile( const TPoint& aScreenCoord ) const
	{
	return TPoint( aScreenCoord.iX >> iTileLogWidth, aScreenCoord.iY >> iTileLogHeight );
	}

////////////////////////////////////////////////////////////////////////

TPoint CTilemap::TileToPixel( const TPoint& aScreenCoord ) const
	{
	return TPoint( aScreenCoord.iX << iTileLogWidth, aScreenCoord.iY << iTileLogHeight );
	}

////////////////////////////////////////////////////////////////////////

void CTilemap::Render(const TPoint& aOrigin,const TRect& aScreenRect,CFbsBitGc* aCallerGc) const
	{
	// Find all tiles required to render screen rectangle:
	TRect pixelBoundingRect(aOrigin,aScreenRect.Size());
	TRect tileBoundingRect(PixelToTile(pixelBoundingRect.iTl),PixelToTile(pixelBoundingRect.iBr));

	// Resize to include fragmentary tiles on bottom and right edges;
	tileBoundingRect.Resize(1,1);

	// Intersect this with the bounds of this map:
	TRect tilesAvailableRect(TPoint(0,0),iSizeInTiles);
	tileBoundingRect.Intersection(tilesAvailableRect);

	TPoint rowTileOrigin = aOrigin - TileToPixel( tileBoundingRect.iTl );

	for (int tileY = tileBoundingRect.iTl.iY ; tileY < tileBoundingRect.iBr.iY ; tileY++)
		{
		TPoint tileOrigin = rowTileOrigin;
		TInt tileIndex = GetTileIndex(tileBoundingRect.iTl.iX,tileY);

		for (int tileX = tileBoundingRect.iTl.iX ; tileX < tileBoundingRect.iBr.iX ; tileX++)
			{
			iTiles[tileIndex]->Render(tileOrigin,aScreenRect,aCallerGc);
			tileIndex++;
			tileOrigin.iX -= iTileSize.iWidth;
			}

		rowTileOrigin.iY -= iTileSize.iHeight;
		}
	}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

