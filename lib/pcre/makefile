# cross-compile: make CROSS=xscale-linux- ROOTDIR=/root/sdlinux-moxaart
#
PACKAGE_NAME = pcre-8.30

CONFIGURE_SETTINGS := CC=$(CROSS)gcc \
		LD=$(CROSS)gcc \
		CXX=$(CROSS)g++ \
		CPP=$(CROSS)cpp \
		CFLAGS='-DPATH_MAX=1024' \
		--build=i686-linux \
		--host=$(CONFIGURE_TARGET)-linux \
		--target=$(CONFIGURE_TARGET)-linux \
		--prefix=$(ROOTDIR)/lib/pcre/install_$(CONFIGURE_TARGET)-linux \

#  		--disable-shared
		
all: build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux:build_$(CONFIGURE_TARGET)-linux/src/.libs/
	#make -C build_$(CONFIGURE_TARGET)-linux install;

build_$(CONFIGURE_TARGET)-linux/src/.libs: build_$(CONFIGURE_TARGET)-linux/Makefile
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET)-linux -j 2;

build_$(CONFIGURE_TARGET)-linux/Makefile: makefile
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf $(PACKAGE_NAME).tar.bz2
	mv $(PACKAGE_NAME) build_$(CONFIGURE_TARGET)-linux
	( \
	cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	./configure $(CONFIGURE_SETTINGS) \
	)

romfs:
	echo 'nothing to do'

pachage:
	echo 'nothing to do'

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux install_$(CONFIGURE_TARGET)-linux
