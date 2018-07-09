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
CFLAGS += -DBIG_ENDIAN -DARCH_UC8400 -DARCH_UC9400
else
LDFLAGS :=
endif

PACKAGE_NAME=dhcp-isc

all: 
	if [ ! -d build_$(CONFIGURE_TARGET)-linux ]; then \
		tar xzvf $(PACKAGE_NAME).tar.gz; \
		mv $(PACKAGE_NAME) build_$(CONFIGURE_TARGET)-linux; \
	fi
	make -C build_$(CONFIGURE_TARGET)-linux

romfs: 
	make -C build_$(CONFIGURE_TARGET)-linux romfs

package: all
	# Make the ipkg package
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/bin
	if [ "$(CONFIG_USER_DHCP_ISC_SERVER_DHCPD)" = "y" ]; then \
		mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/init.d; \
		mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/config; \
		cp -a build_$(CONFIGURE_TARGET)-linux/server/dhcpd ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/bin/dhcpd; \
		cp -a build_$(CONFIGURE_TARGET)-linux/server/dhcpd.conf ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/config/dhcpd.conf.example; \
		cp -a build_$(CONFIGURE_TARGET)-linux/server/dhcpd.script.pkg ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/init.d/dhcpd; \
		touch ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/config/dhcpd.leases; \
		$(STRIP) -s ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/bin/dhcpd; \
	fi
	if [ "$(CONFIG_USER_DHCP_ISC_CLIENT_DHCLIENT)" = "y" ]; then \
		mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/init.d; \
		cp -a build_$(CONFIGURE_TARGET)-linux/client/dhclient ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/bin/dhclient; \
		cp -a build_$(CONFIGURE_TARGET)-linux/client/scripts/linux ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/init.d/dhclient; \
	fi
	if [ "$(CONFIG_USER_DHCP_ISC_RELAY_DHCRELAY)" = "y" ]; then \
		cp -a build_$(CONFIGURE_TARGET)-linux/relay/dhcrelay ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/bin/dhcrelay; \
	fi
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_src_root_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

clean:
	/bin/rm -rf build_$(CONFIGURE_TARGET)-linux
	/bin/rm -rf ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr
	/bin/rm -rf ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc

