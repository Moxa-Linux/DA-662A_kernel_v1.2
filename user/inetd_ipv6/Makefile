
SERVICES =    \"/etc/services\"
INETD_CONF =  \"/etc/inetd.conf\"

EXEC = inetd
OBJS = inetd.o config.o

CFLAGS += -DSERVICES=$(SERVICES) -DINETD_CONF=$(INETD_CONF)
CFLAGS += -Wall
# Add by Jared 2011-11-15
CFLAGS += -DCONFIG_IPV6 -DMODIFY_CONFIG -DALT_INETD_CONF -DEMBED -DSUPPORT_BACKGROUND_RUN

# These changes allow for an additional, optional config file in /etc/config/inetd.conf
# which can be modified
CFLAGS += -DMODIFY_CONFIG -DALT_INETD_CONF=\"/etc/config/inetd.conf\"

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS$(LDLIBS_$@))

clean:
	rm -f $(EXEC) *.elf *.gdb *.o

romfs: $(EXEC)
	$(STRIP) -s $(EXEC)
	$(ROMFSINST) $(EXEC) /bin/$(EXEC)

package: $(EXEC)
	# Make the ipkg package
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/bin/
	cp -a ipkg_default/* ipkg_$(CONFIGURE_TARGET)-linux/
	cp -a $(EXEC) ipkg_$(CONFIGURE_TARGET)-linux/bin/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/bin/$(EXEC)
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


