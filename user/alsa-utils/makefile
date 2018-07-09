# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/alsa-utils_only

all: build_$(CONFIGURE_TARGET)-linux/aplay/aplay

#$(TC_INSTALL_PREFIX)/bin/aplay: build_$(CONFIGURE_TARGET)-linux/.libs/aplay
#	#make -C build_$(CONFIGURE_TARGET)-linux install;

build_$(CONFIGURE_TARGET)-linux/aplay/aplay: build_$(CONFIGURE_TARGET)-linux/Makefile
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET)-linux LD=$(CC) -j 2;

build_$(CONFIGURE_TARGET)-linux/Makefile: makefile
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf alsa-utils-1.0.16.tar.bz2
	mv alsa-utils-1.0.16 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	./configure \
	ac_cv_header_panel_h="no" \
	CC=$(CROSS)gcc \
	LD=$(CROSS)ld \
	CXX=$(CROSS)g++ \
	CPP=$(CROSS)cpp \
	CFLAGS="-DPATH_MAX=4096 -I$(TOOLCHAIN_DIR)/include/ncurses" \
	LDFLAGS="-L`pwd`/../../../lib/alsa-lib/build_$(CONFIGURE_TARGET)-linux/src/.libs" \
	--build=i686-linux \
	--host=$(CONFIGURE_TARGET) \
	--target=$(CONFIGURE_TARGET) \
	--with-pic \
	--disable-alsatest \
	--disable-xmlto \
	--disable-largefile \
	--disable-nls \
	--with-alsa-inc-prefix="`pwd`/../../../lib/alsa-lib/build_$(CONFIGURE_TARGET)-linux/include" \
	)


romfs: #build_$(CONFIGURE_TARGET)-linux/aplay/aplay
	echo 'nothing'
#	$(ROMFSINST) $(TC_INSTALL_PREFIX)/lib/libasound.so.2.0.0 /lib/libasound.so.2.0.0
#	$(ROMFSINST) -s libasound.so.2.0.0 /lib/libasound.so.2

package: build_$(CONFIGURE_TARGET)-linux/aplay/aplay
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/sound/usb
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/sound/core
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/drivers/usb/class
	cp -a $(ROOTDIR)/$(LINUXDIR)/sound/usb/snd-usb-lib.ko ipkg_src_root_$(CONFIGURE_TARGET)-linux/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/sound/usb/
	cp -a $(ROOTDIR)/$(LINUXDIR)/sound/usb/snd-usb-audio.ko ipkg_src_root_$(CONFIGURE_TARGET)-linux/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/sound/usb/
	cp -a $(ROOTDIR)/$(LINUXDIR)/sound/core/snd-page-alloc.ko ipkg_src_root_$(CONFIGURE_TARGET)-linux/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/sound/core/
	cp -a $(ROOTDIR)/$(LINUXDIR)/sound/core/snd-hwdep.ko ipkg_src_root_$(CONFIGURE_TARGET)-linux/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/sound/core/
	cp -a $(ROOTDIR)/$(LINUXDIR)/sound/core/snd.ko ipkg_src_root_$(CONFIGURE_TARGET)-linux/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/sound/core/
	cp -a $(ROOTDIR)/$(LINUXDIR)/sound/core/snd-timer.ko ipkg_src_root_$(CONFIGURE_TARGET)-linux/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/sound/core/
	cp -a $(ROOTDIR)/$(LINUXDIR)/sound/core/snd-pcm.ko ipkg_src_root_$(CONFIGURE_TARGET)-linux/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/sound/core/
	cp -a $(ROOTDIR)/$(LINUXDIR)/sound/core/snd-rawmidi.ko ipkg_src_root_$(CONFIGURE_TARGET)-linux/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/sound/core/
	cp -a $(ROOTDIR)/$(LINUXDIR)/sound/soundcore.ko ipkg_src_root_$(CONFIGURE_TARGET)-linux/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/sound/
	cp -a $(ROOTDIR)/$(LINUXDIR)/drivers/usb/class/cdc-acm.ko ipkg_src_root_$(CONFIGURE_TARGET)-linux/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/drivers/usb/class/
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/lib
	cp -a $(ROOTDIR)/lib/alsa-lib/build_$(CONFIGURE_TARGET)-linux/src/.libs/libasound.so.2.0.0 ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/lib/
	$(STRIP) -s ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/lib/libasound.so.2.0.0
	(cd ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/lib ; \
	ln -sf libasound.so.2.0.0 libasound.so; \
	ln -sf libasound.so.2.0.0 libasound.so.2; \
	)
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/bin
	cp -a build_$(CONFIGURE_TARGET)-linux/aplay/aplay ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/amixer/amixer ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/alsactl/alsactl ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/alsamixer/alsamixer ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/amidi/amidi ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/amixer/amixer ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/iecset/iecset ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/seq/aconnect/aconnect ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/seq/aseqdump/aseqdump ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/seq/aplaymidi/aplaymidi ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/seq/aseqnet/aseqnet ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/seq/aseqnet/aseqnet ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/bin/
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/share/alsa
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/share/alsa/cards
	cp -a $(ROOTDIR)/lib/alsa-lib/build_$(CONFIGURE_TARGET)-linux/src/conf/cards/aliases.alisp ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/share/alsa/cards/
	cp -a $(ROOTDIR)/lib/alsa-lib/build_$(CONFIGURE_TARGET)-linux/src/conf/cards/aliases.conf ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/share/alsa/cards/
	cp -a $(ROOTDIR)/lib/alsa-lib/build_$(CONFIGURE_TARGET)-linux/src/conf/alsa.conf ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/share/alsa/
	cp -a $(ROOTDIR)/lib/alsa-lib/build_$(CONFIGURE_TARGET)-linux/src/conf/pcm ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/share/alsa/
	$(STRIP) build_$(CONFIGURE_TARGET)-linux/aplay/aplay
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_src_root_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


clean distclean:
	rm -fr build_$(CONFIGURE_TARGET)-linux
	rm -fr ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/lib/libasound.so*
