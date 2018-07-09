# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011    Aeluin Chen.    do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	cd stdlinux
#	make user/openvpn-2.1.1_only

all: $(TC_INSTALL_PREFIX)/sbin/openvpn


$(TC_INSTALL_PREFIX)/sbin/openvpn: build_$(CONFIGURE_TARGET)-linux/openvpn
	make -C build_$(CONFIGURE_TARGET)-linux install


build_$(CONFIGURE_TARGET)-linux/openvpn: build_$(CONFIGURE_TARGET)-linux/build
	make -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/build: makefile
	rm -rf build_$(CONFIGURE_TARGET)-linux; \
	mkdir build_$(CONFIGURE_TARGET)-linux; \
	( \
	cd build_$(CONFIGURE_TARGET)-linux; \
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	../configure CC=$(CROSS)gcc \
	--build=i686-pc-linux \
	--host=arm-linux \
	--prefix=$(TC_INSTALL_PREFIX) \
	--with-lzo-lib=$(TC_LIBDIR) \
	--with-lzo-headers=$(ROOTDIR)/lib/lzo/build_$(CONFIGURE_TARGET)-linux/include \
	)
	touch build_$(CONFIGURE_TARGET)-linux/build
	#--enable-shared
	#--with-ssl-lib=$(TC_INSTALL_PREFIX)/lib/libssl
	#--with-ssl-headers=$(ROOTDIR)/lib/libssl/include


romfs: $(TC_INSTALL_PREFIX)/sbin/openvpn
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/sbin/openvpn /sbin/openvpn
	$(STRIP) $(ROMFSDIR)/sbin/openvpn


package:
	echo "Nothing to do"

clean distclean:
	rm -fr build_$(CONFIGURE_TARGET)-linux
