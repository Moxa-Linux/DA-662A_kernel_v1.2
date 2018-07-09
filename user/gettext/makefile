# History:
# Date		Author			Comment
# 07-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011    Aeluin Chen.    do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/gettext_only


all: build_$(CONFIGURE_TARGET)-linux/gettext-runtime/src/gettext
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install

build_$(CONFIGURE_TARGET)-linux/gettext-runtime/src/gettext: build_$(CONFIGURE_TARGET)-linux/configured
	find $(TC_INSTALL_PREFIX) -name "*.la"|xargs rm -rf
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf gettext-0.18.1.1.tar.gz
	mv gettext-0.18.1.1 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	CC=$(CROSS)gcc \
	LD=$(CROSS)ld \
	CPP=$(CROSS)cpp \
	STRIP=$(CROSS)strip \
	sh ./configure \
	--host=$(CONFIGURE_TARGET) \
	--build=i686 \
	--prefix="$(TC_INSTALL_PREFIX)" \
	--with-libiconv-prefix="$(TC_INSTALL_PREFIX)" \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured

clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/local/sbin \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib

romfs: #build_$(CONFIGURE_TARGET)-linux/gettext-runtime/src/gettext
#	$(STRIP) build_$(CONFIGURE_TARGET)-linux/gettext-runtime/src/gettext
#	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/gettext-runtime/src/gettext /usr/sbin/gettext

package: build_$(CONFIGURE_TARGET)-linux/gettext-runtime/src/gettext
	# Make the ipkg package
	rm -rf ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/local/sbin \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/local/sbin/
	cp -a build_$(CONFIGURE_TARGET)-linux/gettext-runtime/src/gettext ipkg_$(CONFIGURE_TARGET)-linux/usr/local/sbin/gettext
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/usr/local/sbin/gettext
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/src/msgcmp ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/src/msgfmt ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/src/msgmerge ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/src/msgunfmt ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/src/xgettext ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/src/msgattrib ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/src/msgcat ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/src/msgcomm ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/src/msgconv ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/src/msgen ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/src/msgexec ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/src/msggrep ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/src/msginit ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/src/msguniq ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/src/recode-sr-latin ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/*
	cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/src/msgfilter ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/misc/gettextize ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/misc/autopoint ipkg_$(CONFIGURE_TARGET)-linux/usr/local/bin/
	#mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib
	#cp -a build_$(CONFIGURE_TARGET)-linux/gettext-runtime/intl/.libs/libgnuintl.so.8.1.1 ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib
	#(cd ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib; ln -sf libgnuintl.so.8.1.1 libgnuintl.so.8; ln -sf libgnuintl.so.8.1.1 libgnuintl.so)
	#cp -a build_$(CONFIGURE_TARGET)-linux/gettext-runtime/libasprintf/.libs/libasprintf.so.0.0.0 ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib
	#(cd ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib; ln -sf libasprintf.so.0.0.0 libasprintf.so.0; ln -sf libasprintf.so.0.0.0 libasprintf.so)
	#cp -a build_$(CONFIGURE_TARGET)-linux/gettext-runtime/libasprintf/.libs/libasprintf.so.0.0.0 ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib
	#(cd ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib; ln -sf libasprintf.so.0.0.0 libasprintf.so.0; ln -sf libasprintf.so.0.0.0 libasprintf.so)
	#cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/intl/.libs/libgnuintl.so.8.1.1 ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib
	#(cd ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib; ln -sf libgnuintl.so.8.1.1 libgnuintl.so.8; ln -sf libgnuintl.so.8.1.1 libgnuintl.so)
	#cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/gnulib-lib/.libs/libgettextlib-0.18.1.so ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib
	#(cd ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib; ln -sf libgettextlib-0.18.1.so libgettextlib.so)
	#cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/src/.libs/libgettextsrc-0.18.1.so ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib
	#(cd ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib; ln -sf libgettextsrc-0.18.1.so libgettextsrc.so)
	#cp -a build_$(CONFIGURE_TARGET)-linux/gettext-tools/libgettextpo/.libs/libgettextpo.so.0.5.1 ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib
	#(cd ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib; ln -sf libgettextpo.so.0.5.1 libgettextpo.so.0; ln -sf libgettextpo.so.0.5.1 libgettextpo.so)
	#$(STRIP) -s ipkg_$(CONFIGURE_TARGET)-linux/usr/local/lib/*
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


