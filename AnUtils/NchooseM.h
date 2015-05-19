#ifndef MchooseN_h
#define MchooseN_h
#include <vector>
#include <stdexcept>
#include "mybitset.h"

// The function below returns a list of all unique combinations of M objects T from the
// 'input'. If a column-separated pattern is provided with the 'match' argument, these
// objects are required to match pattern. For example, if T is a particle, then all
// possible combinations of oppositely charged muon pairs would have to be requested
// as "-13:+13" (PDG codes are used), or "13:13" for any di-muon pair, while "0:0"
// select all di-jets. The template type T should implement operator==(const char*) 

namespace NchooseM {

    template<class T>
    std::vector< std::vector<T> > combinations(const std::vector<T>& input, unsigned int M){
        std::vector< std::vector<T> > result;

        mybitset flagarray(input.size());
        flagarray.setNBits(M);

        do{
            std::vector<T> noname;
            flagarray.setCounter();
            typedef typename std::vector<T>::const_iterator VI;
            VI iter = input.begin();
            while(iter != input.end()){
                if(flagarray.nextBit())noname.push_back(*iter);
                iter++;
            }
            result.push_back(noname);
        } while(flagarray.moveForward());

        return result;
    }

    template<class T>
    std::vector< std::vector<T> > combinations(const std::vector<T>& input, const char *mask){

        std::vector< std::vector<T> > result;

        // Find number of requested objects in the mask
        char tmp_[strlen(mask) + 1], *name = tmp_; strcpy(name,mask);
        unsigned int nelements = 1;
        while(*++name)if(*name == ':'){ nelements++; *name = '\0'; }
        if(nelements > input.size())return result;
        name = tmp_;

//        // For fast comparison convert strings to hash numbers based in their PDG codes
//        int item_type[nelements], *item = item_type;
//        while(item_type + nelements > item){
//            *item++ = atoi(name);
//            name += strlen(name) + 1;
//        }

        // The algorithm: for each name/code element in the mask loop over the current choise of the
        // selected objects and look for the matching type. If no match is found - leave the loop.
        // If match is found, remove the element from next iterations and start over. Once every element
        // is removed store the choise of elements and move over to a new choise of elements (moveForward)
        mybitset flagarray(input.size());
        flagarray.setNBits(nelements);                   // Define the first subarray

        do{
            mybitset tmp = flagarray;                    // Auxiliary subarray
//            item = item_type; name = tmp_;               // Beginning of objects from the mask
//            while(item_type + nelements > item){         // Loop over the objects

            name = tmp_; int element = 0;             // Beginning of objects from the mask
            while(element < nelements){                // Loop over the objects
                typedef typename std::vector<T>::const_iterator VI;
                VI iter = input.begin(); 
                while(iter != input.end()){                 // Loop over the elements from the current choise
                    if(tmp.nextBit()){                      // Does element exists in the subarray
//                        if(*iter == *item){                 // Check for a match
                        if(*iter == name){                  // Check for a match
                            tmp.clearBit(tmp.getCounter()-1); tmp.setCounter(); break;
                        }
                    } iter++;
                }
//                if(iter == input.end()){ item = item_type + nelements; break; } // No such element
                if(iter == input.end()) break;  // No such element
                name += strlen(name) + 1;
            }

            if(tmp.getNumberOfBits() == 0){              // No spares left
                tmp = flagarray; tmp.setCounter();
                std::vector<T> noname;
                typedef typename std::vector<T>::const_iterator VI;
                VI iter = input.begin();
                while(iter != input.end()){             // Loop over the elements of the array
                    if(tmp.nextBit())noname.push_back(*iter);
                    iter++;
                }
                result.push_back(noname);
            }
        } while(flagarray.moveForward());
        return result;
    }

};

#endif

