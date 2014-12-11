using namespace std;
#include "METReader.h"
#include "TTree.h"
#include "TBranch.h"

#include "ProductArea/BNcollections/interface/BNmet.h"
#include "DataFormats/Common/interface/Wrapper.h"

edm::Wrapper<BNmetCollection> *__bnMETs = new edm::Wrapper<BNmetCollection>();

AppResult METReader::beginRun(AppEvent& event) {
    TTree *Events = 0;
    if( event.get("Events",Events) || !Events )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'Events' tree found");

    TBranch *inputMetBranch = Events->GetBranch("BNmets_BNproducer_pfType1CorrectedMetBN_BEANs.");
    if( !inputMetBranch )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'BNmets_BNproducer_pfType1CorrectedMetBN_BEANs.' branch found");
    inputMetBranch->SetAddress(&__bnMETs);

    return AppResult();
}

AppResult METReader::event(AppEvent& event) {

    met.setEPxPyPz(0,0,0,0);

    if( __bnMETs->isPresent() ){
        const BNmet &pmet = __bnMETs->product()->front();
        met.setEPxPyPz( sqrt(pmet.px*pmet.px + pmet.py*pmet.py), pmet.px, pmet.py, 0 );
    }

    event.put("ETmiss",(const MET*)&met);

    return AppResult();
}
