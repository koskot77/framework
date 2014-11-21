#ifndef MyModule2_h
#define MyModule2_h
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"
#include <iostream>

class MyModule2:public AppModule{
private:
        int counter;
public:
        struct MyType { int counter; MyType(){ counter=0; } } obj;

        class Counter : public AppAgent<int>, public AppAgent<long> {
        public:
                int count;
                list<string> dataList(void) const { list<string> retval; retval.push_back("counter3"); return retval; };
                bool set  (const char *name, const int &c){ std::cout<<"Setting  "<<name<<" to "<<c<<endl; count=c;   return false; }
                bool fetch(const char *name,       int &c){ std::cout<<"Fetching "<<name<<endl;            c=count++; return false; }

                bool set  (const char *name, const long &c){ std::cout<<"Setting  "<<name<<" to "<<c<<endl; count=c;   return false; }
                bool fetch(const char *name,       long &c){ std::cout<<"Fetching "<<name<<endl;            c=count++; return false; }

                AppResult beginRun(const AppEvent& ev){ return AppResult(); }
                AppResult beginJob(void){ return AppResult(); }
                AppResult endJob  (void){ return AppResult(); }

                Counter(void):AppAgentWrapper("counter3","simple object-counter"),AppAgent<int>(),AppAgent<long>(){ count=0; }
        } obj2;

private:
        AppResult beginJob(AppEvent& event){ event.registerAgent(&obj2); cout<<"job begin"<<endl; return AppResult(); }
        AppResult beginRun(AppEvent& event){ cout<<"run begin"<<endl; return AppResult(); }
        AppResult endRun  (AppEvent& event){ cout<<"run ended"<<endl; return AppResult(); }
        AppResult endJob  (AppEvent& event){ cout<<"job ended"<<endl; return AppResult(); }

        AppResult event(AppEvent& event){
                event.put("counter1",counter);
                event.put("counter2",(const MyType*)&obj);
                counter++; obj.counter--;
                return AppResult();
        }

public:
        MyModule2(const char *nm, const char *descr):AppModule(nm,descr){ counter=0; }
        virtual ~MyModule2(){}
};

#endif
