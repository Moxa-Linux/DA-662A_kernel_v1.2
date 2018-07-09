# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# Pre-requirement:
#	apt-get -y install autoconf
#
# compile: 
#	cd sdlinux-moxaart
#	make user/apache_only

CONFIGURE_SETTINGS := --build=i686 \
		--host=$(CONFIGURE_TARGET)-linux \
		--enable-rewrite \
		--enable-ssl \
		--enable-so \
		--prefix=$(TC_INSTALL_PREFIX) \
		--sysconfdir="/etc/apache/conf" \
		--enable-mime \
		--enable-headers \
		--with-z=$(TC_INSTALL_PREFIX) \
		--with-ssl=$(TC_INSTALL_PREFIX) \
		--with-included-apr

		#--with-z=$(ROOTDIR)/lib/zlib/build_$(CONFIGURE_TARGET)-linux \
		#--with-ssl=$(ROOTDIR)/lib/openssl/build_$(CONFIGURE_TARGET)-linux \


#		--enable-deflate \

all: $(TC_INSTALL_PREFIX)/bin/httpd

$(TC_INSTALL_PREFIX)/bin/httpd: build_$(CONFIGURE_TARGET)-linux/.libs/httpd
	make -f Makefile -C build_$(CONFIGURE_TARGET)-linux install
	#cp -f apxs_$(CONFIGURE_TARGET) $(TC_INSTALL_PREFIX)/bin/apxs
	sed -e '212 s/exit 1/# exit 1/g' build_$(CONFIGURE_TARGET)-linux/support/apxs >apxs.new
	chmod a+x apxs.new
	mv ./apxs.new $(TC_INSTALL_PREFIX)/bin/apxs

build_$(CONFIGURE_TARGET)-linux/.libs/httpd: build_$(CONFIGURE_TARGET)-linux/Makefile
	unset CFLAGS CC CPPFLAGS LDFLAGS CXXFLAGS && \
	make -f Makefile -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/Makefile:
# If the autoconf hasn't installed, you should remove the comment to install it.
	#apt-get -y install autoconf
	# Remove the redundancy file in toolchain
	rm -rf /usr/local/arm-linux-4.4.2/lib/libaprutil-1.la /usr/local/arm-linux-4.4.2/lib/libapr-1.la
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf httpd-2.2.8.tar.bz2
	mv httpd-2.2.8 build_$(CONFIGURE_TARGET)-linux
	sed -i "s/AC_CHECK_FILE(\/dev\/zero)/#AC_CHECK_FILE(\/dev\/zero)/g" build_$(CONFIGURE_TARGET)-linux/srclib/apr/configure.in; 
	sed -i "s/APR_CHECK_TCP_NODELAY_WITH_CORK/#APR_CHECK_TCP_NODELAY_WITH_CORK/g" build_$(CONFIGURE_TARGET)-linux/srclib/apr/configure.in; 
	( cd build_$(CONFIGURE_TARGET)-linux/srclib/apr; autoconf; cd ../.. ; ) 
	sed -i "s/ssize_t_fmt='#error Can not determine the proper size for ssize_t'/ssize_t_fmt=\'#define APR_SSIZE_T_FMT \"d\"\'/g" build_$(CONFIGURE_TARGET)-linux/srclib/apr/configure; 
	sed -i "s/size_t_fmt='#error Can not determine the proper size for size_t'/size_t_fmt=\'#define APR_SIZE_T_FMT \"d\"\'/g" build_$(CONFIGURE_TARGET)-linux/srclib/apr/configure; 
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CC CPP CPPFLAGS LDFLAGS CXXFLAGS && \
	ac_cv_func_setpgrp_void=yes \
	ac_cv_o_nonblock_inherited=no \
	apr_cv_process_shared_works=no \
	ap_void_ptr_lt_long=no \
	CC=$(CROSS)gcc \
	LD=$(CROSS)ld \
	CXX=$(CROSS)g++ \
	CPP="$(CROSS)gcc -E" \
	STRIP=$(CROSS)strip \
	CFLAGS='-DBIG_SECURITY_HOLE -DPATH_MAX=4096 -I$(TC_INSTALL_PREFIX)/include' \
	EXTRA_CFLAGS='-DBIG_SECURITY_HOLE -DPATH_MAX=4096 -I$(TC_INSTALL_PREFIX)/include' \
	LDFLAGS='-L$(TC_INSTALL_PREFIX)/lib' \
	EXTRA_LDFLAGS='-L$(TC_INSTALL_PREFIX)/lib' \
	./configure $(CONFIGURE_SETTINGS) ; \
	)
	sed -i "s/^	\.\/dftables /	#\.\/dftables /g" build_$(CONFIGURE_TARGET)-linux/srclib/pcre/Makefile
	sed -i "s/CLEAN_TARGETS = dftables chartables.c/CLEAN_TARGETS = dftables # chartables.c/g" build_$(CONFIGURE_TARGET)-linux/srclib/pcre/Makefile
	cp chartables.c build_$(CONFIGURE_TARGET)-linux/srclib/pcre/
	sed -i 's/^	\.\/gen_test_char > test_char.h/	#\.\/gen_test_char > test_char.h/g' build_$(CONFIGURE_TARGET)-linux/server/Makefile
	cp test_char.h build_$(CONFIGURE_TARGET)-linux/server/

