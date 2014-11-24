#ifndef Analyser_h
#define Analyser_h
#include "AppFramework/AppParameter.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"

class Analyser : public AppModule{
private:
        AppParameter<char>   mychar;
        AppParameter<int>    myint;
        AppParameter<double> mydouble;
        AppParameter<string> mystring;

        AppResult beginJob(AppEvent& event){ cout<<"job begin"<<endl; return AppResult(); }
        AppResult beginRun(AppEvent& event){ cout<<"run begin"<<endl; return AppResult(); }
        AppResult endRun  (AppEvent& event){ cout<<"run ended"<<endl; return AppResult(); }
        AppResult endJob  (AppEvent& event){ cout<<"job ended"<<endl; return AppResult(); }

        AppResult event (AppEvent& event);

public:
        string getMyString(void){ return mystring; }
        double getMyDouble(void){ return mydouble; }

        Analyser(const char *nm, const char *descr):AppModule(nm,descr),
                mychar  (*this,"mychar",'a'),      myint   (*this,"myint",0),
                mydouble(*this,"mydouble",123.321),mystring(*this,"mystring","QWE") {}
        virtual ~Analyser(void){}
};

#endif
