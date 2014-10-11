///////////////////////////////////////////////////////////////////////////////////////
//
// Value_t - is an analog ScalarValue in perl, which does not defferentiate integral and floating types.
//
///////////////////////////////////////////////////////////////////////////////////////
#ifndef Value_t_h
#define Value_t_h
#include <string>
#include <iostream>
#include <stdexcept>

class UsingUndefValue:public std::runtime_error{
public:
        UsingUndefValue(const string& str):runtime_error(str){}
};

class DoubleInsteadInteger:public std::runtime_error{
public:
        DoubleInsteadInteger(const string& str):runtime_error(str){}
};

class Value_t{
private:
        string name;
        char   type;
        int    like_int;
        double like_double;

        friend ostream& operator<<(ostream &str, const Value_t &value) throw() ;
        friend istream& operator>>(istream &str, Value_t &value) throw() ;

public:

        Value_t& operator= (Value_t value){ like_int = value.like_int; like_double = value.like_double; type = value.type; return *this; }

        bool operator< (Value_t value) const throw (UsingUndefValue) {
                switch(type){
                        case 'i':
                                switch(value.type){
                                        case 'i': return like_int < value.like_int;    break;
                                        case 'd': return like_int < value.like_double; break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator<" );
                                } break;
                        case 'd':
                                switch(value.type){
                                        case 'i': return like_double < value.like_int;    break;
                                        case 'd': return like_double < value.like_double; break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator<" );
                                } break;
                        default : throw UsingUndefValue( "undefined value "+name+" in operator<" );
                }
        }

        bool operator<= (Value_t value) const throw (UsingUndefValue) {
                switch(type){
                        case 'i':
                                switch(value.type){
                                        case 'i': return like_int <= value.like_int;    break;
                                        case 'd': return like_int <= value.like_double; break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator<=" );
                                } break;
                        case 'd':
                                switch(value.type){
                                        case 'i': return like_double <= value.like_int;    break;
                                        case 'd': return like_double <= value.like_double; break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator<=" );
                                } break;
                        default : throw UsingUndefValue( "undefined value "+name+" in operator<=" );
                }
        }

        bool operator== (Value_t value) const throw (UsingUndefValue) {
                switch(type){
                        case 'i':
                                switch(value.type){
                                        case 'i': return like_int == value.like_int;    break;
                                        case 'd': return like_int == value.like_double; break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator==" );
                                } break;
                        case 'd':
                                switch(value.type){
                                        case 'i': return like_double == value.like_int;    break;
                                        case 'd': return like_double == value.like_double; break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator==" );
                                } break;
                        default : throw UsingUndefValue( "undefined value "+name+" in operator==" );
                }
        }

        bool operator!= (Value_t value) const throw (UsingUndefValue) {
                switch(type){
                        case 'i':
                                switch(value.type){
                                        case 'i': return like_int != value.like_int;    break;
                                        case 'd': return like_int != value.like_double; break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator!=" );
                                } break;
                        case 'd':
                                switch(value.type){
                                        case 'i': return like_double != value.like_int;    break;
                                        case 'd': return like_double != value.like_double; break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator!=" );
                                } break;
                        default : throw UsingUndefValue( "undefined value "+name+" in operator!=" );
                }
        }

        bool operator>= (Value_t value) const throw (UsingUndefValue) {
                switch(type){
                        case 'i':
                                switch(value.type){
                                        case 'i': return like_int >= value.like_int;    break;
                                        case 'd': return like_int >= value.like_double; break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator>=" );
                                } break;
                        case 'd':
                                switch(value.type){
                                        case 'i': return like_double >= value.like_int;    break;
                                        case 'd': return like_double >= value.like_double; break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator>=" );
                                } break;
                        default : throw UsingUndefValue( "undefined value "+name+" in operator>=" );
                }
        }

