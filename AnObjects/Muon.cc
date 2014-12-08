#include "Muon.h"
bool operator<(MuonPointer i, MuonPointer j) { return ((bool)i && (bool)j ? (*i)<(*j) : false); }

using namespace std;

bool Muon::isGood() const{
    // bool passesPt = pt() > 35;
    // bool passesPt = pt() > 42; // For use wth HLT_Mu40...
    bool passesPt = pt() > 20; // For use wth HLT_Mu40...
    bool passesEta = fabs(eta()) < 2.1;
    bool passesD0 = fabs(d0()) < 0.02; //cm
    bool passesDistanceToPV = fabs(zDistanceToPrimaryVertex) < 1.0;
    return passesPt && passesEta && passesD0 && passesDistanceToPV && is_Global;
}

bool Muon::isTight() const{
// https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId#Basline_muon_selections_for_2012
    bool passesChi2 = normalizedChi2 < 10;
    bool passesD0 = fabs(d0()) < 0.2;
    bool passesDZ = fabs(ZDistanceToPrimaryVertex()) < 0.5;
    bool passesMuonHits = numberOfValidMuonHits > 0;
    bool passesMatchedStations = numberOfMatchedStations > 1;
    bool passesPixelHits = numberOfValidPixelHits > 0;
    bool passesLayers = trackerLayersWithMeasurement > 5;
    return is_Global && is_PFMuon && passesChi2 && passesD0 && passesDZ &&
           passesMuonHits && passesMatchedStations && passesPixelHits && passesLayers;
}
bool Muon::isLoose() const {
    bool passesPt = pt() > 10;
    bool passesEta = fabs(eta()) < 2.5;
    bool looseisolated = pfIsolationDeltaBeta() < 0.2;
    // bool looseisolated = trackerIsolation() < 0.1;
    return passesPt && passesEta && looseisolated && (is_Global || is_Tracker);
}
