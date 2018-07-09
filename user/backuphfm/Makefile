# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
# 06-05-2013	Jared Wu.		Install into romfs
#
# compile: 
#	make lib/backuphfm_only
#

-include $(ARCH_CONFIG)

#CROSS=xscale-linux-
CC=$(CROSS)gcc
STRIP=$(CROSS)strip
GDB=$(CROSS)gdb
CFLAGS= $(VENDOR_CFLAGS) -DHW_ID='"$(HW_ID)"' -DAP_ID='"$(AP_ID)"' -I../../host/include -I../../lib/redboot_config -I../mtd-utils/include/

ifeq ("$(CONFIGURE_TARGET)","xscale")
CFLAGS += -DBIG_ENDIAN -DARCH_UC8400 
LDFLAGS := -lredboot -L$(TC_INSTALL_PREFIX)/lib
else
LDFLAGS :=
endif

EXEC=backuphfm

all: $(EXEC).$(CONFIG_PRODUCT)

$(EXEC).$(CONFIG_PRODUCT): $(EXEC)
	cp -a $(EXEC) $(EXEC).$(CONFIG_PRODUCT)

$(EXEC): $(EXEC).c
	$(CC) $(EXEC).c $(CFLAGS) $(LDFLAGS) -o $(EXEC)
	$(CC) $(EXEC).c $(CFLAGS) -DDEBUG -DTEST $(LDFLAGS) -o $(EXEC).debug

romfs: $(EXEC).$(CONFIG_PRODUCT)
	$(ROMFSINST) $(EXEC).$(CONFIG_PRODUCT) /bin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)

package: $(EXEC).$(CONFIG_PRODUCT)
	# Make the ipkg package
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/bin
	cp -a $(EXEC).$(CONFIG_PRODUCT) ipkg_$(CONFIGURE_TARGET)-linux/bin/$(EXEC)
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/bin/$(EXEC)
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

clean:
	/bin/rm -f $(EXEC) $(EXEC).debug
	/bin/rm -f ipkg_$(CONFIGURE_TARGET)-linux/bin/*
