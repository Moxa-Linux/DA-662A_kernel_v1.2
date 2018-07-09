# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011    Aeluin Chen.    do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
# 	make user/e2fsprogs_only

ifeq ("$(CONFIGURE_TARGET)","xscale")
	ENDIAN_FLAG := -DWORDS_BIGENDIAN=1
endif

all: $(TC_INSTALL_PREFIX)/bin/mke2fs

$(TC_INSTALL_PREFIX)/bin/mke2fs: misc/mke2fs
	cp misc/mke2fs $(TC_INSTALL_PREFIX)/bin/
	cp e2fsck/e2fsck $(TC_INSTALL_PREFIX)/bin/

misc/mke2fs:
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	make -f Makefile CC="$(CROSS)gcc $(ENDIAN_FLAG)"

romfs:
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/mke2fs /bin/mke2fs
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/e2fsck /bin/e2fsck
	$(STRIP) -s $(ROMFSDIR)/bin/mke2fs $(ROMFSDIR)/bin/e2fsck

package:

clean distclean:
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	make -f Makefile clean
