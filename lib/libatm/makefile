# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/libatm_only
#

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install-me-here
TARGET_SRC=linux-atm-2.5.2


all: $(INSTALL_PREFIX)/lib/libatm.so

untar-all-source:
	if [ ! -d $(TARGET_SRC) ]; then \
		[ ! -f $(TARGET_SRC).tar.gz ] && \
			wget http://downloads.sourceforge.net/project/linux-atm/linux-atm/2.5.2/$(TARGET_SRC).tar.gz; \
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
	AR=$(CROSS)ar \
	../$(TARGET_SRC)/configure \
	--prefix="$(INSTALL_PREFIX)" \
	--host=arm-linux \
	--build=i686-pc-linux \
	--enable-shared \
	)
	make -C build_$(CONFIGURE_TARGET)-linux
	make -C build_$(CONFIGURE_TARGET)-linux install


$(INSTALL_PREFIX)/lib/libatm.so:
	make untar-all-source
	make build

$(TC_INSTALL_PREFIX)/lib/libatm.so: $(INSTALL_PREFIX)/lib/libatm.so
	make install


# install into toolchain
install:
	cp -R $(INSTALL_PREFIX)/* $(TC_INSTALL_PREFIX)


# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/libatm.so
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libatm.so*" /lib


# copy from toolchain to romfs
tc2romfs: 
	$(ROMFSINST) "$(TC_LIBDIR)/libatm.so*" /lib


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/libatm.so
		cp -a $(INSTALL_PREFIX)/include/* $(TC_INSTALL_PREFIX)/include
		cp -a $(INSTALL_PREFIX)/lib/libatm.a $(TC_INSTALL_PREFIX)/lib
		cp -a $(INSTALL_PREFIX)/lib/libatm.so* $(TC_INSTALL_PREFIX)/lib


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf $(TARGET_SRC)

