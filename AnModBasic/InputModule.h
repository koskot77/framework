#ifndef InputModule_h
#define InputModule_h
#include "AppFramework/AppParameter.h"
#include "AppFramework/AppInputModule.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"
#include <string>
#include <vector>

#include "TChain.h"

class InputModule : public AppInputModule {
private:
    AppParameter<string> path;               // a colon-separated list of wildcards; allows recursion via $path 
    AppParameter<long>   showProgressPeriod; // report processed event number
    AppParameter<long>   firstEntry;         // starting point

    AppResult beginJob(AppEvent& event);
    AppResult beginRun(AppEvent& event){ return AppResult(); }
    AppResult event   (AppEvent& event);
    AppResult endRun  (AppEvent& event){ return AppResult(); }
    AppResult endJob  (AppEvent& event){ return AppResult(); }

    void setParameter(const char* parname, const char* value);

    vector<string> inputFiles;
    TChain *chain;
    int chainEntryNumber;
//    int currentRunNumber;

public:

    InputModule(const char *nm, const char *descr) : AppInputModule(nm,descr),
          path(*this,"path", ""), showProgressPeriod(*this,"showProgressPeriod",1000),firstEntry(*this,"firstEntry",0){}
    virtual ~InputModule(void){}
};

#endif
