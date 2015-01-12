#ifndef MyModule1_h
#define MyModule1_h
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"

class MyModule1:public AppModule{
private:
        AppResult beginJob(AppEvent& event){ cout<<"job begin"<<endl; return AppResult(); }
        AppResult beginRun(AppEvent& event){ cout<<"run begin"<<endl; return AppResult(); }
        AppResult endRun  (AppEvent& event){ cout<<"run ended"<<endl; return AppResult(); }
        AppResult endJob  (AppEvent& event){ cout<<"job ended"<<endl; return AppResult(); }

                AppResult event   (AppEvent& event){ return AppResult(); }

public:
        MyModule1(const char *nm, const char *descr):AppModule(nm,descr){}
        virtual ~MyModule1(){}
};

#endif
