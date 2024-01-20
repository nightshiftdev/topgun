#ifndef MYACTIVE_H_
#define MYACTIVE_H_

#include <e32std.h>
 
class CMixerThread;
 
class MyActive : public CActive
    {
    public:  // Constructors
        void Request();
        MyActive(CMixerThread* aThread);
        
    private:  // Methods from base classes
        void DoCancel();
        void RunL();
 
    private:  // Data
        CMixerThread* iThread;
    };

#endif /*MYACTIVE_H_*/
