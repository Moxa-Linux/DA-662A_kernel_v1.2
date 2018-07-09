# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or
#					LERoot
#	INSTALL_ROOT= $(ROOTDIR)/BERoot
#	INSTALL_ROOT= $(ROOTDIR)/LERoot
#
# compile: 
#   make CROSS=xscale-linux- ROOTDIR=/root/sdlinux-moxaart INSTALL_ROOT=/root/sdlinux-moxaart/BERoot
#   make CROSS=arm-linux- ROOTDIR=/root/sdlinux-moxaart INSTALL_ROOT=/root/sdlinux-moxaart/LERoot

CONFIGURE_TARGET := $(strip $(subst -, ,$(CROSS)))
CONFIGURE_TARGET := $(word 1,$(CONFIGURE_TARGET))-$(word 2,$(CONFIGURE_TARGET))
INSTALL_ROOT=$(CURDIR)/build_$(CONFIGURE_TARGET)/install-me-here

PACKAGE=mysql-5.6.11
LIBNAME=libmysqlclient.so.18.0.0
LIBNAME_LINK=$(strip $(patsubst %.0.0,%,$(LIBNAME)))

# find the toolchain file
ifeq ($(TOOLCHAIN_DIR),)
	CMAKE_TOOLCHAIN_FILE = toolchain.cmake
else
	CMAKE_TOOLCHAIN_FILE = $(word 3,$(subst /, ,$(TOOLCHAIN_DIR))).cmake
endif


CMAKE_CONFIG := \
	-DCMAKE_TOOLCHAIN_FILE="$(CURDIR)/toolchain/$(CMAKE_TOOLCHAIN_FILE)" \
	-DCMAKE_C_FLAGS:STRING="-Os -pipe -DHAVE_PSI_INTERFACE" \
	-DCMAKE_CXX_FLAGS:STRING="-O3 -felide-constructors -fno-exceptions -fno-rtti -DHAVE_GETHOSTBYNAME_R_GLIBC2_STYLE -DHAVE_PSI_INTERFACE" \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_INSTALL_PREFIX=$(INSTALL_ROOT) \
	-DINSTALL_DOCDIR=share/doc/mysql \
	-DINSTALL_DOCREADMEDIR=share/doc/mysql \
	-DINSTALL_INCLUDEDIR=include/mysql \
	-DINSTALL_INFODIR=share/info \
	-DINSTALL_MANDIR=share/man \
	-DINSTALL_MYSQLSHAREDIR=share/mysql \
	-DINSTALL_MYSQLTESTDIR=share/mysql/test \
	-DINSTALL_PLUGINDIR=lib/mysql \
	-DINSTALL_SBINDIR=sbin \
	-DINSTALL_SCRIPTDIR=bin \
	-DINSTALL_SQLBENCHDIR=share/mysql/bench \
	-DINSTALL_SUPPORTFILESDIR=share/mysql/support \
	-DMYSQL_UNIX_ADDR=/var/run/mysqld/mysql.sock \
	-DSYSCONFDIR=/etc/mysql \
	-DWITH_PARTITION_STORAGE_ENGINE=OFF \
	-DWITH_PERFSCHEMA_STORAGE_ENGINE=OFF \
	-DWITH_SSL=system \
	-DWITH_ZLIB=system \
	-DSTACK_DIRECTION=1 


all: $(INSTALL_ROOT)/sbin/mysqld
	echo "Build Complete!"

$(INSTALL_ROOT)/sbin/mysqld: build_$(CONFIGURE_TARGET)/Makefile
	( \
		cd build_$(CONFIGURE_TARGET); \
		make -j 2; \
		make install; \
	)

build_$(CONFIGURE_TARGET)/Makefile: build_$(CONFIGURE_TARGET).host/extra/comp_err
	rm -rf build_$(CONFIGURE_TARGET)
	tar xzvf $(PACKAGE).tar.gz;
	mv $(PACKAGE) build_$(CONFIGURE_TARGET)
	( \
		cd build_$(CONFIGURE_TARGET); \
		patch -p1 < ../$(PACKAGE).patch; \
		cp -a ../build_$(CONFIGURE_TARGET).host/extra/comp_err extra/; \
		cp -a ../build_$(CONFIGURE_TARGET).host/scripts/comp_sql scripts/; \
		cp -a ../build_$(CONFIGURE_TARGET).host/sql/gen_lex_hash sql/; \
		cmake $(CMAKE_CONFIG); \
	)


build_$(CONFIGURE_TARGET).host/extra/comp_err: build_$(CONFIGURE_TARGET).host/Makefile
	( \
		cd build_$(CONFIGURE_TARGET).host; \
		make -j 2; \
	)

