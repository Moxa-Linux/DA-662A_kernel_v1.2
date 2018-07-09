#
# compile: 
#	make lib/busybox-x.x.x_only
#

TARGET_SRC = busybox-1.18.5


all: build_$(CONFIGURE_TARGET)/busybox
	@echo "build complete!"

build_$(CONFIGURE_TARGET)/busybox: build_$(CONFIGURE_TARGET)/.config
	( cd build_$(CONFIGURE_TARGET); \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	make -f Makefile ARCH=arm CROSS_COMPILE=$(CROSS) CFLAGS="-isystem $(TC_INSTALL_PREFIX)/include" V=1 )

build_$(CONFIGURE_TARGET)/.config: 
	make untar-source
	@echo "update user/$(TARGET_SRC)/build_$(CONFIGURE_TARGET)/.config";
	@if [ -f $(ROOTDIR)/vendors/$(CONFIG_VENDOR)/$(CONFIG_PRODUCT)/busybox_defconfig-1.18.5 ]; then \
		echo "update customized .config"; \
		cp -a $(ROOTDIR)/vendors/$(CONFIG_VENDOR)/$(CONFIG_PRODUCT)/busybox_defconfig-1.18.5 build_$(CONFIGURE_TARGET)/.config; \
	else \
		cp -a ./busybox_defconfig build_$(CONFIGURE_TARGET)/.config; \
		make menuconfig; \
	fi

untar-source:
	rm -rf build_$(CONFIGURE_TARGET)
	tar xjvf $(TARGET_SRC).tar.bz2
	mv $(TARGET_SRC) build_$(CONFIGURE_TARGET)
	(cd build_$(CONFIGURE_TARGET); \
		patch -N -Z -p1 < ../$(TARGET_SRC)-libbb.patch; \
		patch -p1 < ../FEATURE_BLKID_TYPE.diff; \
	)

menuconfig: 
	if [ ! -d "build_$(CONFIGURE_TARGET)" ]; then \
		make untar-source; \
	fi
	@cp -a $(ROOTDIR)/vendors/$(CONFIG_VENDOR)/$(CONFIG_PRODUCT)/busybox_defconfig build_$(CONFIGURE_TARGET)/.config
	make -C build_$(CONFIGURE_TARGET) menuconfig
	@echo "Copy setting to ./busybox_defconfig"
	@cp -a build_$(CONFIGURE_TARGET)/.config $(ROOTDIR)/vendors/$(CONFIG_VENDOR)/$(CONFIG_PRODUCT)/busybox_defconfig

romfs: build_$(CONFIGURE_TARGET)/busybox
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET) -f Makefile install CFLAGS="-isystem $(TC_INSTALL_PREFIX)/include"
	chmod a+s $(ROMFSDIR)/bin/busybox

package:

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)
