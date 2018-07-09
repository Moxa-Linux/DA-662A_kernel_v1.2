############################################################################
#
# Makefile -- Top level uClinux makefile.
#
# Copyright (c) 2001-2004, SnapGear (www.snapgear.com)
# Copyright (c) 2001, Lineo
#
############################################################################
#
# Lets work out what the user wants, and if they have configured us yet
#

ifeq (.config,$(wildcard .config))
include .config

VERSIONSTR = $(CONFIG_VENDOR)/$(CONFIG_PRODUCT) Version $(VERSIONPKG)

#all: subdirs romfs modules modules_install image
#all: linux include lib include user host romfs modules modules_install image test
all: linux include modules lib user romfs modules_install image

# 06-21-2009. Add by Jared for fast building the firmware and ROM files
fast_build: linux romfs modules modules_install image
else
all: config_error
endif

############################################################################
#
# Get the core stuff worked out
#

LINUXDIR = $(CONFIG_LINUXDIR)
KERNEL_VER= `cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`
LIBCDIR  = $(CONFIG_LIBCDIR)
ROOTDIR  = $(shell pwd)
PATH	 := $(PATH):$(ROOTDIR)/tools
HOSTCC   = cc
IMAGEDIR = $(ROOTDIR)/images
ROMFSDIR = $(ROOTDIR)/romfs
ROMFSINST= romfs-inst.sh
IPKGBUILD = ipkg-build.sh
IPKGMAKEINDEX = ipkg-make-index
IPKGDIR =   $(ROOTDIR)/packages
SCRIPTSDIR = $(ROOTDIR)/config/scripts
TFTPDIR    = /tftpboot


LINUX_CONFIG  = $(ROOTDIR)/$(LINUXDIR)/.config
CONFIG_CONFIG = $(ROOTDIR)/config/.config
MODULES_CONFIG = $(ROOTDIR)/modules/.config
PROJECTS_CONFIG = $(ROOTDIR)/projects/.config


CONFIG_SHELL := $(shell if [ -x "$$BASH" ]; then echo $$BASH; \
	  else if [ -x /bin/bash ]; then echo /bin/bash; \
	  else echo sh; fi ; fi)

ifeq (config.arch,$(wildcard config.arch))
include config.arch
ARCH_CONFIG = $(ROOTDIR)/config.arch
export ARCH_CONFIG
endif

ifneq ($(SUBARCH),)
# Using UML, so make the kernel and non-kernel with different ARCHs
MAKEARCH = $(MAKE) ARCH=$(SUBARCH) CROSS_COMPILE=$(CROSS_COMPILE)
MAKEARCH_KERNEL = $(MAKE) ARCH=$(ARCH) SUBARCH=$(SUBARCH) CROSS_COMPILE=$(CROSS_COMPILE)
else
MAKEARCH = $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)
MAKEARCH_KERNEL = $(MAKEARCH)
endif

DIRS    = $(VENDOR_TOPDIRS) include lib include user projects
VENDDIR = $(ROOTDIR)/vendors/$(CONFIG_VENDOR)/$(CONFIG_PRODUCT)/.

export VENDOR PRODUCT ROOTDIR LINUXDIR HOSTCC CONFIG_SHELL
export CONFIG_CONFIG LINUX_CONFIG MODULES_CONFIG ROMFSDIR SCRIPTSDIR
export VERSIONPKG VERSIONSTR ROMFSINST PATH IMAGEDIR RELFILES TFTPDIR LIBCDIR
export FULLVERSIONPKG
export MAKEARCH MAKEARCH_KERNEL

############################################################################

#
# Config stuff,  we recall ourselves to load the new config.arch before
# running the kernel and other config scripts
#

.PHONY: config.tk config.in

config.in:
	@chmod u+x config/mkconfig
	config/mkconfig > config.in

