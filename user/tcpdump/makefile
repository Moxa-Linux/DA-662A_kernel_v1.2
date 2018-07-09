# History:
# Date		Author			Comment
# 07-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/tcpdump_only

all: $(TC_INSTALL_PREFIX)/sbin/tcpdump

$(TC_INSTALL_PREFIX)/sbin/tcpdump: build_$(CONFIGURE_TARGET)-linux/tcpdump
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install

build_$(CONFIGURE_TARGET)-linux/tcpdump: build_$(CONFIGURE_TARGET)-linux/build
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/build: makefile
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf tcpdump-3.9.8.tar.gz
	mv tcpdump-3.9.8 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	ac_cv_linux_vers=2 \
	sh ./configure \
	CC=$(CROSS)gcc \
	LD=$(CROSS)ld \
	CPP=$(CROSS)cpp \
	STRIP=$(CROSS)strip \
	LDFLAGS="-L$(TC_INSTALL_PREFIX)/lib" \
	CFLAGS="-I$(TC_INSTALL_PREFIX)/include" \
	--host=$(CONFIGURE_TARGET) \
	--build=i686 \
	--without-crypto \
	--disable-smb \
	--disable-ipv6 \
	--prefix="$(TC_INSTALL_PREFIX)" \
	)
	#touch build_$(CONFIGURE_TARGET)-linux/build

	#sed -i 's/ac_cv_linux_vers=unknown/ac_cv_linux_vers=2/g' configure.in; \
	#autoconf; \

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux;

romfs: #$(TC_INSTALL_PREFIX)/sbin/tcpdump
#	$(STRIP) build_$(CONFIGURE_TARGET)-linux/tcpdump
#	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/tcpdump /usr/local/bin/tcpdump

package: $(TC_INSTALL_PREFIX)/sbin/tcpdump
	# Make the ipkg package
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/bin
	cp -a $(TC_INSTALL_PREFIX)/sbin/tcpdump ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	$(STRIP) -s ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/bin/tcpdump
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_src_root_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


