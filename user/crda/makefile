# History:
# Date		Author			Comment
# 06-25-2014	Jared Wu.		First created
#
# pre-requirement:
#	apt-get install python-m2crypto libgpg-error-dev libgcrypt11-dev libnl-dev
#
# compile: 
#	make user/crda_only
#

all: wireless-regdb/regulatory.bin build_$(CONFIGURE_TARGET)-linux/host-regdbdump build_$(CONFIGURE_TARGET)-linux/regdbdump

wireless-regdb/regulatory.bin:
	unset CC LD LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	make -C wireless-regdb
	# put the public key to right location
	mkdir -p /etc/wireless-regdb/pubkeys
	cp wireless-regdb/root.key.pub.pem /etc/wireless-regdb/pubkeys/

build_$(CONFIGURE_TARGET)-linux/host-regdbdump:build_$(CONFIGURE_TARGET)-linux/host-configured
	unset CC LD LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	USE_OPENSSL=1 \
	REG_BIN=`pwd`/wireless-regdb/regulatory.bin \
	V=1 \
	NLLIBS="-L/usr/lib -lnl" \
	CFLAGS="-I/usr/include" \
	LDFLAGS="-lssl" \
	make -C build_$(CONFIGURE_TARGET)-linux
	mv build_$(CONFIGURE_TARGET)-linux/regdbdump build_$(CONFIGURE_TARGET)-linux/host-regdbdump
	make -C build_$(CONFIGURE_TARGET)-linux clean

build_$(CONFIGURE_TARGET)-linux/host-configured:build_$(CONFIGURE_TARGET)-linux/pre-installed
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjf crda-1.1.2.tar.bz2
	mv crda-1.1.2 build_$(CONFIGURE_TARGET)-linux
	touch build_$(CONFIGURE_TARGET)-linux/host-configured


build_$(CONFIGURE_TARGET)-linux/regdbdump:build_$(CONFIGURE_TARGET)-linux/configured
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	USE_OPENSSL=1 \
	REG_BIN=`pwd`/wireless-regdb/regulatory.bin \
	NLLIBNAME="libnl-1" \
	NLLIBS="-L$(TC_LIBDIR) -lnl" \
	CFLAGS="-I$(TC_INSTALL_PREFIX)/include -I$(ROOTDIR)/lib/libnl/build_$(CONFIGURE_TARGET)-linux/include" \
	LDFLAGS="-lssl -lcrypto" \
	CC="$(CC)" \
	LD="$(LD)" \
	V=1 \
	make -C build_$(CONFIGURE_TARGET)-linux 
	touch build_$(CONFIGURE_TARGET)-linux/build
	#$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/crda
	#scp build_$(CONFIGURE_TARGET)-linux/crda 192.168.31.205:/dev/shm/

build_$(CONFIGURE_TARGET)-linux/configured:build_$(CONFIGURE_TARGET)-linux/pre-installed
	sed -i 's/NLLIBS += `pkg-config --libs $$(NLLIBNAME)`/#NLLIBS += `pkg-config --libs $$(NLLIBNAME)`/g' build_$(CONFIGURE_TARGET)-linux/Makefile
	sed -i 's/CFLAGS += `pkg-config --cflags $$(NLLIBNAME)`/#CFLAGS += `pkg-config --cflags $$(NLLIBNAME)`/g' build_$(CONFIGURE_TARGET)-linux/Makefile
	sed -i 's/`pkg-config --cflags openssl`//g' build_$(CONFIGURE_TARGET)-linux/Makefile
	sed -i 's/`pkg-config --libs openssl`//g' build_$(CONFIGURE_TARGET)-linux/Makefile
	sed -i 's/.\/regdbdump/.\/host-regdbdump/g' build_$(CONFIGURE_TARGET)-linux/Makefile
	touch build_$(CONFIGURE_TARGET)-linux/configured

build_$(CONFIGURE_TARGET)-linux/pre-installed:
	# If your system has not install these packages, you can uncomment this line to install the packages
	apt-get install -y python-m2crypto libgpg-error-dev libgcrypt11-dev libnl-dev

romfs:
	mkdir -p $(ROMFSDIR)/etc/default \
	$(ROMFSDIR)/lib/udev/rules.d \
	$(ROMFSDIR)/lib/crda/pubkeys \
	$(ROMFSDIR)//etc/wireless-regdb/pubkeys
	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/regdbdump
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/regdbdump /sbin/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/crda /sbin/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/debian/crda.default /etc/default/crda
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/debian/crda.udev /lib/udev/rules.d/60-crda.rules
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/udev/regulatory.rules /lib/udev/rules.d/85-regulatory.rules
	sed -i 's/$$(SBINDIR)/\/sbin\//g' $(ROMFSDIR)/lib/udev/rules.d/85-regulatory.rules
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/debian/setregdomain /lib/crda/
	chmod a+x $(ROMFSDIR)/lib/crda/setregdomain
	$(ROMFSINST) wireless-regdb/regulatory.bin /lib/crda/
	$(ROMFSINST) wireless-regdb/root.key.pub.pem /etc/wireless-regdb/pubkeys/

package:

clean distclean:
	rm -fr build_$(CONFIGURE_TARGET)-linux;
	make -C wireless-regdb mrproper
	make -C wireless-regdb clean
