using namespace std;
#include "METReader.h"
#include "TTree.h"
#include "TBranch.h"

#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METCollection.h"
//#include "DataFormats/PatCandidates/interface/MET.h"
//edm::Wrapper<pat::METCollection> *__patMETs = new edm::Wrapper<pat::METCollection>();
edm::Wrapper<reco::METCollection> *__pfMETs = new edm::Wrapper<reco::METCollection>();

AppResult METReader::beginRun(AppEvent& event) {
    TTree *Events = 0;
    if( event.get("Events",Events) || !Events )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'Events' tree found");

//    TBranch *inputMetBranch = Events->GetBranch("patMETs_slimmedMETs__PAT.");
//    if( !inputMetBranch )
//        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'patMETs_slimmedMETs__PAT.' branch found");
//    inputMetBranch->SetAddress(&__patMETs);

    TBranch *inputMetBranch = Events->GetBranch("recoPFMETs_pfMet__HLT.");
    if( !inputMetBranch )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'recoPFMETs_pfMet__HLT.' branch found");
    inputMetBranch->SetAddress(&__pfMETs);

    return AppResult();
}

AppResult METReader::event(AppEvent& event) {

    met.setEPxPyPz(0,0,0,0);

//    if( __patMETs->isPresent() ){
//        const pat::MET &pmet = __patMETs->product()->front();
    if( __pfMETs->isPresent() ){
        const reco::MET &pmet = __pfMETs->product()->front();
        met.setEPxPyPz( sqrt(pmet.px()*pmet.px() + pmet.py()*pmet.py()), pmet.px(), pmet.py(), 0 );
    }

    event.put("ETmiss",(const MET*)&met);

    return AppResult();
}
