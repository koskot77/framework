using namespace std;
#include "AppFramework.h"

extern "C" {
    AppFramework* AppFramework_new(const char *seq=0){
          return new AppFramework(seq);
    }
    void AppFramework_modList(AppFramework* fw){
          return fw->modList();
    }
    bool AppFramework_verbose(AppFramework* fw, const char *target, const char *action){
          return fw->verbose(target,action);
    }
    const char* AppFramework_getParameter(AppFramework* fw, const char *modparname){
          return fw->getParameter(modparname);
    }
    bool AppFramework_modify(AppFramework* fw, const char* modparname, const char *parvalue){
          return fw->modify(modparname,parvalue);
    }
    long AppFramework_process(AppFramework* fw, long number){
          return fw->process(number);
    }
    void AppFramework_beginJob(AppFramework* fw){
          return fw->beginJob();
    }
    void AppFramework_endJob(AppFramework* fw){
          return fw->endJob();
    }
}
