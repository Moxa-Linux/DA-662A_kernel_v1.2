# History:
# Date			Author			Comment
# 01-06-2010    Jared Wu.       install at BERoot or LERoot
# 09-11-2011    Aeluin Chen     complete the installation
#                               --disable-mibs is required for snmptrapd
# 08-28-2013	Simon Wang		Remove if-else for each product, and use PRODUCT_ID
#
# Pre-requirement:
#	None
#
# compile: 
#	make user/net-snmp_only

# get the first numbers for product id
PRODUCT_ID=$(shell echo $(CONFIG_PRODUCT) | sed 's/[a-zA-Z]\+/ /g' | sed 's/-/ /g' | awk '{print $$1}' )

PACKAGE=net-snmp-5.7.1
INSTALL_PREFIX=$(CURDIR)/build_$(CONFIGURE_TARGET)-linux

ifeq ($(CONFIG_PRODUCT), rnas1200)
MIB_MODULES="rnas1200 mibII agent_mibs ucd_snmp util_funcs utilities/execute agent/extend ucd-snmp/extensible ucd-snmp/pass ucd-snmp/dlmod"
else
MIB_MODULES="mibII agent_mibs ucd_snmp util_funcs utilities/execute agent/extend ucd-snmp/extensible ucd-snmp/pass ucd-snmp/dlmod"
endif

.PHONY: mib_module

all: build_$(CONFIGURE_TARGET)-linux/usr/sbin/snmpd mib_modules
	echo "Build Complete!"

mib_modules:
	-@[ -d $(CURDIR)/dist/mib_modules/$(CONFIG_PRODUCT) ] && make -C $(CURDIR)/dist/mib_modules/$(CONFIG_PRODUCT)

build_$(CONFIGURE_TARGET)-linux/usr/sbin/snmpd: build_$(CONFIGURE_TARGET)-linux/Makefile
	( cd build_$(CONFIGURE_TARGET)-linux; \
	make; \
	make install )

build_$(CONFIGURE_TARGET)-linux/Makefile:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf $(PACKAGE).tar.gz
	mv $(PACKAGE) build_$(CONFIGURE_TARGET)-linux
	@if [ "$$CONFIG_PRODUCT" = "rnas1200" ]; then \
		cp -r ./dist/mib_modules/rnas1200 ./build_$(CONFIGURE_TARGET)-linux/agent/mibgroup/rnas1200; \
		cp ./dist/mib_modules/rnas1200.h ./build_$(CONFIGURE_TARGET)-linux/agent/mibgroup/; \
		cp ./dist/mib_modules/RNAS-MIB.txt ./build_$(CONFIGURE_TARGET)-linux/mibs/; \
	fi
	( cd build_$(CONFIGURE_TARGET)-linux; \
	patch -p1 -i ../net-snmp-5.7.1-ucd-snmp-pass-requires-execute.patch; \
	patch -p1 -i ../net-snmp-snmplib.patch; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	CPPFLAGS="-Dlinux=1" \
	CC=$(CROSS)gcc \
	CXX=$(CROSS)g++ \
	CPP=$(CROSS)cpp \
	LD=$(CROSS)ld \
	LDFLAGS="-L$(TC_INSTALL_PREFIX)/lib -lsqlite3" \
	STRIP=$(CROSS)strip \
	./configure \
	--build=i686 \
	--host=$(CONFIGURE_TARGET)-linux \
	--prefix=/usr/local \
	--with-install-prefix=$(CURDIR)/build_$(CONFIGURE_TARGET)-linux \
	--sysconfdir=/etc \
	--with-mibdirs=/etc/snmp/mibs:/usr/local/share/snmp/mibs \
	--with-cc=$(CROSS)gcc \
	--with-linkcc=$(CROSS)gcc \
	--with-ar=$(CROSS)ar \
	--enable-shared \
	--with-enterprise-oid=661 \
	--with-enterprise-sysoid=.1.3.6.1.4.1.8691.12.$(PRODUCT_ID) \
	--disable-debugging \
	--disable-des \
	--disable-privacy \
	--disable-manuals \
	--disable-snmptrapd-subagent \
	--disable-embedded-perl \
	--with-sys-contact="support@moxa.com" \
	--with-sys-location="New Taipei City, Taiwan" \
	--with-logfile="/dev/null" \
	--with-persistent-directory="/var/net-snmp" \
	--with-copy-persistent-files="no" \
	--with-default-snmp-version="2" \
	--with-endianness=big \
	--without-root-access \
	--enable-mini-agent \
	--with-out-mib-modules="snmpv3mibs agentx" \
	--with-mib-modules=$(MIB_MODULES) \
	--without-perl-modules )

