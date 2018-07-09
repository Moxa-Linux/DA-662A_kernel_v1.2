# History:
# Date		Author			Comment
# 10-15-2013	Aeluin Chen.	new create
#
# compile: 
#	make lib/busybox-x.x.x_only
#
# reference:
#   http://www.busybox.net/
#

TARGET_VER=1.21.1
TARGET_PRJ=busybox
TARGET_SRC=$(TARGET_PRJ)-$(TARGET_VER)

PRODUCT_DIR=$(ROOTDIR)/vendors/$(CONFIG_VENDOR)/$(CONFIG_PRODUCT)

CONFIG_FILE = $(PRODUCT_DIR)/busybox_defconfig-$(TARGET_VER)
ifeq ($(MAKECMDGOALS),initramfs)
	CONFIG_FILE := $(CONFIG_FILE)-initramfs
endif
	

all: build_$(CONFIGURE_TARGET)/busybox
	@echo "build complete!"

untar-source:
	rm -rf build_$(CONFIGURE_TARGET)
	tar xjvf $(TARGET_SRC).tar.bz2
	mv $(TARGET_SRC) build_$(CONFIGURE_TARGET)
	(cd build_$(CONFIGURE_TARGET); \
		patch -N -Z -p1 < ../$(TARGET_SRC)-tirpc.patch; \
		patch -N -Z -p1 < ../$(TARGET_SRC)-adduser.patch; \
	)

menuconfig: 
	if [ ! -d "build_$(CONFIGURE_TARGET)" ]; then \
		make untar-source; \
	fi
	@if [ -f $(PRODUCT_DIR)/busybox_defconfig-$(TARGET_VER) ]; then \
		cp -a $(PRODUCT_DIR)/busybox_defconfig-$(TARGET_VER) build_$(CONFIGURE_TARGET)/.config; \
	elif [ -f $(PRODUCT_DIR)/busybox_defconfig ]; then \
		cp -a $(PRODUCT_DIR)/busybox_defconfig build_$(CONFIGURE_TARGET)/.config; \
	else \
		cp -a ./busybox_defconfig build_$(CONFIGURE_TARGET)/.config; \
	fi
	make -C build_$(CONFIGURE_TARGET) menuconfig
	@echo "Copy setting to ./busybox_defconfig-$(TARGET_VER)"
	@cp -a build_$(CONFIGURE_TARGET)/.config $(PRODUCT_DIR)/busybox_defconfig-$(TARGET_VER)


build_$(CONFIGURE_TARGET)/busybox: build_$(CONFIGURE_TARGET)/.config
	( cd build_$(CONFIGURE_TARGET); \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	make -f Makefile ARCH=arm CROSS_COMPILE=$(CROSS) CFLAGS="-isystem $(TC_INSTALL_PREFIX)/include" V=1 )

build_$(CONFIGURE_TARGET)/.config: 
	make untar-source
	@echo "update user/$(TARGET_SRC)/build_$(CONFIGURE_TARGET)/.config";
	@if [ -f $(CONFIG_FILE) ]; then \
		echo "update customized .config"; \
		cp -a $(CONFIG_FILE) build_$(CONFIGURE_TARGET)/.config; \
	elif [ -f $(PRODUCT_DIR)/busybox_defconfig ]; then \
		echo "update customized .config"; \
		cp -a $(PRODUCT_DIR)/busybox_defconfig build_$(CONFIGURE_TARGET)/.config; \
	else \
		cp -a ./busybox_defconfig build_$(CONFIGURE_TARGET)/.config; \
		make menuconfig; \
	fi

initramfs: build_$(CONFIGURE_TARGET)/busybox
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET) -f Makefile install CFLAGS="-isystem $(TC_INSTALL_PREFIX)/include"
	chmod a+s $(INITRAMFS_DIR)/bin/busybox

romfs: build_$(CONFIGURE_TARGET)/busybox
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET) -f Makefile install CFLAGS="-isystem $(TC_INSTALL_PREFIX)/include"
	chmod a+s $(ROMFSDIR)/bin/busybox

package:

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)
