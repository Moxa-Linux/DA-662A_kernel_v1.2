# History:
# Date		Author			Comment
# 06-23-2014	Jared Wu.		First created
#
# compile: 
#	make user/nano_only

all: build_$(CONFIGURE_TARGET)-linux/src/nano

build_$(CONFIGURE_TARGET)-linux/src/nano: build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf nano-2.2.4.tar.bz2
	mv nano-2.2.4 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	CC="$(CC)" \
	LD="$(LD)" \
	CPP="$(CPP)" \
	LDFLAGS="-L$(TC_INSTALL_PREFIX)/lib" \
	CFLAGS="-I$(TC_INSTALL_PREFIX)/include/ncurses" \
	sh ./configure \
	--host=$(CONFIGURE_TARGET) \
	--build=i686 \
	--prefix="$(TC_INSTALL_PREFIX)" \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux;

romfs:
	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/src/nano
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/nano /bin/
	$(ROMFSINST) -s /bin/nano /bin/rnano
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/debian/nanorc /etc/

package:
	# Make the ipkg package
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/bin \
	ipkg_$(CONFIGURE_TARGET)-linux/etc
	cp -a build_$(CONFIGURE_TARGET)-linux/bin/nano ipkg_$(CONFIGURE_TARGET)-linux/bin/
	(cd ipkg_$(CONFIGURE_TARGET)-linux/bin/; ln -sf nano rnano)
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/bin/nano
	cp -a debian/nanorc ipkg_$(CONFIGURE_TARGET)-linux/etc/
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

