# History:
# Date		Author			Comment
# 06-23-2014	Jared Wu.		First created
#
# compile: 
#	make lib/pcsc-lite_only
#	make user/OpenSC_only

all: build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/opensc-tool

build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/opensc-tool: build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf opensc-0.13.0.tar.gz
	mv opensc-0.13.0 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	CC="$(CC)" \
	LD="$(LD)" \
	CPP="$(CPP)" \
	CFLAGS="-I$(ROOTDIR)/lib/pcsc-lite/build_$(CONFIGURE_TARGET)-linux/src/PCSC/ -I$(TC_INSTALL_PREFIX)/include" \
	LDFLAGS="-L$(ROOTDIR)/lib/pcsc-lite/build_$(CONFIGURE_TARGET)-linux/src/.libs" \
	sh ./configure \
	--host=$(CONFIGURE_TARGET)-linux \
	--build=i686-linux \
	--prefix="`pwd`/install-me-here" \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux

romfs:
	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/opensc-tool \
	build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/pkcs15-crypt \
	build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/eidenv \
	build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/cryptoflex-tool \
	build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/pkcs15-init \
	build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/piv-tool \
	build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/cardos-tool \
	build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/netkey-tool \
	build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/opensc-explorer \
	build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/pkcs11-tool \
	build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/pkcs15-tool \
	build_$(CONFIGURE_TARGET)-linux/src/libopensc/.libs/libopensc.so.3.0.0
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/opensc-tool /usr/bin/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/pkcs15-crypt /usr/bin/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/eidenv /usr/bin/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/cryptoflex-tool /usr/bin/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/pkcs15-init /usr/bin/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/piv-tool /usr/bin/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/cardos-tool /usr/bin/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/netkey-tool /usr/bin/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/opensc-explorer /usr/bin/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/pkcs11-tool /usr/bin/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/pkcs15-tool /usr/bin/
	$(ROMFSINST) "build_$(CONFIGURE_TARGET)-linux/src/libopensc/.libs/libopensc.so*" /lib/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/pkcs11/.libs/opensc-pkcs11.so /lib/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/pkcs11/.libs/pkcs11-spy.so /lib/
	mkdir -p $(ROMFSDIR)/lib/pkcs11
	$(ROMFSINST) -s /lib/opensc-pkcs11.so /lib/pkcs11/opensc-pkcs11.so
	$(ROMFSINST) -s /lib/pkcs11-spy.so /lib/pkcs11/pkcs11-spy.so

package:
	# Make the ipkg package
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/bin \
	ipkg_$(CONFIGURE_TARGET)-linux/lib
	cp -a build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/opensc-tool ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/pkcs15-crypt ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/eidenv ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/cryptoflex-tool ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/pkcs15-init ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/piv-tool ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/cardos-tool ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/netkey-tool ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/opensc-explorer ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/pkcs11-tool ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/src/tools/.libs/pkcs15-tool ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/src/libopensc/.libs/libopensc.so.3.0.0 ipkg_$(CONFIGURE_TARGET)-linux/lib/
	(cd ipkg_$(CONFIGURE_TARGET)-linux/lib/; \
	ln -sf libopensc.so.3.0.0 libopensc.so.3; \
	ln -sf libopensc.so.3.0.0 libopensc.so; \
	)
	cp -a build_$(CONFIGURE_TARGET)-linux/src/pkcs11/.libs/opensc-pkcs11.so ipkg_$(CONFIGURE_TARGET)-linux/lib/
	(cd ipkg_$(CONFIGURE_TARGET)-linux/lib/pkcs11; \
	ln -sf ../opensc-pkcs11.so opensc-pkcs11.so; \
	)
	cp -a build_$(CONFIGURE_TARGET)-linux/src/pkcs11/.libs/pkcs11-spy.so ipkg_$(CONFIGURE_TARGET)-linux/lib/
	(cd ipkg_$(CONFIGURE_TARGET)-linux/lib/pkcs11; \
	ln -sf ../pkcs11-spy.so pkcs11-spy.so; \
	)
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/opensc-tool \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/pkcs15-crypt \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/eidenv \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/cryptoflex-tool \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/pkcs15-init \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/piv-tool \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/cardos-tool \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/netkey-tool \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/opensc-explorer \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/pkcs11-tool \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/pkcs15-tool \
	ipkg_$(CONFIGURE_TARGET)-linux/lib/libopensc.so.3.0.0 \
	ipkg_$(CONFIGURE_TARGET)-linux/lib/opensc-pkcs11.so \
	ipkg_$(CONFIGURE_TARGET)-linux/lib/pkcs11-spy.so
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

