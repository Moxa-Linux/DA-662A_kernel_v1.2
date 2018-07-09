
#CROSS=xscale-none-linux-gnueabi
#CC=$(CROSS)-gcc
#LD=$(CROSS)-gcc
#STRIP=$(CROSS)-strip
#AR=$(CROSS)-ar
CFLAGS+= -I../mtd-utils/include -I../../lib/redboot_config
LDFLAGS+=
LIBS+=-lredboot

all: read_t3_flag set_t3_flag clear_t3_flag beeperon #install t3test.tgz

read_t3_flag:	read_t3_flag.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

set_t3_flag:	set_t3_flag.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

clear_t3_flag:	clear_t3_flag.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

beeperon:	beeperon.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f read_t3_flag set_t3_flag clear_t3_flag beeperon t3test.tgz
	rm -fr home/ etc/

romfs:
	$(STRIP) -s read_t3_flag set_t3_flag clear_t3_flag beeperon
	$(ROMFSINST) read_t3_flag /usr/sbin/
	$(ROMFSINST) set_t3_flag /usr/sbin/
	$(ROMFSINST) clear_t3_flag /usr/sbin/
	$(ROMFSINST) beeperon /usr/sbin/
	$(ROMFSINST) t3-test.sh /usr/sbin/
	$(ROMFSINST) blank_ready_led.sh /usr/sbin/
	$(ROMFSINST) t3test /etc/init.d/
	(cd $(ROMFSDIR)/etc/rc.d/rc3.d; \
	ln -s ../../init.d/t3test S99t3test; \
	cd $(ROMFSDIR)/etc/rc.d/rc0.d; \
	ln -s ../../init.d/t3test K01t3test; \
	cd $(ROMFSDIR)/etc/rc.d/rc1.d; \
	ln -s ../../init.d/t3test K01t3test; \
	cd $(ROMFSDIR)/etc/rc.d/rc6.d; \
	ln -s ../../init.d/t3test K01t3test; \
	)

install:	read_t3_flag set_t3_flag clear_t3_flag beeperon t3-test.sh blank_ready_led.sh t3test
	if [ ! -d ./home ]; then \
		mkdir ./home; \
	fi
	if [ ! -d ./home/t3test ]; then \
		mkdir ./home/t3test; \
	fi
	if [ ! -d ./etc ]; then \
		mkdir ./etc; \
	fi
	if [ ! -d ./etc/init.d ]; then \
		mkdir ./etc/init.d; \
	fi
	if [ ! -d ./etc/rc.d ]; then \
		mkdir ./etc/rc.d; \
	fi
	if [ ! -d ./etc/rc.d/rc3.d ]; then \
		mkdir ./etc/rc.d/rc3.d; \
	fi
	if [ ! -d ./etc/rc.d/rc6.d ]; then \
		mkdir ./etc/rc.d/rc6.d; \
	fi
	cp -f read_t3_flag ./home/t3test
	cp -f set_t3_flag ./home/t3test
	cp -f clear_t3_flag ./home/t3test
	cp -f beeperon ./home/t3test
	cp -f t3-test.sh ./home/t3test
	cp -f blank_ready_led.sh ./home/t3test
	cp -f t3test ./etc/init.d
	cd ./etc/rc.d/rc3.d && ln -s ../../init.d/t3test S99t3test
	cd ./etc/rc.d/rc6.d && ln -s ../../init.d/t3test K99t3test

t3test.tgz:	install
	tar cvfz t3test.tgz home/ etc/
