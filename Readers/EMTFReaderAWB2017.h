#ifndef EMTFReaderAWB2017_h
#define EMTFReaderAWB2017_h

#include "AppFramework/AppParameter.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"

#include "AnObjects/EMTFTrackSep2016.h"

class EMTFReaderAWB2017 : public AppModule {
private:
        EMTFTrackSep2016Collection tracks;

protected:
        AppResult beginJob(AppEvent& event){ return AppResult(); }
        AppResult beginRun(AppEvent& event);
        AppResult event (AppEvent& event);
        AppResult endRun(AppEvent& event){ return AppResult(); }
        AppResult endJob(AppEvent& event){ return AppResult(); }

public:
	//virtual const MuonCollection& getMuons(void){ return muons; }

        EMTFReaderAWB2017(const char *nm, const char *descr):AppModule(nm,descr){}
	virtual ~EMTFReaderAWB2017(void){}
};


#endif
