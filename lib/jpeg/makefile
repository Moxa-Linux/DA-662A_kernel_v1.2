# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/alsa-lib_only
#

TARGET_SRC=jpeg-7

all: $(TC_INSTALL_PREFIX)/lib/libjpeg.so.7.0.0

untar-all-source:
	if [ ! -d $(TARGET_SRC) ]; then \
		[ ! -f $(TARGET_SRC).tar.gz ] && wget  ftp://ftp.kfki.hu/pub/Sun/freeware/SOURCES/$(TARGET_SRC).tar.gz; \
		[ -f $(TARGET_SRC).tar.gz ] && tar xzf $(TARGET_SRC).tar.gz; \
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
	--prefix="$(TC_INSTALL_PREFIX)" \
	--host=arm-linux \
	--build=i686-pc-linux \
	--enable-shared \
	)
	make -C build_$(CONFIGURE_TARGET)-linux

# install into toolchain
install:
	make -C build_$(CONFIGURE_TARGET)-linux install


$(TC_INSTALL_PREFIX)/lib/libjpeg.so.7.0.0:
	make untar-all-source
	make build

# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/libjpeg.so.7.0.0
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libjpeg.so*" /lib

package:
	echo "Nothing to do"

# copy from toolchain to romfs
tc2romfs:
	$(ROMFSINST) "$(TC_LIBDIR)/libjpeg.so*" /lib

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf $(TARGET_SRC)

