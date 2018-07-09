# History:
# Date		Author		Comment
# 07-17-2009    Jared Wu.	Build from source
#	INSTALL_ROOT= $(ROOTDIR)/BERoot or $(ROOTDIR)/LERoot
#
# compile: 
# make CROSS=xscale-linux-
# make CROSS=arm-linux-
# History:
# Date		Author			Comment
# 07-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/iperf_only

all: build_$(CONFIGURE_TARGET)-linux
#	make -f Makefile install

build_$(CONFIGURE_TARGET)-linux: build_$(CONFIGURE_TARGET)-linux/Makefile
	unset LDFLAGS CC CFLAGS CPPFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/Makefile: makefile
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf iperf-2.0.5.tar.gz
	mv iperf-2.0.5 build_$(CONFIGURE_TARGET)-linux;
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	CC=$(CROSS)gcc \
	CXX=$(CROSS)g++ \
	CPP=$(CROSS)cpp \
	AR=$(CROSS)ar \
	RANLIB=$(CROSS)ranlib \
	LDLAGS="-static" \
	LD=$(CROSS)ld \
	ac_cv_func_malloc_0_nonnull=yes \
	./configure \
	--build=i686-linux          \
	--host=$(CONFIGURE_TARGET)-linux  \
	)

romfs:
	#$(STRIP) -s build_$(CONFIGURE_TARGET)/src/iperf
	#$(ROMFSINST) build_$(CONFIGURE_TARGET)/src/iperf

package:
	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/src/iperf
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/bin
	cp -a build_$(CONFIGURE_TARGET)-linux/src/iperf ipkg_$(CONFIGURE_TARGET)-linux/bin/
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux \
	ipkg_$(CONFIGURE_TARGET)-linux/bin
