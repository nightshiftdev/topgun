#ifndef _GAMETIMER_H_
#define _GAMETIMER_H_

#include <e32base.h>
#include <e32std.h>

class MGameTimerObserver;

class CGameTimer : public CBase
	{
	public:
		enum { StopTicking, TickAgain };

	public:
		static CGameTimer* NewL(MGameTimerObserver& aObserver);
		static CGameTimer* NewLC(MGameTimerObserver& aObserver);

		~CGameTimer();

		void Restart();
		TBool IsActive();		
		void CancelTimer();

	protected:
		CGameTimer(MGameTimerObserver& aObserver);

	private:
		void ConstructL();

		TInt HandleGameTimerCompleted();
		TInt HandleGameTimerCompletedL();

		static TInt TimerCompleted(TAny* aObject);

	private:
		MGameTimerObserver& iObserver;
		CPeriodic* iPeriodicTimer;
	};

		


#endif