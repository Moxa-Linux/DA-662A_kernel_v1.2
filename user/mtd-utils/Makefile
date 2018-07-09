
# -*- sh -*-

# Jared --- begin
BUILDDIR := build_$(CROSS:-=)
DESTDIR=$(TC_INSTALL_PREFIX)
CC=$(CROSS)gcc
LD=$(CROSS)gcc
STRIP=$(CROSS)strip
WITHOUT_XATTR=1
WITHOUT_LZO=1
CFLAGS:=
#CPPFLAGS +=  -I./include -I$(BUILDDIR)/include -I./ubi-utils/include $(ZLIBCPPFLAGS) $(LZOCPPFLAGS)
# Jared, 07-23-2013, Add -I(TC_INSTALL_PREFIX) for supporting UBIFS
CPPFLAGS :=  -I./include -I$(BUILDDIR)/include -I./ubi-utils/include $(ZLIBCPPFLAGS) $(LZOCPPFLAGS)  -I$(TC_INSTALL_PREFIX)/include
LDFLAGS :=
LDLIBS :=
LZOLDLIBS :=
# Jared --- end

VERSION = 1.4.9


ifeq ($(WITHOUT_XATTR), 1)
  CPPFLAGS += -DWITHOUT_XATTR
endif
ifeq ($(WITHOUT_LZO), 1)
  CPPFLAGS += -DWITHOUT_LZO
else
  LZOLDLIBS = -llzo2
endif

TESTS = tests

MTD_BINS = \
	ftl_format flash_erase nanddump doc_loadbios \
	ftl_check mkfs.jffs2 flash_lock flash_unlock \
	flash_otp_info flash_otp_dump mtd_debug flashcp nandwrite nandtest \
	jffs2dump \
	nftldump nftl_format docfdisk \
	rfddump rfdformat \
	serve_image recv_image \
	sumtool jffs2reader
UBI_BINS = \
	ubiupdatevol ubimkvol ubirmvol ubicrc32 ubinfo ubiattach \
	ubidetach ubinize ubiformat ubirename mtdinfo ubirsvol

BINS = $(MTD_BINS)
# Jared, 07-23-2013, For supporting UBIFS
#BINS += mkfs.ubifs/mkfs.ubifs
#BINS += $(addprefix ubi-utils/,$(UBI_BINS))
SCRIPTS = flash_eraseall

TARGETS = $(BINS)
TARGETS += lib/libmtd.a
#TARGETS += ubi-utils/libubi.a

OBJDEPS = $(BUILDDIR)/include/version.h

include common.mk

CLEAN_FIND = find "$(BUILDDIR)/" -xdev '(' -name '*.[ao]' -o -name '.*.c.dep' ')'

clean::
ifneq ($(BUILDDIR)/.git,)
ifneq ($(BUILDDIR),.)
ifneq ($(BUILDDIR),$(CURDIR))
	rm -rf $(BUILDDIR)
endif
endif
endif
# findutils v4.1.x (RHEL 4) do not have '+' syntax
	@if test -d "$(BUILDDIR)/"; then \
		$(CLEAN_FIND) -exec rm -f {} + 2> /dev/null || \
		$(CLEAN_FIND) -exec rm -f {} \; ; \
	fi
	rm -f $(BUILDDIR)/include/version.h
	$(MAKE) -C $(TESTS) clean

romfs:
	##$(ROMFSINST) $(DESTDIR)/usr/sbin/flash_erase /bin/flash_erase
	##$(STRIP) -s $(ROMFSDIR)/bin/flash_erase
	##$(ROMFSINST) $(DESTDIR)/usr/sbin/flash_eraseall /bin/eraseall
	$(STRIP) -s $(BUILDDIR)/flash_erase
	$(ROMFSINST) $(BUILDDIR)/flash_erase     /bin/eraseall
	# Jared, 07-23-2013, For supporting UBIFS
	#$(STRIP) -s $(BUILDDIR)/ubi-utils/ubiupdatevol \
	#$(BUILDDIR)/ubi-utils/ubimkvol \
	#$(BUILDDIR)/ubi-utils/ubirmvol \
	#$(BUILDDIR)/ubi-utils/ubicrc32 \
	#$(BUILDDIR)/ubi-utils/ubinfo \
	#$(BUILDDIR)/ubi-utils/ubiattach \
	#$(BUILDDIR)/ubi-utils/ubidetach \
	#$(BUILDDIR)/ubi-utils/ubinize \
	#$(BUILDDIR)/ubi-utils/ubiformat \
	#$(BUILDDIR)/ubi-utils/ubirename \
	#$(BUILDDIR)/ubi-utils/mtdinfo \
	#$(BUILDDIR)/ubi-utils/ubirsvol
	#$(ROMFSINST) $(BUILDDIR)/ubi-utils/ubiupdatevol /bin/ubimkvol
	#$(ROMFSINST) $(BUILDDIR)/ubi-utils/ubimkvol /bin/ubimkvol
	#$(ROMFSINST) $(BUILDDIR)/ubi-utils/ubirmvol /bin/ubirmvol
	#$(ROMFSINST) $(BUILDDIR)/ubi-utils/ubicrc32 /bin/ubicrc32
	#$(ROMFSINST) $(BUILDDIR)/ubi-utils/ubinfo /bin/ubinfo
	#$(ROMFSINST) $(BUILDDIR)/ubi-utils/ubiattach /bin/ubiattach
	#$(ROMFSINST) $(BUILDDIR)/ubi-utils/ubidetach /bin/ubidetach
	#$(ROMFSINST) $(BUILDDIR)/ubi-utils/ubinize /bin/ubinize
	#$(ROMFSINST) $(BUILDDIR)/ubi-utils/ubiformat /bin/ubiformat
	#$(ROMFSINST) $(BUILDDIR)/ubi-utils/ubirename /bin/ubirename
	#$(ROMFSINST) $(BUILDDIR)/ubi-utils/mtdinfo /bin/mtdinfo
	#$(ROMFSINST) $(BUILDDIR)/ubi-utils/ubirsvol /bin/ubirsvol
	#$(STRIP) -s $(BUILDDIR)/flash_eraseall 
	#$(ROMFSINST) $(BUILDDIR)/flash_eraseall     /bin/flash_eraseall
	#$(ROMFSINST) -s /bin/flash_eraseall     /bin/eraseall
