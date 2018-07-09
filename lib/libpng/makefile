

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install-me-here
TARGET_SRC=libpng-1.4.0


all: $(INSTALL_PREFIX)/lib/libpng.a

untar-all-source:
	if [ ! -d $(TARGET_SRC) ]; then \
		[ ! -f $(TARGET_SRC).tar.bz2 ] && \
			wget http://ftp.heanet.ie/mirrors/sourceforge/l/project/li/libpng/libpng14/older-releases/1.4.0/$(TARGET_SRC).tar.bz2; \
		[ -f $(TARGET_SRC).tar.bz2 ] && tar xjf $(TARGET_SRC).tar.bz2; \
	fi

build:
	if [ -d "build_$(CONFIGURE_TARGET)-linux" ]; then \
		rm -rf build_$(CONFIGURE_TARGET)-linux; \
	fi
	mkdir -p build_$(CONFIGURE_TARGET)-linux
	(cd build_$(CONFIGURE_TARGET)-linux; \
	CC=$(CROSS)gcc \
	LD=$(CROSS)gcc \
	LDFLAGS="-L$(TC_INSTALL_PREFIX)" \
	STRIP=$(CROSS)strip \
	CXX=$(CROSS)g++ \
	CPP="$(CROSS)gcc -E" \
	AS=$(CROSS)as \
	NM=$(CROSS)nm \
	AR=$(CROSS)ar \
	RANLIB=$(CROSS)ranlib \
	../$(TARGET_SRC)/configure \
	--prefix="$(INSTALL_PREFIX)" \
	--host=arm-linux \
	--build=i686-pc-linux \
	--enable-shared \
	--with-libpng=$(TC_INSTALL_PREFIX) \
	)
	make -C build_$(CONFIGURE_TARGET)-linux
	make -C build_$(CONFIGURE_TARGET)-linux install


$(INSTALL_PREFIX)/lib/libpng.a:
	make untar-all-source
	make build

$(TC_INSTALL_PREFIX)/lib/libpng.a: $(INSTALL_PREFIX)/lib/libpng.a
	make install


# install into toolchain
install:
	cp -R $(INSTALL_PREFIX)/* $(TC_INSTALL_PREFIX)


# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/libpng.a
	#$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libpng*.a*" /lib

package:
	echo "Nothing to do"

# copy from toolchain to romfs
tc2romfs:
	#$(ROMFSINST) "$(TC_LIBDIR)/libpng*.a" /lib


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/libpng.a
	cp -a $(INSTALL_PREFIX)/include/*  $(TC_INSTALL_PREFIX)/include
	cp -a $(INSTALL_PREFIX)/lib/*.a  $(TC_INSTALL_PREFIX)/lib


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf $(TARGET_SRC)

