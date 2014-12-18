using namespace std;
#include "TriggerReader.h"
#include "TTree.h"
#include "TBranch.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include <utility>
#include "DataFormats/Provenance/interface/ParameterSetBlob.h"
#include "DataFormats/Provenance/interface/Hash.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

edm::Wrapper<edm::TriggerResults>                *__patTrigs = new edm::Wrapper<edm::TriggerResults>();
//edm::Wrapper<pat::PackedTriggerPrescales>        *__patPresc = new edm::Wrapper<pat::PackedTriggerPrescales>();
pair<edm::ParameterSetID,edm::ParameterSetBlob>  *__psets    = new pair<edm::ParameterSetID,edm::ParameterSetBlob>();
map <edm::ParameterSetID,edm::ParameterSetBlob> psetMap;
edm::ParameterSetID                             currentTriggerId, previousTriggerId;

AppResult TriggerReader::beginRun(AppEvent& event) {
    TTree *Events = 0, *PSets = 0;
    if( event.get("Events",Events) || !Events )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'Events' tree found");
    if( event.get("PSets", PSets)  || !PSets )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'PSets' tree found");

    TBranch *inputPSetBranch = PSets->GetBranch("IdToParameterSetsBlobs");
    if( !inputPSetBranch )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'IdToParameterSetsBlobs' branch found");
    inputPSetBranch->SetAddress(&__psets);

    psetMap.clear();
    for(int i=0; i<PSets->GetEntries(); i++){
        PSets->GetEntry(i);
        psetMap.insert( pair<edm::ParameterSetID,edm::ParameterSetBlob>(__psets->first, __psets->second) );
    }

//    TBranch *inputPrescalesBranch = Events->GetBranch("edmTriggerResults_TriggerResults__PAT.");
//    if( !inputPrescalesBranch )
//        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'patPackedTriggerPrescales_patTrigger__PAT.' branch found");
//    inputPrescalesBranch->SetAddress(&__patPresc);

    TBranch *inputTriggerBranch = Events->GetBranch("edmTriggerResults_TriggerResults__HLT.");
    if( !inputTriggerBranch )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'edmTriggerResults_TriggerResults__PAT.' branch found");
    inputTriggerBranch->SetAddress(&__patTrigs);

    return AppResult();
}

AppResult TriggerReader::event(AppEvent& event) {

///    result.clear();

    if( __patTrigs->isPresent() ){
        const edm::TriggerResults *trgRes = __patTrigs->product();
        if( !trgRes ) AppResult(AppResult::STOP|AppResult::ERROR,"Problem with getting the trigger results");
        if(  trgRes->size() < result.maxSize )
            AppResult(AppResult::STOP|AppResult::ERROR,"Too many trigges");

        if( currentTriggerId != trgRes->parameterSetID() ){

            previousTriggerId = currentTriggerId;
            currentTriggerId  = trgRes->parameterSetID();

            map<edm::ParameterSetID,edm::ParameterSetBlob>::const_iterator trigPSet = psetMap.find( currentTriggerId );
            if( trigPSet == psetMap.end() )
                AppResult(AppResult::STOP|AppResult::ERROR,"Problem with getting the trigger names");

            edm::ParameterSet pset(trigPSet->second.pset());
            pset.setID(trigPSet->first);

            vector<string> triggerNames = pset.getParameter< vector<string> >("@trigger_paths");
            if( triggerNames.size() < result.maxSize )
                result.setNames( triggerNames );
            else
                AppResult(AppResult::STOP|AppResult::ERROR,"Too many trigger names");

        } else
            result.resetNamesChanged();

        for(unsigned int triggerIndex=0; triggerIndex<trgRes->size(); triggerIndex++){
            result.setWasRun(triggerIndex, trgRes->wasrun(triggerIndex) );
            result.setAccept(triggerIndex, trgRes->accept(triggerIndex) );
            result.setError (triggerIndex, trgRes->error (triggerIndex) );
        }
    }

    event.put("trigger",(const Trigger*)&result);

    return AppResult();
}
