# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/gd_only
#

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install-me-here
TARGET_SRC=gd-2.0.35

all: $(INSTALL_PREFIX)/lib/libgd.so

untar-all-source:
	if [ ! -d $(TARGET_SRC) ]; then \
		[ ! -f $(TARGET_SRC).tar.bz2 ] && wget ftp://ftp.dei.uc.pt/pub/linux/gentoo/distfiles/$(TARGET_SRC).tar.bz2; \
		if [ -f "$(TARGET_SRC).tar.bz2" ]; then \
			tar xjf $(TARGET_SRC).tar.bz2; \
			patch -p0 < gd-2.0.35-png_check_sig.diff; \
		fi \
	fi

build:
	mkdir -p build_$(CONFIGURE_TARGET)-linux
	(cd build_$(CONFIGURE_TARGET)-linux; \
	CC="$(CC) -I$(TC_INSTALL_PREFIX)/include/freetype2" \
	LD="$(LD) -L$(TC_LIBDIR)" \
	CXX="$(CXX)" \
	CPP="$(CPP)" \
	../$(TARGET_SRC)/configure \
	--prefix="$(INSTALL_PREFIX)" \
	--host=arm-linux \
	--build=i686-pc-linux \
	--enable-shared \
	--with-freetype="$(TC_INSTALL_PREFIX)" \
	--with-jpeg="$(TC_INSTALL_PREFIX)" \
	--without-x \
	--without-png \
	)
	rm -rf build_$(CONFIGURE_TARGET)-linux/libgd.la
	make -C build_$(CONFIGURE_TARGET)-linux
	make -C build_$(CONFIGURE_TARGET)-linux install

	#--with-png="$(TC_INSTALL_PREFIX)" \


$(INSTALL_PREFIX)/lib/libgd.so:
	make untar-all-source
	make build

$(TC_INSTALL_PREFIX)/lib/libgd.so: $(INSTALL_PREFIX)/lib/libgd.so
	make install


# install into toolchain
install:
	cp -R $(INSTALL_PREFIX)/* $(TC_INSTALL_PREFIX)


# install to romfs
romfs: #$(TC_INSTALL_PREFIX)/lib/libgd.so
	#$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libgd.so*" /lib

package:

# copy from toolchain to romfs
tc2romfs:
	$(ROMFSINST) "$(TC_LIBDIR)/libgd.so*" /lib


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/libgd.so
	cp -a $(INSTALL_PREFIX)/include/* $(TC_INSTALL_PREFIX)/include
	cp -a $(INSTALL_PREFIX)/lib/libgd.so* $(TC_INSTALL_PREFIX)/lib
	cp -a $(INSTALL_PREFIX)/lib/libgd.a $(TC_INSTALL_PREFIX)/lib


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf $(TARGET_SRC)
