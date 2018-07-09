# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011    Aeluin Chen.    do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/ipkg_only

CONFIGURE_SETTINGS :=	
all: $(TC_INSTALL_PREFIX)/bin/ipkg-cl


$(TC_INSTALL_PREFIX)/bin/ipkg-cl: build_$(CONFIGURE_TARGET)-linux/.libs/ipkg-cl
	make -C build_$(CONFIGURE_TARGET)-linux install


build_$(CONFIGURE_TARGET)-linux/.libs/ipkg-cl: build_$(CONFIGURE_TARGET)-linux/Makefile
	unset LDFLAGS CC CFLAGS CPPFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET)-linux


build_$(CONFIGURE_TARGET)-linux/Makefile: makefile
	rm -rf build_$(CONFIGURE_TARGET)-linux;
	tar xzvf ipkg-0.99.163.tar.gz
	mv ipkg-0.99.163 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux;		\
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS &&  \
	CC=$(CROSS)gcc		\
	LD=$(CROSS)ld		\
	./configure				\
	--build=i686-linux			\
	--host=$(CONFIGURE_TARGET)		\
	--without-shared			\
	--disable-shared			\
	--without-pic				\
	--with-ipkglibdir="/home/ipkg"		\
	--prefix=$(TC_INSTALL_PREFIX)		\
	)

romfs: $(TC_INSTALL_PREFIX)/bin/ipkg-cl
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/ipkg-cl /bin/ipkg-cl
	$(STRIP) -s $(ROMFSDIR)/bin/ipkg-cl

package: #$(TC_INSTALL_PREFIX)/bin/ipkg-cl

# Jared 08-05-2008 said:
# In order to speed-up the making proess, we don't clean the pre-build binaries.
# If you need to rebuild the binaries, please make distclean in this directory.
#clean:
#	make -C build_$(CONFIGURE_TARGET)-linux clean

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux
