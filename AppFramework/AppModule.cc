using namespace std;
#include <iostream>
#include "AppModule.h"
#include "AppParameter.h"

void AppModule::addParameter(AppParameter_t *par){
        pair< unordered_map< string, AppParameter_t* >::iterator, bool > res =
                hashOfParameters.insert( pair< string, AppParameter_t* >(par->name(),par) );
        if( !res.second ) cerr<<"Parameter '"<<par->name()<<"' already added"<<endl;
}
void AppModule::removeParameter(AppParameter_t *par){
        int res = hashOfParameters.erase(par->name());
        if( !res ) cerr<<"Parameter '"<<par->name()<<"' not found"<<endl;
}

void AppModule::setParameter(const char* parname, const char* value){
        unordered_map< string, AppParameter_t* >::iterator iter = hashOfParameters.find(parname);
        if( iter != hashOfParameters.end() ){
                *(iter->second) = value;
                cout<<parname<<" is set to "<<*(iter->second)<<endl;
        } else
                cerr<<"Wrong name "<<parname<<endl;
}

string AppModule::getParameter(const char* parname) const {
        unordered_map< string, AppParameter_t* >::const_iterator iter;
        if( parname ){
                iter = hashOfParameters.find(parname);
                if( iter != hashOfParameters.end() )
                        return iter->second->toString();
                else {
                        const_cast<AppOStream&>(cerr)<<"Wrong name "<<parname<<endl;
                        return string();
                }
        } else {
                string par_list;
                iter = hashOfParameters.begin();
                while(iter != hashOfParameters.end()){
                        par_list.append( "  " );
                        par_list.append( iter->first );
                        par_list.append( " = " );
                        par_list.append( iter->second->toString() );
                        par_list.append( "\n" );
                        iter++;
                }
                return par_list;
        }
}
