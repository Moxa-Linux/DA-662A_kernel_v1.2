# History:
# Date		Author			Comment
# 01-25-2010	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/libusb_only


INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install-me-here
TARGET_SRC=libusb-0.1.12


all: $(INSTALL_PREFIX)/lib/libusb-0.1.so

untar-all-source:
	if [ ! -d $(TARGET_SRC) ]; then \
		[ -f $(TARGET_SRC).tar.gz ] && tar xzf $(TARGET_SRC).tar.gz; \
	fi


build:
	if [ -d "build_$(CONFIGURE_TARGET)-linux" ]; then \
		rm -rf build_$(CONFIGURE_TARGET)-linux; \
	fi
	mkdir -p build_$(CONFIGURE_TARGET)-linux
	(cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	../$(TARGET_SRC)/configure \
	CC=$(CROSS)gcc \
	CXX=$(CROSS)g++ \
	LD=$(CROSS)ld \
	CPP=$(CROSS)cpp \
	CFLAGS="" \
	--host=arm-linux \
	--build=i686 \
	--prefix="$(INSTALL_PREFIX)" \
	)
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install


$(INSTALL_PREFIX)/lib/libusb-0.1.so:
	make untar-all-source
	make build

$(TC_INSTALL_PREFIX)/lib/libusb-0.1.so: $(INSTALL_PREFIX)/lib/libusb-0.1.so
	make install


# install into toolchain
install:
	cp -R $(INSTALL_PREFIX)/* $(TC_INSTALL_PREFIX)
	find $(TC_INSTALL_PREFIX)/lib/ -maxdepth 1 -type f -name libusb*.so* -exec $(STRIP) -s {} \;


# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/libusb-0.1.so
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libusb*.so*" /lib


# copy from toolchain to romfs
tc2romfs:
	$(ROMFSINST) "$(TC_LIBDIR)/libusb*.so*" /lib


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/libusb-0.1.so
	cp -a $(INSTALL_PREFIX)/include/*  $(TC_INSTALL_PREFIX)/include
	cp -a $(INSTALL_PREFIX)/lib/*.so*  $(TC_INSTALL_PREFIX)/lib
	cp -a $(INSTALL_PREFIX)/lib/*.a  $(TC_INSTALL_PREFIX)/lib


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf $(TARGET_SRC)
	rm -rf ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/*


package: $(INSTALL_PREFIX)/lib/libusb-0.1.so
	# Make the ipkg package
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/bin
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib
	cp -a $(INSTALL_PREFIX)/bin/libusb-config ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a $(INSTALL_PREFIX)/lib/libusb*.so* ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_src_root_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

