#include <e32base.h>
 
#include "MyActive.h"
#include "CMixerThread.h"
 
// Constructs the AO and sets its priority
MyActive::MyActive(CMixerThread* aThread) :
    CActive(CActive::EPriorityStandard),
    iThread(aThread)
    {
    // Adds the active object to the queue
    CActiveScheduler::Add(this);
    }
 
// Cancels this task
void MyActive::DoCancel()
    {
    TRequestStatus* status = &iStatus;
    RThread().RequestComplete(status, KErrCancel);
    }
 
// Request accepting function
void MyActive::Request()
    {
    SetActive();
    iStatus = KRequestPending;
    }
 
// Informs the requestor that there is user activity or the timer is fired
void MyActive::RunL()
    {
    iThread->HandleExceptionL();
    Request();
    }

// End of file
