# History:
# Date		Author			Comment
# 01-16-2014	Aeluin Chen.	new create
#
# compile: 
#	make project/gpsdo_only
#
# required:
#   make user/sqlite3_only
#   make user/gpsd_only
#

CONFIGURE_TARGET=$(CROSS:-=)
INSTALL_PREFIX = $(CURDIR)/build_$(CONFIGURE_TARGET)/install-me-here
TARGET_VER=1.0
TARGET_PRJ=gpsdo
TARGET_BIN=gps-daemon
TARGET_SRC=$(TARGET_PRJ)-$(TARGET_VER)
TARGET_SRC_DIR=target_src_dir

LOCAL_PREFIX=/usr


all: $(INSTALL_PREFIX)$(LOCAL_PREFIX)/bin/$(TARGET_BIN)

untar-all-source:
	if [ ! -d $(TARGET_SRC_DIR) ]; then \
		[ ! -f $(TARGET_SRC).tar.bz2 ] && \
		wget http://192.168.27.133/release/gpsdo.tar.bz2; \
		[ -f $(TARGET_SRC).tar.bz2 ] && tar xjf $(TARGET_SRC).tar.bz2; \
		(cd $(TARGET_SRC); \
		); \
		mv $(TARGET_SRC) $(TARGET_SRC_DIR); \
	fi


build:
	if [ -d "build_$(CONFIGURE_TARGET)" ]; then \
		rm -rf build_$(CONFIGURE_TARGET); \
	fi
	# Rename for different configure
	mv $(TARGET_SRC_DIR) build_$(CONFIGURE_TARGET)
	# Configure 
	(cd build_$(CONFIGURE_TARGET); \
	autoreconf --install; \
	CFLAGS='-I $(TC_INSTALL_PREFIX)/include' \
	./configure \
		--host=$(CONFIGURE_TARGET) \
		--prefix=$(LOCAL_PREFIX) \
		--sysconfdir=/etc \
		--htmldir=/home/httpd/htdocs; \
	)
	make -C build_$(CONFIGURE_TARGET)
	make -C build_$(CONFIGURE_TARGET) install DESTDIR=$(INSTALL_PREFIX)


$(INSTALL_PREFIX)$(LOCAL_PREFIX)/bin/$(TARGET_BIN):
	make untar-all-source
	make build

$(TC_INSTALL_PREFIX)$(LOCAL_PREFIX)/bin/$(TARGET_BIN): $(INSTALL_PREFIX)$(LOCAL_PREFIX)/bin/$(TARGET_BIN)
	make install


# install into toolchain
install:
	cp -R $(INSTALL_PREFIX)/* $(TC_INSTALL_PREFIX)


# install to romfs
romfs: $(INSTALL_PREFIX)$(LOCAL_PREFIX)/bin/$(TARGET_BIN)
	$(ROMFSINST) -n dist/etc/init.d /etc/init.d
	$(ROMFSINST) -e CONFIG_PROJECT_GPSDO_START -n dist/etc/rc.d /etc/rc.d
	$(ROMFSINST) -n $(INSTALL_PREFIX) /

package:

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)
	rm -rf $(TARGET_SRC_DIR)
