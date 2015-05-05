using namespace std;
#include <stdlib.h>
#include "MaskTypes.h"
#include "mybitset.h"

///////////////////////////////////////////////////////////////////////////////////////
//
//  Return a list of all unique combinations of particles from 'target' that match
//  the 'match' pattern. For example "gamma + e- + e+ + pi- + pi+" таков: would
//  have to be requested as "22:11:-11:-211:211" (PDG codes are used). While "+:-"
//  instructs combining any positively charged particles
//
///////////////////////////////////////////////////////////////////////////////////////
//        Some PDG Codes:
//        22 = Photon; 11 = Electron; -11 = Positron; 13 = Muon-; -13 = Muon+; 15 = Tau-; -15 = Tau+;
//        12 = Neutrino e; -12 = Anti Neutrino e; 14 = Neutrino Muon; -14 = Anti Neutrino Muon;  16 = Neutrino Tau; -16 = Anti Neutrino Tau;
//        111= Pion0;  211= Pion+; -211= Pion-;     221= Eta;
//        321= Kaon+; -321= Kaon-;  130= Kaon Long; 310= Kaon Short
//        TParticlePDG fTParticle(11);

list< list<Particle> > MaskTypes::FindCombination(const char *mask, const list<Particle>& target){
//  Avoiding CPU intensive nested loops is implemented as follows:
//         010011010000110 - a possible combination of 6 particles out of 15 total
//  This is combination is coded with a number 2^1 + 2^2 + 2^7 + 2^9 + 2^10 + 2^13
//  (or generally 2^x1 + 2^x2 + ... + 2^xM, where x1!=x2!=...!=xM particles are considered
//  So the problem of M nested loops boils down to finding combinations of all N-bits numbers
//  with M-bits set to 1. This is done with MoveForward() that costs only O(N*N) operations.

        list< list<Particle> > result;

// Find number of requested particles in the mask
        char tmp_[strlen(mask) + 1], *name = tmp_; strcpy(name,mask);
        unsigned int nelements = 1;
        while(*++name)if(*name == ':'){ nelements++; *name = '\0'; }
        if(nelements > target.size())return result;
        name = tmp_;

// For fast comparison convert strings to hash numbers based in their PDG codes
        int item_type[nelements], *item = item_type;
        while(item_type + nelements > item){
                *item++ = atoi(name);
                name += strlen(name) + 1;
        }

//  The algorithm: for each name/code element in the mask loop over the subarray of the particles
//  and look for the matching type. If no match is found - leave the function all together.
//  If match is found, remove the element from next iterations and start over. Once each element
//  are removed - the 'MoveForward' algorithm spawns a next subarray from the 'target' and repeats
        mybitset flagarray(target.size());
        flagarray.setNBits(nelements);                   // Define the first subarray
        do{
                mybitset tmp = flagarray;                    // Auxiliary subarray
                item = item_type; name = tmp_;               // Beginning of mask's hash
                while(item_type + nelements > item){         // Loop over the has elements

                        list<Particle>::const_iterator iter = target.begin();
                        while(iter != target.end()){                 // Loop over the elements of the array
                                if(tmp.nextBit()){                   // Does element exists in the subarray
                                        if(iter->type() == *item){   // Check if type matches
                                                tmp.clearBit(tmp.getCounter()-1); tmp.setCounter(); break;
                                        }
                                } iter++;
                        }
                        if(iter == target.end()){ item = item_type + nelements; break; } // No such element
                        name += strlen(name) + 1;  item++;
                }

                if(tmp.getNumberOfBits() == 0){              // No spares left
                        tmp = flagarray; tmp.setCounter();
                        list<Particle> noname;
                        list<Particle>::const_iterator iter = target.begin();
                        while(iter != target.end()){             // Loop over the elements of the array
                                if(tmp.nextBit())noname.push_back(*iter);
                                iter++;
                        }
                        result.push_back(noname);
                }
        } while(flagarray.moveForward());
        return result;
}
