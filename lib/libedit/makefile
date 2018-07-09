# History:
# Date		Author			Comment
# 03-08-2013	Jared Wu.		Build the libedit which is a replacement of GNU readline library.
#					user/ike dependent on this library.
#
# compile: 
#	make lib/libedit_only

PACKAGE_NAME=libedit-20121213-3.0

all: $(TC_INSTALL_PREFIX)/lib/libedit.a


$(TC_INSTALL_PREFIX)/lib/libedit.a:build_$(CONFIGURE_TARGET)-linux/src/.libs/libedit.a
	make -C build_$(CONFIGURE_TARGET)-linux install


build_$(CONFIGURE_TARGET)-linux/src/.libs/libedit.a:build_$(CONFIGURE_TARGET)-linux/configured
	make -C build_$(CONFIGURE_TARGET)-linux


build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf $(PACKAGE_NAME).tar.gz
	mv $(PACKAGE_NAME) build_$(CONFIGURE_TARGET)-linux
	( \
	cd build_$(CONFIGURE_TARGET)-linux; \
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	CC="$(CC)" \
	LD="$(CC)" \
	AR="$(AR)" \
	LDFLAGS="-lncurses" \
	./configure \
	--build=i686-linux \
	--host=$(CONFIGURE_TARGET) \
	--prefix=$(TC_INSTALL_PREFIX) \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured


romfs:
	echo "Nothing to install"


package:


clean distclean:
	rm -fr build_$(CONFIGURE_TARGET)-linux
