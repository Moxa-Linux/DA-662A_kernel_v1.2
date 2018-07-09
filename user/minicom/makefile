# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/minicom_only


all: build_$(CONFIGURE_TARGET)-linux/src/minicom

build_$(CONFIGURE_TARGET)-linux/src/minicom:build_$(CONFIGURE_TARGET)-linux/configured
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET)-linux 

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux;
	tar xzvf minicom-2.4.tar.gz
	mv minicom-2.4 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	CC=$(CROSS)gcc \
	CPP=$(CROSS)cpp \
	LD=$(CROSS)ld \
	CFLAGS="-liconv -I$(TC_INSTALL_PREFIX)/include/ncurses"	\
	./configure \
	--build=i686-linux \
	--host=arm \
	--sysconfdir="/etc/" \
	--enable-dfl-baud=115200 \
	--enable-dfl-port="/dev/ttyM0" \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured

romfs:build_$(CONFIGURE_TARGET)-linux/src/minicom
	#$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/src/minicom
	#$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/minicom /bin/minicom
	#touch $(ROMFSDIR)/etc/minirc.dfl

package:build_$(CONFIGURE_TARGET)-linux/src/minicom
	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/src/minicom
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/etc
	touch ipkg_$(CONFIGURE_TARGET)-linux/etc/minirc.dfl
	cp -a build_$(CONFIGURE_TARGET)-linux/src/minicom ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf ipkg_src_root_$(CONFIGURE_TARGET)-linux
