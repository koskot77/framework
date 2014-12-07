using namespace std;
#include "ElectronReader.h"
#include "TTree.h"
#include "TBranch.h"

#include "ProductArea/BNcollections/interface/BNelectron.h"
#include "DataFormats/Common/interface/Wrapper.h"

edm::Wrapper<BNelectronCollection> *__bnElectrons = new edm::Wrapper<BNelectronCollection>();

AppResult ElectronReader::beginRun(AppEvent& event) {
    TTree *Events;
    event.get("Events",Events);

    TBranch *inputElectronsBranch = Events->GetBranch("BNelectrons_BNproducer_selectedPatElectronsLoosePFlow_BEANs.");
    if( !inputElectronsBranch )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'BNelectrons_BNproducer_selectedPatElectronsLoosePFlow_BEANs.' branch found");
    inputElectronsBranch->SetAddress(&__bnElectrons);

    return AppResult();
}

AppResult ElectronReader::event(AppEvent& event) {
    electrons.clear();
    if( __bnElectrons->isPresent() )
      for(BNelectronCollection::const_iterator pele = __bnElectrons->product()->begin(); pele != __bnElectrons->product()->end(); pele++){
        float energy = pele->energy;
        float px = pele->px;
        float py = pele->py;
        float pz = pele->pz;
        ElectronPointer electron(new Electron(energy, px, py, pz));
//        electron->setXYZ   ( pele->vx(), pele->vy(), pele->vz() );
        electron->setCharge( pele->charge );

///        electrons->setEcalIsolation   ( pele->ecalIsoDR03  );
///        electrons->setHcalIsolation   ( pele->hcalIsoDR03  );
///        electrons->setTrackerIsolation( pele->trackIsoDR03 );
        // new PU corrected isolation components
        electron->setEcalIsolation       ( pele->photonIsoDR03          );
        electron->setHcalIsolation       ( pele->neutralHadronIsoDR03   );
        electron->setTrackerIsolation    ( pele->chargedHadronIsoDR03   );
        electron->setPFChargedPUinIsoCone( pele->puChargedHadronIsoDR03 );
        electron->setPFEventEnergyDensity( pele->rhoPrime );
        electron->setEffectiveArea       ( pele->AEffDr03 );

        electron->setMvaNonTrigV0eID( pele->mvaNonTrigV0 );
        electron->setMvaTrigV0eID   ( pele->mvaTrigV0    );

        electron->setD0                      ( pele->correctedD0Vertex );
        electron->setZDistanceToPrimaryVertex( pele->correctedDZ       );

        electron->setPFGammaIsolation        ( pele->photonIso        );
        electron->setPFChargedHadronIsolation( pele->chargedHadronIso );
        electron->setPFNeutralHadronIsolation( pele->neutralHadronIso );

        if( pele->genId ){
            Particle mc; //, mother; //, daughter1, daughter2;
            mc.setPdgId   ( pele->genId );
            mc.setPtEtaPhi( pele->genPT, pele->genEta, pele->genPhi );
            mc.setCharge  ( pele->genCharge );
            mc.setMotherPdgId( pele->genMotherId );
//            mother.setPdgId   (pele->genMotherId);
//            mother.setPtEtaPhi(pele->genMotherPT,pele->genMotherEta,pele->genMotherPhi);
//            mother.setCharge  (pele->genMotherCharge);
            electron->setGenLepton(mc);
        }

        if( electron->isLoose() ) electrons.push_back(electron);
    }

    sort   (electrons.begin(),electrons.end());
    reverse(electrons.begin(),electrons.end());

    event.put("electrons",(const ElectronCollection*)&electrons);

    return AppResult();
}
