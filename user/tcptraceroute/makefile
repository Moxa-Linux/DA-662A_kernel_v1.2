# History:
# Date		Author			Comment
# 07-12-2009	Jared Wu.		install at BERoot/LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/tcptraceroute_only

all: $(TC_INSTALL_PREFIX)/bin/tcptraceroute

$(TC_INSTALL_PREFIX)/bin/tcptraceroute: build_$(CONFIGURE_TARGET)-linux/build
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install

build_$(CONFIGURE_TARGET)-linux/build: $(TC_INSTALL_PREFIX)/bin/tcptraceroute
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/build: makefile
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf tcptraceroute-1.5.tar.gz
	mv tcptraceroute-1.5 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	patch -p1 <../tcptraceroute.diff; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	ac_cv_func_malloc_0_nonnull=yes \
	./configure \
	CC=$(CROSS)gcc \
	CXX=$(CROSS)g++ \
	LD=$(CROSS)ld \
	CPP=$(CROSS)cpp \
	STRIP=$(CROSS)strip \
	--host=$(CONFIGURE_TARGET)-linux \
	--build=i686 \
	--prefix="$(TC_INSTALL_PREFIX)" \
	--with-libnet="${TC_INSTALL_PREFIX}" \
	--with-libpcap="${TC_INSTALL_PREFIX}" \
	)
	touch build_$(CONFIGURE_TARGET)-linux/build

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux-linux;

romfs:
	$(STRIP) $(TC_INSTALL_PREFIX)/bin/tcptraceroute
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/tcptraceroute /bin/tcptraceroute

package:
	# Make the ipkg package
#	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib
#	cp -a build_$(CONFIGURE_TARGET)-linux/libnet.so.1.6.0 ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/bin/tcptraceroute
#	$(STRIP) -s ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/bin/tcptraceroute
#	$(IPKGBUILD) ipkg_src_root_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

