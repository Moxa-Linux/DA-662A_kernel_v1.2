
EXEC = ftpd
OBJS = auth.o conf.o ftpcmd.o ftpd.o popen.o server_mode.o localhost.o xgetcwd.o logwtmp.o xmalloc.o

CFLAGS += -DHAVE_CONFIG_H=1 -I.
CFLAGS += -DPATH_FTPWELCOME=\"/etc/ftpwelcome\"
CFLAGS += -DPATH_FTPLOGINMESG=\"/etc/motd\"
CFLAGS += -DPATH_FTPUSERS=\"/etc/ftpusers\"
CFLAGS += -DPATH_BSHELL=\"/bin/sh\"
CFLAGS += -DPATH_FTPCHROOT=\"/usr/sbin/chroot\"
CFLAGS += -DPATH_FTPDPID=\"/var/run/ftpd.pid\"
CFLAGS += -DPATH_DEVNULL=\"/dev/null\"

ifeq ($(findstring glibc,$(LIBCDIR)),glibc)
CFLAGS += -DPATH_NOLOGIN=\"/etc/nologin\"
CFLAGS += -DPATH_WTMP=\"/etc/wtmp\"
CFLAGS += -DHAVE_ERR
endif

CFLAGS += -DKEEP_OPEN=1

LIBCRYPT=-lcrypt
EXTRALIBS = $(LIBCRYPT)

# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011    Aeluin Chen.    do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#

all:  $(TC_INSTALL_PREFIX)/bin/$(EXEC)

$(TC_INSTALL_PREFIX)/bin/$(EXEC): $(EXEC)
	cp $(EXEC) $(TC_INSTALL_PREFIX)/bin/
	[ -d $(ROMFSDIR)/etc ] || mkdir -p $(ROMFSDIR)/etc
	$(ROMFSINST) -e CONFIG_USER_FTPD_FTPD \
		-a "ftp     stream tcp nowait root /bin/ftpd -l" /etc/inetd.conf
	mv $(ROMFSDIR)/etc/inetd.conf $(TC_INSTALL_PREFIX)/etc/

$(EXEC): $(OBJS)
	$(CC) -o $@ $(OBJS) $(EXTRALIBS)

romfs:
ifeq ("$(CONFIG_PRODUCT)", "rnas1200")
	$(ROMFSINST) $(EXEC) /bin/$(EXEC)
else
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/$(EXEC) /bin/$(EXEC)
endif
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)
	$(ROMFSINST) -e CONFIG_USER_FTPD_FTPD \
		-a "ftp     stream tcp nowait root /bin/ftpd -l" /etc/inetd.conf

package:

clean:
	-rm -f $(EXEC) *.elf *.elf2flt *.gdb *.o

