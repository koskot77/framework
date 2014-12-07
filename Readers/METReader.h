#ifndef METReader_h
#define METReader_h

#include "AppFramework/AppParameter.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"

#include "AnObjects/MET.h"

class METReader : public AppModule {
private:
	MET met;

protected:
        AppResult beginJob(AppEvent& event){ return AppResult(); }
        AppResult beginRun(AppEvent& event);
        AppResult event (AppEvent& event);
        AppResult endRun(AppEvent& event){ return AppResult(); }
        AppResult endJob(AppEvent& event){ return AppResult(); }

public:
	//virtual const METCollection& getMETs(void){ return muons; }

        METReader(const char *nm, const char *descr):AppModule(nm,descr){}
	virtual ~METReader(void){}
};


#endif
