# History:
# Date		Author		Comment
# 10-24-2012	Jared Wu.	First create
#
# compile: 
#	make lib/libiconv_only
#	make lib/libffi_only
#	make lib/glib2_only
#	make user/wpa_supplicant-0.7.3_only
#	make user/wimax-service_only
#


all: $(TC_INSTALL_PREFIX)/bin/wimaxcu

$(TC_INSTALL_PREFIX)/bin/wimaxcu: build_$(CONFIGURE_TARGET)-linux/InfraStack/OSDependent/Linux/wimaxcu/.libs/wimaxcu
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install

build_$(CONFIGURE_TARGET)-linux/InfraStack/OSDependent/Linux/wimaxcu/.libs/wimaxcu: build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf wimax-1.5.1.tar.gz
	mv wimax-1.5.1 build_$(CONFIGURE_TARGET)-linux
	sed -i '11614s/{ (exit 1); exit 1; };//' build_$(CONFIGURE_TARGET)-linux/configure
	sed -i '11638s/{ (exit 1); exit 1; };//' build_$(CONFIGURE_TARGET)-linux/configure
	sed -i 's/typedef unsigned char u8;/\/\/typedef unsigned char u8;/' build_$(CONFIGURE_TARGET)-linux/InfraStack/OSAgnostic/WiMax/Wrappers/Supplicant/wmxSDK_Sup_Impl.c
	sed -i 's/typedef int pid_t/\/\/typedef int pid_t/' build_$(CONFIGURE_TARGET)-linux/InfraStack/OSDependent/Linux/OSAL/Primitives/wimax_osal_basictypes.h
	sed -i 'a #include <dirent.h>' build_$(CONFIGURE_TARGET)-linux/InfraStack/OSDependent/Linux/wimaxcu/wimaxcu.h
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS LIBTOOL AM_LIBTOOLFLAGS AM_CFLAGS LIBTOOLFLAGS && \
	sh ./configure \
	CC="$(CC)" \
	LD="$(LD)" \
	CPP="$(CPP)" \
	NM="$(NM)" \
	RANLIB="$(RANLIB)" \
	LIBWIMAX_LIBS="-L$(TC_LIBDIR) -lwimaxll" \
	LIBWIMAX_CFLAGS="-I$(TC_INSTALL_PREFIX)/include" \
	LIBEAP0_LIBS="-L$(TC_INSTALL_PREFIX)/usr/lib -leap" \
	LIBEAP0_CFLAGS="-I$(TC_INSTALL_PREFIX)/usr/include" \
	LDFLAGS="-liconv" \
	--host=arm-unknown-linux \
	--build=i686 \
	--prefix="$(TC_INSTALL_PREFIX)" \
	--with-linux="$(ROOTDIR)/$(LINUXDIR)" \
	--with-libwimaxll="$(TC_LIBDIR)" \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux \
	  ipkg_$(CONFIGURE_TARGET)-linux/bin \
	  ipkg_$(CONFIGURE_TARGET)-linux/lib


romfs: #$(TC_INSTALL_PREFIX)/bin/wimaxcu
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/wimaxcu /bin/wimaxcu
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/wimaxd /bin/wimaxd
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/wimax_monitor /bin/wimax_monitor
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/usr/lib/libeap.so.0.0.0 /lib/libeap.so.0
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/lib/libiWmxSdk.so.0.0.0 /lib/libiWmxSdk.so.0
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/lib/libWmxInstrument.so.0.0.0 /lib/libWmxInstrument.so.0
	$(STRIP) $(ROMFSDIR)/bin/wimaxcu \
	  $(ROMFSDIR)/bin/wimaxd \
	  $(ROMFSDIR)/bin/wimax_monitor \
	  $(ROMFSDIR)/lib/libeap.so.0 \
	  $(ROMFSDIR)/lib/libiWmxSdk.so.0 \
	  $(ROMFSDIR)/lib/libWmxInstrument.so.0


package: $(TC_INSTALL_PREFIX)/bin/wimaxcu
	# Make the ipkg package
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/bin \
	  ipkg_$(CONFIGURE_TARGET)-linux/lib
	cp -a $(TC_INSTALL_PREFIX)/bin/wimaxcu ipkg_$(CONFIGURE_TARGET)-linux/bin/
	cp -a $(TC_INSTALL_PREFIX)/bin/wimax_monitor ipkg_$(CONFIGURE_TARGET)-linux/bin/
	cp -a $(TC_INSTALL_PREFIX)/bin/wimaxd ipkg_$(CONFIGURE_TARGET)-linux/bin/
	cp -a $(TC_INSTALL_PREFIX)/usr/lib/libeap.so.0.0.0 ipkg_$(CONFIGURE_TARGET)-linux/lib/libeap.so.0
	cp -a $(TC_INSTALL_PREFIX)/lib/libiWmxSdk.so.0.0.0 ipkg_$(CONFIGURE_TARGET)-linux/lib/libiWmxSdk.so.0
	cp -a $(TC_INSTALL_PREFIX)/lib/libWmxInstrument.so.0.0.0 ipkg_$(CONFIGURE_TARGET)-linux/lib/libWmxInstrument.so.0
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/bin/* \
 	  ipkg_$(CONFIGURE_TARGET)-linux/lib/*
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


