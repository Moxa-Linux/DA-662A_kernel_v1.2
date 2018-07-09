
EXEC=moxaburn
OBJS=moxaburn.o
LDLIBS  := -lpthread 
LDFLAGS :=

CFLAGS += -DHW_ID='"$(HW_ID)"' -DAP_ID='"$(AP_ID)"' 
LDLIBS += -lmoxalib

ifeq ($(CONFIG_PRODUCT),ia261-hoson)
OBJS += CommPort.o ModbusRTU.o
endif

ifeq ($(CONFIG_PRODUCT),ia261-shw)
OBJS += CommPort.o ModbusRTU.o
endif

ifeq ($(CONFIG_PRODUCT),w416)
OBJS += CommPort.o ModbusRTU.o
endif

CFLAGS += -DFACTORY_MODE #-DDEBUG #-DHW_ID='"0x026c"' -DAP_ID='"0x8000026c"'

all: $(EXEC).$(CONFIG_PRODUCT)

$(EXEC).$(CONFIG_PRODUCT): $(EXEC)
	cp -a $(EXEC) $(EXEC).$(CONFIG_PRODUCT)

$(EXEC): $(OBJS) 
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

romfs:
	$(ROMFSINST) $(EXEC).$(CONFIG_PRODUCT) /bin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)

clean:
	rm -fr $(EXEC) $(OBJS) *.o

package:
