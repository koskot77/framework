#ifndef Jet_h
#define Jet_h
#include "Particle.h"
#include <vector>
#include <string>
///#include <boost/array.hpp>

namespace BtagAlgorithm {
  enum value {
    GenPartonFlavour,
    CombinedSecondaryVertexBTag,
    NUMBER_OF_BTAGALGORITHMS
  };
  enum workingPoint {
    LOOSE, MEDIUM, TIGHT
  };
}

namespace JetAlgorithm {
  enum value {
    Calo_AntiKT_Cone05,
    CA08PF,
    PF2PAT,
    PF2PATBEAN,
    NUMBER_OF_JETALGORITHMS
  };
}

namespace JetCorrDirection {
  enum value { NONE, PLUS, MINUS };
}

namespace JetBtagVariation {
  enum value { NONE, PLUS, MINUS };
}

namespace JetResVariation {
  enum value { NONE, PLUS, MINUS };
}

class Jet: public Particle {
private:
    JetAlgorithm::value usedAlgorithm;
    float  matchedGenJetPt;
    float  electromagneticFraction;
    int    numberOfRecHitsContaining90PercentOfTheJetEnergy;
    float  fractionOfEnergyIntheHottestHPDReadout;
    int    numberOfDaughters;
    float  chargedEmEnergyFraction, neutralHadronEnergyFraction, neutralEmEnergyFraction;
    float  chargedHadronEnergyFraction, chargedMultiplicity;
    double jecUncertainty;
    int    partonFlavr;
    std::vector<double> diffVec;
    std::vector<double> btag_discriminators;

public:
    bool isGood() const;
    bool isBJet(BtagAlgorithm::value      type = BtagAlgorithm::CombinedSecondaryVertexBTag,
                BtagAlgorithm::workingPoint wp = BtagAlgorithm::MEDIUM) const;
    bool isTaggedAsBJet(BtagAlgorithm::value      type = BtagAlgorithm::CombinedSecondaryVertexBTag,
                        BtagAlgorithm::workingPoint wp = BtagAlgorithm::MEDIUM) const;
    JetAlgorithm::value getUsedAlgorithm() const { return usedAlgorithm; }
    float genJetPt() const { return matchedGenJetPt; }
    float emf()      const { return electromagneticFraction; }
    float n90Hits()  const { return numberOfRecHitsContaining90PercentOfTheJetEnergy; }
    float fHPD()const { return fractionOfEnergyIntheHottestHPDReadout; }
    float NOD() const { return numberOfDaughters;           }
    float CEF() const { return chargedEmEnergyFraction;     }
    float NHF() const { return neutralHadronEnergyFraction; }
    float NEF() const { return neutralEmEnergyFraction;     }
    float CHF() const { return chargedHadronEnergyFraction; }
    float NCH() const { return chargedMultiplicity;         }
    int partonFlavor() const { return partonFlavr; }
    double JECUnc() const { return jecUncertainty; }
//    const FourVector &DiffVec;

    void setUsedAlgorithm(JetAlgorithm::value algo)   { usedAlgorithm = algo; }
    void setBtagVariation(JetBtagVariation::value var){ bTagVariation = var;  }
    void setResVariation(JetResVariation::value var)  { resVariation  = var;  }
    void setGenJetPt(float genJetPt)                  { matchedGenJetPt = genJetPt; }
    void setEMF(float emf)                            { electromagneticFraction = emf; }
    void setN90Hits(int n90Hits)                      { numberOfRecHitsContaining90PercentOfTheJetEnergy = n90Hits; }
    void setPartonFlavour(int partonFlvr)             { partonFlavr = partonFlvr; }
    void setFHPD(float fHPD)                          { fractionOfEnergyIntheHottestHPDReadout = fHPD; }
    void setDiscriminatorForBtagType(float discriminator, BtagAlgorithm::value type){ btag_discriminators[type] = discriminator; }
    float getDiscriminatorForBtagType(BtagAlgorithm::value type) const { return btag_discriminators[type]; }

    void setNOD(int   nod){ numberOfDaughters           = nod; }
    void setCEF(float cef){ chargedEmEnergyFraction     = cef; }
    void setNHF(float nhf){ neutralHadronEnergyFraction = nhf; }
    void setNEF(float nef){ neutralEmEnergyFraction     = nef; }
    void setCHF(float chf){ chargedHadronEnergyFraction = chf; }
    void setNCH(float nch){ chargedMultiplicity         = nch; }
    void setJECUnc(double jecUnc){ jecUncertainty = jecUnc; }

    static JetCorrDirection::value correctDirection;
    static JetBtagVariation::value bTagVariation;
    static JetResVariation::value  resVariation;

    void adjForUnc();
    void adjForRes();

    Jet(void);
    Jet(const Particle& particle);
    Jet(float energy, float px, float py, float pz);
    virtual ~Jet(void){}
};

///typedef boost::shared_ptr<Jet> JetPointer;
///typedef std::vector<JetPointer> JetCollection;

#endif
