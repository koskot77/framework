#ifndef CompiledFormula_h
#define CompiledFormula_h

#include <list>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include "Value_t.h"
#include "Action_t.h"

class CompileError:public std::runtime_error{
public:
        CompileError(const char *str):runtime_error(str){}
};

class CompiledFormula{
private:
        string formula_;
        list<Action_t>  fChain;     // The chain of actions, which must be performed in method Eval
        unordered_map< string, Value_t, hash<string> > fData;  // Container of data

public:
        Value_t eval(void) throw(UsingUndefValue,DoubleInsteadInteger);
        const char* formula() const throw() { return formula_.c_str(); }

        Value_t& operator[](const string& name) throw() { return fData[name]; }

        CompiledFormula& operator=(const string& formula) throw(CompileError);

        CompiledFormula& operator=(const CompiledFormula &cf) throw();
        CompiledFormula(const CompiledFormula &cf) throw();
        CompiledFormula(void) throw() {}
        ~CompiledFormula(void) throw() {}
};

ostream& operator<<(ostream& ostr,const CompiledFormula &cf) throw();
#endif
