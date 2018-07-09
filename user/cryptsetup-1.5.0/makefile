# History:
# Date		Author			Comment
# 09-01-2012	Lock Lin.		First created
# 09-13-2012	Jared Wu.		Rename to build the package in build_arm-linux or build_xscale-linux
#					Install the pre-requirement package, libgcrypt11-dev
#
# Pre-requirement:
#	apt-get -y install libgcrypt11-dev
#
# compile: 
#	make user/cryptsetup-1.5.0_only


PACKAGE_NAME = cryptsetup-1.5.0

all: build_$(CONFIGURE_TARGET)-linux/lib/.libs/libcryptsetup.so.4.3.0

build_$(CONFIGURE_TARGET)-linux/lib/.libs/libcryptsetup.so.4.3.0: build_$(CONFIGURE_TARGET)-linux/configured
	make -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/configured:
	# Install the pre-requried package.
	#apt-get -y install libgcrypt11-dev
	tar jxvf $(PACKAGE_NAME).tar.bz2
	mv $(PACKAGE_NAME) build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux;\
	unset CXX CXXCPP CXXCPP CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	CC=$(CC) \
	LD=$(LD)\
	CFLAGS="-I$(TC_INSTALL_PREFIX)/include" \
	LDFLAGS="-L$(TC_INSTALL_PREFIX)/lib -lgcrypt" \
	./configure \
	--host=arm-linux \
	--with-libgcrypt-prefix=$(TC_INSTALL_PREFIX) \
	--build=i686-pc-linux-gnu )
	touch build_$(CONFIGURE_TARGET)-linux/configured

romfs:
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/lib/.libs/libcryptsetup.so.4.3.0 /lib
	(cd $(ROMFSDIR)/lib; ln -sf libcryptsetup.so.4.3.0 libcryptsetup.so.4)
	chmod 777 build_$(CONFIGURE_TARGET)-linux/src/.libs/cryptsetup
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/.libs/cryptsetup /sbin/cryptsetup
	$(STRIP) $(ROMFSDIR)/sbin/cryptsetup $(ROMFSDIR)/lib/libcryptsetup.so.4.3.0

package:

clean distclean:
	/bin/rm -rf build_$(CONFIGURE_TARGET)-linux
