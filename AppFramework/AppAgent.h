#ifndef AppAgent_h
#define AppAgent_h
#include <list>
#include <string>
#include <unordered_map>

#include "AppResult.h"
#include "AppOStream.h"

class AppEvent;
class AppParameter_t;

class AppAgentWrapper {
private:
        const char *name_, *descr;
        unordered_map< string, AppParameter_t*, hash<string> >  hashOfParameters;

        friend class AppFramework;
        friend class AppParameter_t;

        void   addParameter   (AppParameter_t *par);
        void   removeParameter(AppParameter_t *par);
        string getParameter   (const char* parname=0) const; // reserved but is not used anywhere
        void   setParameter   (const char* parname,const char* value);

protected:
        AppOStream cout, clog, cerr;

public:
        const char* name       (void) const { return name_; }
        const char* description(void) const { return descr; }

        virtual list<string> dataList(void) const = 0;

        virtual AppResult beginRun(const AppEvent& ev) = 0;
        virtual AppResult beginJob(void) = 0;
        virtual AppResult endJob  (void) = 0;

        AppAgentWrapper(const char* nm, const char* desc):name_(nm),descr(desc){
            string sign("{"); sign.append(name_); sign.append("}: ");
            cout.sign(sign);          cerr.sign(sign);          clog.sign(sign);
            cout.redirect(std::cout); clog.redirect(std::clog); cerr.redirect(std::cerr);
        }
        virtual ~AppAgentWrapper(void){}
};

template<class T> class AppAgent: public virtual AppAgentWrapper {
private:
        AppAgent(const AppAgent<T>& data):AppAgentWrapper("copying is prohibited",""){}

public:
        virtual bool set  (const char *name, const T& obj) = 0;
        virtual bool fetch(const char *name,       T& obj) = 0;

        AppAgent(const char* nm=0, const char* desc=0):AppAgentWrapper(nm,desc){}
        virtual ~AppAgent(void){}
};

#endif
