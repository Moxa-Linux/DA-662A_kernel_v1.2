# History:
# Date		Author			Comment
# 05-24-2012	Jared Wu.		Watchdog daemon for RNAS-1200
#
# Compile:
#	make user/r1200_watchdog_only

EXEC=watchdog
OBJS=watchdog.o
all:	$(TC_INSTALL_PREFIX)/bin/$(EXEC)

CLAGS += -I.

$(TC_INSTALL_PREFIX)/bin/$(EXEC): $(EXEC)
	cp $(EXEC) $(TC_INSTALL_PREFIX)/bin/

$(EXEC):	$(OBJS)
	$(CC) -o $@ $(OBJS)

romfs:
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/$(EXEC) /bin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)

package:

clean:
	rm -fr $(EXEC) $(OBJS)
