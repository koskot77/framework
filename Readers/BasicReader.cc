using namespace std;
#include "BasicReader.h"
#include "TTree.h"
#include "TBranch.h"

#include "DataFormats/VertexReco/interface/Vertex.h"

edm::Wrapper<double> *__rho = new edm::Wrapper<double>();
edm::Wrapper< std::vector<reco::Vertex> > *__vertices = new edm::Wrapper< std::vector<reco::Vertex> >();

AppResult BasicReader::beginJob(AppEvent& event) {
    TTree *Events;
    event.get("Events",Events);

    TBranch *inputRho = Events->GetBranch("double_fixedGridRhoFastjetAll__RECO.");
    if( !inputRho ) return AppResult(AppResult::STOP|AppResult::ERROR,"No 'recoVertexs_offlineSlimmedPrimaryVertices__PAT.' branch found");
    inputRho->SetAddress(&__rho);

    TBranch *inputVtx = Events->GetBranch("recoVertexs_offlineSlimmedPrimaryVertices__PAT.");
    if( !inputVtx ) return AppResult(AppResult::STOP|AppResult::ERROR,"No 'recoVertexs_offlineSlimmedPrimaryVertices__PAT.' branch found");
    inputVtx->SetAddress(&__vertices);

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

    return AppResult();
}
