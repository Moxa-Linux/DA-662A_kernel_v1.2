# History:
# Date          Author			Comment
# 08-13-2013	Simon Wang		First Create
#
# compile:
#	make user/syslog-ng-3.4.1_only
#

PACKAGE_NAME = syslog-ng_3.4.1
EXEC = syslog-ng

all: $(TC_INSTALL_PREFIX)/sbin/$(EXEC)

$(TC_INSTALL_PREFIX)/sbin/$(EXEC):build_$(CONFIGURE_TARGET)-linux/syslog-ng/.libs/$(EXEC)
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install
	echo "Install $(EXEC) complete!"

build_$(CONFIGURE_TARGET)-linux/syslog-ng/.libs/$(EXEC): build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux
	echo "Build $(EXEC) bomplete!"

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf $(PACKAGE_NAME).tar.gz
	mv syslog-ng-3.4.1 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CXX CXXCPP CXXCPP CFLAGS CPPFLAGS LDFLAGS CXXFLAGS PACKAGE_STRING && \
	CC=$(CROSS)gcc \
	STRIP=$(CROSS)strip \
	CXX=$(CROSS)gcc \
	LD=$(CROSS)ld \
	PKG_CONFIG_PATH=$(TC_INSTALL_PREFIX)/lib/pkgconfig \
	./configure \
	--host=$(CONFIGURE_TARGET)-linux \
	--prefix=$(TC_INSTALL_PREFIX) \
	--enable-ssl \
	)
	sed -i 's/\/usr\/local\/arm-mv5sft-linux-gnueabi\/arm-mv5sft-linux-gnueabi//g' build_$(CONFIGURE_TARGET)-linux/config.h
	sed -i 's/PATH_PREFIX \"\"/PATH_PREFIX \"\/\"/g' build_$(CONFIGURE_TARGET)-linux/config.h
	sed -i 's/\/etc/\/etc\/syslog-ng/g' build_$(CONFIGURE_TARGET)-linux/config.h
	sed -i 's/\/share/\/etc\/syslog-ng/g' build_$(CONFIGURE_TARGET)-linux/config.h
	touch build_$(CONFIGURE_TARGET)-linux/configured

romfs:
	mkdir -p $(ROMFSDIR)/lib/syslog-ng
	cp -a $(TC_INSTALL_PREFIX)/lib/syslog-ng/* $(ROMFSDIR)/lib/syslog-ng
	mkdir -p $(ROMFSDIR)/etc/syslog-ng/include/scl/pacct
	mkdir -p $(ROMFSDIR)/etc/syslog-ng/include/scl/syslogconf
	mkdir -p $(ROMFSDIR)/etc/syslog-ng/include/scl/system
	-[ "$(CONFIG_PRODUCT)" = "uc1260" ] || $(ROMFSINST) $(TC_INSTALL_PREFIX)/etc/syslog-ng.conf /etc/syslog-ng/syslog-ng.conf
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/etc/scl.conf /etc/syslog-ng/scl.conf
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/share/include/scl/pacct/plugin.conf /etc/syslog-ng/include/scl/pacct/plugin.conf
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/share/include/scl/syslogconf/plugin.conf /etc/syslog-ng/include/scl/syslogconf/plugin.conf
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/share/include/scl/syslogconf/convert-syslogconf.awk /etc/syslog-ng/include/scl/syslogconf/convert-syslogconf.awk
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/share/include/scl/system/plugin.conf /etc/syslog-ng/include/scl/system/plugin.conf
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/sbin/$(EXEC) /sbin/$(EXEC)
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/lib/libsyslog-ng-3.4.1.so /lib/libsyslog-ng-3.4.1.so
	$(ROMFSINST) -s /lib/libsyslog-ng-3.4.1.so /lib/libsyslog-ng.so
	$(ROMFSINST) -s /lib/syslog-ng/libsyslog-ng-crypto.so /usr/lib/libsyslog-ng-crypto.so
	$(CROSS)strip $(ROMFSDIR)/lib/libsyslog-ng-3.4.1.so $(ROMFSDIR)/sbin/$(EXEC)

clean:
	/bin/rm -rf build_$(CONFIGURE_TARGET)-linux \

distclean:
	/bin/rm -rf build_$(CONFIGURE_TARGET)-linux \

