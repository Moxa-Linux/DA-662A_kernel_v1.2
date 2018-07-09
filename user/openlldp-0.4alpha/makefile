# History:
# Date			Author    	Comment
# 07-02-2013	Simon Wang	First Built.
#
# compile:
# make user/samba-3.5.6_only

PACKAGE_NAME = openlldp-0.4alpha

# Define Modules
MODULES =

# Define Library
LIBS =

# Define DAT files
DATS =

# Define Binary files
BIN_EXEC = lldpneighbors
SBIN_EXEC = lldpd
EXEC = $(BIN_EXEC) $(SBIN_EXEC)


all: build_$(CONFIGURE_TARGET)-linux/src/$(EXEC)
	@echo "Build Complete!"

build_$(CONFIGURE_TARGET)-linux/src/$(EXEC): build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf $(PACKAGE_NAME).tar.bz2; \
	(mv $(PACKAGE_NAME) build_$(CONFIGURE_TARGET)-linux)
	(cd build_$(CONFIGURE_TARGET)-linux; \
	unset CC CPP CFLAGS CPPFLAGS LD LDFLAGS STRIP && \
	CC=$(CROSS)gcc \
	CPP=$(CROSS)cpp \
	LD=$(CROSS)ld \
	AR=$(CROSS)ar \
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
		$(CROSS)strip build_$(CONFIGURE_TARGET)-linux/src/$$i; \
		$(ROMFSINST) -p 777 build_$(CONFIGURE_TARGET)-linux/src/$$i /sbin/$$i; \
	done
	for i in $(BIN_EXEC); do \
		$(CROSS)strip build_$(CONFIGURE_TARGET)-linux/src/$$i; \
		$(ROMFSINST) -p 777 build_$(CONFIGURE_TARGET)-linux/src/$$i /bin/$$i; \
	done

clean:
	rm -rf build_$(CONFIGURE_TARGET)-linux

distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux

