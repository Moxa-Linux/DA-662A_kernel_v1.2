# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/backupuf_only
#

-include $(ARCH_CONFIG)

EXEC=backupuf
OBJS=backupuf.o

all:	$(TC_INSTALL_PREFIX)/bin/$(EXEC)

$(TC_INSTALL_PREFIX)/bin/$(EXEC): $(EXEC)
	cp -a $(EXEC) $(TC_INSTALL_PREFIX)/bin/$(EXEC).$(CONFIG_PRODUCT)

$(EXEC):$(OBJS)
	$(CC)  -o $@ $(OBJS)

romfs:	$(TC_INSTALL_PREFIX)/bin/$(EXEC)
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/$(EXEC).$(CONFIG_PRODUCT) /bin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)

package: #$(TC_INSTALL_PREFIX)/bin/$(EXEC)

clean:
	rm -fr $(EXEC) $(OBJS)
