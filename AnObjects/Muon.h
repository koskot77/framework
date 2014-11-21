#ifndef Muon_h
#define Muon_h
#include "Lepton.h"
#include <vector>
#include <string>
#include <memory>

class Muon: public Lepton {
private:
    bool  is_Global, is_Tracker, is_PFMuon;
    float ecal_Isolation, hcal_Isolation, tracker_Isolation, effectiveArea;

    // for the tight selection:
    double normalizedChi2;
    int numberOfValidMuonHits,  numberOfMatchedStations;
    int numberOfValidPixelHits, trackerLayersWithMeasurement;

public:
    bool isGood()  const ;
    bool isLoose() const ;
    bool isTight() const ;
    bool isPFIsolated() const { return pfIsolationDeltaBeta() < 0.12; }
    bool isTrackerIsolated() const { return trackerIsolation() < 0.05; }
    bool isTracker() const { return is_Tracker; }
    bool isGlobal()  const { return is_Global; }
    bool isPFMuon()  const { return is_PFMuon; }
    float ecalIsolation() const { return ecal_Isolation; }
    float hcalIsolation() const { return hcal_Isolation; }
    float trackerIsolation()  const { return tracker_Isolation; }
    float relativeIsolation() const {
        return (ecal_Isolation + hcal_Isolation + tracker_Isolation) / pt();
    }
    float pfIsolationDeltaBeta() const {
        return (tracker_Isolation + std::max(float(0.0), ecal_Isolation + hcal_Isolation - float(0.5)*PFsumPUPt) ) / pt();
    }
    float pfIsolationAeff() const {
        return (PFChargedHadron_Isolation + std::max(float(0), PFGamma_Isolation + PFNeutralHadron_Isolation - PFrho * effectiveArea) ) / pt();
    }
    float Aeff() const { return effectiveArea; }

    void makeGlobal(bool global)   { is_Global = global; }
    void makePFMuon(bool pf)       { is_PFMuon = pf; }
    void makeTracker(bool tracker) { is_Tracker = tracker; }
    void setEcalIsolation(float isolation){ ecal_Isolation = isolation; }
    void setHcalIsolation(float isolation){ hcal_Isolation = isolation; }
    void setTrackerIsolation(float isolation){ tracker_Isolation = isolation; }
    void setEffectiveArea(float effArea)     { effectiveArea = effArea; }

    void setNormalizedChi2(double chi2){ normalizedChi2 = chi2; }
    void setNumberOfValidMuonHits(int nHits){ numberOfValidMuonHits = nHits; }
    void setNumberOfMatchedStations(int nStations){ numberOfMatchedStations = nStations; }
    void setNumberOfValidPixelHits(int nPixelHits){ numberOfValidPixelHits = nPixelHits; }
    void setNtrackerLayersWithMeasurement(int trkLayers){ trackerLayersWithMeasurement = trkLayers; }

    Muon(void) : Lepton(),
        is_Global(false), is_Tracker(false), is_PFMuon(false), ecal_Isolation(0.), hcal_Isolation(0), tracker_Isolation(0), effectiveArea(0),
        normalizedChi2(0), numberOfValidMuonHits(0), numberOfMatchedStations(0), numberOfValidPixelHits(0), trackerLayersWithMeasurement(0){}
    Muon(float energy, float px, float py, float pz) : Lepton(energy,px,py,pz),
        is_Global(false), is_Tracker(false), is_PFMuon(false), ecal_Isolation(0.), hcal_Isolation(0), tracker_Isolation(0), effectiveArea(0),
        normalizedChi2(0), numberOfValidMuonHits(0), numberOfMatchedStations(0), numberOfValidPixelHits(0), trackerLayersWithMeasurement(0){}
    virtual ~Muon(void){}
};

typedef std::shared_ptr<Muon>    MuonPointer;
typedef std::vector<MuonPointer> MuonCollection;

#endif
