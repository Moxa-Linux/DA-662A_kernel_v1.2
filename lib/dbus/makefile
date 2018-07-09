# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/dbus_only
#

TARGET_SRC=dbus-1.4.0


all: $(TC_INSTALL_PREFIX)/lib/libdbus-1.a

untar-all-source:
	if [ ! -d $(TARGET_SRC) ]; then \
		[ ! -f $(TARGET_SRC).tar.gz ] && wget http://dbus.freedesktop.org/releases/dbus/$(TARGET_SRC).tar.gz; \
		[ -f $(TARGET_SRC).tar.gz ] && tar xzf $(TARGET_SRC).tar.gz; \
	fi

build:
	if [ -d "build_$(CONFIGURE_TARGET)-linux" ]; then \
		rm -rf build_$(CONFIGURE_TARGET)-linux; \
	fi
	mkdir -p build_$(CONFIGURE_TARGET)-linux
	(cd build_$(CONFIGURE_TARGET)-linux; \
	CC="$(CROSS)gcc -I$(TC_INSTALL_PREFIX)/include" \
	LD="$(CROSS)ld -L$(TC_INSTALL_PREFIX)/lib" \
	LDFLAGS="-L$(TC_INSTALL_PREFIX)/lib" \
	STRIP=$(CROSS)strip \
	CXX=$(CROSS)g++ \
	CPP="$(CROSS)gcc -E" \
	AS=$(CROSS)as \
	AR=$(CROSS)ar \
	../$(TARGET_SRC)/configure \
	--prefix="$(TC_INSTALL_PREFIX)" \
	--host=arm-linux \
	--build=i686-pc-linux \
	--enable-shared \
	--without-libtool \
	--without-x \
	--enable-inotify \
	--disable-dnotify \
	)
	make -C build_$(CONFIGURE_TARGET)-linux


$(TC_INSTALL_PREFIX)/lib/libdbus-1.a:
	make untar-all-source
	make build

# install into toolchain
install:
	make -C build_$(CONFIGURE_TARGET)-linux install
	#cp -R $(INSTALL_PREFIX)/* $(TC_INSTALL_PREFIX)

# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/libdbus*.a
	echo "Nothing to do"
	#$(ROMFSINST) $(TC_INSTALL_PREFIX)/lib/libdbus*.a /lib

package: $(TC_INSTALL_PREFIX)/lib/libdbus-1.so.3.4.0
	# Make the ipkg package
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin
	cp -a $(TC_INSTALL_PREFIX)/bin/dbus-send ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a $(TC_INSTALL_PREFIX)/bin/dbus-monitor ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a $(TC_INSTALL_PREFIX)/bin/dbus-launch ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a $(TC_INSTALL_PREFIX)/bin/dbus-cleanup-sockets ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a $(TC_INSTALL_PREFIX)/bin/dbus-uuidgen ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/*
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/local/libexec/
	cp -a $(TC_INSTALL_PREFIX)/libexec/dbus-daemon-launch-helper ipkg_$(CONFIGURE_TARGET)-linux/usr/local/libexec/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/usr/local/libexec/*
	cp -a $(TC_INSTALL_PREFIX)/lib/libdbus-1.so.3.4.0 ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib/libdbus-1.so.3.4.0
	(cd ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib; \
	ln -sf libdbus-1.so.3.4.0 libdbus-1.so.3; \
	ln -sf libdbus-1.so.3.4.0 libdbus-1.so; \
	)
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib/libdbus-1.so.3.4.0
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

# copy from toolchain to romfs
tc2romfs:
	#$(ROMFSINST) "$(TC_LIBDIR)/lib/libdbus*.a" /lib
	echo "Nothing to do"

clean:
	rm -rf build_$(CONFIGURE_TARGET)-linux; \

distclean: #clean
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf $(TARGET_SRC)

