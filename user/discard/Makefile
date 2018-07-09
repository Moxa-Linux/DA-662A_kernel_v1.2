# History:
# Date		Author			Comment
# 06-17-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011    Aeluin Chen.    do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/discard_only

EXEC = discard
OBJS = discard.o

EXEC1 = inetd-echo
OBJS1 = inetd-echo.o

ifeq ($(CONFIG_USER_DISCARD_NO_INSTALL), y)
ROMFSINST_DISCARD=true
else
ROMFSINST_DISCARD=$(ROMFSINST)
endif
export ROMFSINST_DISCARD

ifeq ($(CONFIG_USER_DISCARD_ECHO_NO_INSTALL), y)
ROMFSINST_ECHO=true
else
ROMFSINST_ECHO=$(ROMFSINST)
endif
export ROMFSINST_ECHO

all: $(EXEC) $(EXEC1)
	cp $(EXEC) $(TC_INSTALL_PREFIX)/bin/$(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $@ $(OBJS)
	$(STRIP) -s $(EXEC)

$(EXEC1): $(OBJS1)
	$(CC) -o $@ $(OBJS1)
	$(STRIP) -s $(EXEC1)

romfs:
	$(ROMFSINST) -e CONFIG_USER_DISCARD_DISCARD $(TC_INSTALL_PREFIX)/bin/$(EXEC) /bin/$(EXEC)
	$(ROMFSINST_DISCARD) -e CONFIG_USER_DISCARD_DISCARD \
		-a "discard dgram  udp wait   root /bin/discard" /etc/inetd.conf
	$(ROMFSINST_DISCARD) -e CONFIG_USER_DISCARD_DISCARD \
		-a "discard stream tcp nowait root /bin/discard" /etc/inetd.conf

	$(ROMFSINST) -e CONFIG_USER_DISCARD_INETD_ECHO /bin/$(EXEC1)
	$(ROMFSINST_ECHO) -e CONFIG_USER_DISCARD_INETD_ECHO \
		-a "echo	dgram  udp wait   root /bin/inetd-echo" /etc/inetd.conf
	$(ROMFSINST_ECHO) -e CONFIG_USER_DISCARD_INETD_ECHO \
		-a "echo	stream tcp nowait root /bin/inetd-echo" /etc/inetd.conf


package:

clean:
	-rm -f $(EXEC) $(EXEC1) *.elf *.gdb *.o

