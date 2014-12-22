#ifndef Trigger_h
#define Trigger_h
#include <memory>
#include <vector>
#include <string>
#include <map>

class Trigger {
private:
    std::vector<int> wasRun_, accept_, error_;
    std::vector<int> prescales_;
    std::vector<std::string>  names_;
    std::map<std::string,int> name2bit_;
    bool changedNames_;

public:
    unsigned int maxSize;

    const char* name(unsigned int triggerBit) const {
        return ( triggerBit<maxSize ? names_[triggerBit].c_str() : "" );
    }
    int bit(const char *name) const {
        std::map<std::string,int>::const_iterator it = name2bit_.find(name);
        return ( it != name2bit_.end() ? it->second : -1 );
    }

    bool namesChanged(void) const { return changedNames_;  }
    void resetNamesChanged(void)  { changedNames_ = false; }

    bool wasRun(unsigned int triggerBit) const { return ( triggerBit<maxSize ? wasRun_[triggerBit] : false ); }
    bool accept(unsigned int triggerBit) const { return ( triggerBit<maxSize ? accept_[triggerBit] : false ); }
    bool error (unsigned int triggerBit) const { return ( triggerBit<maxSize ? error_ [triggerBit] : false ); }

    void setName(unsigned int triggerBit, std::string name){ changedNames_ = true; names_[triggerBit] = name; }
    void setNames(const std::vector<std::string> &names)   { changedNames_ = true; names_ = names;            }

    void setWasRun(unsigned int triggerBit, bool state=true) { if( triggerBit<maxSize ) wasRun_[triggerBit] = state; }
    void setAccept(unsigned int triggerBit, bool state=true) { if( triggerBit<maxSize ) accept_[triggerBit] = state; }
    void setError (unsigned int triggerBit, bool state=true) { if( triggerBit<maxSize ) error_ [triggerBit] = state; }

    //Trigger(void):wasRun_(maxSize), accept_(maxSize), error_(maxSize), prescales_(maxSize), names_(maxSize){}
    Trigger(void):wasRun_(200), accept_(200), error_(200), prescales_(200), names_(200){ changedNames_ = false; maxSize = 200; }
    virtual ~Trigger(void){}
};

typedef std::shared_ptr<Trigger> TriggerPointer;

#endif
