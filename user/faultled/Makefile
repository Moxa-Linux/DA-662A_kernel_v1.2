# History:
# Date		Author			Comment
# 2015-06-17	Victor Yu.		initialize it
#

EXEC=faultled
OBJS=faultled.o

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
