# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/traceroute_only

LDFLAGS:=
#CFLAGS:=
#CPPFLAGS:=
#CXXFLAGS:=

all: $(TC_INSTALL_PREFIX)/bin/traceroute

$(TC_INSTALL_PREFIX)/bin/traceroute: traceroute
	cp traceroute $(TC_INSTALL_PREFIX)/bin/
	#$(MAKE) DESTDIR=$(TC_INSTALL_PREFIX) -f Makefile install

traceroute:
	$(MAKE) -f Makefile

romfs:
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/traceroute /bin/traceroute
	$(STRIP) $(ROMFSDIR)/bin/traceroute

package:

clean:
	make -f Makefile clean

distclean:
	#make -f Makefile distclean
	
