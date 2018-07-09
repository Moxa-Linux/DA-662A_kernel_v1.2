
-include $(ARCH_CONFIG)

EXEC=moxaburn
OBJS=moxaburn.o
LDFLAGS=-lpthread
CFLAGS += -DFACTORY_MODE -I../mtd-utils/include -I../../lib/redboot_config $(VENDOR_CFLAGS) -DDEBUG

ifeq ("$(CONFIGURE_TARGET)","xscale")
LDFLAGS += -lredboot -L../../lib/redboot_config
endif

all: $(EXEC).$(CONFIG_PRODUCT)

$(EXEC).$(CONFIG_PRODUCT): $(EXEC)
	cp -a $(EXEC) $(EXEC).$(CONFIG_PRODUCT)

$(EXEC): $(OBJS) 
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $? -c

romfs:
	$(ROMFSINST) $(EXEC).$(CONFIG_PRODUCT) /bin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)

package:

clean:
	rm -fr $(EXEC) $(OBJS)
