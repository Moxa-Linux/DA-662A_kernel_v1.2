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
#	make user/php_only

#CROSS=arm-linux-

CONFIGURE_SETTINGS := --build=i686-linux \
		--host=$(CONFIGURE_TARGET)-linux \
		--target=$(CONFIGURE_TARGET)-linux \
		--oldincludedir="-I/usr/local/arm-linux/include -lpthread -DZTS -DPTHREADS" \
		--disable-gd-native-ttf \
		--disable-gd-jis-conv \
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
		--prefix=$(ROOTDIR)/install_me \
		--with-tsrm-pthreads \
		--with-jpeg-dir=$(ROOTDIR)/lib/libjpeg \
		--with-gd-dir=$(ROOTDIR)/lib/libgd/install_$(CONFIGURE_TARGET)-linux \
		--with-config-file-path=/etc/php \
		--with-zlib-dir=$(ROOTDIR)/lib/zlib/install_$(CONFIGURE_TARGET)-linux \
		--with-libxml-dir=$(ROOTDIR)/lib/libxml2/install_$(CONFIGURE_TARGET)-linux \
		--with-freetype=$(ROOTDIR)/lib/libfreetype/install_$(CONFIGURE_TARGET)-linux \
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


all: build_$(CONFIGURE_TARGET)-linux/sapi/cgi/php

build_$(CONFIGURE_TARGET)-linux/sapi/cgi/php: build_$(CONFIGURE_TARGET)-linux/Makefile
	unset CFLAGS CC CPPFLAGS LDFLAGS CXXFLAGS CFLAGS_CLEAN && \
	make -C build_$(CONFIGURE_TARGET)-linux CFLAGS_CLEAN='-O2 -g'

build_$(CONFIGURE_TARGET)-linux/Makefile:
	rm -rf build_$(CONFIGURE_TARGET)-linux;
	if [ ! -d "build_$(CONFIGURE_TARGET)-linux" ]; then \
		tar xjvf php-5.1.6.tar.bz2; \
		mv php-5.1.6 build_$(CONFIGURE_TARGET)-linux; \
	fi
	sed -i '6336s/exit 1;//' build_$(CONFIGURE_TARGET)-linux/configure; 
	sed -i '7109s/exit 1;//' build_$(CONFIGURE_TARGET)-linux/configure; 
	sed -i '7166s/exit 1;//' build_$(CONFIGURE_TARGET)-linux/configure; 
	sed -i '9286s/exit 1;//' build_$(CONFIGURE_TARGET)-linux/configure; 
	sed -i '9290s/exit 1;//' build_$(CONFIGURE_TARGET)-linux/configure; 
	sed -i '10132s/exit 1;//' build_$(CONFIGURE_TARGET)-linux/configure; 
	sed -i '10137s/exit 1;//' build_$(CONFIGURE_TARGET)-linux/configure; 
	sed -i '10694s/exit 1;//' build_$(CONFIGURE_TARGET)-linux/configure; 
	sed -i '10686,10953 s/^/#/' build_$(CONFIGURE_TARGET)-linux/configure; 
	sed -i '10959,11195 s/^/#/' build_$(CONFIGURE_TARGET)-linux/configure; 
	sed -i '11206,11423 s/^/#/' build_$(CONFIGURE_TARGET)-linux/configure; 
	sed -i '45578s/exit 1;//' build_$(CONFIGURE_TARGET)-linux/configure; 
	sed -i '88409s/exit 1;//' build_$(CONFIGURE_TARGET)-linux/configure; 
	sed -i '103626s/exit 1;//' build_$(CONFIGURE_TARGET)-linux/configure; 
	(cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CC CPPFLAGS LDFLAGS CXXFLAGS CFLAGS_CLEAN && \
	oldincludedir="-I/usr/local/arm-linux/include -lpthread -DZTS -DPTHREADS" \
	CC="$(CROSS)gcc" \
	LD="$(CROSS)gcc -L/usr/local/arm-linux/lib" \
	CPP="$(CROSS)gcc -E" \
	LDFLAGS="-L$(ROOTDIR)/lib/zlib" \
	PHP_SAPI=cgi \
	SAPI_CGI_PATH='sapi/cgi/php' \
	./configure \
	$(CONFIGURE_SETTINGS) \
	)
	sed -i '1,$$ s/-I\/include//g' build_$(CONFIGURE_TARGET)-linux/Makefile
	#sed -i '1,$$ s/-I\"\/usr\/include\"//g' build_$(CONFIGURE_TARGET)-linux/Makefile

romfs:
	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/sapi/cgi/php
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/sapi/cgi/php /bin/php

clean distclean:
	rm -rf ./build_$(CONFIGURE_TARGET)-linux
	rm -fr ./install_me

#%:
#	make -f Makefile $@
