CMSSW        := $(CMSSW_VERSION)
ARCH         := $(SCRAM_ARCH)
ROOT         := /cvmfs/cms.cern.ch/
#CLHEP        := $(lastword $(subst /, ,$(CLHEP_PARAM_PATH)))
CLHEP        := $(firstword $(subst /, ,$(lastword $(subst clhep/, ,$(CMSSW_FWLITE_INCLUDE_PATH))) ) )
BOOST        := $(firstword $(subst /, ,$(lastword $(subst boost/, ,$(CMSSW_FWLITE_INCLUDE_PATH))) ) )
GCCVER       := $(firstword $(subst /, ,$(lastword $(subst external/gcc/, ,$(SRT_PATH_SCRAMRT))) ) )

#CXX          := $(COMPILER_RUNTIME_OBJECTS)/bin/g++
#F77          := $(COMPILER_RUNTIME_OBJECTS)/bin/g++
#LD           := $(COMPILER_RUNTIME_OBJECTS)/bin/g++

ifneq (,$(ARCH))
    CXX      := $(ROOT)/$(ARCH)/external/gcc/$(GCCVER)/bin/g++
    F77      := $(ROOT)/$(ARCH)/external/gcc/$(GCCVER)/bin/g++
    LD       := $(ROOT)/$(ARCH)/external/gcc/$(GCCVER)/bin/g++
else
    CXX      := g++
    F77      := g++
    LD       := g++
endif

CXXFLAGS     += -std=c++0x -fPIC -Wall -frtti -fexceptions -fpic -g \
                -I$(ROOT)/$(ARCH)/cms/cmssw/$(CMSSW)/src/ \
                -I../../

BOOSTFLAGS   := -I$(ROOT)/$(ARCH)/external/boost/$(BOOST)/include/

CLHEPFLAGS   := -I$(ROOT)/$(ARCH)/external/clhep/$(CLHEP)/include/
