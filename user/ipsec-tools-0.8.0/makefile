# History:
# Date          Author                  Comment
# 11-24-2011    Aeluin Chen.            new created
#
#
# compile:
#	cd stdlinux
#	make user/ipsec-tools-0.8.0_only

-include $(ARCH_CONFIG)
-include $(CONFIG_CONFIG)

PACKAGE_NAME=ipsec-tools-0.8.0

ifeq ("$(CONFIGURE_TARGET)","xscale")
	CFLAGS := -DBIG_ENDIAN -I build_$(CONFIGURE_TARGET)-linux/src/include-glibc
	LDFLAGS := -L$(TC_LIBDIR)
	LIBS := -lresolv
endif


all: build_$(CONFIGURE_TARGET)-linux/install_me_here/sbin/racoon
	# Upload to target for testing
	#$(STRIP) build_$(CONFIGURE_TARGET)-linux/install_me_here/sbin/setkey \
	#build_$(CONFIGURE_TARGET)-linux/install_me_here/sbin/racoon \
	#build_$(CONFIGURE_TARGET)-linux/install_me_here/sbin/racoonctl \
	#build_$(CONFIGURE_TARGET)-linux/install_me_here/sbin/plainrsa-gen
	#scp build_$(CONFIGURE_TARGET)-linux/install_me_here/sbin/setkey \
	#build_$(CONFIGURE_TARGET)-linux/install_me_here/sbin/racoon \
	#build_$(CONFIGURE_TARGET)-linux/install_me_here/sbin/racoonctl \
	#build_$(CONFIGURE_TARGET)-linux/install_me_here/sbin/plainrsa-gen \
	#192.168.27.109:/sbin/

build_$(CONFIGURE_TARGET)-linux/install_me_here/sbin/racoon: build_$(CONFIGURE_TARGET)-linux/src/racoon/racoon
	make -C build_$(CONFIGURE_TARGET)-linux install
	echo "Install $(PACKAGE_NAME) completed"

build_$(CONFIGURE_TARGET)-linux/src/racoon/racoon:build_$(CONFIGURE_TARGET)-linux/configured
	make -C build_$(CONFIGURE_TARGET)-linux
	echo "Build $(PACKAGE_NAME) completed"

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf ./$(PACKAGE_NAME).tar.bz2
	mv $(PACKAGE_NAME) build_$(CONFIGURE_TARGET)-linux
	sed -i 's/-Werror//g' build_$(CONFIGURE_TARGET)-linux/configure
	(cd build_$(CONFIGURE_TARGET)-linux ; \
	CC="$(CC)" \
	LD="$(CC)" \
	CXX="$(CXX)" \
	AR="$(AR)" \
	STRIP="$(STRIP)" \
	RANLIB="$(RANLIB)" \
	./configure \
	--host="$(CONFIGURE_TARGET)-unknown-linux-gnu" \
	--build="$(uname -r)" \
	--localstatedir="/var" \
	--sysconfdir="/etc" \
	--with-kernel-headers="$(ROOTDIR)/$(LINUXDIR)/include" \
	--without-readline \
	--enable-security-context=no \
	--enable-adminport \
	--enable-hybrid \
	--enable-frag \
	--enable-dpd \
	--enable-natt \
	--enable-broken-natt \
	--with-flexlib="$(TC_LIBDIR)/libfl.a" \
	--with-openssl="$(TC_INSTALL_PREFIX)" \
	--prefix="`pwd`/install_me_here"; \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured

clean:
	rm -rf build_$(CONFIGURE_TARGET)-linux

romfs: #$(INSTALL_ROOT)
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/setkey/setkey /sbin/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/racoon/racoon /sbin/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/racoon/racoonctl /sbin/
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/src/racoon/plainrsa-gen /sbin/
	$(STRIP) $(ROMFSDIR)/sbin/setkey \
	$(ROMFSDIR/sbin/racoon \
	$(ROMFSDIR)/sbin/racoonctl \
	$(ROMFSDIR)/sbin/plainrsa-gen

package:

