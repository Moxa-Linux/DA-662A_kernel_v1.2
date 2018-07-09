# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/libccmalloc_only
#

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)/install-me-here
TARGET_SRC=ccmalloc-0.4.0

all: $(INSTALL_PREFIX)/lib/libccmalloc.a

untar-all-source:
	if [ ! -d $(TARGET_SRC) ]; then \
		[ -f $(TARGET_SRC).tar.gz ] && tar xzf $(TARGET_SRC).tar.gz; \
		(cd $(TARGET_SRC); \
			patch -p1 -i ../$(TARGET_SRC).patch; \
		) \
	fi

build:
	if [ -d "build_$(CONFIGURE_TARGET)-linux" ]; then \
		rm -rf build_$(CONFIGURE_TARGET-linux); \
	fi
	mkdir -p build_$(CONFIGURE_TARGET)-linux
	(cd build_$(CONFIGURE_TARGET)-linux; \
	CC=$(CROSS)gcc \
	AR=$(CROSS)ar \
	../$(TARGET_SRC)/configure \
	--prefix="$(INSTALL_PREFIX)" \
	--srcdir="$(CURDIR)/$(TARGET_SRC)" \
	--host=arm-linux \
	)
	make -C build_$(CONFIGURE_TARGET)-linux
	make -C build_$(CONFIGURE_TARGET)-linux install


$(INSTALL_PREFIX)/lib/libccmalloc.a:
	make untar-all-source
	make build

$(TC_INSTALL_PREFIX)/lib/libccmalloc.a: $(INSTALL_PREFIX)/lib/libccmalloc.a
	make install


# install into toolchain
install:
	cp -R $(INSTALL_PREFIX)/* $(TC_INSTALL_PREFIX)


# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/libccmalloc.a
	#$(ROMFSINST) $(INSTALL_PREFIX)/lib/libccmalloc.a /lib


# copy from toolchain to romfs
tc2romfs: 
	#$(ROMFSINST) "$(TC_LIBDIR)/libccmalloc.a" /lib


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/libccmalloc.a
	cp -a $(INSTALL_PREFIX)/lib/libccmalloc.a $(TC_INSTALL_PREFIX)/lib


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf $(TARGET_SRC)

