# History:
# Date		Author		Comment
# 10-24-2012	Jared Wu.	First Create
# 12-31-2013	Jared Wu.	Add the romfs: installation for bluez dependent libraries
# 07-08-2016	Jared Wu.	Change the configured from --with-libiconv="gnu" to --with-libiconv="no"
# 				--enable-static 
#
# compile: 
#	make lib/libffi_only
#	make lib/glib2_only
#

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install-me-here
TARGET_ORG=glib2.0_2.33.12+really2.32.4
TARGET_SRC=glib-2.32.4

all: $(TC_INSTALL_PREFIX)/lib/libglib-2.0.so.0.3200.4
	#$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/glib/.libs/libglib-2.0.so.0.3200.4 \
	#build_$(CONFIGURE_TARGET)-linux/gio/.libs/libgio-2.0.so.0.3200.4 \
	#build_$(CONFIGURE_TARGET)-linux/gmodule/.libs/libgmodule-2.0.so.0.3200.4 \
	#build_$(CONFIGURE_TARGET)-linux/gobject/.libs/libgobject-2.0.so.0.3200.4 \
	#build_$(CONFIGURE_TARGET)-linux/gthread/.libs/libgthread-2.0.so.0.3200.4
	#scp build_$(CONFIGURE_TARGET)-linux/glib/.libs/libglib-2.0.so.0.3200.4 \
	#build_$(CONFIGURE_TARGET)-linux/gio/.libs/libgio-2.0.so.0.3200.4 \
	#build_$(CONFIGURE_TARGET)-linux/gmodule/.libs/libgmodule-2.0.so.0.3200.4 \
	#build_$(CONFIGURE_TARGET)-linux/gobject/.libs/libgobject-2.0.so.0.3200.4 \
	#build_$(CONFIGURE_TARGET)-linux/gthread/.libs/libgthread-2.0.so.0.3200.4 \
	#192.168.27.19:/lib/


untar-all-source:
	if [ ! -d $(TARGET_ORG) ]; then \
		[ ! -f $(TARGET_ORG).orig.tar.xz ] && wget http://ftp.de.debian.org/debian/pool/main/g/glib2.0/$(TARGET_ORG).orig.tar.xz; \
		[ -f $(TARGET_ORG).orig.tar.xz ] && tar Jxf $(TARGET_ORG).orig.tar.xz; \
	fi
	sed -i 's/as_fn_exit $$as_status/#as_fn_exit $$as_status/g' ./$(TARGET_SRC)/configure


build:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	mkdir -p build_$(CONFIGURE_TARGET)-linux
	(cd build_$(CONFIGURE_TARGET)-linux; \
	glib_cv_stack_grows=no \
	CC="$(CC)" \
	LD="$(LD)" \
	STRIP="$(STRIP)" \
	CXX="$(CXX)" \
	CPP="$(CPP)" \
	AS="$(AS)" \
	AR="$(AR)" \
	RAMLIB="$(RANLIB)" \
	CFLAGS="-DSSIZE_MAX=0x7fffffff -I$(TC_INSTALL_PREFIX)" \
	LDFLAGS="-L$(TC_LIBDIR)" \
	LIBFFI_LIBS="-L$(ROOTDIR)/lib/libffi/build_$(CONFIGURE_TARGET)-linux/install-me-here/lib -lffi" \
	LIBFFI_CFLAGS="-I$(ROOTDIR)/lib/libffi/build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/libffi-3.0.10/include" \
	../$(TARGET_SRC)/configure \
	--prefix="$(INSTALL_PREFIX)" \
	--host=arm-linux \
	--build=i686-pc-linux \
	--enable-static \
	--with-libiconv="no" \
	)
	# Skip to build with libelf
	sed -i 's/#define HAVE_LIBELF 1/\/\/#undef HAVE_LIBELF/' build_$(CONFIGURE_TARGET)-linux/config.h
	sed -i 's/-lelf//' build_$(CONFIGURE_TARGET)-linux/Makefile
	sed -i 's/-lelf//' build_$(CONFIGURE_TARGET)-linux/gio/Makefile
	# Skip to build the cross-compile program
	sed -i 's/tests po docs/#tests po docs/' build_$(CONFIGURE_TARGET)-linux/Makefile
	sed -i 's/SUBDIRS = . tests/SUBDIRS = .# tests/' build_$(CONFIGURE_TARGET)-linux/gobject/Makefile
	sed -i 's/$$(am__append_14) tests/$$(am__append_14) #tests/g' build_$(CONFIGURE_TARGET)-linux/gio/Makefile
	make -C build_$(CONFIGURE_TARGET)-linux
	make -C build_$(CONFIGURE_TARGET)-linux install


