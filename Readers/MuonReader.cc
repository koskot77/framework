using namespace std;
#include "MuonReader.h"
#include "TTree.h"
#include "TBranch.h"
#include "MuonEffectiveArea.h"

AppResult MuonReader::beginJob(AppEvent& event) {
    TTree *Events;
    event.get("Events",Events);
    bpm = Events->GetBranch("patMuons_slimmedMuons__PAT.obj");
    bpm->SetAddress(&patMuons);
    return AppResult();
}

AppResult MuonReader::event(AppEvent& event) {
    muons.clear();
    for(vector<pat::Muon>::const_iterator pmuon = patMuons.begin(); pmuon != patMuons.end(); pmuon++){
        float energy = pmuon->energy();
        float px = pmuon->px();
        float py = pmuon->py();
        float pz = pmuon->pz();
        MuonPointer muon(new Muon(energy, px, py, pz));
        muon->setXYZ   ( pmuon->vx(), pmuon->vy(), pmuon->vz() );
        muon->setCharge( pmuon->charge()   );

///        muon->setEcalIsolation   ( pmuon->ecalIso()  );
///        muon->setHcalIsolation   ( pmuon->hcalIso()  );
///        muon->setTrackerIsolation( pmuon->trackIso() );
        // new PU corrected isolation components
        muon->setEcalIsolation       ( pmuon->pfIsolationR04().sumPhotonEt        );
        muon->setHcalIsolation       ( pmuon->pfIsolationR04().sumNeutralHadronEt );
        muon->setTrackerIsolation    ( pmuon->pfIsolationR04().sumChargedHadronPt );
        muon->setPFChargedPUinIsoCone( pmuon->pfIsolationR04().sumPUPt            );
        bool   isRealData = true;
        double AEffDr04 = MuonEffectiveArea::GetMuonEffectiveArea(
                                MuonEffectiveArea::kMuGammaAndNeutralHadronIso04,
                                pmuon->eta(),
                                ( isRealData ? MuonEffectiveArea::kMuEAData2012 : MuonEffectiveArea::kMuEAFall11MC )
                          );
        double rho_event = 0;
        if( event.get("rho_event",rho_event) ) return AppResult(AppResult::STOP|AppResult::ERROR,"Can't find rho");
        muon->setPFEventEnergyDensity( std::max(rho_event, 0.0) );
        muon->setEffectiveArea       ( AEffDr04 );

        muon->makeGlobal ( pmuon->isGlobalMuon()  );
        muon->makePFMuon ( pmuon->isPFMuon()      );
        muon->makeTracker( pmuon->isTrackerMuon() );

        double vx=0, vy=0, vz=0;
        if( event.get("vx",rho_event) ) return AppResult(AppResult::STOP|AppResult::ERROR,"Can't find vx");
        if( event.get("vy",rho_event) ) return AppResult(AppResult::STOP|AppResult::ERROR,"Can't find vx");
        if( event.get("vz",rho_event) ) return AppResult(AppResult::STOP|AppResult::ERROR,"Can't find vx");
        math::XYZPoint vertexPosition;
        vertexPosition.SetCoordinates(vx,vy,vz);
//        muon->setD0                      ( pmuon->innerTrack()->dxy(vertexPosition) );
//        muon->setZDistanceToPrimaryVertex( pmuon->innerTrack()->dz (vertexPosition) );

        muon->setPFGammaIsolation        ( pmuon->photonIso()        );
        muon->setPFChargedHadronIsolation( pmuon->chargedHadronIso() );
        muon->setPFNeutralHadronIsolation( pmuon->neutralHadronIso() );
/*
        muon->setNormalizedChi2               ( pmuon->globalTrack()->normalizedChi2()                     );
        muon->setNumberOfValidMuonHits        ( pmuon->globalTrack()->hitPattern().numberOfValidMuonHits() );
        muon->setNumberOfMatchedStations      ( pmuon->numberOfMatchedStations()                           );
        muon->setNumberOfValidPixelHits       ( pmuon->innerTrack()->hitPattern().numberOfValidPixelHits() );
        muon->setNtrackerLayersWithMeasurement( pmuon->track()->hitPattern().trackerLayersWithMeasurement());
*/
        muons.push_back(muon);
    }

    return AppResult();
}
