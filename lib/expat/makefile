# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/expat_only
#

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install-me-here
TARGET_SRC=expat-2.0.1

all: $(INSTALL_PREFIX)/lib/libexpat.so.1.5.2

untar-all-source:
	if [ ! -d $(TARGET_SRC) ]; then \
		[ ! -f $(TARGET_SRC).tar.bz2 ] && wget http://www.stats.ox.ac.uk/pub/Rtools/goodies/sources/$(TARGET_SRC).tar.bz2; \
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
	NM=$(CROSS)nm \
	AR=$(CROSS)ar \
	RANLIB=$(CROSS)ranlib \
	../$(TARGET_SRC)/configure \
	--prefix="$(INSTALL_PREFIX)" \
	--host=arm-linux \
	--build=i686-pc-linux \
	--enable-shared \
	)
	make -C build_$(CONFIGURE_TARGET)-linux
	make -C build_$(CONFIGURE_TARGET)-linux install

$(INSTALL_PREFIX)/lib/libexpat.so.1.5.2:
	make untar-all-source
	make build

# install into toolchain
install:
	cp -a build_$(CONFIGURE_TARGET)-linux/.libs/libexpat.so* $(TC_INSTALL_PREFIX)/lib/
	cp -a build_$(CONFIGURE_TARGET)-linux/xmlwf/.libs/xmlwf $(TC_INSTALL_PREFIX)/bin/
	cp -a $(TARGET_SRC)/lib/expat.h $(TC_INSTALL_PREFIX)/include/
	cp -a $(TARGET_SRC)/lib/expat_external.h $(TC_INSTALL_PREFIX)/include/

# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/libexpat.so.1.5.2
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/lib/libexpat.so.1.5.2 /lib
	$(ROMFSINST) -s libexpat.so.1.5.2 /lib/libexpat.so.1

package:
	echo "Nothing to do"


# copy from toolchain to romfs
tc2romfs:
	$(ROMFSINST) "$(TC_LIBDIR)/libexpat.so*" /lib

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf $(TARGET_SRC)
