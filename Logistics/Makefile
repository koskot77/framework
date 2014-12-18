# Dependencies 
USING := AppFramework

# No specific files to compile 
LIB_SOURCES :=

# Compile everything from current directory
LIB_SOURCES_DIR := ./

# Compiller's flags
CXXFLAGS :=
FFLAGS   :=
CXX_BIN_FLAGS :=
F_BIN_FLAGS   :=
CXX_LIB_FLAGS :=
F_LIB_FLAGS   :=

# No dependencies on 3rd party libraries
EXTERNAL_LIBS :=

# Prefer static (.a) over shared (.so) library?
LIBTYPE := static

# Add extra dependencies that are not figured automatically (form: PACKAGE_NAME/FILE_NAME)
ENFORCE_DEPENDS :=

# Just a link-safe way to refer to top (..) directory
ifeq (,$(WORKDIR))
WORKDIR := $(shell { cd ../ && pwd; } || exit $$? )
endif

# Include machinery
include $(WORKDIR)/rules.mk
