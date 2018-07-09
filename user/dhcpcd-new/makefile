# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011    Aeluin Chen.    do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#	INSTALL_ROOT= $(ROOTDIR)/BERoot
# pre-requirments:
#	apt-get install -y automake1.7
#
# compile: 
#	make user/dhcpcd-new_only

all: $(TC_INSTALL_PREFIX)/sbin/dhcpcd


$(TC_INSTALL_PREFIX)/sbin/dhcpcd: build_$(CONFIGURE_TARGET)-linux/dhcpcd
	make -C build_$(CONFIGURE_TARGET)-linux install

build_$(CONFIGURE_TARGET)-linux/dhcpcd: build_$(CONFIGURE_TARGET)-linux/build
	unset CFLAGS CC CPPFLAGS LDFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/build: makefile
	autoreconf -i
	sed -e 's/CONFIG_DIR		"\/etc\/dhcpc"/CONFIG_DIR		"\/var\/run"/g'  pathnames.h > pathnames.h.new
	mv pathnames.h.new pathnames.h
	rm -rf build_$(CONFIGURE_TARGET)-linux
	mkdir -p build_$(CONFIGURE_TARGET)-linux
	( \
	cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CC CPP CPPFLAGS LDFLAGS CXXFLAGS && \
	CC=$(CC) \
	LD=$(LD) \
	CPP="$(CPP)" \
	STRIP=$(STRIP)\
	CFLAGS="-isystem $(TC_INSTALL_PREFIX)/include" \
	../configure \
	--build=i686 \
	--host="arm-unknown-linux" \
	--prefix=$(TC_INSTALL_PREFIX) \
	)
	touch build_$(CONFIGURE_TARGET)-linux/build

package:
	echo "Nothing to do"

romfs:
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/sbin/dhcpcd /sbin/dhcpcd
	$(STRIP) -s $(ROMFSDIR)/sbin/dhcpcd


clean distclean:
	rm -fr build_$(CONFIGURE_TARGET)-linux

