
EXEC = mail_ip
OBJS = mail_ip.o resolv.o smtpmail.o

all: $(TC_INSTALL_PREFIX)/bin/$(EXEC)
	@echo "$(TC_INSTALL_PREFIX)/bin/$(EXEC) exist. Nothing to do"

$(TC_INSTALL_PREFIX)/bin/$(EXEC): $(EXEC)
	cp $(EXEC) $(TC_INSTALL_PREFIX)/bin/

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

romfs:
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/$(EXEC) /bin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o

