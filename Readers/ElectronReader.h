#ifndef ElectronReader_h
#define ElectronReader_h

#include "AppFramework/AppParameter.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"

#include "AnObjects/Electron.h"

class ElectronReader : public AppModule {
private:
	ElectronCollection electrons;

protected:
        AppResult beginJob(AppEvent& event){ return AppResult(); }
        AppResult beginRun(AppEvent& event);
        AppResult event (AppEvent& event);
        AppResult endRun(AppEvent& event){ return AppResult(); }
        AppResult endJob(AppEvent& event){ return AppResult(); }

public:
	//virtual const MuonCollection& getMuons(void){ return muons; }

        ElectronReader(const char *nm, const char *descr):AppModule(nm,descr){}
	virtual ~ElectronReader(void){}
};


#endif
