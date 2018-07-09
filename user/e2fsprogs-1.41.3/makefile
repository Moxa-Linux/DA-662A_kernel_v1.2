# History:
# Date		Author			Comment
# 07-05-2012	Lock Lin		First create for RNAS-1200
# 10-05-2012	Jared Wu		Support to build for big-endian platform
#
# compile: 
#	make user/e2fsprogs-1.41.3_only

PACKAGE_NAME = e2fsprogs-1.41.3

all: build_$(CONFIGURE_TARGET)-linux/misc/mke2fs
	make -C build_$(CONFIGURE_TARGET)-linux install


build_$(CONFIGURE_TARGET)-linux/misc/mke2fs: build_$(CONFIGURE_TARGET)-linux/configured
	make -C build_$(CONFIGURE_TARGET)-linux


build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf $(PACKAGE_NAME).tar.gz
	mv src build_$(CONFIGURE_TARGET)-linux
	cp -a badblocks.c build_$(CONFIGURE_TARGET)-linux/misc
	( cd build_$(CONFIGURE_TARGET)-linux; \
	CC="$(CC)" \
	LD="$(LD)" \
	./configure \
	--host=$(CONFIGURE_TARGET)-linux \
	--build=i686-pc-linux-gnu  \
	--prefix="$(TC_INSTALL_PREFIX)" \
	)
	sed -i 's/^#define _XOPEN_SOURCE/\/\/#define _XOPEN_SOURCE/' build_$(CONFIGURE_TARGET)-linux/misc/mke2fs.c
	touch build_$(CONFIGURE_TARGET)-linux/configured


romfs:
	chmod 777 build_$(CONFIGURE_TARGET)-linux/misc/mke2fs
	chmod 777 build_$(CONFIGURE_TARGET)-linux/e2fsck/e2fsck
	chmod 777 build_$(CONFIGURE_TARGET)-linux/misc/badblocks
	chmod 777 build_$(CONFIGURE_TARGET)-linux/debugfs/debugfs
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/misc/mke2fs /bin/mke2fs
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/e2fsck/e2fsck /bin/e2fsck
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/misc/badblocks /bin/badblocks
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/debugfs/debugfs /bin/debugfs
	$(STRIP) -s $(ROMFSDIR)/bin/mke2fs $(ROMFSDIR)/bin/e2fsck $(ROMFSDIR)/bin/badblocks $(ROMFSDIR)/bin/debugfs


package:


clean distclean:
	-rm -rf build_$(CONFIGURE_TARGET)-linux
