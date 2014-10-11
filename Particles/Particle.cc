using namespace std;
#include "Particle.h"

Particle::Particle(void){
        type   = -1;
        energy = -1;
        charge = -10;
        vector[0] = 0;
        vector[1] = 0;
        vector[2] = 0;
        vector[3] = 0;
        mass=0; p=0; theta=0; phi=0;
        x=0; y=0; z=0;
        mother_ =  0;
        simulation_ = 0;
}

Particle& Particle::operator+=(const Particle& particle){
        name   = ""; mother_ =  0;
        type   = -1; energy  = -1; charge = -10; idinfo.clear();
        simulation_ = 0;
        x=0; y=0; z=0;
        energy += particle.energy;
        charge += particle.charge;
        vector[0] += particle.vector[0];
        vector[1] += particle.vector[1];
        vector[2] += particle.vector[2];
        vector[3] += particle.vector[3];
        mass = vector[0]*vector[0]-vector[1]*vector[1]-vector[2]*vector[2]-vector[3]*vector[3];
        if( mass<0 ) mass = -1; else mass = sqrt(mass);
        daughters_.push_back(&particle);
        return *this;
}
