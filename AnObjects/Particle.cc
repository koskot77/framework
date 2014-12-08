using namespace std;
#include "Particle.h"

bool operator<(ParticlePointer i, ParticlePointer j) { return ((bool)i && (bool)j ? (*i)<(*j) : false); }

Particle::Particle(void):
   p_(0),
   pt_(0),
   eta_(0),
   theta_(0),
   phi_(0),
   mass_(0),
   charge_(-10),
   x_(0),y_(0),z_(0),d0_(0),
   e_(0),px_(0),py_(0),pz_(0),
   type_(-1),status_(0),
   motherType_(0), daughter1Type_(0), daughter2Type_(0),
   mother_(0)
{ name_ = ""; }

Particle::Particle(double energy, double px, double py, double pz) :
   p_(0),
   pt_(0),
   eta_(0),
   theta_(0),
   phi_(0),
   mass_(0),
   charge_(-10),
   x_(0),y_(0),z_(0),d0_(0),
   e_(energy),px_(px),py_(py),pz_(pz),
   type_(-1),status_(0),
   motherType_(0), daughter1Type_(0), daughter2Type_(0),
   mother_(0)
{

        name_   = "";
        charge_ = -10;
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
        status_ = 0;
        x_=0; y_=0; z_=0;
        charge_   += particle.charge_;
        e_  += particle.e_;
        px_ += particle.px_;
        py_ += particle.py_;
        pz_ += particle.pz_;

        mass_ = e_*e_-px_*px_-py_*py_-pz_*pz_;
        if( mass_<0 ) mass_ = -1; else mass_ = sqrt(mass_);

        daughters_.push_back(&particle);
        if( !daughter1Type_ ) daughter1Type_ = particle.pdgId();
        else
        if( !daughter2Type_ ) daughter2Type_ = particle.pdgId();

        return *this;
}
