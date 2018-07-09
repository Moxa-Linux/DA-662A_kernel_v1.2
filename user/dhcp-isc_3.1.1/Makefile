# History:
# Date          Author                  Comment
# 01-02-2012    Aeluin Chen.            new created
#
# compile:
#	make user/dhcp-isc_only
#

-include $(ARCH_CONFIG)
-include $(CONFIG_CONFIG)

CC=$(CROSS)gcc
STRIP=$(CROSS)strip
GDB=$(CROSS)gdb

ifeq ("$(CONFIGURE_TARGET)","xscale")
CFLAGS += -DBIG_ENDIAN
endif

PACKAGE_NAME=dhcp-3.1.1

all: build_$(CONFIGURE_TARGET)-linux/server/dhcpd

build_$(CONFIGURE_TARGET)-linux/server/dhcpd:build_$(CONFIGURE_TARGET)-linux/configured
	make -C build_$(CONFIGURE_TARGET)-linux
	echo "Build complete"

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	if [ ! -d build_$(CONFIGURE_TARGET)-linux ]; then \
		tar xzvf $(PACKAGE_NAME).tar.gz; \
		mv $(PACKAGE_NAME) build_$(CONFIGURE_TARGET)-linux; \
	fi
	(cd  build_$(CONFIGURE_TARGET)-linux; \
		patch -p1 -i ../$(PACKAGE_NAME)-cross_build.patch; \
		./configure linux-2.2)
	touch build_$(CONFIGURE_TARGET)-linux/configured

romfs: 
ifeq ($(CONFIG_USER_DHCP_ISC_3_1_1_SERVER_DHCPD),y)
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/work.linux-2.2/server/dhcpd /usr/sbin/
	$(STRIP) $(ROMFSDIR)/usr/sbin/dhcpd
endif
ifeq ($(CONFIG_USER_DHCP_ISC_3_1_1_CLIENT_DHCLIENT),y)
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/work.linux-2.2/client/dhclient /sbin/
	$(STRIP) $(ROMFSDIR)/sbin/dhclient
endif
ifeq ($(CONFIG_USER_DHCP_ISC_3_1_1_RELAY_DHCRELAY),y)
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/work.linux-2.2/relay/dhcrelay /sbin/
	$(STRIP) $(ROMFSDIR)/sbin/dhcrelay
endif

package: build_$(CONFIGURE_TARGET)-linux/server/dhcpd
	# Make the ipkg package
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin \
	ipkg_$(CONFIGURE_TARGET)-linux/sbin \
	if [ "$(CONFIG_USER_DHCP_ISC_3_1_1_SERVER_DHCPD)" = "y" ]; then \
		mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/etc/dhcp3; \
		cp -a build_$(CONFIGURE_TARGET)-linux/work.linux-2.2/server/dhcpd  ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin/; \
		cp -a build_$(CONFIGURE_TARGET)-linux/server/dhcpd.conf ipkg_$(CONFIGURE_TARGET)-linux/etc/dhcp3/dhcpd.conf; \
		touch ipkg_$(CONFIGURE_TARGET)-linux/etc/dhcp3/dhcpd.leases; \
		$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin/dhcpd; \
	fi
	if [ "$(CONFIG_USER_DHCP_ISC_3_1_1_CLIENT_DHCLIENT)" = "y" ]; then \
		cp -a build_$(CONFIGURE_TARGET)-linux/work.linux-2.2/client/dhclient ipkg_$(CONFIGURE_TARGET)-linux/sbin/dhclient; \
	fi
	if [ "$(CONFIG_USER_DHCP_ISC_3_1_1_RELAY_DHCRELAY)" = "y" ]; then \
		cp -a build_$(CONFIGURE_TARGET)-linux/work.linux-2.2/relay/dhcrelay ipkg_$(CONFIGURE_TARGET)-linux/sbin/dhcrelay; \
	fi
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

clean:
	/bin/rm -rf build_$(CONFIGURE_TARGET)-linux \
	ipkg_$(CONFIGURE_TARGET)-linux/usr \
	ipkg_$(CONFIGURE_TARGET)-linux/sbin \
	ipkg_$(CONFIGURE_TARGET)-linux/etc/dhcpd.conf

