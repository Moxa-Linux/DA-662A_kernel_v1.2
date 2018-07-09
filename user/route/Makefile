
ROUTE = route
ROUTEOBJS = route.o

IFCONFIG = ifconfig
IFCONFIGOBJS = ifconfig.o

ARP = arp
ARPOBJS = arp.o

NETSTAT = netstat
NETSTATOBJS = netstat.o

MIITOOL = mii-tool
MIITOOLOBJS = mii-tool.o

#CFLAGS += -D_GNU_SOURCE -D__USE_BSD -Iinclude -Ilib -I../pppd/pppd
CFLAGS := -D_GNU_SOURCE -D__USE_BSD -Iinclude -Ilib -I../pppd/pppd
LIBROUTE = lib/libroute.a

# ROUTE, IFCONFIG, ARP are comment out by Jared
all: libs  $(MIITOOL) # $(ROUTE) $(IFCONFIG) $(ARP)
#	cp $(ROUTE) $(TC_INSTALL_PREFIX)/bin/$(ROUTE)
#	cp $(IFCONFIG) $(TC_INSTALL_PREFIX)/bin/$(IFCONFIG)
#	cp $(ARP) $(TC_INSTALL_PREFIX)/bin/$(ARP)
	cp $(MIITOOL) $(TC_INSTALL_PREFIX)/bin/$(MIITOOL)

$(ROUTE): $(ROUTEOBJS) $(LIBROUTE)
	$(CC) -o $@ $(ROUTEOBJS) $(LIBROUTE) -L$(TC_INSTALL_PREFIX)/lib/libnet.a
	$(STRIP) -s $(ROUTE)

$(IFCONFIG): $(IFCONFIGOBJS) $(LIBROUTE)
	$(CC) -o $@ $(IFCONFIGOBJS) $(LIBROUTE) -L$(TC_INSTALL_PREFIX)/lib/libnet.a
	$(STRIP) -s $(IFCONFIG)

$(ARP): $(ARPOBJS) $(LIBROUTE) 
	$(CC) -o $@ $(ARPOBJS) $(LIBROUTE) -L$(TC_INSTALL_PREFIX)/lib/libnet.a
	$(STRIP) -s $(ARP)

$(NETSTAT): $(NETSTATOBJS) $(LIBROUTE)
	FLTFLAGS="-s 32000" \
	$(CC) -o $@ $(NETSTATOBJS) $(LIBROUTE) -L$(TC_INSTALL_PREFIX)/lib/libnet.a
	$(STRIP) -s $(NETSTAT)

$(MIITOOL): $(MIITOOLOBJS) $(LIBROUTE) 
	$(CC) -o $@ $(MIITOOLOBJS) $(LIBROUTE) -L$(TC_INSTALL_PREFIX)/lib/libnet.a
	$(STRIP) -s $(MIITOOL)

romfs:
	$(ROMFSINST) -e CONFIG_USER_ROUTE_ROUTE $(TC_INSTALL_PREFIX)/bin/route /bin/route
	$(ROMFSINST) -e CONFIG_USER_ROUTE_IFCONFIG $(TC_INSTALL_PREFIX)/bin/ifconfig /bin/ifconfig
	$(ROMFSINST) -e CONFIG_USER_ROUTE_ARP      $(TC_INSTALL_PREFIX)/bin/arp /bin/arp
	$(ROMFSINST) -e CONFIG_USER_ROUTE_NETSTAT  $(TC_INSTALL_PREFIX)/bin/netstat /bin/netstat
	$(ROMFSINST) -e CONFIG_USER_ROUTE_MIITOOL  $(TC_INSTALL_PREFIX)/bin/mii-tool /bin/mii-tool

package:

clean:
	-rm -f $(ROUTE) $(IFCONFIG) $(ARP) $(NETSTAT) $(MIITOOL) *.elf *.gdb *.o
	-$(MAKE) -C lib clean

.PHONY: libs
libs:
	$(MAKE) -C lib


