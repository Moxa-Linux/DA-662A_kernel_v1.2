
-include $(ARCH_CONFIG)

EXEC=mxmptest
OBJS=mxmptest.o

CFLAGS += $(VENDOR_CFLAGS)
LDFLAGS :=
LDLIBS :=

all:	$(EXEC).$(CONFIG_PRODUCT)

$(EXEC).$(CONFIG_PRODUCT): $(EXEC)
	cp -a $(EXEC) $(EXEC).$(CONFIG_PRODUCT)

$(EXEC):	$(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

romfs: $(EXEC).$(CONFIG_PRODUCT)
	$(ROMFSINST) $(EXEC).$(CONFIG_PRODUCT) /bin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)

package: $(EXEC).$(CONFIG_PRODUCT)

clean:
	rm -fr $(EXEC) $(OBJS)
