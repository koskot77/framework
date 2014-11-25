#ifndef Particle_h
#define Particle_h
#include <list>
#include <vector>
#include <math.h>
#include <string>

class Particle {
private:
        std::string name_;
        int    type_;
        double p_, pt_, eta_, theta_, phi_, mass_, charge_;
        double x_, y_, z_, d0_;
        std::vector<double> vector; // energy-momentum, duplicates the previous data members
        const Particle *mother_;
        std::list<const Particle*> daughters_;

public:
        const char* name(void) const { return name_.c_str(); }
        void   setName(std::string str) { name_ = str; }
        double    charge(void) const { return charge_; }
        void   setCharge(double ch)  { charge_ = ch;   }
        int       type(void)   const { return type_;   }
        void   setType(int tp)       { type_ = tp;     }

        double e (void) const { return vector[0]; }
        double px(void) const { return vector[1]; }
        double py(void) const { return vector[2]; }
        double pz(void) const { return vector[3]; }
        double pt(void) const { return sqrt(vector[1]*vector[1]+vector[2]*vector[2]); }

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

        std::vector<double> fourVector(void) const { return vector; }

        void setEPxPyPz(double e, double px, double py, double pz) {
            vector[0] = e;
            vector[1] = px;
            vector[2] = py;
            vector[3] = pz;
            p_     = sqrt( px*px + py*py + pz*pz );
            pt_    = sqrt( px*px + py*py );
            theta_ = atan2( pt_, pz );
            eta_   = -log( tan( theta_/2. ) );
            phi_   = atan2( py, px );
            if( e>=p_ ) mass_ = sqrt( e*e - p_*p_ );
            else        mass_ = -1;
        }
        void setMass (double m) { // preserves total momentum and direction
            mass_  = m;
            vector[0] = sqrt( p_*p_ + m*m );
        }
        void setP    (double r) { // preserves direction only
            p_     = r;
            vector[3] = p_*cos(theta_);
            vector[1] = p_*sin(theta_)*cos(phi_);
            vector[2] = p_*sin(theta_)*sin(phi_);
            if(vector[0]>=p_) mass_ = sqrt( vector[0]*vector[0] - p_*p_ );
            else              mass_ = -1;
        }
        void setTheta(double t) { // preserves total momentum
            theta_ = t;
            eta_   = -log( tan( theta_/2. ) );
            vector[3] = p_*cos(t);
            vector[1] = p_*sin(t)*cos(phi_);
            vector[2] = p_*sin(t)*sin(phi_);
        }
        void setPhi  (double f) {  // preserves total momentum
            phi_ = f;
            vector[1] = p_*sin(theta_)*cos(phi_);
            vector[2] = p_*sin(theta_)*sin(phi_);
        }
        void setXYZ(double x, double y, double z) { x_ = x; y_ = y; z_ = z; }
        void setD0 (double _d0){ d0_ = _d0; }

        const Particle*            mother   (void) const { return mother_;    }
        std::list<const Particle*> daughters(void) const { return daughters_; }

        Particle& operator+=(const Particle& particle);
        Particle  operator+ (const Particle& particle) const { Particle tmp(*this); tmp+=particle; return tmp; }
        bool      operator< (const Particle& particle){ return pt() > particle.pt(); } // for sorting

        Particle(void);
        Particle(double energy, double px, double py, double pz);
        virtual ~Particle(void){}
};

#endif
