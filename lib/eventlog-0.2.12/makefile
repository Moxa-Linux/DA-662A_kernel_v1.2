# History:
# Date			Author			Comment
# 08-13-2011	Simon Wang		First create
#
# compile: 
#	make lib/eventlog-0.2.12_only
#

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install-me-here
TARGET_SRC=eventlog_0.2.12

all: $(TC_INSTALL_PREFIX)/lib/libevtlog.so.0.0.0


build:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xvf $(TARGET_SRC).tar.gz
	mv eventlog-0.2.12 build_$(CONFIGURE_TARGET)-linux
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


$(INSTALL_PREFIX)/lib/libevtlog.so.0.0.0:
	make build

$(TC_INSTALL_PREFIX)/lib/libevtlog.so.0.0.0: $(INSTALL_PREFIX)/lib/libevtlog.so.0.0.0
	make install


# install into toolchain
install:
	cp -a $(INSTALL_PREFIX)/include/* $(TC_INSTALL_PREFIX)/include
	cp -a $(INSTALL_PREFIX)/lib/* $(TC_INSTALL_PREFIX)/lib


# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/libevtlog.so.0.0.0
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libevtlog.so*" /lib

package:
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/lib
	cp -a $(TC_INSTALL_PREFIX)/lib/libevtlog.so.0.0.0 ipkg_$(CONFIGURE_TARGET)-linux/lib/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/lib/libevtlog.so.0.0.0
	(cd ipkg_$(CONFIGURE_TARGET)-linux/lib ; \
	ln -sf libevtlog.so.0.0.0 libevtlog.so; \
	ln -sf libevtlog.so.0.0.0 libevtlog.so.-; \
	)
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


# copy from toolchain to romfs
tc2romfs:
	$(ROMFSINST) "$(TC_LIBDIR)/libevtlog.so*" /lib


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/libevtlog.so.0.0.0
	cp -a $(INSTALL_PREFIX)/include/*  $(TC_INSTALL_PREFIX)/include
	cp -a $(INSTALL_PREFIX)/lib/*.so*  $(TC_INSTALL_PREFIX)/lib
	cp -a $(INSTALL_PREFIX)/lib/*.a  $(TC_INSTALL_PREFIX)/lib


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux \
	ipkg_$(CONFIGURE_TARGET)-linux/lib \
	$(TARGET_SRC)

