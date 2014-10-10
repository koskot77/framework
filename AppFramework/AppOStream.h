#ifndef AppOStream_h
#define AppOStream_h
#include <string>
#include <stdio.h>
#include <iostream>

class AppOStream : public ostream {
private:
        class buffer : public streambuf{};
        buffer     buff;
        streambuf *stream;
        streambuf *null;
        string     name;

public:
        void show(void){ rdbuf(stream); }
        void hide(void){ rdbuf(null);   }
        void sign(string nm)     { name=nm; }
        void sign(const char *nm){ name=nm; }

        void redirect(ostream &str){
                stream = str.rdbuf(); tie(&str);
                if( rdbuf() != null ) rdbuf(stream);
        }

        template<class T> ostream& operator<<(const T& val){
                return (*(ostream*)this)<<name<<val;
        }

        AppOStream(void):ostream(cout.rdbuf()),buff(),stream(cout.rdbuf()),null(&buff),name(""){}
        AppOStream(ostream &str):ostream(str.rdbuf()),buff(),stream(str.rdbuf()),null(&buff),name(""){}
/*        AppOStream(const char *name):ostream(cout.rdbuf()),buff(),stream(cout.rdbuf()),null(&buff){
                name = "<"; name.append(name); name.append(">: ");
        }
*/
};

#endif
