#ifndef AppResult_h
#define AppResult_h
#include <string>

class AppResult {
public:
    enum { OK=1, SKIP=2, STOP=4, LOG=8, WARNING=16, ERROR=32 };
        int    status;
        string message;
        AppResult(void):status(OK){}
        AppResult(int stat):status(stat){}
        AppResult(int stat,const string& msg):status(stat),message(msg){}
};

#endif
