PACKAGE_NAME = libgcrypt-1.5.0

all:
ifeq ($(wildcard $(PACKAGE_NAME)/src/.libs/libgcrypt.so.11),)
	tar jxvf $(PACKAGE_NAME).tar.bz2; \
	( cd $(PACKAGE_NAME);\
	unset CXX CXXCPP CXXCPP CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	CC=$(CROSS)gcc \
	LD=$(CROSS)ld \
	CFLAGS="-I$(TC_INSTALL_PREFIX)/include" \
	LDFLAGS="-L$(TC_INSTALL_PREFIX)/lib -lgcrypt" \
	./configure \
	--with-gpg-error-prefix=$(TC_INSTALL_PREFIX) \
	--host=arm-linux \
	--build=i686-pc-linux-gnu )
	( cd $(PACKAGE_NAME); \
	make -f Makefile )
	cp -a $(PACKAGE_NAME)/src/.libs/libgcrypt.so.11.7.0 ../../vendors/Moxa/rnas1200/romfsdisk/lib
	cp -a $(PACKAGE_NAME)/src/.libs/libgcrypt.so.11 ../../vendors/Moxa/rnas1200/romfsdisk/lib 
endif

romfs:

clean distclean:
	/bin/rm -rf $(PACKAGE_NAME)
