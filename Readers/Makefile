# Dependencies 
USING := AppFramework AnObjects

# No specific files to compile 
LIB_SOURCES :=

# Compile everything from current directory
LIB_SOURCES_DIR := ./

# Compiller's flags
CXXFLAGS := -std=c++0x -fPIC -g -Wall `root-config --cflags` -I/cvmfs/cms.cern.ch/slc6_amd64_gcc481/cms/cmssw/CMSSW_7_2_0_pre8/src/ -I/cvmfs/cms.cern.ch/slc6_amd64_gcc481/external/clhep/2.1.4.1-cms/include/
FFLAGS   :=
CXX_BIN_FLAGS :=
F_BIN_FLAGS   :=
CXX_LIB_FLAGS :=
F_LIB_FLAGS   :=

# No dependencies on 3rd party libraries
EXTERNAL_LIBS :=

# Prefer static (.a) over shared (.so) library
LIBTYPE := static

# Add extra dependencies that are not figured automatically (form: PACKAGE_NAME/FILE_NAME)
ifeq (,$(WORKDIR))
WORKDIR := $(shell { cd ../ && pwd; } || exit $$? )
endif

#  Собственно Makefile
include $(WORKDIR)/rules.mk
