# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/tinylogin_only

all: $(TC_INSTALL_PREFIX)/bin/tinylogin


$(TC_INSTALL_PREFIX)/bin/tinylogin: tinylogin
	$(MAKE) CROSS=$(CROSS) PREFIX=$(TC_INSTALL_PREFIX) -f Makefile install

tinylogin:
#	sed -ie "s/^CROSS =/#CROSS =/g" Makefile
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	$(MAKE) CROSS=$(CROSS) -f Makefile tinylogin

romfs: #$(TC_INSTALL_PREFIX)/bin/tinylogin
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/tinylogin /bin/tinylogin
	chmod a+s $(ROMFSDIR)/bin/tinylogin
	chmod a+x $(ROMFSDIR)/bin/tinylogin
	$(ROMFSINST) -s /bin/tinylogin /sbin/sulogin
	$(ROMFSINST) -s /bin/tinylogin /sbin/getty
	$(ROMFSINST) -s /bin/tinylogin /bin/adduser
	$(ROMFSINST) -s /bin/tinylogin /bin/deluser
	$(ROMFSINST) -s /bin/tinylogin /bin/login
	$(ROMFSINST) -s /bin/tinylogin /bin/su
	$(ROMFSINST) -s /bin/tinylogin /bin/passwd
	$(ROMFSINST) -s /bin/tinylogin /bin/addgroup
	$(ROMFSINST) -s /bin/tinylogin /bin/delgroup

package:
	echo "Nothing to do"

clean:
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	make -f Makefile clean

distclean:
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	make -f Makefile distclean
	
