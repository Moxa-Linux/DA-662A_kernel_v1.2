# History:
# Date		Author			Comment
# 07-12-2009	Jared Wu.		install at BERoot/LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/libnet-1.1.5_only

all: $(TC_INSTALL_PREFIX)/lib/libnet.so.1.6.0

$(TC_INSTALL_PREFIX)/lib/libnet.so.1.6.0: build_$(CONFIGURE_TARGET)-linux/build
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install

build_$(CONFIGURE_TARGET)-linux/build: $(TC_INSTALL_PREFIX)/lib/libnet.so.1.6.0
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/build: makefile
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf libnet-1.1.5.tar.gz
	mv libnet-1.1.5 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	sh ./configure \
	CC=$(CROSS)gcc \
	CXX=$(CROSS)g++ \
	LD=$(CROSS)ld \
	CPP=$(CROSS)cpp \
	STRIP=$(CROSS)strip \
	--host=$(CONFIGURE_TARGET)-linux \
	--build=i686 \
	--prefix="$(TC_INSTALL_PREFIX)" \
	)
	touch build_$(CONFIGURE_TARGET)-linux/build

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux;

romfs:
	$(STRIP) $(TC_INSTALL_PREFIX)/lib/libnet.so.1.6.0
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/lib/libnet.so.1.6.0 /lib/libnet.so.1.6.0
	$(ROMFSINST) -s libnet.so.1.6.0 /lib/libnet.so.1

package:
	# Make the ipkg package
#	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib
#	cp -a build_$(CONFIGURE_TARGET)-linux/libnet.so.1.6.0 ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/libnet.so.1.6.0
#	$(STRIP) -s ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/libnet.so.1.6.0
#	$(IPKGBUILD) ipkg_src_root_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

