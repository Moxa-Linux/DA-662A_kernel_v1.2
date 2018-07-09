# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/openssl_only

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)/install-me-here
TARGET_SRC=openssl-0.9.8r


ifeq ("$(CONFIGURE_TARGET)","xscale")
	ENDIAN_FLAG := B_ENDIAN
else
	ENDIAN_FLAG := L_ENDIAN
endif


all: $(INSTALL_PREFIX)/lib/libssl.so


build_$(CONFIGURE_TARGET)/configured:
	rm -rf build_$(CONFIGURE_TARGET)
	tar xzvf $(TARGET_SRC).tar.gz
	mv $(TARGET_SRC) build_$(CONFIGURE_TARGET)
	(cd build_$(CONFIGURE_TARGET); \
	patch -p1 --verbose < ../patches/openssl-0.9.8a-defaults.patch; \
	patch -p1 --verbose < ../patches/openssl-0.9.8a-link-krb5.patch; \
	patch -p1 --verbose < ../patches/openssl-0.9.8a-no-rpath.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-algo-doc.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-apps-dgst.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-ciph-sort.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2010-0433.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2011-4109.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2011-4576.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2011-4619.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2012-0884.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2012-2110.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2012-2333.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2013-0166.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2014-0221.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2014-0224.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2014-3505.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2014-3510.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2015-0204.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2015-0287.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2015-0288.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2015-0289.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2015-0292.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2015-0293.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2015-1789.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2015-1790.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2015-3195.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2015-3197.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2015-4000.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-cve-2016-0797.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-default-paths.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-dh-1024.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8e-disable-sslv2.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8r-fix-buffer-overrun-in-ASN1_parse.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8r-cve-2016-0702.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8r-cve-2016-2105.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8r-cve-2016-2106.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8r-cve-2016-2108.patch; \
	patch -p1 --verbose < ../patches/openssl-fips-0.9.8r-cve-2016-2109.patch; \
	)
	(cd build_$(CONFIGURE_TARGET); \
	patch -Np1 -i ../ocf-linux-20110720/patches/openssl-0.9.8r.patch; \
	patch -p1 < ../Vincent.patch; \
	mkdir -p include/crypto; \
	cp -a ../ocf-linux-20110720/ocf/cryptodev.h include/crypto/ ;\
	./Configure -DDISABLE_CFB_OFB -ldl shared linux-ppc no-hw no-asm no-dso no-RC2 no-RC4 threads --with-cryptodev --prefix=$(INSTALL_PREFIX); \
	)
	sed -i 's/DB_ENDIAN/$(ENDIAN_FLAG)/g' build_$(CONFIGURE_TARGET)/Makefile
	sed -i 's/$$(MAKE) $$(SHARED_LIB)/$$(MAKE) -f Makefile $$(SHARED_LIB)/g' build_$(CONFIGURE_TARGET)/crypto/Makefile
	sed -i 's/$$(MAKE) $$(SHARED_LIB)/$$(MAKE) -f Makefile $$(SHARED_LIB)/g' build_$(CONFIGURE_TARGET)/ssl/Makefile;
	sed -i 's/$$(LIBSSL) $$(LIBKRB5) $$(LIBCRYPTO)/$$(LIBSSL) -L..\/ $$(LIBKRB5) $$(LIBCRYPTO)/g' build_$(CONFIGURE_TARGET)/apps/Makefile
	sed -i 's/$$(LIBSSL) $$(LIBCRYPTO) $$(LIBKRB5)/$$(LIBSSL) -L..\/ $$(LIBCRYPTO) $$(LIBKRB5)/g' build_$(CONFIGURE_TARGET)/test/Makefile
	sed -i 's/$$(MAKE) SHLIBDIRS=crypto build-shared;/$$(MAKE) -f Makefile SHLIBDIRS=crypto build-shared;/g' build_$(CONFIGURE_TARGET)/Makefile
	sed -i 's/$$(MAKE) SHLIBDIRS=ssl/$$(MAKE) -f Makefile SHLIBDIRS=ssl/g' build_$(CONFIGURE_TARGET)/Makefile
	sed -i 's/rc2 rc4//g' build_$(CONFIGURE_TARGET)/Makefile
	#sed -i 's/des aes/aes/g' build_$(CONFIGURE_TARGET)/Makefile
	#sed -i 's/rand err/err/g' build_$(CONFIGURE_TARGET)/Makefile
	#sed -i 's/evp asn1 pem/asn1/g' build_$(CONFIGURE_TARGET)/Makefile
	sed -i 's/test tools/tools/g' build_$(CONFIGURE_TARGET)/Makefile
	touch build_$(CONFIGURE_TARGET)/configured

build_$(CONFIGURE_TARGET)/libssl.so.0.9.8: build_$(CONFIGURE_TARGET)/configured
	$(MAKE) CC="$(CC)" CXX="$(CXX)" LD="$(LD)" CPP="$(CPP)" STRIP="$(STRIP)" RANLIB="$(RANLIB)" -f Makefile -C build_$(CONFIGURE_TARGET)

$(INSTALL_PREFIX)/lib/libssl.so:build_$(CONFIGURE_TARGET)/libssl.so.0.9.8
	$(MAKE) CC="$(CC)" CXX="$(CXX)" LD="$(LD)" CPP="$(CPP)" STRIP="$(STRIP)" RANLIB="$(RANLIB)" -f Makefile -C build_$(CONFIGURE_TARGET) install


# install into toolchain
install:
	cp -Rf $(INSTALL_PREFIX)/* $(TC_INSTALL_PREFIX)


# install to romfs
romfs:
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/openssl /bin/openssl
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libssl.so.0.9.8" /lib
	$(ROMFSINST) -s libssl.so.0.9.8 /lib/libssl.so.0
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libcrypto.so*" /lib
	$(ROMFSINST) -s libcrypto.so.0.9.8 /lib/libcrypto.so.0
	$(STRIP) -s $(ROMFSDIR)/bin/openssl \
	$(ROMFSDIR)/lib/libssl.so.0.9.8 \
	$(ROMFSDIR)/lib/libcrypto.so.0.9.8

package:
	echo "Nothing to do"

# copy from toolchain to romfs
tc2romfs:
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/openssl /bin/openssl
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libssl.so.0.9.8" /lib
	$(ROMFSINST) -s libssl.so.0.9.8 /lib/libssl.so.0
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libcrypto.so.0.9.8" /lib
	$(ROMFSINST) -s libcrypto.so.0.9.8 /lib/libcrypto.so.0
	$(STRIP) -s $(ROMFSDIR)/bin/openssl \
	$(ROMFSDIR)/lib/libssl.so.0.9.8 \
	$(ROMFSDIR)/lib/libcrypto.so.0.9.8


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/libssl.so
	cp -a $(INSTALL_PREFIX)/include/*  $(TC_INSTALL_PREFIX)/include
	cp -a $(INSTALL_PREFIX)/lib/*.so*  $(TC_INSTALL_PREFIX)/lib
	cp -a $(INSTALL_PREFIX)/lib/*.a  $(TC_INSTALL_PREFIX)/lib


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)

