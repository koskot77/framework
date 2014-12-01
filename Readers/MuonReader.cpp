using namespace std;
#include "MuonReader.h"
#include "TTree.h"
#include "TBranch.h"

#include "ProductArea/BNcollections/interface/BNmuon.h"
#include "DataFormats/Common/interface/Wrapper.h"

edm::Wrapper<BNmuonCollection> *__bnMuons = new edm::Wrapper<BNmuonCollection>();

AppResult MuonReader::beginJob(AppEvent& event) {
    TTree *Events;
    event.get("Events",Events);

    TBranch *inputMuonsBranch = Events->GetBranch("BNmuons_BNproducer_selectedPatMuonsLoosePFlow_BEANs.");
    if( !inputMuonsBranch )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'BNmuons_BNproducer_selectedPatMuonsLoosePFlow_BEANs.' branch found");
    inputMuonsBranch->SetAddress(&__bnMuons);

    return AppResult();
}

AppResult MuonReader::event(AppEvent& event) {
    muons.clear();
    if( __bnMuons->isPresent() )
      for(BNmuonCollection::const_iterator pmuon = __bnMuons->product()->begin(); pmuon != __bnMuons->product()->end(); pmuon++){
        float energy = pmuon->energy;
        float px = pmuon->px;
        float py = pmuon->py;
        float pz = pmuon->pz;
        MuonPointer muon(new Muon(energy, px, py, pz));
//        muon->setXYZ   ( pmuon->vx, pmuon->vy, pmuon->vz );
        muon->setCharge( pmuon->charge   );

///        muon->setEcalIsolation   ( pmuon->ecalIso  );
///        muon->setHcalIsolation   ( pmuon->hcalIso  );
///        muon->setTrackerIsolation( pmuon->trackIso );
        // new PU corrected isolation components
        muon->setEcalIsolation       ( pmuon->pfIsoR04SumPhotonEt        );
        muon->setHcalIsolation       ( pmuon->pfIsoR04SumNeutralHadronEt );
        muon->setTrackerIsolation    ( pmuon->pfIsoR04SumChargedHadronPt );
        muon->setPFChargedPUinIsoCone( pmuon->pfIsoR04SumPUPt            );
        muon->setPFEventEnergyDensity( pmuon->rhoPrime                   );
        muon->setEffectiveArea       ( pmuon->AEffDr04 );

        muon->makeGlobal ( pmuon->isGlobalMuon  );
        muon->makePFMuon ( pmuon->isPFMuon      );
        muon->makeTracker( pmuon->isTrackerMuon );

        muon->setD0                      ( pmuon->correctedD0Vertex );
        muon->setZDistanceToPrimaryVertex( pmuon->correctedDZ       );

        muon->setPFGammaIsolation        ( pmuon->photonIso        );
        muon->setPFChargedHadronIsolation( pmuon->chargedHadronIso );
        muon->setPFNeutralHadronIsolation( pmuon->neutralHadronIso );

        muon->setNormalizedChi2               ( pmuon->normalizedChi2        );
        muon->setNumberOfValidMuonHits        ( pmuon->numberOfValidMuonHits );
        muon->setNumberOfValidPixelHits       ( pmuon->numberOfValidPixelHits);
        muon->setNtrackerLayersWithMeasurement( pmuon->numberOfLayersWithMeasurement );
        muon->setNumberOfMatchedStations      ( pmuon->numberOfMatchedStations );

        if( muon->isLoose() ) muons.push_back(muon);
      }

    sort   (muons.begin(),muons.end());
    reverse(muons.begin(),muons.end());

    event.put("muons",(const MuonCollection*)&muons);

    return AppResult();
}
