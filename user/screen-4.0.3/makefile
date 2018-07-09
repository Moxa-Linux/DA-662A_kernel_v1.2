# History:
# Date		Author			Comment
# 06-23-2014	Jared Wu.		First create
#
# compile: 
#	make user/screen-4.0.3_only

all: build_$(CONFIGURE_TARGET)-linux/screen


build_$(CONFIGURE_TARGET)-linux/screen: build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf screen-4.0.3.tar.bz2
	mv screen-4.0.3 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	sed -i 's/{ (exit 1); exit 1; };//g' configure; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	CC="$(CC)" \
	LD="$(LD)" \
	sh ./configure \
	--host=$(CONFIGURE_TARGET) \
	--build=i686 \
	--prefix="$(TC_INSTALL_PREFIX)" \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux;

romfs:
	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/screen
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/screen /usr/bin/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/debian/screenrc /etc/
	$(ROMFSINST) ipkg_$(CONFIGURE_TARGET)-linux/etc/init.d/screen-cleanup /etc/init.d/

package: build_$(CONFIGURE_TARGET)-linux/screen
	# Make the ipkg package
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/bin \
	ipkg_$(CONFIGURE_TARGET)-linux/etc
	cp -a build_$(CONFIGURE_TARGET)-linux/screen ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/debian/screenrc ipkg_$(CONFIGURE_TARGET)-linux/etc/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/screen
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


