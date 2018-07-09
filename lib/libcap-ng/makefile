# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/libcap-ng_only
#

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install-me-here
TARGET_SRC=libcap-ng-0.6.5

all: $(INSTALL_PREFIX)/lib/libcap-ng.so

untar-all-source:
	if [ ! -d $(TARGET_SRC) ]; then \
		[ ! -f $(TARGET_SRC).tar.gz ] && wget http://people.redhat.com/sgrubb/libcap-ng/$(TARGET_SRC).tar.gz; \
		[ -f $(TARGET_SRC).tar.gz ] && tar xzf $(TARGET_SRC).tar.gz; \
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
	--host=$(CONFIGURE_TARGET) \
	--build=i686-pc-linux \
	--enable-shared \
	--with-gnu-ld \
	)
	make -C build_$(CONFIGURE_TARGET)-linux
	make -C build_$(CONFIGURE_TARGET)-linux install


$(INSTALL_PREFIX)/lib/libcap-ng.so:
	make untar-all-source
	make build

$(TC_INSTALL_PREFIX)/lib/libcap-ng.so: $(INSTALL_PREFIX)/lib/libcap-ng.so
	make install


# install into toolchain
install:
	cp -R $(INSTALL_PREFIX)/* $(TC_INSTALL_PREFIX)


# install to romfs
romfs:
	$(ROMFSINST) "$(INSTALL_PREFIX)/lib/libcap-ng.so*" /lib


# copy from toolchain to romfs
tc2romfs: $(TC_INSTALL_PREFIX)/lib/libcap-ng.so
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libcap-ng.so*" /lib


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/libcap-ng.so
	cp -a $(INSTALL_PREFIX)/include  $(TC_INSTALL_PREFIX)
	cp -a $(INSTALL_PREFIX)/lib/libcap-ng.a  $(TC_INSTALL_PREFIX)/lib
	cp -a $(INSTALL_PREFIX)/lib/libcap-ng.so*  $(TC_INSTALL_PREFIX)/lib


clean distclean:
	rm -rf $(TARGET_SRC) \
	build_$(CONFIGURE_TARGET)-linux

