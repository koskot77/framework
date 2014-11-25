#include "Jet.h"
#include "BTagSFUtil.h"

#include <iostream>
using namespace std;

JetCorrDirection::value Jet::correctDirection = JetCorrDirection::NONE;
JetBtagVariation::value Jet::bTagVariation    = JetBtagVariation::NONE;
JetResVariation::value  Jet::resVariation     = JetResVariation::NONE;
	
Jet::Jet(void) : Particle(),
//  DiffVec(diffVec),
  usedAlgorithm(JetAlgorithm::Calo_AntiKT_Cone05),
  matchedGenJetPt(0),
  electromagneticFraction(0.),
  numberOfRecHitsContaining90PercentOfTheJetEnergy(0.),
  fractionOfEnergyIntheHottestHPDReadout(0.),
  numberOfDaughters(0),
  chargedEmEnergyFraction(1),
  neutralHadronEnergyFraction(1),
  neutralEmEnergyFraction(1),
  chargedHadronEnergyFraction(1),
  chargedMultiplicity(0),
  jecUncertainty(0),
  partonFlavr(0),
  diffVec(4),
  btag_discriminators(BtagAlgorithm::NUMBER_OF_BTAGALGORITHMS){
  for(unsigned int btag = 0; btag < btag_discriminators.size(); btag++)
     btag_discriminators[btag] = -9999;
}

Jet::Jet(float energy, float px, float py, float pz) : Particle(energy, px, py, pz),
//  DiffVec(diffVec),
  usedAlgorithm(JetAlgorithm::Calo_AntiKT_Cone05),
  matchedGenJetPt(0),
  electromagneticFraction(0.),
  numberOfRecHitsContaining90PercentOfTheJetEnergy(0.),
  fractionOfEnergyIntheHottestHPDReadout(0.),
  numberOfDaughters(0),
  chargedEmEnergyFraction(1),
  neutralHadronEnergyFraction(1),
  neutralEmEnergyFraction(1),
  chargedHadronEnergyFraction(1),
  chargedMultiplicity(0),
  jecUncertainty(0),
  partonFlavr(0),
  diffVec(4),
  btag_discriminators(BtagAlgorithm::NUMBER_OF_BTAGALGORITHMS){
  for(unsigned int btag = 0; btag < btag_discriminators.size(); btag++)
     btag_discriminators[btag] = -9999;
}

void Jet::adjForRes(){
    double factor = 1.;
    
    double scale_JER = 1., scale_JERup = 1., scale_JERdown = 1.;

    double diff_FullSim_FastSim = 0.;

    double jetAbsETA = eta();

    if( jetAbsETA<0.5 ){ 
        scale_JER = 1.052; scale_JERup = 1.052 + sqrt( 0.012*0.012 + 0.062*0.062 ); scale_JERdown = 1.052 - sqrt( 0.012*0.012 + 0.061*0.061 );
        diff_FullSim_FastSim = 1.35;
    } else if( jetAbsETA<1.1 ){ 
        scale_JER = 1.057; scale_JERup = 1.057 + sqrt( 0.012*0.012 + 0.056*0.056 ); scale_JERdown = 1.057 - sqrt( 0.012*0.012 + 0.055*0.055 );
        diff_FullSim_FastSim = 1.54;
    } else if( jetAbsETA<1.7 ){ 
        scale_JER = 1.096; scale_JERup = 1.096 + sqrt( 0.017*0.017 + 0.063*0.063 ); scale_JERdown = 1.096 - sqrt( 0.017*0.017 + 0.062*0.062 );
        diff_FullSim_FastSim = 1.97;
    } else if( jetAbsETA<2.3 ){ 
        scale_JER = 1.134; scale_JERup = 1.134 + sqrt( 0.035*0.035 + 0.087*0.087 ); scale_JERdown = 1.134 - sqrt( 0.035*0.035 + 0.085*0.085 );
        diff_FullSim_FastSim = 3.12;
    } else if( jetAbsETA<5.0 ){ 
        scale_JER = 1.288; scale_JERup = 1.288 + sqrt( 0.127*0.127 + 0.155*0.155 ); scale_JERdown = 1.288 - sqrt( 0.127*0.127 + 0.153*0.153 );
        diff_FullSim_FastSim = 3.12;
    }

    double jetPt_JER     = pt();
    double jetPt_JERup   = pt();
    double jetPt_JERdown = pt();
    double diff_recojet_genjet = pt() - genJetPt();

    if( genJetPt()>10. ){
        jetPt_JER     = std::max( 0., genJetPt() + scale_JER     * ( diff_recojet_genjet ) );
        jetPt_JERup   = std::max( 0., genJetPt() + scale_JERup   * ( diff_recojet_genjet ) );
        jetPt_JERdown = std::max( 0., genJetPt() + scale_JERdown * ( diff_recojet_genjet ) );
    }

    if( resVariation==JetResVariation::PLUS )      factor = jetPt_JERup   / pt();
    else if( resVariation==JetResVariation::MINUS) factor = jetPt_JERdown / pt();
    else                                           factor = jetPt_JER     / pt();

    if( genJetPt()<=10. ) factor = 1.;

///??    std::vector<double> new4vec = fourVector();
///??    new4vec *= factor;
///??    diffVec  = getFourVector() - new4vec;
///??    setFourVector(new4vec);
///??    if( particleMass != 0.0 ) particleMass *= factor;
    // cout << "oldmass corr newmass " << oldmass << " " << corrFactor;
    // cout << " " << mass() << endl;
}


