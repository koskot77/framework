using namespace std;
#include "GeneratorReader.h"
#include "AnObjects/LundCodesToNames.h"
#include "TTree.h"
#include "TBranch.h"
#include <map>
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"

edm::Wrapper< std::vector<reco::GenParticle> > *__genParticles = new edm::Wrapper< std::vector<reco::GenParticle> >();

AppResult GeneratorReader::beginJob(AppEvent& event) {
    TTree *Events;
    event.get("Events",Events);

    TBranch *inputGen = Events->GetBranch("recoGenParticles_prunedGenParticles__PAT.");
    if( !inputGen ) return AppResult(AppResult::STOP|AppResult::ERROR,"No 'recoGenParticles_prunedGenParticles__PAT.' branch found");
    inputGen->SetAddress(&__genParticles);

    return AppResult();
}

//pair< string, pair<string,string> > GeneratorReader::split(string decay){ "2[9000001,6[5,24[..]]]"; }

AppResult GeneratorReader::event(AppEvent& event) {

    genParticles.clear();

//    map<int, vector<ParticlePointer> > lookup; // hash table by pdgId

    for(vector<reco::GenParticle>::const_iterator p = __genParticles->product()->begin(); p != __genParticles->product()->end(); p++){
        ParticlePointer particle( new Particle(p->energy(), p->px(), p->py(), p->pz()) );
        particle->setXYZ   ( p->vx(), p->vy(), p->vz() );
        particle->setName  ( LundCodesToNames::name(p->pdgId()) );
        particle->setCharge( p->charge() );
        particle->setPdgId ( p->pdgId()  );
        particle->setStatus( p->status() );
        genParticles.push_back(particle);
//        lookup[ p->pdgId() ].push_back(particle);
    }
/*
    // take care of mothers/daughters in the second pass
    vector<reco::GenParticle>::const_iterator p = __genParticles->product()->begin();
    ParticleCollection::iterator             pp =   genParticles.begin();
    for(; p != __genParticles->product()->end() && pp != genParticles.end(); p++, pp++){
        // find the mother
        const reco::Candidate *m = p->mother();
        if( m ){
            ParticlePointer motherMatch;
            double dR = 1000;
            for( vector<ParticlePointer>::const_iterator ppp = lookup[ m->pdgId() ].begin(); ppp != lookup[ m->pdgId() ].end(); ppp++){
                double dist = (*ppp)->dR(m->eta(), m->phi());
                if( dist < dR ){
                    dist = dR;
                    motherMatch = *ppp;
                }
           }
           (*pp)->setMother( motherMatch.get() );
        }
        // go over the daughters
        for(size_t j=0; j<p->numberOfDaughters(); j++){
            const reco::Candidate *d = p->daughter(j);
            if( !d ) continue; // safety precaution (although, this would be strange)
            ParticlePointer daughterMatch;
            double dR = 1000;
            for( vector<ParticlePointer>::const_iterator ppp = lookup[ d->pdgId() ].begin(); ppp != lookup[ d->pdgId() ].end(); ppp++){
                double dist = (*ppp)->dR(d->eta(), d->phi());
                if( dist < dR ){
                    dist = dR;
                    daughterMatch = *ppp;
                }
            }
            (*pp)->addDaughter( daughterMatch.get() );
        }

    }
*/

    event.put("genParticles",(const ParticleCollection*)&genParticles);

    return AppResult();
}
