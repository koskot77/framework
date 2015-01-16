#ifndef OutputModule_h
#define OutputModule_h
#include "AppFramework/AppParameter.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"
#include "AnUtils/CompiledFormula.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "TTree.h"

class OutputModule : public AppModule{
private:
    AppParameter<string> output;          // File name for writing (no files will be created if empty)
    AppParameter<string> leaflist;        // standard root leaflist in a form "par[n]/type:..." (1D arrays only)
    AppParameter<CompiledFormula> filter; // 

    vector<string>  colnames;
    vector<char>    coltypes;
    vector<size_t>  colarray;

    TTree *microTuple;

    ofstream csvfile;

    AppResult beginJob(AppEvent& event);
    AppResult beginRun(AppEvent& event);
    AppResult endRun  (AppEvent& event);
    AppResult endJob  (AppEvent& event);

    AppResult event(AppEvent& event);

public:

    OutputModule(const char *nm, const char *descr):AppModule(nm,descr),
        output  (*this,"output",   ""),
        leaflist(*this,"leaflist", ""),
        filter  (*this,"filter"),
        microTuple(0){ }
    virtual ~OutputModule(){}
};

#endif
