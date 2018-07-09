# History:
# Date		Author			Comment
# 07-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/smtpclient_only

all: $(TC_INSTALL_PREFIX)/bin/smtpclient
	cp -a build_$(CONFIGURE_TARGET)-linux/smtpclient $(TC_INSTALL_PREFIX)/bin/


$(TC_INSTALL_PREFIX)/bin/smtpclient:build_$(CONFIGURE_TARGET)-linux/Makefile
	make -C build_$(CONFIGURE_TARGET)-linux -f Makefile
	
build_$(CONFIGURE_TARGET)-linux/Makefile:makefile
	tar xzvf smtpclient.tar.gz
	mv smtpclient build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	CC=$(CROSS)gcc \
	LD=$(CROSS)ld \
	./configure \
	--host=i686 \
	--target=$(CONFIGURE_TARGET) \
	--prefix=$(TC_INSTALL_PREFIX) \
	)

	
clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux

romfs:
	$(STRIP) $(TC_INSTALL_PREFIX)/bin/smtpclient
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/smtpclient /bin/smtpclient

package:
	# Make the ipkg package

