
include ../MCONFIG

E2IMAGE_PROG= e2image
SPROGS=		mke2fs badblocks tune2fs dumpe2fs $(E2IMAGE_PROG) fsck
USPROGS=	mklost+found

UPROGS=		chattr lsattr uuidgen

TUNE2FS_OBJS=	tune2fs.o util.o get_device_by_label.o
MKLPF_OBJS=	mklost+found.o
MKE2FS_OBJS=	mke2fs.o util.o get_device_by_label.o
CHATTR_OBJS=	chattr.o
LSATTR_OBJS=	lsattr.o
UUIDGEN_OBJS=	uuidgen.o
DUMPE2FS_OBJS=	dumpe2fs.o
BADBLOCKS_OBJS=	badblocks.o
E2IMAGE_OBJS=	e2image.o
FSCK_OBJS=	fsck.o get_device_by_label.o base_device.o fstype.o

LIBS= $(LIBEXT2FS) $(LIBCOM_ERR) 
DEPLIBS= $(LIBEXT2FS) $(LIBCOM_ERR) 

LIBS_E2P= $(LIBEXT2FS) $(LIBE2P) $(LIBCOM_ERR) 
DEPLIBS_E2P= $(LIBEXT2FS) $(LIBE2P) $(LIBCOM_ERR) 

all: $(SPROGS) $(UPROGS) $(USPROGS) $(SMANPAGES) $(UMANPAGES)

findsuper: findsuper.o
	$(CC) -o findsuper findsuper.o
	$(STRIP) -s $@

tune2fs: $(TUNE2FS_OBJS) $(DEPLIBS_E2P) $(DEPLIBUUID)
	$(CC) -o tune2fs $(TUNE2FS_OBJS) $(LIBS_E2P) $(LIBUUID)
	$(STRIP) -s $@

e2image: $(E2IMAGE_OBJS) $(DEPLIBS)
	$(CC) -o e2image $(E2IMAGE_OBJS) $(LIBS)
	$(STRIP) -s $@

base_device: base_device.c
	$(CC) $(CFLAGS) $(ALL_LDFLAGS) $< -DDEBUG -o $@
	$(STRIP) -s $@

mklost+found: $(MKLPF_OBJS)
	$(CC) -o mklost+found $(MKLPF_OBJS)
	$(STRIP) -s $@

mke2fs: $(MKE2FS_OBJS) $(DEPLIBS) $(LIBE2P) $(DEPLIBUUID)
	$(CC) -o mke2fs $(MKE2FS_OBJS) $(LIBS) $(LIBE2P) $(LIBUUID)
	$(STRIP) -s $@

resize2fs: resize2fs.o $(DEPLIBS)
	$(CC) -o resize2fs resize2fs.o $(LIBS)
	$(STRIP) -s $@

chattr: $(CHATTR_OBJS) $(DEPLIBS_E2P)
	$(CC) -o chattr $(CHATTR_OBJS) $(LIBS_E2P)
	$(STRIP) -s $@

lsattr: $(LSATTR_OBJS) $(DEPLIBS_E2P)
	$(CC) -o lsattr $(LSATTR_OBJS) $(LIBS_E2P)
	$(STRIP) -s $@

uuidgen: $(UUIDGEN_OBJS) $(DEPLIBUUID)
	$(CC) -o uuidgen $(UUIDGEN_OBJS) $(LIBUUID)
	$(STRIP) -s $@

dumpe2fs: $(DUMPE2FS_OBJS) $(DEPLIBS_E2P) $(DEPLIBUUID)
	$(CC) -o dumpe2fs $(DUMPE2FS_OBJS) $(LIBS_E2P) $(LIBUUID)
	$(STRIP) -s $@

fsck: $(FSCK_OBJS)
	$(CC) -o fsck $(FSCK_OBJS) $(LIBS)
	$(STRIP) -s $@

badblocks: $(BADBLOCKS_OBJS) $(DEPLIBS)
	$(CC) -o badblocks $(BADBLOCKS_OBJS) $(LIBS)
	$(STRIP) -s $@

romfs:
	$(ROMFSINST) -e CONFIG_USER_E2FSPROGS_MISC_BADBLOCKS /bin/badblocks
	$(ROMFSINST) -e CONFIG_USER_E2FSPROGS_MISC_CHATTR /bin/chattr
	$(ROMFSINST) -e CONFIG_USER_E2FSPROGS_MISC_DUMPE2FS /bin/dumpe2fs
	$(ROMFSINST) -e CONFIG_USER_E2FSPROGS_MISC_E2LABEL /bin/e2label
	$(ROMFSINST) -e CONFIG_USER_E2FSPROGS_MISC_FSCK /bin/fsck
	$(ROMFSINST) -e CONFIG_USER_E2FSPROGS_MISC_LSATTR /bin/lsattr
	$(ROMFSINST) -e CONFIG_USER_E2FSPROGS_MISC_MKE2FS /bin/mke2fs
	$(ROMFSINST) -e CONFIG_USER_E2FSPROGS_MISC_MKLOST_FOUND /bin/mklost+found
	$(ROMFSINST) -e CONFIG_USER_E2FSPROGS_MISC_TUNE2FS /bin/tune2fs
	$(ROMFSINST) -e CONFIG_USER_E2FSPROGS_MISC_UUIDGEN /bin/uuidgen

clean:
	$(RM) -f $(SPROGS) $(USPROGS) $(UPROGS) $(UMANPAGES) $(SMANPAGES) \
		base_device base_device.out mke2fs.static \
		\#* *.s *.gdb *.o *.a *~ core 

$(TUNE2FS_OBJS) $(MKLPF_OBJS) $(MKE2FS_OBJS) $(CHATTR_OBJS): ../MCONFIG
$(LSATTR_OBJS) $(UUIDGEN_OBJS) $(DUMPE2FS_OBJS) $(BADBLOCKS_OBJS): ../MCONFIG
$(E2IMAGE_OBJS) $(FSCK_OBJS): ../MCONFIG

