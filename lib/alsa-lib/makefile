# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/alsa-lib_only
#

all: build_$(CONFIGURE_TARGET)-linux/src/.libs/libasound.so.2.0.0

build_$(CONFIGURE_TARGET)-linux/src/.libs/libasound.so.2.0.0: build_$(CONFIGURE_TARGET)-linux/Makefile
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET)-linux LD=$(CC) -j 2;

build_$(CONFIGURE_TARGET)-linux/Makefile: makefile
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjf alsa-lib-1.0.16.tar.bz2
	mv alsa-lib-1.0.16 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	./configure \
	CC=$(CROSS)gcc \
	LD=$(CROSS)ld \
	CFLAGS="-DCLOCK_MONOTONIC=1" \
	--build=i686-linux \
	--host=$(CONFIGURE_TARGET)-linux \
	--target=$(CONFIGURE_TARGET)-linux \
	--enable-shared \
	--disable-python \
	--with-pic \
	--with-versioned \
	)

romfs:
	echo 'nothing'
#	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/.libs/libasound.so.2.0.0 /lib/libasound.so.2.0.0
#	$(ROMFSINST) -s libasound.so.2.0.0 /lib/libasound.so.2

package: build_$(CONFIGURE_TARGET)-linux/src/.libs/libasound.so.2.0.0
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib
	cp -a build_$(CONFIGURE_TARGET)-linux/src/.libs/libasound.so.2.0.0 ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/
	$(STRIP) -s ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/libasound.so.2.0.0
	(cd ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib ; \
	ln -sf libasound.so.2.0.0 libasound.so; \
	ln -sf libasound.so.2.0.0 libasound.so.2; \
	)
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_src_root_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


clean distclean:
	rm -fr build_*
	rm -fr ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/libasound.so
