CMSSW        := CMSSW_7_2_0_pre8
ARCH         := slc6_amd64_gcc481
ROOT         := /cvmfs/cms.cern.ch/
CLHEP        := 2.1.4.1-cms
BOOST        := 1.51.0-cms

CXX          := $(ROOT)/$(ARCH)/external/gcc/4.8.1/bin/g++
F77          := $(ROOT)/$(ARCH)/external/gcc/4.8.1/bin/g++
LD           := $(ROOT)/$(ARCH)/external/gcc/4.8.1/bin/g++

CXXFLAGS     += -std=c++0x -fPIC -Wall -frtti -fexceptions -fpic -g \
                -I$(ROOT)/$(ARCH)/cms/cmssw/$(CMSSW)/src/ \
                -I../../

BOOSTFLAGS   := -I$(ROOT)/$(ARCH)/external/boost/$(BOOST)/include/

CLHEPFLAGS   := -I$(ROOT)/$(ARCH)/external/clhep/$(CLHEP)/include/
