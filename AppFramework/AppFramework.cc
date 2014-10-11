using namespace std;
#include "AppFramework.h"
#include "AppAgent.h"
#include "AppModule.h"
#include "AppInputModule.h"
#include <list>
#include <iomanip>
#include <iostream>
#include <string.h>  // strlen

AppFramework::~AppFramework(void){
        unordered_map< string, AppModule* >::const_iterator iter = hashOfModules.begin();
        while( iter != hashOfModules.end() ){ delete iter->second; iter++; }
}

int AppFramework::addModule(AppInputModule *module){
        module->subscribe( this );
        return addModule( (AppModule*)module );
}

int AppFramework::addModule(AppModule *module){
        if( !sequence.size() ){ cout.redirect(std::cout); clog.redirect(std::clog); cerr.redirect(std::cerr); }
        pair<unordered_map< string, AppModule* >::iterator,bool> res =
                hashOfModules.insert( pair< string, AppModule* >(module->name(),module) );
        if( !res.second ){
                cerr<<"Wrning!!! only one module instance is allowed: <"<<module->name()<<">"<<endl;
                return sequence.size();
        }
        sequence.insert( sequence.end(), pair<AppModule*,bool>(module,true) );
        updateModulesAccess();
        return sequence.size();
}

int AppFramework::addAgent(AppAgentWrapper *agent){
        if( app_event.registerAgent(agent) ) return hashOfAgents.size();
        pair<unordered_map< string, AppAgentWrapper* >::iterator,bool> res =
                hashOfAgents.insert( pair< string, AppAgentWrapper* >(agent->name(),agent) );
        if( !res.second ){
                cerr<<"Wrning!!! only one module instance is allowed: {"<<agent->name()<<"}"<<endl;
                return hashOfAgents.size();
        }
        return hashOfAgents.size();
}

void AppFramework::modList(void){
        size_t width = 0;
        unordered_map< string, AppModule* >::const_iterator hashiter = hashOfModules.begin();
        while( hashiter != hashOfModules.end() ){
                size_t length = hashiter->first.length();
                if( width < length ) width = length;
                hashiter++;
        }
        cout.setf(ios::left,ios::adjustfield);
        list< pair<AppModule*,bool> >::const_iterator seqiter = sequence.begin();
        while( seqiter != sequence.end() ){
                string name = "<"; name += seqiter->first->name(); name += ">";
                if( seqiter->second )
                        cout<<"Module:  "<<setw(width+2)<<name.c_str()<<setw(0)<<"  [active]"<<endl;
                else
                        cout<<"Module:  "<<setw(width+2)<<name.c_str()<<setw(0)<<"  [disable]"<<endl;
                seqiter++;
        }
}

bool AppFramework::verbose(const char *target, const char *action){
        AppOStream *out=0, *log=0, *err=0;
        if( string(target) == "AppFramework" ){
                out = &cout;
                log = &clog;
                err = &cerr;
        } else {
                unordered_map< string, AppModule* >::const_iterator grep = hashOfModules.find(target);
                if( grep == hashOfModules.end() ){
                        cerr<<"Undefined: <"<<target<<">"<<endl;
                        return true;
                } else {
                        out = &(grep->second->cout);
                        log = &(grep->second->clog);
                        err = &(grep->second->cerr);
                        cout<<action<<" for <"<<target<<">"<<endl;
                }
        }

        string act(action);
        size_t pos = act.find("cout");
        if( pos != string::npos ){
                if( act.find("on", pos) != string::npos ) out->show();
                if( act.find("off",pos) != string::npos ) out->hide();
        }
        pos = act.find("clog");
        if( pos != string::npos ){
                if( act.find("on", pos) != string::npos ) log->show();
                if( act.find("off",pos) != string::npos ) log->hide();
        }
        pos = act.find("cerr");
        if( pos != string::npos ){
                if( act.find("on", pos) != string::npos ) err->show();
                if( act.find("off",pos) != string::npos ) err->hide();
        }
        return false;
}