package: $(TC_INSTALL_PREFIX)/bin/httpd
	echo "Nothing to do"
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/bin
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/lib
	cp -a $(TC_INSTALL_PREFIX)/bin/httpd ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/httpd
	chmod a+s ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/httpd
	cp -a $(TC_INSTALL_PREFIX)/bin/htpasswd ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/htpasswd
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/htpasswd
	if [ -f $(TC_INSTALL_PREFIX)/lib/libapr-1.so.0.2.12 ]; then \
		$(STRIP) -s $(TC_INSTALL_PREFIX)/lib/libapr-1.so.0.2.12; \
		cp -a $(TC_INSTALL_PREFIX)/lib/libapr-1.so.0.2.12 ipkg_$(CONFIGURE_TARGET)-linux/lib/libapr-1.so.0.2.12; \
	else \
		$(STRIP) -s $(TC_INSTALL_PREFIX)/lib/libapr-1.so.0.2.12; \
		cp -a $(TC_INSTALL_PREFIX)/lib/libapr-1.so.0.2.12 ipkg_$(CONFIGURE_TARGET)-linux/lib/libapr-1.so.0.2.12; \
	fi
	if [ -f $(TC_INSTALL_PREFIX)/lib/libaprutil-1.so.0.2.12 ]; then \
		$(STRIP) -s $(TC_INSTALL_PREFIX)/lib/libaprutil-1.so.0.2.12; \
		cp -a $(TC_INSTALL_PREFIX)/lib/libaprutil-1.so.0.2.12 ipkg_$(CONFIGURE_TARGET)-linux/lib/libaprutil-1.so.0.2.12; \
	else \
		$(STRIP) -s $(TC_INSTALL_PREFIX)/lib/libaprutil-1.so.0.2.12; \
		cp -a $(TC_INSTALL_PREFIX)/lib/libaprutil-1.so.0.2.12 ipkg_$(CONFIGURE_TARGET)-linux/lib/libaprutil-1.so.0.2.12; \
	fi
	(cd ipkg_$(CONFIGURE_TARGET)-linux/lib; \
	ln -sf libaprutil-1.so.0.2.12 libaprutil-1.so.0; \
	ln -sf libaprutil-1.so.0.2.12 libaprutil-1.so; \
	ln -sf libapr-1.so.0.2.12 libapr-1.so.0; \
	ln -sf libapr-1.so.0.2.12 libapr-1.so; \
	)
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

romfs: $(TC_INSTALL_PREFIX)/bin/httpd
	$(ROMFSINST) -n dist/home /home
	$(ROMFSINST) -n dist/usr /usr
	$(ROMFSINST) -n dist/etc/apache /etc/apache
	$(ROMFSINST) -n dist/etc/init.d /etc/init.d
	$(ROMFSINST) -e CONFIG_USER_APACHE_START -n dist/etc/rc.d /etc/rc.d
	if [ "$(CONFIG_USER_LIBPHP_5_3_14)" = "y" ]; then \
		sed -i 's:.*php5_module.*:LoadModule php5_module /lib/modules/libphp5.so:g' \
			$(ROMFSDIR)/etc/apache/conf/httpd.conf; \
	fi
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/httpd /usr/bin/httpd
	chmod a+s $(ROMFSDIR)/usr/bin/httpd
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/htpasswd /usr/bin/htpasswd
	if [ -f $(TC_INSTALL_PREFIX)/lib/libapr-1.so.0.2.12 ]; then \
		$(ROMFSINST) $(TC_INSTALL_PREFIX)/lib/libapr-1.so.0.2.12 /lib/libapr-1.so.0.2.12; \
	else \
		$(ROMFSINST) $(TC_INSTALL_PREFIX)/lib/libapr-1.so.0.2.12 /lib/libapr-1.so.0.2.12; \
	fi
	if [ -f $(TC_INSTALL_PREFIX)/lib/libaprutil-1.so.0.2.12 ]; then \
		$(ROMFSINST) $(TC_INSTALL_PREFIX)/lib/libaprutil-1.so.0.2.12 /lib/libaprutil-1.so.0.2.12; \
	else \
		$(ROMFSINST) $(TC_INSTALL_PREFIX)/lib/libaprutil-1.so.0.2.12 /lib/libaprutil-1.so.0.2.12; \
	fi
	$(STRIP) -s $(ROMFSDIR)/usr/bin/htpasswd \
	$(ROMFSDIR)/usr/bin/httpd \
	$(ROMFSDIR)/lib/libaprutil-1.so.0.2.12 \
	$(ROMFSDIR)/lib/libapr-1.so.0.2.12
	$(ROMFSINST) -s /lib/libaprutil-1.so.0.2.12 /lib/libaprutil-1.so.0
	$(ROMFSINST) -s /lib/libaprutil-1.so.0.2.12 /lib/libaprutil-1.so
	$(ROMFSINST) -s /lib/libapr-1.so.0.2.12 /lib/libapr-1.so.0
	$(ROMFSINST) -s /lib/libapr-1.so.0.2.12 /lib/libapr-1.so
	mkdir -p $(ROMFSDIR)/etc/apache/logs

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux \
	ipkg_$(CONFIGURE_TARGET)-linux/lib/* \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/htpasswd \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/httpd 


#%:
#	make -f Makefile $@
