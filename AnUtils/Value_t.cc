using namespace std;
#include "Value_t.h"

ostream& operator<<(ostream& str, const Value_t &value) throw() {
        switch(value.type){
                case 'i': str<<value.like_int;    break;
                case 'd': str<<value.like_double; break;
                default : str<<"undef"<<endl;     break;
        }
        return str;
}
istream& operator>>(istream &str, Value_t &value) throw() {
        str>>value.like_double; value.like_int = int(value.like_double);
        if( value.like_int == value.like_double ) value.type = 'i'; else value.type = 'd';
        return str;
}
