# History:
# Date		Author			Comment
# 07-12-2009	Jared Wu.		install at BERoot/LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/util-linux-2.23.1_only

all: $(TC_INSTALL_PREFIX)/lib/libuuid.so.1.3.0

$(TC_INSTALL_PREFIX)/lib/libuuid.so.1.3.0: build_$(CONFIGURE_TARGET)-linux/build
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install

build_$(CONFIGURE_TARGET)-linux/build: makefile
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjf util-linux-2.23.1.tar.bz2
	mv util-linux-2.23.1 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	./configure \
	CC=$(CROSS)gcc \
	CPP=$(CROSS)cpp \
	CFLAGS="-DLINE_MAX=2048 -I$(TC_INSTALL_PREFIX)/include/ncurses" \
	LD=$(CROSS)ld \
	STRIP=$(CROSS)strip \
	--host=$(CONFIGURE_TARGET)-linux \
	--build=i686 \
	--prefix="$(TC_INSTALL_PREFIX)" \
	--with-static \
	--without-ssl \
	--without-libidn \
	)
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux
	touch build_$(CONFIGURE_TARGET)-linux/build

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux;

romfs:
	$(STRIP) build_$(CONFIGURE_TARGET)-linux/.libs/libuuid.so.1.3.0 \
	build_$(CONFIGURE_TARGET)-linux/.libs/libmount.so.1.1.0 \
	build_$(CONFIGURE_TARGET)-linux/.libs/libblkid.so.1.1.0
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/.libs/libuuid.so.1.3.0 /lib/libuuid.so.1.3.0
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/.libs/libmount.so.1.1.0 /lib/libmount.so.1
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/.libs/libblkid.so.1.1.0 /lib/libblkid.so.1
	$(ROMFSINST) -s libuuid.so.1.3.0 /lib/libuuid.so.1
	$(ROMFSINST) -s libmount.so.1.1.0 /lib/libmount.so.1
	$(ROMFSINST) -s libblkid.so.1.1.0 /lib/libblkid.so.1
	#$(STRIP) build_$(CONFIGURE_TARGET)-linux/.libs/blkid \
	#build_$(CONFIGURE_TARGET)-linux/.libs/cfdisk
	#$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/.libs/blkid /sbin/blkid
	#$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/.libs/cfdisk /sbin/cfdisk

package:
	# Make the ipkg package

