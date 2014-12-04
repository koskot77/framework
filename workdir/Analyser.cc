using namespace std;
#include "Analyser.h"
#include "AnObjects/Jet.h"
#include "AnObjects/Muon.h"
#include "AnObjects/Electron.h"
#include "AnObjects/MET.h"
#include "TLorentzVector.h"

AppResult Analyser::beginJob(AppEvent& event){
    return AppResult();
}

AppResult Analyser::endJob(AppEvent& event){
    return AppResult();
}

AppResult Analyser::event(AppEvent& event){

    jetPtRec[0] = -1;
    jetPtRec[1] = -1;
    jetPtRec[2] = -1;
    jetPtRec[3] = -1;

    if( event.get("runNumber",  run) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No runNumber found");
    if( event.get("eventNumber",evt) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No eventNumber found");

    event.put("run",   (const int*)&run);
    event.put("event", (const int*)&evt);

    cout<<"Event #"<<evt<<endl;

    const ParticleCollection *gen;
    if( event.get("genParticles",gen) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No genParticles found");
    cout<<" #genParts = "<<gen->size()<<std::endl;
    for(unsigned int p=0; p<gen->size() /*&& p<20*/; p++){
        const Particle *gp = gen->at(p).get();
        const Particle *m = gp->mother();
        const list<const Particle*> &d = gp->daughters();
        cout<<" gen "<<gp->pdgId()<<" pT["<<p<<"] = "<<gp->pt()<<" eta="<<gp->eta()<<" phi="<<gp->phi()<<" status="<<gp->status()<<" mId="<<(m?m->pdgId():-1)<<" nd="<<d.size()<<std::endl;
    }

    const JetCollection *jets;
    if( event.get("jets",jets) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No jets found");
    cout<<" #jets = "<<jets->size()<<std::endl;
    numberOfJets = jets->size();
    for(int j=0; j<numberOfJets && j<4; j++){
        cout<<" pTjet["<<j<<"] = "<<jets->at(j)->pt()<<" eta="<<jets->at(j)->eta()<<" phi="<<jets->at(j)->phi()<<std::endl;
        jetPtRec [j] = jets->at(j)->pt();
        jetEtaRec[j] = jets->at(j)->eta();
        jetPhiRec[j] = jets->at(j)->phi();
        jetCSV   [j] = jets->at(j)->isBJet();
    }
    const ElectronCollection *electrons;
    if( event.get("electrons",electrons) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No electrons found");
    cout<<" #electrons = "<<electrons->size()<<std::endl;
//    for(unsigned int e=0; e<electrons->size(); e++)
//        std::cout<<" #pTele["<<e<<"] = "<<electrons->at(e)->pt()<<std::endl;
//
    const MuonCollection *muons;
    if( event.get("muons",muons) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No muons found");
    cout<<" #muons = "<<muons->size()<<std::endl;
//    for(unsigned int m=0; m<muons->size(); m++)
//        std::cout<<" #pTmu["<<m<<"] "<<muons->at(m)->pt()<<std::endl;

    const MET *ETmiss;
    if( event.get("ETmiss",ETmiss) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No met found");
    cout<<" met= "<<ETmiss->pt()<<std::endl;
    met = ETmiss->pt();

    if( numberOfJets==3 && muons->size()==0 && electrons->size()==0 ){
        TLorentzVector jet1, jet2, jet3;
        jet1.SetPtEtaPhiM(jetPtRec[0], jetEtaRec[0], jetPhiRec[0], 0);
        jet2.SetPtEtaPhiM(jetPtRec[1], jetEtaRec[1], jetPhiRec[1], 0);
        jet3.SetPtEtaPhiM(jetPtRec[2], jetEtaRec[2], jetPhiRec[2], 0);
        TLorentzVector sum3j( jet1 + jet2 + jet3 );
        m3jets = sum3j.M();
    }

    event.put("numberOfJets",(const int*)&numberOfJets);
    event.put("m3jets",      (const double*)&m3jets);
    event.put("met",         (const double*)&met);
    event.put("jetPtRec[4]", (const double*)jetPtRec);

    return AppResult();
}
