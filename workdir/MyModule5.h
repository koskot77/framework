#ifndef MyModule5_h
#define MyModule5_h
#include "AppFramework/AppParameter.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"
#include "AnUtils/CompiledFormula.h"
#include <stdlib.h>

class MyModule5:public AppModule{
private:
        AppParameter<CompiledFormula> formula;

        AppResult beginJob(AppEvent& event){ cout<<"job begin"<<endl; return AppResult(); }
        AppResult beginRun(AppEvent& event){ cout<<"run begin"<<endl; return AppResult(); }
        AppResult endRun  (AppEvent& event){ cout<<"run ended"<<endl; return AppResult(); }
        AppResult endJob  (AppEvent& event){ cout<<"job ended"<<endl; return AppResult(); }

        AppResult event (AppEvent& event){
                CompiledFormula &f = formula;
                f["val1"] = rand()%10;
                f["val2"] = rand()%10;
                f["val3"] = rand()%10;
                cout<<"val1: "<<f["val1"]<<"  val2: "<<f["val2"]<<"  val3: "<<f["val3"]<<endl;
                cout<<f.formula()<<" = "<<f.eval()<<endl;
                return AppResult();
        }

public:
        MyModule5(const char *nm, const char *descr):AppModule(nm,descr),formula(*this,"formula"){}
        virtual ~MyModule5(){}
};

#endif
