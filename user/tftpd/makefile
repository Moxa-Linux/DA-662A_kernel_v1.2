# History:
# Date          Author                  Comment
# 11-01-2011    Aeluin Chen.            new created
#
# compile:
#   cd stdlinux
#   make user/tftpd_only
#
#

-include $(ARCH_CONFIG)

CC=$(CROSS)gcc
STRIP=$(CROSS)strip
GDB=$(CROSS)gdb
CFLAGS :=

PACKAGE_NAME=tftp-hpa-0.45
EXEC=tftpd
EXEC_TARGET=in.$(EXEC)

all: build_$(CONFIGURE_TARGET)-linux/tftpd/$(EXEC)
	echo "Build Complete!"

build_$(CONFIGURE_TARGET)-linux/tftpd/$(EXEC):build_$(CONFIGURE_TARGET)-linux/MCONFIG
	make -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/MCONFIG:
	if [ ! -d build_$(CONFIGURE_TARGET)-linux ]; then \
		tar xzvf $(PACKAGE_NAME).tar.gz; \
		mv $(PACKAGE_NAME) build_$(CONFIGURE_TARGET)-linux; \
	fi
	( cd build_$(CONFIGURE_TARGET)-linux; \
		./configure \
		--without-tcpwrappers \
		--host=arm-linux \
		--no-create \
		--no-recursion \
		host_alias=arm-linux; \
	)

romfs:
	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/tftpd/$(EXEC)
	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/tftpd/$(EXEC) /usr/bin/$(EXEC_TARGET)
	$(ROMFSINST) $(EXEC) /etc/init.d/$(EXEC)
	$(ROMFSINST) -d 777 /home/tftpboot

package: build_$(CONFIGURE_TARGET)-linux/tftpd/$(EXEC)
	# Make the ipkg package
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/bin
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/init.d
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)-linux/home/tftpboot
	chmod 777 ipkg_src_root_$(CONFIGURE_TARGET)-linux/home/tftpboot
	cp -a ipkg_default/* ipkg_src_root_$(CONFIGURE_TARGET)-linux/
	cp -a build_$(CONFIGURE_TARGET)-linux/tftpd/$(EXEC) ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/bin/$(EXEC_TARGET)
	cp -a ./$(EXEC).pkg ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc/init.d/$(EXEC)
	$(STRIP) -s ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr/local/bin/$(EXEC_TARGET)
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_src_root_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

clean:
	/bin/rm -rf build_$(CONFIGURE_TARGET)-linux
	/bin/rm -rf ipkg_src_root_$(CONFIGURE_TARGET)-linux/usr
	/bin/rm -rf ipkg_src_root_$(CONFIGURE_TARGET)-linux/home
	/bin/rm -rf ipkg_src_root_$(CONFIGURE_TARGET)-linux/etc

