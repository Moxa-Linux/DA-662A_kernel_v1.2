# History:
# Date		Author			Comment
# 06-23-2014	Jared Wu.		First cretaed
#
# compile: 
#	make user/pcsc-lite_only

all: build_$(CONFIGURE_TARGET)-linux/src/.libs/libpcsclite.so.1.0.0

build_$(CONFIGURE_TARGET)-linux/src/.libs/libpcsclite.so.1.0.0: build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf pcsc-lite-1.8.4.tar.bz2
	mv pcsc-lite-1.8.4 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	CC="$(CC)" \
	LD="$(LD)" \
	CFLAGS="-I$(TC_INSTALL_PREFIX)/include" \
	LDFLAGS="-L$(TC_INSTALL_PREFIX)/lib -lpthread" \
	LIBUDEV_CFLAGS="-I$(ROOTDIR)/lib/udev/build_$(CONFIGURE_TARGET)-linux/libudev/" \
	LIBUDEV_LIBS="-L$(ROOTDIR)/lib/udev/build_$(CONFIGURE_TARGET)-linux/libudev/.libs/ -ludev" \
	LIBUSB_CFLAGS="-I$(ROOTDIR)/lib/libusb/build_$(CONFIGURE_TARGET)-linux/include" \
	LIBUSB_LIBS="-L$(ROOTDIR)/lib/libusb/build_$(CONFIGURE_TARGET)-linux/lib" \
	sh ./configure \
	--host=$(CONFIGURE_TARGET)-linux \
	--build=i686-pc-linux-gnu \
	--prefix="$(TC_INSTALL_PREFIX)" \
	--enable-embedded \
	--enable-shared \
	--with-pci \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux

romfs:
	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/src/.libs/libpcsclite.so.1.0.0 \
	build_$(CONFIGURE_TARGET)-linux/src/spy/.libs/libpcscspy.so.0.0.0
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/.libs/libpcsclite.so.1.0.0 /lib
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/spy/.libs/libpcscspy.so.0.0.0 /lib
	$(ROMFSINST) -s /lib/libpcsclite.so.1.0.0 /lib/libpcsclite.so.1
	$(ROMFSINST) -s /lib/libpcsclite.so.1.0.0 /lib/libpcsclite.so
	$(ROMFSINST) -s /lib/libpcscspy.so.0.0.0 /lib/libpcscspy.so.0
	$(ROMFSINST) -s /lib/libpcscspy.so.0.0.0 /lib/libpcscspy.so

package:
	# Make the ipkg package
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/lib
	cp -a build_$(CONFIGURE_TARGET)-linux/src/.libs/libpcsclite.so.1.0.0 ipkg_$(CONFIGURE_TARGET)-linux/lib/
	(cd ipkg_$(CONFIGURE_TARGET)-linux/lib/; \
	ln -sf libpcsclite.so.1.0.0 libpcsclite.so.1; \
	ln -sf libpcsclite.so.1.0.0 libpcsclite.so; \
	)
	(cd ipkg_$(CONFIGURE_TARGET)-linux/lib/; \
	ln -sf libpcscspy.so.0.0.0 libpcscspy.so.0; \
	ln -sf libpcscspy.so.0.0.0 libpcscspy.so; \
	)
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/lib/libpcsclite.so.1.0.0 \
	ipkg_$(CONFIGURE_TARGET)-linux/lib/libpcscspy.so.0.0.0
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