config.tk: config.in
	$(MAKE) -C $(SCRIPTSDIR) tkparse
	ARCH=dummy $(SCRIPTSDIR)/tkparse < config.in > config.tmp
	@if [ -f /usr/local/bin/wish ];	then \
		echo '#!'"/usr/local/bin/wish -f" > config.tk; \
	else \
		echo '#!'"/usr/bin/wish -f" > config.tk; \
	fi
	cat $(SCRIPTSDIR)/header.tk >> ./config.tk
	cat config.tmp >> config.tk
	rm -f config.tmp
	echo "set defaults \"/dev/null\"" >> config.tk
	echo "set help_file \"config/Configure.help\"" >> config.tk
	cat $(SCRIPTSDIR)/tail.tk >> config.tk
	chmod 755 config.tk

.PHONY: xconfig
xconfig: config.tk
	@wish -f config.tk
	@if [ ! -f .config ]; then \
		echo; \
		echo "You have not saved your config, please re-run make config"; \
		echo; \
		exit 1; \
	 fi
	@chmod u+x config/setconfig
	@config/setconfig defaults
	@if egrep "^CONFIG_DEFAULTS_KERNEL=y" .config > /dev/null; then \
		$(MAKE) linux_xconfig; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_MODULES=y" .config > /dev/null; then \
		$(MAKE) modules_xconfig; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_VENDOR=y" .config > /dev/null; then \
		$(MAKE) config_xconfig; \
	 fi
	@config/setconfig final

.PHONY: config
config: config.in
	@HELP_FILE=config/Configure.help \
		$(CONFIG_SHELL) $(SCRIPTSDIR)/Configure config.in
	@chmod u+x config/setconfig
	@config/setconfig defaults
	@if egrep "^CONFIG_DEFAULTS_KERNEL=y" .config > /dev/null; then \
		$(MAKE) linux_config; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_MODULES=y" .config > /dev/null; then \
		$(MAKE) modules_config; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_VENDOR=y" .config > /dev/null; then \
		$(MAKE) config_config; \
	 fi
	@config/setconfig final

.PHONY: menuconfig
menuconfig: config.in
	$(MAKE) -C $(SCRIPTSDIR)/lxdialog all
	@HELP_FILE=config/Configure.help \
		$(CONFIG_SHELL) $(SCRIPTSDIR)/Menuconfig config.in
	@if [ ! -f .config ]; then \
		echo; \
		echo "You have not saved your config, please re-run make config"; \
		echo; \
		exit 1; \
	 fi
	@chmod u+x config/setconfig
	@config/setconfig defaults
	@if egrep "^CONFIG_DEFAULTS_KERNEL=y" .config > /dev/null; then \
		$(MAKE) linux_menuconfig; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_MODULES=y" .config > /dev/null; then \
		$(MAKE) modules_menuconfig; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_VENDOR=y" .config > /dev/null; then \
		$(MAKE) config_menuconfig; \
	 fi
	@if egrep "^CONFIG_DEFAULTS_PROJECTS=y" .config > /dev/null; then \
		$(MAKE) projects_menuconfig; \
	 fi
	@config/setconfig final

.PHONY: oldconfig
oldconfig: config.in
	@HELP_FILE=config/Configure.help \
		$(CONFIG_SHELL) $(SCRIPTSDIR)/Configure -d config.in
	@$(MAKE) oldconfig_linux
	@$(MAKE) oldconfig_modules
	@$(MAKE) oldconfig_config
	@$(MAKE) oldconfig_uClibc
	@chmod u+x config/setconfig
	@config/setconfig final

.PHONY: modules
modules: #modules_customize
	. $(LINUXDIR)/.config; if [ "$$CONFIG_MODULES" = "y" ]; then \
		echo "Making modules"; \
		$(MAKEARCH_KERNEL) -C $(LINUXDIR) modules; \
	fi

