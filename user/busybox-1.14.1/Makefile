#
# compile: 
#	make lib/busybox-x.x.x_only
#

all: build_$(CONFIGURE_TARGET)-linux/.config
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	make -f Makefile ARCH=arm CROSS_COMPILE=$(CROSS) CFLAGS="-isystem $(TC_INSTALL_PREFIX)/include" V=1 )

build_$(CONFIGURE_TARGET)-linux/.config: build_$(CONFIGURE_TARGET)-linux
	echo "update user/busybox/.config";
	if [ -f $(ROOTDIR)/vendors/$(CONFIG_VENDOR)/$(CONFIG_PRODUCT)/$(VERSIONPKG)/busybox-config-1.14.1 ]; then \
		cp -a $(ROOTDIR)/vendors/$(CONFIG_VENDOR)/$(CONFIG_PRODUCT)/$(VERSIONPKG)/busybox-config-1.14.1 build_$(CONFIGURE_TARGET)-linux/.config; \
	else \
		cp -a ./busybox_defconfig build_$(CONFIGURE_TARGET)-linux/.config; \
	fi
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CC CXX CPP HOSTCC HOSTLD SHIPPED CFLAGS CPPFLAGS CXXFLAGS LDFLAGS && \
	make -f Makefile oldconfig V=1);

build_$(CONFIGURE_TARGET)-linux:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf busybox-1.14.1.tar.bz2
	mv busybox-1.14.1 build_$(CONFIGURE_TARGET)-linux
	(cd build_$(CONFIGURE_TARGET)-linux; \
	patch -p1 < ../patches/busybox-1.14.1-ash.patch; \
	patch -p1 < ../patches/busybox-1.14.1-ftpd.patch; \
	patch -p1 < ../patches/busybox-1.14.1-httpd.patch; \
	patch -p1 < ../patches/busybox-1.14.1-modprobe.patch; \
	patch -p1 < ../patches/busybox-1.14.1-readlink.patch; \
	patch -p1 < ../patches/busybox-1.14.1-telnetd.patch; \
	patch -p1 < ../patches/busybox-1.14.2-df.patch; \
	patch -p1 < ../patches/busybox-1.14.2-ls.patch; \
	patch -p1 < ../patches/busybox-1.14.2-ping6.patch; \
	patch -p1 < ../patches/busybox-1.14.2-test.patch; \
	patch -p1 < ../patches/busybox-1.14.2-udhcpd.patch; \
	patch -p1 < ../patches/busybox-1.14.3-ash.patch; \
	patch -p1 < ../patches/busybox-1.14.3-httpd.patch; \
	patch -p1 < ../patches/busybox-1.14.3-modprobe.patch; \
	patch -p1 < ../patches/busybox-1.14.3-trylink-gc-sections.patch; \
	patch -p1 < ../patches/busybox-1.14.1.moxa.patch; \
	patch -p1 < ../patches/busybox-1.14.1.moxa.921600.patch; \
	)

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
