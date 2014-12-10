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
    std::vector<std::string> names_;

public:
    const unsigned int maxSize = 200;

    const char* name(unsigned int triggerBit) const { return names_.at(triggerBit).c_str(); }

    bool wasRun(unsigned int triggerBit) const { return wasRun_.at(triggerBit); }
    bool accept(unsigned int triggerBit) const { return accept_.at(triggerBit); }
    bool error (unsigned int triggerBit) const { return error_. at(triggerBit); }

    void setName(unsigned int triggerBit, std::string name){ names_[triggerBit] = name; }
    void setNames(const std::vector<std::string> &names){ names_ = names; }

    void setWasRun(unsigned int triggerBit, bool state=true) { wasRun_[triggerBit] = state; }
    void setAccept(unsigned int triggerBit, bool state=true) { accept_[triggerBit] = state; }
    void setError (unsigned int triggerBit, bool state=true) { error_ [triggerBit] = state; }

    //Trigger(void):wasRun_(maxSize), accept_(maxSize), error_(maxSize), prescales_(maxSize), names_(maxSize){}
    Trigger(void):wasRun_(200), accept_(200), error_(200), prescales_(200), names_(200){}
    virtual ~Trigger(void){}
};

typedef std::shared_ptr<Trigger> TriggerPointer;

#endif
