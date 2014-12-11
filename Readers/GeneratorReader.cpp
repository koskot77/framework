using namespace std;
#include "GeneratorReader.h"
#include "TTree.h"
#include "TBranch.h"
#include "AnObjects/LundCodesToNames.h"

#include "ProductArea/BNcollections/interface/BNmcparticle.h"
#include "DataFormats/Common/interface/Wrapper.h"

edm::Wrapper<BNmcparticleCollection> *__genParticles = new edm::Wrapper<BNmcparticleCollection>();

AppResult GeneratorReader::beginRun(AppEvent& event) {
    TTree *Events = 0;
    if( event.get("Events",Events) || !Events )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'Events' tree found");

    TBranch *inputGen = Events->GetBranch("BNmcparticles_BNproducer_MCstatus3_BEANs.");
    if( !inputGen ) return AppResult(AppResult::STOP|AppResult::ERROR,"No 'BNmcparticles_BNproducer_MCstatus3_BEANs.' branch found");
    inputGen->SetAddress(&__genParticles);

    return AppResult();
}

AppResult GeneratorReader::event(AppEvent& event) {

    genParticles.clear();

    for(size_t i=0; i<__genParticles->product()->size(); i++){
        const BNmcparticle &p = (*__genParticles->product())[i];
        ParticlePointer particle( new Particle(p.energy, p.px, p.py, p.pz) );
        particle->setXYZ   ( p.vx, p.vy, p.vz );
        particle->setName  ( LundCodesToNames::name(p.id) );
        particle->setCharge( p.charge );
        particle->setPdgId ( p.id  );
        particle->setStatus( p.status );
        particle->setMotherPdgId( p.motherId );
        particle->setDaughter1PdgId( p.daughter0Id );
        particle->setDaughter2PdgId( p.daughter1Id );
        genParticles.push_back(particle);
    }

    event.put("genParticles",(const ParticleCollection*)&genParticles);

    return AppResult();
}
