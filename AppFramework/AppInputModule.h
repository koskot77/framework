#ifndef AppInputModule_h
#define AppInputModule_h

#include "AppModule.h"
#include "AppResult.h"
#include "AppFramework.h"

class AppEvent;

class AppInputModule : public AppModule {
private:
        AppFramework  *framework;
        static bool inited;

protected:
        AppResult beginJob(AppEvent& event) = 0;
        AppResult beginRun(AppEvent& event) = 0;
        AppResult event (AppEvent& event)   = 0;
        AppResult endRun(AppEvent& event)   = 0;
        AppResult endJob(AppEvent& event)   = 0;

        friend class AppFramework;

        void subscribe     (AppFramework *listener){ framework = listener;  }
        bool beginRunNotify(AppEvent& event){ bool skip=false; if(framework && !inited){ skip|=framework->beginRun(event); inited = true; } return skip; }
        bool endRunNotify  (AppEvent& event){ bool stop=false; if(framework &&  inited){ stop|=framework->endRun  (event); inited = false;} return stop; }

        AppInputModule(const char *nm, const char *descr):AppModule(nm,descr),framework(0){}
        ~AppInputModule(void){}
};

#endif
