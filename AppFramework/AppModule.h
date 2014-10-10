#ifndef AppModule_h
#define AppModule_h
#include <string>
#include <list>
#include <unordered_map>

#include "AppResult.h"
#include "AppOStream.h"

class AppEvent;
class AppFramework;
class AppParameter_t;

class AppModule{
private:
        const char *name_;
        const char *description_;
        unordered_map< string, AppParameter_t*, hash<string> >  hashOfParameters;
        list< AppModule* >                                      accessibleModules;

        friend class AppParameter_t;
        void   addParameter   (AppParameter_t *par);
        void   removeParameter(AppParameter_t *par);

        friend class AppFramework;
        string getParameter   (const char* parname=0) const;
        void   setParameter   (const char* parname,const char* value);

protected:
        AppOStream cout, clog, cerr;

        const char* name        (void) const { return name_;        }
        const char* description (void) const { return description_; }

        virtual AppResult beginJob(AppEvent& event) = 0;
        virtual AppResult beginRun(AppEvent& event) = 0;
        virtual AppResult event (AppEvent& event) = 0;
        virtual AppResult endRun(AppEvent& event) = 0;
        virtual AppResult endJob(AppEvent& event) = 0;

        template<class T> T* moduleGet(short pos=0){
                list<AppModule*>::const_iterator iter = accessibleModules.begin();
                while( iter != accessibleModules.end() ){
                        if(dynamic_cast<T*>(*iter))return (T*)(*iter); iter++;
                }
                return 0;
        }

        AppModule(const char *nm, const char *descr):name_(nm),description_(descr){
                string sign("<"); sign.append(name_); sign.append(">: ");
                cout.sign(sign);          cerr.sign(sign);          clog.sign(sign);
                cout.redirect(std::cout); clog.redirect(std::clog); cerr.redirect(std::cerr);
        }
        virtual ~AppModule(){}
};

#endif
