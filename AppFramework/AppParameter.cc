using namespace std;
#include "AppParameter.h"

ostream& operator<<(ostream& stream, const AppParameter_t& par){ return stream<<par.toString(); }
istream& operator>>(istream& stream, AppParameter_t& par){
       char buffer[65536]; stream>>buffer; par = buffer; return stream; }

