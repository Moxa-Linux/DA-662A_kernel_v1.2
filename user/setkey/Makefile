# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# Compile:
#	make user/setkey_only

-include $(ARCH_CONFIG)

EXEC=setkey

CFLAGS := -I../mtd-utils/include $(VENDOR_CFLAGS)

ifeq ("$(CONFIGURE_TARGET)","xscale")
CFLAGS += -I../../lib/redboot_config
LDFLAGS := -lredboot -L$(TC_INSTALL_PREFIX)/lib
else
LDFLAGS := 
endif

all:	$(TC_INSTALL_PREFIX)/bin/$(EXEC)

$(TC_INSTALL_PREFIX)/bin/$(EXEC): $(EXEC)
	cp $(EXEC) $(TC_INSTALL_PREFIX)/bin/

$(EXEC):	$(EXEC).c
	if [ ! -f $(TC_INSTALL_PREFIX)/lib/libredboot.a ]; then \
		make -C ../../lib/redboot_config; \
		cp -f ../../lib/redboot_config/libredboot.a $(TC_INSTALL_PREFIX)/lib; \
	fi
	$(CC) $(EXEC).c $(CFLAGS) -o $@ $(LDFLAGS)

romfs: $(TC_INSTALL_PREFIX)/bin/$(EXEC)
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/$(EXEC) /bin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)

package: $(TC_INSTALL_PREFIX)/bin/$(EXEC)

clean:
	rm -fr $(EXEC)