const char* AppFramework::getParameter(const char *modparname) const {
        string argument(modparname);
        string modname; const char *parname; size_t pos;
        if( ( pos = argument.find("::") ) != string::npos ){
                modname = argument.substr(0,pos);
                parname = argument.c_str() + pos + 2;
        } else {
                modname = argument;
                parname = 0;
        }
        unordered_map<string,AppModule*>::const_iterator iter = hashOfModules.find(modname);
        if( iter == hashOfModules.end() ){
                const_cast<AppOStream&>(cerr)<<"Undefined module: <"<<modname<<">"<<endl;
                return "undef";
        }
        static string temporal;
        temporal = iter->second->getParameter(parname);
        return temporal.c_str();
}

bool AppFramework::activate(const char *modname){
        list< pair<AppModule*,bool> >::iterator seqiter = sequence.begin();
        while( seqiter != sequence.end() && strcmp(seqiter->first->name(),modname) ) seqiter++;
        if( seqiter == sequence.end() ){
                cerr<<"Undefined module: <"<<modname<<">"<<endl;
                return true;
        }
        if( seqiter->second ){
                cout<<"Already active: <"<<modname<<">"<<endl;
        } else {
                seqiter->second = true;
                updateModulesAccess();
                cout<<"Activated:        <"<<modname<<">"<<endl;
        }
        return false;
}

bool AppFramework::disable(const char *modname){
        list< pair<AppModule*,bool> >::iterator seqiter = sequence.begin();
        while( seqiter != sequence.end() && strcmp(seqiter->first->name(),modname) ) seqiter++;
        if( seqiter == sequence.end() ){
                cerr<<"Undefined module: <"<<modname<<">"<<endl;
                return true;
        }
        if( seqiter->second ) {
                cout<<"Disabled:        <"<<modname<<">"<<endl;
                seqiter->second = false;
                updateModulesAccess();
        } else cout<<"Already disabled: <"<<modname<<">"<<endl;
        return false;
}

void AppFramework::updateModulesAccess(void){
        list<AppModule*>  tmplist;
        list< pair<AppModule*,bool> >::iterator seqiter = sequence.begin();
        while( seqiter != sequence.end() ){
                if( seqiter->second ) {
                        seqiter->first->accessibleModules = tmplist;
                        tmplist.insert(tmplist.end(),seqiter->first);
                }
                seqiter++;
        }
}

void AppFramework::reorder(const char *arg){
        if( !arg || strlen(arg) == 0 ) return;
        string seq(arg);  size_t pos = 0, prev = 0;
        list < pair<AppModule*,bool> > newsequence;
        while(( pos = seq.find("->",pos+2) )){
                string modname = seq.substr(prev,pos-prev);
                unordered_map<string,AppModule*>::const_iterator iter = hashOfModules.find( modname );
                if( iter == hashOfModules.end() ) {
                        cerr<<"No such module: <"<<modname<<">"<<endl;
                        exit(1);
                        return;
                }
                newsequence.insert( newsequence.end(), pair<AppModule*,bool>(iter->second,true) );
                if( pos == string::npos ) break;
                prev = pos+2;
        }
        sequence = newsequence;
        updateModulesAccess();
}

bool AppFramework::modify(const char *modparname, const char *parvalue){
        string argument(modparname);
        string modname; const char *parname; size_t pos;
        if( ( pos = argument.find("::") ) != string::npos ){
                modname = argument.substr(0,pos);
                parname = argument.c_str() + pos + 2;
        } else return true;
        unordered_map< string, AppModule* >::const_iterator iter1 = hashOfModules.find(modname);
        if( iter1 != hashOfModules.end() ){
                iter1->second->setParameter(parname,parvalue);
                return false;
        }
        unordered_map< string, AppAgentWrapper* >::const_iterator iter2 = hashOfAgents.find(modname);
        if( iter2 != hashOfAgents.end() ){
                iter2->second->setParameter(parname,parvalue);
                return false;
        }
        cerr<<"Undefined name: "<<modname<<endl;
        return true;
}

