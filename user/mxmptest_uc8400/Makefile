-include $(ARCH_CONFIG)

EXEC=mxmptest
OBJS=mxmptest.o

CFLAGS += -I. $(VENDOR_CFLAGS)

ifeq ("$(CONFIGURE_TARGET)","xscale")
CFLAGS += -I../../lib/redboot_config -I../mtd-utils/include # -DDEBUG
LDFLAGS := -lredboot -L$(TC_INSTALL_PREFIX)/lib
endif

all:	$(EXEC)
	# Keep the mxmptest for each product
	cp -a $(EXEC) $(EXEC).$(CONFIG_PRODUCT)

$(EXEC): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
	$(STRIP) $(EXEC)

.c.o:
	$(CC) $(CFLAGS) $? -c

romfs: $(EXEC).$(CONFIG_PRODUCT)
	$(ROMFSINST) $(EXEC).$(CONFIG_PRODUCT) /bin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)

package: $(EXEC).$(CONFIG_PRODUCT)

clean:
	rm -fr $(EXEC) $(OBJS)
