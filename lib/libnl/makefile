# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/libnl_only

all: build_$(CONFIGURE_TARGET)-linux/lib/libnl.so.1.1
	make -C build_$(CONFIGURE_TARGET)-linux install

build_$(CONFIGURE_TARGET)-linux/lib/libnl.so.1.1: build_$(CONFIGURE_TARGET)-linux/Makefile
	make -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/Makefile: makefile
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf libnl-1.1.tar.gz
	mv libnl-1.1 build_$(CONFIGURE_TARGET)-linux
	( \
	cd build_$(CONFIGURE_TARGET)-linux; \
	patch -p1 <../ulimits.patch; \
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	CC=$(CROSS)gcc \
	LD=$(CROSS)ld \
	AR=$(CROSS)ar \
	./configure \
	--build=i686-linux \
	--host=$(CONFIGURE_TARGET) \
	--prefix=$(TC_INSTALL_PREFIX) \
	--enable-shared \
	--enable-static ; \
	)
	touch build_$(CONFIGURE_TARGET)-linux/build

	#CFLAGS="-fpic -DULONG_MAX=~0UL -DUINT_MAX=~0UI" \

romfs: $(TC_INSTALL_PREFIX)/lib/libnl.so.1.1
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/lib/libnl.so.1.1 /lib/libnl.so.1.1
	$(STRIP) -s $(TC_INSTALL_PREFIX)/lib/libnl.so.1.1
	(cd $(ROMFSDIR)/lib; ln -sf libnl.so.1.1 libnl.so.1; ln -sf libnl.so.1.1 libnl.so)

package:


clean distclean:
	rm -fr build_$(CONFIGURE_TARGET)-linux
