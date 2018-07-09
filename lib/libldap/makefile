#
# build using configure as best we can, should work for most targets
# David McCullough <davidm@snapgear.com>
#
# History:
# Date		Author			Comment
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/libldap_only
#

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install-me-here
TARGET_SRC=openldap-2.4.23


all: $(INSTALL_PREFIX)/lib/libldap.so

untar-all-source:
	if [ ! -d $(TARGET_SRC) ]; then \
		[ ! -f $(TARGET_SRC).tgz ] && \
			wget ftp://ftp.openldap.org/pub/OpenLDAP/openldap-release/$(TARGET_SRC).tgz; \
		[ -f $(TARGET_SRC).tgz ] && tar xzf $(TARGET_SRC).tgz; \
		(cd $(TARGET_SRC); \
			patch -p1 -i ../$(TARGET_SRC).patch; \
		) \
	fi


build:
	if [ -d "build_$(CONFIGURE_TARGET)-linux" ]; then \
		rm -rf build_$(CONFIGURE_TARGET)-linux; \
	fi
	mkdir -p build_$(CONFIGURE_TARGET)-linux
	(cd build_$(CONFIGURE_TARGET)-linux; \
	CFLAGS="$(subst -Wall,,$(CFLAGS))" \
	LDFLAGS="-lc" \
	CPPFLAGS="" \
	STRIP=$(CROSS)strip \
	../$(TARGET_SRC)/configure \
		--prefix=$(INSTALL_PREFIX) \
		--build=i686-linux \
		--host=$(CONFIGURE_TARGET) \
		--with-headers=$(ROOTDIR)/$(LINUXDIR)/include	\
		--target=$(CONFIGURE_TARGET) \
		--without-threads \
		--without-local \
		--disable-ipv6 \
		--enable-x-compile \
		--disable-slapd \
		--disable-slurpd; \
		make depend; \
	)
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install


# install into toolchain
install:
	# ldap already stripped after install
	cp -R $(INSTALL_PREFIX)/* $(TC_INSTALL_PREFIX)


# install to romfs
romfs:
	$(ROMFSINST) "$(INSTALL_PREFIX)/lib/liblber*.so*" /lib
	$(ROMFSINST) "$(INSTALL_PREFIX)/lib/libldap*.so*" /lib
	$(ROMFSINST) "$(INSTALL_PREFIX)/bin/ldap*" /bin
	$(ROMFSINST) "$(INSTALL_PREFIX)/etc/openldap" /etc


# copy from toolchain to romfs
tc2romfs: $(TC_INSTALL_PREFIX)/lib/libldap.so
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/liblber*.so*" /lib
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/libldap*.so*" /lib
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/bin/ldap*" /bin
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/etc/openldap" /etc


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/libldap.so
	cp -a $(INSTALL_PREFIX)/include/*  $(TC_INSTALL_PREFIX)/include
	cp -a $(INSTALL_PREFIX)/lib/*.so*  $(TC_INSTALL_PREFIX)/lib
	cp -a $(INSTALL_PREFIX)/lib/*.a  $(TC_INSTALL_PREFIX)/lib


distclean clean:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf $(TARGET_SRC)

