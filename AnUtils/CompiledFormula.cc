using namespace std;
#include "CompiledFormula.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "RegEx.h"
///////////////////////////////////////////////////////////////////////////////////////
//
// On construction the provided string argument is compiled into a chain of actions.
// By the time of calling Eval every element of the formula has to be initialized.
// For example formula initialized with string "valie1+value2*3 || (value3-2)*value4!=5"
// can be Eval'ed once valie1, value2, value3 and value4 parameters are set.
//
///////////////////////////////////////////////////////////////////////////////////////

CompiledFormula& CompiledFormula::operator=(const string& formula) throw(CompileError) {
//  Create a chain of actions
        string arg(formula);
        formula_ = formula;
        fChain.clear();

//  Find finction calls
//        arg = RegExSubstitude("(\\w+ *\\(-?\\+?\\w+\\.?\\w*e?\\+?-?\\w*\\))", "($1)" , arg.c_str() );

//  Set up prioritys by sequention
        list<string> operators;
        operators.push_back("*");
        operators.push_back("/");
        operators.push_back("%");
        operators.push_back("+");
        operators.push_back("-");
        operators.push_back("<<");
        operators.push_back(">>");
        operators.push_back("<");
        operators.push_back("<=");
        operators.push_back(">");
        operators.push_back(">=");
        operators.push_back("==");
        operators.push_back("!=");
        operators.push_back("&");
        operators.push_back("^");
        operators.push_back("|");
        operators.push_back("&&");
        operators.push_back("||");

//  Use brackets in formula guiding priority
        list<string>::iterator op = operators.begin();
        while(op != operators.end()){
                size_t pos = 0;
                while( ( pos = arg.find(*op,pos) ) != string::npos ){
                        size_t index1 = pos, index2 = pos; int br_open=0;

                        // We must distinct operator&& from operator& !!!
                        if(op->length()==1){
                                if(ispunct(arg[pos+1]) && arg[pos+1]!='(' && arg[pos+1]!=')'){ pos+=2; continue; }
                        } else { index2++; }

                        // move backward
                        while( index1 ){
                                index1--;
                                if(!br_open && ispunct(arg[index1]) && arg[index1]!=')' && arg[index1]!='.' && arg[index1]!=',' && arg[index1]!='_'){ index1++; break; }
                                if(arg[index1] == ')')br_open++;
                                if(arg[index1] == '(')br_open--;
                        }
                        if(br_open) throw CompileError("bad brackets1");

                        // move forward
                        while( index2++ < arg.length()-1 ){
                                if(!br_open && ispunct(arg[index2]) && arg[index2]!='(' && arg[index2]!='.' && arg[index2]!=',' && arg[index2]!='_')break;
                                if(arg[index2] == '(')br_open++;
                                if(arg[index2] == ')')br_open--;
                        }
                        if(br_open) throw CompileError("bad brackets2");

                        arg.insert(index1,"(");
                        arg.insert(index2+1,")");
                        pos += 2;
                }
                op++;
        }

//  Serialize operations enclosed in brackets into the brackets' container
        list<string> brackets;
        size_t bra,ket,index=1;
        while( ( ket = arg.find(")") ) != string::npos ){
                bra = arg.rfind("(",ket);
                if(bra == string::npos) throw CompileError( string("bad brackets:").append(arg).c_str() );

                //Purge unnecessary brackets
                size_t pos = bra+1;
                while( isspace(arg[pos]) ) pos++;
                size_t start = pos;
                if( arg[pos] == '$' ){
                        while( isdigit(arg[++pos]) );
                        if( pos == ket ) {
                                arg.erase(bra,1);
                                arg.erase(ket-1,1);
                                continue;
                        } else pos = start;
                }
                if( isalpha(arg[pos]) ){
                        while( isalnum(arg[++pos]) );
                        if( pos == ket ) {
                                arg.erase(bra,1);
                                arg.erase(ket-1,1);
                                continue;
                        } else pos = start;
                }
                if( arg[pos] == '+' || arg[pos] == '-' ) {
                        while( isalnum(arg[++pos]) || isspace(arg[pos]) || arg[pos] == '.' );
                        if( pos == ket ) {
                                arg.erase(bra,1);
                                arg.erase(ket-1,1);
                                continue;
                        } else pos = start;
                }

                brackets.push_back(arg.substr(bra+1,ket-bra-1));
                char name[100]; sprintf(name,"$%ld",index); index++; // std ostrstream does'n work here :0(
                arg.replace(bra,ket-bra+1,name);
        }

//  Build the chain of operations using brackets container
        list<string>::const_iterator expression = brackets.begin();
        while(expression != brackets.end()){
                const char *match   = "(-?\\+?\\w+\\.?|-?\\+?\\w+?\\.\\w*|-?\\+?\\w+\\.?e\\+?-?\\w+|-?\\+?\\w+?\\.\\w*e\\+?-?\\w+|\\$\\w+)"  // left operand
                                                          " *(\\*|\\/|\\%|\\+|-|<<|>>|<|<=|>|>=|==|!=|&|\\^|\\||&&|\\|\\|) *"                                    // operator
                                                          "(-?\\+?\\w+\\.?|-?\\+?\\w+?\\.\\w*|-?\\+?\\w+\\.?e\\+?-?\\w+|-?\\+?\\w+?\\.\\w*e\\+?-?\\w+|\\$\\w+)"; // right operand
                map< string, vector<string> > result = RegExSearch(match, expression->c_str());

                if(result.size() != 1)
                        throw CompileError( string("Something wrong with ").append(*expression).c_str() );

                short act=0; const Value_t *val1=0, *val2=0;

                string operand1  = result.begin()->second[0];
                string operation = result.begin()->second[1];
                string operand2  = result.begin()->second[2];

                if(operation.length() == 2){
                        act = operation[0] + operation[1];
                } else {
                        act = operation[0];
                        if(act == '^')act++;  // we must differ '^' from '!' + '='
                        if(act == '|')act++;  // we must differ '|' from '>' + '>'
                }

                if(isalpha(operand1[0])){ // first operand is a variable
                        Value_t &val = fData[operand1];
                        val.setName(operand1);
                        val1 = &val;
                }
                if(isalpha(operand2[0])){ // second operand is a variable
                        Value_t &val = fData[operand2];
                        val.setName(operand2);
                        val2 = &val;
                }

                if(operand1[0] == '$'){   // first operand is already found earlier
                        list<Action_t>::iterator iter = fChain.begin();
                        size_t index = atoi(operand1.c_str()+1);
                        if(index == 0) throw CompileError( string("Can't interpret:").append(operand1).c_str() );
                        while(--index && iter != fChain.end())iter++;
                        if(iter == fChain.end()) throw CompileError( string("No such label:").append(operand1).c_str() );
                        val1 = iter->getResult();
                }
                if(operand2[0] == '$'){   // second operand is already found earlier
                        list<Action_t>::iterator iter = fChain.begin();
                        size_t index = atoi(operand2.c_str()+1);
                        if(index == 0) throw CompileError( string("Can't interpret:").append(operand2).c_str() );
                        while(--index && iter != fChain.end())iter++;
                        if(iter == fChain.end()) throw CompileError( string("No such label:").append(operand2).c_str() );
                        val2 = iter->getResult();
                }

                if(val1 && val2) fChain.push_back( Action_t(act,val1,val2) );

                if(!val1 && val2){    // try to interpret first operand like a value
                        if(operand1.find(".") == string::npos)
                                fChain.push_back( Action_t(act,atoi(operand1.c_str()),val2) );
                        else
                                fChain.push_back( Action_t(act,atof(operand1.c_str()),val2) );
                }
                if(val1 && !val2){    // try to interpret second operand like a value
                        if(operand2.find(".") == string::npos)
                                fChain.push_back( Action_t(act,val1,atoi(operand2.c_str())) );
                        else
                                fChain.push_back( Action_t(act,val1,atof(operand2.c_str())) );
                }
                if(!val1 && !val2){   // try to interpret both operands like a values
                        if(operand1.find(".") == string::npos && operand2.find(".") == string::npos)
                                fChain.push_back( Action_t(act,atoi(operand1.c_str()),atoi(operand2.c_str())) );
                        if(operand1.find(".") == string::npos && operand2.find(".") != string::npos)
                                fChain.push_back( Action_t(act,atoi(operand1.c_str()),atof(operand2.c_str())) );
                        if(operand1.find(".") != string::npos && operand2.find(".") == string::npos)
                                fChain.push_back( Action_t(act,atof(operand1.c_str()),atoi(operand2.c_str())) );
                        if(operand1.find(".") != string::npos && operand2.find(".") != string::npos)
                                fChain.push_back( Action_t(act,atof(operand1.c_str()),atof(operand2.c_str())) );
                }
#ifdef __DEBUG__
                cout<<*expression<<endl;
#endif
                expression++;
        }
        return *this;
}

