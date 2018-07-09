#  Pre-requirement:
#	apt-get install flex man2html bison xmlto
#  Cross compile:
#CROSS=arm-linux-
#ROOTDIR=/home/work/sdlinux-moxaart
#STRIP=$(CROSS)strip
#LINUXDIR=linux-2.6.x
#	make
#  Make ipkg:
#	make package

all: mkprogram mkprogram_install mkmodule26 mkmodule26_install
	echo "compile openswan OK!!"	
	
mkprogram_install:
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	make -f Makefile install \
	-C build_$(CONFIGURE_TARGET)-linux \
	CC="$(CONFIGURE_TARGET)-gcc -I$(TC_INSTALL_PREFIX)/include -DPATH_MAX=1024 -D_POSIX_HOST_NAME_MAX=256 -Wl,-rpath=/usr/local/lib" \
	LD="$(CONFIGURE_TARGET)-ld" \
	RANLIB="$(CONFIGURE_TARGET)-ranlib" \
	AR="$(CONFIGURE_TARGET)-ar" \
	SR="$(CONFIGURE_TARGET)-as" \
	AS="$(CONFIGURE_TARGET)-as" \
	STRIP="$(CONFIGURE_TARGET)-strip" \
	ARCH="arm" \
	DESTDIR="${TC_INSTALL_PREFIX}" 

#	INC_USRLOCAL="/usr/local/ipsec"
#	OSMODLIB="${TC_INSTALL_PREFIX}"

mkprogram:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf openswan-2.4.15.tar.gz
	#patch -p0 <./pluto_crypt.patch
	mv openswan-2.4.15 build_$(CONFIGURE_TARGET)-linux
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	make -f Makefile programs \
	-C build_$(CONFIGURE_TARGET)-linux \
	CC="$(CROSS)gcc -I$(TC_INSTALL_PREFIX)/include -DPATH_MAX=1024 -D_POSIX_HOST_NAME_MAX=256 -fno-strict-aliasing" \
	LD="$(CROSS)ld" \
	RANLIB="$(CROSS)ranlib" \
	AR="$(CROSS)ar" \
	SR="$(CROSS)as" \
	AS="$(CROSS)as" \
	STRIP="$(CROSS)strip" \
	LDFLAGS="-L$(TC_INSTALL_PREFIX)/lib" \
	ARCH="arm"

mkmodule26_install:
	mkdir -p $(TC_INSTALL_PREFIX)/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/net/ipsec
	cp build_$(CONFIGURE_TARGET)-linux/modobj26/ipsec.ko $(TC_INSTALL_PREFIX)/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/net/ipsec/

#	make -f Makefile minstall26 \
	-C build_$(CONFIGURE_TARGET)-linux \
	KERNELSRC="$(ROOTDIR)/$(LINUXDIR)" \
	MOD26BUILDDIR="`pwd`/build_$(CONFIGURE_TARGET)-linux/modobj26" \
	ARCH="arm" \
	OSMODLIB="$(TC_INSTALL_PREFIX)/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/net/ipsec"

	#DESTDIR="$(TC_INSTALL_PREFIX)"

mkmodule26:
	rm -rf build_$(CONFIGURE_TARGET)-linux/modobj26
	mkdir -p build_$(CONFIGURE_TARGET)-linux/modobj26
	(cd build_$(CONFIGURE_TARGET)-linux; \
	rm -rf modobj26/des;\
	mkdir -p modobj26/des \
	&& cd modobj26/des \
	&& ln -s -f linux/net/ipsec/des/* . \
	&& ln -s -f Makefile.fs2_6 Makefile)
	(cd build_$(CONFIGURE_TARGET)-linux; \
	rm -rf modobj26/aes; \
	mkdir -p modobj26/aes \
	&& cd modobj26/aes \
	&& ln -s -f linux/net/ipsec/aes/* . \
	&& ln -s -f Makefile.fs2_6 Makefile)
	(cd build_$(CONFIGURE_TARGET)-linux; \
	mkdir -p modobj26/aes; \
	cp packaging/makefiles/module26.make modobj26/Makefile; \
	ln -s -f linux/net/ipsec/match*.S  modobj26 \
	)
	sed -i 's/NF_INET_LOCAL_OUT/NF_IP_LOCAL_OUT/' build_$(CONFIGURE_TARGET)-linux/linux/net/ipsec/ipsec_xmit.c
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	make module26 -f Makefile \
	-C build_$(CONFIGURE_TARGET)-linux \
	CC="$(CONFIGURE_TARGET)-gcc" \
	AS="$(CONFIGURE_TARGET)-as" \
	LD="$(CONFIGURE_TARGET)-ld" \
	STRIP="$(CONFIGURE_TARGET)-strip" \
	ARCH="arm" \
	KERNELSRC="$(ROOTDIR)/$(LINUXDIR)" \
	MOD26BUILDDIR="`pwd`/build_$(CONFIGURE_TARGET)-linux/modobj26"

package: mkprogram mkprogram_install mkmodule26 mkmodule26_install
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/sbin
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/ipsec
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec
	cp $(TC_INSTALL_PREFIX)/lib/libgmp.so.3.4.4 ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/
	(cd ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/; ln -sf libgmp.so.3.4.4 libgmp.so.3; ln -sf libgmp.so.3.4.4 libgmp.so; )
	cp $(TC_INSTALL_PREFIX)/usr/local/sbin/ipsec ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/sbin/ipsec
	cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_copyright ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/ipsec/_copyright
	$(STRIP) ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/ipsec/_copyright
	cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_confread ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/ipsec/_confread
	cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_include ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/ipsec/_include
	cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_keycensor ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/ipsec/_keycensor
	cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_plutoload ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/ipsec/_plutoload
	cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_plutorun ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/ipsec/_plutorun
	cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_realsetup ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/ipsec/_realsetup
	sed -i 's/rmmod -s/rmmod/g'  ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/ipsec/_realsetup
	chmod a+x ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/ipsec/_realsetup
	cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_secretcensor ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/ipsec/_secretcensor
	cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_startklips ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/ipsec/_startklips
	#cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_startnetkey ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/ipsec/_startnetkey
	cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_updown ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/ipsec/_updown
	#cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_updown.klips ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/ipsec/_updown.klips
	#cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_updown.mast ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/ipsec/_updown.mast
	#cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_updown.netkey ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/ipsec/_updown.netkey
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/auto ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/auto
	#cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/addconn ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/addconn
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/barf ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/barf
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/eroute ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/eroute
	$(STRIP) ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/eroute
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/ikeping ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/ikeping
	$(STRIP) ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/ikeping
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/klipsdebug ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/klipsdebug
	$(STRIP) ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/klipsdebug
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/look ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/look
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/newhostkey ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/newhostkey
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/pf_key ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/pf_key
	$(STRIP) ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/pf_key
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/pluto ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/pluto
	$(STRIP) ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/pluto
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/_pluto_adns ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/_pluto_adns
	$(STRIP) ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/_pluto_adns
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/ranbits ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/ranbits
	$(STRIP) ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/ranbits
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/ranbits ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/ranbits
	$(STRIP) ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/ranbits
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/rsasigkey ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/rsasigkey
	$(STRIP) ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/rsasigkey
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/secrets ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/secrets
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/showdefaults ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/showdefaults
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/showhostkey ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/showhostkey
	#$(STRIP) ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/showhostkey
	#cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/showpolicy ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/showpolicy
	#$(STRIP) ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/showpolicy
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/spi ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/spi
	$(STRIP) ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/spi
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/spigrp ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/spigrp
	$(STRIP) ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/spigrp
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/tncfg ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/tncfg
	$(STRIP) ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/tncfg
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/verify ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/verify
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/whack ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/whack
	$(STRIP) ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/whack
	cp -a $(TC_INSTALL_PREFIX)/etc/ipsec.conf ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/ipsec.conf
	cp -a $(TC_INSTALL_PREFIX)/etc/ipsec.d ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/
	# Jared: 07-14-2009, Update linux-2.6.x/include/config/kernel.release
	make -C $(ROOTDIR)/$(LINUXDIR) include/config/kernel.release
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/net/ipsec
	# Jared: 07-14-2009, Followin are openswan kernel modules,
	# Now we suppoted KLIPS, we don't choose NETKEY
	cp $(TC_INSTALL_PREFIX)/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/net/ipsec/ipsec.ko ipkg_src_root_$(CONFIGURE_TARGET)-linux/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/net/ipsec/
	# Now we support NETKEY
	#cp -a $(TC_INSTALL_PREFIX)/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/net/* ipkg_src_root_$(CONFIGURE_TARGET)-linux/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/net/
	#cp -a $(TC_INSTALL_PREFIX)/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/crypto ipkg_src_root_$(CONFIGURE_TARGET)-linux/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/crypto/
	#cp -a $(TC_INSTALL_PREFIX)/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/modules.dep ipkg_src_root_$(CONFIGURE_TARGET)-linux/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/
	# Jared: 07-27-2009, Comment out /etc/rc.d/init.d/ipsec line.122~128
	cp $(TC_INSTALL_PREFIX)/etc/rc.d/init.d/ipsec ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/rc.d/init.d/ipsec
	sed -i '122,128 s/^/#/' ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/rc.d/init.d/ipsec
	chmod a+x ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/rc.d/init.d/ipsec
	# Make the ipkg package
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_src_root_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

	#cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/livetest ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/livetest
	#cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/lwdnsq ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/lwdnsq
	#$(STRIP) ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec/lwdnsq

romfs: #mkprogram mkprogram_install mkmodule26 mkmodule26_install
	mkdir -p $(ROMFSDIR)/usr/local/sbin
	mkdir -p $(ROMFSDIR)/usr/local/lib/ipsec
	mkdir -p $(ROMFSDIR)/usr/local/libexec/ipsec
	cp $(TC_INSTALL_PREFIX)/usr/local/sbin/ipsec $(ROMFSDIR)/usr/local/sbin/ipsec
	cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_copyright $(ROMFSDIR)/usr/local/lib/ipsec/_copyright
	$(STRIP) $(ROMFSDIR)/usr/local/lib/ipsec/_copyright
	cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_confread $(ROMFSDIR)/usr/local/lib/ipsec/_confread
	cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_include $(ROMFSDIR)/usr/local/lib/ipsec/_include
	cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_keycensor $(ROMFSDIR)/usr/local/lib/ipsec/_keycensor
	cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_plutoload $(ROMFSDIR)/usr/local/lib/ipsec/_plutoload
	cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_plutorun $(ROMFSDIR)/usr/local/lib/ipsec/_plutorun
	cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_realsetup $(ROMFSDIR)/usr/local/lib/ipsec/_realsetup
	sed -i 's/rmmod -s/rmmod/g'  $(ROMFSDIR)/usr/local/lib/ipsec/_realsetup
	chmod a+x $(ROMFSDIR)/usr/local/lib/ipsec/_realsetup
	cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_secretcensor $(ROMFSDIR)/usr/local/lib/ipsec/_secretcensor
	cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_startklips $(ROMFSDIR)/usr/local/lib/ipsec/_startklips
	cp $(TC_INSTALL_PREFIX)/usr/local/lib/ipsec/_updown $(ROMFSDIR)/usr/local/lib/ipsec/_updown
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/auto $(ROMFSDIR)/usr/local/libexec/ipsec/auto
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/barf $(ROMFSDIR)/usr/local/libexec/ipsec/barf
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/eroute $(ROMFSDIR)/usr/local/libexec/ipsec/eroute
	$(STRIP) $(ROMFSDIR)/usr/local/libexec/ipsec/eroute
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/ikeping $(ROMFSDIR)/usr/local/libexec/ipsec/ikeping
	$(STRIP) $(ROMFSDIR)/usr/local/libexec/ipsec/ikeping
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/klipsdebug $(ROMFSDIR)/usr/local/libexec/ipsec/klipsdebug
	$(STRIP) $(ROMFSDIR)/usr/local/libexec/ipsec/klipsdebug
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/look $(ROMFSDIR)/usr/local/libexec/ipsec/look
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/newhostkey $(ROMFSDIR)/usr/local/libexec/ipsec/newhostkey
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/pf_key $(ROMFSDIR)/usr/local/libexec/ipsec/pf_key
	$(STRIP) $(ROMFSDIR)/usr/local/libexec/ipsec/pf_key
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/pluto $(ROMFSDIR)/usr/local/libexec/ipsec/pluto
	$(STRIP) $(ROMFSDIR)/usr/local/libexec/ipsec/pluto
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/_pluto_adns $(ROMFSDIR)/usr/local/libexec/ipsec/_pluto_adns
	$(STRIP) $(ROMFSDIR)/usr/local/libexec/ipsec/_pluto_adns
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/ranbits $(ROMFSDIR)/usr/local/libexec/ipsec/ranbits
	$(STRIP) $(ROMFSDIR)/usr/local/libexec/ipsec/ranbits
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/ranbits $(ROMFSDIR)/usr/local/libexec/ipsec/ranbits
	$(STRIP) $(ROMFSDIR)/usr/local/libexec/ipsec/ranbits
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/rsasigkey $(ROMFSDIR)/usr/local/libexec/ipsec/rsasigkey
	$(STRIP) $(ROMFSDIR)/usr/local/libexec/ipsec/rsasigkey
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/secrets $(ROMFSDIR)/usr/local/libexec/ipsec/secrets
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/showdefaults $(ROMFSDIR)/usr/local/libexec/ipsec/showdefaults
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/showhostkey $(ROMFSDIR)/usr/local/libexec/ipsec/showhostkey
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/spi $(ROMFSDIR)/usr/local/libexec/ipsec/spi
	$(STRIP) $(ROMFSDIR)/usr/local/libexec/ipsec/spi
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/spigrp $(ROMFSDIR)/usr/local/libexec/ipsec/spigrp
	$(STRIP) $(ROMFSDIR)/usr/local/libexec/ipsec/spigrp
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/tncfg $(ROMFSDIR)/usr/local/libexec/ipsec/tncfg
	$(STRIP) $(ROMFSDIR)/usr/local/libexec/ipsec/tncfg
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/verify $(ROMFSDIR)/usr/local/libexec/ipsec/verify
	cp $(TC_INSTALL_PREFIX)/usr/local/libexec/ipsec/whack $(ROMFSDIR)/usr/local/libexec/ipsec/whack
	$(STRIP) $(ROMFSDIR)/usr/local/libexec/ipsec/whack
	cp -a $(TC_INSTALL_PREFIX)/etc/ipsec.conf $(ROMFSDIR)/etc/ipsec.conf
	cp -a $(TC_INSTALL_PREFIX)/etc/ipsec.d $(ROMFSDIR)/etc/
	# Jared: 07-14-2009, Followin are openswan kernel modules,
	# Now we suppoted KLIPS, we don't choose NETKEY
	#cp $(TC_INSTALL_PREFIX)/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/net/ipsec/ipsec.ko $(ROMFSDIR)/lib/modules/`cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`/kernel/net/ipsec/
	cp $(TC_INSTALL_PREFIX)/etc/rc.d/init.d/ipsec $(ROMFSDIR)/etc/rc.d/init.d/ipsec
	sed -i '122,128 s/^/#/' $(ROMFSDIR)/etc/rc.d/init.d/ipsec
	chmod a+x $(ROMFSDIR)/etc/rc.d/init.d/ipsec

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	rm -rf ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/ipsec.conf
	rm -rf ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/ipsec.d
	rm -rf ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/sbin
	rm -rf ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/lib/ipsec
	rm -rf ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/libexec/ipsec
	rm -rf ipkg_src_root_$(CONFIGURE_TARGET)-linux/lib/modules

