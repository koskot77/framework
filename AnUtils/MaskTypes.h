#ifndef MaskTypes_h
#define MaskTypes_h
#include <list>
#include <stdexcept>
#include "AnObjects/Particle.h"

class MaskTypes{
public:
       static list< list<Particle> > FindCombination(const char *mask, const list<Particle>& target);
};

#endif

