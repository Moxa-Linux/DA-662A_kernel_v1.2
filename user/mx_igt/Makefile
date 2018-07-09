# History:
# Date		Author			Comment
# 03-15-2012	Jared Wu.		Read IGT status (bit 7), reboot if IGT keep pressed over 10 sec.
#
# compile: 
#	make user/mx_igt_only

EXEC =  mx_igt

all: $(EXEC)

$(EXEC): $(EXEC).o
	$(CC) -o $@ $(EXEC).o

romfs:
	#$(STRIP) -s $(EXEC)
	#$(ROMFSINST) /bin/$(EXEC)

package: $(EXEC)
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/bin/
	cp -a $(EXEC) ipkg_$(CONFIGURE_TARGET)-linux/bin/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/bin/$(EXEC)
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

clean:
	-rm -rf $(EXEC) *.o ipkg_$(CONFIGURE_TARGET)-linux/bin/

