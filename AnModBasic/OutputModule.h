#ifndef OutputModule_h
#define OutputModule_h
#include "AppFramework/AppParameter.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"
//#include "AnUtils/CompiledFormula.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "TFile.h"
#include "TTree.h"

class OutputModule : public AppModule{
private:
    AppParameter<string> output;          // File name for writing (no files will be created if empty)
    AppParameter<string> leaflist;        // standard root leaflist in a form "par[n]/type:..." (1D arrays only)
//    AppParameter<CompiledFormula> filter; // 

    std::vector<std::string> colnames;
    std::vector<char>        coltypes;
    std::vector<size_t>      colarray;

    TFile *outputFile;
    TTree *microTuple;

    ofstream csvfile;

    AppResult beginJob(AppEvent& event);
    AppResult beginRun(AppEvent& event);
    AppResult endRun  (AppEvent& event);
    AppResult endJob  (AppEvent& event);

    AppResult event(AppEvent& event);

public:

    OutputModule(const char *nm, const char *descr):AppModule(nm,descr),
        output  (*this,"output",   "./output.csv"),
        leaflist(*this,"leaflist", ""),
//        filter  (*this,"filter",   ""),
        outputFile(0), microTuple(0){ }
    virtual ~OutputModule(){}
};

#endif
