# Dependencies 
USING := AppFramework AnObjects Logistics

# No specific files to compile 
LIB_SOURCES :=

# Compile everything from current directory
LIB_SOURCES_DIR := ./

CCSUFF := cxx

# Compiller's flags
CXXFLAGS  = `root-config --cflags` $(CLHEPFLAGS) $(BOOSTFLAGS)
FFLAGS   :=
CXX_BIN_FLAGS :=
F_BIN_FLAGS   :=
CXX_LIB_FLAGS := -std=c++1y -I$(CMSSW_BASE)/src/
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
