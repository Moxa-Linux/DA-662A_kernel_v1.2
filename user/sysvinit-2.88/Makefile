# Makefile for Linux System V init utility
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
#
# History:
# Date		Author			Comment
# 07-22-2005	Victor Yu.		Create it.
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
BINS=bootlogd halt init last mesg mountpoint runlevel shutdown sulogin utmpdump wall
DIRS = src #contrib

all: $(TC_INSTALL_PREFIX)/sbin/init


$(TC_INSTALL_PREFIX)/sbin/init: $(BINS)
	mkdir -p $(TC_INSTALL_PREFIX)/usr/include
	mkdir -p $(TC_INSTALL_PREFIX)/usr/share/man/man1
	mkdir -p $(TC_INSTALL_PREFIX)/usr/share/man/man5
	mkdir -p $(TC_INSTALL_PREFIX)/usr/share/man/man8
	mkdir -p $(TC_INSTALL_PREFIX)/usr/bin
	mkdir -p $(TC_INSTALL_PREFIX)/usr/sbin
	for i in $(DIRS) ; do \
		unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
		make ROOT=$(TC_INSTALL_PREFIX) -C $$i install; \
	done

$(BINS):
	for i in $(DIRS) ; do \
		unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
		CFLAGS="-DINITDEBUG -DDEBUG " \
		make CC=$(CROSS)gcc -C $$i $@; \
	done

romfs: #$(TC_INSTALL_PREFIX)/sbin/init
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/sbin/init /sbin/init
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/sbin/halt /sbin/halt
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/sbin/shutdown /sbin/shutdown
	$(STRIP) -s $(ROMFSDIR)/sbin/init 
	$(STRIP) -s $(ROMFSDIR)/sbin/halt 
	$(STRIP) -s $(ROMFSDIR)/sbin/shutdown
	( cd  ${ROMFSDIR}/sbin; \
	[ -L telinit ] || ln -s halt telinit; \
	[ -L reboot ] || ln -s halt reboot; \
	[ -L poweroff ] || ln -s halt poweroff; \
	)

package: #$(TC_INSTALL_PREFIX)/sbin/init

clean distclean:
	for i in $(DIRS) ; do \
		make -C $$i clobber ;\
	done

