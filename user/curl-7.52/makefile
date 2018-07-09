# History:
# Date		Author			Comment
# 10-17-2013	Jared Wu.		First Created
#
# compile: 
#	make user/curl-7.52.1_only

all: build_$(CONFIGURE_TARGET)-linux/src/curl

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjf curl-7.52.1.tar.bz2
	mv curl-7.52.1 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	./configure \
	CC="$(CC)" \
	CPP="$(CPP)" \
	LD="$(CC)" \
	STRIP="$(STRIP)" \
	--host=$(CONFIGURE_TARGET) \
	--build=i686 \
	--prefix="$(TC_INSTALL_PREFIX)" \
	--disable-shared \
	--enable-static \
	--without-ssl \
	--without-libidn \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured

build_$(CONFIGURE_TARGET)-linux/src/curl: build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux \
	ipkg_$(CONFIGURE_TARGET)/usr

romfs:
	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/src/curl
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/curl /usr/bin/curl

package:
	# Make the ipkg package
	mkdir -p ipkg_$(CONFIGURE_TARGET)/usr/bin
	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/src/curl
	cp -a build_$(CONFIGURE_TARGET)-linux/src/curl ipkg_$(CONFIGURE_TARGET)/usr/bin/
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET) $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)

