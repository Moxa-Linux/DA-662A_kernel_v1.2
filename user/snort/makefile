
ifdef CONFIG_USER_MYSQL_CLIB
CFLAGS += -I$(TC_INSTALL_PREFIX)/include/mysql -DENABLE_MYSQL
LDLIBS := -L$(TC_INSTALL_PREFIX)/lib/mysql -lmysqlclient $(LDLIBS)
endif

all: build/Makefile
	$(MAKE) -C build MAKE="$(MAKE) -f Makefile"

build/Makefile: makefile
	rm -rf build
	mkdir build
	( \
		cd build; \
		CC="$(CC) $(CFLAGS)" LDFLAGS="$(LDFLAGS)" LIBS="$(LDLIBS)" \
			../configure --host=$(CROSS_COMPILE:-=) \
			; \
	)

romfs:
	$(ROMFSINST) build/src/snort /bin/snort
	$(ROMFSINST) etc/snort.snapgear /etc/default/snort.conf
	$(ROMFSINST) etc/classification.config /etc/default/classification.config
	$(ROMFSINST) etc/reference.config /etc/default/reference.config
	# Copy default rule set across
	[ -d $(ROMFSDIR)/etc/snort ] || mkdir $(ROMFSDIR)/etc/snort
	for i in rules/*.rules; do $(ROMFSINST) $$i /etc/snort/`basename $$i`; done
	# Local rules go elsewhere...
	[ -f $(ROMFSDIR)/etc/snort/local.rules ] && rm -f $(ROMFSDIR)/etc/snort/local.rules
	$(ROMFSINST) rules/local.rules /etc/default/local.rules

clean:
	rm -rf build

