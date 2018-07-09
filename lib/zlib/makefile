# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/zlib_only
#

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install-me-here
TARGET_SRC=zlib-1.2.3


all: $(INSTALL_PREFIX)/lib/libz.so

untar-all-source:
	if [ ! -d $(TARGET_SRC) ]; then \
		[ ! -f $(TARGET_SRC).tar.bz2 ] && \
			wget http://www.gzip.org/zlib/$(TARGET_SRC).tar.bz2; \
		[ -f $(TARGET_SRC).tar.bz2 ] && tar xjf $(TARGET_SRC).tar.bz2; \
	fi


build:
	if [ -d "build_$(CONFIGURE_TARGET)-linux" ]; then \
		rm -rf build_$(CONFIGURE_TARGET)-linux; \
	fi
	cp -a $(TARGET_SRC) build_$(CONFIGURE_TARGET)-linux; \
	(cd build_$(CONFIGURE_TARGET)-linux; \
	CC=$(CROSS)gcc \
	CXX=$(CROSS)g++ \
	CPP="$(CROSS)gcc -E" \
	LD="$(CROSS)ld -lgcc_s" \
	AR="$(CROSS)ar rcs" \
	AS=$(CROSS)as \
	./configure \
		--prefix="$(INSTALL_PREFIX)" \
	)
	make -C build_$(CONFIGURE_TARGET)-linux
	make -C build_$(CONFIGURE_TARGET)-linux install
	#CFLAGS=-UNO_DIVIDE \

		#--shared \

$(INSTALL_PREFIX)/lib/libz.so:
	make untar-all-source
	make build

$(TC_INSTALL_PREFIX)/lib/libz.so: $(INSTALL_PREFIX)/lib/libz.so
	make install


# install into toolchain
install:
	cp -R $(INSTALL_PREFIX)/* $(TC_INSTALL_PREFIX)


# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/libz.so
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libz.so*" /lib

package:
	echo "Nothing to do"

# copy from toolchain to romfs
tc2romfs:
	$(ROMFSINST) "$(TC_LIBDIR)/libz.so*" /lib


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/libz.so
	cp -a $(INSTALL_PREFIX)/include/*  $(TC_INSTALL_PREFIX)/include
	cp -a $(INSTALL_PREFIX)/lib/*.so*  $(TC_INSTALL_PREFIX)/lib


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf $(TARGET_SRC)
