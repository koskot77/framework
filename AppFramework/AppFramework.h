#ifndef AppFramework_h
#define AppFramework_h
#include <list>
#include <string>
#include <utility>
#include <unordered_map>

#include "AppEvent.h"
#include "AppModule.h"
#include "AppOStream.h"

class AppModule;
class AppInputModule;
class AppAgentWrapper;

class AppFramework{
private:
	unordered_map< string, AppAgentWrapper*, hash<string> > hashOfAgents;
	unordered_map< string, AppModule*,       hash<string> > hashOfModules;
	list   < pair<AppModule*,bool> >                  sequence;
	AppEvent   app_event;
	AppOStream cout, clog, cerr;
	void updateModulesAccess(void);
	int  addModule(AppInputModule *module);
	int  addModule(AppModule *module);
	int  addAgent (AppAgentWrapper *agent);
	void reorder(const char *arg);

public:
	void  modList (void);
	bool  verbose (const char *target, const char *action);
	const char* getParameter(const char *modparname) const ;
	bool  activate(const char *modname);
	bool  disable (const char *modname);
	bool  modify  (const char *modparname, const char *parvalue);
	long  process (long  number);
	void  beginJob(void);
	bool  beginRun(AppEvent &event);
	bool  endRun  (AppEvent &event);
	void  endJob  (void);

	AppFramework(const char* seq=0);
	~AppFramework(void);
};

#endif
