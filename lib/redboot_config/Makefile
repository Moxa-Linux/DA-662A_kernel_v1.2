# Cross Compiled: make CROSS=xscale-linux-


CFLAGS += -DDEBUG -I../../user/mtd-utils/include
LDFLAGS = -lredboot -L.

all: $(TC_INSTALL_PREFIX)/lib/libredboot.a rbctest setswid rbcdump rbcrm rbcswlock setmpflag setburningflag getserialno
	$(STRIP) rbctest rbcdump rbcrm rbcswlock setburningflag getserialno

getserialno: getserialno.c
	$(CC) getserialno.c $(CFLAGS) $(LDFLAGS) -o getserialno
	$(STRIP) getserialno

rbctest: rbctest.c
	$(CC) rbctest.c $(CFLAGS) $(LDFLAGS) -o rbctest
	$(STRIP) rbctest

setswid: setswid.c
	$(CC) setswid.c $(CFLAGS) $(LDFLAGS) -o setswid
	$(STRIP) setswid

rbcdump: rbcdump.c
	$(CC) rbcdump.c $(CFLAGS) $(LDFLAGS) -o rbcdump
	$(STRIP) rbcdump

rbcrm: rbcrm.c
	$(CC) rbcrm.c $(CFLAGS) $(LDFLAGS) -o rbcrm
	$(STRIP) rbcrm

setburningflag: setburningflag.c
	$(CC) setburningflag.c $(CFLAGS) $(LDFLAGS) -o setburningflag
	$(STRIP) setburningflag

rbcswlock: rbcswlock.c
	$(CC) rbcswlock.c $(CFLAGS) $(LDFLAGS) -o rbcswlock
	$(STRIP) rbcswlock

setmpflag: setmpflag.c
	$(CC) setmpflag.c $(CFLAGS) $(LDFLAGS) -o setmpflag
	$(STRIP) setmpflag

$(TC_INSTALL_PREFIX)/lib/libredboot.a: libredboot.a
	cp libredboot.a $(TC_INSTALL_PREFIX)/lib/
	cp redboot_config.h $(TC_INSTALL_PREFIX)/include
	cp swid.h $(TC_INSTALL_PREFIX)/include

libredboot.a: redboot_config.c redboot_config.h
	$(CC) redboot_config.c -c $(CFLAGS)
	$(AR) crv libredboot.a redboot_config.o

romfs: 
	# Do nothing

package:
	echo "Nothing to do"

clean:
	$(RM) rbcswlock rbctest rbcdump rbcrm redboot_config.o setswid setmpflag setburningflag getserialno
	$(RM) libredboot.a *.o
