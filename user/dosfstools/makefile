# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 10-07-2009	Jared Wu.		Upgrade to new version v.3.0.6
# 12-29-2011    Aeluin Chen.    do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# Cross compile argument:
#	make user/dosfstools_only

all: $(TC_INSTALL_PREFIX)/bin/mkdosfs

$(TC_INSTALL_PREFIX)/bin/mkdosfs: build_$(CONFIGURE_TARGET)-linux/mkdosfs
	cp -a build_$(CONFIGURE_TARGET)-linux/mkdosfs $(TC_INSTALL_PREFIX)/bin/

build_$(CONFIGURE_TARGET)-linux/mkdosfs:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf dosfstools-3.0.6.tar.bz2
	sed -i 's/sys\/mount.h/linux\/fs.h/' dosfstools-3.0.6/src/mkdosfs.c
	mv dosfstools-3.0.6 build_$(CONFIGURE_TARGET)-linux
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET)-linux \
	CC="$(CROSS)gcc -DPATH_MAX=4096 -isystem $(TC_INSTALL_PREFIX)/include -lgcc_s" \
	mkdosfs
	#V=1

romfs:
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/mkdosfs /bin/mkdosfs
	$(STRIP) $(ROMFSDIR)/bin/mkdosfs

package:

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux
