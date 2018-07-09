# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 03-12-2013	Aeluin Chen.		fixed for newer kernel version
#
# compile: 
#	make user/pppd-2.4.5_only
#
# reference:
# 	http://forums.gentoo.org/viewtopic-t-931534-start-0.html
#
# note:
# 	remove the patch (ppp-2.4.5-pppol2tp.patch) if using older kernel
#

TARGET_IP=192.168.27.110

all: $(TC_INSTALL_PREFIX)/sbin/pppd
	#$(STRIP) build_$(CONFIGURE_TARGET)-linux/pppd/pppd \
	#build_$(CONFIGURE_TARGET)-linux/chat/chat
	#scp build_$(CONFIGURE_TARGET)-linux/pppd/pppd $(TARGET_IP):/sbin/
	#scp build_$(CONFIGURE_TARGET)-linux/chat/chat $(TARGET_IP):/usr/sbin/
	#scp build_$(CONFIGURE_TARGET)-linux/pppd/plugins/rp-pppoe/rp-pppoe.so $(TARGET_IP):/lib/

$(TC_INSTALL_PREFIX)/sbin/pppd: build_$(CONFIGURE_TARGET)-linux/pppd
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	make DESTDIR=$(TC_INSTALL_PREFIX) -C build_$(CONFIGURE_TARGET)-linux -f Makefile install

build_$(CONFIGURE_TARGET)-linux/pppd: build_$(CONFIGURE_TARGET)-linux/Makefile
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET)-linux/chat -f Makefile CC=$(CROSS)gcc CXX=$(CROSS)g++ LD=$(CROSS)ld CPP=$(CROSS)cpp STRIP=$(CROSS)strip;
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET)-linux/pppd/plugins -f Makefile CC="$(CROSS)gcc -DHAVE_NET_IF_H" CXX=$(CROSS)g++ LD=$(CROSS)ld CPP=$(CROSS)cpp STRIP=$(CROSS)strip;
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET)-linux/pppd -f Makefile CC=$(CROSS)gcc CXX=$(CROSS)g++ LD=$(CROSS)ld CPP=$(CROSS)cpp STRIP=$(CROSS)strip;
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET)-linux/pppstats -f Makefile CC=$(CROSS)gcc CXX=$(CROSS)g++ LD=$(CROSS)ld CPP=$(CROSS)cpp STRIP=$(CROSS)strip;
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET)-linux/pppdump -f Makefile CC=$(CROSS)gcc CXX=$(CROSS)g++ LD=$(CROSS)ld CPP=$(CROSS)cpp STRIP=$(CROSS)strip;

build_$(CONFIGURE_TARGET)-linux/Makefile: makefile
	tar xzvf ppp-2.4.5.tar.gz
	mv -f ppp-2.4.5 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	patch -p1 -i ../ppp-2.4.5-pppol2tp.patch; \
	./configure; \
	sed -ie 's/-s//g' chat/Makefile;\
	sed -ie 's/$$(INSTALL) -s/$$(INSTALL)/g' pppd/Makefile;\
	sed -ie 's/$$(INSTALL) -s/$$(INSTALL)/g' pppstats/Makefile;\
	sed -ie 's/$$(INSTALL) -s/$$(INSTALL)/g' pppdump/Makefile;\
	sed -ie 's/$$(INSTALL) -s/$$(INSTALL)/g' pppd/plugins/rp-pppoe/Makefile;\
	sed -ie 's/$$(INSTALL) -s/$$(INSTALL)/g' pppd/plugins/radius/Makefile;\
	)
	
romfs: #$(TC_INSTALL_PREFIX)/sbin/pppd 
	[ -d $(ROMFSDIR)/share/man/man8 ] || mkdir -p $(ROMFSDIR)/share/man/man8
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/sbin/pppd /sbin/pppd
	$(STRIP) -s $(ROMFSDIR)/sbin/pppd
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/sbin/chat /usr/sbin/chat
	$(STRIP) -s $(ROMFSDIR)/usr/sbin/chat
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/lib/pppd/2.4.5/rp-pppoe.so /lib/rp-pppoe.so
	$(STRIP) -s $(ROMFSDIR)/lib/rp-pppoe.so

package:
	# Make the ipkg package
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/sbin \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin \
	ipkg_$(CONFIGURE_TARGET)-linux/lib
	cp -a $(TC_INSTALL_PREFIX)/sbin/pppd ipkg_$(CONFIGURE_TARGET)-linux/sbin/
	cp -a $(TC_INSTALL_PREFIX)/sbin/chat ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin/
	cp -a $(TC_INSTALL_PREFIX)/lib/pppd/2.4.5/rp-pppoe.so ipkg_$(CONFIGURE_TARGET)-linux/lib/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/sbin/pppd \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin/chat
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux \
	ipkg_$(CONFIGURE_TARGET)-linux/sbin \
	ipkg_$(CONFIGURE_TARGET)-linux/usr \
	ipkg_$(CONFIGURE_TARGET)-linux/lib

