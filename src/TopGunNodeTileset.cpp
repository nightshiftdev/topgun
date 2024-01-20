////////////////////////////////////////////////////////////////////////
//
// TopGunNodeTileset.cpp
//
// Copyright (c) 2003 ilPadrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#include <e32math.h>
#include "RenderableFactory.h"
#include "Tilemap.h"
#include "TopGunLeafTileset.h"
#include "TopGunNodeTileset.h"

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CTopGunNodeTileset* CTopGunNodeTileset::NewL(CTopGunLeafTileset& aLeafFactory)
	{
	CTopGunNodeTileset* self = new(ELeave) CTopGunNodeTileset();
	CleanupStack::PushL(self);
	self->ConstructL(aLeafFactory);
	CleanupStack::Pop();
	return self;
	}

////////////////////////////////////////////////////////////////////////

CTopGunNodeTileset::CTopGunNodeTileset()
	{
	}

////////////////////////////////////////////////////////////////////////

CTopGunNodeTileset::~CTopGunNodeTileset()
	{
	}

////////////////////////////////////////////////////////////////////////

void CTopGunNodeTileset::ConstructL(CTopGunLeafTileset& aLeafFactory)
    {
	TInt dataBuffer[16];

	TInt mapBaseData[] = 
		{
		4,0,4,0,
		0,4,0,4,
		4,0,4,0,
		0,4,0,4,
		};
	TInt mapAdjustTopLeft[] = 
		{
		3,3,3,2,
		3,3,3,2,
		2,1,2,0,
		0,0,0,0,
		};
	TInt mapAdjustTopRight[] = 
		{
		0,1,3,3,
		0,1,3,3,
		0,1,3,3,
		0,0,1,2,
		};
	TInt mapAdjustBottomLeft[] = 
		{
		1,2,0,0,
		3,3,2,0,
		3,3,2,0,
		3,3,2,0,
		};
	TInt mapAdjustBottomRight[] = 
		{
		0,0,0,0,
		0,1,2,1,
		1,3,3,3,
		1,3,3,3,
		};

	AllocRenderablesL(16);

	iTileLogWidth = 2 + aLeafFactory.TileLogWidth();
	iTileLogHeight = 2 + aLeafFactory.TileLogHeight();

	for (TInt tile = 0 ; tile < 16 ; tile++ )
		{
		// Set tile to "all background":
		CopyData(mapBaseData,dataBuffer);

		// Merge in foreground corners as 
		if ((tile & KTopLeftIsForeground) != 0)
			{
			MergeData(mapAdjustTopLeft,dataBuffer);
			}

		if ((tile & KBottomLeftIsForeground) != 0)
			{
			MergeData(mapAdjustBottomLeft,dataBuffer);
			}

		if ((tile & KTopRightIsForeground) != 0)
			{
			MergeData(mapAdjustTopRight,dataBuffer);
			}

		if ((tile & KBottomRightIsForeground) != 0)
			{
			MergeData(mapAdjustBottomRight,dataBuffer);
			}

		AddRenderable(CTilemap::NewL(4,4,&dataBuffer[0],aLeafFactory));
		}
	}

////////////////////////////////////////////////////////////////////////

void CTopGunNodeTileset::CopyData(TInt aSrc[], TInt aDest[])
{
	for (TInt cell = 0 ; cell < 16 ; cell++ )
		{
		aDest[cell] = aSrc[cell];
		}
}

////////////////////////////////////////////////////////////////////////

void CTopGunNodeTileset::MergeData(TInt aSrc[], TInt aDest[])
{
	for (TInt cell = 0 ; cell < 16 ; cell++ )
		{
		aDest[cell] |= aSrc[cell];
		}
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
