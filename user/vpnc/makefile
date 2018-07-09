# History:
# Date		Author			Comment
# 01-6-2010	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# Pre-requirement:
#	libgpg-error, libgcrypt
#
# compile: 
#	make user/vpnc_only

all: $(TC_INSTALL_PREFIX)/usr/local/sbin/vpnc
	echo "Build Complete!"

$(TC_INSTALL_PREFIX)/usr/local/sbin/vpnc:build_$(CONFIGURE_TARGET)-linux/vpnc
	make -C build_$(CONFIGURE_TARGET)-linux DESTDIR=$(TC_INSTALL_PREFIX) install

build_$(CONFIGURE_TARGET)-linux/vpnc:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf vpnc-0.5.3.tar.gz
	mv vpnc-0.5.3 build_$(CONFIGURE_TARGET)-linux
	sed -i 's/#OPENSSL_GPL_VIOLATION/OPENSSL_GPL_VIOLATION/g' build_$(CONFIGURE_TARGET)-linux/Makefile
	sed -i 's/\$\(shell libgcrypt-config \-\-cflags\)//g' build_$(CONFIGURE_TARGET)-linux/Makefile
	sed -i 's/\$\(shell libgcrypt-config \-\-libs\)//g' build_$(CONFIGURE_TARGET)-linux/Makefile
	sed -i 's/vpnc.8 vpnc-script/vpnc-script/g' build_$(CONFIGURE_TARGET)-linux/Makefile
	sed -i 's/install -m644 vpnc.8/#install -m644 vpnc.8/g' build_$(CONFIGURE_TARGET)-linux/Makefile
	sed -i 's/install -m644 pcf2vpnc.1/#install -m644 pcf2vpnc.1/g' build_$(CONFIGURE_TARGET)-linux/Makefile
	sed -i 's/install -m644 cisco-decrypt.1/#install -m644 cisco-decrypt.1/g' build_$(CONFIGURE_TARGET)-linux/Makefile
	sed -i 's/install -m644 COPYING/#install -m644 COPYING/g' build_$(CONFIGURE_TARGET)-linux/Makefile
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	make CC="$(CROSS)gcc -I$(TC_INSTALL_PREFIX)/include" \
	LD=$(CROSS)ld \
	LDFLAGS="-L$(TC_INSTALL_PREFIX)/lib -lgcrypt -lgpg-error -lcrypto" \
	)

romfs:

package: $(TC_INSTALL_PREFIX)/usr/local/sbin/vpnc
	mkdir -p ipkg_vpnc_$(CONFIGURE_TARGET)-linux/bin
	mkdir -p ipkg_vpnc_$(CONFIGURE_TARGET)-linux/sbin
	mkdir -p ipkg_vpnc_$(CONFIGURE_TARGET)-linux/etc
	cp -a $(TC_INSTALL_PREFIX)/usr/local/sbin/vpnc* ipkg_vpnc_$(CONFIGURE_TARGET)-linux/sbin/
	cp -a $(TC_INSTALL_PREFIX)/usr/local/bin/pcf2vpnc ipkg_vpnc_$(CONFIGURE_TARGET)-linux/bin/
	cp -a $(TC_INSTALL_PREFIX)/usr/local/bin/cisco-decrypt ipkg_vpnc_$(CONFIGURE_TARGET)-linux/bin/
	cp -a $(TC_INSTALL_PREFIX)/etc/vpnc ipkg_vpnc_$(CONFIGURE_TARGET)-linux/etc/
	$(STRIP) -s ipkg_vpnc_$(CONFIGURE_TARGET)-linux/bin/cisco-decrypt \
		ipkg_vpnc_$(CONFIGURE_TARGET)-linux/sbin/vpnc
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_vpnc_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

clean distclean: 
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf ipkg_vpnc_$(CONFIGURE_TARGET)-linux/bin \
		ipkg_vpnc_$(CONFIGURE_TARGET)-linux/sbin \
		ipkg_vpnc_$(CONFIGURE_TARGET)-linux/etc

