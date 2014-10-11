using namespace std;
#include <stdlib.h>
#include "MaskTypes.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>

class mybitset{
private:
        unsigned char *array;
        int   counter;
        int   size;

public:
        void SetSize(int size_){ if(array)delete array; size=size_; array = new unsigned char[size]; }
        int  GetSize(void){ return size; }

        bool SetBit(int bit,bool value){ if(bit >= size)return false; array[bit] = value; return true; }
        bool SetBit(int bit){ return SetBit(bit,true); }
        bool ClearBit(int bit){ return SetBit(bit,false); }
        void ClearAll(void){ for(int i=0; i<size; i++)array[i] = 0; }
        bool GetBit(int bit){ if(bit >= size)return false; return array[bit]; }
        void Inverse(void){ for(int i=0; i<size; i++)array[i] =! array[i]; }

        int  GetLast(int n_bits=-1){ int last = ((n_bits > 0) ? (n_bits) : (size)); while(--last>=0 && !array[last]); return last; }
        int  GetNumberOfBits(int n_bits=-1){ int count=0, last = ((n_bits > 0) ? (n_bits) : (size)); for(int i=0; i<last; i++)if(GetBit(i))count++; return count; }
        string GetBinary(void){ char tmp[size+1]; for(int i=0; i<size; i++)if(GetBit(i))tmp[i] = '1'; else tmp[i] = '0'; tmp[size] = '\0'; string tmp_ = tmp; return tmp_; }

        int  Next(int counter_ = -1){
                if(counter_ >= size)return -1;
                if(counter_ >= 0) counter = counter_;
                return GetBit(counter++);
        }
        bool GetBit(void){ return GetBit(counter); }
        int  GetCounter(void){ return counter; }
        void SetCounter(int counter_ = 0){ counter = counter_; }

        bool SetNBits(int n_bits);
        bool MoveForvard(int n_bits=-1, int number=-1);

        operator int(void);
        mybitset&        operator++(void);
        mybitset        operator++(int);
        mybitset&        operator=(const mybitset& ex);

        mybitset(int size_=sizeof(long)*8);
        mybitset(const mybitset& ex);
        ~mybitset(void);
};

mybitset::mybitset(int size_):size(size_){ array = new unsigned char[size_]; counter = 0; }
mybitset::~mybitset(void){ if(array)delete array; }
mybitset::mybitset(const mybitset& bs){
        size = bs.size; array  = new unsigned char[size];
        for(int i=0; i<size; i++)array[i] = bs.array[i];
        counter = bs.counter;
}

bool mybitset::SetNBits(int n_bits){
        if(n_bits>size)return false;
        for(int i=0; i<size; i++)
                if(i<n_bits)array[i]=true; else array[i]=false;
        return true;
}

bool mybitset::MoveForvard(int n_bits,int number){
        if(n_bits == 1)return false;
        n_bits = ((n_bits > 0) ? (n_bits) : (size));
        int last = GetLast(n_bits);
        number = ((number > 0) ? (number) : (GetNumberOfBits(n_bits)));
        if(!MoveForvard(n_bits-1,number-array[n_bits-1])){
                if(last >= 0 && last < n_bits-1){
                        array[last] = false; array[last+1] = true;
                        for(int i=0; i<=last; i++)
                                if(i<number-1)array[i]=true; else array[i]=false;
                        return true;
                } else return false;
        } return true;
}

mybitset::operator int(void){
        if((size_t)size > sizeof(int)*8) return 0;
        int tmp = 0; int count = 0;
        while(size>count++)if(array[count])tmp += (1<<count);
        return tmp;
}

mybitset& mybitset::operator=(const mybitset& bs){
        if(size != bs.size){
                if(array)delete array; size = bs.size;
                array  = new unsigned char[size];
        }
        for(int i=0; i<size; i++)array[i] = bs.array[i];
        counter = bs.counter;
        return *this;
}

mybitset& mybitset::operator++(void){
        int last = 0;
        while(array[last] && (last<size))array[last++] = 0;
        if(last<size)array[last] = true; else {
                if(array)delete array; size += 10;
                array = new unsigned char[size+10];
                array[last] = 1;
        }
        return *this;
}

mybitset mybitset::operator++(int){
        int last = 0;
        while(array[last] && (last<size))array[last++] = 0;
        if(last<size)array[last] = true; else {
                if(array)delete array; size += 10;
                array = new unsigned char[size+10];
                array[last] = 1;
        }
        return *this;
}

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
//  with M-bits set to 1. This is done with MoveForvard() that costs only O(N*N) operations.

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
        flagarray.SetNBits(nelements);                   // Define the first subarray
        do{
                mybitset tmp = flagarray;                    // Auxiliary subarray
                item = item_type; name = tmp_;               // Beginning of mask's hash
                while(item_type + nelements > item){         // Loop over the has elements

                        list<Particle>::const_iterator iter = target.begin();
                        while(iter != target.end()){                 // Loop over the elements of the array
                                if(tmp.Next()){                      // Does element exists in the subarray
                                        if(iter->getType() == *item){ // Check if type matches
                                                tmp.ClearBit(tmp.GetCounter()-1); tmp.SetCounter(); break;
                                        }
                                } iter++;
                        }
                        if(iter == target.end()){ item = item_type + nelements; break; } // No such element
                        name += strlen(name) + 1;  item++;
                }

                if(tmp.GetNumberOfBits() == 0){              // No spares left
                        tmp = flagarray; tmp.SetCounter();
                        list<Particle> noname;
                        list<Particle>::const_iterator iter = target.begin();
                        while(iter != target.end()){             // Loop over the elements of the array
                                if(tmp.Next())noname.push_back(*iter);
                                iter++;
                        }
                        result.push_back(noname);
                }
        } while(flagarray.MoveForvard());
        return result;
}
