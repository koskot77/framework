#ifndef AppEvent_h
#define AppEvent_h
#include <list>
#include <string>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <typeinfo>

class AppDataWrapper {
private:
        string name_;

public:
        const char*    name(void) const { return name_.c_str(); }
        virtual bool   serializable(void) const = 0;

        virtual size_t write(ostream& buff) const = 0;
        virtual void   read (istream& data) = 0;

        virtual const char* type(void) const = 0;

        AppDataWrapper(const char *str):name_(str){}
        virtual ~AppDataWrapper(void){}
};

template<class T> class AppData {
protected:
        T obj;
        AppData(const T& data):obj(data){}

public:
        operator T& (void) { return obj; }
};

template<class T> class AppData1 : public AppDataWrapper, public AppData<T> {
private:
        typedef size_t (T::*Writer)(ostream&) const ;
        typedef void   (T::*Reader)(istream&);
        Writer writer;
        Reader reader;

public:
        bool serializable(void) const { return writer; }

        size_t write(ostream& buff) const { if(writer) return (AppData<T>::obj.*writer)(buff); return 0; }
        void   read (istream& data)       { if(reader) return (AppData<T>::obj.*reader)(data); return  ; }

        const char* type(void) const { return typeid(T).name(); }

        AppData1(const char *name, const T& data, Writer w=0, Reader r=0):AppDataWrapper(name),AppData<T>(data),writer(w),reader(r){}
};

template<class T> class AppData2 : public AppDataWrapper, public AppData<T> {
private:
        typedef size_t (*Writer)(const T&, ostream&);
        typedef void   (*Reader)(      T&, istream&);
        Writer writer;
        Reader reader;

public:
        bool serializable(void) const { return writer; }

        size_t write(ostream& buff) const { if(writer) return writer(AppData<T>::obj,buff); return 0; }
        void   read (istream& data)       { if(reader) return reader(AppData<T>::obj,data); return  ; }

        const char* type(void) const { return typeid(T).name(); }

        AppData2(const char *name, const T& data, Writer w=0, Reader r=0):AppDataWrapper(name),AppData<T>(data),writer(w),reader(r){}
};

template<> class AppData2<char>: public AppDataWrapper, public AppData<char> {
public:
        bool serializable(void) const { return true; }

        size_t write(ostream& buff) const { buff.write((const char*)&obj,sizeof(obj)); return sizeof(obj); }
        void   read (istream& data)       { data.read (      (char*)&obj,sizeof(obj)); }

        const char* type(void) const { return typeid(obj).name(); }

        AppData2(const char *name, const char& data):AppDataWrapper(name),AppData<char>(data){}
};

template<> class AppData2<unsigned char>: public AppDataWrapper, public AppData<unsigned char> {
public:
        bool serializable(void) const { return true; }

        size_t write(ostream& buff) const { buff.write((const char*)&obj,sizeof(obj)); return sizeof(obj); }
        void   read (istream& data)       { data.read (      (char*)&obj,sizeof(obj)); }

        const char* type(void) const { return typeid(obj).name(); }

        AppData2(const char *name, const unsigned char& data):AppDataWrapper(name),AppData<unsigned char>(data){}
};

template<> class AppData2<signed char>: public AppDataWrapper, public AppData<signed char> {
public:
        bool serializable(void) const { return true; }

        size_t write(ostream& buff) const { buff.write((const char*)&obj,sizeof(obj)); return sizeof(obj); }
        void   read (istream& data)       { data.read (      (char*)&obj,sizeof(obj)); }

        const char* type(void) const { return typeid(obj).name(); }

        AppData2(const char *name, const signed char& data):AppDataWrapper(name),AppData<signed char>(data){}
};

template<> class AppData2<int>: public AppDataWrapper, public AppData<int> {
public:
        bool serializable(void) const { return true; }

        size_t write(ostream& buff) const { buff.write((const char*)&obj,sizeof(obj)); return sizeof(obj); }
        void   read (istream& data)       { data.read (      (char*)&obj,sizeof(obj)); }

        const char* type(void) const { return typeid(obj).name(); }

        AppData2(const char *name, const int& data):AppDataWrapper(name),AppData<int>(data){}
};

