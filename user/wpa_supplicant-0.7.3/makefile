# History:
# Date		Author		Comment
# 09-21-2011	Jared Wu.	install at BERoot/LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
# 10-24-2012	Jared Wu.	Patched for building libeap.so which works with wimax
# 
#
# Pre-requirement:
#	ncurses ( has support in toolchain arm-linux-gcc v.4.4.2 )
#
# compile: 
#	make user/wpa_supplicant_only


all: build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_supplicant
	echo "Build Complete!"
	# 10-24-2012, Jared Wu, Patched for building libeap.so which works with wimax
	CC=$(CC) \
	LD=$(LD) \
	DESTDIR=$(TC_INSTALL_PREFIX) \
	make -C build_$(CONFIGURE_TARGET)-linux/src/eap_peer install


build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_supplicant:build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/.config
	CC=$(CC) \
	LD=$(LD) \
	make -C build_$(CONFIGURE_TARGET)-linux/wpa_supplicant
	# 10-24-2012, Jared Wu, Patched for building libeap.so which works with wimax
	CC=$(CC) \
	LD=$(LD) \
	DESTDIR=$(TC_INSTALL_PREFIX) \
	make -C build_$(CONFIGURE_TARGET)-linux/src/eap_peer

build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/.config:
	-rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf wpa_supplicant-0.7.3.tar.gz
	mv wpa_supplicant-0.7.3 build_$(CONFIGURE_TARGET)-linux
	cp -a build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/defconfig build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/.config
	# 10-24-2012, Jared Wu, Patched for building libeap.so which works with wimax
	(cd build_$(CONFIGURE_TARGET)-linux; patch -p1 < ../wpa_supplicant-0.7.2-generate-libeap-peer.patch_)

	#sed -i 's/^#CONFIG_DRIVER_MADWIFI=y/CONFIG_DRIVER_MADWIFI=y/g' build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/.config
	#sed -i 's/#CFLAGS \+\= -I..\/..\/madwifi/CFLAGS += -I..\/..\/madwifi/g' build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/.config

romfs: build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_supplicant
	$(STRIP) build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_supplicant
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_supplicant /bin/wpa_supplicant
	$(STRIP) build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_passphrase
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_passphrase /bin/wpa_passphrase
	$(STRIP) build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_cli
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_cli /bin/wpa_cli
	#$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/wpa_supplicant/wpa_supplicant.conf /etc/wpa_supplicant.conf
	#sed -i '557,$$ d' $(ROMFSDIR)/etc/wpa_supplicant.conf
	# 10-24-2012, Jared Wu, Patched for building libeap.so which works with wimax
	[ -f build_$(CONFIGURE_TARGET)-linux/src/eap_peer/libeap.so.0.0.0 ] || $(STRIP) build_$(CONFIGURE_TARGET)-linux/src/eap_peer/libeap.so.0.0.0
	$(ROMFSINST) -e CONFIG_USER_WIMAX_SERVICE build_$(CONFIGURE_TARGET)-linux/src/eap_peer/libeap.so.0.0.0 /lib/libeap.so.0
	


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
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf ipkg_wpa_$(CONFIGURE_TARGET)-linux/bin
	rm -rf ipkg_wpa_$(CONFIGURE_TARGET)-linux/etc

