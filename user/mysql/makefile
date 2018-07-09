# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/mysql_only

CONFIGURE_SETTINGS :=	--build=i686-linux                              \
		--host=$(CONFIGURE_TARGET)-linux                        \
		--target=$(CONFIGURE_TARGET)-linux                      \
		--prefix="$(TC_INSTALL_PREFIX)"				\
		--program-transform-name=""                             \
		--without-perl                                          \
		--without-man                                           \
		--without-bench                                         \
		--without-docs                                          \
		--without-debug                                         \
		--with-low-memory                                       \

#		--with-zlib-dir=$(TOOLCHAIN_DIR)/$(CONFIGURE_TARGET)	\


#		--with-client-ldflags=-all-static                       \
#		--with-mysqld-ldflags=-all-static                       \
#		--enable-static						\
#		--with-embedded-server                                  \
#		--datadir="/home/mysql/var"				\
#		--with-client-ldflags=-static                           \
#		--with-mysqld-ldflags=-static                           \
#		--with-big-tables                                       \
#		--with-mit-threads=no                                   \
#		--with-pthread                                          \
#		--disable-assembler                                     \
#		--without-server                                        \

all: $(TC_INSTALL_PREFIX)/lib/mysql/libmysqlclient.so.16.0.0


$(TC_INSTALL_PREFIX)/lib/mysql/libmysqlclient.so.16.0.0: build_$(CONFIGURE_TARGET)-linux/libmysql/.libs/libmysqlclient.so.16.0.0
	make -C build_$(CONFIGURE_TARGET)-linux install 

#build_$(CONFIGURE_TARGET)-linux/libmysql/.libs/libmysqlclient.so.16.0.0:
build_$(CONFIGURE_TARGET)-linux/libmysql/.libs/libmysqlclient.so.16.0.0: build_$(CONFIGURE_TARGET)-linux/build
	[ -f ./build_$(CONFIGURE_TARGET)-linux/sql/lex_hash.h ] || cp -f ./lex_hash.h ./build_$(CONFIGURE_TARGET)-linux/sql/lex_hash.h ;
	unset CFLAGS CPPFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET)-linux -j 2;

build_$(CONFIGURE_TARGET)-linux/build:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf mysql-5.1.23-rc.tar.gz;
	mv mysql-5.1.23-rc build_$(CONFIGURE_TARGET)-linux
	( \
	cd build_$(CONFIGURE_TARGET)-linux; \
	sed -e "s/^AC_SYS_RESTARTABLE_SYSCALLS/#AC_SYS_RESTARTABLE_SYSCALLS/" < configure.in > configure.in.temp; \
	sed -i "s/^MYSQL_STACK_DIRECTION/#MYSQL_STACK_DIRECTION/g"  configure.in.temp; \
	mv -f configure.in.temp configure.in; \
	autoconf; \
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	./configure \
	ac_cv_func_dlerror=no \
	ac_cv_func_dlopen=no \
	ac_cv_lib_dl_dlopen=no \
	mysql_use_mysys_new=yes \
	CC=$(CROSS)gcc \
	CXX=$(CROSS)gcc \
	LD=$(CROSS)ld \
	CPPFLAGS="-DSTACK_DIRECTION=1 -DHAVE_GETHOSTBYNAME_R_GLIBC2_STYLE" \
	CFLAGS="-Os -pipe -isystem $(TC_INSTALL_PREFIX)/include" \
	CXXFLAGS="-O3 -felide-constructors -fno-exceptions -fno-rtti -isystem $(TC_INSTALL_PREFIX)/include" \
	LDFLAGS="-L$(TC_INSTALL_PREFIX)/lib" \
	$(CONFIGURE_SETTINGS) ; \
	)
	touch build_$(CONFIGURE_TARGET)-linux/build


romfs: # $(TC_INSTALL_PREFIX)/lib/mysql/libmysqlclient.so.16.0.0
#	10-01-2008. Comment out by Jared. libmysqlclient will not be a default built-in software. It will be provided as a package.
#	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/libmysql/.libs/libmysqlclient.so.16.0.0
#	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/libmysql/.libs/libmysqlclient.so.16.0.0 /lib/libmysqlclient.so.16.0.0
#	( cd  ${ROMFSDIR}/lib; \
	[ -L libmysqlclient.so.16 ] || ln -s libmysqlclient.so.16.0.0 libmysqlclient.so.16; \
	[ -L libmysqlclient.so ] || ln -s libmysqlclient.so.16.0.0 libmysqlclient.so; \
	)

