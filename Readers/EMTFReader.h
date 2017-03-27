#ifndef EMTFReader_h
#define EMTFReader_h

#include "AppFramework/AppParameter.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"

#include "AnObjects/EMTFTrackSep2016.h"

class EMTFReader : public AppModule {
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

        EMTFReader(const char *nm, const char *descr):AppModule(nm,descr){}
	virtual ~EMTFReader(void){}
};


#endif
