using namespace std;
#include "Analyser.h"
#include "AnObjects/Jet.h"
#include "AnObjects/Muon.h"
#include "AnObjects/Electron.h"
#include "AnObjects/MET.h"
#include "TLorentzVector.h"

AppResult Analyser::beginJob(AppEvent& event){
//    outputFile = TFile::Open(output.c_str(),"RECREATE");
    microTuple = new TTree("micro","micro");

    microTuple->Branch("numberOfJets", &numberOfJets, "numberOfJets/I");
    microTuple->Branch("jetPtRec",      jetPtRec,     "jetPtRec[4]/D");
    microTuple->Branch("jetEtaRec",     jetEtaRec,    "jetEtaRec[4]/D");
    microTuple->Branch("jetPhiRec",     jetPhiRec,    "jetPhiRec[4]/D");
    microTuple->Branch("jetCSV",        jetCSV,       "jetCSV[4]/D");

    microTuple->Branch("m3jets",       &m3jets,       "m3jets/D");
    microTuple->Branch("met",          &met,          "met/D");

    return AppResult();
}

AppResult Analyser::endJob(AppEvent& event){
    outputFile = TFile::Open(output.c_str(),"RECREATE");
    microTuple->Write();
    outputFile->Close();
//   if( microTuple ) delete microTuple;
//   if( outputFile ) delete outputFile;
    return AppResult();
}

AppResult Analyser::event(AppEvent& event){

    jetPtRec[0] = -1;
    jetPtRec[1] = -1;
    jetPtRec[2] = -1;
    jetPtRec[3] = -1;

    const vector<ParticlePointer> *gen;
    if( event.get("genParticles",gen) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No genParticles found");
    std::cout<<" #genParts = "<<gen->size()<<std::endl;
//    for(unsigned int p=0; p<gen->size(); p++){
//        std::cout<<" gen "<<gen->at(p)->type()<<" pT["<<p<<"] = "<<gen->at(p)->pt()<<" eta="<<gen->at(p)->eta()<<" phi="<<gen->at(p)->phi()<<std::endl;
//    }

    const JetCollection *jets;
    if( event.get("jets",jets) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No jets found");
    std::cout<<" #jets = "<<jets->size()<<std::endl;
    numberOfJets = jets->size();
    for(unsigned int j=0; j<numberOfJets && j<4; j++){
        std::cout<<" pTjet["<<j<<"] = "<<jets->at(j)->pt()<<" eta="<<jets->at(j)->eta()<<" phi="<<jets->at(j)->phi()<<std::endl;
        jetPtRec [j] = jets->at(j)->pt();
        jetEtaRec[j] = jets->at(j)->eta();
        jetPhiRec[j] = jets->at(j)->phi();
        jetCSV   [j] = jets->at(j)->isBJet();
    }
    const ElectronCollection *electrons;
    if( event.get("electrons",electrons) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No electrons found");
    std::cout<<" #electrons = "<<electrons->size()<<std::endl;
//    for(unsigned int e=0; e<electrons->size(); e++)
//        std::cout<<" #pTele["<<e<<"] = "<<electrons->at(e)->pt()<<std::endl;
//
    const MuonCollection *muons;
    if( event.get("muons",muons) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No muons found");
    std::cout<<" #muons = "<<muons->size()<<std::endl;
//    for(unsigned int m=0; m<muons->size(); m++)
//        std::cout<<" #pTmu["<<m<<"] "<<muons->at(m)->pt()<<std::endl;

    const MET *ETmiss;
    if( event.get("ETmiss",ETmiss) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No met found");
    std::cout<<" met= "<<ETmiss->pt()<<std::endl;
    met = ETmiss->pt();

    if( numberOfJets==3 && muons->size()==0 && electrons->size()==0 ){
        TLorentzVector jet1, jet2, jet3;
        jet1.SetPtEtaPhiM(jetPtRec[0], jetEtaRec[0], jetPhiRec[0], 0);
        jet2.SetPtEtaPhiM(jetPtRec[1], jetEtaRec[1], jetPhiRec[1], 0);
        jet3.SetPtEtaPhiM(jetPtRec[2], jetEtaRec[2], jetPhiRec[2], 0);
        TLorentzVector sum3j( jet1 + jet2 + jet3 );
        m3jets = sum3j.M();
    }

    microTuple->Fill();

    event.put("numberOfJets",(const int*)&numberOfJets);
    event.put("m3jets",(const double*)&m3jets);
    event.put("met",   (const double*)&met);

    if( event.get("runNumber",  run) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No runNumber found");
    if( event.get("eventNumber",evt) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No eventNumber found");

    event.put("run",   (const int*)&run);
    event.put("event", (const int*)&evt);

    return AppResult();
}
