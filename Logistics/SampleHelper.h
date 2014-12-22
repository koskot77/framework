#ifndef SampleHelper_h
#define SampleHelper_h
#include "AppFramework/AppParameter.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"
#include <string>
#include <vector>
#include <map>

class SampleHelper : public AppAgent< vector<string> >, public AppAgent<string>{
private:
    AppParameter<string> sample;
    AppParameter<string> sampleSection;
    vector<string>       filesToProcess;
    map<string,list<string> > allFiles;

public:
    list<string> dataList(void) const {
        list<string> retval;
        retval.push_back("path");
        retval.push_back("identifier");
        return retval;
    }

    bool set  (const char *name, const vector<string> &c){ return true; } // no one is allowed to request a change of any of the data members
    bool fetch(const char *name,       vector<string> &c);

    bool set  (const char *name, const string &c){ return true; }  // no one is allowed to request a change of any of the data members
    bool fetch(const char *name,       string &c);

    AppResult beginRun(const AppEvent& ev){ return AppResult(); }
    AppResult beginJob(void);
    AppResult endJob  (void){ return AppResult(); }

    SampleHelper(const char* name, const char *descr);// : AppAgentWrapper("sampleHelper","simple object-counter"),AppAgent< list<string> >(),AppAgent<string>(),
//         sample(*this,"sample", ""), sampleSection(*this,"sampleSection", "1of1"){}
};

#endif
