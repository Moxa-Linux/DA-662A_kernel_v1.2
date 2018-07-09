# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/readline_only
#

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install-me-here
TARGET_SRC=readline-6.1


all: $(INSTALL_PREFIX)/lib/libreadline.so

untar-all-source:
	if [ ! -d $(TARGET_SRC) ]; then \
		[ ! -f $(TARGET_SRC).tar.gz ] && \
			wget http://ftp.gnu.org/gnu/readline/$(TARGET_SRC).tar.gz; \
		[ -f $(TARGET_SRC).tar.gz ] && tar xzf $(TARGET_SRC).tar.gz; \
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
		--host=arm-none-linux \
		--build=i686-pc-linux \
		--enable-shared \
		--with-curses \
	)
	make -C build_$(CONFIGURE_TARGET)-linux
	make -C build_$(CONFIGURE_TARGET)-linux install


$(INSTALL_PREFIX)/lib/libreadline.so:
	make untar-all-source
	make build

$(TC_INSTALL_PREFIX)/lib/libreadline.so: $(INSTALL_PREFIX)/lib/libreadline.so
	make install


# install into toolchain
install:
	cp -Rf $(INSTALL_PREFIX)/* $(TC_INSTALL_PREFIX)


# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/libreadline.so
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libreadline.so*" /lib
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libhistory.so*" /lib


# copy from toolchain to romfs
tc2romfs:
	$(ROMFSINST) "$(TC_LIBDIR)/libreadline.so*" /lib
	$(ROMFSINST) "$(TC_LIBDIR)/libhistory.so*" /lib


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/libreadline.so
	cp -a $(INSTALL_PREFIX)/include/*  $(TC_INSTALL_PREFIX)/include
	cp -a $(INSTALL_PREFIX)/lib/*.so*  $(TC_INSTALL_PREFIX)/lib
	cp -a $(INSTALL_PREFIX)/lib/*.a  $(TC_INSTALL_PREFIX)/lib


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf $(TARGET_SRC)

package:
	# Make the ipkg package
	rm -rf ipkg_$(CONFIGURE_TARGET)-linux/lib
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/lib
	cp -a build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/libreadline.*so* ipkg_$(CONFIGURE_TARGET)-linux/lib/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/lib/libreadline.so.6.1
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


