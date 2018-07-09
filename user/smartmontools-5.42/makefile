# History:
# Date		Author			Comment
# 09-01-2012	Simon Wang.		First created
# 09-13-2012	Jared Wu.		Untar the source, auto-configure and build it
#
# compile: 
#	make user/smartmontools-5.42_only


PACKAGE_NAME = smartmontools-5.42

all: build_$(CONFIGURE_TARGET)-linux/bin/smartctl
	echo "Nothing to do"

build_$(CONFIGURE_TARGET)-linux/bin/smartctl: build_$(CONFIGURE_TARGET)-linux/configured
	make -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/configured:
	tar jxvf $(PACKAGE_NAME).tar.bz2
	mv $(PACKAGE_NAME) build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux;\
	unset CFLAGS CPPFLAGS CXXFLAGS && \
	CC=$(CC) \
	LD=$(LD)\
	./configure \
	--host=arm-linux \
	--build=i686-pc-linux-gnu )
	touch build_$(CONFIGURE_TARGET)-linux/configured

romfs:
	$(ROMFSINST) -e CONFIG_USER_SMART build_$(CONFIGURE_TARGET)-linux/smartctl /bin/smartctl
	$(STRIP) $(ROMFSDIR)/bin/smartctl

package:

clean distclean:
	/bin/rm -rf build_$(CONFIGURE_TARGET)-linux