CompiledFormula::CompiledFormula(const CompiledFormula &cf) throw() :formula_(cf.formula_),fData(cf.fData){
//  Coping this object requires accurate treatment of the list of actions
        list<Action_t>::const_iterator action = cf.fChain.begin();
        while(action != cf.fChain.end()){
                short          act    = action->getAction();
                const Value_t *arg1   = action->getArg1();
                const Value_t *arg2   = action->getArg2();
                short          clcode = action->getClCode();

                list<Action_t>::iterator       curr = --fChain.end();
                list<Action_t>::const_iterator prev = action;

                while(prev-- != cf.fChain.begin()){
                        if(arg1 == prev->getResult()) arg1 = curr->getResult();
                        if(arg2 == prev->getResult()) arg2 = curr->getResult();
                        curr--;
                }

                unordered_map<string,Value_t>::const_iterator data = cf.fData.begin();
                while( data != cf.fData.end() ){
                        if( arg1 == &(data->second) ) arg1 = &fData[data->first];
                        if( arg2 == &(data->second) ) arg2 = &fData[data->first];
                        data++;
                }

                switch(clcode){
                        case 0: fChain.push_back( Action_t(act, arg1, arg2) ); break;
                        case 1: fChain.push_back( Action_t(act,*arg1, arg2) ); break;
                        case 2: fChain.push_back( Action_t(act, arg1,*arg2) ); break;
                        case 3: fChain.push_back( Action_t(act,*arg1,*arg2) ); break;
                        default: cout<<"It will newer occur"<<endl; break;
                }
                action++;
        }
}

