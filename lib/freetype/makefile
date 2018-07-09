# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/freetype_only
#

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install-me-here
TARGET_SRC=freetype-2.3.11

all: $(INSTALL_PREFIX)/lib/libfreetype.so

untar-all-source:
	if [ ! -d $(TARGET_SRC) ]; then \
		[ ! -f $(TARGET_SRC).tar.bz2 ] && wget http://ftp.twaren.net/Unix/NonGNU/freetype/$(TARGET_SRC).tar.bz2; \
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
	)
	make -C build_$(CONFIGURE_TARGET)-linux
	make -C build_$(CONFIGURE_TARGET)-linux install


$(INSTALL_PREFIX)/lib/libfreetype.so:
	make untar-all-source
	make build

$(TC_INSTALL_PREFIX)/lib/libfreetype.so: $(INSTALL_PREFIX)/lib/libfreetype.so
	make install


# install into toolchain
install:
	cp -R $(INSTALL_PREFIX)/* $(TC_INSTALL_PREFIX)


# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/libfreetype.so
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libfreetype.so*" /lib

package:
	echo "Nothing to do"


# copy from toolchain to romfs
tc2romfs: 
	$(ROMFSINST) "$(TC_LIBDIR)/libfreetype.so*" /lib


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/libfreetype.so
	cp -a $(INSTALL_PREFIX)/include/*  $(TC_INSTALL_PREFIX)/include
	cp -a $(INSTALL_PREFIX)/lib/libfreetype.a  $(TC_INSTALL_PREFIX)/lib
	cp -a $(INSTALL_PREFIX)/lib/libfreetype.so*  $(TC_INSTALL_PREFIX)/lib


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf $(TARGET_SRC)

