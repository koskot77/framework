using namespace std;
#include "JetReader.h"
#include "TTree.h"
#include "TBranch.h"

AppResult JetReader::beginJob(AppEvent& event) {
    TTree *Events;
    event.get("Events",Events);
    bpj = Events->GetBranch("patJets_slimmedJetsAK8__PAT.obj");
    bpj->SetAddress(&patJets);
    return AppResult();
}

AppResult JetReader::event(AppEvent& event) {
    jets.clear();
    for(vector<pat::Jet>::const_iterator pjet = patJets.begin(); pjet != patJets.end(); pjet++){
        float energy = pjet->energy();
        float px = pjet->px();
        float py = pjet->py();
        float pz = pjet->pz();
        JetPointer jet(new Jet(energy, px, py, pz));
//        jet->setUsedAlgorithm(usedAlgorithm);
        jet->setMass  ( pjet->mass()     );
        jet->setCharge( pjet->jetCharge());
        if( pjet->genJet() )
            jet->setGenJetPt(pjet->genJet()->pt());

        jet->setEMF( pjet->emEnergyFraction() );
        jet->setN90Hits( pjet->jetID().n90Hits );
        jet->setFHPD( pjet->jetID().approximatefHPD );

        jet->setDiscriminatorForBtagType(
            pjet->bDiscriminator("combinedSecondaryVertexBJetTags"),
            BtagAlgorithm::CombinedSecondaryVertexBTag);

        jet->setNOD( pjet->nConstituents() );
        jet->setCEF( pjet->chargedEmEnergyFraction() );
        jet->setNHF( pjet->neutralHadronEnergyFraction() );
        jet->setNEF( pjet->neutralEmEnergyFraction() );
        jet->setCHF( pjet->chargedHadronEnergyFraction() );
        jet->setNCH( pjet->chargedMultiplicity() );
        jet->setPartonFlavour( pjet->partonFlavour() );
        jet->setJECUnc( -1 );//pjet->);
        // perform adjustment only for MC:
        if( pjet->partonFlavour() != 0 ){
//            jet->adjForRes();
//            jet->adjForUnc();
        }

        jets.push_back(jet);
    }

    return AppResult();
}
