PACKAGES := AnUtils AppFramework AnModBasic AnObjects Readers

######################

ifeq (,$(WORKDIR))
WORKDIR := $(shell pwd)
endif

SUBDIRS := $(foreach var,$(PACKAGES),$(shell if test -d $(var); then echo $(var); fi))

all: lib bin doc

lib: $(foreach var,$(SUBDIRS),$(var).lib)

bin: $(foreach var,$(SUBDIRS),$(var).bin)

doc: $(foreach var,$(SUBDIRS),$(var).doc)

clean: $(foreach var,$(SUBDIRS),$(var).clean)

$(foreach var,$(SUBDIRS),$(var).all):
	@echo "-> $@"
	@$(MAKE) -C $(subst ., ,$@) WORKDIR=$(WORKDIR) PACKAGE_NAME=$(subst .all,,$@)

$(foreach var,$(SUBDIRS),$(var).lib):
	@echo "-> $@"
	@$(MAKE) -C $(subst ., ,$@) WORKDIR=$(WORKDIR) PACKAGE_NAME=$(subst .lib,,$@)

$(foreach var,$(SUBDIRS),$(var).bin):
	@echo "-> $@"
	@$(MAKE) -C $(subst ., ,$@) WORKDIR=$(WORKDIR) PACKAGE_NAME=$(subst .bin,,$@)

$(foreach var,$(SUBDIRS),$(var).doc):
	@echo "-> $@"
	@$(MAKE) -C $(subst ., ,$@) WORKDIR=$(WORKDIR) PACKAGE_NAME=$(subst .doc,,$@)

$(foreach var,$(SUBDIRS),$(var).clean):
	@echo "-> $@"
	@$(MAKE) -C $(subst ., ,$@) WORKDIR=$(WORKDIR) PACKAGE_NAME=$(subst .clean,,$@)


%.all:
	@echo "-> $@"
	@$(MAKE) -C $(WORKDIR)/$* all WORKDIR=$(WORKDIR) PACKAGE_NAME=$*

%.lib:
	@echo "-> $@"
	@$(MAKE) -C $(WORKDIR)/$* lib WORKDIR=$(WORKDIR) PACKAGE_NAME=$*

%.bin:
	@echo "-> $@:"
	@$(MAKE) -C $(WORKDIR)/$* bin WORKDIR=$(WORKDIR) PACKAGE_NAME=$*

%.clean:
	@echo "-> $@:"
	@$(MAKE) -C $(WORKDIR)/$* clean WORKDIR=$(WORKDIR) PACKAGE_NAME=$*

%.doc:
	@echo "-> $@:"
	@$(MAKE) -C $(WORKDIR)/$* doc WORKDIR=$(WORKDIR) PACKAGE_NAME=$*

allall: liball binall docall

liball: $(foreach var,$(PACKAGES),$(var).lib)

binall: $(foreach var,$(PACKAGES),$(var).bin)

docall: $(foreach var,$(PACKAGES),$(var).doc)

cleanall: $(foreach var,$(PACKAGES),$(var).clean)

