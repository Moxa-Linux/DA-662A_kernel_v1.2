# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
#
# compile: 
# 	cd stdlinux
#	make user/iptables_only

all: $(TC_INSTALL_PREFIX)/sbin/iptables

$(TC_INSTALL_PREFIX)/sbin/iptables: build_$(CONFIGURE_TARGET)-linux/iptables
	make -C build_$(CONFIGURE_TARGET)-linux install
	[ -d $(TC_INSTALL_PREFIX)/lib/iptables ] || mkdir -p $(TC_INSTALL_PREFIX)/lib/iptables
	cp -a build_$(CONFIGURE_TARGET)-linux/extensions/*.so $(TC_INSTALL_PREFIX)/lib/iptables


build_$(CONFIGURE_TARGET)-linux/iptables: build_$(CONFIGURE_TARGET)-linux/Makefile
	unset LDFLAGS CC CFLAGS CPPFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/Makefile: makefile
	rm -rf build_$(CONFIGURE_TARGET)-linux;
	mkdir build_$(CONFIGURE_TARGET)-linux;
	( cd build_$(CONFIGURE_TARGET)-linux;		\
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS &&	\
	CC=$(CROSS)gcc					\
	LD=$(CROSS)ld					\
	AR=$(CROSS)ar					\
	STRIP=$(CROSS)strip				\
	../configure					\
	--build=i686-linux				\
	--host=$(CONFIGURE_TARGET)-linux		\
	--with-kernel=$(ROOTDIR)/$(LINUXDIR)/include	\
	--prefix=$(TC_INSTALL_PREFIX)			\
	--with-xtlibdir=/lib/iptables			\
	)

	#--with-xtlibdir=/lib/iptables			\


romfs: $(TC_INSTALL_PREFIX)/sbin/iptables
	[ -d $(ROMFSDIR)/lib/iptables ] || mkdir -p $(ROMFSDIR)/lib/iptables
	cp -a $(TC_INSTALL_PREFIX)/lib/iptables/*.so $(ROMFSDIR)/lib/iptables
	$(STRIP) -s $(ROMFSDIR)/lib/iptables/*.so
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/sbin/iptables /sbin/iptables
	$(STRIP) -s $(ROMFSDIR)/sbin/iptables
ifeq ($(DO_MULTI), 1)
	$(ROMFSINST) -l $(ROMFSDIR)/sbin/iptables /sbin/iptables-save
	$(ROMFSINST) -l $(ROMFSDIR)/sbin/iptables /sbin/iptables-restore
else
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/sbin/iptables-save /sbin/iptables-save
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/sbin//iptables-restore /sbin/iptables-restore
	$(STRIP) -s $(ROMFSDIR)/sbin/iptables-save
	$(STRIP) -s $(ROMFSDIR)/sbin/iptables-restore
endif
#	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/iptables
#	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/iptables-save
#	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/iptables-restore
#	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/extensions/*.so
#	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/iptables /sbin/iptables
#	[ -d $(ROMFSDIR)/lib/iptables ] || mkdir -p $(ROMFSDIR)/lib/iptables
#	cp -a build_$(CONFIGURE_TARGET)-linux/extensions/*.so $(ROMFSDIR)/lib/iptables
#ifeq ($(DO_MULTI), 1)
#	$(ROMFSINST) -l $(ROMFSDIR)/sbin/iptables /sbin/iptables-save
#	$(ROMFSINST) -l $(ROMFSDIR)/sbin/iptables /sbin/iptables-restore
#else
#	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/iptables-save /sbin/iptables-save
#	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/iptables-restore /sbin/iptables-restore
#endif

package: #$(TC_INSTALL_PREFIX)/sbin/iptables

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux
