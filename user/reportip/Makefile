# History:
# Date		Author			Comment
# 06-17-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
# 	cd stdlinux
# 	make user/reportip_only

-include $(ARCH_CONFIG)

EXEC=reportip

CFLAGS += -I../../lib/redboot_config -I../mtd-utils/include -DKVERSION='"$(FULLVERSIONPKG)"' $(VENDOR_CFLAGS)

CFLAGS += -DHW_ID='"$(HW_ID)"' -DAP_ID='"$(AP_ID)"'

ifeq ("$(CONFIGURE_TARGET)","xscale")
CFLAGS += -D__BYTE_ORDER=__BIG_ENDIAN 
LDFLAGS := -lredboot -L$(TC_INSTALL_PREFIX)/lib
else
LDFLAGS := 
endif

all: $(EXEC).$(CONFIG_PRODUCT)
	
$(EXEC).$(CONFIG_PRODUCT): $(EXEC)
	cp $(EXEC) $(EXEC).$(CONFIG_PRODUCT)

$(EXEC):	$(EXEC).o
	$(CC) -o $@ $(EXEC).o $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $? -c

romfs: $(EXEC).$(CONFIG_PRODUCT)
	$(ROMFSINST) $(EXEC).$(CONFIG_PRODUCT) /bin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)

package:

clean:
	rm -fr $(EXEC) $(EXEC).o
