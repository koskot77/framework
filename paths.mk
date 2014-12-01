CMSSW        := CMSSW_5_3_8
ARCH         := slc5_amd64_gcc462
ROOT         := /cvmfs/cms.cern.ch/

CXX          := $(ROOT)/$(ARCH)/external/gcc/4.6.2/bin/g++
F77          := $(ROOT)/$(ARCH)/external/gcc/4.6.2/bin/g++
LD           := $(ROOT)/$(ARCH)/external/gcc/4.6.2/bin/g++

CXXFLAGS     += -std=c++0x -fPIC -Wall -frtti -fexceptions -fpic -g \
                -I$(ROOT)/$(ARCH)/cms/cmssw/$(CMSSW)/src/ \
                -I$(ROOT)/$(ARCH)/external/clhep/2.1.1.0-cms/include/ \
                -I$(ROOT)/$(ARCH)/external/boost/1.47.0/include/ \
                -I../../
