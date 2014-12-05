using namespace std;
#include "GeneratorReader.h"
#include "TTree.h"
#include "TBranch.h"

#include "ProductArea/BNcollections/interface/BNmcparticle.h"
#include "DataFormats/Common/interface/Wrapper.h"

edm::Wrapper<BNmcparticleCollection> *__genParticles = new edm::Wrapper<BNmcparticleCollection>();

AppResult GeneratorReader::beginJob(AppEvent& event) {
    TTree *Events;
    event.get("Events",Events);

    TBranch *inputGen = Events->GetBranch("BNmcparticles_BNproducer_MCstatus3_BEANs.");
    if( !inputGen ) return AppResult(AppResult::STOP|AppResult::ERROR,"No 'BNmcparticles_BNproducer_MCstatus3_BEANs.' branch found");
    inputGen->SetAddress(&__genParticles);

    return AppResult();
}

AppResult GeneratorReader::event(AppEvent& event) {

    genParticles.clear();

    for(size_t i=0; i<__genParticles->product()->size(); i++){
        const BNmcparticle &p = (*__genParticles->product())[i];
//        int st = p.status();  
//        const Candidate * mom = p.mother();
//        int n = p.numberOfDaughters();
//        for(size_t j = 0; j < n; ++ j) {
//            const Candidate * d = p.daughter( j );
//            int dauId = d->pdgId();
//        }
        ParticlePointer particle( new Particle(p.energy, p.px, p.py, p.pz) );
        particle->setXYZ   ( p.vx, p.vy, p.vz );
        particle->setName  ( "qwe" );
        particle->setCharge( p.charge );
        particle->setPdgId ( p.id  );
        particle->setStatus( p.status );
        genParticles.push_back(particle);
    }

    event.put("genParticles",(const ParticleCollection*)&genParticles);

    return AppResult();
}
