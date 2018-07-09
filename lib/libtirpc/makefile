# History:
# Date		Author			Comment
# 08-10-2013	Aeluin Chen.	new create
#
# compile: 
#	make lib/libtirpc_only
#

INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)/install-me-here
INSTALL_PREFIX = $(TC_INSTALL_PREFIX)
TARGET_VER=0.2.3
TARGET_PRJ=libtirpc
TARGET_SRC=$(TARGET_PRJ)-$(TARGET_VER)
TARGET_SRC_DIR=target_src_dir

all: $(INSTALL_PREFIX)/lib/$(TARGET_PRJ).so

untar-all-source:
	if [ ! -d $(TARGET_SRC_DIR) ]; then \
		[ ! -f $(TARGET_SRC).tar.bz2 ] && \
			wget http://downloads.sourceforge.net/project/libtirpc/libtirpc/$(TARGET_VER)/$(TARGET_SRC).tar.bz2; \
		[ -f $(TARGET_SRC).tar.bz2 ] && tar xjf $(TARGET_SRC).tar.bz2; \
		(cd $(TARGET_SRC); \
			patch -Np1 -i ../libtirpc-0.2.3-remove_nis-1.patch; \
			patch -Np1 -i ../libtirpc-0.2.3-add_key.patch; \
			sed "s@AM_CONFIG_HEADER@AC_CONFIG_HEADERS@g" -i configure.ac; \
			autoreconf -fi; \
		); \
		mv $(TARGET_SRC) $(TARGET_SRC_DIR); \
	fi


build:
	if [ -d "build_$(CONFIGURE_TARGET)" ]; then \
		rm -rf build_$(CONFIGURE_TARGET); \
	fi
	# Rename for different configure
	mkdir -p build_$(CONFIGURE_TARGET)
	# Configure 
	(cd build_$(CONFIGURE_TARGET); \
	../$(TARGET_SRC_DIR)/configure \
	CFLAGS="-fPIC" \
	--prefix="$(INSTALL_PREFIX)" \
	--host=$(CONFIGURE_TARGET) \
	)
	make -C build_$(CONFIGURE_TARGET)
	make -C build_$(CONFIGURE_TARGET) install

$(INSTALL_PREFIX)/lib/$(TARGET_PRJ).so:
	make untar-all-source
	make build


# install to romfs
romfs: $(TC_INSTALL_PREFIX)/lib/$(TARGET_PRJ).so
	$(ROMFSINST) "$(TC_INSTALL_PREFIX)/lib/$(TARGET_PRJ).so*" /lib


# copy from toolchain to romfs
tc2romfs:
	#$(ROMFSINST) "$(TC_LIBDIR)/$(TARGET_PRJ).so" /lib


# only install include files & libraries
min-install: $(INSTALL_PREFIX)/lib/$(TARGET_PRJ).so
	cp -a $(INSTALL_PREFIX)/include/* $(TC_INSTALL_PREFIX)/include
	cp -a $(INSTALL_PREFIX)/lib/$(TARGET_PRJ).so $(TC_INSTALL_PREFIX)/lib


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)
	rm -rf $(TARGET_SRC_DIR)
