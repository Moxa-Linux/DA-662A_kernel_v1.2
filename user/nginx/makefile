# History:
# Date          Author                  Comment
# 23-02-2012    Aeluin Chen./Jared Wu	new created for working with php-5.1.6 fast-cgi
#
# compile:
#	make lib/pcre_only
#	make user/nginx_only
#

PACKAGE_NAME = nginx-1.0.15
EXEC = nginx

all: build_$(CONFIGURE_TARGET)-linux/objs/$(EXEC)
	echo "Build Complete!"

build_$(CONFIGURE_TARGET)-linux/objs/$(EXEC): build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf $(PACKAGE_NAME).tar.gz; \
	(cd $(PACKAGE_NAME); \
	patch -p1 -i ../$(PACKAGE_NAME)-cross-compile.patch; \
	sed -i 's/\#define NGX_SENDFILE_LIMIT  0x80000000/\/\/#define NGX_SENDFILE_LIMIT  0x80000000/g' src/os/unix/ngx_linux_config.h )
	mv $(PACKAGE_NAME) build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	CC=$(CROSS)gcc \
	LD=$(CROSS)ld \
	CPP=$(CROSS)cpp \
	STRIP=$(CROSS)strip \
	./configure \
	--crossbuild=Linux:$(CONFIGURE_TARGET) \
	--with-cc=$(CROSS)gcc \
	--with-cc-opt='-I$(ROOTDIR)/lib/pcre/build_$(CONFIGURE_TARGET)-linux -DIOV_MAX=64 -UNGX_HAVE_POSIX_FADVISE -O2' \
	--with-cpp=$(CROSS)cpp \
	--with-int=4 \
	--with-long=4 \
	--with-long-long=8 \
	--with-ptr-size=4 \
	--with-sig-atomic-t=4 \
	--with-size-t=4 \
	--with-off-t=4 \
	--with-time-t=4 \
	--with-sys-nerr=132 \
	--http-client-body-temp-path='/home/nginx/html/client_body' \
	--http-proxy-temp-path='/home/nginx/html/proxy' \
	--http-fastcgi-temp-path='/home/nginx/html/fastcgi' \
	--http-uwsgi-temp-path='/home/nginx/html/uwsgi' \
	--http-scgi-temp-path='/home/nginx/html/scgi' \
	--conf-path='/etc/nginx/nginx.conf' \
	--error-log-path='/var/log/nginx_error.log' \
	--http-log-path='/var/log/nginx_access.log' \
	--lock-path='/var/lock/nginx' \
	--pid-path='/var/run/nginx.pid' \
	--prefix='/' \
	--user='nobody' \
	--group='nobody' \
	--with-pcre="$(ROOTDIR)/lib/pcre/build_$(CONFIGURE_TARGET)-linux" \
	--with-zlib="$(TC_INSTALL_PREFIX)/lib" \
	--with-http_degradation_module \
	--without-mail_pop3_module \
	--without-mail_imap_module \
	--without-mail_smtp_module \
	)
	sed -i 's/&& if \[ -f Makefile \]/#&& if \[ -f Makefile \]/' build_$(CONFIGURE_TARGET)-linux/objs/Makefile
	sed -i 's/&& CC\=\"$$(CC)\"/#&& CC\=\"$$(CC)\"/' build_$(CONFIGURE_TARGET)-linux/objs/Makefile
	sed -i 's/\.\/configure --disable-shared/#\.\/configure --disable-shared/' build_$(CONFIGURE_TARGET)-linux/objs/Makefile
	sed -i 's/&& $$(MAKE) libpcre.la/#&& $$(MAKE) libpcre.la/' build_$(CONFIGURE_TARGET)-linux/objs/Makefile
	sed -i 's/\/pcre.h:/\/pcre.h:#/' build_$(CONFIGURE_TARGET)-linux/objs/Makefile
	# Skip to rebuild zlib
	sed -i 's/&& $$(MAKE) distclean/#&& $$(MAKE) distclean/' build_$(CONFIGURE_TARGET)-linux/objs/Makefile
	sed -i 's/&& CFLAGS=\"-O2 -fomit-frame-pointer -pipe \"/#&& CFLAGS=\"-O2 -fomit-frame-pointer -pipe \"/' build_$(CONFIGURE_TARGET)-linux/objs/Makefile
	sed -i 's/\.\/configure \\/#\.\/configure \\/' build_$(CONFIGURE_TARGET)-linux/objs/Makefile
	sed -i 's/&& $$(MAKE) libz.a/#&& $$(MAKE) libz.a/' build_$(CONFIGURE_TARGET)-linux/objs/Makefile
	sed -i 's/libz.a/libz.so/' build_$(CONFIGURE_TARGET)-linux/objs/Makefile
	#sed -i 's/use_accept4 = 1/use_accept4 = 0/'  build_$(CONFIGURE_TARGET)-linux/src/event/ngx_event_accept.c
	touch build_$(CONFIGURE_TARGET)-linux/configured

	#--with-zlib="$(ROOTDIR)/lib/zlib/build_$(CONFIGURE_TARGET)-linux" \

romfs:
	$(CROSS)strip build_$(CONFIGURE_TARGET)-linux/objs/$(EXEC)
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/objs/$(EXEC) /usr/bin/$(EXEC)
	cp -a ipkg_$(CONFIGURE_TARGET)-linux/home/nginx $(ROMFSDIR)/home/
	chmod -R a+rx-w $(ROMFSDIR)/home/nginx
	chmod a+w $(ROMFSDIR)/home/nginx/html/*.php
	cp -a ipkg_$(CONFIGURE_TARGET)-linux/etc/* $(ROMFSDIR)/etc/

package: build_$(CONFIGURE_TARGET)-linux/objs/$(EXEC)
	# Make the ipkg package
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/bin
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/etc/init.d
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/home/nginx
	chmod -R a+rx-w ipkg_$(CONFIGURE_TARGET)-linux/home
	chmod a+w ipkg_$(CONFIGURE_TARGET)-linux/home/nginx/html/*.php
	cp -a build_$(CONFIGURE_TARGET)-linux/objs/$(EXEC) ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/$(EXEC)
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/$(EXEC)
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

clean:
	/bin/rm -rf build_$(CONFIGURE_TARGET)-linux \
	ipkg_$(CONFIGURE_TARGET)-linux/usr

