# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/libgmp_only
#

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install-me-here
TARGET_SRC=libgmp

all: $(INSTALL_PREFIX)/lib/libgmp.so

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
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	CC=$(CROSS)gcc \
	CPP=$(CROSS)cpp \
	AR=$(CROSS)ar \
	NM=$(CROSS)nm \
	STRIP=$(CROSS)strip \
	RANLIB=$(CROSS)ranlib \
	LD=$(CROSS)ld \
	CFLAGS="-O2" \
	../$(TARGET_SRC)/configure \
	--build=i686-linux \
	--host=arm-linux \
	--prefix=$(INSTALL_PREFIX) \
	)
	make -C build_$(CONFIGURE_TARGET)-linux
	make -C build_$(CONFIGURE_TARGET)-linux install


$(INSTALL_PREFIX)/lib/libgmp.so:
	make untar-all-source
	make build

$(TC_INSTALL_PREFIX)/lib/libgmp.so: $(INSTALL_PREFIX)/lib/libgmp.so
	make install


# install into toolchain
install:
	cp -R $(INSTALL_PREFIX)/* $(TC_INSTALL_PREFIX)
	find $(TC_INSTALL_PREFIX)/lib/ -maxdepth 1 -type f -name libgmp.so* -exec $(STRIP) -s {} \;


# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/libgmp.so
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libgmp.so*" /lib

package:
	echo "Nothing to do"

# copy from toolchain to romfs
tc2romfs:
	$(ROMFSINST) "$(TC_LIBDIR)/libgmp.so*" /lib


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/libgmp.so
	cp -a $(INSTALL_PREFIX)/include/* $(TC_INSTALL_PREFIX)/include
	cp -a $(INSTALL_PREFIX)/lib/libgmp.so* $(TC_INSTALL_PREFIX)/lib
	cp -a $(INSTALL_PREFIX)/lib/libgmp.a $(TC_INSTALL_PREFIX)/lib


clean distclean:
	-rm -rf build_$(CONFIGURE_TARGET)-linux
	-rm -rf $(TARGET_SRC)

