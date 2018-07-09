# History:
# Date		Author			Comment
# 2010-12-28	Jared Wu.		install at BERoot or
#					LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
# 	cd stdlinux
#	make user/libgcrypt_only

TARGET_SRC=libgcrypt-1.2.4

CONFIGURE_SETTINGS := CC=$(CROSS)gcc \
		LD=$(CROSS)ld \
		CXX=$(CROSS)g++ \
		LDFLAGS="-L$(TC_INSTALL_PREFIX)/lib" \
		--build=i686-linux \
		--host=$(CONFIGURE_TARGET) \
		--prefix=$(TC_INSTALL_PREFIX) \
		--with-gpg-error-prefix=$(TC_INSTALL_PREFIX)


all: $(TC_INSTALL_PREFIX)/lib/libgcrypt.so

untar-all-source:
	if [ ! -d $(TARGET_SRC) ]; then \
		[ -f $(TARGET_SRC).tar.bz2 ] && tar xjf $(TARGET_SRC).tar.bz2; \
		(cd $(TARGET_SRC); \
			patch -p1 -i ../$(TARGET_SRC).patch; \
		) \
	fi

build:
	if [ -d "build_$(CONFIGURE_TARGET)-linux" ]; then \
		rm -rf build_$(CONFIGURE_TARGET)-linux; \
	fi
	mkdir -p build_$(CONFIGURE_TARGET)-linux
	(cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	../$(TARGET_SRC)/configure $(CONFIGURE_SETTINGS) \
	)
	LDFLAGS="-L$(TC_INSTALL_PREFIX)/lib" \
	make -C build_$(CONFIGURE_TARGET)-linux src
	make -C build_$(CONFIGURE_TARGET)-linux install-strip SUBDIRS="m4 mpi cipher src"
	make -C build_$(CONFIGURE_TARGET)-linux install


$(TC_INSTALL_PREFIX)/lib/libgcrypt.so:
	make untar-all-source
	make build


# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/libgcrypt.so
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libgcrypt.so*" /lib


# copy from toolchain to romfs
tc2romfs:
	$(ROMFSINST) "$(TC_LIBDIR)/libgcrypt.so*" /lib

clean distclean:
	rm -rf $(TARGET_SRC)
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf ipkg_libgcrypt_$(CONFIGURE_TARGET)-linux/lib


package: $(TC_INSTALL_PREFIX)/lib/libgcrypt.so
	mkdir -p ipkg_libgcrypt_$(CONFIGURE_TARGET)-linux/lib
	cp -a $(TC_INSTALL_PREFIX)/lib/libgcrypt.so* ipkg_libgcrypt_$(CONFIGURE_TARGET)-linux/lib/
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_libgcrypt_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

