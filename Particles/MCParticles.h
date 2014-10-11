#ifndef MCParticles_h
#define MCParticles_h
#include <vector>
#include "AppFramework/AppEvent.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppParameter.h"
#include "Particle.h"

class MCParticles:public AppModule{
private:
	AppParameter<bool> createMCPrimary;
	AppParameter<bool> createMCHistory;
	AppParameter<bool> placeParticlesAsReal;

	vector<Particle> part_pool;   // Recycle particles from this pool rather than new/delete them each time
	vector<Particle> particles;
	void expand(int size){ while(size--) part_pool.push_back( Particle() ); }

	AppResult beginJob(AppEvent &event){ return AppResult(); }
	AppResult beginRun(AppEvent &event){ return AppResult(); }
	AppResult event (AppEvent &event);
	AppResult endRun(AppEvent &event){ return AppResult(); }
	AppResult endJob(AppEvent &event){ return AppResult(); }

public:
//	const vector<Particle>& particles(void) const { return particles; }

	MCParticles(const char *nm, const char *descr):AppModule(nm,descr),
		createMCPrimary(*this,"createMCPrimary",true),
		createMCHistory(*this,"createMCHistory",false),
		placeParticlesAsReal(*this,"placeParticlesAsReal",false){ expand(20); }
	~MCParticles(void){}
};

#endif