        bool operator> (Value_t value) const throw (UsingUndefValue) {
                switch(type){
                        case 'i':
                                switch(value.type){
                                        case 'i': return like_int > value.like_int;    break;
                                        case 'd': return like_int > value.like_double; break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator>" );
                                } break;
                        case 'd':
                                switch(value.type){
                                        case 'i': return like_double > value.like_int;    break;
                                        case 'd': return like_double > value.like_double; break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator>" );
                                } break;
                        default : throw UsingUndefValue( "undefined value "+name+" in operator>" );
                }
                return false;
        }

        bool operator&& (Value_t value) const throw (UsingUndefValue) {
                switch(type){
                        case 'i':
                                switch(value.type){
                                        case 'i': return like_int && value.like_int;    break;
                                        case 'd': return like_int && value.like_double; break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator&&" );
                                } break;
                        case 'd':
                                switch(value.type){
                                        case 'i': return like_double && value.like_int;    break;
                                        case 'd': return like_double && value.like_double; break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator&&" );
                                } break;
                        default : throw UsingUndefValue( "undefined value "+name+" in operator&&" );
                }
                return false;
        }

        bool operator|| (Value_t value) const throw (UsingUndefValue) {
                switch(type){
                        case 'i':
                                switch(value.type){
                                        case 'i': return like_int || value.like_int;    break;
                                        case 'd': return like_int || value.like_double; break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator||" );
                                } break;
                        case 'd':
                                switch(value.type){
                                        case 'i': return like_double || value.like_int;    break;
                                        case 'd': return like_double || value.like_double; break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator||" );
                                } break;
                        default : throw UsingUndefValue( "undefined value "+name+" in operator||" );
                }
                return false;
        }

        Value_t operator+ (Value_t value) const throw (UsingUndefValue) {
                switch(type){
                        case 'i':
                                switch(value.type){
                                        case 'i': return Value_t("( "+name+" + "+value.name+" )",like_int + value.like_int);    break;
                                        case 'd': return Value_t("( "+name+" + "+value.name+" )",like_int + value.like_double); break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator+" );
                                } break;
                        case 'd':
                                switch(value.type){
                                        case 'i': return Value_t("( "+name+" + "+value.name+" )",like_double + value.like_int);    break;
                                        case 'd': return Value_t("( "+name+" + "+value.name+" )",like_double + value.like_double); break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator+" );
                                } break;
                        default : throw UsingUndefValue( "undefined value "+name+" in operator+" );
                }
        }

        Value_t operator- (Value_t value) const throw (UsingUndefValue) {
                switch(type){
                        case 'i':
                                switch(value.type){
                                        case 'i': return Value_t("( "+name+" - "+value.name+" )",like_int - value.like_int);    break;
                                        case 'd': return Value_t("( "+name+" - "+value.name+" )",like_int - value.like_double); break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator-" );
                                } break;
                        case 'd':
                                switch(value.type){
                                        case 'i': return Value_t("( "+name+" - "+value.name+" )",like_double - value.like_int);    break;
                                        case 'd': return Value_t("( "+name+" - "+value.name+" )",like_double - value.like_double); break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator-" );
                                } break;
                        default : throw UsingUndefValue( "undefined value "+name+" in operator-" );
                }
        }

        Value_t operator* (Value_t value) const throw (UsingUndefValue) {
                switch(type){
                        case 'i':
                                switch(value.type){
                                        case 'i': return Value_t("( "+name+" * "+value.name+" )",like_int * value.like_int);    break;
                                        case 'd': return Value_t("( "+name+" * "+value.name+" )",like_int * value.like_double); break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator*" );
                                } break;
                        case 'd':
                                switch(value.type){
                                        case 'i': return Value_t("( "+name+" * "+value.name+" )",like_double * value.like_int);    break;
                                        case 'd': return Value_t("( "+name+" * "+value.name+" )",like_double * value.like_double); break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator*" );
                                } break;
                        default : throw UsingUndefValue( "undefined value "+name+" in operator*" );
                }
        }

