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
        jet->setMass(pjet->mass());
        jet->setCharge(pjet->charge());
//        jet->setGenJetPt(pjet->genJetPT);

        jet->setEMF(0);
        jet->setN90Hits(0);
        jet->setFHPD(0);

//        jet->setDiscriminatorForBtagType(
//            jetstruc->btagCombinedSecVertex,
//            BtagAlgorithm::CombinedSecondaryVertexBTag);
/*
        jet->setNOD(jetstruc->nconstituents);
        jet->setCEF(jetstruc->chargedEmEnergyFraction);
        jet->setNHF(jetstruc->neutralHadronEnergyFraction);
        jet->setNEF(jetstruc->neutralEmEnergyFraction);
        jet->setCHF(jetstruc->chargedHadronEnergyFraction);
        jet->setNCH(jetstruc->chargedMultiplicity);
        jet->setPartonFlavour(jetstruc->flavour);
        jet->setJECUnc(jetstruc->JESunc);
        // perform adjustment only for MC:
        if( jetstruc->flavour!=0 ){
//            jet->adjForRes();
//            jet->adjForUnc();
        }
*/
        jets.push_back(jet);
    }

    return AppResult();
}
