# History:
# Date		Author			Comment
# 09-21-2011	Jared Wu.		install at BERoot/LERoot
# 12-29-2011    Aeluin Chen.    do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# Pre-requirement:
#	ncurses ( has support in toolchain arm-linux-gcc v.4.4.2 )
#
# compile: 
#	cd sdlinux
#	make user/gpsd_only
#
# reference:
# 	cgps.c is a client modified by the original one, add "file flag" to 
# 	write gps information into a file

all: build_$(CONFIGURE_TARGET)-linux/.libs/gpsd
	echo "Build Complete!"

build_$(CONFIGURE_TARGET)-linux/.libs/gpsd:build_$(CONFIGURE_TARGET)-linux/Makefile
	make -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/Makefile: build_$(CONFIGURE_TARGET)-linux/patched
	(cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	CC="$(CC)" \
	CXX="$(CXX)" \
	CPP="$(CPP)" \
	LD="$(LD)" \
	CFLAGS="-I$(TC_INSTALL_PREFIX)/include/ncurses/" \
	LDFLAGS="-L$(TC_LIBDIR)" \
	./configure \
	--build=i686-linux \
	--host=$(CONFIGURE_TARGET)-linux \
	--with-cc="$(CC)" \
	--with-ar="$(AR)" \
	--disable-libQgpsmm \
	)

build_$(CONFIGURE_TARGET)-linux/patched:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf gpsd_2.95.orig.tar.gz
	mv gpsd-2.95 build_$(CONFIGURE_TARGET)-linux
	### patch for signal issue ###
	(cd build_$(CONFIGURE_TARGET)-linux; patch -p1 < ../gpsd_2.95-15.diff)
	#(cd build_$(CONFIGURE_TARGET)-linux; patch -p1 < ../gpsd_2.95-13.1.diff)
	touch build_$(CONFIGURE_TARGET)-linux/patched

romfs:
	$(ROMFSINST) -n dist/etc/init.d /etc/init.d
	$(ROMFSINST) -e CONFIG_USER_GPSD_START -n dist/etc/rc.d /etc/rc.d
	$(STRIP) build_$(CONFIGURE_TARGET)-linux/.libs/cgps \
	build_$(CONFIGURE_TARGET)-linux/.libs/gpsctl \
	build_$(CONFIGURE_TARGET)-linux/.libs/gpsd \
	build_$(CONFIGURE_TARGET)-linux/.libs/gpsdecode \
	build_$(CONFIGURE_TARGET)-linux/.libs/gpsmon \
	build_$(CONFIGURE_TARGET)-linux/.libs/gpspipe \
	build_$(CONFIGURE_TARGET)-linux/.libs/gpxlogger \
	build_$(CONFIGURE_TARGET)-linux/.libs/lcdgps \
	build_$(CONFIGURE_TARGET)-linux/.libs/libgps.so.19.0.0 \
	build_$(CONFIGURE_TARGET)-linux/.libs/libgpsd.so.0.0.0
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/.libs/cgps /usr/sbin
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/.libs/gpsctl /usr/sbin
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/.libs/gpsd /usr/sbin
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/.libs/gpsdecode /usr/sbin
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/.libs/gpsmon /usr/sbin
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/.libs/gpspipe /usr/sbin
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/.libs/gpxlogger /usr/sbin
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/.libs/lcdgps /usr/sbin
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/.libs/libgps.so.19.0.0 /lib
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/.libs/libgpsd.so.0.0.0 /lib
	( cd $(ROMFSDIR)/lib; \
	ln -sf libgps.so.19.0.0 libgps.so.19; \
	ln -sf libgpsd.so.0.0.0 libgpsd.so.0; \
	)

package:
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/lib
	cp -a build_$(CONFIGURE_TARGET)-linux/.libs/cgps ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin/
	cp -a build_$(CONFIGURE_TARGET)-linux/.libs/gpsctl ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin/
	cp -a build_$(CONFIGURE_TARGET)-linux/.libs/gpsd ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin/
	cp -a build_$(CONFIGURE_TARGET)-linux/.libs/gpsdecode ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin/
	cp -a build_$(CONFIGURE_TARGET)-linux/.libs/gpsmon ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin/
	cp -a build_$(CONFIGURE_TARGET)-linux/.libs/gpspipe ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin/
	cp -a build_$(CONFIGURE_TARGET)-linux/.libs/gpxlogger ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin/
	cp -a build_$(CONFIGURE_TARGET)-linux/.libs/lcdgps ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin/
	cp -a build_$(CONFIGURE_TARGET)-linux/.libs/libgps.so.19.0.0 ipkg_$(CONFIGURE_TARGET)-linux/lib/
	cp -a build_$(CONFIGURE_TARGET)-linux/.libs/libgpsd.so.0.0.0 ipkg_$(CONFIGURE_TARGET)-linux/lib/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin/* \
	ipkg_$(CONFIGURE_TARGET)-linux/lib/* 
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	( cd ipkg_$(CONFIGURE_TARGET)-linux/lib; \
	ln -sf libgps.so.19.0.0 libgps.so.19; \
	ln -sf libgpsd.so.0.0.0 libgpsd.so.0; \
	)
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

clean distclean: 
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf ipkg_gpsd_$(CONFIGURE_TARGET)-linux/usr/
	rm -rf ipkg_gpsd_$(CONFIGURE_TARGET)-linux/lib

