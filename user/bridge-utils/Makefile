# Generated automatically from Makefile.in by configure.

DESTDIR=
KERNEL_HEADERS=-I/usr/src/linux/include

INSTALL=install -s

prefix=$(TC_INSTALL_PREFIX)
exec_prefix=${prefix}
bindir=${exec_prefix}/bin
sbindir=${exec_prefix}/sbin
mandir=${prefix}/man

SUBDIRS=libbridge brctl doc


all: $(TC_INSTALL_PREFIX)/sbin/brctl
	@echo "brctl has installed at $(TC_INSTALL_PREFIX)"

$(TC_INSTALL_PREFIX)/sbin/brctl: brctl/brctl
	for x in $(SUBDIRS); do (cd $$x && $(MAKE) prefix=$(TC_INSTALL_PREFIX) install ); done
	@echo "Install bridge-utils finish"

brctl/brctl:
	for x in $(SUBDIRS); do (cd $$x && $(MAKE) ); done

clean:
	for x in $(SUBDIRS); do (cd $$x && $(MAKE) clean ); done

distclean:	clean
	rm -f config.log
	rm -f config.cache
	rm -f config.status

maintainer-clean: distclean
	rm -f Makefile
	rm -f brctl/Makefile
	rm -f libbridge/Makefile
	rm -f doc/Makefile

install:
	for x in $(SUBDIRS); do (cd $$x && $(MAKE) install ); done

romfs: $(TC_INSTALL_PREFIX)/sbin/brctl
	#$(ROMFSINST) brctl/brctl /bin/brctl
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/sbin/brctl /sbin/brctl
	$(STRIP) -s $(ROMFSDIR)/sbin/brctl

package: #$(TC_INSTALL_PREFIX)/sbin/brctl
