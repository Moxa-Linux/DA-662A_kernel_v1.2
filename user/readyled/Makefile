# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#

EXEC=readyled
OBJS=readyled.o

all: $(TC_INSTALL_PREFIX)/bin/$(EXEC)


$(TC_INSTALL_PREFIX)/bin/$(EXEC): $(EXEC)
	cp $(EXEC) $(TC_INSTALL_PREFIX)/bin/

$(EXEC):	$(OBJS)
	$(CC) -o $@ $(OBJS)

romfs:
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/$(EXEC) /bin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)

package:

clean:
	rm -fr $(EXEC) $(OBJS)
