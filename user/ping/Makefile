
EXEC = ping
OBJS = ping.o

all: $(EXEC)
	cp $(EXEC) $(TC_INSTALL_PREFIX)/bin/$(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $@ $(OBJS)
	$(STRIP) -s $(EXEC)
	chmod u+s $(EXEC)

romfs:
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/$(EXEC) /bin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)

package:

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o

