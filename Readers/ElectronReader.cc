using namespace std;
#include "ElectronReader.h"
#include "TTree.h"
#include "TBranch.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
edm::Wrapper<pat::ElectronCollection> *__patElectrons = new edm::Wrapper<pat::ElectronCollection>();

AppResult ElectronReader::beginJob(AppEvent& event) {
    TTree *Events;
    event.get("Events",Events);

    TBranch *inputElectronsBranch = Events->GetBranch("patElectrons_slimmedElectrons__PAT.");
    if( !inputElectronBranch )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'patElectrons_slimmedElectrons__PAT.' branch found");
    inputJetsBranch->SetAddress(&__patElectrons);

    return AppResult();
}

AppResult ElectronReader::event(AppEvent& event) {
    electrons.clear();
    for(vector<pat::Electron>::const_iterator pele = __patElectrons->product()->begin(); pele != __patElectrons->product()->end(); pele++){
        float energy = pele->energy();
        float px = pele->px();
        float py = pele->py();
        float pz = pele->pz();
        ElectronPointer electron(new Electron(energy, px, py, pz));
        electrons->setXYZ   ( pele->vx(), pele->vy(), pele->vz() );
        electrons->setCharge( pele->charge()   );

///        electrons->setEcalIsolation   ( pele->ecalIso()  );
///        electrons->setHcalIsolation   ( pele->hcalIso()  );
///        electrons->setTrackerIsolation( pele->trackIso() );
        // new PU corrected isolation components
        electrons->setEcalIsolation       ( pele->dr03EcalRecHitSumEt() );
        electrons->setHcalIsolation       ( pele->dr03HcalTowerSumEt()  );
        electrons->setTrackerIsolation    ( pele->dr03TkSumPt()         );
        electrons->setPFChargedPUinIsoCone( pele->userIso(3)            );
        bool   isRealData = true;
        double AEffDr03 = ElectronEffectiveArea::GetElectronEffectiveArea(
                                ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03,
                                pele->eta(),
                                ( isRealData ? ElectronEffectiveArea::kEleEAData2012 : ElectronEffectiveArea::kEleEAFall11MC )
                          );

        double rho_event = 0;
        if( event.get("rho_event",rho_event) ) return AppResult(AppResult::STOP|AppResult::ERROR,"Can't find rho");
        electrons->setPFEventEnergyDensity( std::max(rho_event, 0.0) );
        electrons->setEffectiveArea       ( AEffDr03 );

        electrons->setMvaNonTrigV0eID( pele->electronID("mvaNonTrigV0") );
        electrons->setMvaTrigV0eID   ( pele->electronID("mvaTrigV0")    );

        double vx=0, vy=0, vz=0;
        if( event.get("vx",rho_event) ) return AppResult(AppResult::STOP|AppResult::ERROR,"Can't find vx");
        if( event.get("vy",rho_event) ) return AppResult(AppResult::STOP|AppResult::ERROR,"Can't find vx");
        if( event.get("vz",rho_event) ) return AppResult(AppResult::STOP|AppResult::ERROR,"Can't find vx");
        math::XYZPoint vertexPosition;
        vertexPosition.SetCoordinates(vx,vy,vz);
        electrons->setD0                      ( pele->gsfTrack()->dxy(vertexPosition) );
        electrons->setZDistanceToPrimaryVertex( pele->gsfTrack()->dz (vertexPosition) );

        electrons->setPFGammaIsolation        ( pele->photonIso()        );
        electrons->setPFChargedHadronIsolation( pele->chargedHadronIso() );
        electrons->setPFNeutralHadronIsolation( pele->neutralHadronIso() );

        electronss.push_back(electrons);
    }

    return AppResult();
}
