using namespace std;
#include "Analyser.h"
#include "AnObjects/Jet.h"
#include "AnObjects/Muon.h"
#include "AnObjects/Electron.h"
#include "AnObjects/MET.h"

AppResult Analyser::event(AppEvent& event){

    const JetCollection *jets;
    if( event.get("jets",jets) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No jets found");
    std::cout<<" #jets = "<<jets->size()<<std::endl;

    const ElectronCollection *electrons;
    if( event.get("electrons",electrons) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No electrons found");
    std::cout<<" #electrons = "<<electrons->size()<<std::endl;

    const MuonCollection *muons;
    if( event.get("muons",muons) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No muons found");
    std::cout<<" #muons = "<<muons->size()<<std::endl;

    const MET *met;
    if( event.get("met",met) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No met found");
    std::cout<<" met= "<<met->pt()<<std::endl;

    return AppResult();
}