void Jet::adjForUnc(){
    double corrFactor = 1.0;
    switch( correctDirection ){
       case JetCorrDirection::NONE: return;
            break;
       case JetCorrDirection::PLUS:
            corrFactor += jecUncertainty;
            break;
       case JetCorrDirection::MINUS:
            corrFactor -= jecUncertainty;
            break;
       default: return;
            break;
    }

    if( corrFactor == 1.0 || corrFactor <= 0 || corrFactor > 5.0 ) return;  // Reject excess values
//??    FourVector new4vec = getFourVector();
    // double oldmass = massFromEnergyAndMomentum();
//??    new4vec *= corrFactor;
//??    diffVec += getFourVector() - new4vec;
//??    setFourVector(new4vec);
//??    if( particleMass != 0.0 ) particleMass *= corrFactor;
    // cout << "oldmass corr newmass " << oldmass << " " << corrFactor;
    // cout << " " << mass() << endl;
}


bool Jet::isGood() const {
    bool passesPt = pt() > 35;
    bool passesEta = fabs(eta()) < 2.4;
    bool jetID = false;
//    if( usedAlgorithm == JetAlgorithm::CA08PF || usedAlgorithm == JetAlgorithm::PF2PAT ){
        bool passNOD = NOD() > 1;
        bool passCEF = CEF() < 0.99;
        bool passNHF = NHF() < 0.99;
        bool passNEF = NEF() < 0.99;
        bool passCHF = true;
        bool passNCH = true;
        if( fabs(eta()) < 2.4 ){
            passCHF = CHF() > 0;
            passNCH = NCH() > 0;
        }
        jetID = passNOD && passCEF && passNHF && passNEF && passCHF && passNCH;
/*    } else {
        bool passesEMF = emf() > 0.01;
        bool passesN90Hits = n90Hits() > 1;
        bool passesFHPD = fHPD() < 0.98;
        jetID = passesEMF && passesN90Hits && passesFHPD;
    }
*/    return passesPt && passesEta && jetID;
}

// Values taken from
// https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagPerformance
bool Jet::isTaggedAsBJet(BtagAlgorithm::value type, BtagAlgorithm::workingPoint wp) const {
    float cut = -9998;
    switch( type ){
       case BtagAlgorithm::GenPartonFlavour:
            return abs(partonFlavor()) == 5;
            break;
       // WPs are taken from https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagPerformanceOP
       case BtagAlgorithm::CombinedSecondaryVertexBTag:
            if( wp == BtagAlgorithm::LOOSE )
                cut = 0.244;
            else if( wp == BtagAlgorithm::MEDIUM )
                cut = 0.679;
            else if( wp == BtagAlgorithm::TIGHT )
                cut = 0.898;
            break;
      default:
            return false;
    }
    return btag_discriminators[type] > cut;
}

bool Jet::isBJet(BtagAlgorithm::value type, BtagAlgorithm::workingPoint wp) const {
    int   jet_flavor = partonFlavor();
    float jet_pt   = pt();
    float jet_phi  = phi();
    float jet_eta  = eta();
    bool  isTagged = isTaggedAsBJet(type,wp);
/*
    //Initialize class with an unique seed
    BTagSFUtil btsfutil( int(jet_phi*100000) );

    //modify tags, make sure factors agree with the BtagAlgorithm above
    int workingPoint = BTagSFUtil::MEDIUM; 
    switch( wp ){
       case BtagAlgorithm::LOOSE : workingPoint = BTagSFUtil::LOOSE;  break;
       case BtagAlgorithm::MEDIUM: workingPoint = BTagSFUtil::MEDIUM; break;
       case BtagAlgorithm::TIGHT : workingPoint = BTagSFUtil::TIGHT;  break;
       default: break;
    }

    int var = 0;
    switch( bTagVariation ){
       case JetBtagVariation::NONE : var = 0; break;
       case JetBtagVariation::PLUS : var = 1; break;
       case JetBtagVariation::MINUS: var =-1; break;
       default: break;
    }

    //if( var!=0 && abs(jet_flavor)!=5 && abs(jet_flavor)!=4 ) var = 0;
///jet_pt += 20;
    double BTagSF      =  btsfutil.GetBTagSF(jet_pt, jet_eta, var, workingPoint);
    double BTageff     =  btsfutil.GetBTageff(workingPoint);
    double LightJetSF  =  btsfutil.GetLightJetSF(jet_pt, jet_eta, var, workingPoint);
    double LightJeteff =  btsfutil.GetLightJeteff(jet_pt, jet_eta, var, workingPoint);

    // SFc = SFb with twice the quoted uncertainty:
    if( var != 0 && abs(jet_flavor)==4 ){
       double delta = BTagSF - btsfutil.GetBTagSF(jet_pt, jet_eta, 0, workingPoint);
       BTagSF += delta;
    }

    btsfutil.modifyBTagsWithSF(isTagged, jet_flavor, BTagSF, BTageff, LightJetSF, LightJeteff);
*/    return (isTagged);
}
