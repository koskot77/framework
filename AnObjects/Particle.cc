using namespace std;
#include "Particle.h"

Particle::Particle(void):
   name_(""),
   type_(-1),
   p_(0),
   pt_(0),
   eta_(0),
   theta_(0),
   phi_(0),
   mass_(0),
   charge_(-10),
   x_(0),y_(0),z_(0),
   vector(4),
   mother_(0){}

Particle::Particle(double energy, double px, double py, double pz) : name_(""), type_(-1),
   p_(0),
   pt_(0),
   eta_(0),
   theta_(0),
   phi_(0),
   mass_(0),
   charge_(-10),
   x_(0),y_(0),z_(0),
   vector(4)
{
        charge_ = -10;
        vector[0] = energy;
        vector[1] = px;
        vector[2] = py;
        vector[3] = pz;
        mass_     = 0;
        p_     = sqrt(px*px+py*py+pz*pz);
        pt_    = sqrt(px*px+py*py);
        theta_ = atan2( pt_, pz );
        eta_   = -log( tan( theta_/2. ) );
        phi_   = atan2(py,px);
        x_=0; y_=0; z_=0;
        mother_ =  0;
}

Particle& Particle::operator+=(const Particle& particle){
        name_   = ""; mother_ =  0;
        type_   = -1; charge_ = -10;
        x_=0; y_=0; z_=0;
        charge_   += particle.charge_;
        vector[0] += particle.vector[0];
        vector[1] += particle.vector[1];
        vector[2] += particle.vector[2];
        vector[3] += particle.vector[3];
        mass_ = vector[0]*vector[0]-vector[1]*vector[1]-vector[2]*vector[2]-vector[3]*vector[3];
        if( mass_<0 ) mass_ = -1; else mass_ = sqrt(mass_);
        daughters_.push_back(&particle);
        return *this;
}
