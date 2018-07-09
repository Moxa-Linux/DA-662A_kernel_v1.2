# History:
# Date          Author			Comment
# 08-13-2013	Simon Wang		First Create
#
# compile:
#	make user/syslog-ng-3.4.1_only
#

PACKAGE_NAME = ntp-4.2.6p5
EXEC = ntpd

all: $(TC_INSTALL_PREFIX)/bin/$(EXEC)

$(TC_INSTALL_PREFIX)/bin/$(EXEC): build_$(CONFIGURE_TARGET)-linux/ntpd/$(EXEC)
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install
	echo "Install $(EXEC) complete!"

build_$(CONFIGURE_TARGET)-linux/ntpd/$(EXEC): build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux
	echo "Build $(EXEC) bomplete!"

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf $(PACKAGE_NAME).tar.gz
	mv ntp-4.2.6p5 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CXX CXXCPP CXXCPP CFLAGS CPPFLAGS LDFLAGS CXXFLAGS PACKAGE_STRING && \
	CC=$(CROSS)gcc \
	STRIP=$(CROSS)strip \
	CXX=$(CROSS)gcc \
	LD=$(CROSS)ld \
	./configure \
	--host=$(CONFIGURE_TARGET)-linux \
	--prefix=$(TC_INSTALL_PREFIX) \
	--sysconfdir=/etc \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured

romfs:
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/$(EXEC) /bin/$(EXEC)
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/ntpq /bin/ntpq
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/ntpdate /bin/ntpdate
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/ntptime /bin/ntptime
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/sntp /bin/sntp
	$(CROSS)strip $(ROMFSDIR)/bin/$(EXEC) $(ROMFSDIR)/bin/ntpq $(ROMFSDIR)/bin/ntpdate $(ROMFSDIR)/bin/ntptime $(ROMFSDIR)/bin/sntp

clean:
	/bin/rm -rf build_$(CONFIGURE_TARGET)-linux \

distclean:
	/bin/rm -rf build_$(CONFIGURE_TARGET)-linux \

