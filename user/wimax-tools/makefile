# History:
# Date		Author		Comment
# 10-24-2012	Jared Wu.	First create
#
# compile: 
#	make lib/libnl_only
#	make lib/libffi_only
#	make lib/glib2_only
#	make user/wimax-tools_only
#


all: $(TC_INSTALL_PREFIX)/bin/wimaxll

$(TC_INSTALL_PREFIX)/bin/wimaxll: build_$(CONFIGURE_TARGET)-linux/bin/.libs/wimaxll
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install

build_$(CONFIGURE_TARGET)-linux/bin/.libs/wimaxll: build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf wimax-tools-1.4.4.tar.gz
	mv wimax-tools-1.4.4 build_$(CONFIGURE_TARGET)-linux
	sed -i '11605s/{ (exit 1); exit 1; };//' build_$(CONFIGURE_TARGET)-linux/configure
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS LIBTOOL AM_LIBTOOLFLAGS AM_CFLAGS LIBTOOLFLAGS && \
	sh ./configure \
	CC="$(CC)" \
	LD="$(LD)" \
	CPP="$(CPP)" \
	NM="$(NM)" \
	RANLIB="$(RANLIB)" \
	CFLAGS="-I$(TC_INSTALL_PREFIX)/include/glib-2.0/include/glib -I$(TC_INSTALL_PREFIX)/lib/glib-2.0/include" \
	LIBNL1_LIBS="-L$(TC_LIBDIR) -lnl" \
	LIBNL1_CFLAGS="-I$(TC_INSTALL_PREFIX)/include" \
	GLIB_LIBS="-L$(TC_LIBDIR) -lglib-2.0" \
	GLIB_CFLAGS="-I$(TC_INSTALL_PREFIX)/include/glib-2.0" \
	--host=arm-unknown-linux \
	--build=i686 \
	--prefix=$(TC_INSTALL_PREFIX) \
	--disable-doxygen-doc \
	--with-linux="$(ROOTDIR)/$(LINUXDIR)" \
	--with-libnl1="$(TC_INSTALL_PREFIX)" \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured
	(cd build_$(CONFIGURE_TARGET)-linux; patch -p1 <../wimax-tools.moxa-libtool.patch)


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux;
	rm -rf ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/wimaxll

romfs: #$(TC_INSTALL_PREFIX)/bin/wimaxll
	$(STRIP) build_$(CONFIGURE_TARGET)-linux/wimaxll
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/wimaxll /bin/wimaxll

package: $(TC_INSTALL_PREFIX)/bin/wimaxll
	# Make the ipkg package
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/bin
	cp -a $(TC_INSTALL_PREFIX)/bin/wimaxll ipkg_$(CONFIGURE_TARGET)-linux/bin/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/bin/wimaxll
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


