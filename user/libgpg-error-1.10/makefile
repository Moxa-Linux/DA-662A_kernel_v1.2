PACKAGE_NAME = libgpg-error-1.10

all:
ifeq ($(wildcard $(PACKAGE_NAME)/src/.libs/libgpg-error.so.0),)
	tar jxvf $(PACKAGE_NAME).tar.bz2; \
	( cd $(PACKAGE_NAME);\
	unset CXX CXXCPP CXXCPP CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	CC=$(CROSS)gcc \
	LD=$(CROSS)ld \
	CFLAGS="-I$(TC_INSTALL_PREFIX)/include" \
	LDFLAGS="-L$(TC_INSTALL_PREFIX)/lib -lgcrypt" \
	./configure \
	--host=arm-linux \
	--build=i686-pc-linux-gnu )
	( cd $(PACKAGE_NAME); \
	make -f Makefile )
	cp -a $(PACKAGE_NAME)/src/.libs/libgpg-error.so.0.8.0 ../../vendors/Moxa/rnas1200/romfsdisk/lib
	cp -a $(PACKAGE_NAME)/src/.libs/libgpg-error.so.0 ../../vendors/Moxa/rnas1200/romfsdisk/lib 
endif

romfs:

clean distclean:
	/bin/rm -rf $(PACKAGE_NAME)
