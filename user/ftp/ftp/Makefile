# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at install_big_endian or
#					install_little_endian
#	INSTALL_ROOT= $(ROOTDIR)/install_big_endian
#

EXEC = ftp
OBJS = cmds.o cmdtab.o domacro.o ftp.o glob.o main.o ruserpass.o

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $@ $(OBJS)

romfs:
	$(ROMFSINST) $(EXEC) /bin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.gdb *.elf *.o

