# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/libxml2_only
#

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install-me-here
TARGET_SRC=libxml2-2.7.6

all: $(INSTALL_PREFIX)/lib/libxml2.so

untar-all-source:
	if [ ! -d $(TARGET_SRC) ]; then \
		[ ! -f $(TARGET_SRC).tar.bz2 ] && wget http://ftp.nluug.nl/pub/os/Linux/distr/fatdog/source/l/$(TARGET_SRC).tar.bz2; \
		[ -f $(TARGET_SRC).tar.bz2 ] && tar xjf $(TARGET_SRC).tar.bz2; \
	fi


build:
	if [ -d "build_$(CONFIGURE_TARGET)-linux" ]; then \
		rm -rf build_$(CONFIGURE_TARGET)-linux; \
	fi
	mkdir -p build_$(CONFIGURE_TARGET)-linux
	(cd build_$(CONFIGURE_TARGET)-linux; \
	CC=$(CROSS)gcc \
	LD=$(CROSS)ld \
	STRIP=$(CROSS)strip \
	CXX=$(CROSS)g++ \
	CPP="$(CROSS)gcc -E" \
	AS=$(CROSS)as \
	AR=$(CROSS)ar \
	../$(TARGET_SRC)/configure \
	--prefix="$(INSTALL_PREFIX)" \
	--host=arm-linux \
	--build=i686-pc-linux \
	--enable-shared \
	--enable-ipv6=yes \
	)
	make -C build_$(CONFIGURE_TARGET)-linux
	make -C build_$(CONFIGURE_TARGET)-linux install


$(INSTALL_PREFIX)/lib/libxml2.so:
	make untar-all-source
	make build

$(TC_INSTALL_PREFIX)/lib/libxml2.so: $(INSTALL_PREFIX)/lib/libxml2.so
	make install


# install into toolchain
install:
	cp -R $(INSTALL_PREFIX)/* $(TC_INSTALL_PREFIX)


# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/libxml2.so
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libxml2*.so*" /lib

package:
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/lib
	cp -a $(TC_INSTALL_PREFIX)/lib/libxml2.so.2.7.6 ipkg_$(CONFIGURE_TARGET)-linux/lib/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/lib/libxml2.so.2.7.6
	(cd ipkg_$(CONFIGURE_TARGET)-linux/lib ; \
	ln -sf libxml2.so.2.7.6 libxml2.so; \
	ln -sf libxml2.so.2.7.6 libxml2.so.2; \
	)
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


# copy from toolchain to romfs
tc2romfs:
	$(ROMFSINST) "$(TC_LIBDIR)/libxml2*.so*" /lib


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/libxml2.so
	cp -a $(INSTALL_PREFIX)/include/*  $(TC_INSTALL_PREFIX)/include
	cp -a $(INSTALL_PREFIX)/lib/*.so*  $(TC_INSTALL_PREFIX)/lib
	cp -a $(INSTALL_PREFIX)/lib/*.a  $(TC_INSTALL_PREFIX)/lib


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux \
	ipkg_$(CONFIGURE_TARGET)-linux/lib \
	$(TARGET_SRC)

