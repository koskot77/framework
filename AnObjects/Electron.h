#ifndef Electron_h
#define Electron_h
#include "Lepton.h"
#include <vector>
#include <memory>

class Electron: public Lepton {
private:
    float ecal_Isolation, hcal_Isolation, tracker_Isolation;
    float mvaTrigV0, mvaNonTrigV0;
    float superCluser_Eta, effectiveArea;

public:
    float mvaTrigV0eID    (void) const { return mvaTrigV0;      }
    float mvaNonTrigV0eID (void) const { return mvaNonTrigV0;   }
    float ecalIsolation   (void) const { return ecal_Isolation; }
    float hcalIsolation   (void) const { return hcal_Isolation; }
    float trackerIsolation(void) const { return tracker_Isolation; }
    float superClusterEta (void) const { return superCluser_Eta;   }
    float Aeff            (void) const { return effectiveArea;     }
    float pfIsolationAeff (void) const { 
        return (PFChargedHadron_Isolation + std::max(float(0), PFGamma_Isolation + PFNeutralHadron_Isolation - PFrho * effectiveArea) ) / pt();
    }

    void setEffectiveArea(float effArea)  { effectiveArea  = effArea;   }
    void setEcalIsolation(float isolation){ ecal_Isolation = isolation; }
    void setHcalIsolation(float isolation){ hcal_Isolation = isolation; }
    void setTrackerIsolation(float isolation){ tracker_Isolation = isolation; }
    void setSuperClusterEta(float eta)    { superCluser_Eta = eta; }

    void setMvaTrigV0eID   (float mvaID){ mvaTrigV0    = mvaID; }
    void setMvaNonTrigV0eID(float mvaID){ mvaNonTrigV0 = mvaID; }

    bool isInCrack(void) const {
        return fabs(superClusterEta()) > 1.4442 && fabs(superClusterEta()) < 1.5660;
    }

    bool isGood(void)  const {
        bool passesPt  = pt() > 20.0;
        bool passesEta = fabs(eta()) < 2.5 && !isInCrack();
        bool passesD0  = fabs(d0()) < 0.02;
        bool passesDistanceToPV = fabs(zDistanceToPrimaryVertex) < 0.1;
        bool passesID = mvaNonTrigV0eID() > 0;
        return passesPt && passesEta && passesD0 && passesID && passesDistanceToPV;
    }
    bool isLoose(void) const {
        bool passesEt   = pt() > 20; // et()
        bool passesEta  = fabs(eta()) < 2.5;
        bool isolated   = pfIsolationAeff() < 0.2;
        bool notInCrack = isInCrack() == false;
        bool passesID   = mvaNonTrigV0eID() > 0; //
        return passesEt && passesEta && isolated && notInCrack && passesID;
    }

    Electron(void) : Lepton(),
       ecal_Isolation(0), hcal_Isolation(0), tracker_Isolation(0), mvaTrigV0(0), mvaNonTrigV0(0), superCluser_Eta(0), effectiveArea(0){}

    Electron(float energy, float px, float py, float pz) : Lepton(energy,px,py,pz),
       ecal_Isolation(0), hcal_Isolation(0), tracker_Isolation(0), mvaTrigV0(0), mvaNonTrigV0(0), superCluser_Eta(0), effectiveArea(0){}

    virtual ~Electron(void){}
};

typedef std::shared_ptr<Electron>    ElectronPointer;
typedef std::vector<ElectronPointer> ElectronCollection;

#endif
