EXEC = telnetd


# -DAUTHENTICATE

# If having unused tty devices root.root and mode 600 bugs you,
# take out -DPARANOID_TTYS.

CFLAGS += '-DISSUE_FILE="/etc/issue.net"' -DPARANOID_TTYS \
	   -DNO_REVOKE -DKLUDGELINEMODE -DDIAGNOSTICS \
	   -DNO_LOGIN_P

CFLAGS += -I$(TC_INSTALL_PREFIX)/include/ncurses
#LIBS += $(LIBTERMCAP)
LIBS +=-lutil

OBJS = telnetd.o state.o termstat.o slc.o sys_term.o utility.o \
	global.o setproctitle.o

# authenc.o (empty)
all: $(EXEC)
	cp -a $(EXEC) $(TC_INSTALL_PREFIX)/bin/$(EXEC)

# logout.o logwtmp.o (now from -lutil)


$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

romfs: $(EXEC)
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/$(EXEC) /bin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)
	$(ROMFSINST) -e CONFIG_USER_TELNETD_TELNETD \
		-a "telnet  stream tcp6 nowait root /bin/telnetd" /etc/inetd.conf

package:

$(OBJS): defs.h ext.h pathnames.h telnetd.h logwtmp.h logout.h setproctitle.h
telnetd.o: version.h

install: $(EXEC)
	install -s -m$(DAEMONMODE) $(EXEC) $(INSTALLROOT)$(SBINDIR)/in.telnetd
	install -m$(MANMODE) issue.net.5 $(INSTALLROOT)$(MANDIR)/man5/
	install -m$(MANMODE) telnetd.8 $(INSTALLROOT)$(MANDIR)/man8/in.telnetd.8
	ln -sf in.telnetd.8 $(INSTALLROOT)$(MANDIR)/man8/telnetd.8

clean:
	rm -f *.o $(EXEC) 

