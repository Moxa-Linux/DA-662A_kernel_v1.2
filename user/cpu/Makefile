# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011    Aeluin Chen.    do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile:
#       make lib/cpu_only

EXEC = cpu
OBJS = cpu.o

all:  $(TC_INSTALL_PREFIX)/bin/$(EXEC)
	@echo "$(EXEC) has installed at $(TC_INSTALL_PREFIX)"

$(TC_INSTALL_PREFIX)/bin/$(EXEC): $(EXEC)
	cp $(EXEC) $(TC_INSTALL_PREFIX)/bin/
	@echo "Install $(EXEC) finish"

$(EXEC): $(OBJS)
	$(CC) -o $@ $(OBJS)

romfs:  $(TC_INSTALL_PREFIX)/bin/$(EXEC)
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/$(EXEC) /bin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)

package: 

clean distclean:
	-rm -f $(EXEC) *.elf *.o

