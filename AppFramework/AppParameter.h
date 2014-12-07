#ifndef AppParameter_h
#define AppParameter_h
#include <string>
#include <sstream>
#include <string.h>
#include "AppAgent.h"
#include "AppModule.h"

class AppParameter_t{
private:
        AppModule       *module;
        AppAgentWrapper *agent;
        const char      *parname;
public:
        virtual AppParameter_t& operator= (const char *str) = 0;
        virtual string toString(void) const = 0;

        const char* name(void){ return parname; }
        AppParameter_t(void):module(0),parname(0){}
        AppParameter_t(const AppParameter_t& par){ if( par.module && par.parname ){ module=par.module; parname=par.parname; } }
        AppParameter_t(AppModule&       mod, const char* nm):module(&mod),agent(0),parname(nm){ module->addParameter(this); }
        AppParameter_t(AppAgentWrapper& ag,  const char* nm):module(0),agent(&ag), parname(nm){  agent->addParameter(this); }
        virtual ~AppParameter_t(void){ if(module)module->removeParameter(this); if(agent)agent->removeParameter(this); }
};

template<class T> class AppParameter:public AppParameter_t{
public:
        typedef T value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;

private:
        value_type  value;

public:
        operator T& (void)   { return  value; }
        pointer operator& () { return &value; }
        AppParameter<T>& operator= (T val)          { value = val; return *this; }
        AppParameter<T>& operator= (const char *str){ value = str; return *this; }
        string toString(void) const {
#if (__GNUC__==2)
                strstream str; str<<value; string retval(str.str()); delete str.str(); return retval;
#else
                stringstream str; str<<value; return str.str();
#endif
        }

        AppParameter(void):AppParameter_t(),value( T() ){}
        AppParameter(const T& initval):AppParameter_t(),value(initval){}
        AppParameter(const AppParameter& par):AppParameter_t(par),value(par.value){}
        AppParameter(AppModule& mod, const char* nm):AppParameter_t(mod,nm),value( T() ){}
        AppParameter(AppModule& mod, const char* nm, const T& initval):AppParameter_t(mod,nm),value(initval){}
        AppParameter(AppAgentWrapper& ag, const char* nm):AppParameter_t(ag,nm),value( T() ){}
        AppParameter(AppAgentWrapper& ag, const char* nm, const T& initval):AppParameter_t(ag,nm),value(initval){}
        ~AppParameter(void){}
};

template<> class AppParameter<bool>:public AppParameter_t{
public:
        typedef bool value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;

private:
        value_type  value;

public:
        operator bool& (void) { return value;  }
        pointer operator& ()  { return &value; }

        AppParameter<bool>& operator= (bool val)        { value = val; return *this; }
        AppParameter<bool>& operator= (const char *str) { if(!strcmp(str,"true"))value = true; else value = (bool)atoi(str); return *this; }
        string toString(void) const {
#if (__GNUC__==2)
                char buf[10]; strstream str(buf,10); str<<value; return str.str();
#else
                stringstream str; str<<value; return str.str();
#endif
        }

        AppParameter(void):AppParameter_t(),value( bool() ){}
        AppParameter(const bool& initval):AppParameter_t(),value(initval){}
        AppParameter(const char *str):AppParameter_t(),value(  ){}
        AppParameter(const AppParameter& par):AppParameter_t(par),value(par.value){}
        AppParameter(AppModule& mod, const char* nm):AppParameter_t(mod,nm),value( bool() ){}
        AppParameter(AppModule& mod, const char* nm, const bool& initval):AppParameter_t(mod,nm),value(initval){}
        AppParameter(AppAgentWrapper& ag, const char* nm):AppParameter_t(ag,nm),value( bool() ){}
        AppParameter(AppAgentWrapper& ag, const char* nm, const bool& initval):AppParameter_t(ag,nm),value(initval){}
        ~AppParameter(void){}
};

template<> class AppParameter<char>:public AppParameter_t{
public:
        typedef char value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;

private:
        value_type  value;

public:
        operator char& (void) { return value;  }
        pointer operator& ()  { return &value; }

        AppParameter<char>& operator= (char val)        { value = val;    return *this; }
        AppParameter<char>& operator= (const char *str) { value = str[0]; return *this; }
        string toString(void) const {
#if (__GNUC__==2)
                char buf[10]; strstream str(buf,10); str<<value; return str.str();
#else
                stringstream str; str<<value; return str.str();
#endif
        }

        AppParameter(void):AppParameter_t(),value( char() ){}
        AppParameter(const char& initval):AppParameter_t(),value(initval){}
        AppParameter(const char *str):AppParameter_t(),value( str[0] ){}
        AppParameter(const AppParameter& par):AppParameter_t(par),value(par.value){}
        AppParameter(AppModule& mod, const char* nm):AppParameter_t(mod,nm),value( char() ){}
        AppParameter(AppModule& mod, const char* nm, const char& initval):AppParameter_t(mod,nm),value(initval){}
        AppParameter(AppAgentWrapper& ag, const char* nm):AppParameter_t(ag,nm),value( char() ){}
        AppParameter(AppAgentWrapper& ag, const char* nm, const char& initval):AppParameter_t(ag,nm),value(initval){}
        ~AppParameter(void){}
};

