# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011    Aeluin Chen.    do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile:
#       make user/cron_only					
#

EXEC = cron


all: $(TC_INSTALL_PREFIX)/bin/$(EXEC)
	@echo "$(EXEC) has installed at $(TC_INSTALL_PREFIX)"

$(TC_INSTALL_PREFIX)/bin/$(EXEC): $(EXEC)
	cp $(EXEC) $(TC_INSTALL_PREFIX)/bin/$(EXEC)
	@echo "Install $(EXEC) finish"

$(EXEC): cron.o cron-parent.o bitstring.h
	$(CC) -o $@ cron.o cron-parent.o

romfs: $(TC_INSTALL_PREFIX)/bin/$(EXEC)
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/$(EXEC) /bin/$(EXEC)
	$(STRIP) $(ROMFSDIR)/bin/$(EXEC)

package:

clean distclean:
	-rm -f $(EXEC) *.elf *.gdb *.o

