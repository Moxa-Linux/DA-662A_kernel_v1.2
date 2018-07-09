# History:
# Date		Author			Comment
# 07-12-2009	Jared Wu.		install at BERoot/LERoot
#
# compile: 
#	make user/ftpd_0.3.3_only

all: $(TC_INSTALL_PREFIX)/bin/ftpd


$(TC_INSTALL_PREFIX)/bin/ftpd: build_$(CONFIGURE_TARGET)-linux/ftpd
	# copy the ftpd
	cp build_$(CONFIGURE_TARGET)-linux/ftpd $(TC_INSTALL_PREFIX)/bin/

build_$(CONFIGURE_TARGET)-linux/ftpd:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf ftpd-bsd-0.3.3.tar.gz
	mv ftpd-bsd-0.3.3 build_$(CONFIGURE_TARGET)-linux
	# Add the SA_LEN macro in ftpd.c
	sed -i '77 a #define SA_LEN(_x)      __libc_sa_len((_x)->sa_family)' build_$(CONFIGURE_TARGET)-linux/ftpd.c
	# Cross compile ftpd
	$(MAKE) CC="$(CROSS)gcc" \
	LD=$(CROSS)ld \
	CFLAGS=" -isystem $(TC_INSTALL_PREFIX)/include -DAUTO_UNCOMPRESS -DINTERNAL_LS -DUSE_SHADOW" \
	LIBS="-lutil -lnsl -lcrypt -ldl" \
	-C build_$(CONFIGURE_TARGET)-linux


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux;

romfs:
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/ftpd /bin/ftpd
	$(STRIP) -s $(ROMFSDIR)/bin/ftpd
	$(ROMFSINST) -e CONFIG_USER_FTPD_FTPD \
		-a "ftp     stream tcp6 nowait root /bin/ftpd -l" /etc/inetd.conf

package: build_$(CONFIGURE_TARGET)-linux/ftpd
	# Make the ipkg package
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/ftpd ipkg_$(CONFIGURE_TARGET)-linux/bin/ftpd
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/bin/ftpd
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