template<> class AppParameter<short>:public AppParameter_t{
public:
        typedef short value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;

private:
        value_type  value;

public:
        operator short& (void) { return value;  }
        pointer operator& ()   { return &value; }

        AppParameter<short>& operator= (short val)       { value = val;       return *this; }
        AppParameter<short>& operator= (const char *str) { value = atoi(str); return *this; }
        string toString(void) const {
#if (__GNUC__==2)
                char buf[100]; strstream str(buf,100); str<<value; return str.str();
#else
                stringstream str; str<<value; return str.str();
#endif
        }

        AppParameter(void):AppParameter_t(),value( short() ){}
        AppParameter(const short& initval):AppParameter_t(),value(initval){}
        AppParameter(const char *str):AppParameter_t(),value( atoi(str) ){}
        AppParameter(const AppParameter& par):AppParameter_t(par),value(par.value){}
        AppParameter(AppModule& mod, const char* nm):AppParameter_t(mod,nm),value( short() ){}
        AppParameter(AppModule& mod, const char* nm, const short& initval):AppParameter_t(mod,nm),value(initval){}
        AppParameter(AppAgentWrapper& ag, const char* nm):AppParameter_t(ag,nm),value( short() ){}
        AppParameter(AppAgentWrapper& ag, const char* nm, const short& initval):AppParameter_t(ag,nm),value(initval){}
        ~AppParameter(void){}
};

template<> class AppParameter<int>:public AppParameter_t{
public:
        typedef int value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;

private:
        value_type  value;

public:
        operator int& (void) { return value;  }
        pointer operator& () { return &value; }

        AppParameter<int>& operator= (int val)         { value = val;       return *this; }
        AppParameter<int>& operator= (const char *str) { value = atoi(str); return *this; }
        string toString(void) const {
#if (__GNUC__==2)
                char buf[100]; strstream str(buf,100); str<<value; return str.str();
#else
                stringstream str; str<<value; return str.str();
#endif
        }

        AppParameter(void):AppParameter_t(),value( int() ){}
        AppParameter(const int& initval):AppParameter_t(),value(initval){}
        AppParameter(const char *str):AppParameter_t(),value( atoi(str) ){}
        AppParameter(const AppParameter& par):AppParameter_t(par),value(par.value){}
        AppParameter(AppModule& mod, const char* nm):AppParameter_t(mod,nm),value( int() ){}
        AppParameter(AppModule& mod, const char* nm, const int& initval):AppParameter_t(mod,nm),value(initval){}
        AppParameter(AppAgentWrapper& ag, const char* nm):AppParameter_t(ag,nm),value( int() ){}
        AppParameter(AppAgentWrapper& ag, const char* nm, const int& initval):AppParameter_t(ag,nm),value(initval){}
        ~AppParameter(void){}
};

template<> class AppParameter<long>:public AppParameter_t{
public:
        typedef long value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;

private:
        value_type  value;

public:
        operator long& (void) { return value;  }
        pointer operator& ()  { return &value; }

        AppParameter<long>& operator= (long val)        { value = val;       return *this; }
        AppParameter<long>& operator= (const char *str) { value = atol(str); return *this; }
        string toString(void) const {
#if (__GNUC__==2)
                char buf[100]; strstream str(buf,100); str<<value; return str.str();
#else
                stringstream str; str<<value; return str.str();
#endif
        }

        AppParameter(void):AppParameter_t(),value( long() ){}
        AppParameter(const long& initval):AppParameter_t(),value(initval){}
        AppParameter(const char *str):AppParameter_t(),value( atol(str) ){}
        AppParameter(const AppParameter& par):AppParameter_t(par),value(par.value){}
        AppParameter(AppModule& mod, const char* nm):AppParameter_t(mod,nm),value( long() ){}
        AppParameter(AppModule& mod, const char* nm, const long& initval):AppParameter_t(mod,nm),value(initval){}
        AppParameter(AppAgentWrapper& ag, const char* nm):AppParameter_t(ag,nm),value( long() ){}
        AppParameter(AppAgentWrapper& ag, const char* nm, const long& initval):AppParameter_t(ag,nm),value(initval){}
        ~AppParameter(void){}
};

