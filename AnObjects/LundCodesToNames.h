#ifndef LundCodesToNames_h
#define LundCodesToNames_h

#include <unordered_map>

class LundCodesToNames {
private:
        static unordered_map<int,const char*> names;
public:
        static const char* name(int code);
        LundCodesToNames(void);
};

#endif