long AppFramework::process(long nevent){
        if( nevent !=0 )
                clog<<"Process "<<nevent<<" event"<<(nevent>1?"s":"")<<endl;
        else
                clog<<"Process all events form run"<<endl;

        for(int curevent=0; nevent==0 || curevent<nevent; curevent++){
                app_event.clear();
                list< pair<AppModule*,bool> >::iterator seqiter = sequence.begin();
                while( seqiter != sequence.end() ){
                        if( seqiter->second ){
                                AppResult result = seqiter->first->event(app_event);
                                if( result.status & AppResult::LOG     ) clog<<"<"<<seqiter->first->name()<<">"<<"   LOG : "    <<result.message<<endl;
                                if( result.status & AppResult::WARNING ) cout<<"<"<<seqiter->first->name()<<">"<<"   WARNING : "<<result.message<<endl;
                                if( result.status & AppResult::ERROR   ) cerr<<"<"<<seqiter->first->name()<<">"<<"   ERROR : "  <<result.message<<endl;
                                if( result.status & AppResult::STOP    ) {
                                        cout<<"Stop at "<<curevent<<" event in module:  <"<<seqiter->first->name()<<">"<<endl;
                                        return curevent;
                                }
                                if( result.status & AppResult::SKIP    ) {
                                        clog<<"Skip at "<<curevent<<" event by module:  <"<<seqiter->first->name()<<">"<<endl;
                                        break;
                                }
                        }
                        seqiter++;
                }
        }
        return nevent;
}

