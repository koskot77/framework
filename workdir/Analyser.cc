using namespace std;
#include "Analyser.h"
#include "AnObjects/Jet.h"
#include "AnObjects/Muon.h"
#include "AnObjects/Electron.h"
#include "AnObjects/MET.h"
#include "TLorentzVector.h"

AppResult Analyser::beginJob(AppEvent& event){
ev.insert(6659626);
ev.insert(42557868);
ev.insert(51644177);
ev.insert(39415088);
ev.insert(55274477);
ev.insert(30360438);
ev.insert(25761164);
ev.insert(1999544);
ev.insert(54624139);
ev.insert(63831890);
ev.insert(23433918);
ev.insert(33312782);
ev.insert(539736);
ev.insert(55674530);
ev.insert(16759774);
ev.insert(62731499);
ev.insert(66246117);
ev.insert(29277497);
ev.insert(14654639);
ev.insert(54743754);
ev.insert(4643894);
ev.insert(63676046);
ev.insert(21193798);
ev.insert(63998559);
ev.insert(26942503);
ev.insert(17543481);
ev.insert(62335886);
ev.insert(26701365);
ev.insert(29122812);
ev.insert(55210098);
ev.insert(35929139);
ev.insert(36359310);
ev.insert(20020671);
ev.insert(58678255);
ev.insert(25420298);
ev.insert(19414066);
ev.insert(39208778);
ev.insert(18037245);
ev.insert(8591515);
ev.insert(281905);
ev.insert(51172300);
ev.insert(54570628);
ev.insert(32327426);
ev.insert(17288354);
ev.insert(7905983);
ev.insert(14934577);
ev.insert(10776611);
ev.insert(46536415);
ev.insert(40647194);
ev.insert(43525469);
ev.insert(25257627);
ev.insert(16651116);
ev.insert(69057340);
ev.insert(41447604);
ev.insert(53763023);
ev.insert(64394239);
ev.insert(59443177);
ev.insert(68405079);
ev.insert(5361447);
ev.insert(37655322);
ev.insert(23034412);
ev.insert(59338500);
ev.insert(64276278);
ev.insert(15879981);
ev.insert(43436905);
ev.insert(35222743);
ev.insert(20274384);
ev.insert(41202079);
ev.insert(65410630);
ev.insert(45657553);
ev.insert(27953423);
ev.insert(35647268);
ev.insert(70623281);
ev.insert(45737212);
ev.insert(11799828);
ev.insert(53727156);
ev.insert(13572143);
ev.insert(4792030);
ev.insert(29154406);
ev.insert(3835583);
ev.insert(637356);
ev.insert(37036952);
ev.insert(21370631);
ev.insert(12257342);
ev.insert(50227022);
ev.insert(17266258);
ev.insert(50421819);
ev.insert(42251356);
ev.insert(3026640);
ev.insert(6865120);
ev.insert(20506954);
ev.insert(63188625);
ev.insert(31415450);
ev.insert(45413855);
ev.insert(57349112);
ev.insert(56266467);
ev.insert(31650827);
ev.insert(57002251);
ev.insert(47450842);
ev.insert(29731635);
ev.insert(19507580);
ev.insert(42135500);
ev.insert(39161101);
ev.insert(13242553);
ev.insert(31808454);
ev.insert(1274889);
ev.insert(39162460);
ev.insert(18891272);
ev.insert(8302262);
ev.insert(26550928);
ev.insert(26568094);
ev.insert(56176173);
ev.insert(68615099);
ev.insert(888986);
ev.insert(4541333);
ev.insert(44707649);
ev.insert(5506079);
ev.insert(8652070);
ev.insert(44720281);
ev.insert(60705186);
ev.insert(5963662);
ev.insert(57469738);
ev.insert(23996342);
ev.insert(39035641);
ev.insert(30418088);
ev.insert(18993181);
ev.insert(61321656);
ev.insert(46914852);
ev.insert(20364125);
ev.insert(4322860);
ev.insert(60311928);
ev.insert(61521317);
ev.insert(45152330);
ev.insert(53812347);
ev.insert(14202096);
ev.insert(19451559);
ev.insert(38365861);
ev.insert(52786598);
ev.insert(28848383);
ev.insert(4292912);
ev.insert(23293763);
ev.insert(22311372);
ev.insert(24587484);
ev.insert(25501249);
ev.insert(13583740);
ev.insert(69805110);
ev.insert(29571439);
ev.insert(57455391);
ev.insert(48689721);
ev.insert(21101530);
ev.insert(26172679);
ev.insert(11202376);
ev.insert(70556593);
ev.insert(3296962);
ev.insert(2409757);
ev.insert(2842654);
ev.insert(372189);
ev.insert(4391706);
ev.insert(4458692);
ev.insert(42012234);
ev.insert(5668078);
ev.insert(42116206);
ev.insert(2480929);
ev.insert(1821946);
ev.insert(14479840);
ev.insert(54798564);
ev.insert(32384221);
ev.insert(44094931);
ev.insert(61178174);
ev.insert(42050631);
ev.insert(67551714);
ev.insert(7124372);
ev.insert(325960);
ev.insert(7254700);
ev.insert(41594240);
ev.insert(13732187);
ev.insert(59074417);
ev.insert(62318294);
ev.insert(38082813);
ev.insert(833368);
ev.insert(55278996);
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

    const char *path = 0;

    if( event.get("runNumber",       run) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No runNumber found");
    if( event.get("eventNumber",     evt) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No eventNumber found");
    if( event.get("chainEntryNumber",etr) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No chainEntryNumber found");
    if( event.get("inputFileName",  path) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No inputFileName found");

if( ev.find(evt) != ev.end() ) std::cout<<"Found event "<<evt<<" in "<<path<<endl;

    event.put("run",   (const int*)&run);
    event.put("event", (const int*)&evt);
    event.put("entry", (const int*)&etr);

    cout<<"Event #"<<evt<<endl;

    const ParticleCollection *gen;
    if( event.get("genParticles",gen) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No genParticles found");
    cout<<" #genParts = "<<gen->size()<<std::endl;
    for(unsigned int p=0; p<gen->size() /*&& p<20*/; p++){
        const Particle *gp = gen->at(p).get();
        const Particle *m  = gp->mother();
        const list<const Particle*> &d = gp->daughters();
        cout<<" gen "<<gp->pdgId()<<" pT["<<p<<"] = "<<gp->pt()<<" eta="<<gp->eta()<<" phi="<<gp->phi()<<" status="<<gp->status()<<" mId="<<(m?m->pdgId():-1)<<" nd="<<d.size()<<std::endl;
        if( abs(gp->pdgId())==11 && gp->status() == 1 && numberOfGenMuons<4 ){
            muPtGen [ numberOfGenMuons ] = gp->pt();
            muEtaGen[ numberOfGenMuons ] = gp->eta();
            muPhiGen[ numberOfGenMuons ] = gp->phi();
            numberOfGenMuons++;
        }
        if( gp->pdgId() == 23 ) ZpT = gp->pt();
    }
    if( numberOfGenMuons == 2 ){
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

//    const MuonCollection *muons;
//    if( event.get("muons",muons) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No muons found");
    const ElectronCollection *muons;
    if( event.get("electrons",muons) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No electrons found");
    cout<<" #muons = "<<muons->size()<<std::endl;
    for(unsigned int m=0; m<muons->size(); m++){
        cout<<" #pTmu["<<m<<"] "<<muons->at(m)->pt()<<" gen="<<muons->at(m)->genLepton().pt()<<std::endl;
        muPtRec [ numberOfRecMuons ] = muons->at(m)->pt();
        muEtaRec[ numberOfRecMuons ] = muons->at(m)->eta();
        muPhiRec[ numberOfRecMuons ] = muons->at(m)->phi();
///        muPfIso [ numberOfRecMuons ] = muons->at(m)->pfIsolationDeltaBeta();
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