template<> class AppData2<unsigned int>: public AppDataWrapper, public AppData<unsigned int> {
public:
        bool serializable(void) const { return true; }

        size_t write(ostream& buff) const { buff.write((const char*)&obj,sizeof(obj)); return sizeof(obj); }
        void   read (istream& data)       { data.read (      (char*)&obj,sizeof(obj)); }

        const char* type(void) const { return typeid(obj).name(); }

        AppData2(const char *name, const unsigned int& data):AppDataWrapper(name),AppData<unsigned int>(data){}
};

template<> class AppData2<long>: public AppDataWrapper, public AppData<long> {
public:
        bool serializable(void) const { return true; }

        size_t write(ostream& buff) const { buff.write((const char*)&obj,sizeof(obj)); return sizeof(obj); }
        void   read (istream& data)       { data.read (      (char*)&obj,sizeof(obj)); }

        const char* type(void) const { return typeid(obj).name(); }

        AppData2(const char *name, const long& data):AppDataWrapper(name),AppData<long>(data){}
};

template<> class AppData2<unsigned long>: public AppDataWrapper, public AppData<unsigned long> {
public:
        bool serializable(void) const { return true; }

        size_t write(ostream& buff) const { buff.write((const char*)&obj,sizeof(obj)); return sizeof(obj); }
        void   read (istream& data)       { data.read (      (char*)&obj,sizeof(obj)); }

        const char* type(void) const { return typeid(obj).name(); }

        AppData2(const char *name, const unsigned long& data):AppDataWrapper(name),AppData<unsigned long>(data){}
};

#if defined(__GNUC__)
template<> class AppData2<long long>: public AppDataWrapper, public AppData<long long> {
public:
        bool serializable(void) const { return true; }

        size_t write(ostream& buff) const { buff.write((const char*)&obj,sizeof(obj)); return sizeof(obj); }
        void   read (istream& data)       { data.read (      (char*)&obj,sizeof(obj)); }

        const char* type(void) const { return typeid(obj).name(); }

        AppData2(const char *name, const long long& data):AppDataWrapper(name),AppData<long long>(data){}
};

template<> class AppData2<unsigned long long>: public AppDataWrapper, public AppData<unsigned long long> {
public:
        bool serializable(void) const { return true; }

        size_t write(ostream& buff) const { buff.write((const char*)&obj,sizeof(obj)); return sizeof(obj); }
        void   read (istream& data)       { data.read (      (char*)&obj,sizeof(obj)); }

        const char* type(void) const { return typeid(obj).name(); }

        AppData2(const char *name, const unsigned long long& data):AppDataWrapper(name),AppData<unsigned long long>(data){}
};
#endif

template<> class AppData2<short>: public AppDataWrapper, public AppData<short> {
public:
        bool serializable(void) const { return true; }

        size_t write(ostream& buff) const { buff.write((const char*)&obj,sizeof(obj)); return sizeof(obj); }
        void   read (istream& data)       { data.read (      (char*)&obj,sizeof(obj)); }

        const char* type(void) const { return typeid(obj).name(); }

        AppData2(const char *name, const short& data):AppDataWrapper(name),AppData<short>(data){}
};

template<> class AppData2<unsigned short>: public AppDataWrapper, public AppData<unsigned short> {
public:
        bool serializable(void) const { return true; }

        size_t write(ostream& buff) const { buff.write((const char*)&obj,sizeof(obj)); return sizeof(obj); }
        void   read (istream& data)       { data.read (      (char*)&obj,sizeof(obj)); }

        const char* type(void) const { return typeid(obj).name(); }

        AppData2(const char *name, const unsigned short& data):AppDataWrapper(name),AppData<unsigned short>(data){}
};

#if _G_HAVE_BOOL
template<> class AppData2<bool>: public AppDataWrapper, public AppData<bool> {
public:
        bool serializable(void) const { return true; }

        size_t write(ostream& buff) const { buff.write((const char*)&obj,sizeof(obj)); return sizeof(obj); }
        void   read (istream& data)       { data.read (      (char*)&obj,sizeof(obj)); }

        const char* type(void) const { return typeid(obj).name(); }

        AppData2(const char *name, const bool& data):AppDataWrapper(name),AppData<bool>(data){}
};
#endif

