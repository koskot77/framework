#ifndef MyModule3_h
#define MyModule3_h
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"

#include "MyModule2.h"

class MyModule3:public AppModule{
private:
        AppResult beginJob(AppEvent& event){ cout<<"job begin"<<endl; return AppResult(); }
        AppResult beginRun(AppEvent& event){ cout<<"run begin"<<endl; return AppResult(); }
        AppResult endRun  (AppEvent& event){ cout<<"run ended"<<endl; return AppResult(); }
        AppResult endJob  (AppEvent& event){ cout<<"job ended"<<endl; return AppResult(); }

        AppResult event   (AppEvent& event){
                 int counter=0, counter3=0;
                 const MyModule2::MyType *obj;
                 if( event.get("counter1",counter) ) return AppResult(AppResult::STOP|AppResult::ERROR,"Can't fetch counter1");
                 if( event.get("counter2",obj)     ) return AppResult(AppResult::STOP|AppResult::ERROR,"Can't fetch counter2");
                 if( event.get("counter3",counter3)) return AppResult(AppResult::STOP|AppResult::ERROR,"Can't fetch counter3");
                 cout<<"Fetched counter1: "<<counter<<endl;
                 cout<<"Fetched counter2: "<<obj->counter<<endl;
                 cout<<"Fetched counter3: "<<counter3<<endl;
                 if( event.put("counter3",(counter3+1)*10)) return AppResult(AppResult::STOP|AppResult::ERROR,"Can't set counter3");
                 return AppResult();
        }

public:
        MyModule3(const char *nm, const char *descr):AppModule(nm,descr){}
        virtual ~MyModule3(){}
};

#endif
