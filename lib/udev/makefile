# History:
# Date		Author			Comment
# 06-21-2014	Jared Wu.		First created
#
# compile:
#	make lib/libffi_only
#	make lib/glib2_only
#	make user/udev_only


all: build_$(CONFIGURE_TARGET)-linux/install-me-here/sbin/udevd

build_$(CONFIGURE_TARGET)-linux/install-me-here/sbin/udevd: build_$(CONFIGURE_TARGET)-linux/udev/udevd
	DESTDIR='`pwd`/install-me-here' \
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install

build_$(CONFIGURE_TARGET)-linux/udev/udevd:build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux


build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf udev-164.tar.bz2
	mv udev-164 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	CFLAGS="-DPATH_MAX=4096 -I$(ROOTDIR)/$(LINUXDIR)/drivers/staging/usbvideo" \
	GLIB_CFLAGS="-I$(ROOTDIR)/lib/glib2/build_$(CONFIGURE_TARGET)-linux/install-me-here/include/glib-2.0 -I$(ROOTDIR)/lib/glib2/build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/glib-2.0/include" \
	GLIB_LIBS="-L$(ROOTDIR)/lib/glib2/build_$(CONFIGURE_TARGET)-linux/install-me-here/lib" \
	CC="$(CC)" \
	LD="$(LD)" \
	CPP="$(CPP)" \
	sh ./configure \
	--host=arm-unknown-linux \
	--build=i686 \
	--prefix="/" \
	--bindir=/sbin \
	--libdir=/usr/lib \
	--libexecdir="/lib/udev" \
	--sysconfdir=/etc \
	--with-rootprefix= \
	--with-rootdir=/lib \
	--without-selinux \
	--enable-splite-usr \
	--enable-libkmod \
	--enable-rule_generator \
	--disable-keymap \
	--disable-introspection \
	--disable-extras \
	--with-pci-ids-path=/usr/share/hwdata/ \
	--with-firmware-path=/lib/firmware; \
	)
	sed -i 's/^\textras\/v4l_id/#^\ttextras\/v4l_id/' build_$(CONFIGURE_TARGET)-linux/Makefile
	touch build_$(CONFIGURE_TARGET)-linux/configured

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux;

romfs:
	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/install-me-here/sbin/udevd \
	build_$(CONFIGURE_TARGET)-linux/install-me-here/sbin/udevadm \
	build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/udev/ata_id \
	build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/udev/cdrom_id \
	build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/udev/collect \
	build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/udev/create_floppy_devices \
	build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/udev/edd_id \
	build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/udev/firmware \
	build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/udev/fstab_import \
	build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/udev/input_id \
	build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/udev/path_id \
	build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/udev/scsi_id \
	build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/udev/usb_id
	mkdir -p $(ROMFSDIR)/lib/udev \
	$(ROMFSDIR)/lib/udev/rules.d \
	$(ROMFSDIR)/etc/udev/rules.d \
	$(ROMFSDIR)/etc/rc.d/rcS.d \
	$(ROMFSDIR)/etc/init.d \
	$(ROMFSDIR)/etc/usbmount/mount.d \
	$(ROMFSDIR)/etc/usbmount/umount.d \
	$(ROMFSDIR)/usr/share/usbmount \
	$(ROMFSDIR)/var/run/usbmount \
	$(ROMFSDIR)/media/usb0 \
	$(ROMFSDIR)/media/usb1 \
	$(ROMFSDIR)/media/usb2 \
	$(ROMFSDIR)/media/usb3 \
	$(ROMFSDIR)/media/usb4 \
	$(ROMFSDIR)/media/usb5 \
	$(ROMFSDIR)/media/usb6 \
	$(ROMFSDIR)/media/usb7
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/install-me-here/sbin/udevd /sbin/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/install-me-here/sbin/udevadm /sbin/
	$(ROMFSINST) ipkg_$(CONFIGURE_TARGET)-linux/etc/init.d/udev /etc/init.d/
	$(ROMFSINST) -s /etc/init.d/udev /etc/rc.d/rcS.d/S02udev
	$(ROMFSINST) ipkg_$(CONFIGURE_TARGET)-linux/usr/share/usbmount/usbmount /usr/share/usbmount
	$(ROMFSINST) ipkg_$(CONFIGURE_TARGET)-linux/etc/usbmount/usbmount.conf /etc/usbmount/
	$(ROMFSINST) ipkg_$(CONFIGURE_TARGET)-linux/etc/usbmount/mount.d/00_create_model_symlink /etc/usbmount/mount.d/
	$(ROMFSINST) ipkg_$(CONFIGURE_TARGET)-linux/etc/usbmount/umount.d/00_remove_model_symlink /etc/usbmount/umount.d/
	$(ROMFSINST) "ipkg_$(CONFIGURE_TARGET)-linux/lib/udev/rules.d/50-firmware.rules" /lib/udev/rules.d/
	$(ROMFSINST) "ipkg_$(CONFIGURE_TARGET)-linux/lib/udev/rules.d/50-udev-default.rules" /lib/udev/rules.d/
	#$(ROMFSINST) "ipkg_$(CONFIGURE_TARGET)-linux/lib/udev/rules.d/60-cdrom_id.rules" /lib/udev/rules.d/
	#$(ROMFSINST) "ipkg_$(CONFIGURE_TARGET)-linux/lib/udev/rules.d/60-floppy.rules" /lib/udev/rules.d/
	#$(ROMFSINST) "ipkg_$(CONFIGURE_TARGET)-linux/lib/udev/rules.d/60-persistent-input.rules" /lib/udev/rules.d/
	#$(ROMFSINST) "ipkg_$(CONFIGURE_TARGET)-linux/lib/udev/rules.d/60-persistent-serial.rules" /lib/udev/rules.d/
	#$(ROMFSINST) "ipkg_$(CONFIGURE_TARGET)-linux/lib/udev/rules.d/60-persistent-storage.rules" /lib/udev/rules.d/
	#$(ROMFSINST) "ipkg_$(CONFIGURE_TARGET)-linux/lib/udev/rules.d/60-persistent-storage-tape.rules" /lib/udev/rules.d/
	#$(ROMFSINST) "ipkg_$(CONFIGURE_TARGET)-linux/lib/udev/rules.d/61-persistent-storage-edd.rules" /lib/udev/rules.d/
	#$(ROMFSINST) "ipkg_$(CONFIGURE_TARGET)-linux/lib/udev/rules.d/75-cd-aliases-generator.rules" /lib/udev/rules.d/
	#$(ROMFSINST) "ipkg_$(CONFIGURE_TARGET)-linux/lib/udev/rules.d/75-persistent-net-generator.rules" /lib/udev/rules.d/
	#$(ROMFSINST) "ipkg_$(CONFIGURE_TARGET)-linux/lib/udev/rules.d/79-fstab_import.rules" /lib/udev/rules.d/
	#$(ROMFSINST) "ipkg_$(CONFIGURE_TARGET)-linux/lib/udev/rules.d/80-drivers.rules" /lib/udev/rules.d/
	$(ROMFSINST) "ipkg_$(CONFIGURE_TARGET)-linux/lib/udev/rules.d/95-udev-late.rules" /lib/udev/rules.d/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/extra/hotplug.functions /lib/udev
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/udev/ata_id /lib/udev/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/udev/collect /lib/udev/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/udev/firmware /lib/udev/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/udev/path_id /lib/udev/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/udev/scsi_id /lib/udev/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/udev/usb_id /lib/udev/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/install-me-here/etc/udev/udev.conf /etc/udev/