void AppFramework::beginJob(void){
        unordered_map< string, AppAgentWrapper* >::const_iterator agent = hashOfAgents.begin();
        while( agent != hashOfAgents.end() ){
                if( agent->second ){
                        AppResult result = agent->second->beginJob();
                        if( result.status & AppResult::LOG     ) clog<<"{"<<agent->second->name()<<"}"<<" beginJob LOG : "    <<result.message<<endl;
                        if( result.status & AppResult::WARNING ) cout<<"{"<<agent->second->name()<<"}"<<" beginJob WARNING : "<<result.message<<endl;
                        if( result.status & AppResult::ERROR   ) cerr<<"{"<<agent->second->name()<<"}"<<" beginJob ERROR : "  <<result.message<<endl;
                }
                agent++;
        }
        list< pair<AppModule*,bool> >::const_iterator iter = sequence.begin();
        while( iter != sequence.end() ){
                if( iter->second ){
                        AppResult result = iter->first->beginJob(app_event);
                        if( result.status & AppResult::LOG     ) clog<<"<"<<iter->first->name()<<">"<<" beginJob LOG : "    <<result.message<<endl;
                        if( result.status & AppResult::WARNING ) cout<<"<"<<iter->first->name()<<">"<<" beginJob WARNING : "<<result.message<<endl;
                        if( result.status & AppResult::ERROR   ) cerr<<"<"<<iter->first->name()<<">"<<" beginJob ERROR : "  <<result.message<<endl;
                        if( result.status & AppResult::STOP    ) {
                                cout<<"Stop at beginJob in module:  <"<<iter->first->name()<<">"<<endl;
                                exit(0);
                        }
                }
                iter++;
        }
        app_event.clear();
}
bool AppFramework::beginRun(AppEvent &event){
        unordered_map< string, AppAgentWrapper* >::const_iterator agent = hashOfAgents.begin();
        while( agent != hashOfAgents.end() ){
                if( agent->second ){
                        AppResult result = agent->second->beginRun(event);
                        if( result.status & AppResult::LOG     ) clog<<"{"<<agent->second->name()<<"}"<<" beginRun LOG : "    <<result.message<<endl;
                        if( result.status & AppResult::WARNING ) cout<<"{"<<agent->second->name()<<"}"<<" beginRun WARNING : "<<result.message<<endl;
                        if( result.status & AppResult::ERROR   ) cerr<<"{"<<agent->second->name()<<"}"<<" beginRun ERROR : "  <<result.message<<endl;
                }
                agent++;
        }
        bool skipTheRun = false;
        list< pair<AppModule*,bool> >::const_iterator iter = sequence.begin();
        while( iter != sequence.end() ){
                if( iter->second ){
                        AppResult result = iter->first->beginRun(event);
                        if( result.status & AppResult::LOG     ) clog<<"<"<<iter->first->name()<<">"<<" beginRun LOG : "    <<result.message<<endl;
                        if( result.status & AppResult::WARNING ) cout<<"<"<<iter->first->name()<<">"<<" beginRun WARNING : "<<result.message<<endl;
                        if( result.status & AppResult::ERROR   ) cerr<<"<"<<iter->first->name()<<">"<<" beginRun ERROR : "  <<result.message<<endl;
                        if( result.status & AppResult::SKIP    ) {
                                cout<<"Skip at beginRun in module:  <"<<iter->first->name()<<">"<<endl;
                                skipTheRun = true;
                        }
                }
                iter++;
        }
        app_event.clear();
        return skipTheRun;
}
bool AppFramework::endRun(AppEvent &event){
        bool returnFromProcessing = false;
        list< pair<AppModule*,bool> > doubleLinked(sequence.begin(),sequence.end());
        list< pair<AppModule*,bool> >::reverse_iterator iter = doubleLinked.rbegin();
        while( iter != doubleLinked.rend() ){
                if( iter->second ) {
                        AppResult result = iter->first->endRun(event);
                        if( result.status & AppResult::LOG     ) clog<<"<"<<iter->first->name()<<">"<<" endRun LOG : "    <<result.message<<endl;
                        if( result.status & AppResult::WARNING ) cout<<"<"<<iter->first->name()<<">"<<" endRun WARNING : "<<result.message<<endl;
                        if( result.status & AppResult::ERROR   ) cerr<<"<"<<iter->first->name()<<">"<<" endRun ERROR : "  <<result.message<<endl;
                        if( result.status & AppResult::STOP    ) {
                                cout<<"Stop at endRun in module:  <"<<iter->first->name()<<">"<<endl;
                                returnFromProcessing = true;
                        }
                }
                iter++;
        }
        app_event.clear();
        return returnFromProcessing;
}
void AppFramework::endJob(void){
        list< pair<AppModule*,bool> > doubleLinked(sequence.begin(),sequence.end());
        list< pair<AppModule*,bool> >::reverse_iterator iter = doubleLinked.rbegin();
        while( iter != doubleLinked.rend() ){
                if( iter->second ){
                        AppResult result = iter->first->endJob(app_event);
                        if( result.status & AppResult::LOG     ) clog<<"<"<<iter->first->name()<<">"<<" endJob LOG : "    <<result.message<<endl;
                        if( result.status & AppResult::WARNING ) cout<<"<"<<iter->first->name()<<">"<<" endJob WARNING : "<<result.message<<endl;
                        if( result.status & AppResult::ERROR   ) cerr<<"<"<<iter->first->name()<<">"<<" endJob ERROR : "  <<result.message<<endl;
                }
                iter++;
        }
        unordered_map< string, AppAgentWrapper* >::const_iterator agent = hashOfAgents.begin();
        while( agent != hashOfAgents.end() ){
                if( agent->second ){
                        AppResult result = agent->second->endJob();
                        if( result.status & AppResult::LOG     ) clog<<"{"<<agent->second->name()<<"}"<<" endJob LOG : "    <<result.message<<endl;
                        if( result.status & AppResult::WARNING ) cout<<"{"<<agent->second->name()<<"}"<<" endJob WARNING : "<<result.message<<endl;
                        if( result.status & AppResult::ERROR   ) cerr<<"{"<<agent->second->name()<<"}"<<" endJob ERROR : "  <<result.message<<endl;
                }
                agent++;
        }
        app_event.clear();
}