        Value_t operator/ (Value_t value) const throw (UsingUndefValue) {
                switch(type){
                        case 'i':
                                switch(value.type){
                                        case 'i': return Value_t("( "+name+" / "+value.name+" )",like_int / value.like_int);    break;
                                        case 'd': return Value_t("( "+name+" / "+value.name+" )",like_int / value.like_double); break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator/" );
                                } break;
                        case 'd':
                                switch(value.type){
                                        case 'i': return Value_t("( "+name+" / "+value.name+" )",like_double / value.like_int);    break;
                                        case 'd': return Value_t("( "+name+" / "+value.name+" )",like_double / value.like_double); break;
                                        default : throw UsingUndefValue( "undefined value "+value.name+" in operator/" );
                                } break;
                        default : throw UsingUndefValue( "undefined value "+name+" in operator/" );
                }
        }

        Value_t operator% (Value_t value) const throw (DoubleInsteadInteger) {
                if( type != 'i' )       throw DoubleInsteadInteger( "using of double "+name+" instead integer in "+name+" % "+value.name );
                if( value.type != 'i' ) throw DoubleInsteadInteger( "using of double "+value.name+" instead integer in "+name+" % "+value.name );
                return Value_t("( "+name+" % "+value.name+" )",like_int % value.like_int);
        }

        Value_t operator<< (Value_t value) const throw (DoubleInsteadInteger) {
                if( type != 'i' )       throw DoubleInsteadInteger( "using of double "+name+" instead integer in "+name+" << "+value.name );
                if( value.type != 'i' ) throw DoubleInsteadInteger( "using of double "+value.name+" instead integer in "+name+" << "+value.name );
                return Value_t("( "+name+" << "+value.name+" )",like_int << value.like_int);
        }

        Value_t operator>> (Value_t value) const throw (DoubleInsteadInteger) {
                if( type != 'i' )       throw DoubleInsteadInteger( "using of double "+name+" instead integer in "+name+" >> "+value.name );
                if( value.type != 'i' ) throw DoubleInsteadInteger( "using of double "+value.name+" instead integer in "+name+" >> "+value.name );
                return Value_t("( "+name+" >> "+value.name+" )",like_int >> value.like_int);
        }

        Value_t operator& (Value_t value) const throw (DoubleInsteadInteger) {
                if( type != 'i' )       throw DoubleInsteadInteger( "using of double "+name+" instead integer in "+name+" & "+value.name );
                if( value.type != 'i' ) throw DoubleInsteadInteger( "using of double "+value.name+" instead integer in "+name+" & "+value.name );
                return Value_t("( "+name+" & "+value.name+" )",like_int & value.like_int);
        }

        Value_t operator^ (Value_t value) const throw (DoubleInsteadInteger) {
                if( type != 'i' )       throw DoubleInsteadInteger( "using of double "+name+" instead integer in "+name+" ^ "+value.name );
                if( value.type != 'i' ) throw DoubleInsteadInteger( "using of double "+value.name+" instead integer in "+name+" ^ "+value.name );
                return Value_t("( "+name+" ^ "+value.name+" )",like_int ^ value.like_int);
        }

        Value_t operator| (Value_t value) const throw (DoubleInsteadInteger) {
                if( type != 'i' )       throw DoubleInsteadInteger( "using of double "+name+" instead integer in "+name+" | "+value.name );
                if( value.type != 'i' ) throw DoubleInsteadInteger( "using of double "+value.name+" instead integer in "+name+" | "+value.name );
                return Value_t("( "+name+" | "+value.name+" )",like_int | value.like_int);
        }

        Value_t operator! (void) const throw (DoubleInsteadInteger) {
                if( type != 'i' ) throw DoubleInsteadInteger( "using of double "+name+" instead integer in !"+name );
                return Value_t("!"+name,!like_int);
        }

        void        setName  (const char *nm)   throw() { name = nm; }
        void        setName  (const string& nm) throw() { name = nm; }
        const char* getName  (void) const throw() { return name.c_str(); }

