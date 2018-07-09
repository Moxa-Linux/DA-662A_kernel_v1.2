# History:
# Date		Author		Comment
# 11-05-2012	Jared Wu.	wget to support ftp login
#
# compile: 
#	make user/wget_only


all: $(TC_INSTALL_PREFIX)/bin/wget

$(TC_INSTALL_PREFIX)/bin/wget: build_$(CONFIGURE_TARGET)-linux/src/wget
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install

build_$(CONFIGURE_TARGET)-linux/src/wget: build_$(CONFIGURE_TARGET)-linux/build
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/build:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf wget_1.12.orig.tar.gz
	mv wget-1.12 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	sh ./configure \
	CC=$(CROSS)gcc \
	LD=$(CROSS)ld \
	CPP=$(CROSS)cpp \
	STRIP=$(CROSS)strip \
	--host=arm-unknown-linux \
	--build=i686 \
	--prefix=$(TC_INSTALL_PREFIX) \
	)
	touch build_$(CONFIGURE_TARGET)-linux/build

romfs: #$(TC_INSTALL_PREFIX)/bin/wget
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/wget /usr/bin/wget
	$(STRIP) $(ROMFSDIR)/usr/bin/wget

package: $(TC_INSTALL_PREFIX)/bin/wget
	# Make the ipkg package
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/bin
	cp -a $(TC_INSTALL_PREFIX)/bin/wget ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/wget
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux \
	  ipkg_$(CONFIGURE_TARGET)-linux/usr/bin
