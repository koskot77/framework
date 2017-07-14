#ifndef HLTEvent_h
#define HLTEvent_h
#include <memory>
#include "Particle.h"

typedef std::shared_ptr<Particle>     HLTObjectPointer;
typedef std::vector<HLTObjectPointer> HLTObjectCollection;

#endif
