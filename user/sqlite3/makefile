# History:
# Date          Author                  Comment
# 07-13-2012	Jimmy_Chen
#
# compile:
#	make user/sqlite3_only
#

PACKAGE_NAME = sqlite-autoconf-3071201
EXEC = sqlite3

all: $(TC_INSTALL_PREFIX)/bin/$(EXEC)

$(TC_INSTALL_PREFIX)/bin/$(EXEC):build_$(CONFIGURE_TARGET)-linux/objs/$(EXEC)
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install
	echo "Install $(EXEC) complete!"

build_$(CONFIGURE_TARGET)-linux/objs/$(EXEC): build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux
	echo "Build $(EXEC) bomplete!"

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf $(PACKAGE_NAME).tar.gz
	mv sqlite3 build_$(CONFIGURE_TARGET)-linux
	sed -i 's/sqlite 3.7.12/sqlite_3.7.12/g' build_$(CONFIGURE_TARGET)-linux/configure
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CXX CXXCPP CXXCPP CFLAGS CPPFLAGS LDFLAGS CXXFLAGS PACKAGE_STRING && \
	CC=$(CROSS)gcc \
	STRIP=$(CROSS)strip \
	CXX=$(CROSS)gcc \
	LD=$(CROSS)ld \
	./configure \
	--host=$(CONFIGURE_TARGET)-linux \
	--prefix=$(TC_INSTALL_PREFIX) \
	--disable-readline \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured

romfs:
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/$(EXEC) /usr/bin/$(EXEC)
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/lib/libsqlite3.so.0.8.6 /lib/libsqlite3.so.0.8.6
	$(ROMFSINST) -s /lib/libsqlite3.so.0.8.6 /lib/libsqlite3.so.0
	$(ROMFSINST) -s /lib/libsqlite3.so.0 /lib/libsqlite3.so
	$(CROSS)strip $(ROMFSDIR)/lib/libsqlite3.so.0.8.6 $(ROMFSDIR)/usr/bin/$(EXEC)

package: $(TC_INSTALL_PREFIX)/bin/$(EXEC)
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/bin
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/lib
	cp -a $(TC_INSTALL_PREFIX)/bin/$(EXEC) ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a $(TC_INSTALL_PREFIX)/lib/libsqlite3.so.0.8.6 ipkg_$(CONFIGURE_TARGET)-linux/lib/libsqlite3.so.0.8.6
	$(CROSS)strip ipkg_$(CONFIGURE_TARGET)-linux/usr/bin/$(EXEC) \
	ipkg_$(CONFIGURE_TARGET)-linux/lib/libsqlite3.so.0.8.6
	(cd ipkg_$(CONFIGURE_TARGET)-linux/lib; \
	ln -sf libsqlite3.so.0.8.6 libsqlite3.so.0; \
	ln -sf libsqlite3.so.0.8.6 libsqlite3.so; \
	)
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

clean:
	/bin/rm -rf build_$(CONFIGURE_TARGET)-linux \
	ipkg_$(CONFIGURE_TARGET)-linux/usr \
	ipkg_$(CONFIGURE_TARGET)-linux/lib

