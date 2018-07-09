# History:
# Date          Author                  Comment
# 06-28-2012    Jared Wu.               new created
#
# compile:
#	make user/hostapd-1.0_only
#
-include $(ARCH_CONFIG)

PACKAGE_NAME=pure-ftpd-1.0.30
EXEC=pure-ftpd

all: $(TC_INSTALL_PREFIX)/sbin/$(EXEC)

$(TC_INSTALL_PREFIX)/sbin/$(EXEC): build_$(CONFIGURE_TARGET)-linux/$(EXEC)/$(EXEC)
	cp -a build_$(CONFIGURE_TARGET)-linux/src/$(EXEC) $(TC_INSTALL_PREFIX)/sbin/
	echo "Install $(EXEC) complete!"

build_$(CONFIGURE_TARGET)-linux/$(EXEC)/$(EXEC): build_$(CONFIGURE_TARGET)-linux/configured
	make -C build_$(CONFIGURE_TARGET)-linux/
	echo "Build $(EXEC) complete!"

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf $(PACKAGE_NAME).tar.gz
	mv $(PACKAGE_NAME) build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	sh ./configure \
	CC="$(CC)" \
	LD="$(LD)" \
	CPP="$(CPP)" \
	STRIP="$(STRIP)" \
	--host=arm-unknown-linux \
	--build=i686 \
	--prefix=$(TC_INSTALL_PREFIX) \
	--with-cookie \
	--with-welcomemsg \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured

	

romfs:
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/sbin/$(EXEC) /sbin/
	$(STRIP) $(ROMFSDIR)/sbin/$(EXEC)

package: $(TC_INSTALL_PREFIX)/sbin/$(EXEC)
	# Make the ipkg package
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/sbin
	cp -a ipkg_default/* ipkg_$(CONFIGURE_TARGET)-linux/
	cp -a $(TC_INSTALL_PREFIX)/sbin/$(EXEC) ipkg_$(CONFIGURE_TARGET)-linux/sbin/
	#cp -a $(TC_INSTALL_PREFIX)/sbin/$(EXEC)_cli ipkg_$(CONFIGURE_TARGET)-linux/sbin/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/sbin/$(EXEC)
	#$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/sbin/$(EXEC)_cli
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

clean distclean:
	/bin/rm -rf build_$(CONFIGURE_TARGET)-linux \
	ipkg_$(CONFIGURE_TARGET)-linux/sbin

