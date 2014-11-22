#ifndef OutputModule_h
#define OutputModule_h
#include "AppFramework/AppParameter.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"
#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <bzlib.h>

class OutputModule : public AppModule{
private:
	AppParameter<string>  output;          // File name for writing ( no files will be created if empty )
	AppParameter<bool>    append;          // Writing mode ( appending for default )

	ofstream file;
	BZFILE  *bzfile;

	AppResult beginJob(AppEvent& event){ return AppResult(); }
	AppResult beginRun(AppEvent& event);
	AppResult endRun  (AppEvent& event);
	AppResult endJob  (AppEvent& event){ return AppResult(); }

	AppResult event(AppEvent& event);
//	size_t addRecoData(AppEvent& event, RawEvent& erec);

public:

	OutputModule(const char *nm, const char *descr):AppModule(nm,descr),
		output        (*this,"output", "./output.dat"),
		append        (*this,"append", true){ bzfile=0; }
	virtual ~OutputModule(){}
};

#endif
