# Define directories for objects, libraries, and executables
#TmpDir      := $(WORKDIR)/tmp/
#LibDir	     := $(WORKDIR)/lib/
#BinDir	     := $(WORKDIR)/bin/

ifneq "$(findstring /disk3/KEDR/release/,$(WORKDIR))" ""
      TmpDir := $(WORKDIR)/tmp/
      LibDir := $(WORKDIR)/lib/
      BinDir := $(WORKDIR)/bin/
else
   ifeq (,$(TmpDir))
      TmpDir := $(WORKDIR)/tmp/
   endif
   ifeq (,$(LibDir))
      LibDir := $(WORKDIR)/lib/
   endif
   ifeq (,$(BinDir))
      BinDir := $(WORKDIR)/bin/
   endif
endif


#  Suffixes like .cc .cxx .cpp can be set with FSUFF, CSUFF, and CCSUFF (default are *.f , *.c и *.cc)
ifeq (,$(FSUFF))
      FSUFF  := f
endif
ifeq (,$(CSUFF))
      CSUFF  := c
endif
ifeq (,$(CCSUFF))
      CCSUFF := cc
endif

# Compiler and linker
CXX          := /cvmfs/cms.cern.ch/slc6_amd64_gcc481/external/gcc/4.8.1/bin/g++
F77          := /cvmfs/cms.cern.ch/slc6_amd64_gcc481/external/gcc/4.8.1/bin/g77
LD           := /cvmfs/cms.cern.ch/slc6_amd64_gcc481/external/gcc/4.8.1/bin/g++

# Find out the package name that is using this rules 
ifeq (,$(PACKAGE_NAME))
PACKAGE_NAME := $(word $(words $(subst /, ,$(PWD))),$(subst /, ,$(PWD)))
endif

#  LIBTYPE can be '' (compile nothing), 'static', 'dymanic', or 'static dymanic' (to build both)
ifneq "$(findstring static,$(LIBTYPE))" ""
STATIC_LIB_NAME  := $(LibDir)lib$(PACKAGE_NAME).a
endif
ifneq "$(findstring dynamic,$(LIBTYPE))" ""
DYNAMIC_LIB_NAME := $(LibDir)lib$(PACKAGE_NAME).so
endif

#  If BINARIES defines executables all the related source files are sought in
BIN_NAMES := $(foreach var, $(BINARIES), $(BinDir)$(var))

