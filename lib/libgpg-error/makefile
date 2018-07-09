# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/alsa-lib_only
#

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install-me-here
TARGET_SRC=libgpg-error-1.6

CONFIGURE_SETTINGS := CC=$(CROSS)gcc \
		LD=$(CROSS)ld \
		CXX=$(CROSS)g++ \
		--build=i686-linux \
		--host=$(CONFIGURE_TARGET) \
		--prefix=$(INSTALL_PREFIX)


all: $(INSTALL_PREFIX)/lib/libgpg-error.so

untar-all-source:
	if [ ! -d $(TARGET_SRC) ]; then \
		[ -f $(TARGET_SRC).tar.bz2 ] && tar xjf $(TARGET_SRC).tar.bz2; \
	fi


build:
	if [ -d "build_$(CONFIGURE_TARGET)-linux" ]; then \
		rm -rf build_$(CONFIGURE_TARGET)-linux; \
	fi
	mkdir -p build_$(CONFIGURE_TARGET)-linux
	(cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CC CPPFLAGS LDFLAGS CXXFLAGS && \
	../$(TARGET_SRC)/configure $(CONFIGURE_SETTINGS) \
	)
	unset CFLAGS CC CPPFLAGS LDFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET)-linux
	make -C build_$(CONFIGURE_TARGET)-linux -f Makefile install


$(INSTALL_PREFIX)/lib/libgpg-error.so:
	make untar-all-source
	make build

$(TC_INSTALL_PREFIX)/lib/libgpg-error.so: $(INSTALL_PREFIX)/lib/libgpg-error.so
	make install


# install into toolchain
install:
	cp -R $(INSTALL_PREFIX)/* $(TC_INSTALL_PREFIX)
	$(STRIP) -s $(INSTALL_PREFIX)/bin/gpg-error
	find $(TC_INSTALL_PREFIX)/lib/ -maxdepth 1 -type f -name libgpg-error.so* -exec $(STRIP) -s {} \;


# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/libgpg-error.so
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/bin/gpg-error*" /bin
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libgpg-error.so*" /lib


# copy from toolchain to romfs
tc2romfs:
	$(ROMFSINST) "$(TC_LIBDIR)/bin/libgpg-error*" /bin
	$(ROMFSINST) "$(TC_LIBDIR)/lib/libgpg-error.so*" /lib


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/libgpg-error.so
	cp -a $(INSTALL_PREFIX)/include/* $(TC_INSTALL_PREFIX)/include
	cp -a $(INSTALL_PREFIX)/lib/libgpg-error.a $(TC_INSTALL_PREFIX)/lib
	cp -a $(INSTALL_PREFIX)/lib/libgpg-error.so* $(TC_INSTALL_PREFIX)/lib


clean distclean:
	-rm -rf build_$(CONFIGURE_TARGET)-linux
	-rm -rf ipkg_libgpg-error_$(CONFIGURE_TARGET)/lib/libgpg-error.so*
	-rm -rf $(TARGET_SRC)


package:$(INSTALL_PREFIX)/lib/libgpg-error.so
	mkdir -p ipkg_libgpg-error_$(CONFIGURE_TARGET)-linux/bin
	mkdir -p ipkg_libgpg-error_$(CONFIGURE_TARGET)-linux/lib
	cp -a $(INSTALL_PREFIX)/bin/gpg* ipkg_libgpg-error_$(CONFIGURE_TARGET)-linux/bin
	cp -a $(INSTALL_PREFIX)/lib/libgpg-error.so* ipkg_libgpg-error_$(CONFIGURE_TARGET)-linux/lib
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_libgpg-error_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

