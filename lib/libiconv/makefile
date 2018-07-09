# History:
# Date			Author			Comment
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
#
# compile: 
# 	cd stdlinux
#	make user/libiconv-1.13.1_only

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install-me-here
TARGET_SRC=libiconv-1.13.1

all: $(INSTALL_PREFIX)/lib/libiconv.so

untar-all-source:
	if [ ! -d $(TARGET_SRC) ]; then \
		[ ! -f $(TARGET_SRC).tar.gz ] && wget http://ftp.gnu.org/gnu/libiconv/$(TARGET_SRC).tar.gz; \
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


$(INSTALL_PREFIX)/lib/libiconv.so:
	make untar-all-source
	make build

$(TC_INSTALL_PREFIX)/lib/libiconv.so: $(INSTALL_PREFIX)/lib/libiconv.so
	make install


# install into toolchain
install:
	cp -R $(INSTALL_PREFIX)/* $(TC_INSTALL_PREFIX)


# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/libiconv.so.2.5.0
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/lib/libiconv.so* /lib

package: $(TC_INSTALL_PREFIX)/lib/libiconv.so.2.5.0
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/lib
	$(STRIP) -s $(TC_INSTALL_PREFIX)/lib/libiconv.so.2.5.0
	cp -a $(TC_INSTALL_PREFIX)/lib/libiconv.so.2.5.0 ipkg_$(CONFIGURE_TARGET)-linux/lib/
	(cd ipkg_$(CONFIGURE_TARGET)-linux/lib ; \
	ln -sf libiconv.so.2.5.0 libiconv.so; \
	ln -sf libiconv.so.2.0.0 libiconv.so.2; \
	)
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

# copy from toolchain to romfs
tc2romfs:
	$(ROMFSINST) "$(TC_LIBDIR)/libiconv.so*"  /lib


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/libiconv.so
	cp -a $(INSTALL_PREFIX)/include/*  $(TC_INSTALL_PREFIX)/include
	cp -a $(INSTALL_PREFIX)/lib/*.so*  $(TC_INSTALL_PREFIX)/lib
	cp -a $(INSTALL_PREFIX)/lib/*.a  $(TC_INSTALL_PREFIX)/lib


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf $(TARGET_SRC)