# If defined the LIB_SOURCES_DIR will be prefered to build the list of all targets matching the wildcard
# (list of specific files will be ignored)
ifneq (,$(LIB_SOURCES_DIR))
LIB_SOURCES := $(wildcard $(LIB_SOURCES_DIR)/*.*)
endif

#  phony target
.PHONY:      doc clean PRE_MAKE POST_MAKE

#  standard targets
all:         PRE_MAKE $(STATIC_LIB_NAME) $(DYNAMIC_LIB_NAME) $(BIN_NAMES) POST_MAKE
bin:         PRE_MAKE $(BIN_NAMES) POST_MAKE
lib:         PRE_MAKE $(STATIC_LIB_NAME) $(DYNAMIC_LIB_NAME) POST_MAKE

# Libraries and headers of upstream dependencies

RELEASE  := $(shell cat $(WORKDIR)/.release)
CXXFLAGS += -I$(WORKDIR) -I$(RELEASE)
FFLAGS   += -I$(WORKDIR) -I$(RELEASE)
ifneq (,$(USING))
  DEPEND := $(sort $(shell for i in $(USING);  do if test -d $(WORKDIR)/$$i; then cat $(WORKDIR)/$$i/.depend; else cat $(RELEASE)/$$i/.depend; fi; echo $$i; done ) )
endif
ifneq (,$(DEPEND))
  FFLAGS += $(sort $(shell for i in $(DEPEND); do if test -d $(WORKDIR)/$$i; then echo -I$(WORKDIR)/$$i; else echo -I$(RELEASE)/$$i; fi; done ) )
endif
INTERNAL_LIBS   := -L$(WORKDIR)/lib -L/$(RELEASE)/lib
ifneq (,$(DEPEND))
  INTERNAL_LIBS += $(sort $(shell for i in $(DEPEND); do if test -f $(WORKDIR)/lib/lib$$i.a || test -f $(WORKDIR)/lib/lib$$i.so || test -f $(RELEASE)/lib/lib$$i.a || test -f $(RELEASE)/lib/lib$$i.so; then echo -l$$i; fi; done ) )
endif
ifneq (,$(DEPEND))
  EXTERNAL_LIBS += $(sort $(shell for i in $(DEPEND); do if test -d $(WORKDIR)/$$i; then cat $(WORKDIR)/$$i/.extlibs; else cat $(RELEASE)/$$i/.extlibs; fi; done ) )
endif

# Manually defined dependencies 
ifneq (,$(ENFORCE_DEPENDS))
  TMP_DEP_VAR     := $(sort $(shell for i in $(ENFORCE_DEPENDS); do if test -f $(WORKDIR)/$$i; then echo $(WORKDIR)/$$i; else echo $(RELEASE)/$$i; fi; done ) )
  ENFORCE_DEPENDS := $(TMP_DEP_VAR)
endif

# Temporary storage 
OBJ_DIR     := $(shell echo $(TmpDir)/$(PACKAGE_NAME)/ | sed -e 's|\([^\. ]\)[\./]\?/|\1/|g' -e 's|/\+|/|g' )
BIN_OBJ_DIR := $(OBJ_DIR)binobj/
LIB_OBJ_DIR := $(OBJ_DIR)libobj/

#  Object files (only C[++] and FORTRAN)
SOURCES := $(LIB_SOURCES) $(BIN_SOURCES)
#ifneq (,$(filter-out %.$(FSUFF) %.$(CSUFF) %.$(CCSUFF), $(SOURCES)))
#@echo "Unknown sources type: $(filter-out %.$(FSUFF) %.$(CSUFF) %.$(CCSUFF) , $(SOURCES)). Use only .$(FSUFF), .$(CSUFF) and .$(CCSUFF) suffixes.\n"
#endif

C_BIN_OBJS    := $(sort $(addprefix $(BIN_OBJ_DIR),$(patsubst %.$(CSUFF),  %.o, $(filter %.$(CSUFF),  $(foreach var, $(BINARIES), $($(var)_SOURCES)) ))) )
CC_BIN_OBJS   := $(sort $(addprefix $(BIN_OBJ_DIR),$(patsubst %.$(CCSUFF), %.o, $(filter %.$(CCSUFF), $(foreach var, $(BINARIES), $($(var)_SOURCES)) ))) )
F_BIN_OBJS    := $(sort $(addprefix $(BIN_OBJ_DIR),$(patsubst %.$(FSUFF),  %.o, $(filter %.$(FSUFF),  $(foreach var, $(BINARIES), $($(var)_SOURCES)) ))) )

C_LIB_OBJS    := $(sort $(addprefix $(LIB_OBJ_DIR),$(patsubst %.$(CSUFF),  %.o, $(filter %.$(CSUFF),  $(LIB_SOURCES)))) )
CC_LIB_OBJS   := $(sort $(addprefix $(LIB_OBJ_DIR),$(patsubst %.$(CCSUFF), %.o, $(filter %.$(CCSUFF), $(LIB_SOURCES)))) )
F_LIB_OBJS    := $(sort $(addprefix $(LIB_OBJ_DIR),$(patsubst %.$(FSUFF),  %.o, $(filter %.$(FSUFF),  $(LIB_SOURCES)))) )

# Purge various /./ ./// etc. from the file names
ifneq (,$(C_BIN_OBJS))
  C_BIN_OBJS  := $(shell echo $(C_BIN_OBJS)  | sed -e 's|\([^\. ]\)[\./]\?/|\1/|g' -e 's|/\+|/|g' )
endif
ifneq (,$(CC_BIN_OBJS))
  CC_BIN_OBJS := $(shell echo $(CC_BIN_OBJS) | sed -e 's|\([^\. ]\)[\./]\?/|\1/|g' -e 's|/\+|/|g' )
endif
ifneq (,$(F_BIN_OBJS))
  F_BIN_OBJS  := $(shell echo $(F_BIN_OBJS)  | sed -e 's|\([^\. ]\)[\./]\?/|\1/|g' -e 's|/\+|/|g' )
endif
ifneq (,$(C_LIB_OBJS))
  C_LIB_OBJS  := $(shell echo $(C_LIB_OBJS)  | sed -e 's|\([^\. ]\)[\./]\?/|\1/|g' -e 's|/\+|/|g' )
endif
ifneq (,$(CC_LIB_OBJS))
  CC_LIB_OBJS := $(shell echo $(CC_LIB_OBJS) | sed -e 's|\([^\. ]\)[\./]\?/|\1/|g' -e 's|/\+|/|g' )
endif
ifneq (,$(F_LIB_OBJS))
  F_LIB_OBJS  := $(shell echo $(F_LIB_OBJS)  | sed -e 's|\([^\. ]\)[\./]\?/|\1/|g' -e 's|/\+|/|g' )
endif

# Gather objec code for linking 
BIN_OBJS := $(F_BIN_OBJS) $(C_BIN_OBJS) $(CC_BIN_OBJS)
LIB_OBJS := $(F_LIB_OBJS) $(C_LIB_OBJS) $(CC_LIB_OBJS)

# Link binaries
$(BIN_NAMES):	$(BIN_OBJS) #Makefile
		$(LD) $(LINK_BIN_OPTS) $(patsubst %.$(FSUFF), $(BIN_OBJ_DIR)/%.o, $(patsubst %.$(CSUFF), $(BIN_OBJ_DIR)/%.o, $(patsubst %.$(CCSUFF), $(BIN_OBJ_DIR)/%.o, $($(patsubst $(BinDir)%,%,$@)_SOURCES) ) ) ) $(STATIC_LIB_NAME) $(DYNAMIC_LIB_NAME) $(INTERNAL_LIBS) $(EXTERNAL_LIBS) $(INTERNAL_LIBS) $(EXTERNAL_LIBS) -o $@
		@echo $(patsubst %.$(FSUFF), $(BIN_OBJ_DIR)/%.o, $(patsubst %.$(CSUFF), $(BIN_OBJ_DIR)/%.o, $(patsubst %.$(CCSUFF), $(BIN_OBJ_DIR)/%.o, $($(patsubst $(BinDir)%,%,$@)_SOURCES) ) ) )
		$(shell echo $(DEPEND) > .depend )
		@echo "$@ done"

# Link libraries
$(STATIC_LIB_NAME):	Makefile $(LIB_OBJS)
		$(AR) rulc $(LINKER_OPTS) $(STATIC_LIB_NAME) $(LIB_OBJS)
		$(shell echo "$(DEPEND)" > .depend )
		$(shell echo "$(EXTERNAL_LIBS)" > .extlibs )
		@echo "$(STATIC_LIB_NAME) done"

$(DYNAMIC_LIB_NAME):	Makefile $(LIB_OBJS)
		$(LD) -shared $(LINKER_OPTS) $(LIB_OBJS) $(INTERNAL_LIBS) $(EXTERNAL_LIBS) $(INTERNAL_LIBS) $(EXTERNAL_LIBS) -o $(DYNAMIC_LIB_NAME)
		$(shell echo "$(DEPEND)" > .depend )
		$(shell echo "$(EXTERNAL_LIBS)" > .extlibs )
		@echo "$(DYNAMIC_LIB_NAME) done"

#  Compile C++
$(CC_LIB_OBJS):  $(LIB_OBJ_DIR)%.o: %.$(CCSUFF)
		@if ! test -d $(dir $@); then mkdir -p $(dir $@); fi
		$(CXX) -o $@ -c $(CXXFLAGS) $(CXX_LIB_FLAGS) $<
$(CC_BIN_OBJS):  $(BIN_OBJ_DIR)%.o: %.$(CCSUFF)
		@if ! test -d $(dir $@); then mkdir -p $(dir $@); fi
		$(CXX) -o $@ -c $(CXXFLAGS) $(CXX_BIN_FLAGS) $<

# Compile C
$(C_LIB_OBJS):  $(LIB_OBJ_DIR)%.o: %.$(CSUFF)
		if ! test -d $(dir $@); then mkdir -p $(dir $@); fi
		$(CXX) -o $@ -c $(CXXFLAGS) $(CXX_LIB_FLAGS) $<
$(C_BIN_OBJS):  $(BIN_OBJ_DIR)%.o: %.$(CSUFF)
		if ! test -d $(dir $@); then mkdir -p $(dir $@); fi
		$(CXX) -o $@ -c $(CXXFLAGS) $(CXX_BIN_FLAGS) $<

# Compile FORTRAN
$(F_LIB_OBJS):  $(LIB_OBJ_DIR)%.o: %.$(FSUFF)
		if ! test -d $(dir $@); then mkdir -p $(dir $@); fi
		$(F77) -o $@ -c $(FFLAGS) $(F_LIB_FLAGS) $<
$(F_BIN_OBJS):  $(BIN_OBJ_DIR)%.o: %.$(FSUFF)
		if ! test -d $(dir $@); then mkdir -p $(dir $@); fi
		$(F77) -o $@ -c $(FFLAGS) $(F_BIN_FLAGS) $<

# Generate dependencies
#DEPEND_C_FOR_MAKE_LIB := $(foreach var, $(C_LIB_OBJS:%.o=%.d), $(shell if test -s `basename $(var) .d`.$(CSUFF); then echo $(var); fi) )
DEPEND_C_FOR_MAKE_LIB := $(foreach var, $(C_LIB_OBJS:%.o=%.d), $(var))
ifneq (,$(DEPEND_C_FOR_MAKE_LIB))
  DEPEND_C_FOR_MAKE_LIB_EXIST := $(shell for i in $(DEPEND_C_FOR_MAKE_LIB); do if test -f $$i; then echo $$i; fi; done)
endif
$(DEPEND_C_FOR_MAKE_LIB): $(LIB_OBJ_DIR)%.d: %.$(CSUFF)
	@if ! test -d $(dir $@); then mkdir -p $(dir $@); fi
	@set -e; $(CXX) -E -MM $(CXXFLAGS) $(CXX_LIB_FLAGS) $< \
	  | sed 's|\w\+\.o[ :]\+|$(LIB_OBJ_DIR)$*.o $@ : |g' > $@;

#DEPEND_C_FOR_MAKE_BIN := $(foreach var, $(C_BIN_OBJS:%.o=%.d), $(shell if test -s `basename $(var) .d`.$(CSUFF); then echo $(var); fi) )
DEPEND_C_FOR_MAKE_BIN := $(foreach var, $(C_BIN_OBJS:%.o=%.d), $(var))
ifneq (,$(DEPEND_C_FOR_MAKE_BIN))
  DEPEND_C_FOR_MAKE_BIN_EXIST := $(shell for i in $(DEPEND_C_FOR_MAKE_BIN); do if test -f $$i; then echo $$i; fi; done)
endif
$(DEPEND_C_FOR_MAKE_BIN): $(BIN_OBJ_DIR)%.d: %.$(CSUFF)
	@if ! test -d $(dir $@); then mkdir -p $(dir $@); fi
	@set -e; $(CXX) -E -MM $(CXXFLAGS) $(CXX_BIN_FLAGS) $< \
	  | sed 's|\w\+\.o[ :]\+|$(BIN_OBJ_DIR)$*.o $@ : |g' > $@;

#DEPEND_CC_FOR_MAKE_LIB := $(foreach var, $(CC_LIB_OBJS:%.o=%.d), $(shell if test -s `basename $(var) .d`.$(CCSUFF); then echo $(var); fi) )
DEPEND_CC_FOR_MAKE_LIB := $(foreach var, $(CC_LIB_OBJS:%.o=%.d), $(var))
ifneq (,$(DEPEND_CC_FOR_MAKE_LIB))
  DEPEND_CC_FOR_MAKE_LIB_EXIST := $(shell for i in $(DEPEND_CC_FOR_MAKE_LIB); do if test -f $$i; then echo $$i; fi; done)
endif
$(DEPEND_CC_FOR_MAKE_LIB): $(LIB_OBJ_DIR)%.d: %.$(CCSUFF)
	@if ! test -d $(dir $@); then mkdir -p $(dir $@); fi
	@set -e; $(CXX) -E -MM $(CXXFLAGS) $(CXX_LIB_FLAGS) $< \
	  | sed 's|\w\+\.o[ :]\+|$(LIB_OBJ_DIR)$*.o $@ : |g' > $@;

#DEPEND_CC_FOR_MAKE_BIN := $(foreach var, $(CC_BIN_OBJS:%.o=%.d), $(shell if test -s `basename $(var) .d`.$(CCSUFF); then echo $(var); fi) )
DEPEND_CC_FOR_MAKE_BIN := $(foreach var, $(CC_BIN_OBJS:%.o=%.d), $(var))
ifneq (,$(DEPEND_CC_FOR_MAKE_BIN))
  DEPEND_CC_FOR_MAKE_BIN_EXIST := $(shell for i in $(DEPEND_CC_FOR_MAKE_BIN); do if test -f $$i; then echo $$i; fi; done)
endif
$(DEPEND_CC_FOR_MAKE_BIN): $(BIN_OBJ_DIR)%.d: %.$(CCSUFF)
	@if ! test -d $(dir $@); then mkdir -p $(dir $@); fi
	@set -e; $(CXX) -E -MM $(CXXFLAGS) $(CXX_BIN_FLAGS) $< \
	  | sed 's|\w\+\.o[ :]\+|$(BIN_OBJ_DIR)$*.o $@ : |g' > $@;

PRE_MAKE: $(DEPEND_C_FOR_MAKE_LIB) $(DEPEND_C_FOR_MAKE_BIN) $(DEPEND_CC_FOR_MAKE_LIB) $(DEPEND_CC_FOR_MAKE_BIN)

ifneq ($(findstring all,$(MAKECMDGOALS)),)
ifneq (,$(DEPEND_C_FOR_MAKE_LIB_EXIST))
include $(DEPEND_C_FOR_MAKE_LIB_EXIST)
endif
ifneq (,$(DEPEND_CC_FOR_MAKE_LIB_EXIST))
include $(DEPEND_CC_FOR_MAKE_LIB_EXIST)
endif
ifneq (,$(DEPEND_C_FOR_MAKE_BIN_EXIST))
include $(DEPEND_C_FOR_MAKE_BIN_EXIST)
endif
ifneq (,$(DEPEND_CC_FOR_MAKE_BIN_EXIST))
include $(DEPEND_CC_FOR_MAKE_BIN_EXIST)
endif
endif

ifeq ($(MAKECMDGOALS),)
ifneq (,$(DEPEND_C_FOR_MAKE_LIB_EXIST))
include $(DEPEND_C_FOR_MAKE_LIB_EXIST)
endif
ifneq (,$(DEPEND_CC_FOR_MAKE_LIB_EXIST))
include $(DEPEND_CC_FOR_MAKE_LIB_EXIST)
endif
ifneq (,$(DEPEND_C_FOR_MAKE_BIN_EXIST))
include $(DEPEND_C_FOR_MAKE_BIN_EXIST)
endif
ifneq (,$(DEPEND_CC_FOR_MAKE_BIN_EXIST))
include $(DEPEND_CC_FOR_MAKE_BIN_EXIST)
endif
endif

ifneq ($(findstring lib,$(MAKECMDGOALS)),)
ifneq (,$(DEPEND_C_FOR_MAKE_LIB_EXIST))
include $(DEPEND_C_FOR_MAKE_LIB_EXIST)
endif
ifneq (,$(DEPEND_CC_FOR_MAKE_LIB_EXIST))
include $(DEPEND_CC_FOR_MAKE_LIB_EXIST)
endif
endif

ifneq ($(findstring bin,$(MAKECMDGOALS)),)
ifneq (,$(DEPEND_C_FOR_MAKE_BIN_EXIST))
include $(DEPEND_C_FOR_MAKE_BIN_EXIST)
endif
ifneq (,$(DEPEND_CC_FOR_MAKE_BIN_EXIST))
include $(DEPEND_CC_FOR_MAKE_BIN_EXIST)
endif
endif

# Clean it all
clean:
		@rm -rf $(BIN_NAMES) $(DYNAMIC_LIB_NAME) $(STATIC_LIB_NAME) $(OBJ_DIR)
