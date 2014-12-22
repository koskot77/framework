#ifndef SamplerHelper_h
#define SamplerHelper_h
#include "AppFramework/AppParameter.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"
#include <string>
#include <list>
#include <map>

class SampleHelper : public AppAgent< list<string> >, public AppAgent<string>{
private:
    AppParameter<string> sample;
    AppParameter<string> sampleSection;
    list<string>         filesToProcess;
    map<string,list<string> > allFiles;

public:
    list<string> dataList(void) const {
        list<string> retval;
        retval.push_back("path");
        retval.push_back("identifier");
        return retval;
    }

    bool set  (const char *name, const list<string> &c){ return true; } // no one is allowed to request a change of any of the data members
    bool fetch(const char *name,       list<string> &c);

    bool set  (const char *name, const string &c){ return true; }  // no one is allowed to request a change of any of the data members
    bool fetch(const char *name,       string &c);

    AppResult beginRun(const AppEvent& ev){ return AppResult(); }
    AppResult beginJob(void);
    AppResult endJob  (void){ return AppResult(); }

    SampleHelper(void);// : AppAgentWrapper("sampleHelper","simple object-counter"),AppAgent< list<string> >(),AppAgent<string>(),
//         sample(*this,"sample", ""), sampleSection(*this,"sampleSection", "1of1"){}
};

#endif