romfs:
	[ "$(CONFIG_PRODUCT)" = "da662a" ] && sed 's/9400/6620/g' dist/snmpd.conf > build_$(CONFIGURE_TARGET)-linux/snmpd.conf
	mkdir -p $(ROMFSDIR)/etc/snmp
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/snmpd.conf /etc/snmp/snmpd.conf
	$(ROMFSINST) dist/snmptrapd.conf /etc/snmp/snmptrapd.conf
	$(ROMFSINST) dist/passtest /etc/snmp/passtest
	$(ROMFSINST) dist/passtest2 /etc/snmp/passtest2
	( cd build_$(CONFIGURE_TARGET)-linux/usr/local/share/snmp; \
	$(ROMFSINST) /etc/snmp/mibs )
	-[ "$(CONFIG_PRODUCT)" = "uc1260" ] || ( cd dist; \
	$(ROMFSINST) /etc/init.d/snmpd; \
	$(ROMFSINST) /etc/init.d/snmptrapd )
	-[ "$(CONFIG_PRODUCT)" = "uc1260" ] && ( cd dist; \
	$(ROMFSINST) /etc/init.d/snmp; \
	$(ROMFSINST) /etc/init.d/snmptrap )
	( cd build_$(CONFIGURE_TARGET)-linux/usr/local/lib; \
	$(ROMFSINST) "libnetsnmp*.so*" /lib )
	( cd build_$(CONFIGURE_TARGET)-linux/usr/local/sbin; \
	$(ROMFSINST) /usr/sbin/snmptrapd; \
	$(ROMFSINST) /usr/sbin/snmpd )
	( cd build_$(CONFIGURE_TARGET)-linux/usr/local/bin; \
	$(ROMFSINST) /usr/bin/snmptrap; \
	$(ROMFSINST) /usr/bin/snmpbulkwalk; \
	$(ROMFSINST) /usr/bin/snmpget; \
	$(ROMFSINST) /usr/bin/snmpset )
	$(STRIP) -s $(ROMFSDIR)/usr/sbin/snmp*
	$(STRIP) -s $(ROMFSDIR)/usr/bin/snmp*
	-@[ -d $(CURDIR)/dist/mib_modules/$(CONFIG_PRODUCT) ] && mkdir -p $(ROMFSDIR)/usr/lib/mib_modules && cp -a $(CURDIR)/dist/mib_modules/$(CONFIG_PRODUCT)/$(CONFIG_PRODUCT).so $(ROMFSDIR)/usr/lib/mib_modules/

