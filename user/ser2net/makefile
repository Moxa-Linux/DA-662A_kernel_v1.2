# History:
# Date		Author			Comment
# 07-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/ser2net_only


all: build_$(CONFIGURE_TARGET)-linux/ser2net

build_$(CONFIGURE_TARGET)-linux/ser2net: build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf ser2net-2.7.tar.gz
	mv ser2net-2.7 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	CC=$(CROSS)gcc \
	LD=$(CROSS)ld \
	CPP=$(CROSS)cpp \
	STRIP=$(CROSS)strip \
	sh ./configure \
	--host=$(CONFIGURE_TARGET) \
	--build=i686 \
	--prefix="$(TC_INSTALL_PREFIX)" \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux;

romfs: #build_$(CONFIGURE_TARGET)-linux/ser2net
#	$(STRIP) build_$(CONFIGURE_TARGET)-linux/ser2net
#	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/ser2net /usr/sbin/ser2net

package: build_$(CONFIGURE_TARGET)-linux/ser2net
	# Make the ipkg package
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/sbin/
	cp -a build_$(CONFIGURE_TARGET)-linux/ser2net ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/sbin/ser2net
	$(STRIP) -s ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/sbin/ser2net
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_src_root_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


