#ifndef Analyser_h
#define Analyser_h
#include "AppFramework/AppParameter.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"

#include "TFile.h"
#include "TTree.h"

class Analyser : public AppModule{
private:
    AppParameter<string> output;

    int    run, evt;
    int    numberOfJets;
    double jetPtRec[4], jetEtaRec[4], jetPhiRec[4], jetCSV[4];
    double m3jets, met;

    TFile *outputFile;
    TTree *microTuple;

    AppResult beginJob(AppEvent& event);
    AppResult beginRun(AppEvent& event){ return AppResult(); }
    AppResult endRun  (AppEvent& event){ return AppResult(); }
    AppResult endJob  (AppEvent& event);

    AppResult event (AppEvent& event);

public:
    Analyser(const char *nm, const char *descr):AppModule(nm,descr),output(*this,"output","output.root") {}
    virtual ~Analyser(void){}
};

#endif
