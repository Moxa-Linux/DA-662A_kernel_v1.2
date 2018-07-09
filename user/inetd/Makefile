# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011    Aeluin Chen.    do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile:
#	make user/inetd_only

SERVICES =    \"/etc/services\"
INETD_CONF =  \"/etc/inetd.conf\"

EXEC = inetd
OBJS = inetd.o config.o

CFLAGS += -DSERVICES=$(SERVICES) -DINETD_CONF=$(INETD_CONF)
CFLAGS += -Wall

# These changes allow for an additional, optional config file in /etc/config/inetd.conf
# which can be modified
CFLAGS += -DMODIFY_CONFIG -DALT_INETD_CONF=\"/etc/config/inetd.conf\"

all: $(TC_INSTALL_PREFIX)/bin/$(EXEC)

$(TC_INSTALL_PREFIX)/bin/$(EXEC): $(EXEC)
	cp $(EXEC) $(TC_INSTALL_PREFIX)/bin/

$(EXEC): $(OBJS)
	$(CC) -o $@ $(OBJS)

clean:
	rm -f $(EXEC) *.elf *.gdb *.o

romfs:
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/$(EXEC) /bin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)

package:
