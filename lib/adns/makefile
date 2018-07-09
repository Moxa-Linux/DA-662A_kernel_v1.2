# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/adns_only
#

all: build_$(CONFIGURE_TARGET)-linux/config.cache
	( cd build_$(CONFIGURE_TARGET)-linux; \
	$(MAKE) MAKE="$(MAKE) -f Makefile" \
	)

romfs: build_$(CONFIGURE_TARGET)-linux/config.cache
	( cd build_$(CONFIGURE_TARGET)-linux; \
	$(MAKE) MAKE="$(MAKE) -f Makefile " $@ \
	)

build_$(CONFIGURE_TARGET)-linux/config.cache: makefile
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf adns.tar.bz2
	mv adns build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	./configure --with-headers=$(TC_INSTALL_PREFIX)/include \
	--host=$(CONFIGURE_TARGET) \
	)

clean:
	$(MAKE) MAKE="$(MAKE) -f Makefile" clean

distclean: #clean
	rm -rf build_*

package:
