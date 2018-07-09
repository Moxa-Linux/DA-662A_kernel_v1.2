# History:
# Date          Author                  Comment
# 11-24-2011    Aeluin Chen.            new created
#
#
# compile:
#	cd stdlinux
#	make user/ipsec-tools-0.7_only

-include $(ARCH_CONFIG)
-include $(CONFIG_CONFIG)

PACKAGE_NAME=ipsec-tools-0.7

ifeq ("$(CONFIGURE_TARGET)","xscale")
	CFLAGS += -DBIG_ENDIAN -DARCH_UC8400 -DARCH_UC9400
else
	LDFLAGS :=
endif


all: build_$(CONFIGURE_TARGET)-linux/Makefile

build_$(CONFIGURE_TARGET)-linux/Makefile:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf ./$(PACKAGE_NAME).tar.bz2
	mv $(PACKAGE_NAME) build_$(CONFIGURE_TARGET)-linux
	(cd build_$(CONFIGURE_TARGET)-linux ; \
	CC="$(CROSS)gcc -isystem $(TC_INSTALL_PREFIX)/include" \
	CXX=$(CROSS)g++ \
	AR=$(CROSS)ar \
	STRIP=$(CROSS)strip \
	RANLIB=$(CROSS)ranlib \
	LD=$(CROSS)ld \
	AS=$(CROSS)as \
	NM=$(CROSS)nm \
	./configure --host=$(CONFIGURE_TARGET) \
	--with-kernel-headers=$(LINUXDIR)/include \
	--without-readline \
	--enable-security-context=no \
	--with-flexlib=$(CURDIR)/build_$(CONFIGURE_TARGET)-linux/flex/build/libfl.a \
	--prefix=$(CURDIR)/build_$(CONFIGURE_TARGET)-linux/install_me_here; \
	make ; \
	make install ; \
	)

clean:
	rm -rf build_$(CONFIGURE_TARGET)-linux

romfs: #$(INSTALL_ROOT)

package:

