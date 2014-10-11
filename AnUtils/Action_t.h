///////////////////////////////////////////////////////////////////////////////////////
//
// Auxilary class defining an operation to be performed over 'arg1' and 'arg2'
//
///////////////////////////////////////////////////////////////////////////////////////
#ifndef Action_t_h
#define Action_t_h
#include <stdexcept>
#include "Value_t.h"

class Action_t{
private:
        short          action;        // The code of operation
        const Value_t *arg1;          // First argument
        const Value_t *arg2;          // Second argument
        Value_t       *result;        // Return value
        short          cleanup_code;  // Delete helper

public:
        void eval(void) throw (UsingUndefValue,DoubleInsteadInteger) {
//                if(action=='!'){ *result = (arg1!=0); return; }
                if(!arg1 || !arg2)return;
                switch(action){
                        case '*'     : *result = *arg1  * *arg2; break;
                        case '/'     : *result = *arg1  / *arg2; break;
                        case '%'     : *result = *arg1  % *arg2; break;
                        case '+'     : *result = *arg1  + *arg2; break;
                        case '-'     : *result = *arg1  - *arg2; break;
                        case '<'+'<' : *result = *arg1 << *arg2; break;
                        case '>'+'>' : *result = *arg1 >> *arg2; break;
                        case '<'     : *result = *arg1  < *arg2; break;
                        case '<'+'=' : *result = *arg1 <= *arg2; break;
                        case '>'     : *result = *arg1  > *arg2; break;
                        case '>'+'=' : *result = *arg1 >= *arg2; break;
                        case '='+'=' : *result = *arg1 == *arg2; break;
                        case '!'+'=' : *result = *arg1 != *arg2; break;
                        case '&'     : *result = *arg1  & *arg2; break;
                        case '^'+1   : *result = *arg1  ^ *arg2; break;
                        case '|'+1   : *result = *arg1  | *arg2; break;
                        case '&'+'&' : *result = *arg1 && *arg2; break;
                        case '|'+'|' : *result = *arg1 || *arg2; break;
                        default      : cout<<"Undefined operator!"<<endl;
                }
        }

        short          getAction(void) const throw() { return action; }
        const Value_t* getArg1  (void) const throw() { return arg1;   }
        const Value_t* getArg2  (void) const throw() { return arg2;   }
        const Value_t* getResult(void) const throw() { return result; }
        short          getClCode(void) const throw() { return cleanup_code; }

        Action_t(short act, Value_t val1, Value_t val2) throw() :action(act){ arg1 = new Value_t(val1); arg2 = new Value_t(val2); result = new Value_t(); cleanup_code = 3; }

        Action_t(short act, Value_t val1, const Value_t *val2) throw() :action(act){ arg1 = new Value_t(val1); arg2 = val2; result = new Value_t(); cleanup_code = 1; }
        Action_t(short act, const Value_t *val1, Value_t val2) throw() :action(act){ arg1 = val1; arg2 = new Value_t(val2); result = new Value_t(); cleanup_code = 2; }

        Action_t(short act, const Value_t *val1, const Value_t *val2=0) throw() :action(act),
                arg1(val1),arg2(val2),result(new Value_t()),cleanup_code(0){}

        Action_t& operator=(const Action_t &act) throw() {
                switch(cleanup_code){
                        case 0: delete result; break;
                        case 1: delete result; delete arg1; break;
                        case 2: delete result; delete arg2; break;
                        case 3: delete result; delete arg1; delete arg2; break;
                        default: break;
                }
                action = act.action;
                cleanup_code = act.cleanup_code;
                switch(act.cleanup_code){
                        case 0: result = new Value_t(*act.result); arg1 = act.arg1; arg2 = act.arg2; break;
                        case 1: result = new Value_t(*act.result); arg1 = new Value_t(*act.arg1); arg2 = act.arg2; break;
                        case 2: result = new Value_t(*act.result); arg2 = new Value_t(*act.arg2); arg1 = act.arg1; break;
                        case 3: result = new Value_t(*act.result); arg1 = new Value_t(*act.arg1); arg2 = new Value_t(*act.arg2); break;
                        default: break;
                }
                return *this;
        }
        Action_t(const Action_t &act) throw() :action(act.action),arg1(act.arg1),arg2(act.arg2),result(0),cleanup_code(act.cleanup_code){
                switch(act.cleanup_code){
                        case 0: result = new Value_t(*act.result); break;
                        case 1: result = new Value_t(*act.result); arg1 = new Value_t(*act.arg1); break;
                        case 2: result = new Value_t(*act.result); arg2 = new Value_t(*act.arg2); break;
                        case 3: result = new Value_t(*act.result); arg1 = new Value_t(*act.arg1); arg2 = new Value_t(*act.arg2); break;
                        default: break;
                }
        }
        ~Action_t(void) throw() {
                switch(cleanup_code){
                        case 0: delete result; break;
                        case 1: delete result; delete arg1; break;
                        case 2: delete result; delete arg2; break;
                        case 3: delete result; delete arg1; delete arg2; break;
                        default: break;
                }
        }
};

#endif