CompiledFormula& CompiledFormula::operator=(const CompiledFormula &cf) throw() {
//  Coping this object requires accurate treatment of the list of actions
        fChain.clear(); formula_ = cf.formula_; fData = cf.fData;
        list<Action_t>::const_iterator action = cf.fChain.begin();
        while(action != cf.fChain.end()){
                short          act  = action->getAction();
                const Value_t *arg1 = action->getArg1();
                const Value_t *arg2 = action->getArg2();
                short          clcode = action->getClCode();

                list<Action_t>::iterator       curr = --fChain.end();
                list<Action_t>::const_iterator prev = action;

                while(prev-- != cf.fChain.begin()){
                        if(arg1 == prev->getResult()) arg1 = curr->getResult();
                        if(arg2 == prev->getResult()) arg2 = curr->getResult();
                        curr--;
                }

                unordered_map<string,Value_t>::const_iterator data = cf.fData.begin();
                while( data != cf.fData.end() ){
                        if( arg1 == &(data->second) ) arg1 = &fData[data->first];
                        if( arg2 == &(data->second) ) arg2 = &fData[data->first];
                        data++;
                }

                switch(clcode){
                        case 0: fChain.push_back( Action_t(act, arg1, arg2) ); break;
                        case 1: fChain.push_back( Action_t(act,*arg1, arg2) ); break;
                        case 2: fChain.push_back( Action_t(act, arg1,*arg2) ); break;
                        case 3: fChain.push_back( Action_t(act,*arg1,*arg2) ); break;
                        default: cout<<"It will newer occur"<<endl; break;
                }
                action++;
        }
        return *this;
}

Value_t CompiledFormula::eval(void) throw (UsingUndefValue,DoubleInsteadInteger) {
        list<Action_t>::iterator iter = fChain.begin();
        while(iter != fChain.end()) iter++->eval();
        if(fChain.size()) return *((--iter)->getResult());
        else return Value_t("undef");
}

ostream& operator<<(ostream& ostr,const CompiledFormula &cf) throw() { ostr<<cf.formula(); return ostr; }
