////////////////////////////////////////////////////////////////////////
//
// GameTimerObserver.h
//
// ilPadrino group.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef _GAMETIMEROBSERVER_H
#define _GAMETIMEROBSERVER_H

class MGameTimerObserver 
	{
	public:   
		virtual TInt DoGameFrame() = 0;		
	};

#endif //GAMETIMEROBSERVER_H