        char        getType  (void) const throw() { return type;        }
        int         getInt   (void) const throw() { return like_int;    }
        double      getDouble(void) const throw() { return like_double; }

        Value_t(const char   *nm         ) throw() :name(nm),type('\0'),like_int(int()),like_double(double()){}
        Value_t(const string& nm="noname") throw() :name(nm),type('\0'),like_int(int()),like_double(double()){}

        Value_t(bool           value) throw() :name("noname"),type('i'),like_int(int(value)),like_double(double(value)){}
        Value_t(char           value) throw() :name("noname"),type('i'),like_int(int(value)),like_double(double(value)){}
        Value_t(unsigned char  value) throw() :name("noname"),type('i'),like_int(int(value)),like_double(double(value)){}
        Value_t(short          value) throw() :name("noname"),type('i'),like_int(int(value)),like_double(double(value)){}
        Value_t(unsigned short value) throw() :name("noname"),type('i'),like_int(int(value)),like_double(double(value)){}
        Value_t(int            value) throw() :name("noname"),type('i'),like_int(int(value)),like_double(double(value)){}
        Value_t(unsigned int   value) throw() :name("noname"),type('i'),like_int(int(value)),like_double(double(value)){ cout<<"Warning convertion from unsigned int to int"<<endl; }
        Value_t(long           value) throw() :name("noname"),type('i'),like_int(int(value)),like_double(double(value)){ cout<<"Warning convertion from long to int"<<endl; }
        Value_t(unsigned long  value) throw() :name("noname"),type('i'),like_int(int(value)),like_double(double(value)){ cout<<"Warning convertion from unsigned long to int"<<endl; }
        Value_t(float          value) throw() :name("noname"),type('d'),like_int(int(value)),like_double(double(value)){}
        Value_t(double         value) throw() :name("noname"),type('d'),like_int(int(value)),like_double(double(value)){}
        Value_t(long double    value) throw() :name("noname"),type('d'),like_int(int(value)),like_double(double(value)){ cout<<"Warning convertion from long double to double"<<endl; }

        Value_t(const string& nm, bool           value) throw() :name(nm),type('i'),like_int(int(value)),like_double(double(value)){}
        Value_t(const string& nm, char           value) throw() :name(nm),type('i'),like_int(int(value)),like_double(double(value)){}
        Value_t(const string& nm, unsigned char  value) throw() :name(nm),type('i'),like_int(int(value)),like_double(double(value)){}
        Value_t(const string& nm, short          value) throw() :name(nm),type('i'),like_int(int(value)),like_double(double(value)){}
        Value_t(const string& nm, unsigned short value) throw() :name(nm),type('i'),like_int(int(value)),like_double(double(value)){}
        Value_t(const string& nm, int            value) throw() :name(nm),type('i'),like_int(int(value)),like_double(double(value)){}
        Value_t(const string& nm, unsigned int   value) throw() :name(nm),type('i'),like_int(int(value)),like_double(double(value)){ cout<<"Warning convertion from unsigned int to int"<<endl; }
        Value_t(const string& nm, long           value) throw() :name(nm),type('i'),like_int(int(value)),like_double(double(value)){ cout<<"Warning convertion from long to int"<<endl; }
        Value_t(const string& nm, unsigned long  value) throw() :name(nm),type('i'),like_int(int(value)),like_double(double(value)){ cout<<"Warning convertion from unsigned long to int"<<endl; }
        Value_t(const string& nm, float          value) throw() :name(nm),type('d'),like_int(int(value)),like_double(double(value)){}
        Value_t(const string& nm, double         value) throw() :name(nm),type('d'),like_int(int(value)),like_double(double(value)){}
        Value_t(const string& nm, long double    value) throw() :name(nm),type('d'),like_int(int(value)),like_double(double(value)){ cout<<"Warning convertion from long double to double"<<endl; }
};

#endif
