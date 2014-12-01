using namespace std;
#include "BasicReader.h"
#include "TTree.h"
#include "TBranch.h"

#include "ProductArea/BNcollections/interface/BNprimaryvertex.h"
#include "ProductArea/BNcollections/interface/BNevent.h"
#include "DataFormats/Common/interface/Wrapper.h"

edm::Wrapper<BNprimaryvertexCollection> *__vertices = new edm::Wrapper<BNprimaryvertexCollection>();
edm::Wrapper<BNeventCollection>         *__eventAux = new edm::Wrapper<BNeventCollection>();

AppResult BasicReader::beginJob(AppEvent& event) {
    TTree *Events;
    event.get("Events",Events);

    TBranch *inputVtx = Events->GetBranch("BNprimaryvertexs_BNproducer_offlinePrimaryVertices_BEANs.");
    if( !inputVtx ) return AppResult(AppResult::STOP|AppResult::ERROR,"No 'BNprimaryvertexs_BNproducer_offlinePrimaryVertices_BEANs.' branch found");
    inputVtx->SetAddress(&__vertices);

    TBranch *inputAux = Events->GetBranch("BNevents_BNproducer__BEANs.");
    if( !inputAux ) return AppResult(AppResult::STOP|AppResult::ERROR,"No 'BNevents_BNproducer__BEANs.' branch found");
    inputAux->SetAddress(&__eventAux);

    return AppResult();
}

AppResult BasicReader::event(AppEvent& event) {

    bool foundVertex = false;
    if( __vertices->isPresent() )
      for(BNprimaryvertexCollection::const_iterator vtx = __vertices->product()->begin(); vtx != __vertices->product()->end(); vtx++){
        // looks for the first good vertex
        if( vtx->ndof > 4 && fabs(vtx->z) <= 24 && fabs(vtx->rho) <= 2 ){
            vx = vtx->x;
            vy = vtx->y;
            vz = vtx->z;
            foundVertex = true;
            break;
        }
      }

    if( !foundVertex ) return AppResult(AppResult::SKIP|AppResult::WARNING,"Didn't find a good vertex in event");

    event.put("vx",vx);
    event.put("vy",vy);
    event.put("vz",vz);

    const BNevent &evtstruc = __eventAux->product()->at(0);

    eventNumber = evtstruc.evt;
    runNumber   = evtstruc.run;

    event.put("eventNumber",eventNumber);
    event.put("runNumber",  runNumber  );

    return AppResult();
}
