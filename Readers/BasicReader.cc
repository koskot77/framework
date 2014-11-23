using namespace std;
#include "BasicReader.h"
#include "TTree.h"
#include "TBranch.h"

AppResult BasicReader::beginJob(AppEvent& event) {
    TTree *Events;
    event.get("Events",Events);
//    brho = Events->GetBranch("double_fixedGridRhoFastjetAll__RECO.obj");
//    brho->SetAddress(&rho);
    bvtx = Events->GetBranch("recoVertexs_offlineSlimmedPrimaryVertices__PAT.obj");
    bvtx->SetAddress(&vertices);
    return AppResult();
}

AppResult BasicReader::event(AppEvent& event) {

    bool foundVertex = false;
    for(std::vector<reco::Vertex>::const_iterator vtx = vertices.begin(); vtx != vertices.end(); vtx++){
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
    event.put("rho_event",rho);

    return AppResult();
}
