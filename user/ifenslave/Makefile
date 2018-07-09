# History:
# Date		Author			Comment
# 06-17-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011    Aeluin Chen.    do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/ifenslave_only

EXEC=ifenslave

all: $(TC_INSTALL_PREFIX)/sbin/$(EXEC)

$(TC_INSTALL_PREFIX)/sbin/$(EXEC): $(EXEC)
	cp $(EXEC) $(TC_INSTALL_PREFIX)/sbin/$(EXEC)

$(EXEC): $(EXEC).c
	$(CC) -o $@ $(EXEC).c

romfs: $(TC_INSTALL_PREFIX)/sbin/$(EXEC)
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/sbin/$(EXEC) /sbin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/sbin/$(EXEC)

package: $(TC_INSTALL_PREFIX)/sbin/$(EXEC)

clean:
	rm -fr $(EXEC)