template<> class AppParameter<float>:public AppParameter_t{
public:
        typedef float value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;

private:
        value_type  value;

public:
        operator float& (void) { return value;  }
        pointer operator& ()   { return &value; }

        AppParameter<float>& operator= (float val)       { value = val;       return *this; }
        AppParameter<float>& operator= (const char *str) { value = atof(str); return *this; }
        string toString(void) const {
#if (__GNUC__==2)
                char buf[100]; strstream str(buf,100); str<<value; return str.str();
#else
                stringstream str; str<<value; return str.str();
#endif
        }

        AppParameter(void):AppParameter_t(),value( float() ){}
        AppParameter(const float& initval):AppParameter_t(),value(initval){}
        AppParameter(const char *str):AppParameter_t(),value( atof(str) ){}
        AppParameter(const AppParameter& par):AppParameter_t(par),value(par.value){}
        AppParameter(AppModule& mod, const char* nm):AppParameter_t(mod,nm),value( float() ){}
        AppParameter(AppModule& mod, const char* nm, const float& initval):AppParameter_t(mod,nm),value(initval){}
        AppParameter(AppAgentWrapper& ag, const char* nm):AppParameter_t(ag,nm),value( float() ){}
        AppParameter(AppAgentWrapper& ag, const char* nm, const float& initval):AppParameter_t(ag,nm),value(initval){}
        ~AppParameter(void){}
};

template<> class AppParameter<double>:public AppParameter_t{
public:
        typedef double value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;

private:
        value_type  value;

public:
        operator double& (void) { return value;  }
        pointer operator& ()    { return &value; }

        AppParameter<double>& operator= (double val)        { value = val;       return *this; }
        AppParameter<double>& operator= (const char *str)   { value = atof(str); return *this; }
        string toString(void) const {
#if (__GNUC__==2)
                char buf[100]; strstream str(buf,100); str<<value; return str.str();
#else
                stringstream str; str<<value; return str.str();
#endif
        }

        AppParameter(void):AppParameter_t(),value( double() ){}
        AppParameter(const double& initval):AppParameter_t(),value(initval){}
        AppParameter(const char *str):AppParameter_t(),value( atof(str) ){}
        AppParameter(const AppParameter& par):AppParameter_t(par),value(par.value){}
        AppParameter(AppModule& mod, const char* nm):AppParameter_t(mod,nm),value( double() ){}
        AppParameter(AppModule& mod, const char* nm, const double& initval):AppParameter_t(mod,nm),value(initval){}
        AppParameter(AppAgentWrapper& ag, const char* nm):AppParameter_t(ag,nm),value( double() ){}
        AppParameter(AppAgentWrapper& ag, const char* nm, const double& initval):AppParameter_t(ag,nm),value(initval){}
        ~AppParameter(void){}
};

template<> class AppParameter<string>:public AppParameter_t{
public:
        typedef string value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;

private:
        value_type  value;

public:
        operator string&     (void) { return value;         }
//        operator const char* (void) { return value.c_str(); }

        AppParameter<string>& operator= (const string &val){ value = val; return *this; }
        AppParameter<string>& operator= (const char *str)  { value = str; return *this; }
        string toString(void) const { return value; }

        char& operator[](int i) { return value[i]; }
        const char* c_str(void) const { return value.c_str(); }
        string::size_type length () const { return value.length(); }

        friend int operator==(const AppParameter<string> &x, const char *s)  { return x.value == s;       }
        friend int operator==(const AppParameter<string> &x, const string &y){ return x.value == y;       }
        friend int operator==(const AppParameter<string> &x, AppParameter<string> &s){ return x.value == s.value; }
        friend int operator!=(const AppParameter<string> &x, const char *s)  { return x.value != s;       }
        friend int operator!=(const AppParameter<string> &x, const string &y){ return x.value != y;       }
        friend int operator!=(const AppParameter<string> &x, AppParameter<string> &s){ return x.value != s.value; }

        AppParameter(void):AppParameter_t(),value( string() ){}
        AppParameter(const string& initval):AppParameter_t(),value(initval){}
        AppParameter(const char *str):AppParameter_t(),value( str ){}
        AppParameter(const AppParameter& par):AppParameter_t(par),value(par.value){}
        AppParameter(AppModule& mod, const char* nm):AppParameter_t(mod,nm),value( string() ){}
        AppParameter(AppModule& mod, const char* nm, const string& initval):AppParameter_t(mod,nm),value(initval){}
        AppParameter(AppAgentWrapper& ag, const char* nm):AppParameter_t(ag,nm),value( string() ){}
        AppParameter(AppAgentWrapper& ag, const char* nm, const string& initval):AppParameter_t(ag,nm),value(initval){}
        ~AppParameter(void){}
};

ostream& operator<<(ostream& stream, const AppParameter_t& par);
istream& operator>>(istream& stream, AppParameter_t& par);

#endif
