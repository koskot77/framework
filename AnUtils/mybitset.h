#ifndef MYBITSET_H
#define MYBITSET_H
#include <string>   // std::string
#include <string.h> // bzero, strlen, strcpy

class mybitset{
private:
        unsigned char *array;
        int   counter;
        int   size;

public:
        void setSize(int size_){ if(array)delete array; size=size_; array = new unsigned char[size]; }
        int  getSize(void){ return size; }

        bool setBit(int bit,bool value){ if(bit >= size)return false; array[bit] = value; return true; }
        bool setBit(int bit){ return setBit(bit,true); }
        bool clearBit(int bit){ return setBit(bit,false); }
        void clearAll(void){ bzero(array,size); } //for(int i=0; i<size; i++)array[i] = 0; }
        bool getBit(int bit){ if(bit >= size)return false; return array[bit]; }
        void inverse(void){ for(int i=0; i<size; i++)array[i] =! array[i]; }

        int  getLast(int n_bits=-1){ int last = ((n_bits > 0) ? (n_bits) : (size)); while(--last>=0 && !array[last]); return last; }
        int  getNumberOfBits(int n_bits=-1){ int count=0, last = ((n_bits > 0) ? (n_bits) : (size)); for(int i=0; i<last; i++)if(getBit(i))count++; return count; }
        std::string getBinary(void){ char tmp[size+1]; for(int i=0; i<size; i++)if(getBit(i))tmp[i] = '1'; else tmp[i] = '0'; tmp[size] = '\0'; string tmp_ = tmp; return tmp_; }

        int  nextBit(int counter_ = -1){
                if(counter_ >= size)return -1;
                if(counter_ >= 0) counter = counter_;
                return getBit(counter++);
        }
        bool getBit(void){ return getBit(counter); }
        int  getCounter(void){ return counter; }
        void setCounter(int counter_ = 0){ counter = counter_; }

// the two key functions of the container: (1) set first N bits, (2) sequentially iterate over all combinations with N bits set:
        bool setNBits(int n_bits);
        bool moveForward(int n_bits=-1, int number=-1);

        operator int(void);
        mybitset&        operator++(void);
        mybitset        operator++(int);
        mybitset&        operator=(const mybitset& ex);

        mybitset(int size_=sizeof(long)*8);
        mybitset(const mybitset& ex);
        ~mybitset(void);
};

inline mybitset::mybitset(int size_):size(size_){ array = new unsigned char[size_]; counter = 0; }
inline mybitset::~mybitset(void){ if(array)delete array; }
inline mybitset::mybitset(const mybitset& bs){
        size = bs.size; array  = new unsigned char[size];
        for(int i=0; i<size; i++)array[i] = bs.array[i];
        counter = bs.counter;
}

inline
bool mybitset::setNBits(int n_bits){
        if(n_bits>size)return false;
        for(int i=0; i<size; i++)
                if(i<n_bits)array[i]=true; else array[i]=false;
        return true;
}

inline
bool mybitset::moveForward(int n_bits,int number){
        if(n_bits == 1)return false;
        n_bits = ((n_bits > 0) ? (n_bits) : (size));
        int last = getLast(n_bits);
        number = ((number > 0) ? (number) : (getNumberOfBits(n_bits)));
        if(!moveForward(n_bits-1,number-array[n_bits-1])){
                if(last >= 0 && last < n_bits-1){
                        array[last] = false; array[last+1] = true;
                        for(int i=0; i<=last; i++)
                                if(i<number-1)array[i]=true; else array[i]=false;
                        return true;
                } else return false;
        } return true;
}

inline
mybitset::operator int(void){
        if((size_t)size > sizeof(int)*8) return 0;
        int tmp = 0; int count = 0;
        while(size>count){if(array[count])tmp += (1<<count); count++;}
        return tmp;
}

inline
mybitset& mybitset::operator=(const mybitset& bs){
        if(size != bs.size){
                if(array)delete array; size = bs.size;
                array  = new unsigned char[size];
        }
        for(int i=0; i<size; i++)array[i] = bs.array[i];
        counter = bs.counter;
        return *this;
}

inline
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

inline
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

#endif
