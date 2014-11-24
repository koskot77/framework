using namespace std;
#include "MuonReader.h"
#include "TTree.h"
#include "TBranch.h"
#include "MuonEffectiveArea.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
edm::Wrapper<pat::MuonCollection> *__patMuons = new edm::Wrapper<pat::MuonCollection>();

AppResult MuonReader::beginJob(AppEvent& event) {
    TTree *Events;
    event.get("Events",Events);

    TBranch *inputMuonsBranch = Events->GetBranch("patMuons_slimmedMuons__PAT.");
    if( !inputMuonsBranch )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'patMuons_slimmedMuons__PAT.' branch found");
    inputMuonsBranch->SetAddress(&__patMuons);

    return AppResult();
}

AppResult MuonReader::event(AppEvent& event) {
    muons.clear();
    if( __patMuons->isPresent() )
      for(vector<pat::Muon>::const_iterator pmuon = __patMuons->product()->begin(); pmuon != __patMuons->product()->end(); pmuon++){
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
        muon->setD0                      ( (pmuon->isTrackerMuon() ? pmuon->innerTrack()->dxy(vertexPosition) : 99999));
        muon->setZDistanceToPrimaryVertex( (pmuon->isTrackerMuon() ? pmuon->innerTrack()->dz (vertexPosition) : 99999));

        muon->setPFGammaIsolation        ( pmuon->photonIso()        );
        muon->setPFChargedHadronIsolation( pmuon->chargedHadronIso() );
        muon->setPFNeutralHadronIsolation( pmuon->neutralHadronIso() );

        muon->setNormalizedChi2               ( (pmuon->isGlobalMuon()  ? pmuon->globalTrack()->normalizedChi2() : -1)                     );
        muon->setNumberOfValidMuonHits        ( (pmuon->isGlobalMuon()  ? pmuon->globalTrack()->hitPattern().numberOfValidMuonHits() : -1) );
        muon->setNumberOfValidPixelHits       ( (pmuon->isTrackerMuon() ? pmuon->innerTrack()->hitPattern().numberOfValidPixelHits() : -1) );
        muon->setNtrackerLayersWithMeasurement( (pmuon->isTrackerMuon() ? pmuon->track()->hitPattern().trackerLayersWithMeasurement(): -1) );
        muon->setNumberOfMatchedStations      ( pmuon->numberOfMatchedStations() );

        if( muon->isLoose() ) muons.push_back(muon);
      }

    event.put("muons",(const MuonCollection*)&muons);

    return AppResult();
}
