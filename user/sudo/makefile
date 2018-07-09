# History:
# Date		Author			Comment
# 07-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/sudo_only

all: build_$(CONFIGURE_TARGET)-linux/src/surd

build_$(CONFIGURE_TARGET)-linux/src/surd: build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf sudo-1.8.1p2.tar.gz
	mv sudo-1.8.1p2 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	CC=$(CROSS)gcc \
	LD=$(CROSS)ld \
	CPP=$(CROSS)cpp \
	STRIP=$(CROSS)strip \
	sh ./configure \
	--host=$(CONFIGURE_TARGET)-linux \
	--build=i686 \
	--prefix="$(TC_INSTALL_PREFIX)" \
	--libexecdir="/usr/local/libexec" \
	--with-editor="/bin/vi" \
	--without-pam \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/bin \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/local/libexec \
	ipkg_$(CONFIGURE_TARGET)-linux/lib
	

romfs: #build_$(CONFIGURE_TARGET)-linux/src/surd
#	$(STRIP) build_$(CONFIGURE_TARGET)-linux/src/surd
#	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/surd /usr/sbin/sudo

package: build_$(CONFIGURE_TARGET)-linux/src/surd
	# Making the ipkg package
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/src/sudo ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/sudo
	chmod a+s ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/sudo
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/sudo
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin/
	cp -a build_$(CONFIGURE_TARGET)-linux/plugins/sudoers/visudo ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin/visudo
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin/visudo
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/local/libexec
	cp -a build_$(CONFIGURE_TARGET)-linux/src/.libs/libsudo_noexec.so ipkg_$(CONFIGURE_TARGET)-linux/usr/local/libexec/
	cp -a build_$(CONFIGURE_TARGET)-linux/plugins/sudoers/.libs/sudoers.so ipkg_$(CONFIGURE_TARGET)-linux/usr/local/libexec/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/usr/local/libexec/*
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/lib
	cp -a $(TC_LIBDIR)/libpam.so* ipkg_$(CONFIGURE_TARGET)-linux/lib/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/lib/*
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/etc
	cp -a build_$(CONFIGURE_TARGET)-linux/plugins/sudoers/sudoers ipkg_$(CONFIGURE_TARGET)-linux/etc
	chmod a-w ipkg_$(CONFIGURE_TARGET)-linux/etc/sudoers
	chmod o-r ipkg_$(CONFIGURE_TARGET)-linux/etc/sudoers
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


