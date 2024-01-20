////////////////////////////////////////////////////////////////////////
//
// UtilsAnim.h
//
// Copyright (c) 2004 IlPadrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef _UTILSANIM_H_
#define _UTILSANIM_H_

#include "BaseAnim.h"

////////////////////////////////////////////////////////////////////////

class CUtilsAnim : public CBaseAnim
	{
public:
	static CUtilsAnim* NewL(CFbsBitmap& aSourceImageconst, 
							const TPoint &aScreenPos,
							const TSize &aSize);
	CUtilsAnim();
	~CUtilsAnim();
    void ConstructL(CFbsBitmap& aSourceImageconst, 
					const TPoint &aScreenPos,
					const TSize &aSize);
	void SetUtilsScreenPos(const TPoint &aUtilsScreenPos);

private:

	void BuildFrame(CFbsBitmap& aSourceImage,TInt frame_no,CFbsBitGc* aGc);

	//Utils's parameters
	TPoint iUtilsScreenPos;
	};

////////////////////////////////////////////////////////////////////////

#endif

