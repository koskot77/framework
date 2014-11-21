using namespace std;
#include "MuonReader.h"
#include "TTree.h"
#include "TBranch.h"

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

        muon->setCharge          ( pmuon->charge()   );
        muon->setEcalIsolation   ( pmuon->ecalIso()  );
        muon->setHcalIsolation   ( pmuon->hcalIso()  );
        muon->setTrackerIsolation( pmuon->trackIso() );
        muon->setXYZ             ( pmuon->vx(), pmuon->vy(), pmuon->vz() );
/*
        // new PU corrected isolation components
        muon->setEcalIsolation( pmuon->pfIsoR04SumPhotonEt);
        muon->setHcalIsolation( pmuon->pfIsoR04SumNeutralHadronEt);
        muon->setTrackerIsolation( pmuon->pfIsoR04SumChargedHadronPt);
        muon->setPFChargedPUinIsoCone( pmuon->pfIsoR04SumPUPt);
        muon->setPFEventEnergyDensity( pmuon->rhoPrime);
        muon->setEffectiveArea( pmuon->AEffDr04);
        muon->makeGlobal( pmuon->isGlobalMuon);
        muon->makePFMuon( pmuon->isPFMuon);
        muon->makeTracker( pmuon->isTrackerMuon);
*/
//math::XYZPoint vertexPosition;
//vertexPosition.SetCoordinates(0,0,0);
//vertexPosition = vtx->position();

//        muon->setD0( pmuon->innerTrack()->dxy(vertexPosition) );
//        muon->setZDistanceToPrimaryVertex( pmuon->innerTrack()->dz(vertexPosition) );
        muon->setPFGammaIsolation( pmuon->photonIso() );
        muon->setPFChargedHadronIsolation( pmuon->chargedHadronIso() );
        muon->setPFNeutralHadronIsolation( pmuon->neutralHadronIso() );

//        muon->setNormalizedChi2( pmuon->globalTrack()->normalizedChi2() );
//        muon->setNumberOfValidMuonHits( pmuon->globalTrack()->hitPattern().numberOfValidMuonHits() );
//        muon->setNumberOfMatchedStations( pmuon->numberOfMatchedStations() );
//        muon->setNumberOfValidPixelHits( pmuon->innerTrack()->hitPattern().numberOfValidPixelHits() );
//        muon->setNtrackerLayersWithMeasurement( pmuon->track()->hitPattern().trackerLayersWithMeasurement() );

        muons.push_back(muon);
    }

    return AppResult();
}
