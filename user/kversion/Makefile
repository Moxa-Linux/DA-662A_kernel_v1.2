-include $(ARCH_CONFIG)

EXEC = kversion
OBJS = kversion.o
NOW=$(shell date +%y%m%d%H)
#CFLAGS += -DFIRM_VER='"$(VERSIONPKG)"' -DDATE='"$(NOW)"' $(VENDOR_CFLAGS)
CFLAGS += -DFIRM_VER='"$(VERSIONPKG)"' -DDATE='"$(NOW)"'

ifeq ("$(CONFIGURE_TARGET)","xscale")
CFLAGS += -I../../lib/redboot_config -I../mtd-utils/include # -DDEBUG
LDFLAGS := -lredboot -L../../lib/redboot_config
endif


all: clean $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $@ $(OBJS)

romfs: clean $(EXEC)
	$(STRIP) -s $(EXEC)
	$(ROMFSINST) /bin/$(EXEC)
	chmod 755 $(ROMFSDIR)/bin/$(EXEC)
#	chmod a+s $(ROMFSDIR)/bin/$(EXEC)

package:

clean:
	-rm -f $(EXEC) *.elf *.o *.gdb