template<> class AppData2<double>: public AppDataWrapper, public AppData<double> {
public:
        bool serializable(void) const { return true; }

        size_t write(ostream& buff) const { buff.write((const char*)&obj,sizeof(obj)); return sizeof(obj); }
        void   read (istream& data)       { data.read (      (char*)&obj,sizeof(obj)); }

        const char* type(void) const { return typeid(obj).name(); }

        AppData2(const char *name, const double& data):AppDataWrapper(name),AppData<double>(data){}
};

template<> class AppData2<float>: public AppDataWrapper, public AppData<float> {
public:
        bool serializable(void) const { return true; }

        size_t write(ostream& buff) const { buff.write((const char*)&obj,sizeof(obj)); return sizeof(obj); }
        void   read (istream& data)       { data.read (      (char*)&obj,sizeof(obj)); }

        const char* type(void) const { return typeid(obj).name(); }

        AppData2(const char *name, const float& data):AppDataWrapper(name),AppData<float>(data){}
};

//#if _G_HAVE_LONG_DOUBLE_IO //??????
template<> class AppData2<long double>: public AppDataWrapper, public AppData<long double> {
public:
        bool serializable(void) const { return true; }

        size_t write(ostream& buff) const { buff.write((const char*)&obj,sizeof(obj)); return sizeof(obj); }
        void   read (istream& data)       { data.read (      (char*)&obj,sizeof(obj)); }

        const char* type(void) const { return typeid(obj).name(); }

        AppData2(const char *name, const long double& data):AppDataWrapper(name),AppData<long double>(data){}
};
//#endif

class AppFramework;
//class AppAgentWrapper;
//template<class T> class AppAgent;
#include "AppAgent.h"

class AppEvent {
private:
        // access to any data, already have been stored in this event
        unordered_map< string, AppAgentWrapper*, hash<string> > persistent;
        unordered_map< string, AppDataWrapper*,  hash<string> > dictionary;
        list< AppDataWrapper* > storesequence;

        // readed object ( use for restore )
        unordered_map< string, pair< string, pair<char*,size_t> >, hash<string> > rawrecord;

        friend class AppFramework;   // Only this class allow to call 'clear()'
        // simple cleanup function
        void clear(void){ // all data will be lost if go to next event !
                unordered_map< string, AppDataWrapper* >::const_iterator iter = dictionary.begin();
                while( iter != dictionary.end() ) { delete iter->second; iter++; }
                dictionary.clear();
                storesequence.clear();
        }

        AppEvent(const AppEvent&){} // prevent as from copy ( because not implemented yet )

public:
        bool registerAgent(AppAgentWrapper* agent);

        template<class T> bool put(const char *name, const T& obj, size_t (T::*writer)(ostream&) const ){
                unordered_map< string, AppDataWrapper* >::const_iterator iter = dictionary.find(name);
                if( iter != dictionary.end() ) return true;
                AppDataWrapper *ptr = new AppData1<T>(name, obj, writer);
                dictionary[name] = ptr;
                storesequence.push_back(ptr);
                return false;
        }

        template<class T> bool put(const char *name, const T& obj, size_t (*writer)(const T&,ostream&)){
                unordered_map< string, AppDataWrapper* >::const_iterator iter = dictionary.find(name);
                if( iter != dictionary.end() ) return true;
                AppDataWrapper *ptr = new AppData2<T>(name, obj, writer);
                dictionary[name] = ptr;
                storesequence.push_back(ptr);
                return false;
        }

        template<class T> bool put(const char *name, const T& obj){
                unordered_map< string, AppAgentWrapper* >::const_iterator agent = persistent.find(name);
                if( agent != persistent.end() ){
                        AppAgent<T> *ptr = dynamic_cast< AppAgent<T>* >( agent->second );
                        if( !ptr ) return true;
                        return ptr->set(name,obj);
                }
                unordered_map< string, AppDataWrapper* >::const_iterator iter = dictionary.find(name);
                if( iter != dictionary.end() ) return true;
                AppDataWrapper *ptr = new AppData2<T>(name, obj);
                dictionary[name] = ptr;
                if( ptr->serializable() ) storesequence.push_back(ptr);
                return false;
        }

