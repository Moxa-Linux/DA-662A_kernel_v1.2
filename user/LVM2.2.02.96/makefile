PACKAGE_NAME = LVM2.2.02.96

all:
ifeq ($(wildcard $(PACKAGE_NAME)/tools/dmsetup),)
	tar xzvf $(PACKAGE_NAME).tgz; \
	cd $(PACKAGE_NAME);\
	unset CXX CXXCPP CXXCPP CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	CC=$(CROSS)gcc;export CC; ./configure --host=arm-linux --build=i686-pc-linux-gnu --with-clvmd --with-cluster=shared --with-lvm1=none
	cp configure.h $(PACKAGE_NAME)/lib/misc/configure.h
	cd $(PACKAGE_NAME);\
	unset CXX CXXCPP CXXCPP CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	make -f Makefile 
	cp -a $(PACKAGE_NAME)/libdm/ioctl/libdevmapper.so.1.02 ../../vendors/Moxa/rnas1200/romfsdisk/lib 
endif
romfs:
	chmod 777 $(PACKAGE_NAME)/tools/dmsetup
	$(ROMFSINST) $(PACKAGE_NAME)/tools/dmsetup /sbin/dmsetup
	$(STRIP) $(ROMFSDIR)/sbin/dmsetup

clean distclean:
	/bin/rm -rf $(PACKAGE_NAME)
