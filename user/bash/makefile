# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
# 10-22-2014	Aeluin Chen.	apply patches 1-57
#
# compile: 
#	make lib/bash_only
#

TARGET_VER=3.2
TARGET_PRJ=bash
TARGET_SRC=$(TARGET_PRJ)-$(TARGET_VER)
TARGET_SRC_DIR=target_src_dir

CONFOPTS = CC=$(CROSS)gcc \
	LD=$(CROSS)ld \
	CXX=$(CROSS)g++ \
	CPP=$(CROSS)cpp \
	STRIP=$(CROSS)strip \
	GMSGFMT=$(CROSS)msgfmt \
	--host=$(CONFIGURE_TARGET)-linux \
	--build=i686 \
	--prefix=$(TC_INSTALL_PREFIX) \

all: $(TC_INSTALL_PREFIX)/bin/bash


download-patches:
	mkdir -p patches-official
	-@count=0; until [ $$? -ne 0 ]; do \
		count=`expr $$count + 1`; \
		num=`echo $$count|awk '{printf "%03d", $$1}'`; \
		wget -q -nc http://ftp.gnu.org/gnu/$(TARGET_PRJ)/$(TARGET_SRC)-patches/$(TARGET_PRJ)$(subst .,,$(TARGET_VER))-$$num -P patches-official; \
	done;

patch-source:
	for patch in $$(ls patches-official); do \
		patch -N -Z -p0 -i patches-official/$$patch; \
		if [ $$? -gt 1 ]; then exit 1; fi; \
	done;


$(TC_INSTALL_PREFIX)/bin/bash: build_$(CONFIGURE_TARGET)-linux/bash
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install


build_$(CONFIGURE_TARGET)-linux/bash: build_$(CONFIGURE_TARGET)-linux/build
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/build: makefile
	rm -rf build_$(CONFIGURE_TARGET)-linux
	mkdir build_$(CONFIGURE_TARGET)-linux
	touch parse.y
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	sh ../configure $(CONFOPTS); \
	sed -ie 's/strip $(Program)/$(STRIP) $(Program)/g' Makefile; \
	sed -ie 's/install: install-exec install-data/install: install-exec #install-data/g' po/Makefile; \
	)
	touch build_$(CONFIGURE_TARGET)-linux/build

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux;

romfs: #$(TC_INSTALL_PREFIX)/bin/bash
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/bash /bin/bash
	$(ROMFSINST) -s /bin/bash /bin/sh
	$(STRIP) $(ROMFSDIR)/bin/bash

package: #$(TC_INSTALL_PREFIX)/bin/bash
	rm -rf ipkg_$(TARGET_PRJ)_$(CONFIGURE_TARGET)-linux
	cp -a ipkg_default ipkg_$(TARGET_PRJ)_$(CONFIGURE_TARGET)-linux
	mkdir -p ipkg_$(TARGET_PRJ)_$(CONFIGURE_TARGET)-linux/bin
	cp -a $(TC_INSTALL_PREFIX)/bin/bash ipkg_$(TARGET_PRJ)_$(CONFIGURE_TARGET)-linux/bin
	$(STRIP) ipkg_$(TARGET_PRJ)_$(CONFIGURE_TARGET)-linux/bin/bash
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(TARGET_PRJ)_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
