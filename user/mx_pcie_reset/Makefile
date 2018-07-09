# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/mx_pcie_reset_only

EXEC = mx_pcie_reset

all: $(EXEC)

$(EXEC): $(EXEC).o
	$(CC) -o $@ $(EXEC).o

romfs: $(EXEC)
	$(STRIP) -s $(EXEC)
	$(ROMFSINST) /bin/$(EXEC)

package:

clean:
	-rm -f $(EXEC) *.elf *.o *.gdb

