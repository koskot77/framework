using namespace std;
#include "JetReader.h"
#include "TTree.h"
#include "TBranch.h"
#include "ProductArea/BNcollections/interface/BNjet.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include <algorithm>

edm::Wrapper<BNjetCollection> *__bnJets = new edm::Wrapper<BNjetCollection>();

AppResult JetReader::beginJob(AppEvent& event) {
    TTree *Events;
    event.get("Events",Events);

    TBranch *inputJetsBranch = Events->GetBranch("BNjets_BNproducer_selectedPatJetsPFlow_BEANs.");
    if( !inputJetsBranch )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'BNjets_BNproducer_selectedPatJetsPFlow_BEANs.' branch found");
    inputJetsBranch->SetAddress(&__bnJets);

    return AppResult();
}

AppResult JetReader::event(AppEvent& event) {
    jets.clear();
    if( __bnJets->isPresent() )
      for(BNjetCollection::const_iterator pjet = __bnJets->product()->begin(); pjet != __bnJets->product()->end(); pjet++){
        float energy = pjet->energy;
        float px = pjet->px;
        float py = pjet->py;
        float pz = pjet->pz;
        JetPointer jet(new Jet(energy, px, py, pz));
//        jet->setUsedAlgorithm(usedAlgorithm);
        jet->setMass  ( pjet->mass     );
        jet->setCharge( pjet->charge   );
        jet->setGenJetPt(pjet->genJetPT);

        jet->setEMF( 0 );
        jet->setN90Hits( 0 );
        jet->setFHPD( 0 );

        jet->setDiscriminatorForBtagType(
            pjet->btagCombinedSecVertex,
            BtagAlgorithm::CombinedSecondaryVertexBTag);

        jet->setNOD( pjet->nconstituents );
        jet->setCEF( pjet->chargedEmEnergyFraction );
        jet->setNHF( pjet->neutralHadronEnergyFraction );
        jet->setNEF( pjet->neutralEmEnergyFraction );
        jet->setCHF( pjet->chargedHadronEnergyFraction );
        jet->setNCH( pjet->chargedMultiplicity );
        jet->setPartonFlavour( pjet->flavour );
        jet->setJECUnc( pjet->JESunc );//pjet->);
        // perform adjustment only for MC:
        if( pjet->flavour != 0 ){
//            jet->adjForRes();
//            jet->adjForUnc();
        }
        if( jet->isGood() ) jets.push_back(jet);
      }

    sort   (jets.begin(),jets.end());
    reverse(jets.begin(),jets.end());

    event.put("jets",(const JetCollection*)&jets);

    return AppResult();
}
