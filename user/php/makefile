# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# Pre-requirement:
#	apt-get install -y bison flex libxml2 libxml2-dev btyacc libjpeg62 libjpeg-dev
#
# compile: 
#	make user/php_only

GCC_INC_PATH := $(TC_INSTALL_PREFIX)/include

# Jared, these have moved to define in vendors/PROCUCT/MODULE/config.arch
#ifeq ("$(CROSS)","xscale-linux-")
#	TC_LIBDIR := $(TOOLCHAIN_DIR)/lib/be
#else
#ifeq ("$(CROSS)","arm-linux-")
#	TC_LIBDIR := $(TOOLCHAIN_DIR)/lib
#endif
#endif
#
#ifeq ("$(CROSS)","xscale-none-linux-gnueabi-")
#	TC_LIBDIR := $(TC_INSTALL_PREFIX)/lib/
#else
#ifeq ("$(CROSS)","arm-none-linux-gnueabi-")
#	TC_LIBDIR := $(TC_INSTALL_PREFIX)/lib
#endif
#endif

all: $(TC_INSTALL_PREFIX)/usr/local/lib/libphp5.so

$(TC_INSTALL_PREFIX)/usr/local/lib/libphp5.so: build_$(CONFIGURE_TARGET)-linux/.libs/libphp5.so 
	mkdir -p $(TC_INSTALL_PREFIX)/usr/local/lib/
	cp build_$(CONFIGURE_TARGET)-linux/.libs/libphp5.so $(TC_INSTALL_PREFIX)/usr/local/lib/
	cp build_$(CONFIGURE_TARGET)-linux/modules/gettext.so $(TC_INSTALL_PREFIX)/usr/local/lib/
	cp build_$(CONFIGURE_TARGET)-linux/modules/sqlite.so $(TC_INSTALL_PREFIX)/usr/local/lib/
	cp -a build_$(CONFIGURE_TARGET)-linux/ext/gettext/tests/locale $(TC_INSTALL_PREFIX)/

build_$(CONFIGURE_TARGET)-linux/.libs/libphp5.so: build_$(CONFIGURE_TARGET)-linux/Makefile
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux
	#$(MAKE) -C build_$(CONFIGURE_TARGET)-linux -f Makefile -j 2 libphp5.la;
	#rm -rf $(TC_LIBDIR)/libxml2.la
	#rm -rf $(TC_LIBDIR)/libiconv.la

build_$(CONFIGURE_TARGET)-linux/Makefile: makefile
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf php-5.0.5.tar.bz2
	mv php-5.0.5 build_$(CONFIGURE_TARGET)-linux
#	apt-get install -y bison flex libxml2 libxml2-dev btyacc libjpeg62 libjpeg-dev
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset LIBS CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	ac_cv_c_bigendian_php="yes" \
	ac_cv_func_dlopen="no" \
	CC="$(CROSS)gcc" \
	CXX="$(CROSS)g++" \
	AR="$(CROSS)ar" \
	AS="$(CROSS)as" \
	LD="$(CROSS)ld" \
	CPP="$(CROSS)cpp" \
	NM="$(CROSS)nm" \
	RANLIB="$(CROSS)ranlib" \
	STRIP="$(CROSS)strip" \
	OBJCOPY="$(CROSS)objcopy" \
	LDFLAGS="-L$(TC_LIBDIR) -ldl -liconv" \
	CFLAGS="-I$(GCC_INC_PATH)" \
	ARCH="arm" \
	./configure \
	--build=i686-linux \
	--host="$(CONFIGURE_TARGET)-linux" \
	--disable-ipv6 \
	--without-gd \
	--with-config-file-path="/etc/apache/php" \
	--with-pic \
	--enable-shared \
	--with-apxs2="$(TC_INSTALL_PREFIX)/bin/apxs" \
	--with-mysql="$(TC_INSTALL_PREFIX)" \
	--enable-sysvmsg \
	--enable-sysvsem \
	--enable-sysvshm \
	--enable-dba \
	--with-gnu-ld="$(CROSS)ld" \
	--enable-xmlreader \
	--enable-dom \
	--enable-pdo=shared \
	--with-pdo-sqlite=shared \
	--with-sqlite=shared \
	--enable-sockets \
	--enable-shmop \
	--enable-pear \
	--enable-mbstring \
	--with-gettext=shared \
	--with-libxml-dir="$(TC_INSTALL_PREFIX)/include/libxml2" \
	)

#	--includedir="$(GCC_INC_PATH)" \
#	--with-libdir="$(TC_LIBDIR)" \
#	--with-jpeg-dir="$(TC_INSTALL_PREFIX)"\
#	--with-zlib="$(TC_INSTALL_PREFIX)" \
#	--with-zlib-dir="$(TC_INSTALL_PREFIX)" \
#	--with-openssl-dir="$(TC_INSTALL_PREFIX)" \
	#--enable-debug \
#	--disable-libxml \
#	--with-libxml-dir="$(ROOTDIR)/lib/libxml2/install_$(CONFIGURE_TARGET)" \
#	--without-iconv \


romfs: #$(TC_INSTALL_PREFIX)/usr/local/lib/libphp5.so
#	10-01-2008. Comment out by Jared. libphp5 will not be a default built-in software. It will be provided as a package.
	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/.libs/libphp5.so
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/.libs/libphp5.so /lib/libphp5.so

package: $(TC_INSTALL_PREFIX)/usr/local/lib/libphp5.so
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux
	cp -a $(TC_INSTALL_PREFIX)/locale ipkg_src_root_$(CONFIGURE_TARGET)-linux
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib
	cp -a $(TC_INSTALL_PREFIX)/usr/local/lib/libphp5.so ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/
	cp -a $(TC_INSTALL_PREFIX)/usr/local/lib/gettext.so ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/
	cp -a $(TC_INSTALL_PREFIX)/usr/local/lib/sqlite.so ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/
	$(STRIP) -s ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/*.so
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/apache/php
	cp php.ini ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/apache/php/
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_src_root_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

clean distclean:
	-rm -rf build_$(CONFIGURE_TARGET)-linux
	-rm -rf ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib
	-rm -rf ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/apache
