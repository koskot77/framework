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

//match(const double *source, double *destination){}

AppResult Analyser::event(AppEvent& event){
    ZpT = 0;
    diMuPtRec = 0;
    diMuPtGen = 0;

    numberOfJets = 0;
    jetPtRec[0] = -1;
    jetPtRec[1] = -1;
    jetPtRec[2] = -1;
    jetPtRec[3] = -1;

    numberOfRecMuons = 0;
    muPtRec[0] = -1;
    muPtRec[1] = -1;
    muPtRec[2] = -1;
    muPtRec[3] = -1;

    numberOfGenMuons = 0;
    muPtGen[0] = -1;
    muPtGen[1] = -1;
    muPtGen[2] = -1;
    muPtGen[3] = -1;

    muPfIso[0] = -1;
    muPfIso[1] = -1;
    muPfIso[2] = -1;
    muPfIso[3] = -1;

    const char *path = 0;

    if( event.get("runNumber",       run) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No runNumber found");
    if( event.get("eventNumber",     evt) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No eventNumber found");
    if( event.get("chainEntryNumber",etr) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No chainEntryNumber found");
    if( event.get("inputFileName",  path) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No inputFileName found");

    event.put("run",   (const int*)&run);
    event.put("event", (const int*)&evt);
    event.put("entry", (const int*)&etr);

    cout<<"Event #"<<evt<<endl;

    const ParticleCollection *gen;
    if( event.get("genParticles",gen) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No genParticles found");
    cout<<" #genParts = "<<gen->size()<<std::endl;
    bool diMuonZdecay = false;
    for(unsigned int p=0; p<gen->size() /*&& p<20*/; p++){
        const Particle *gp = gen->at(p).get();
        const Particle *m  = gp->mother();
        const list<const Particle*> &d = gp->daughters();
        cout<<" gen "<<gp->pdgId()<<" pT["<<p<<"] = "<<gp->pt()<<" eta="<<gp->eta()<<" phi="<<gp->phi()<<" status="<<gp->status()<<" mId="<<(m?m->pdgId():-1)<<" nd="<<d.size()<<std::endl;
        if( abs(gp->pdgId())==13 && abs(gp->motherPdgId())==23 ) diMuonZdecay = true;
        if( abs(gp->pdgId())==13 && gp->status() == 1 && diMuonZdecay && numberOfGenMuons<4 ){
            muPtGen [ numberOfGenMuons ] = gp->pt();
            muEtaGen[ numberOfGenMuons ] = gp->eta();
            muPhiGen[ numberOfGenMuons ] = gp->phi();
            numberOfGenMuons++;
        }
        if( gp->pdgId() == 23 ) ZpT = gp->pt();
    }
    if( numberOfGenMuons >= 2 ){
      TLorentzVector lepton1, lepton2;
      lepton1.SetPtEtaPhiM(muPtGen[0], muEtaGen[0], muPhiGen[0], 0.113);
      lepton2.SetPtEtaPhiM(muPtGen[1], muEtaGen[1], muPhiGen[1], 0.113);
      TLorentzVector sum( lepton1 + lepton2 );
      diMuPtGen = sum.Pt();
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
    for(unsigned int e=0; e<electrons->size(); e++){
        cout<<" #pTele["<<e<<"] = "<<electrons->at(e)->pt()<<" gen="<<electrons->at(e)->genLepton().pt()<<std::endl;
    }

    const MuonCollection *muons;
    if( event.get("muons",muons) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No muons found");
    cout<<" #muons = "<<muons->size()<<std::endl;
    for(unsigned int m=0; m<muons->size(); m++){
        cout<<" #pTmu["<<m<<"] "<<muons->at(m)->pt()<<" gen="<<muons->at(m)->genLepton().pt()<<std::endl;
        muPtRec [ numberOfRecMuons ] = muons->at(m)->pt();
        muEtaRec[ numberOfRecMuons ] = muons->at(m)->eta();
        muPhiRec[ numberOfRecMuons ] = muons->at(m)->phi();
        muPfIso [ numberOfRecMuons ] = muons->at(m)->pfIsolationDeltaBeta();
        numberOfRecMuons++;
    }

    if( numberOfRecMuons == 2 ){
      TLorentzVector lepton1, lepton2;
      lepton1.SetPtEtaPhiM(muPtRec[0], muEtaRec[0], muPhiRec[0], 0.113);
      lepton2.SetPtEtaPhiM(muPtRec[1], muEtaRec[1], muPhiRec[1], 0.113);
      TLorentzVector sum( lepton1 + lepton2 );
      diMuPtRec = sum.Pt();
    }

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

    event.put("numberOfRecMuons", (const int*)&numberOfRecMuons);
    event.put("muPtRec[4]",  (const double*)muPtRec);
    event.put("muEtaRec[4]", (const double*)muEtaRec);
    event.put("muPhiRec[4]", (const double*)muPhiRec);
    event.put("muPfIso[4]",  (const double*)muPfIso);

    event.put("numberOfGenMuons", (const int*)&numberOfGenMuons);
    event.put("muPtGen[4]",  (const double*)muPtGen);
    event.put("muEtaGen[4]", (const double*)muEtaGen);
    event.put("muPhiGen[4]", (const double*)muPhiGen);

    event.put("diMuPtRec", (const double*)&diMuPtRec);
    event.put("diMuPtGen", (const double*)&diMuPtGen);
    event.put("ZpT",       (const double*)&ZpT);

    return AppResult();
}
