#ifndef MuonReader_h
#define MuonReader_h

#include "AppFramework/AppParameter.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"

#include "AnObjects/Muon.h"

#include "TBranch.h"

#include "DataFormats/PatCandidates/interface/Muon.h"

class MuonReader : public AppModule {
private:
	MuonCollection       muons;
        TBranch*             bpm;
        pat::MuonCollection  patMuons;

protected:
        AppResult beginJob(AppEvent& event);
        AppResult beginRun(AppEvent& event){ return AppResult(); }
        AppResult event (AppEvent& event);
        AppResult endRun(AppEvent& event){ return AppResult(); }
        AppResult endJob(AppEvent& event){ return AppResult(); }

public:
	virtual const MuonCollection& getMuons(void){ return muons; }

        MuonReader(const char *nm, const char *descr):AppModule(nm,descr){}
	virtual ~MuonReader(void){}
};


#endif
