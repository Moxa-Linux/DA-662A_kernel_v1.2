
EXEC1 = vplay
EXEC2 = vrec
EXEC3 = mixer

OBJS = vplay.o
OBJS3 = mixer.o

all: $(TC_INSTALL_PREFIX)/bin/$(EXEC1) $(TC_INSTALL_PREFIX)/bin/$(EXEC3)

$(TC_INSTALL_PREFIX)/bin/$(EXEC1): $(EXEC1)
	cp -a $(EXEC1) $(TC_INSTALL_PREFIX)/bin/$(EXEC1)

$(TC_INSTALL_PREFIX)/bin/$(EXEC3): $(EXEC3)
	cp -a $(EXEC3) $(TC_INSTALL_PREFIX)/bin/$(EXEC3)

$(EXEC1): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

$(EXEC2): $(EXEC1)
	ln -sf $(EXEC1) $(EXEC2)

$(EXEC3): $(OBJS3)
	$(CC) $(LDFLAGS) -o $@ $(OBJS3) $(LDLIBS)

romfs: #$(TC_INSTALL_PREFIX)/bin/$(EXEC1) $(TC_INSTALL_PREFIX)/bin/$(EXEC3)
	$(ROMFSINST) -e CONFIG_USER_VPLAY_VPLAY $(TC_INSTALL_PREFIX)/bin/$(EXEC1) /bin/$(EXEC1)
	$(ROMFSINST) -e CONFIG_USER_VPLAY_VREC -s /bin/$(EXEC1) /bin/$(EXEC2) 
	$(ROMFSINST) -e CONFIG_USER_VPLAY_MIXER $(TC_INSTALL_PREFIX)/bin/$(EXEC3) /bin/$(EXEC3)

clean:
	-rm -f $(EXEC1) $(EXEC2) $(EXEC3) *.elf *.gdb *.o

package:
	echo "Nothing to do"
