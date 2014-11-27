#ifndef OutputModule_h
#define OutputModule_h
#include "AppFramework/AppParameter.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"
#include <string>
//#include <fstream>
//#include <iostream>
#include "TFile.h"
#include "TTree.h"

class OutputModule : public AppModule{
private:
	AppParameter<string> output;          // File name for writing ( no files will be created if empty )
	AppParameter<string> leaflist;        // Writing mode ( appending for default )

        TFile *outputFile;
        TTree *microTuple;

//	ofstream file;

	AppResult beginJob(AppEvent& event);
	AppResult beginRun(AppEvent& event);
	AppResult endRun  (AppEvent& event);
	AppResult endJob  (AppEvent& event);

	AppResult event(AppEvent& event);
//	size_t addRecoData(AppEvent& event, RawEvent& erec);

public:

	OutputModule(const char *nm, const char *descr):AppModule(nm,descr),
		output  (*this,"output",   "./output.root"),
		leaflist(*this,"leaflist", ""),
                outputFile(0), microTuple(0){ }
	virtual ~OutputModule(){}
};

#endif
