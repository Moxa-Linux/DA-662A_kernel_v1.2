# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 03-13-2012	Jared Wu.		Configure and build the fast cgi program php-cgi
#
# Pre-requirement:
#	apt-get install -y bison flex libxml2 libxml2-dev btyacc libjpeg62 libjpeg-dev
#
# compile: 
#	make lib/pcre_only
#	make lib/libjpeg_only
#	make lib/libssl_only
#	make lib/zlib_only
#	make lib/expat_only
#	make lib/libxml2_only
#	make user/php-5.3.10_only

#CROSS=arm-linux-

CONFIGURE_SETTINGS := --build=i686-linux \
		--host=$(CONFIGURE_TARGET)-linux \
		--target=$(CONFIGURE_TARGET)-linux \
		--oldincludedir="-I$(TC_INSTALL_PREFIX)/include -lpthread -DZTS -DPTHREADS" \
		--disable-gd-native-ttf \
		--disable-gd-jis-conv \
		--enable-mbstring \
		--enable-bcmath \
		--enable-zip  \
		--enable-sockets \
		--enable-exif \
		--enable-cgi \
		--enable-fastcgi \
		--enable-cli \
		--enable-memory-limit \
		--with-layout=GNU \
		--with-regex=php \
		--disable-dba \
		--disable-zend-multibyte \
		--prefix=$(TC_INSTALL_PREFIX) \
		--with-tsrm-pthreads \
		--with-jpeg-dir=$(ROOTDIR)/lib/libjpeg \
		--with-gd-dir=$(TC_INSTALL_PREFIX) \
		--with-config-file-path=/etc/php \
		--with-zlib=$(TC_INSTALL_PREFIX) \
		--with-libxml-dir=$(ROOTDIR)/lib/libxml2/build_$(CONFIGURE_TARGET)-linux/install-me-here \
		--with-freetype=$(TC_INSTALL_PREFIX) \
		--with-sqlite \
		--with-pdo-sqlite \
		--enable-sqlite-utf8 \
		--without-pi3web \
		--without-roxen \
		--without-roxen-zts \
		--without-thttpd \
		--without-tux \
		--without-WebJames \
		--without-mysql \
		--without-aolserver \
		--without-continuity \
		--without-mod_charset \
		--without-gettext \
		--without-qdbm \
		--without-gdbm \
		--without-ndbm \
		--without-db4 \
		--without-db3 \
		--without-db2 \
		--without-dbm \
		--without-cdb \
		--disable-dbase \
		--disable-dbx \
		--disable-dom

#		--with-zlib=$(ROOTDIR)/lib/zlib/build_$(CONFIGURE_TARGET)-linux/install-me-here \
#		--with-libxml-dir=$(TC_INSTALL_PREFIX) \
		--with-freetype=$(ROOTDIR)/lib/libfreetype/build_$(CONFIGURE_TARGET)-linux/install-me-here \
		--with-gd-dir=$(ROOTDIR)/lib/libgd/build_$(CONFIGURE_TARGET)-linux/install-me-here \

all: build_$(CONFIGURE_TARGET)-linux/sapi/cgi/php

build_$(CONFIGURE_TARGET)-linux/sapi/cgi/php: build_$(CONFIGURE_TARGET)-linux/Makefile
	sed -i 's/pharcmd\: ext\/phar\/phar.php ext\/phar\/phar.phar/pharcmd\: #ext\/phar\/phar.php ext\/phar\/phar.phar/' build_$(CONFIGURE_TARGET)-linux/Makefile
	cp -a ./phar.phar build_$(CONFIGURE_TARGET)-linux/ext/phar/phar.phar
	cp -a ./phar.php build_$(CONFIGURE_TARGET)-linux/ext/phar/phar.php
	unset CFLAGS CC CPPFLAGS LDFLAGS CXXFLAGS CFLAGS_CLEAN && \
	make -C build_$(CONFIGURE_TARGET)-linux CFLAGS_CLEAN='-O2 -g'

build_$(CONFIGURE_TARGET)-linux/Makefile:
	rm -rf build_$(CONFIGURE_TARGET)-linux;
	if [ ! -d "build_$(CONFIGURE_TARGET)-linux" ]; then \
		tar xjvf php-5.3.10.tar.bz2; \
		mv php-5.3.10 build_$(CONFIGURE_TARGET)-linux; \
	fi
	#ac_cv_c_bigendian_php="yes"
	(cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CC CPPFLAGS LDFLAGS CXXFLAGS CFLAGS_CLEAN && \
	oldincludedir="-I$(TC_INSTALL_PREFIX)/include -lpthread -DZTS -DPTHREADS" \
	CC="$(CROSS)gcc" \
	LD="$(CROSS)gcc -L$(TC_INSTALL_PREFIX)/lib" \
	CPP="$(CROSS)gcc -E" \
	CFLAGS="-I$(TC_INSTALL_PREFIX)/include" \
	LDFLAGS="-L$(TC_INSTALL_PREFIX)/lib" \
	PHP_SAPI=cgi \
	SAPI_CGI_PATH='sapi/cgi/php' \
	./configure \
	$(CONFIGURE_SETTINGS) \
	)

romfs:
	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/sapi/cgi/php-cgi
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/sapi/cgi/php-cgi /bin/
	cp -a ipkg_$(CONFIGURE_TARGET)-linux/etc/* $(ROMFSDIR)/etc/
	# do not pre-install rc3.d link, instead register it in inittab
	rm -rf $(ROMFSDIR)/etc/rc.d/rc3.d/S90php-fcgi

package: build_$(CONFIGURE_TARGET)-linux/sapi/cgi/php
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/bin
	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/sapi/cgi/php-cgi
	cp -a build_$(CONFIGURE_TARGET)-linux/sapi/cgi/php-cgi ipkg_$(CONFIGURE_TARGET)-linux/bin/
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

clean distclean:
	rm -rf ./build_$(CONFIGURE_TARGET)-linux \
	./ipkg_$(CONFIGURE_TARGET)-linux/bin

#%:
#	make -f Makefile $@
