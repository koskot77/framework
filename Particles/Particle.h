#ifndef Particle_h
#define Particle_h
#include <list>
#include <utility>
#include <math.h>
#include <string>
#include <unordered_map>

class Particle {
private:
        string name;
        int    type;
        double energy, charge;
        double vector[4], mass, p, theta, phi;
        double x,y,z;
        unordered_map< string, pair<double,double>, hash<string> > idinfo;
        const Particle    *simulation_;
        const Particle    *mother_;
        list<const Particle*> daughters_;

public:
        const char* getName(void) const { return name.c_str(); }
        void        setName(string str) { name = str;          }
        double getEnergy(void) const { return energy; }
        void   setEnergy(double en)  { energy = en;   }
        double getCharge(void) const { return charge; }
        void   setCharge(double ch)  { charge = ch;   }

        void   setType(int tp)     { type = tp;   }
        int    getType(void) const { return type; }

        double getE (void) const { return vector[0]; }
        double getPx(void) const { return vector[1]; }
        double getPy(void) const { return vector[2]; }
        double getPz(void) const { return vector[3]; }

        double getMass (void) const { return mass;  }
        double getP    (void) const { return p;     }
        double getTheta(void) const { return theta; }
        double getPhi  (void) const { return phi;   }
        double getX    (void) const { return x;     }
        double getY    (void) const { return y;     }
        double getZ    (void) const { return z;     }

        void setE     (double e) { vector[0] = e; if(e>=p) mass = sqrt( e*e - p*p ); else mass = -1; }
        void setPxPyPz(double px, double py, double pz) {
                vector[1] = px; vector[2] = py; vector[3] = pz;
                p     = sqrt( px*px + py*py + pz*pz );
                theta = (( p )        ? atan2( sqrt(px*px+py*py), pz ) : 0);
                phi   = (( px || py ) ? atan2( py, px ) : 0 );
                if(vector[0]>=p) mass = sqrt( vector[0]*vector[0] - px*px - py*py - pz*pz ); else mass = -1;
        }
        void setMass (double m) { mass  = m; vector[0] = sqrt( p*p + m*m ); }
        void setP    (double r) { p     = r; vector[3] = p*cos(theta);
                vector[1] = p*sin(theta)*cos(phi);vector[2] = p*sin(theta)*sin(phi);
                if(vector[0]>=p)mass = sqrt( vector[0]*vector[0] - p*p ); else mass = -1;
        }
        void setTheta(double t) { theta = t; vector[3] = p*cos(t);
                vector[1] = p*sin(t)*cos(phi);   vector[2] = p*sin(t)*sin(phi);
        }
        void setPhi  (double f) { phi = f; vector[1] = p*sin(theta)*cos(phi); vector[2] = p*sin(theta)*sin(phi); }
        void setXYZ  (double _x, double _y, double _z) { x=_x; y=_y; z=_z; }

        const unordered_map< string, pair<double,double> >& getID(void) const { return idinfo; }
              unordered_map< string, pair<double,double> >& getID(void)       { return idinfo; }

        const Particle*       mother   (void) const { return mother_;    }
        list<const Particle*> daughters(void) const { return daughters_; }

        Particle& operator+=(const Particle& particle);
        Particle  operator+ (const Particle& particle) const { Particle tmp(*this); tmp+=particle; return tmp; }

        Particle(void);
        ~Particle(void){}
};

#endif
