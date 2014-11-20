using namespace std;
#include "JetReader.h"
#include "TTree.h"
#include "TBranch.h"
///#include "ProductArea/BNcollections/interface/BNjet.h"

AppResult JetReader::beginJob(AppEvent& event) {
    TTree *Events;
    event.get("Events",Events);
///    bnj = Events->GetBranch("BNjets_BNproducer_selectedPatJetsPFlow_BEANs.obj");
///    bnj->SetAddress(&bnJets);
    return AppResult();
}

AppResult JetReader::event(AppEvent& event) {
/*
    jets.clear();
    for(vector<BNjet>::const_iterator jetstruc = bnJets.begin(); jetstruc != bnJets.end(); jetstruc++){
        float energy = jetstruc->energy;
        float px = jetstruc->px;
        float py = jetstruc->py;
        float pz = jetstruc->pz;
        JetPointer jet(new Jet(energy, px, py, pz));
//        jet->setUsedAlgorithm(usedAlgorithm);
        jet->setMass(jetstruc->mass);
        jet->setCharge(jetstruc->charge);
        jet->setGenJetPt(jetstruc->genJetPT);

        jet->setEMF(0);
        jet->setN90Hits(0);
        jet->setFHPD(0);

//        jet->setDiscriminatorForBtagType(
//            jetstruc->btagCombinedSecVertex,
//            BtagAlgorithm::CombinedSecondaryVertexBTag);

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

        jets.push_back(jet);
    }
*/
    return AppResult();
}
