////////////////////////////////////////////////////////////////////////
//
// RenderableFactory.h
//
// ilPadrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef __RENDERABLEFACTORY_H
#define __RENDERABLEFACTORY_H

#include <coeccntx.h>

////////////////////////////////////////////////////////////////////////

class MRenderable 
	{
public:   
	virtual void Render(const TPoint& aOrigin,const TRect& aScreenRect,CFbsBitGc* aCallerGc) const = 0;		
	};

////////////////////////////////////////////////////////////////////////

class CRenderable : public CBase, public MRenderable
	{
public:
	~CRenderable(){}

public:   
	void Render(const TPoint& aOrigin,const TRect& aScreenRect,CFbsBitGc* aCallerGc) const = 0;		

protected:
	CRenderable(){}
	};

////////////////////////////////////////////////////////////////////////

class CRenderableFactory : public CBase
	{
public:
	~CRenderableFactory();

public:   
	MRenderable* Renderable(int idx);
	void FreeMemory();

protected:
	void AllocRenderablesL(TInt aMaxRenderableCount);
	void AddRenderable(CRenderable* aRenderable);
	void FreeRenderables();

private:
	CRenderable** iRenderables;
	TInt iRenderableCount;
	TInt iMaxRenderableCount;
	};

////////////////////////////////////////////////////////////////////////

#endif

