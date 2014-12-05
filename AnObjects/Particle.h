#ifndef Particle_h
#define Particle_h
#include <list>
#include <vector>
#include <math.h>
#include <string>
#include <memory>

class Particle {
private:
        double p_, pt_, eta_, theta_, phi_, mass_, charge_;
        double x_, y_, z_, d0_;
        double e_, px_, py_, pz_; // energy-momentum, duplicates the previous data members

        // pure MC truth information
        int    type_, status_, motherType_, daughter1Type_, daughter2Type_;
        const Particle *mother_;
        std::list<const Particle*> daughters_;

        std::string name_;

public:
        const char* name(void) const { return name_.c_str(); }
        void   setName(std::string str) { name_ = str; }
        double    charge(void) const { return charge_; }
        void   setCharge(double ch)  { charge_ = ch;   }
        int       type(void)   const { return type_;   }
        int      pdgId(void)   const { return type_;   }
        void  setPdgId(int tp)       { type_ = tp;     }
        int     status(void)   const { return status_; }
        void setStatus(int  s)       { status_ = s;    }

        double e (void) const { return e_;  }
        double px(void) const { return px_; }
        double py(void) const { return py_; }
        double pz(void) const { return pz_; }
        double pt(void) const { return pt_; }

        double mass (void) const { return mass_;  }
        double p    (void) const { return p_;     }
        double theta(void) const { return theta_; }
        double eta  (void) const { return eta_;   }
        double phi  (void) const { return phi_;   }
        double x    (void) const { return x_;     }
        double y    (void) const { return y_;     }
        double z    (void) const { return z_;     }
        double d0   (void) const { return d0_;    }
        double dz   (void) const { return z_;     }

        int  motherPdgId(void) const { return motherType_; }
        void setMotherPdgId(int id)  { motherType_  =  id; }

        int  daughter1PdgId(void) const { return daughter1Type_; }
        void setDaughter1PdgId(int id)  { daughter1Type_  =  id; }
        int  daughter2PdgId(void) const { return daughter2Type_; }
        void setDaughter2PdgId(int id)  { daughter2Type_  =  id; }

        double dR(double eta, double phi){ return sqrt( (eta-eta_)*(eta-eta_) + (phi-phi_)*(phi-phi_) ); }

        void setEPxPyPz(double e, double px, double py, double pz) {
            e_  = e;
            px_ = px;
            py_ = py;
            pz_ = pz;
            p_     = sqrt( px*px + py*py + pz*pz );
            pt_    = sqrt( px*px + py*py );
            theta_ = atan2( pt_, pz );
            eta_   = -log( tan( theta_/2. ) );
            phi_   = atan2( py, px );
            if( e>=p_ ) mass_ = sqrt( e*e - p_*p_ );
            else        mass_ = -1;
        }
        void setPtEtaPhi(double pt, double eta, double phi){
            pt_  = pt;
            eta_ = eta;
            phi_ = phi;
            theta_ = 2 * atan(exp(-eta));
            p_   = pt_ / sin(theta_);
            px_  = p_*sin(theta_)*cos(phi_);
            py_  = p_*sin(theta_)*sin(phi_);
            pz_  = p_*cos(theta_);
            e_   = p_;
        }
        void setMass (double m) { // preserves total momentum and direction
            mass_  = m;
            e_ = sqrt( p_*p_ + m*m );
        }
        void setP    (double r) { // preserves direction only
            p_  = r;
            pz_ = p_*cos(theta_);
            px_ = p_*sin(theta_)*cos(phi_);
            py_ = p_*sin(theta_)*sin(phi_);
            if(e_>=p_) mass_ = sqrt( e_*e_ - p_*p_ );
            else       mass_ = -1;
        }
        void setTheta(double t) { // preserves total momentum
            theta_ = t;
            eta_   = -log( tan( theta_/2. ) );
            pz_ = p_*cos(t);
            px_ = p_*sin(t)*cos(phi_);
            py_ = p_*sin(t)*sin(phi_);
        }
        void setPhi  (double f) {  // preserves total momentum
            phi_ = f;
            px_  = p_*sin(theta_)*cos(phi_);
            py_  = p_*sin(theta_)*sin(phi_);
        }
        void setXYZ(double x, double y, double z) { x_ = x; y_ = y; z_ = z; }
        void setD0 (double _d0){ d0_ = _d0; }

        const Particle*            mother   (void) const { return mother_;    }
        std::list<const Particle*> daughters(void) const { return daughters_; }
        void setMother  (const Particle *m){ mother_ = m; }
        void addDaughter(const Particle *d){ daughters_.push_back(d); }

        Particle& operator+=(const Particle& particle);
        Particle  operator+ (const Particle& particle) const { Particle tmp(*this); tmp+=particle; return tmp; }
        bool      operator< (const Particle& particle){ return pt() > particle.pt(); } // for sorting

        Particle(void);
        Particle(double energy, double px, double py, double pz);
        virtual ~Particle(void){}
};

typedef std::shared_ptr<Particle>    ParticlePointer;
typedef std::vector<ParticlePointer> ParticleCollection;

#endif