$(INSTALL_PREFIX)/lib/libglib-2.0.so.0.3200.4:
	make untar-all-source
	make build

$(TC_INSTALL_PREFIX)/lib/libglib-2.0.so.0.3200.4: $(INSTALL_PREFIX)/lib/libglib-2.0.so.0.3200.4
	make install


# install into toolchain
install:
	cp -a $(INSTALL_PREFIX)/include/* $(TC_INSTALL_PREFIX)/include
	cp -a $(INSTALL_PREFIX)/lib/* $(TC_INSTALL_PREFIX)/lib




# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/libglib-2.0.so.0.3200.4
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libglib*.so*" /lib
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libgio*.so*" /lib
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libgmodule*.so*" /lib
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libgobject*.so*" /lib
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libgthread*.so*" /lib

package:
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/lib
	cp -a $(TC_INSTALL_PREFIX)/lib/libglib-2.0.so.0.3200.4 ipkg_$(CONFIGURE_TARGET)-linux/lib/
	cp -a $(TC_INSTALL_PREFIX)/lib/libgio-2.0.so.0.3200.4 ipkg_$(CONFIGURE_TARGET)-linux/lib/
	cp -a $(TC_INSTALL_PREFIX)/lib/libgmodule-2.0.so.0.3200.4 ipkg_$(CONFIGURE_TARGET)-linux/lib/
	cp -a $(TC_INSTALL_PREFIX)/lib/libgobject-2.0.so.0.3200.4 ipkg_$(CONFIGURE_TARGET)-linux/lib/
	cp -a $(TC_INSTALL_PREFIX)/lib/libgthread-2.0.so.0.3200.4 ipkg_$(CONFIGURE_TARGET)-linux/lib/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/lib/*
	(cd ipkg_$(CONFIGURE_TARGET)-linux/lib ; \
	ln -sf libglib-2.0.so.0.3200.4 libglib-2.0.so; \
	ln -sf libglib-2.0.so.0.3200.4 libglib-2.0.so.0; \
	ln -sf libgio-2.0.so.0.3200.4 libgio-2.0.so.0; \
	ln -sf libgio-2.0.so.0.3200.4 libgio-2.0.so; \
	ln -sf libgmodule-2.0.so.0.3200.4 libgmodule-2.0.so.0; \
	ln -sf libgmodule-2.0.so.0.3200.4 libgmodule-2.0.so; \
	ln -sf libgobject-2.0.so.0.3200.4 libgobject-2.0.so.0; \
	ln -sf libgobject-2.0.so.0.3200.4 libgobject-2.0.so; \
	ln -sf libgthread-2.0.so.0.3200.4 libgthread-2.0.so.0; \
	ln -sf libgthread-2.0.so.0.3200.4 libgthread-2.0.so; \
	)
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


# copy from toolchain to romfs
tc2romfs:
	$(ROMFSINST) "$(TC_LIBDIR)/libglib*.so*" /lib


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/libglib-2.0.so.0.3200.4
	cp -a $(INSTALL_PREFIX)/include/*  $(TC_INSTALL_PREFIX)/include
	cp -a $(INSTALL_PREFIX)/lib/*.so*  $(TC_INSTALL_PREFIX)/lib
	cp -a $(INSTALL_PREFIX)/lib/*.a  $(TC_INSTALL_PREFIX)/lib


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux \
	ipkg_$(CONFIGURE_TARGET)-linux/lib \
	$(TARGET_SRC)

