using namespace std;
#include "HLTReader.h"
#include "TTree.h"
#include "TBranch.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"

edm::Wrapper<trigger::TriggerEvent> *__HLTevs = new edm::Wrapper<trigger::TriggerEvent>;

AppResult HLTReader::beginRun(AppEvent& event) {
    TTree *Events = 0;
    if( event.get("Events",Events) || !Events )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'Events' tree found");

    TBranch *inputHltBranch = Events->GetBranch("triggerTriggerEvent_hltTriggerSummaryAOD__HLT.");
    if( !inputHltBranch )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'triggerTriggerEvent_hltTriggerSummaryAOD__HLT.' branch found");
    inputHltBranch->SetAddress(&__HLTevs);

    return AppResult();
}

AppResult HLTReader::event(AppEvent& event) {
    objects.clear();

    if( __HLTevs->isPresent() ){
        const trigger::TriggerEvent *hlt = __HLTevs->product();
        const trigger::TriggerObjectCollection& toc = hlt->getObjects();
        for(std::vector<trigger::TriggerObject>::const_iterator obj=toc.begin(); obj!=toc.end(); obj++){
            ParticlePointer particle( new Particle(obj->energy(), obj->px(), obj->py(), obj->pz()) );
            particle->setPdgId( obj->id() );
            objects.push_back( particle );
        }
    }

    sort   (objects.begin(),objects.end());
    reverse(objects.begin(),objects.end());

    event.put("HLTObjects",(const HLTObjectCollection*)&objects);

    return AppResult();
}