build_$(CONFIGURE_TARGET).host/Makefile:
	rm -rf build_$(CONFIGURE_TARGET).host
	tar xzvf $(PACKAGE).tar.gz;
	mv $(PACKAGE) build_$(CONFIGURE_TARGET).host
	( \
		cd build_$(CONFIGURE_TARGET).host; \
		unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS CC CXX && cmake .; \
	)

romfs: # $(INSTALL_ROOT)/sbin/mysqld
#	10-01-2008. Comment out by Jared. libmysqlclient will not be a default 
#	built-in software. It will be provided as a package.
#	$(STRIP) -s $(INSTALL_ROOT)/lib/$(LIBNAME)
#	$(ROMFSINST) $(INSTALL_ROOT)/lib/$(LIBNAME) /lib/$(LIBNAME)
#	( cd  ${ROMFSDIR}/lib; \
	[ -L $(LIBNAME_LINK) ] || ln -s $(LIBNAME) $(LIBNAME_LINK); \
	[ -L libmysqlclient.so ] || ln -s $(LIBNAME) libmysqlclient.so; \
	)

package: $(INSTALL_ROOT)/sbin/mysqld
	# Make the libmysqlclient.so client library package
	mkdir -p ipkg_src_libmysqlclient_$(CONFIGURE_TARGET)/usr
	mkdir -p ipkg_src_libmysqlclient_$(CONFIGURE_TARGET)/usr/local
	mkdir -p ipkg_src_libmysqlclient_$(CONFIGURE_TARGET)/usr/local/lib
	mkdir -p ipkg_src_libmysqlclient_$(CONFIGURE_TARGET)/usr/local/bin
	mkdir -p ipkg_src_libmysqlclient_$(CONFIGURE_TARGET)/home
	mkdir -p ipkg_src_libmysqlclient_$(CONFIGURE_TARGET)/home/mysql
	mkdir -p ipkg_src_libmysqlclient_$(CONFIGURE_TARGET)/home/mysql/bin
	mkdir -p ipkg_src_libmysqlclient_$(CONFIGURE_TARGET)/home/mysql/lib
	cp -a $(INSTALL_ROOT)/lib/$(LIBNAME) ipkg_src_libmysqlclient_$(CONFIGURE_TARGET)/home/mysql/lib
	$(STRIP) -s ipkg_src_libmysqlclient_$(CONFIGURE_TARGET)/home/mysql/lib/$(LIBNAME)
	cp -a $(INSTALL_ROOT)/bin/mysql ipkg_src_libmysqlclient_$(CONFIGURE_TARGET)/home/mysql/bin
	$(STRIP) -s ipkg_src_libmysqlclient_$(CONFIGURE_TARGET)/home/mysql/bin/mysql
	( \
		cd ipkg_src_libmysqlclient_$(CONFIGURE_TARGET)/usr/local/lib; \
		ln -s /home/mysql/lib/$(LIBNAME) $(LIBNAME_LINK); \
		ln -s /home/mysql/lib/$(LIBNAME) $(LIBNAME_LINK).0; \
	)
	( \
		cd ipkg_src_libmysqlclient_$(CONFIGURE_TARGET)/usr/local/bin; \
		ln -s /home/mysql/bin/mysql mysql; \
	)
	$(IPKGBUILD) ipkg_src_libmysqlclient_$(CONFIGURE_TARGET) $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)

	# Make the MySQL server package
	mkdir -p ipkg_src_server_root_$(CONFIGURE_TARGET)/etc
	mkdir -p ipkg_src_server_root_$(CONFIGURE_TARGET)/etc/mysql
	mkdir -p ipkg_src_server_root_$(CONFIGURE_TARGET)/etc/rc.d
	mkdir -p ipkg_src_server_root_$(CONFIGURE_TARGET)/etc/rc.d/init.d
	mkdir -p ipkg_src_server_root_$(CONFIGURE_TARGET)/home
	mkdir -p ipkg_src_server_root_$(CONFIGURE_TARGET)/home/mysql
	mkdir -p ipkg_src_server_root_$(CONFIGURE_TARGET)/home/mysql/bin
	mkdir -p ipkg_src_server_root_$(CONFIGURE_TARGET)/home/mysql/sbin
	mkdir -p ipkg_src_server_root_$(CONFIGURE_TARGET)/home/mysql/var
	mkdir -p ipkg_src_server_root_$(CONFIGURE_TARGET)/home/mysql/share
	mkdir -p ipkg_src_server_root_$(CONFIGURE_TARGET)/home/mysql/share/mysql
	cp -a my.cnf ipkg_src_server_root_$(CONFIGURE_TARGET)/etc/mysql
	cp -a $(INSTALL_ROOT)/share/mysql/support/my-default.cnf ipkg_src_server_root_$(CONFIGURE_TARGET)/home/mysql/share/
	cp -a $(INSTALL_ROOT)/share/mysql/support/mysql.server ipkg_src_server_root_$(CONFIGURE_TARGET)/etc/rc.d/init.d/
	sed -i '66 s/basedir=.*/basedir=\/home\/mysql/g' ipkg_src_server_root_$(CONFIGURE_TARGET)/etc/rc.d/init.d/mysql.server
	sed -i '67 s/bindir=.*/bindir=\/home\/mysql\/bin/g' ipkg_src_server_root_$(CONFIGURE_TARGET)/etc/rc.d/init.d/mysql.server
	sed -i '70 s/datadir=.*/datadir=\/home\/mysql\/var/g' ipkg_src_server_root_$(CONFIGURE_TARGET)/etc/rc.d/init.d/mysql.server
	sed -i '72 s/sbindir=.*/sbindir=\/home\/mysql\/sbin/g' ipkg_src_server_root_$(CONFIGURE_TARGET)/etc/rc.d/init.d/mysql.server
	sed -i '73 s/libexecdir=.*/libexecdir=\/home\/mysql\/sbin/g' ipkg_src_server_root_$(CONFIGURE_TARGET)/etc/rc.d/init.d/mysql.server
	cp $(INSTALL_ROOT)/bin/my_print_defaults ipkg_src_server_root_$(CONFIGURE_TARGET)/home/mysql/bin/
	cp $(INSTALL_ROOT)/bin/mysqladmin ipkg_src_server_root_$(CONFIGURE_TARGET)/home/mysql/bin/
	cp $(INSTALL_ROOT)/bin/mysqld_safe ipkg_src_server_root_$(CONFIGURE_TARGET)/home/mysql/bin/
	cp $(INSTALL_ROOT)/bin/mysql_install_db ipkg_src_server_root_$(CONFIGURE_TARGET)/home/mysql/bin/
	cp $(INSTALL_ROOT)/sbin/mysqld ipkg_src_server_root_$(CONFIGURE_TARGET)/home/mysql/sbin/
	cp $(INSTALL_ROOT)/share/mysql/fill_help_tables.sql ipkg_src_server_root_$(CONFIGURE_TARGET)/home/mysql/share/mysql
	cp $(INSTALL_ROOT)/share/mysql/mysql_system_tables.sql ipkg_src_server_root_$(CONFIGURE_TARGET)/home/mysql/share/mysql
	cp $(INSTALL_ROOT)/share/mysql/mysql_system_tables_data.sql ipkg_src_server_root_$(CONFIGURE_TARGET)/home/mysql/share/mysql
	cp $(INSTALL_ROOT)/share/mysql/mysql_security_commands.sql ipkg_src_server_root_$(CONFIGURE_TARGET)/home/mysql/share/mysql
	cp -a $(INSTALL_ROOT)/share/mysql/english ipkg_src_server_root_$(CONFIGURE_TARGET)/home/mysql/share/mysql/
	$(STRIP) ipkg_src_server_root_$(CONFIGURE_TARGET)/home/mysql/bin/my_print_defaults
	$(STRIP) ipkg_src_server_root_$(CONFIGURE_TARGET)/home/mysql/bin/mysqladmin
	$(STRIP) ipkg_src_server_root_$(CONFIGURE_TARGET)/home/mysql/sbin/mysqld
	$(IPKGBUILD) ipkg_src_server_root_$(CONFIGURE_TARGET) $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)

# Jared 08-05-2008 said:
# In order to speed-up the making proess, we don't clean the pre-build binaries.
# If you need to rebuild the binaries, please make distclean in this directory.
#clean:
#	-[ -d build_$(CONFIGURE_TARGET) ] && make -C build_$(CONFIGURE_TARGET) clean;
#	find -type l | xargs rm -rf;

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET) 
	rm -rf ipkg_src_client_root_$(CONFIGURE_TARGET)/usr \
		ipkg_src_server_root_$(CONFIGURE_TARGET)/usr \
		ipkg_src_server_root_$(CONFIGURE_TARGET)/home \
		ipkg_src_mysqlclient_$(CONFIGURE_TARGET)/home \
		ipkg_src_libmysqlclient_$(CONFIGURE_TARGET)/usr

