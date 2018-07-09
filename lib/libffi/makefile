# History:
# Date		Author			Comment
# 10-24-2012	Jared Wu.		First create
#
# compile: 
#	make lib/libffi_only
#

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install-me-here
TARGET_SRC=libffi_3.0.10.orig

all: $(INSTALL_PREFIX)/lib/libffi.so.5.0.10


build:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xvf $(TARGET_SRC).tar.gz
	mv libffi-3.0.10 build_$(CONFIGURE_TARGET)-linux
	(cd build_$(CONFIGURE_TARGET)-linux; \
	CC="$(CC)" \
	LD="$(LD)" \
	STRIP="$(STRIP)" \
	CXX="$(CXX)" \
	CPP="$(CPP)" \
	AS="$(AS)" \
	AR="$(AR)" \
	./configure \
	--prefix="$(INSTALL_PREFIX)" \
	--host=arm-linux \
	--build=i686-pc-linux \
	--enable-shared \
	)
	make -C build_$(CONFIGURE_TARGET)-linux
	make -C build_$(CONFIGURE_TARGET)-linux install
	rm -rf $(TC_INSTALL_PREFIX)/lib/libffi.la


$(INSTALL_PREFIX)/lib/libffi.so.5.0.10:
	make build

$(TC_INSTALL_PREFIX)/lib/libffi.so.5.0.10: $(INSTALL_PREFIX)/lib/libffi.so.5.0.10
	make install


# install into toolchain
install:
	cp -R $(INSTALL_PREFIX)/* $(TC_INSTALL_PREFIX)


# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/libffi.so.5.0.10
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libffi*.so*" /lib

package:
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/lib
	cp -a $(TC_INSTALL_PREFIX)/lib/libffi.so.5.0.10 ipkg_$(CONFIGURE_TARGET)-linux/lib/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/lib/libffi.so.5.0.10
	(cd ipkg_$(CONFIGURE_TARGET)-linux/lib ; \
	ln -sf libffi.so.5.0.10 libffi.so; \
	ln -sf libffi.so.5.0.10 libffi.so.5; \
	)
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


# copy from toolchain to romfs
tc2romfs:
	$(ROMFSINST) "$(TC_LIBDIR)/libffi*.so*" /lib


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/libffi.so.5.0.10
	cp -a $(INSTALL_PREFIX)/include/*  $(TC_INSTALL_PREFIX)/include
	cp -a $(INSTALL_PREFIX)/lib/*.so*  $(TC_INSTALL_PREFIX)/lib
	cp -a $(INSTALL_PREFIX)/lib/*.a  $(TC_INSTALL_PREFIX)/lib


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux \
	ipkg_$(CONFIGURE_TARGET)-linux/lib \
	$(TARGET_SRC)

