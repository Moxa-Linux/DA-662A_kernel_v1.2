PACKAGE_NAME = bc-1.06

all: build_$(CONFIGURE_TARGET)-linux/lib/libbc.a
	make -C build_$(CONFIGURE_TARGET)-linux install


build_$(CONFIGURE_TARGET)-linux/lib/libbc.a: build_$(CONFIGURE_TARGET)-linux/configured
	make -C build_$(CONFIGURE_TARGET)-linux


build_$(CONFIGURE_TARGET)-linux/configured:
	tar xzvf $(PACKAGE_NAME).tar.gz
	mv $(PACKAGE_NAME) build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux;\
	unset CXX CXXCPP CXXCPP CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	CC=$(CC) \
	LD=$(LD)\
	./configure \
	--prefix=$(TC_INSTALL_PREFIX) \
	--host=$(CONFIGURE_TARGET)-linux \
	--build=i686-pc-linux-gnu  \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured

romfs:
	chmod 777 build_$(CONFIGURE_TARGET)-linux/bc/bc
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/bc/bc /usr/bin/bc
	$(STRIP) $(ROMFSDIR)/usr/bin/bc

clean distclean:
	/bin/rm -rf build_$(CONFIGURE_TARGET)-linux
