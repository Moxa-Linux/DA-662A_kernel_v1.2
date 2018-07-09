# History:
# Date		Author			Comment
# 07-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/strace_only


all: $(TC_INSTALL_PREFIX)/bin/strace

$(TC_INSTALL_PREFIX)/bin/strace: build_$(CONFIGURE_TARGET)-linux/strace
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install-binPROGRAMS  #install

build_$(CONFIGURE_TARGET)-linux/strace: build_$(CONFIGURE_TARGET)-linux/build
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/build: makefile
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf strace-4.5.20.tar.bz2
	mv strace-4.5.20 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	sh ./configure \
	CC=$(CROSS)gcc \
	LD=$(CROSS)ld \
	CPP=$(CROSS)cpp \
	STRIP=$(CROSS)strip \
	--host=arm-unknown-linux \
	--build=i686 \
	--prefix=$(TC_INSTALL_PREFIX) \
	)
	#touch build_$(CONFIGURE_TARGET)-linux/build

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux;
	rm -rf ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/bin/strace

romfs: #$(TC_INSTALL_PREFIX)/bin/strace
#	$(STRIP) build_$(CONFIGURE_TARGET)-linux/strace
#	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/strace /usr/local/bin/strace

package: $(TC_INSTALL_PREFIX)/bin/strace
	# Make the ipkg package
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/bin
	cp -a $(TC_INSTALL_PREFIX)/bin/strace ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	$(STRIP) -s ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/bin/strace
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_src_root_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