package: build_$(CONFIGURE_TARGET)-linux/usr/local/sbin/snmpd
	mkdir -p ipkg_snmp_$(CONFIGURE_TARGET)-linux/usr/local/lib
	mkdir -p ipkg_snmp_$(CONFIGURE_TARGET)-linux/usr/local/sbin
	mkdir -p ipkg_snmp_$(CONFIGURE_TARGET)-linux/usr/local/bin
	mkdir -p ipkg_snmp_$(CONFIGURE_TARGET)-linux/usr/local/share/snmp
	mkdir -p ipkg_snmp_$(CONFIGURE_TARGET)-linux/etc/init.d
	mkdir -p ipkg_snmp_$(CONFIGURE_TARGET)-linux/etc/snmp
	### Patch the OID in snmpd.conf ####
	[ "$(CONFIG_PRODUCT)" = "em1280" ] && sed 's/8410/1280/g' dist/snmpd.conf > ipkg_snmp_$(CONFIGURE_TARGET)-linux/etc/snmp/snmpd.conf; \
	[ "$(CONFIG_PRODUCT)" = "em2260" -o "$(CONFIG_PRODUCT)" = "em2260-xj" -o "$(CONFIG_PRODUCT)" = "em2260cdxj" ] && sed 's/8410/2260/g' dist/snmpd.conf > ipkg_snmp_$(CONFIGURE_TARGET)-linux/etc/snmp/snmpd.conf; \
	[ "$(CONFIG_PRODUCT)" = "ia260" ] && sed 's/8410/260/g' dist/snmpd.conf > ipkg_snmp_$(CONFIGURE_TARGET)-linux/etc/snmp/snmpd.conf; \
	[ "$(CONFIG_PRODUCT)" = "ia261" -o "$(CONFIG_PRODUCT)" = "ia261-hoson" -o "$(CONFIG_PRODUCT)" = "ia261-shw" -o "$(CONFIG_PRODUCT)" = "ia261-xj" -o "$(CONFIG_PRODUCT)" = "ia261r256" ] && sed 's/8410/261/g' dist/snmpd.conf > ipkg_snmp_$(CONFIGURE_TARGET)-linux/etc/snmp/snmpd.conf; \
	[ "$(CONFIG_PRODUCT)" = "ia262" ] && sed 's/8410/262/g' dist/snmpd.conf > ipkg_snmp_$(CONFIGURE_TARGET)-linux/etc/snmp/snmpd.conf; \
	[ "$(CONFIG_PRODUCT)" = "uc8400" ] && sed 's/8410/8410/g' dist/snmpd.conf > ipkg_snmp_$(CONFIGURE_TARGET)-linux/etc/snmp/snmpd.conf; \
	[ "$(CONFIG_PRODUCT)" = "uc8411" ] && sed 's/8410/8411/g' dist/snmpd.conf > ipkg_snmp_$(CONFIGURE_TARGET)-linux/etc/snmp/snmpd.conf; \
	[ "$(CONFIG_PRODUCT)" = "uc8430" ] && sed 's/8410/8430/g' dist/snmpd.conf > ipkg_snmp_$(CONFIGURE_TARGET)-linux/etc/snmp/snmpd.conf; \
	[ "$(CONFIG_PRODUCT)" = "dcu8620" ] && sed 's/8410/8620/g' dist/snmpd.conf > ipkg_snmp_$(CONFIGURE_TARGET)-linux/etc/snmp/snmpd.conf; \
	[ "$(CONFIG_PRODUCT)" = "da662a" ] && sed 's/9400/6620/g' dist/snmpd.conf > ipkg_snmp_$(CONFIGURE_TARGET)-linux/etc/snmp/snmpd.conf; \
	[ "$(CONFIG_PRODUCT)" = "w406" ] && sed 's/8410/406/g' dist/snmpd.conf > ipkg_snmp_$(CONFIGURE_TARGET)-linux/etc/snmp/snmpd.conf; \
	cp -a dist/snmptrapd.conf ipkg_snmp_$(CONFIGURE_TARGET)-linux/etc/snmp/snmptrapd.conf
	cp -a dist/snmpd.pkg ipkg_snmp_$(CONFIGURE_TARGET)-linux/etc/init.d/snmpd
	cp -a dist/snmptrapd.pkg ipkg_snmp_$(CONFIGURE_TARGET)-linux/etc/init.d/snmptrapd
	cp -a dist/passtest ipkg_snmp_$(CONFIGURE_TARGET)-linux/etc/snmp
	cp -a dist/passtest2 ipkg_snmp_$(CONFIGURE_TARGET)-linux/etc/snmp
	cp -a build_$(CONFIGURE_TARGET)-linux/usr/local/sbin/snmpd ipkg_snmp_$(CONFIGURE_TARGET)-linux/usr/local/sbin/
	cp -a build_$(CONFIGURE_TARGET)-linux/usr/local/sbin/snmptrapd ipkg_snmp_$(CONFIGURE_TARGET)-linux/usr/local/sbin/
	cp -a build_$(CONFIGURE_TARGET)-linux/usr/local/bin/snmptrap ipkg_snmp_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/usr/local/bin/snmpbulkwalk ipkg_snmp_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/usr/local/bin/snmpwalk ipkg_snmp_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/usr/local/bin/snmpget ipkg_snmp_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/usr/local/bin/snmpset ipkg_snmp_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -ax build_$(CONFIGURE_TARGET)-linux/usr/local/lib/lib*.so* ipkg_snmp_$(CONFIGURE_TARGET)-linux/usr/local/lib
	cp -R build_$(CONFIGURE_TARGET)-linux/usr/local/share/snmp/mibs ipkg_snmp_$(CONFIGURE_TARGET)-linux/etc/snmp
	$(STRIP) -s ipkg_snmp_$(CONFIGURE_TARGET)-linux/usr/local/sbin/* \
		ipkg_snmp_$(CONFIGURE_TARGET)-linux/usr/local/bin/* \
		ipkg_snmp_$(CONFIGURE_TARGET)-linux/usr/local/lib/*
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_snmp_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

clean distclean: 
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf ipkg_snmp_$(CONFIGURE_TARGET)-linux/usr \
		ipkg_snmp_$(CONFIGURE_TARGET)-linux/lib \
		ipkg_snmp_$(CONFIGURE_TARGET)-linux/man
	-@[ -d $(CURDIR)/dist/mib_modules/$(CONFIG_PRODUCT) ] && make -C $(CURDIR)/dist/mib_modules/$(CONFIG_PRODUCT) clean

