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

all: $(TC_INSTALL_PREFIX)/modules/libphp5.so

$(TC_INSTALL_PREFIX)/modules/libphp5.so: build_$(CONFIGURE_TARGET)-linux/.libs/libphp5.so
	# Skip the php pear installation
	sed -i 's/install-pear install-pharcmd/#install-pear install-pharcmd/g' build_$(CONFIGURE_TARGET)-linux/Makefile
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install

build_$(CONFIGURE_TARGET)-linux/.libs/libphp5.so: build_$(CONFIGURE_TARGET)-linux/Makefile
	sed -i 's/\@$$(PHP_PHARCMD_EXECUTABLE)/\#\@$$(PHP_PHARCMD_EXECUTABLE)/'  build_$(CONFIGURE_TARGET)-linux/Makefile
	cp -a phar.phar build_$(CONFIGURE_TARGET)-linux/ext/phar/phar
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux


build_$(CONFIGURE_TARGET)-linux/Makefile:
#	apt-get install -y bison flex libxml2 libxml2-dev btyacc libjpeg62 libjpeg-dev
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf php-5.3.14.tar.bz2
	mv php-5.3.14 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset LIBS CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	ac_cv_func_dlopen="no" \
	libgcc_libpath="$(TC_LIBDIR)" \
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
	CFLAGS="-I$(TC_INSTALL_PREFIX)/include" \
	CXXFLAGS="-I$(TC_INSTALL_PREFIX)/include" \
	CPPFLAGS="-I$(TC_INSTALL_PREFIX)/include" \
	LDFLAGS="-L$(TC_LIBDIR) -L$(TC_INSTALL_PREFIX)/sys-root/usr/lib/ -ldl" \
	EXTRA_LDFLAGS_PROGRAM="" \
	ARCH="arm" \
	./configure \
	--build=i686-linux \
	--host="$(CONFIGURE_TARGET)-linux" \
	--prefix="$(TC_INSTALL_PREFIX)" \
	--enable-ipv6 \
	--without-gd \
	--with-sqlite3="$(TC_INSTALL_PREFIX)" \
	--with-config-file-path="/etc/apache/php" \
	--with-pic \
	--enable-shared \
	--with-apxs2="$(TC_INSTALL_PREFIX)/bin/apxs" \
	--enable-sysvmsg \
	--enable-sysvsem \
	--enable-sysvshm \
	--enable-dba \
	--with-gnu-ld="$(CROSS)ld" \
	--without-iconv \
	--enable-xmlreader \
	--enable-dom \
	--enable-pdo=shared \
	--with-pdo-sqlite=shared \
	--with-sqlite=shared \
	--enable-sockets \
	--enable-shmop \
	--enable-pear \
	--enable-mbstring \
	--with-libxml-dir=$(ROOTDIR)/lib/libxml2/build_$(CONFIGURE_TARGET)-linux/install-me-here \
	--enable-bcmath \
	--without-zend-vm \
	--disable-zend-multibyte \
	)

	#sed -i '/\/bin\/sh/ a set -x' build_$(CONFIGURE_TARGET)-linux/configure
	#ac_cv_c_bigendian_php="yes" \
	#--with-gettext=shared \
	#--enable-debug \
	#--with-mysql="$(TC_INSTALL_PREFIX)" \
	#--with-libxml-dir="$(TC_INSTALL_PREFIX)/include/libxml2" \


romfs: #$(TC_INSTALL_PREFIX)/modules/libphp5.so
	mkdir -p $(ROMFSDIR)/lib/modules
	#$(ROMFSINST) $(TC_INSTALL_PREFIX)/usr/local/lib/gettext.so /lib/modules/gettext.so
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/modules/libphp5.so /lib/modules/libphp5.so
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/lib/php/extensions/no-debug-non-zts-20090626/sqlite.so /lib/modules/sqlite.so
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/lib/php/extensions/no-debug-non-zts-20090626/pdo_sqlite.so /lib/modules/pdo_sqlite.so
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/lib/php/extensions/no-debug-non-zts-20090626/pdo.so /lib/modules/pdo.so
	$(STRIP) -s $(ROMFSDIR)/lib/modules/*.so
	# Comment out by Jared, Please customized your php.ini in vendors/Moxa/$(CONFIG_PRODUCT)/romfsdisk/etc/apache/php/php.ini
	#cp -a ipkg_$(CONFIGURE_TARGET)-linux/etc/apache $(ROMFSDIR)/etc/
	#sed -i 's/#\+ LoadModule php5_module \/usr\/local\/lib\/libphp5.so/ LoadModule php5_module \/lib\/modules\/libphp5.so/' $(ROMFSDIR)/etc/apache/conf/httpd.conf
	#sed -i 's/\/usr\/local\/lib/\/lib\/modules/' $(ROMFSDIR)/etc/apache/php/php.ini

package: $(TC_INSTALL_PREFIX)/modules/libphp5.so
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux
	#cp -a $(TC_INSTALL_PREFIX)/bin/locale ipkg_$(CONFIGURE_TARGET)-linux
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib
	#cp -a $(TC_INSTALL_PREFIX)/lib/php/extensions/no-debug-non-zts-20090626/gettext.so ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib/
	cp -a $(TC_INSTALL_PREFIX)/modules/libphp5.so ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib/
	cp -a $(TC_INSTALL_PREFIX)/lib/php/extensions/no-debug-non-zts-20090626/sqlite.so ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib/
	cp -a $(TC_INSTALL_PREFIX)/lib/php/extensions/no-debug-non-zts-20090626/pdo.so ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib/
	cp -a $(TC_INSTALL_PREFIX)/lib/php/extensions/no-debug-non-zts-20090626/pdo_sqlite.so ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib/*.so*
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

clean distclean:
	-rm -rf build_$(CONFIGURE_TARGET)-linux \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib
