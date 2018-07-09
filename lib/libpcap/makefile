# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/libpng_only
#

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install-me-here
TARGET_SRC=libpcap-1.0.0

all: $(INSTALL_PREFIX)/lib/libpcap.a

untar-all-source:
	if [ ! -f $(TARGET_SRC) ]; then \
		[ ! -f $(TARGET_SRC).tar.bz2 ] && \
			wget http://ftp.nluug.nl/pub/os/Linux/distr/fatdog/source/l/$(TARGET_SRC).tar.bz2; \
		[ -f $(TARGET_SRC).tar.bz2 ] && tar xjf $(TARGET_SRC).tar.bz2; \
		(cd $(TARGET_SRC); \
			patch -p1 -i ../libpcap-1.0.0.patch; \
		) \
	fi

build:
	if [ -d "build_$(CONFIGURE_TARGET)-linux" ]; then \
		rm -rf build_$(CONFIGURE_TARGET)-linux; \
	fi
	# Rename for different configure
	mkdir -p build_$(CONFIGURE_TARGET)-linux
	# Configure 
	(cd build_$(CONFIGURE_TARGET)-linux; \
	ac_cv_linux_vers=2.6.38.8 \
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
	--enable-ipv6 \
	--with-pcap=linux \
	)
	make -C build_$(CONFIGURE_TARGET)-linux
	make -C build_$(CONFIGURE_TARGET)-linux install


$(INSTALL_PREFIX)/lib/libpcap.a:
	make untar-all-source
	make build

$(TC_INSTALL_PREFIX)/lib/libpcap.a: $(INSTALL_PREFIX)/lib/libpcap.a
	make install


# install into toolchain
install:
	cp -R $(INSTALL_PREFIX)/* $(TC_INSTALL_PREFIX)


# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/libpcap.a
	#$(ROMFSINST) $(TC_INSTALL_PREFIX)/lib/libpcap.a /lib


# copy from toolchain to romfs
tc2romfs:
	#$(ROMFSINST) "$(TC_LIBDIR)/libpcap.a" /lib


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/libpcap.a
	cp -a $(INSTALL_PREFIX)/include/* $(TC_INSTALL_PREFIX)/include
	cp -a $(INSTALL_PREFIX)/lib/libpcap.a $(TC_INSTALL_PREFIX)/lib


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf $(TARGET_SRC)
