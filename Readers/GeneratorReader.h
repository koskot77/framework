#ifndef GeneratorReader_h
#define GeneratorReader_h

#include "AppFramework/AppParameter.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"
#include "AnObjects/Particle.h"

class GeneratorReader : public AppModule {
private:
        ParticleCollection genParticles;

protected:
        AppResult beginJob(AppEvent& event);
        AppResult beginRun(AppEvent& event){ return AppResult(); }
        AppResult event (AppEvent& event);
        AppResult endRun(AppEvent& event){ return AppResult(); }
        AppResult endJob(AppEvent& event){ return AppResult(); }

public:

        GeneratorReader(const char *nm, const char *descr):AppModule(nm,descr){}
	virtual ~GeneratorReader(void){}
};


#endif
