# History:
# Date		Author			Comment
# 06-17-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
# 05-20-2016	Jared Wu.		only allow administrator to upgrade firmware.
#
# Cross-compile big-endian:
#	make user/upgradehfm_only

include $(ARCH_CONFIG)

EXEC=upgradehfm
CFLAGS := $(VENDOR_CFLAGS) -DHW_ID='"$(HW_ID)"' -DAP_ID='"$(AP_ID)"' -I../../lib/redboot_config -I../mtd-utils/include/

ifeq ("$(CONFIGURE_TARGET)","xscale")
CFLAGS += -DBIG_ENDIAN 
else
endif
LDFLAGS := -lredboot -L$(TC_INSTALL_PREFIX)/lib

all: $(EXEC).$(CONFIG_PRODUCT)

$(EXEC).$(CONFIG_PRODUCT): #$(EXEC)
	cp -a $(EXEC) $(EXEC).$(CONFIG_PRODUCT)

$(EXEC): $(EXEC).c
	$(CC) $(EXEC).c $(CFLAGS) $(LDFLAGS) -o $@ 
	$(CC) $(EXEC).c $(CFLAGS) -DDEBUG -DTEST $(LDFLAGS) -o $@.debug

romfs:	$(EXEC).$(CONFIG_PRODUCT)
	$(ROMFSINST) $(EXEC).$(CONFIG_PRODUCT) /bin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)
	chmod og-rx $(ROMFSDIR)/bin/$(EXEC)
	(cd $(ROMFSDIR)/bin; ln -sf $(EXEC) upfirm)

package:

clean:
	# rm -fr $(EXEC) $(EXEC).debug
