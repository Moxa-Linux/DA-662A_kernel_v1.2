#
#	Makefile -- Build instructions for Moxa ART CPU
#
MODEL_SN = 6622
MODEL_ID = DA662A

KERNELZ  = $(IMAGEDIR)/zImage
JFFS2DISK = $(IMAGEDIR)/jffs2disk
USRDISK = $(IMAGEDIR)/UserDisk
HOSTDIR = $(ROOTDIR)/host
MKHFMDIR = $(HOSTDIR)/mkhfm
MKHFM = $(MKHFMDIR)/mkhfm
HFMFILE = $(IMAGEDIR)/firmware_$(MODEL_SN).hfm
HFMFILE_BOOT =  $(IMAGEDIR)/firmware_boot_$(MODEL_SN).hfm
HFMFILE_KERN =  $(IMAGEDIR)/firmware_kern_$(MODEL_SN).hfm
HFMFILE_ROOT =  $(IMAGEDIR)/firmware_root_$(MODEL_SN).hfm
#for new format firmware file(.hfm)
MKROMDIR = $(HOSTDIR)/mkrom
MKROM = $(MKROMDIR)/mkrom
ROMFILE = $(IMAGEDIR)/romfile_$(MODEL_SN)
ROMCONFFILE = $(MKROMDIR)/confall_$(MODEL_SN)
HFMCONFFILE_ALL = $(MKROMDIR)/confhfm_all_$(MODEL_SN)
HFMCONFFILE_KERNEL = $(MKROMDIR)/confhfm_kernel_$(MODEL_SN)
HFMCONFFILE_ROOT = $(MKROMDIR)/confhfm_root_$(MODEL_SN)
HFMCONFFILE_USRDISK = $(MKROMDIR)/confhfm_usrdisk_$(MODEL_SN)
KERNEL_VER= `cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`

NOWDIR = `pwd`
NOW=$(shell date +%y%m%d%H)

.PHONY: clean romfs image

clean:
	-rm -rf $(HFMFILE) \
	$(HFMFILE_KERN) \
	$(ROMFILE) \
	$(ROMCONFFILE) \
	$(KERNELZ) \
	$(JFFS2DISK) \
	$(USRDISK) \
	$(HFMCONFFILE) \
	$(HFMCONFFILE_USRDISK) \
	$(HFMCONFFILE_KERNEL)

customized:
	if [ -f $(VERSIONPKG)/customized_romfs.sh ]; then \
		$(VERSIONPKG)/customized_romfs.sh; \
	fi