.PHONY: modules_install
modules_install: linux_modules_install #modules_customize_install
	. $(LINUXDIR)/.config; if [ "$$CONFIG_MODULES" = "y" ]; then \
		rm -f $(ROMFSDIR)/lib/modules/*/build; \
		find $(ROMFSDIR)/lib/modules -type f -name "*o" | xargs -r $(STRIP) -g; \
		rm -rf $(ROMFSDIR)/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/source; \
	fi
	$(ROOTDIR)/tools/module-init-tools-3.4.1/depmod -a -b $(ROMFSDIR) -v $(KERNEL_VER)
	rm -rf 	$(ROMFSDIR)/lib/modules/$(KERNEL_VER)/modules.builtin \
	$(ROMFSDIR)/lib/modules/$(KERNEL_VER)/modules.builtin.bin \
	$(ROMFSDIR)/lib/modules/$(KERNEL_VER)/modules.ccwmap \
	$(ROMFSDIR)/lib/modules/$(KERNEL_VER)/modules.devname \
	$(ROMFSDIR)/lib/modules/$(KERNEL_VER)/modules.ieee1394map \
	$(ROMFSDIR)/lib/modules/$(KERNEL_VER)/modules.inputmap \
	$(ROMFSDIR)/lib/modules/$(KERNEL_VER)/modules.isapnpmap \
	$(ROMFSDIR)/lib/modules/$(KERNEL_VER)/modules.ofmap \
	$(ROMFSDIR)/lib/modules/$(KERNEL_VER)/modules.order \
	$(ROMFSDIR)/lib/modules/$(KERNEL_VER)/modules.pcimap \
	$(ROMFSDIR)/lib/modules/$(KERNEL_VER)/modules.seriomap \
	$(ROMFSDIR)/lib/modules/$(KERNEL_VER)/modules.softdep \
	$(ROMFSDIR)/lib/modules/$(KERNEL_VER)/modules.usbmap \
	$(ROMFSDIR)/lib/modules/$(KERNEL_VER)/modules.symbols*

#	$(ROMFSDIR)/lib/modules/$(KERNEL_VER)/modules.alias \
#	$(ROMFSDIR)/lib/modules/$(KERNEL_VER)/modules.alias.bin \


modules_customize:
	. $(LINUXDIR)/.config; if [ "$$CONFIG_MODULES" = "y" ]; then \
		echo "Making customize modules"; \
		$(MAKEARCH_KERNEL) -C modules; \
	fi

modules_customize_install:
	. $(LINUXDIR)/.config; if [ "$$CONFIG_MODULES" = "y" ]; then \
		$(MAKEARCH_KERNEL) -C modules INSTALL_MOD_PATH=$(ROMFSDIR) DEPMOD=true install; \
	fi

.PHONY: projects
projects:
	echo "Making customize projects"
	$(MAKEARCH_KERNEL) -C projects

linux_xconfig:
	$(MAKEARCH_KERNEL) -C $(LINUXDIR) xconfig
linux_menuconfig:
	$(MAKEARCH_KERNEL) -C $(LINUXDIR) menuconfig
linux_config:
	$(MAKEARCH_KERNEL) -C $(LINUXDIR) config
linux_modules_install:
	. $(LINUXDIR)/.config; if [ "$$CONFIG_MODULES" = "y" ]; then \
		$(MAKEARCH_KERNEL) -C $(LINUXDIR) INSTALL_MOD_PATH=$(ROMFSDIR) DEPMOD=true modules_install; \
	fi
linux_clean:
	$(MAKEARCH_KERNEL) -C $(LINUXDIR) clean
linux_distclean:
	$(MAKEARCH_KERNEL) -C $(LINUXDIR) distclean
modules_xconfig:
	[ ! -d modules ] || $(MAKEARCH) -C modules xconfig
modules_menuconfig:
	[ ! -d modules ] || $(MAKEARCH) -C modules menuconfig
modules_config:
	[ ! -d modules ] || $(MAKEARCH) -C modules config
modules_clean:
	-[ ! -d modules ] || $(MAKEARCH) -C modules clean
config_xconfig:
	$(MAKEARCH) -C config xconfig
config_menuconfig:
	$(MAKEARCH) -C config menuconfig
config_config:
	$(MAKEARCH) -C config config
oldconfig_config:
	$(MAKEARCH) -C config oldconfig
oldconfig_modules:
	[ ! -d modules ] || $(MAKEARCH) -C modules oldconfig
oldconfig_linux:
	$(MAKEARCH_KERNEL) -C $(LINUXDIR) oldconfig
oldconfig_uClibc:
	[ -z "$(findstring uClibc,$(LIBCDIR))" ] || $(MAKEARCH) -C $(LIBCDIR) oldconfig
projects_menuconfig:
	[ ! -d projects ] || $(MAKEARCH) -C projects menuconfig

############################################################################
#
# normal make targets
#

.PHONY: test
test:
	cd test && make release && cd ..
	cd test && make debug && cd ..

.PHONY: host
host:
	cd host && make release && cd ..
	cd host && make debug && cd ..

.PHONY: romfs
romfs:
	@echo "Create the root file system directory."
	rm -rf $(ROMFSDIR)
	mkdir -p $(ROMFSDIR)
	for i in $(ROMFS_DIRS); do \
		[ -d $(ROMFSDIR)/$$i ] || mkdir -p $(ROMFSDIR)/$$i; \
	done
	for dir in $(DIRS) ; do [ ! -d $$dir ] || $(MAKEARCH) -C $$dir romfs || exit 1 ; done
	-find $(ROMFSDIR)/. -name CVS | xargs -r rm -rf

.PHONY: image
image:
	[ -d $(IMAGEDIR) ] || mkdir $(IMAGEDIR)
	$(MAKEARCH) -C $(VENDDIR) image

.PHONY: package
package:
	mkdir -p $(IPKGDIR)/ipkg_arm-linux $(IPKGDIR)/ipkg_xscale-linux
	for dir in lib user ; do [ ! -d $$dir ] || $(MAKEARCH) -C $$dir package || exit 1 ; done
	(cd $(IPKGDIR)/ipkg_arm-linux; $(IPKGMAKEINDEX) . > ./Packages )
	(cd $(IPKGDIR)/ipkg_xscale-linux; $(IPKGMAKEINDEX) . > ./Packages )

.PHONY: netflash
netflash netflash_only:
	make -C prop/mstools CONFIG_PROP_MSTOOLS_NETFLASH_NETFLASH=y

%_fullrelease:
	@echo "This target no longer works"
	@echo "Do a make -C release $@"
	exit 1
#
# fancy target that allows a vendor to have other top level
# make targets,  for example "make vendor_flash" will run the
# vendor_flash target in the vendors directory
#

vendor_%:
	$(MAKEARCH) -C $(VENDDIR) $@

.PHONY: linux
linux linux%_only:
	### Comment out by Jared 02-02-2011 ###
	#@if [ $(LINUXDIR) != linux-2.5.x -a $(LINUXDIR) != linux-2.6.x -a ! -f $(LINUXDIR)/.depend ] ; then \
	#	echo "ERROR: you need to do a 'make dep' first" ; \
	#	exit 1 ; \
	#fi
	### Add by Jared 06-26-2009 loading the customized kernel configure ###
	#if [ -f vendors/$(CONFIG_VENDOR)/$(CONFIG_PRODUCT)/config.$(LINUXDIR) ]; then \
	if [ ! -f $(LINUXDIR)/.config ]; then \
		@echo "update $(LINUXDIR)/.config"; \
		cp -a vendors/$(CONFIG_VENDOR)/$(CONFIG_PRODUCT)/config.$(LINUXDIR) $(LINUXDIR)/.config; \
	fi 
	### Add by Jared 06-26-2009 loading the customized kernel configure ###
	$(MAKEARCH_KERNEL) -C $(LINUXDIR) $(LINUXTARGET) || exit 1
	if [ -f $(LINUXDIR)/vmlinux ]; then \
		ln -f $(LINUXDIR)/vmlinux $(LINUXDIR)/linux ; \
	fi


.PHONY: subdirs
#subdirs: linux
subdirs:
	for dir in $(DIRS) ; do [ ! -d $$dir ] || $(MAKEARCH_KERNEL) -C $$dir || exit 1 ; done

.PHONY: include
include:
	$(MAKEARCH_KERNEL) -C include || exit 1

.PHONY: lib
lib:
	# add by Jared 06-12-2009 for creating sub directories in TC_INSTALL_PREFIX
	for i in $(INSTALL_ROOT_DIRS); do \
		[ -d $(TC_INSTALL_PREFIX)/$$i ] || mkdir -p $(TC_INSTALL_PREFIX)/$$i; \
	done
	$(MAKEARCH_KERNEL) -C lib || exit 1

lib-rebuild:
	$(MAKEARCH_KERNEL) -C build || exit 1

.PHONY: user
user:
	$(MAKEARCH_KERNEL) -C user || exit 1

dep:
	@if [ ! -f $(LINUXDIR)/.config ] ; then \
		echo "ERROR: you need to do a 'make config' first" ; \
		exit 1 ; \
	fi
	$(MAKEARCH_KERNEL) -C $(LINUXDIR) dep

# This one removes all executables from the tree and forces their relinking
.PHONY: relink
relink:
	find user -name '*.gdb' | sed 's/^\(.*\)\.gdb/\1 \1.gdb/' | xargs rm -f
	find prop -name '*.gdb' | sed 's/^\(.*\)\.gdb/\1 \1.gdb/' | xargs rm -f
	find $(VENDDIR) -name '*.gdb' | sed 's/^\(.*\)\.gdb/\1 \1.gdb/' | xargs rm -f

clean: modules_clean
	for dir in $(LINUXDIR) $(DIRS); do [ ! -d $$dir ] || $(MAKEARCH) -C $$dir clean ; done
	rm -rf $(ROMFSDIR)/*
	rm -f $(IMAGEDIR)/*
	rm -f config.tk
	rm -f $(LINUXDIR)/linux
	rm -rf $(LINUXDIR)/net/ipsec/alg/libaes $(LINUXDIR)/net/ipsec/alg/perlasm
	# Add by Jared 06-30-2009
	rm -rf $(IPKGDIR)/ipkg_arm-linux
	rm -rf $(IPKGDIR)/ipkg_xscale-linux
#	cd test && make clean && cd ..
#	cd host && make clean && cd ..
#	cd moxalib && make clean && cd ..

real_clean mrproper: clean
	-$(MAKEARCH_KERNEL) -C $(LINUXDIR) mrproper
	-$(MAKEARCH) -C config clean
	rm -rf romfs config.in config.arch config.tk images
	rm -f modules/config.tk
	rm -rf .config .config.old .oldconfig autoconf.h

distclean: mrproper
	-$(MAKEARCH_KERNEL) -C $(LINUXDIR) distclean
	-rm -f user/tinylogin/applet_source_list user/tinylogin/config.h

%_only %_romfs %_debianfs %_package %_menuconfig %_clean:
	@if [ "$(dir $@)" = "./" ]; then \
		[ ! -d "$(word 1,$(subst _, ,$@))" ] || \
			$(MAKEARCH) -C $(word 1,$(subst _, ,$@)) \
			$(word 2,$(subst _, ,$@)); \
	else \
		[ ! -d "$(dir $@)" ] || (cd $(dir $@) && $(MAKEARCH) $(notdir $@)) \
	fi

config_error:
	@echo "*************************************************"
	@echo "You have not run make config."
	@echo "The build sequence for this source tree is:"
	@echo "1. 'make config' or 'make xconfig'"
	@echo "2. 'make dep'"
	@echo "3. 'make'"
	@echo "*************************************************"
	@exit 1

prune:
	$(MAKE) -C user prune

dist-prep:
	-find $(ROOTDIR) -name 'Makefile*.bin' | while read t; do \
		$(MAKEARCH) -C `dirname $$t` -f `basename $$t` $@; \
	 done

############################################################################
