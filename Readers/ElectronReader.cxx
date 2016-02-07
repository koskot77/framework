using namespace std;
#include "ElectronReader.h"
#include "TTree.h"
#include "TBranch.h"
#include "Logistics/ElectronEffectiveArea.h"

//#include "DataFormats/PatCandidates/interface/Electron.h"
//edm::Wrapper<pat::ElectronCollection> *__patElectrons = new edm::Wrapper<pat::ElectronCollection>();

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronCore.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronCoreFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
edm::Wrapper<reco::GsfElectronCollection> *__pfElectrons    = new edm::Wrapper<reco::GsfElectronCollection>();

AppResult ElectronReader::beginRun(AppEvent& event) {
    TTree *Events = 0;
    if( event.get("Events",Events) || !Events )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'Events' tree found");
/*
    TBranch *inputElectronsBranch = Events->GetBranch("patElectrons_slimmedElectrons__PAT.");
    if( !inputElectronsBranch )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'patElectrons_slimmedElectrons__PAT.' branch found");
    inputElectronsBranch->SetAddress(&__patElectrons);
*/
    TBranch *inputElectronsBranch = Events->GetBranch("recoGsfElectrons_gsfElectrons__HLT.");
    if( !inputElectronsBranch )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'recoGsfElectrons_gsfElectrons__HLT.' branch found");
    inputElectronsBranch->SetAddress(&__pfElectrons);

    return AppResult();
}

AppResult ElectronReader::event(AppEvent& event) {
    electrons.clear();
//    if( __patElectrons->isPresent() )
//      for(vector<pat::Electron>::const_iterator pele = __patElectrons->product()->begin(); pele != __patElectrons->product()->end(); pele++){
   if( __pfElectrons->isPresent() /*&& __pfTracks->isPresent()*/ )
      for(vector<reco::GsfElectron>::const_iterator pele = __pfElectrons->product()->begin(); pele != __pfElectrons->product()->end(); pele++){
        float energy = pele->energy();
        float px = pele->px();
        float py = pele->py();
        float pz = pele->pz();
        ElectronPointer electron(new Electron(energy, px, py, pz));
        electron->setXYZ   ( pele->vx(), pele->vy(), pele->vz() );
        electron->setCharge( pele->charge()   );

///        electrons->setEcalIsolation   ( pele->ecalIso()  );
///        electrons->setHcalIsolation   ( pele->hcalIso()  );
///        electrons->setTrackerIsolation( pele->trackIso() );
        // new PU corrected isolation components
        electron->setEcalIsolation       ( pele->dr03EcalRecHitSumEt() );
        electron->setHcalIsolation       ( pele->dr03HcalTowerSumEt()  );
        electron->setTrackerIsolation    ( pele->dr03TkSumPt()         );
///        electron->setPFChargedPUinIsoCone( pele->userIso(3)            );
        bool   isRealData = true;
        double AEffDr03 = ElectronEffectiveArea::GetElectronEffectiveArea(
                                ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03,
                                pele->eta(),
                                ( isRealData ? ElectronEffectiveArea::kEleEAData2012 : ElectronEffectiveArea::kEleEAFall11MC )
                          );

        double rho_event = 0;
        if( event.get("rho_event",rho_event) ) return AppResult(AppResult::STOP|AppResult::ERROR,"Can't find rho");
        electron->setPFEventEnergyDensity( std::max(rho_event, 0.0) );
        electron->setEffectiveArea       ( AEffDr03 );

//        electron->setMvaNonTrigV0eID( pele->electronID("eidRobustLoose") ); //mvaNonTrigV0") );
//        electron->setMvaTrigV0eID   ( pele->electronID("eidTight") ); //mvaTrigV0")    );

        double vx=0, vy=0, vz=0;
        if( event.get("vx",vx) ) return AppResult(AppResult::STOP|AppResult::ERROR,"Can't find vx");
        if( event.get("vy",vy) ) return AppResult(AppResult::STOP|AppResult::ERROR,"Can't find vy");
        if( event.get("vz",vz) ) return AppResult(AppResult::STOP|AppResult::ERROR,"Can't find vz");
        math::XYZPoint vertexPosition;
        vertexPosition.SetCoordinates(vx,vy,vz);
        if( pele->core().isNonnull() && pele->gsfTrack().isNonnull() ){
            electron->setD0                      ( pele->gsfTrack()->dxy(vertexPosition) );
            electron->setZDistanceToPrimaryVertex( pele->gsfTrack()->dz (vertexPosition) );
        } else {
            electron->setD0                      ( 0 );
            electron->setZDistanceToPrimaryVertex( 0 );
        }
        electron->setPFGammaIsolation        ( pele->pfIsolationVariables().sumPhotonEt        ); // photonIso()
        electron->setPFChargedHadronIsolation( pele->pfIsolationVariables().sumChargedHadronPt ); // chargedHadronIso()
        electron->setPFNeutralHadronIsolation( pele->pfIsolationVariables().sumNeutralHadronEt ); // neutralHadronIso()
/*
        if( pele->genLepton() ){
            Particle mc;
            mc.setPdgId   ( pele->genLepton()->pdgId() );
            mc.setPtEtaPhi( pele->genLepton()->pt(), pele->genLepton()->eta(), pele->genLepton()->phi() );
            mc.setCharge  ( pele->genLepton()->charge() );
            if( pele->genLepton()->numberOfMothers()>0 ){
                const reco::Candidate *mother = pele->genLepton()->mother();

                bool staytrapped = true;
                while( (mother->pdgId()==mc.pdgId() && staytrapped) ){
                    if( mother->numberOfMothers()>=1 ) mother = mother->mother();
                    else staytrapped = false;
                }

                mc.setMotherPdgId( mother->pdgId() );
//            .setPdgId   (mother->genMotherId);
//            .setPtEtaPhi(mother->pt(),mother->eta(),mother->phi());
//            .setCharge  (mother->charge());
            }
            electron->setGenLepton(mc);
        }
*/
//        if( electron->isLoose() ) electrons.push_back(electron);
    }

    sort   (electrons.begin(),electrons.end());
    reverse(electrons.begin(),electrons.end());

    event.put("electrons",(const ElectronCollection*)&electrons);

    return AppResult();
}
