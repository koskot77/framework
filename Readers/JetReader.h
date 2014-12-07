#ifndef JetReader_h
#define JetReader_h

#include "AppFramework/AppParameter.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"
#include "AnObjects/Jet.h"

class JetReader : public AppModule {
private:
	JetCollection jets;

protected:
        AppResult beginJob(AppEvent& event){ return AppResult(); }
        AppResult beginRun(AppEvent& event);
        AppResult event (AppEvent& event);
        AppResult endRun(AppEvent& event){ return AppResult(); }
        AppResult endJob(AppEvent& event){ return AppResult(); }

public:
	//virtual const JetCollection& getJets(void){ return jets; }

        JetReader(const char *nm, const char *descr):AppModule(nm,descr){}
	virtual ~JetReader(void){}
};


#endif
