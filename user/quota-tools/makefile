# History:
# Date    Author    Comment
# 06-11-2012   Simon Wang   First Built.
#
# compile:
# make user/quota-tools_only

PACKAGE_NAME = quota-4.00
ETC_FILE = quotagrpadmins quotatab warnquota.conf
BIN_EXEC = quota quotasync
SBIN_EXEC = convertquota edquota quot quotacheck quotaon quotastats repquota rpc.rquotad setquota warnquota xqmstats
EXEC = $(BIN_EXEC) $(SBIN_EXEC)

all: build_$(CONFIGURE_TARGET)-linux/$(EXEC)
	echo "Build Complete!"

build_$(CONFIGURE_TARGET)-linux/$(EXEC): build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf $(PACKAGE_NAME).tar.gz; \
	(mv quota-tools build_$(CONFIGURE_TARGET)-linux)
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CC CPP CFLAGS CPPFLAGS LD LDFLAGS STRIP && \
	CC=$(CROSS)gcc \
	CPP=$(CROSS)cpp \
	LD=$(CROSS)ld \
	STRIP=$(CROSS)strip \
	CFLAGS="-I$(TC_INSTALL_PREFIX)/include" \
	CPPFLAGS="-I$(TC_INSTALL_PREFIX)/include" \
	LDGLAGS="-L$(TC_INSTALL_PREFIX)/lib" \
	./configure \
	--host=$(CONFIGURE_TARGET)-linux \
	--prefix=/ \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured

romfs:
	for i in $(SBIN_EXEC); do \
		$(CROSS)strip build_$(CONFIGURE_TARGET)-linux/$$i; \
		$(ROMFSINST) -p 777 build_$(CONFIGURE_TARGET)-linux/$$i /sbin/$$i; \
	done
	for i in $(BIN_EXEC); do \
		$(CROSS)strip build_$(CONFIGURE_TARGET)-linux/$$i; \
		$(ROMFSINST) -p 777 build_$(CONFIGURE_TARGET)-linux/$$i /bin/$$i; \
	done
	for i in $(ETC_FILE); do \
		$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/$$i /etc/$$i; \
	done
	$(ROMFSINST) -s /sbin/quotaon /sbin/quotaoff


package: build_$(CONFIGURE_TARGET)-linux/$(EXEC)

clean:
	/bin/rm -rf build_$(CONFIGURE_TARGET)-linux \

