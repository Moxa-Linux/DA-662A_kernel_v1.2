# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011    Aeluin Chen.    do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	cd stdlinux
#	make user/pppd-2.4.4_only


all: $(TC_INSTALL_PREFIX)/sbin/pppd 

$(TC_INSTALL_PREFIX)/sbin/pppd: pppd
	[ -d $(ROMFSDIR)/share/man/man8 ] || mkdir -p $(ROMFSDIR)/share/man/man8
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	make DESTDIR=$(TC_INSTALL_PREFIX) -f Makefile install

pppd: Makefile
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	make -C chat -f Makefile CC=$(CROSS)gcc CXX=$(CROSS)g++ LD=$(CROSS)ld CPP=$(CROSS)cpp STRIP=$(CROSS)strip;
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	make -C pppd/plugins -f Makefile CC=$(CROSS)gcc CXX=$(CROSS)g++ LD=$(CROSS)ld CPP=$(CROSS)cpp STRIP=$(CROSS)strip;
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	make -C pppd -f Makefile CC=$(CROSS)gcc CXX=$(CROSS)g++ LD=$(CROSS)ld CPP=$(CROSS)cpp STRIP=$(CROSS)strip;
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	make -C pppstats -f Makefile CC=$(CROSS)gcc CXX=$(CROSS)g++ LD=$(CROSS)ld CPP=$(CROSS)cpp STRIP=$(CROSS)strip;
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	make -C pppdump -f Makefile CC=$(CROSS)gcc CXX=$(CROSS)g++ LD=$(CROSS)ld CPP=$(CROSS)cpp STRIP=$(CROSS)strip;

Makefile: makefile
	./configure
	sed -ie 's/-s//g' chat/Makefile
	sed -ie 's/$$(INSTALL) -s/$$(INSTALL)/g' pppd/Makefile
	sed -ie 's/$$(INSTALL) -s/$$(INSTALL)/g' pppstats/Makefile
	sed -ie 's/$$(INSTALL) -s/$$(INSTALL)/g' pppdump/Makefile
	sed -ie 's/$$(INSTALL) -s/$$(INSTALL)/g' pppd/plugins/rp-pppoe/Makefile
	sed -ie 's/$$(INSTALL) -s/$$(INSTALL)/g' pppd/plugins/radius/Makefile
	
romfs: #$(TC_INSTALL_PREFIX)/sbin/pppd 
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/sbin/pppd /sbin/pppd
	$(STRIP) -s $(ROMFSDIR)/sbin/pppd
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/sbin/chat /usr/sbin/chat
	$(STRIP) -s $(ROMFSDIR)/usr/sbin/chat
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/lib/pppd/2.4.4/rp-pppoe.so /lib/rp-pppoe.so
	#$(STRIP) -s $(ROMFSDIR)/lib/rp-pppoe.so

package:
	echo "Nothing to do"

clean distclean:
	make -f Makefile dist-clean;

