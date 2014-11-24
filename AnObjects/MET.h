#ifndef MET_h
#define MET_h
#include "Particle.h"
#include <memory>

class MET: public Particle {
public:
    MET(void) : Particle(){}
    MET(float px_, float py_) : Particle(sqrt(px_*px_+py_*py_),px_,py_,0){}
    virtual ~MET(void){}
};

typedef std::shared_ptr<MET> METPointer;

#endif
