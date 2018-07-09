# History:
# Date    Author    Comment
# 06-11-2012   Simon Wang   First Built.
#
# compile:
# make user/samba-3.5.6_only

PACKAGE_NAME = samba-3.5.6

# Define Modules
VFS_MODULES = recycle.so audit.so extd_audit.so full_audit.so netatalk.so fake_perms.so default_quota.so readonly.so cap.so expand_msdfs.so shadow_copy.so shadow_copy2.so xattr_tdb.so catia.so streams_xattr.so streams_depot.so readahead.so aio_fork.so preopen.so syncops.so acl_xattr.so acl_tdb.so smb_traffic_analyzer.so dirsort.so scannedonly.so
PERFCOUNT_MODULES =
PDB_MODULES =
RPC_MODULES =
IDMAP_MODULES =
CHARSET_MODULES = CP850.so CP437.so
AUTH_MODULES = script.so
NSS_INFO_MODULES =
GPEXT_MODULES =
MODULES = $(VFS_MODULES) $(PDB_MODULES) $(RPC_MODULES) $(IDMAP_MODULES) \
					$(CHARSET_MODULES) $(AUTH_MODULES) $(NSS_INFO_MODULES) \
					$(GPEXT_MODULES) $(PERFCOUNT_MODULES)

# Define Library
LIBS = libnetapi.so.0 libsmbclient.so.0 libsmbsharemodes.so.0 libtalloc.so.2 libtdb.so.1

# Define DAT files
DATS = lowcase.dat upcase.dat valid.dat

# Define Binary files
BIN_EXEC = pdbedit smbpasswd smbclient testparm
SBIN_EXEC = smbd nmbd
EXEC = $(BIN_EXEC) $(SBIN_EXEC)



all: build_$(CONFIGURE_TARGET)-linux/source3/$(EXEC)
	echo "Build Complete!"

build_$(CONFIGURE_TARGET)-linux/source3/$(EXEC): build_$(CONFIGURE_TARGET)-linux/source3/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux/source3

build_$(CONFIGURE_TARGET)-linux/source3/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf $(PACKAGE_NAME).tar.gz; \
	(mv samba-3.5.6 build_$(CONFIGURE_TARGET)-linux)
	( cd build_$(CONFIGURE_TARGET)-linux/source3; \
	unset CC CPP CFLAGS CPPFLAGS LD LDFLAGS STRIP && \
	CC=$(CROSS)gcc \
	CPP=$(CROSS)cpp \
	LD=$(CROSS)ld \
	AR=$(CROSS)ar \
	STRIP=$(CROSS)strip \
	CFLAGS="-I$(TC_INSTALL_PREFIX)/include" \
	CPPFLAGS="-I$(TC_INSTALL_PREFIX)/include" \
	LDGLAGS="-L$(TC_INSTALL_PREFIX)/lib" \
	./configure \
	--host=$(CONFIGURE_TARGET)-linux \
	--prefix=/ \
	--enable-swat=no \
	--enable-cups=no \
	--enable-iprint=no \
	--enable-dnssd=no \
	--enable-shared-libs=yes \
	--with-ldap=no \
	--with-ads=no \
	--with-winbind=no \
	--with-sendfile-support=yes \
	--with-libiconv=$(TC_INSTALL_PREFIX) \
	--with-configdir=/etc \
	--with-privatedir=/etc \
	--with-logfilebase=/dev/null \
	)
	touch build_$(CONFIGURE_TARGET)-linux/source3/configured
	sed -ie 's/\/\* #undef USE_SETRESUID \*\//#define USE_SETRESUID 1/g' build_$(CONFIGURE_TARGET)-linux/source3/include/config.h

romfs:
	-$(ROMFSINST) -d 755 /lib/vfs
	-$(ROMFSINST) -d 755 /lib/charset
	-$(ROMFSINST) -d 755 /lib/auth
	-$(ROMFSINST) -d 755 /var/locks
	-$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/source3/smb.conf /etc/smb.conf;
	-$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/source3/passdb.tdb /etc/passdb.tdb;
	
	for i in $(SBIN_EXEC); do \
		$(CROSS)strip build_$(CONFIGURE_TARGET)-linux/source3/bin/$$i; \
		$(ROMFSINST) -p 777 build_$(CONFIGURE_TARGET)-linux/source3/bin/$$i /sbin/$$i; \
	done
	for i in $(BIN_EXEC); do \
		$(CROSS)strip build_$(CONFIGURE_TARGET)-linux/source3/bin/$$i; \
		$(ROMFSINST) -p 777 build_$(CONFIGURE_TARGET)-linux/source3/bin/$$i /bin/$$i; \
	done
	for i in $(LIBS); do \
		$(CROSS)strip build_$(CONFIGURE_TARGET)-linux/source3/bin/$$i; \
		$(ROMFSINST) -p 777 build_$(CONFIGURE_TARGET)-linux/source3/bin/$$i /lib/$$i; \
		$(ROMFSINST) -s /lib/$$i /lib/`echo $$i | sed 's/.[0-9]$$//'`; \
	done
	for i in $(VFS_MODULES); do \
		$(CROSS)strip build_$(CONFIGURE_TARGET)-linux/source3/bin/$$i; \
		$(ROMFSINST) -p 777 build_$(CONFIGURE_TARGET)-linux/source3/bin/$$i /lib/vfs/$$i; \
	done
	for i in $(CHARSET_MODULES); do \
		$(CROSS)strip build_$(CONFIGURE_TARGET)-linux/source3/bin/$$i; \
		$(ROMFSINST) -p 777 build_$(CONFIGURE_TARGET)-linux/source3/bin/$$i /lib/charset/$$i; \
	done
	for i in $(AUTH_MODULES); do \
		$(CROSS)strip build_$(CONFIGURE_TARGET)-linux/source3/bin/$$i; \
		$(ROMFSINST) -p 777 build_$(CONFIGURE_TARGET)-linux/source3/bin/$$i /lib/auth/$$i; \
	done
	for i in $(DATS); do \
		$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/codepages/$$i /lib/$$i; \
	done


package: build_$(CONFIGURE_TARGET)-linux/source3/$(EXEC)

clean:
	/bin/rm -rf build_$(CONFIGURE_TARGET)-linux \

