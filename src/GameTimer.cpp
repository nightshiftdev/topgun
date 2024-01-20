////////////////////////////////////////////////////////////////////////
//
// GameTimer.cpp
//
// Copyright (c) 2003 Nokia Mobile Phones Ltd.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#include "GameTimer.h"


#include <e32svr.h>
#include <coemain.h>

#include "GameTimerObserver.h"

const TInt KCallbackInterval = 10000;

CGameTimer* CGameTimer::NewLC(MGameTimerObserver& aObserver)
	{
	CGameTimer* self = new (ELeave) CGameTimer(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CGameTimer* CGameTimer::NewL(MGameTimerObserver& aObserver)
	{
	CGameTimer* self = CGameTimer::NewLC(aObserver);
	CleanupStack::Pop();
	return self;
	}

CGameTimer::CGameTimer(MGameTimerObserver& aObserver):
	iObserver(aObserver)
	{
	}

void CGameTimer::ConstructL()
	{
	iPeriodicTimer = CPeriodic::NewL(CActive::EPriorityIdle);
//	iPeriodicTimer = CPeriodic::NewL(CActive::EPriorityLow);
	}

void CGameTimer::Restart()
	{
	iPeriodicTimer->Start(0,KCallbackInterval,TCallBack(CGameTimer::TimerCompleted,this));
	}

void CGameTimer::CancelTimer()
	{
	iPeriodicTimer->Cancel();
	}

TBool CGameTimer::IsActive()
	{
	return iPeriodicTimer->IsActive();
	}

CGameTimer::~CGameTimer()
	{
	delete iPeriodicTimer;
	iPeriodicTimer = 0;
	}

TInt CGameTimer::TimerCompleted(TAny* aObject)
	{
	return ((CGameTimer*)aObject)->HandleGameTimerCompleted();
	}

TInt CGameTimer::HandleGameTimerCompleted()
	{
	TInt result = iObserver.DoGameFrame();
	if(result == StopTicking)
		{
		iPeriodicTimer->Cancel();
		}
	return result;
	}

