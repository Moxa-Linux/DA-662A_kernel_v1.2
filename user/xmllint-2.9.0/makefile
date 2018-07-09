# History:
# Date			Author    	Comment
# 07-02-2013	Simon Wang	First Built.
#
# compile:
# make user/libxml2-2.9.0_only

PACKAGE_NAME = libxml2-2.9.0

# Define Modules
MODULES =

# Define Library
LIBS =

# Define DAT files
DATS =

# Define Binary files
BIN_EXEC = xmllint
SBIN_EXEC =
EXEC = $(BIN_EXEC) $(SBIN_EXEC)


all: build_$(CONFIGURE_TARGET)-linux/$(EXEC)
	@echo "Build Complete!"

build_$(CONFIGURE_TARGET)-linux/$(EXEC): build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux xmllint

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf $(PACKAGE_NAME).tar.gz; \
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
	--disable-shared \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured

romfs:
	for i in $(BIN_EXEC); do \
		$(CROSS)strip build_$(CONFIGURE_TARGET)-linux/$$i; \
		$(ROMFSINST) -p 777 build_$(CONFIGURE_TARGET)-linux/$$i /bin/$$i; \
	done

clean:
	rm -rf build_$(CONFIGURE_TARGET)-linux

distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux

