#ifndef HLTReader_h
#define HLTReader_h

#include "AppFramework/AppParameter.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"

#include "AnObjects/HLTEvent.h"

class HLTReader : public AppModule {
private:
        HLTObjectCollection objects;

protected:
        AppResult beginJob(AppEvent& event){ return AppResult(); }
        AppResult beginRun(AppEvent& event);
        AppResult event (AppEvent& event);
        AppResult endRun(AppEvent& event){ return AppResult(); }
        AppResult endJob(AppEvent& event){ return AppResult(); }

public:
	//virtual const METCollection& getMETs(void){ return muons; }

        HLTReader(const char *nm, const char *descr):AppModule(nm,descr){}
	virtual ~HLTReader(void){}
};


#endif
