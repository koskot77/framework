#ifndef Lepton_h
#define Lepton_h
#include "Particle.h"
#include <vector>
#include <string>
#include <memory>

class Lepton: public Particle {
private:

protected:
    float xyDistanceToPrimaryVertex, zDistanceToPrimaryVertex;
    float PFGamma_Isolation, PFChargedHadron_Isolation, PFNeutralHadron_Isolation;
    float PFrho, PFsumPUPt;  

public:
    float XyDistanceToPrimaryVertex(void) const { return xyDistanceToPrimaryVertex; }
    float ZDistanceToPrimaryVertex (void) const { return zDistanceToPrimaryVertex;  }
    float PFGammaIsolation         (void) const { return PFGamma_Isolation; }
    float PFChargedHadronIsolation (void) const { return PFChargedHadron_Isolation; }
    float PFNeutralHadronIsolation (void) const { return PFNeutralHadron_Isolation; }
    float PFChargedPUinIsoCone     (void) const { return PFsumPUPt; }
    float PFEventEnergyDensity     (void) const { return PFrho; }

    void setXyDistanceToPrimaryVertex(float dist){ xyDistanceToPrimaryVertex = dist; }
    void setZDistanceToPrimaryVertex (float dist){  zDistanceToPrimaryVertex = dist; }
    void setPFGammaIsolation         (float pfGammaIso){ PFGamma_Isolation = pfGammaIso; }
    void setPFChargedHadronIsolation (float chargedHadronIso){ PFChargedHadron_Isolation = chargedHadronIso; }
    void setPFNeutralHadronIsolation (float neutralHadronIso){ PFNeutralHadron_Isolation = neutralHadronIso; }
    void setPFChargedPUinIsoCone     (float sumPUPt){ PFsumPUPt = sumPUPt; }
    void setPFEventEnergyDensity     (float rho){ PFrho = rho; }

    Lepton(void) : Particle(),
        xyDistanceToPrimaryVertex(0), zDistanceToPrimaryVertex(0),
        PFGamma_Isolation(0), PFChargedHadron_Isolation(0), PFNeutralHadron_Isolation(0){}

    Lepton(float energy, float px, float py, float pz) : Particle(energy,px,py,pz),
        xyDistanceToPrimaryVertex(0), zDistanceToPrimaryVertex(0),
        PFGamma_Isolation(0), PFChargedHadron_Isolation(0), PFNeutralHadron_Isolation(0),
        PFrho(0), PFsumPUPt(0){}
    virtual ~Lepton(void){}
};

typedef std::shared_ptr<Lepton>    LeptonPointer;
typedef std::vector<LeptonPointer> LeptonCollection;

#endif /* Lepton_h */
