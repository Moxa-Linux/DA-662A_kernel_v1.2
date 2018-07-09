# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011    Aeluin Chen.    do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/iw_only

all: $(TC_INSTALL_PREFIX)/usr/sbin/iw

$(TC_INSTALL_PREFIX)/usr/sbin/iw:build_$(CONFIGURE_TARGET)-linux/iw
	DESTDIR=$(TC_INSTALL_PREFIX) \
	make -C build_$(CONFIGURE_TARGET)-linux install

build_$(CONFIGURE_TARGET)-linux/iw: build_$(CONFIGURE_TARGET)-linux/Makefile
	CC=$(CROSS)gcc \
	LIBS="-lnl" \
	CFLAGS="-I$(TC_INSTALL_PREFIX)/include" \
	LDFLAGS="-L$(TC_INSTALL_PREFIX)/lib" \
	make -C build_$(CONFIGURE_TARGET)-linux V=1

build_$(CONFIGURE_TARGET)-linux/Makefile: makefile
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf iw-3.1.tar.bz2
	mv iw-3.1 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	patch -p1 <../iw.patch; \
	cp -a ../version.c .; \
	)
	touch build_$(CONFIGURE_TARGET)-linux/build


romfs:
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/usr/sbin/iw /usr/sbin/iw
	$(STRIP) -s $(ROMFSDIR)/usr/sbin/iw

package: #$(TC_INSTALL_PREFIX)/usr/sbin/iw


clean distclean:
	rm -fr build_$(CONFIGURE_TARGET)-linux
	find -type l | xargs rm -rf;
