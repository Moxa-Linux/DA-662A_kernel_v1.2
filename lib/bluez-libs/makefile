# History:
# Date		Author			Comment
# 01-25-2010	Jared Wu.		install at BERoot or
#					LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/bluez-libs_only
#

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install-me-here

all: $(INSTALL_PREFIX)/lib/libbluetooth.so.2.11.2
	@echo "Build Complete!"

$(INSTALL_PREFIX)/lib/libbluetooth.so.2.11.2: build_$(CONFIGURE_TARGET)-linux/build
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install

build_$(CONFIGURE_TARGET)-linux/build: build_$(CONFIGURE_TARGET)-linux/Makefile
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/Makefile: 
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf bluez-libs-3.36.tar.gz
	mv bluez-libs-3.36 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	sh ./configure \
	CC=$(CROSS)gcc \
	LD=$(CROSS)ld \
	CPP=$(CROSS)cpp \
	STRIP=$(CROSS)strip \
	--host=$(CONFIGURE_TARGET)-linux \
	--build=i686 \
	--prefix="$(INSTALL_PREFIX)" \
	)
	touch build_$(CONFIGURE_TARGET)-linux/build

clean distclean:
	rm -rf build_*
	rm -rf ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/*

romfs: $(INSTALL_PREFIX)/lib/libbluetooth.so.2.11.2
	$(STRIP) $(INSTALL_PREFIX)/lib/libbluetooth.so.2.11.2
	$(ROMFSINST) $(INSTALL_PREFIX)/lib/libbluetooth.so.2.11.2 /usr/local/lib/libbluetooth.so.2.11.2
	$(ROMFSINST) -s libbluetooth.so.2.11.2 /usr/local/lib/libbluetooth.so.2
	$(ROMFSINST) -s libbluetooth.so.2.11.2 /usr/local/lib/libbluetooth.so

package: $(INSTALL_PREFIX)/lib/libbluetooth.so.2.11.2
	# Make the ipkg package
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib
	cp -a $(INSTALL_PREFIX)/lib/libbluetooth.so.2.11.2 ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/libbluetooth.so.2.11.2
	(cd ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib; \
	ln -sf libbluetooth.so.2.11.2 libbluetooth.so.2; \
	ln -sf libbluetooth.so.2.11.2 libbluetooth.so; \
	)
	$(STRIP) -s ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/libbluetooth.so.2.11.2
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_src_root_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


