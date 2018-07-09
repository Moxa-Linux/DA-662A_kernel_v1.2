###############################################################################
#
# (c) 2011 Cinterion Wireless Modules GmbH <technical.feedback@cinterion.com>
#
# All Rights Reserved.
#
# Cinterion provides this source code under the GPL v2 License.
# The GPL v2 license is available at
#
# http://opensource.org/licenses/gpl-license.php
#
###############################################################################
EXEC = linmuxcfg

include ../../modules/LinuxMux/major_devnums

DEFS = -DFS_DEVNUM=$(fs_major_devnum) -DFS_DEVNAME='"$(fs_device_name)"' -I../../modules/LinuxMux

$(EXEC): $(EXEC).o
	$(CC) -o $(EXEC) $(EXEC).o

$(EXEC).o: $(EXEC).c
	$(CC) $(DEFS) -c $(EXEC).c

all: clean $(EXEC)

clean:
	rm -f $(EXEC).o $(EXEC)

romfs:
	$(STRIP) $(EXEC)
	$(ROMFSINST) $(EXEC) /usr/sbin

install: uninstall all
	cp $(EXEC) /usr/sbin

uninstall:
	rm -f /usr/sbin/$(EXEC)

