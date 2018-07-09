EXEC = snmpd
OBJS = snmpd.o

CFLAGS += -I../snmplib -I../modules

# Need a bigger stack than the default
#FLTFLAGS += -s 8000

USELIBS = ../modules/libmodules.a ../snmplib/libsnmplib.a

all: $(EXEC).$(CONFIG_PRODUCT)

$(EXEC).$(CONFIG_PRODUCT): $(EXEC)
	cp -a $(EXEC) $(EXEC).$(CONFIG_PRODUCT)

$(EXEC): $(OBJS) $(USELIBS)
	$(CC) -o $@ $(OBJS) $(USELIBS)

romfs:
	$(ROMFSINST) $(EXEC).$(CONFIG_PRODUCT) /bin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)

distclean clean:
	-rm -f $(EXEC) *.gdb *.elf *.o

test:
