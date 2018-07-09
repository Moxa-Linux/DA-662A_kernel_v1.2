#
# compile: 
#	make lib/busybox-x.x.x_only
#

all: build_$(CONFIGURE_TARGET)-linux/.config
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	make -f Makefile ARCH=xscale CROSS_COMPILE=$(CROSS) CFLAGS="-isystem $(TC_INSTALL_PREFIX)/include" V=1 )

build_$(CONFIGURE_TARGET)-linux/.config: build_$(CONFIGURE_TARGET)-linux
	echo "update user/busybox/.config";
	cp -a ./busybox_defconfig build_$(CONFIGURE_TARGET)-linux/.config
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CC CXX CPP HOSTCC HOSTLD SHIPPED CFLAGS CPPFLAGS CXXFLAGS LDFLAGS && \
	make -f Makefile oldconfig V=1);

build_$(CONFIGURE_TARGET)-linux:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf busybox-1.15.3.tar.tar
	mv busybox-1.15.3 build_$(CONFIGURE_TARGET)-linux

menuconfig:
	@cp ./busybox_defconfig build_$(CONFIGURE_TARGET)-linux/.config
	make -C build_$(CONFIGURE_TARGET)-linux menuconfig
	@echo "Copy setting to ./busybox_defconfig"
	@cp build_$(CONFIGURE_TARGET)-linux/.config ./busybox_defconfig 

romfs: build_$(CONFIGURE_TARGET)-linux/.config
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET)-linux -f Makefile install CFLAGS="-isystem $(TC_INSTALL_PREFIX)/include"
	chmod a+s $(ROMFSDIR)/bin/busybox

package:

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux
