#ifndef EMTFReaderSep2016_h
#define EMTFReaderSep2016_h

#include "AppFramework/AppParameter.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"

#include "AnObjects/EMTFTrack.h"

class EMTFReaderSep2016 : public AppModule {
private:
        EMTFTrackCollection tracks;

protected:
        AppResult beginJob(AppEvent& event){ return AppResult(); }
        AppResult beginRun(AppEvent& event);
        AppResult event (AppEvent& event);
        AppResult endRun(AppEvent& event){ return AppResult(); }
        AppResult endJob(AppEvent& event){ return AppResult(); }

public:
	//virtual const MuonCollection& getMuons(void){ return muons; }

        EMTFReaderSep2016(const char *nm, const char *descr):AppModule(nm,descr){}
	virtual ~EMTFReaderSep2016(void){}
};


#endif
