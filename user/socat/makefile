# History:
# Date		Author			Comment
# 06-23-2014	Jared Wu.		First created
#
# compile: 
#	make user/socat_only

all:  build_$(CONFIGURE_TARGET)-linux/socat
	#$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install

build_$(CONFIGURE_TARGET)-linux/socat: build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf socat-1.7.1.3.tar.bz2
	mv socat-1.7.1.3 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	sc_cv_sys_crdly_shift=9 \
	sc_cv_sys_tabdly_shift=11 \
	sc_cv_sys_csize_shift=4 \
	ac_cv_ispeed_offset=13 \
	ac_ct_CC="$(CC)" \
	CC="$(CC)" \
	CPP="$(CPP)" \
	LD="$(LD)" \
	sh ./configure \
	--host=$(CONFIGURE_TARGET) \
	--build=i686 \
	--prefix="$(TC_INSTALL_PREFIX)" \
	--disable-readline \
	--disable-ext2 \
	--disable-sycls \
	--disable-retry \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured

	#sed -i 's/ac_cv_linux_vers=unknown/ac_cv_linux_vers=2/g' configure.in; \
	#autoconf; \

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux;

romfs:
	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/socat \
	build_$(CONFIGURE_TARGET)-linux/filan \
	build_$(CONFIGURE_TARGET)-linux/procan
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/socat /usr/bin
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/filan /usr/bin
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/procan /usr/bin

package:
	# Make the ipkg package
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/bin
	cp -a build_$(CONFIGURE_TARGET)-linux/socat \
	build_$(CONFIGURE_TARGET)-linux/filan \
	build_$(CONFIGURE_TARGET)-linux/procan \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/*
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


