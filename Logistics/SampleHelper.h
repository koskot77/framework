#ifndef SamplerHelper_h
#define SamplerHelper_h
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"
#include <iostream>
#include <map>

class SampleHelper : public AppAgent<string>, public AppAgent<const char*> {
private:
//    AppParameter<int> split;
    map<string,string> sampleNames;

public:
    list<string> dataList(void) const ;

    bool set  (const char *name, const string &c){ return false; } // do nothing 
    bool fetch(const char *name,       string &c);

    bool set  (const char *name, const char* &c){ return false; } // do nothing
    bool fetch(const char *name, const char* &c);

    AppResult beginRun(const AppEvent& ev){ return AppResult(); }
    AppResult beginJob(void){ return AppResult(); }
    AppResult endJob  (void){ return AppResult(); }

    SampleHelper(void); //:AppAgentWrapper("sampleHelper","simple object-counter"),AppAgent<string>(),AppAgent<const char*>(){}
};

#endif
