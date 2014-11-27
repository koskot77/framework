using namespace std;
#include "OutputModule.h"

AppResult OutputModule::beginJob(AppEvent& event){
    outputFile = TFile::Open(output.c_str(),"RECREATE");
    microTuple = new TTree("micro","micro");
    return AppResult();
}

AppResult OutputModule::endJob  (AppEvent& event){
//   if( microTuple ) delete microTuple;
//   if( outputFile ) delete outputFile;
    return AppResult();
}

AppResult OutputModule::beginRun(AppEvent& event){
    // Check output state

    return AppResult();
}

AppResult OutputModule::endRun(AppEvent& event){

    return AppResult();
}

AppResult OutputModule::event(AppEvent& event){

    return AppResult();
}
