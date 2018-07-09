PACKAGE_NAME = httptunnel
EXEC = htc hts

all: build_$(CONFIGURE_TARGET)-linux/$(EXEC)
	@echo "Build Complete!"

build_$(CONFIGURE_TARGET)-linux/$(EXEC): build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

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
	--prefix= \
	--exec-prefix=/bin \
	--includedir=$(ROOTDIR)/$(LINUXDIR)/include	\
	--bindir=/bin \
	--datadir=/etc \
	--host=$(CONFIGURE_TARGET) \
	--cache-file=config.cache \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured

romfs:
	$(ROMFSINST) -e CONFIG_USER_HTTPTUNNEL_CLIENT build_$(CONFIGURE_TARGET)-linux/htc /bin/htc
	$(ROMFSINST) -e CONFIG_USER_HTTPTUNNEL_SERVER build_$(CONFIGURE_TARGET)-linux/hts /bin/hts
	$(ROMFSINST) -e build_$(CONFIGURE_TARGET)-linux/hts /bin/hts

clean:
	rm -rf build_$(CONFIGURE_TARGET)-linux

distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux

