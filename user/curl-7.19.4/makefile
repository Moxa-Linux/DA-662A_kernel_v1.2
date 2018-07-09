# History:
# Date		Author			Comment
# 07-12-2009	Jared Wu.		install at BERoot/LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/curl-7.19.4_only

all: build_$(CONFIGURE_TARGET)-linux/build

build_$(CONFIGURE_TARGET)-linux/build: makefile
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjf curl-7.19.4.tar.bz2
	mv curl-7.19.4 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	./configure \
	CC=$(CROSS)gcc \
	CPP=$(CROSS)cpp \
	LD=$(CROSS)ld \
	STRIP=$(CROSS)strip \
	--host=$(CONFIGURE_TARGET)-linux \
	--build=i686 \
	--prefix="$(TC_INSTALL_PREFIX)" \
	--with-static \
	--without-ssl \
	--without-libidn \
	)
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux
	touch build_$(CONFIGURE_TARGET)-linux/build

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux;

romfs:
	$(STRIP) build_$(CONFIGURE_TARGET)-linux/src/.libs/curl
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/.libs/curl /bin/curl
	$(STRIP) build_$(CONFIGURE_TARGET)-linux/lib/.libs/libcurl.so.4.1.1
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/lib/.libs/libcurl.so.4.1.1 /lib/libcurl.so.4.1.1
	$(ROMFSINST) -s libcurl.so.4.1.1 /lib/libcurl.so.4

package:
	# Make the ipkg package