        template<class T> bool get(const char *name, T& obj, void (T::*reader)(istream&), size_t (T::*writer)(ostream&) const) const {
                unordered_map< string, AppDataWrapper* >::const_iterator iter_ = dictionary.find(name);
                if( iter_ != dictionary.end() ){
                        AppData<T> *ptr = dynamic_cast< AppData<T>* >( iter_->second );
                        if( !ptr ) return true;
                        obj = *ptr;
                        return false;
                }
//                unordered_map< string, pair< string, pair<char*,size_t> > >::const_iterator iter = rawrecord.find(name);
//                if( iter == rawrecord.end() ) return true;
//                if( iter->second.first != typeid(obj).name() ) return true;
//                AppData1<T> *ptr = new AppData1<T>(name, obj, writer, reader);
//#if (__GNUC__==2)
//                istrstream    istr(iter->second.second.first,iter->second.second.second);
//#else
//                istringstream istr(string(iter->second.second.first,iter->second.second.second));
//#endif
//                ptr->read(istr); dictionary[name] = ptr; obj = *ptr;
                ///if( writer ) storesequence.push_back(ptr);
///                return false;
                return true;
        }

        template<class T> bool get(const char *name, T& obj, void (*reader)(T&,istream&), size_t (*writer)(const T&,ostream&)) const {
                unordered_map< string, AppDataWrapper* >::const_iterator iter_ = dictionary.find(name);
                if( iter_ != dictionary.end() ){
                        AppData<T> *ptr = dynamic_cast< AppData<T>* >( iter_->second );
                        if( !ptr ) return true;
                        obj = *ptr;
                        return false;
                }
//                unordered_map< string, pair< string, pair<char*,size_t> > >::const_iterator iter = rawrecord.find(name);
//                if( iter == rawrecord.end() ) return true;
//                if( iter->second.first != typeid(obj).name() ) return true;
//                AppData2<T> *ptr = new AppData2<T>(name, obj, writer, reader);
//#if (__GNUC__==2)
//                istrstream    istr(iter->second.second.first,iter->second.second.second);
//#else
//                istringstream istr(string(iter->second.second.first,iter->second.second.second));
//#endif
//                ptr->read(istr); dictionary[name] = ptr; obj = *ptr;
                ///if( writer ) storesequence.push_back(ptr);
///                return false;
                return true;
        }

        template<class T> bool get(const char *name, T& obj) const {
                unordered_map< string, AppAgentWrapper* >::const_iterator agent = persistent.find(name);
                if( agent != persistent.end() ){
                        AppAgent<T> *ptr = dynamic_cast< AppAgent<T>* >( agent->second );
                        if( !ptr ) return true;
                        return ptr->fetch(name,obj);
                }
                unordered_map< string, AppDataWrapper* >::const_iterator iter_ = dictionary.find(name);
                if( iter_ != dictionary.end() ){
                        AppData<T> *ptr = dynamic_cast< AppData<T>* >( iter_->second );
                        if( !ptr ) return true;
                        obj = *ptr;
                        return false;
                }
//                unordered_map< string, pair< string, pair<char*,size_t> > >::const_iterator iter = rawrecord.find(name);
//                if( iter == rawrecord.end() ) return true;
//                if( iter->second.first != typeid(obj).name() ) return true;
//                AppData2<T> *ptr = new AppData2<T>(name, obj);
//#if (__GNUC__==2)
//                istrstream    istr(iter->second.second.first,iter->second.second.second);
//#else
//                istringstream istr(string(iter->second.second.first,iter->second.second.second));
//#endif
                //ptr->read(istr); dictionary[name] = ptr; obj = *ptr;
                ///storesequence.push_back(ptr);
///                return false;
                return true;
        }

        void print(ostream &ostr);     // Info about content
        void printRaw(ostream &ostr);  // Info about serialized content

        pair<const char*,size_t> getRaw(const char *name);

        void     save (string names=""); // Mark objects with names from list 'names' (separated by ":" delimeter) for serializing
        ostream& write(ostream& ostr);
        istream& read (istream& istr);

        AppEvent(void){}
        ~AppEvent(void){ clear(); }
};

#endif
