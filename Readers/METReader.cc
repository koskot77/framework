using namespace std;
#include "METReader.h"
#include "TTree.h"
#include "TBranch.h"

#include "DataFormats/PatCandidates/interface/MET.h"
edm::Wrapper<pat::METCollection> *__patMETs = new edm::Wrapper<pat::METCollection>();

AppResult METReader::beginRun(AppEvent& event) {
    TTree *Events = 0;
    if( event.get("Events",Events) || !Events )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'Events' tree found");

    TBranch *inputMetBranch = Events->GetBranch("patMETs_slimmedMETs__PAT.");
    if( !inputMetBranch )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'patMETs_slimmedMETs__PAT.' branch found");
    inputMetBranch->SetAddress(&__patMETs);

    return AppResult();
}

AppResult METReader::event(AppEvent& event) {

    met.setEPxPyPz(0,0,0,0);

    if( __patMETs->isPresent() ){
        const pat::MET &pmet = __patMETs->product()->front();
        met.setEPxPyPz( sqrt(pmet.px()*pmet.px() + pmet.py()*pmet.py()), pmet.px(), pmet.py(), 0 );
    }

    event.put("ETmiss",(const MET*)&met);

    return AppResult();
}
