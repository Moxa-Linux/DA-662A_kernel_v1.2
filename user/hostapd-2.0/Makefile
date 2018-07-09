# History:
# Date          Author                  Comment
# 06-28-2012    Jared Wu.               new created
#
# compile:
#	make user/hostapd-2.0_only
#
-include $(ARCH_CONFIG)

PACKAGE_NAME=hostapd-2.0
EXEC=hostapd

all: $(TC_INSTALL_PREFIX)/usr/bin/$(EXEC)

$(TC_INSTALL_PREFIX)/usr/bin/$(EXEC): build_$(CONFIGURE_TARGET)-linux/$(EXEC)/$(EXEC)
	cp -a build_$(CONFIGURE_TARGET)-linux/$(EXEC)/$(EXEC) $(TC_INSTALL_PREFIX)/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/$(EXEC)/$(EXEC)_cli $(TC_INSTALL_PREFIX)/usr/bin/
	mkdir -p $(TC_INSTALL_PREFIX)/etc/$(EXEC)
	cp -a build_$(CONFIGURE_TARGET)-linux/$(EXEC)/$(EXEC).conf $(TC_INSTALL_PREFIX)/etc/$(EXEC)
	echo "Install $(EXEC) complete!"
	# Used for test: upload to target
	#$(CROSS)strip build_$(CONFIGURE_TARGET)-linux/$(EXEC)/$(EXEC)
	#scp build_$(CONFIGURE_TARGET)-linux/$(EXEC)/$(EXEC) 192.168.27.55:/dev/shm/

build_$(CONFIGURE_TARGET)-linux/$(EXEC)/$(EXEC): build_$(CONFIGURE_TARGET)-linux/.config
	CC="$(CC) -I$(TC_INSTALL_PREFIX)/include " \
	LD="$(CC) -L$(TC_INSTALL_PREFIX)/lib " \
	AR=$(AR) \
	V=1 \
	   make -C build_$(CONFIGURE_TARGET)-linux/$(EXEC)
	echo "Build $(EXEC) complete!"

build_$(CONFIGURE_TARGET)-linux/.config:
	if [ ! -d build_$(CONFIGURE_TARGET)-linux ]; then \
		tar xzvf $(PACKAGE_NAME).tar.gz; \
		mv $(PACKAGE_NAME) build_$(CONFIGURE_TARGET)-linux; \
		cp -a build_$(CONFIGURE_TARGET)-linux/$(EXEC)/defconfig build_$(CONFIGURE_TARGET)-linux/$(EXEC)/.config; \
		sed -i 's/#CONFIG_DRIVER_NL80211=y/CONFIG_DRIVER_NL80211=y/' build_$(CONFIGURE_TARGET)-linux/$(EXEC)/.config; \
		sed -i 's/#CONFIG_IEEE80211N=y/CONFIG_IEEE80211N=y/' build_$(CONFIGURE_TARGET)-linux/$(EXEC)/.config; \
		sed -i 's/CONFIG_DRIVER_HOSTAP=y/#CONFIG_DRIVER_HOSTAP=y/' build_$(CONFIGURE_TARGET)-linux/$(EXEC)/.config; \
	fi

romfs:
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/usr/bin/$(EXEC) /usr/bin/
	$(STRIP) -s $(ROMFSDIR)/usr/bin/$(EXEC)
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/usr/bin/$(EXEC)_cli /usr/bin/
	$(STRIP) -s $(ROMFSDIR)/usr/bin/$(EXEC)_cli
	mkdir -p $(ROMFSDIR)/etc/$(EXEC)
	$(ROMFSINST) ipkg_$(CONFIGURE_TARGET)-linux/etc/$(EXEC)/$(EXEC).conf /etc/$(EXEC)
	$(ROMFSINST) ipkg_$(CONFIGURE_TARGET)-linux/etc/init.d/$(EXEC) /etc/init.d/$(EXEC)
	#$(ROMFSINST) ipkg_$(CONFIGURE_TARGET)-linux/etc/rc.d/rc0.d/K01$(EXEC) /etc/rc.d/rc0.d/
	#$(ROMFSINST) ipkg_$(CONFIGURE_TARGET)-linux/etc/rc.d/rc1.d/K01$(EXEC) /etc/rc.d/rc1.d/
	#$(ROMFSINST) ipkg_$(CONFIGURE_TARGET)-linux/etc/rc.d/rc6.d/K01$(EXEC) /etc/rc.d/rc6.d/
	#$(ROMFSINST) ipkg_$(CONFIGURE_TARGET)-linux/etc/rc.d/rc3.d/S99$(EXEC) /etc/rc.d/rc3.d/

package: $(TC_INSTALL_PREFIX)/usr/bin/$(EXEC)
	# Make the ipkg package
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/bin
	cp -a $(TC_INSTALL_PREFIX)/usr/bin/$(EXEC) ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a $(TC_INSTALL_PREFIX)/usr/bin/$(EXEC)_cli ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/$(EXEC) \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/$(EXEC)_cli
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

clean distclean:
	/bin/rm -rf build_$(CONFIGURE_TARGET)-linux \
	ipkg_$(CONFIGURE_TARGET)-linux/usr

