using namespace std;
#include "JetReader.h"
#include "TTree.h"
#include "TBranch.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/CATopJetTagInfo.h"
#include <algorithm>

edm::Wrapper<pat::JetCollection>             *__patJets = new edm::Wrapper<pat::JetCollection>();
edm::Wrapper<vector<reco::CATopJetTagInfo> > *__tagInfo = new edm::Wrapper<vector<reco::CATopJetTagInfo> >();

AppResult JetReader::beginRun(AppEvent& event) {
    TTree *Events = 0;
    if( event.get("Events",Events) || !Events )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'Events' tree found");

    TBranch *inputJetsBranch = Events->GetBranch("patJets_slimmedJetsAK8__PAT.");
    if( !inputJetsBranch )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'patJets_slimmedJetsAK8__PAT.' branch found");
    inputJetsBranch->SetAddress(&__patJets);

    TBranch *inputTagsBranch = Events->GetBranch("recoCATopJetTagInfos_caTopTagInfos__PAT.");
    if( !inputTagsBranch )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'recoCATopJetTagInfos_caTopTagInfos__PAT.' branch found");
    inputTagsBranch->SetAddress(&__tagInfo);

    return AppResult();
}

AppResult JetReader::event(AppEvent& event) {
    jets.clear();
    if( __patJets->isPresent() )
      for(vector<pat::Jet>::const_iterator pjet = __patJets->product()->begin(); pjet != __patJets->product()->end(); pjet++){
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

        //jet->setEMF( pjet->emEnergyFraction() ); // only for CaloJets
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
        if( jet->isGood() ) jets.push_back(jet);
      }


    if( __tagInfo->isPresent() ){
      int jetIndex = 0;
      for(vector<reco::CATopJetTagInfo>::const_iterator ptag = __tagInfo->product()->begin(); ptag != __tagInfo->product()->end(); ptag++, jetIndex++){
        if( jetIndex < jets.size() ){
          // ptag->jet(); doesn't work in miniAOD
          jets[jetIndex]->setTagMass ( ptag->properties().topMass  );
          jets[jetIndex]->setNSubJets( ptag->properties().nSubJets );
        }
      }
    }

    sort   (jets.begin(),jets.end());
    reverse(jets.begin(),jets.end());

    event.put("jets",(const JetCollection*)&jets);

    return AppResult();
}
