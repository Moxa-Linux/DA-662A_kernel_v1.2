# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/ncurses_only
#
INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install-me-here
TARGET_SRC=ncurses-5.7


all: $(INSTALL_PREFIX)/lib/libncurses.a

untar-all-source:
	if [ ! -d $(TARGET_SRC) ]; then \
		[ ! -f $(TARGET_SRC).tar.gz ] && \
			wget http://ftp.gnu.org/gnu/ncurses/$(TARGET_SRC).tar.gz; \
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
	--host=arm-linux \
	--build=i686-pc-linux \
	--with-shared \
	--without-ada \
	--with-terminfo-dirs="/usr/share/terminfo" \
	--with-libtool="no" \
	--disable-big-core \
	--disable-assertions \
	--without-dmalloc \
	--without-dbmalloc \
	--without-valgrind \
	--disable-leaks \
	--disable-expanded \
	--disable-macros \
	--without-trace \
	)
	make -C build_$(CONFIGURE_TARGET)-linux
	make -C build_$(CONFIGURE_TARGET)-linux install


$(INSTALL_PREFIX)/lib/libncurses.a:
	make untar-all-source
	make build

$(TC_INSTALL_PREFIX)/lib/libncurses.a: $(INSTALL_PREFIX)/lib/libncurses.a
	make install


# install into toolchain
install:
	cp -R $(INSTALL_PREFIX)/* $(TC_INSTALL_PREFIX)


# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/libncurses.a
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libncurses.so.5*" /lib
	#@echo "nothing to be copied."


# copy from toolchain to romfs
tc2romfs:
	$(ROMFSINST) "$(TC_LIBDIR)/libncurses.so.5*" /lib
	#@echo "nothing to be copied."


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/libncurses.a
	cp -a $(INSTALL_PREFIX)/include/*  $(TC_INSTALL_PREFIX)/include
	cp -a $(INSTALL_PREFIX)/lib/*.a  $(TC_INSTALL_PREFIX)/lib
	cp -a $(INSTALL_PREFIX)/lib/*.so* $(TC_INSTALL_PREFIX)/lib


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf $(TARGET_SRC)