package: $(TC_INSTALL_PREFIX)/lib/mysql/libmysqlclient.so.16.0.0
	# Make the libmysqlclient.so.16.0.0 client library package
	mkdir -p ipkg_libmysqlclient_$(CONFIGURE_TARGET)-linux/lib
	mkdir -p ipkg_libmysqlclient_$(CONFIGURE_TARGET)-linux/bin
	cp -a $(TC_INSTALL_PREFIX)/lib/mysql/libmysqlclient.so.16.0.0 ipkg_libmysqlclient_$(CONFIGURE_TARGET)-linux/lib/libmysqlclient.so.16.0.0
	$(STRIP) -s ipkg_libmysqlclient_$(CONFIGURE_TARGET)-linux/lib/libmysqlclient.so.16.0.0
	( cd  ipkg_libmysqlclient_$(CONFIGURE_TARGET)-linux/lib; \
	[ -L libmysqlclient.so.16 ] || ln -s libmysqlclient.so.16.0.0 libmysqlclient.so.16; \
	[ -L libmysqlclient.so ] || ln -s libmysqlclient.so.16.0.0 libmysqlclient.so;)
	cp -a $(TC_INSTALL_PREFIX)/bin/mysql ipkg_libmysqlclient_$(CONFIGURE_TARGET)-linux/bin/
	$(STRIP) -s ipkg_libmysqlclient_$(CONFIGURE_TARGET)-linux/lib/libmysqlclient.so.16.0.0
	$(STRIP) -s  ipkg_libmysqlclient_$(CONFIGURE_TARGET)-linux/bin/mysql

	$(IPKGBUILD) ipkg_libmysqlclient_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	# Make the mysql client package
	#mkdir -p ipkg_mysqlclient_$(CONFIGURE_TARGET)-linux/home/mysql/bin
	#cp -a $(TC_INSTALL_PREFIX)/bin/mysql ipkg_mysqlclient_$(CONFIGURE_TARGET)-linux/home/mysql/bin/
	#$(STRIP) -s ipkg_mysqlclient_$(CONFIGURE_TARGET)-linux/home/mysql/bin/mysql
	#$(IPKGBUILD) ipkg_mysqlclient_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	# Make the MySQL server package
	mkdir -p ipkg_server_root_$(CONFIGURE_TARGET)-linux/etc/rc.d/init.d
	mkdir -p ipkg_server_root_$(CONFIGURE_TARGET)-linux/home/mysql/bin
	mkdir -p ipkg_server_root_$(CONFIGURE_TARGET)-linux/home/mysql/libexec
	mkdir -p ipkg_server_root_$(CONFIGURE_TARGET)-linux/home/mysql/var
	mkdir -p ipkg_server_root_$(CONFIGURE_TARGET)-linux/home/mysql/share/mysql
	cp -a $(TC_INSTALL_PREFIX)/share/mysql/mysql.server ipkg_server_root_$(CONFIGURE_TARGET)-linux/etc/rc.d/init.d/
	sed -i '46 s/basedir=/basedir=\/home\/mysql/g' ipkg_server_root_$(CONFIGURE_TARGET)-linux/etc/rc.d/init.d/mysql.server
	sed -i '47 s/datadir=/datadir=\/home\/mysql\/var/g' ipkg_server_root_$(CONFIGURE_TARGET)-linux/etc/rc.d/init.d/mysql.server
	cp $(TC_INSTALL_PREFIX)/bin/my_print_defaults ipkg_server_root_$(CONFIGURE_TARGET)-linux/home/mysql/bin/
	cp $(TC_INSTALL_PREFIX)/bin/mysqladmin ipkg_server_root_$(CONFIGURE_TARGET)-linux/home/mysql/bin/
	cp $(TC_INSTALL_PREFIX)/libexec/mysqld  ipkg_server_root_$(CONFIGURE_TARGET)-linux/home/mysql/libexec/
	cp $(TC_INSTALL_PREFIX)/bin/mysqld_safe ipkg_server_root_$(CONFIGURE_TARGET)-linux/home/mysql/bin/
	cp $(TC_INSTALL_PREFIX)/bin/mysql_install_db ipkg_server_root_$(CONFIGURE_TARGET)-linux/home/mysql/bin/
	cp $(TC_INSTALL_PREFIX)/share/mysql/fill_help_tables.sql ipkg_server_root_$(CONFIGURE_TARGET)-linux/home/mysql/share/mysql
	cp $(TC_INSTALL_PREFIX)/share/mysql/mysql_system_tables.sql ipkg_server_root_$(CONFIGURE_TARGET)-linux/home/mysql/share/mysql
	cp $(TC_INSTALL_PREFIX)/share/mysql/mysql_system_tables_data.sql ipkg_server_root_$(CONFIGURE_TARGET)-linux/home/mysql/share/mysql
	cp -a $(TC_INSTALL_PREFIX)/share/mysql/english ipkg_server_root_$(CONFIGURE_TARGET)-linux/home/mysql/share/mysql/
	$(STRIP) ipkg_server_root_$(CONFIGURE_TARGET)-linux/home/mysql/bin/my_print_defaults
	$(STRIP) ipkg_server_root_$(CONFIGURE_TARGET)-linux/home/mysql/bin/mysqladmin
	$(STRIP) ipkg_server_root_$(CONFIGURE_TARGET)-linux/home/mysql/libexec/mysqld
	$(IPKGBUILD) ipkg_server_root_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux 
	rm -rf ipkg_client_root_$(CONFIGURE_TARGET)-linux/lib \
		ipkg_server_root_$(CONFIGURE_TARGET)-linux/usr \
		ipkg_server_root_$(CONFIGURE_TARGET)-linux/home/mysql \
		ipkg_mysqlclient_$(CONFIGURE_TARGET)-linux/home \
		ipkg_libmysqlclient_$(CONFIGURE_TARGET)-linux/usr