##	$(STRIP) -s flash_erase
##	$(ROMFSINST) -e $(BUILDDIR)/flash_erase     /bin/flash_erase
##	$(STRIP) -s $(BUILDDIR)/ftl_check
##	$(ROMFSINST) $(BUILDDIR)/ftl_check    /bin/ftl_check
##	$(STRIP) -s $(BUILDDIR)/ftl_format
##	$(ROMFSINST) $(BUILDDIR)/ftl_format   /bin/ftl_format
##	$(STRIP) -s $(BUILDDIR)/mkfs.jffs
##	$(ROMFSINST) $(BUILDDIR)/mkfs.jffs     /bin/mkfs.jffs
#	$(STRIP) -s $(BUILDDIR)/mkfs.jffs2
##	$(ROMFSINST) $(BUILDDIR)/bin/mkfs.jffs2    /bin/mkfs.jffs2
##	$(STRIP) -s $(BUILDDIR)/nftldump
##	$(ROMFSINST) $(BUILDDIR)/nftldump     /bin/nftldump
##	$(STRIP) -s $(BUILDDIR)/nftl_format
##	$(ROMFSINST) $(BUILDDIR)/nftl_format /bin/nftl_format
##	$(STRIP) -s $(BUILDDIR)/nanddump
##	$(ROMFSINST) $(BUILDDIR)/nanddump    /bin/nanddump
#	$(STRIP) -s $(BUILDDIR)/nandtest
##	$(ROMFSINST) $(BUILDDIR)/nandtest   /bin/nandtest
##	$(STRIP) -s $(BUILDDIR)/nandwrite
##	$(ROMFSINST) $(BUILDDIR)/nandwrite    /bin/nandwrite
#	$(STRIP) -s $(BUILDDIR)/doc_loadbios
##	$(ROMFSINST) $(BUILDDIR)/doc_loadbios /bin/doc_loadbios
#	$(STRIP) -s $(BUILDDIR)/doc_loadipl
##	$(ROMFSINST) $(BUILDDIR)/doc_ioadipl  /bin/doc_loadipl

package:


install:: $(addprefix $(BUILDDIR)/,${BINS}) ${SCRIPTS}
	mkdir -p ${DESTDIR}/${SBINDIR}
	install -m 0755 $^ ${DESTDIR}/${SBINDIR}/
	mkdir -p ${DESTDIR}/${MANDIR}/man1
	install -m 0644 mkfs.jffs2.1 ${DESTDIR}/${MANDIR}/man1/
	-gzip -9f ${DESTDIR}/${MANDIR}/man1/*.1

tests::
	$(MAKE) -C $(TESTS)

cscope:
	cscope -bR

$(BUILDDIR)/include/version.h: $(BUILDDIR)/include/version.h.tmp
	$(call BECHO,CHK)
	$(Q)cmp -s $@ $@.tmp && rm -f $@.tmp || mv $@.tmp $@
$(BUILDDIR)/include/version.h.tmp:
	${Q}mkdir -p $(dir $@)
	$(Q)echo '#define VERSION "$(VERSION)"' > $@

#
# Utils in top level
#
obj-mkfs.jffs2 = compr_rtime.o compr_zlib.o compr_lzo.o compr.o rbtree.o
LDFLAGS_mkfs.jffs2 = $(ZLIBLDFLAGS) $(LZOLDFLAGS)
LDLIBS_mkfs.jffs2  = -lz $(LZOLDLIBS)

LDFLAGS_jffs2reader = $(ZLIBLDFLAGS) $(LZOLDFLAGS)
LDLIBS_jffs2reader  = -lz $(LZOLDLIBS)

$(foreach v,$(MTD_BINS),$(eval $(call mkdep,,$(v))))

#
# Common libmtd
#
obj-libmtd.a = libmtd.o libmtd_legacy.o libcrc32.o libfec.o
$(call _mkdep,lib/,libmtd.a)

#
# Utils in mkfs.ubifs subdir
#
obj-mkfs.ubifs = crc16.o lpt.o compr.o devtable.o \
	hashtable/hashtable.o hashtable/hashtable_itr.o
LDLIBS_mkfs.ubifs = -lz -llzo2 -lm -luuid
$(call mkdep,mkfs.ubifs/,mkfs.ubifs,,ubi-utils/libubi.a)

#
# Utils in ubi-utils/ subdir
#
obj-libiniparser.a = libiniparser.o dictionary.o
obj-libscan.a      = libscan.o
obj-libubi.a       = libubi.o
obj-libubigen.a    = libubigen.o

obj-mtdinfo   = libubigen.a
obj-ubinize   = libubigen.a libiniparser.a
obj-ubiformat = libubigen.a libscan.a

$(foreach v,libubi.a libubigen.a libiniparser.a libscan.a,$(eval $(call _mkdep,ubi-utils/,$(v))))
$(foreach v,$(UBI_BINS),$(eval $(call mkdep,ubi-utils/,$(v),libubi.a ubiutils-common.o)))
