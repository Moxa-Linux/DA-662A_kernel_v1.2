# History:
# Date		Author			Comment
# 09-03-2010	Jared Wu.		
#
# compile: 
#	make user/pimd_only

all: build_$(CONFIGURE_TARGET)-linux/pimd

build_$(CONFIGURE_TARGET)-linux/pimd:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf pimd-2.1.0-alpha29.17.tar.gz
	mv pimd-2.1.0-alpha29.17 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	patch -p1 < ../pimd_2.1.0-alpha29.17-8.1.diff; \
	sed -i '95 s/^/#/g' Makefile; \
	sed -i '137 s/#CFLAGS/CFLAGS/g' Makefile; \
	sed -i '25 s/^/\/\//g' include/linux/netinet/in-my.h \
	)
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET)-linux CC=$(CC) LD=$(CC) -j 2;

romfs:
	echo 'nothing'

package: build_$(CONFIGURE_TARGET)-linux/pimd
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/sbin
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc
	cp -a build_$(CONFIGURE_TARGET)-linux/pimd ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/sbin/
	cp -a build_$(CONFIGURE_TARGET)-linux/pimd.conf ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/
	$(STRIP) ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/sbin/*
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_src_root_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


clean distclean:
	rm -fr build_$(CONFIGURE_TARGET)-linux
	rm -fr ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/sbin/*
	rm -fr ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/*
