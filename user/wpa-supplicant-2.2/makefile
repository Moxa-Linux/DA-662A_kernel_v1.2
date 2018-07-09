# History:
# Date		Author		Comment
# 07-04-2014	Jared Wu.	Created for P2P features
#
# compile: 
#	make user/wpa_supplicant-2.2_only


all: build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_supplicant
	echo "Build Complete!"


build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_supplicant:build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/.config
	CC=$(CC) \
	LD=$(LD) \
	CFLAGS="-I$(TC_INSTALL_PREFIX)/include" \
	LDFLAGS="-L$(ROOTDIR)/lib/libnl/build_$(CONFIGURE_TARGET)-linux/lib -lnl" \
	make -C build_$(CONFIGURE_TARGET)-linux/wpa_supplicant
	CC=$(CC) \
	LD=$(LD) \
	CFLAGS="-I$(TC_INSTALL_PREFIX)/include" \
	LDFLAGS="-L$(ROOTDIR)/lib/libnl/build_$(CONFIGURE_TARGET)-linux/lib -lnl" \
	make -C build_$(CONFIGURE_TARGET)-linux/src


build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/.config:
	-rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf wpa_supplicant-2.2.tar.gz
	mv wpa_supplicant-2.2 build_$(CONFIGURE_TARGET)-linux
	cp -a build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/defconfig build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/.config
	sed -i 's/^#CONFIG_DRIVER_NL80211=y/CONFIG_DRIVER_NL80211=y/g' build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/.config
	sed -i 's/^#CONFIG_WPS=y/CONFIG_WPS=y/g' build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/.config
	sed -i 's/^#CONFIG_WPS2=y/CONFIG_WPS2=y/g' build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/.config
	sed -i 's/^#CONFIG_P2P=y/CONFIG_P2P=y/g' build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/.config
	sed -i 's/^#CONFIG_AP=y/CONFIG_AP=y/g' build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/.config

romfs:
	$(STRIP) build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_supplicant
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_supplicant /bin/wpa_supplicant
	$(STRIP) build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_passphrase
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_passphrase /bin/wpa_passphrase
	$(STRIP) build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_cli
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_cli /bin/wpa_cli


package:
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/bin
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/etc
	cp -a build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_supplicant ipkg_$(CONFIGURE_TARGET)-linux/bin/wpa_supplicant
	cp -a build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_passphrase ipkg_$(CONFIGURE_TARGET)-linux/bin/wpa_passphrase
	cp -a build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_cli ipkg_$(CONFIGURE_TARGET)-linux/bin/wpa_cli
	cp -a build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_supplicant.conf ipkg_$(CONFIGURE_TARGET)-linux/etc//wpa_supplicant.conf
	sed -i '557,$$ d' ipkg_$(CONFIGURE_TARGET)-linux/etc/wpa_supplicant.conf
	$(STRIP) ipkg_$(CONFIGURE_TARGET)-linux/bin/*
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

clean distclean: 
	rm -rf build_$(CONFIGURE_TARGET)-linux \
	ipkg_wpa_$(CONFIGURE_TARGET)-linux/bin \
	ipkg_wpa_$(CONFIGURE_TARGET)-linux/etc

