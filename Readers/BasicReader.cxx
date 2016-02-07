using namespace std;
#include "BasicReader.h"
#include "TTree.h"
#include "TBranch.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"

edm::Wrapper<double>                      *__rho      = new edm::Wrapper<double>();
edm::Wrapper< std::vector<reco::Vertex> > *__vertices = new edm::Wrapper< std::vector<reco::Vertex> >();
              edm::EventAuxiliary         *__eventAux = new edm::EventAuxiliary();

AppResult BasicReader::beginRun(AppEvent& event) {
    TTree *Events = 0;
    if( event.get("Events",Events) || !Events )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'Events' tree found");

//    TBranch *inputRho = Events->GetBranch("double_fixedGridRhoFastjetAll__RECO.");
//    if( !inputRho ) return AppResult(AppResult::STOP|AppResult::ERROR,"No 'double_fixedGridRhoFastjetAll__RECO.' branch found");
    TBranch *inputRho = Events->GetBranch("double_fixedGridRhoFastjetAll__HLT.");
    if( !inputRho ) return AppResult(AppResult::STOP|AppResult::ERROR,"No 'double_fixedGridRhoFastjetAll__HLT.' branch found");
    inputRho->SetAddress(&__rho);

//    TBranch *inputVtx = Events->GetBranch("recoVertexs_offlineSlimmedPrimaryVertices__PAT.");
//    if( !inputVtx ) return AppResult(AppResult::STOP|AppResult::ERROR,"No 'recoVertexs_offlineSlimmedPrimaryVertices__PAT.' branch found");
    TBranch *inputVtx = Events->GetBranch("recoVertexs_offlinePrimaryVertices__HLT.");
    if( !inputVtx ) return AppResult(AppResult::STOP|AppResult::ERROR,"No 'recoVertexs_offlinePrimaryVertices__HLT.' branch found");
    inputVtx->SetAddress(&__vertices);

    TBranch *inputAux = Events->GetBranch("EventAuxiliary");
    if( !inputAux ) return AppResult(AppResult::STOP|AppResult::ERROR,"No 'EventAuxiliary' branch found");
    inputAux->SetAddress(&__eventAux);

    return AppResult();
}

AppResult BasicReader::event(AppEvent& event) {

    bool foundVertex = false;
    if( __vertices->isPresent() )
      for(std::vector<reco::Vertex>::const_iterator vtx = __vertices->product()->begin(); vtx != __vertices->product()->end(); vtx++){
        // looks for the first good vertex
        if( vtx->ndof() > 4 && fabs(vtx->z()) <= 24 && fabs(vtx->position().rho()) <= 2 ){
            vx = vtx->x();
            vy = vtx->y();
            vz = vtx->z();
            foundVertex = true;
            break;
        }
      }

    if( !foundVertex ) return AppResult(AppResult::SKIP|AppResult::WARNING,"Didn't find a good vertex in event");

    event.put("vx",vx);
    event.put("vy",vy);
    event.put("vz",vz);
    event.put("rho_event", (__rho->isPresent() ? *(__rho->product()) : -1) );

    eventNumber = __eventAux->id().event();
    runNumber   = __eventAux->id().run();

    event.put("eventNumber",eventNumber);
    event.put("runNumber",  runNumber  );

    return AppResult();
}
