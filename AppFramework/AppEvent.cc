using namespace std;
#include "AppEvent.h"
#include "AppAgent.h"

pair<const char*,size_t> AppEvent::getRaw(const char *name){
        pair<const char*,size_t> result;
        unordered_map< string, pair< string, pair<char*,size_t> > >::const_iterator iter = rawrecord.find(name);
        if( iter != rawrecord.end() ){
                result.first  = iter->second.second.first;
                result.second = iter->second.second.second;
        }
        return result;
}

void AppEvent::print(ostream &ostr){  // Info about content
        unordered_map< string, AppDataWrapper* >::const_iterator iter = dictionary.begin();
        while( iter != dictionary.end() ){
                ostr<<iter->first<<endl;
                iter++;
        }
        unordered_map< string, AppAgentWrapper* >::const_iterator agent = persistent.begin();
        while( agent != persistent.end() ){
                ostr<<agent->first<<endl;
                agent++;
        }
}

void AppEvent::printRaw(ostream &ostr){  // Info about serialized content
        unordered_map< string, pair< string, pair<char*,size_t> > >::const_iterator iter = rawrecord.begin();
        while( iter != rawrecord.end() ){
                ostr<<iter->first<<" ["<<iter->second.first<<"] size: "<<iter->second.second.second<<endl;
                iter++;
        }
}

void AppEvent::save(string names){ // Mark objects with names from list 'names' (separated by ":" delameter) for storing
        storesequence.clear();
        if( !names.length() ) return;
        string::size_type pos = 0, prev = 0;
        while( true ){
                pos = names.find(":");
                string item = names.substr(prev,pos-prev);
                unordered_map< string, AppDataWrapper* >::iterator iter = dictionary.find(item);
                if( iter != dictionary.end() && iter->second->serializable() ) storesequence.push_back( iter->second );
                if( iter == dictionary.end() ) cerr<<item<<" not found"<<endl;
                if(!iter->second->serializable() ) cerr<<item<<" is transistent"<<endl;
                if( pos == string::npos ) break;
                prev = pos+1;
        }
}

ostream& AppEvent::write(ostream& ostr){
        list< AppDataWrapper* >::const_iterator iter = storesequence.begin();
        while( iter != storesequence.end() ){
                ostr<<(*iter)->name()<<'\n';
                ostr<<(*iter)->type()<<'\n';
#if (__GNUC__==2)
                strstream buffer;
#else
                stringstream buffer;
#endif
                size_t size = (*iter)->write(buffer);
                ostr.write((const char*)&size,sizeof(size));
                ostr<<buffer.rdbuf();
                iter++;
        }
        return ostr;
}

istream& AppEvent::read(istream& istr){
        unordered_map< string, pair<string, pair<char*,size_t> > >::const_iterator iter = rawrecord.begin();
        while( iter != rawrecord.end() ) delete [] (iter++)->second.second.first;
        rawrecord.clear();
        while( istr.peek() != EOF ){
                string name, type; size_t size;
                istr>>name>>type; istr.get();
                if( istr.peek() == EOF ) break;
                istr.read((char*)&size,sizeof(size));
                char *tmp = new char[size];
                istr.read(tmp,size);
                pair<char*,size_t> data(tmp,size);
                rawrecord[name] = pair<string, pair<char*,size_t> >(type,data);
        }
        return istr;
}

bool AppEvent::registerAgent(AppAgentWrapper* agent){
        list<string> dataList = agent->dataList();
        list<string>::const_iterator iter = dataList.begin();
        while( iter != dataList.end() ){
                unordered_map< string, AppAgentWrapper* >::const_iterator agent = persistent.find(*iter);
                if( agent != persistent.end() ) return true;
                iter++;
        }
        iter = dataList.begin();
        while( iter != dataList.end() ){
                if( !persistent.insert(pair<string,AppAgentWrapper*>(*iter,agent)).second ) return true;
                iter++;
        }
        return false;
}