romfs:
	echo "Untar the pre-built romfs."
	#tar xjvf romfsdisk.tar.bz2 -C $(ROMFSDIR)/
	cp -a romfsdisk/* $(ROMFSDIR)/
	chmod og-r $(ROMFSDIR)/etc/ssh/ssh*
	mkdir -p $(ROMFSDIR)/etc/network/if-down.d
	mkdir -p $(ROMFSDIR)/etc/network/if-post-down.d
	mkdir -p $(ROMFSDIR)/etc/network/if-pre-up.d
	mkdir -p $(ROMFSDIR)/etc/network/if-up.d
	(cd $(ROMFSDIR); ln -sf home/root root)
	#mkdir -p $(ROMFSDIR)/dev
	#mkfifo -m 0644 $(ROMFSDIR)/dev/initctl
	#mknod -m 0644 $(ROMFSDIR)/dev/console c 4 64 
	#mknod -m 0644 $(ROMFSDIR)/dev/mtdblock0 b 31 0 
	#mknod -m 0644 $(ROMFSDIR)/dev/mtdblock1 b 31 1
	#mknod -m 0644 $(ROMFSDIR)/dev/mtdblock2 b 31 2
	#mknod -m 0644 $(ROMFSDIR)/dev/mtdblock3 b 31 3
	#mknod -m 0644 $(ROMFSDIR)/dev/mtdblock4 b 31 4
	#mknod -m 0644 $(ROMFSDIR)/dev/nfs b 0 255
	#mknod -m 0644 $(ROMFSDIR)/dev/ram0 b 1 0
	#mknod -m 0644 $(ROMFSDIR)/dev/ram15 b 1 15
	echo "Start to copy file the root file system."
	$(ROMFSINST) ../../Generic/romfs/etc/services /etc/services
	#chmod og+w $(ROMFSDIR)/tmp

image: customized
	#@echo "Create the modules.dep files."
	#depmod -ae -E $(ROOTDIR)/$(LINUXDIR)/Module.symvers -b $(ROMFSDIR) -v $(KERNEL_VER)
	@echo "Make image at $(IMAGEDIR)."
	[ -d $(IMAGEDIR) ] || mkdir -p $(IMAGEDIR)
	[ -f "$(USRDISK)" ] || touch $(USRDISK)
	cp -a $(ROOTDIR)/$(LINUXDIR)/arch/arm/boot/zImage $(KERNELZ)
	@echo "Make root file system file system."
	mkfs.jffs2 -l -p -e 0x20000 -d $(ROMFSDIR)/ -o $(JFFS2DISK)
	@echo "Make the kernel released firmware file(new format .hfm)."
	echo "output $(HFMFILE_KERN)" > $(HFMCONFFILE_KERNEL)
	echo "apid $(AP_ID)" >> $(HFMCONFFILE_KERNEL)
	echo "modelname $(MODEL_ID)" >> $(HFMCONFFILE_KERNEL)
	echo "input $(KERNELZ) kernel $(FULLVERSIONPKG) 1 0x44100000 0x44400000 0x00300000 0x00000000" >> $(HFMCONFFILE_KERNEL)
	$(MKHFM) $(HFMCONFFILE_KERNEL)
	@echo "Make the root disk released firmware file(new format .hfm)."
	echo "output $(HFMFILE_ROOT)" > $(HFMCONFFILE_ROOT)
	echo "apid $(AP_ID)" >> $(HFMCONFFILE_ROOT)
	echo "modelname $(MODEL_ID)" >> $(HFMCONFFILE_ROOT)
	echo "input $(JFFS2DISK) root-file-system $(FULLVERSIONPKG) 2 0x44400000 0x45000000 0x00C00000 0x00000000" >> $(HFMCONFFILE_ROOT)
	$(MKHFM) $(HFMCONFFILE_ROOT)
	@echo "Make the released firmware file(new format .hfm)."
	echo "output $(HFMFILE)" > $(HFMCONFFILE_ALL)
	echo "apid $(AP_ID)" >> $(HFMCONFFILE_ALL)
	echo "modelname $(MODEL_ID)" >> $(HFMCONFFILE_ALL)
	echo "input $(KERNELZ) kernel $(FULLVERSIONPKG) 1 0x44100000 0x44400000 0x00300000 0x00000000" >> $(HFMCONFFILE_ALL)
	echo "input $(JFFS2DISK) root-file-system $(FULLVERSIONPKG) 2 0x44400000 0x45000000 0x00C00000 0x00000000" >> $(HFMCONFFILE_ALL)
	# Customize the root file system
	if [ -f $(VERSIONPKG)/customized_$(HFMCONFFILE_ALL).sh ]; then \
		$(VERSIONPKG)/customized_$(HFMCONFFILE_ALL).sh; \
	fi
	$(MKHFM) $(HFMCONFFILE_ALL)
	@echo "Make manufacturing firmware ROM file."
	echo "output $(ROMFILE)" > $(ROMCONFFILE)
	echo "input $(NOWDIR)/bootloader/bootloader-$(MODEL_SN) 0x100000" >> $(ROMCONFFILE)
	echo "input $(KERNELZ) 0x300000" >> $(ROMCONFFILE)
	echo "input $(JFFS2DISK) 0x0C00000" >> $(ROMCONFFILE)
	# Customize the $(ROMCONFFILE)
	if [ -f $(VERSIONPKG)/customized_confromall_$(MODEL_SN).sh ]; then \
		$(VERSIONPKG)/customized_confromall_$(MODEL_SN).sh; \
	fi
	$(MKROM) $(ROMCONFFILE) -c2
	@echo "Copy all files to the $(TFTPDIR) directory."
	[ -d "$(TFTPDIR)/$(MODEL_ID)_$(VERSIONPKG)_Build_$(NOW)/$(MODEL_SN)" ] || mkdir -p "$(TFTPDIR)/$(MODEL_ID)_$(VERSIONPKG)_Build_$(NOW)/$(MODEL_SN)"
	if [ -d $(TFTPDIR) ]; then \
		cp $(KERNELZ) $(TFTPDIR)/$(MODEL_ID)_$(VERSIONPKG)_Build_$(NOW)/$(CONFIG_PRODUCT)-kernel-$(VERSIONPKG).bin 2> /dev/null; \
		cp $(JFFS2DISK) $(TFTPDIR)/$(MODEL_ID)_$(VERSIONPKG)_Build_$(NOW)/$(CONFIG_PRODUCT)-rootdisk-$(VERSIONPKG).dsk 2> /dev/null; \
		mv $(HFMFILE_BOOT) $(TFTPDIR)/$(MODEL_ID)_$(VERSIONPKG)_Build_$(NOW)/$(MODEL_SN)/FWR_$(MODEL_ID)_boot_V$(VERSIONPKG)_Build_$(NOW).hfm 2> /dev/null; \
		mv $(HFMFILE) $(TFTPDIR)/$(MODEL_ID)_$(VERSIONPKG)_Build_$(NOW)/$(MODEL_SN)/FWR_$(MODEL_ID)_V$(VERSIONPKG)_Build_$(NOW).hfm 2> /dev/null; \
		mv $(HFMFILE_KERN) $(TFTPDIR)/$(MODEL_ID)_$(VERSIONPKG)_Build_$(NOW)/$(MODEL_SN)/FWR_$(MODEL_ID)_kernel_V$(VERSIONPKG)_Build_$(NOW).hfm 2> /dev/null; \
		mv $(HFMFILE_ROOT) $(TFTPDIR)/$(MODEL_ID)_$(VERSIONPKG)_Build_$(NOW)/$(MODEL_SN)/FWR_$(MODEL_ID)_root_V$(VERSIONPKG)_Build_$(NOW).hfm 2> /dev/null; \
		mv "$(ROMFILE)1" "$(TFTPDIR)/$(MODEL_ID)_$(VERSIONPKG)_Build_$(NOW)/$(MODEL_SN)/$(MODEL_ID)_R1-$(BOOT_LOADER_VERSION)-$(VERSIONPKG)_Build_$(NOW)" 2> /dev/null; \
		if [ -f "$(ROMFILE)2" ]; then \
			mv "$(ROMFILE)2" "$(TFTPDIR)/$(MODEL_ID)_$(VERSIONPKG)_Build_$(NOW)/$(MODEL_SN)/$(MODEL_ID)_R2-$(BOOT_LOADER_VERSION)-$(VERSIONPKG)_Build_$(NOW)" 2> /dev/null; \
		fi \
	fi
