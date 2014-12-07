#ifndef BasicReader_h
#define BasicReader_h

#include "AppFramework/AppParameter.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"

class BasicReader : public AppModule {
private:
        int    eventNumber, runNumber;
        double rho;
        double vx, vy, vz;

protected:
        AppResult beginJob(AppEvent& event){ return AppResult(); }
        AppResult beginRun(AppEvent& event);
        AppResult event (AppEvent& event);
        AppResult endRun(AppEvent& event){ return AppResult(); }
        AppResult endJob(AppEvent& event){ return AppResult(); }

public:

        BasicReader(const char *nm, const char *descr):AppModule(nm,descr){}
	virtual ~BasicReader(void){}
};


#endif
