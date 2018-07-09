# History:
# Date		Author			Comment
# 06-24-2014	Jared Wu.		First created.
#
# compile: 
#	make user/python-2.7.5_only


all: build_$(CONFIGURE_TARGET)-linux/python_for_build build_$(CONFIGURE_TARGET)-linux/install-me-here/bin/python


build_$(CONFIGURE_TARGET)-linux/install-me-here/bin/python:build_$(CONFIGURE_TARGET)-linux/python
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install \
	HOSTPYTHON="./python_for_build" \
	BLDSHARED="$(CC) -shared" \
	CROSS_COMPILE="$(CROSS)" \
	CROSS_COMPILE_TARGET="yes" \
	DESTDIR="install-me-here"



build_$(CONFIGURE_TARGET)-linux/python: build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux \
	HOSTPYTHON="./python_for_build" \
	HOSTPGEN="./pgen/hostpgen" \
	BLDSHARED="$(CC) -shared" \
	CROSS_COMPILE="$(CROSS)" \
	CROSS_COMPILE_TARGET="yes" \
	HOSTARCH="$(CONFIGURE_TARGET)-linux" \
	BUILDARCH=x86_64-linux-gnu


build_$(CONFIGURE_TARGET)-linux/configured: build_$(CONFIGURE_TARGET)-linux/patched
	#sed -i '12764,12767s/^/#/' build_$(CONFIGURE_TARGET)-linux/configure
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	CC="$(CC)" \
	CXX="$(CXX)" \
	LD="$(CC)" \
	CPP="$(CPP)" \
	AR="$(AR)" \
	RANLIB="$(RANLIB)" \
	BASECFLAGS="-I$(TC_INSTALL_PREFIX)/include/ncurses" \
	CFLAGS="-I$(TC_INSTALL_PREFIX)/include/ncurses -I$(TC_INSTALL_PREFIX)/include" \
	CPPFLAGS="-I$(TC_INSTALL_PREFIX)/include/ncurses -I$(TC_INSTALL_PREFIX)/include" \
	LDFLAGS="-L$(TC_LIBDIR)" \
	SSL="$(TC_INSTALL_PREFIX)" \
	ac_cv_file__dev_ptmx=no \
	ac_cv_file__dev_ptc=no \
	ac_cv_have_long_long_format=yes \
	ac_cv_func_lchmod=no \
	ac_cv_have_chflags=no \
	ac_cv_func_tmpnam=no \
	ac_cv_func_tmpnam_r=no \
	sh ./configure \
	--host="$(CONFIGURE_TARGET)-linux" \
	--build=i686-linux-gnu \
	--prefix="/" \
	--disable-ipv6 \
	--enable-shared \
	--with-threads=yes \
	)
	find build_$(CONFIGURE_TARGET)-linux -name Makefile|xargs sed -i 's/-ltermcap/-lncurses/g'
	sed -i 's/-L$$(exec_prefix)\/lib//g' build_$(CONFIGURE_TARGET)-linux/Makefile
	sed -i 's/-DSVNVERSION=\"\\"`LC_ALL=C\ $$(SVNVERSION)`\\"\"\ //g' build_$(CONFIGURE_TARGET)-linux/Makefile
	touch build_$(CONFIGURE_TARGET)-linux/configured

build_$(CONFIGURE_TARGET)-linux/patched:
	( cd build_$(CONFIGURE_TARGET)-linux; \
	patch -p3 < ../Python-2.7.5-xcompile.patch; \
	)
	touch build_$(CONFIGURE_TARGET)-linux/patched

build_$(CONFIGURE_TARGET)-linux/python_for_build:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjf Python-2.7.5.tar.bz2
	mv Python-2.7.5 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CC CXX LD CPP CFLAGS AR RANLIB CPPFLAGS LDFLAGS CXXFLAGS && \
	sh ./configure \
	)
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux python Parser/pgen
	cp -a build_$(CONFIGURE_TARGET)-linux/python build_$(CONFIGURE_TARGET)-linux/python_for_build
	cp -a build_$(CONFIGURE_TARGET)-linux/Parser/pgen build_$(CONFIGURE_TARGET)-linux/Parser/pgen_for_build
	make -C build_$(CONFIGURE_TARGET)-linux distclean


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux \
	ipkg_python_$(CONFIGURE_TARGET)-linux/home \

romfs:
	mkdir -p $(ROMFSDIR)/lib/python2.7
	$(STRIP) -s build_$(CONFIGURE_TARGET)-linux/install-me-here/bin/python2.7 \
	build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/libpython2.7.so.1.0
	$(ROMFSINST) "build_$(CONFIGURE_TARGET)-linux/install-me-here/bin/python2.7*" /bin/
	$(ROMFSINST) "build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/libpython*.so*" /lib/
	#cp -a build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/python2.7 $(ROMFSDIR)/lib/
	$(ROMFSINST) "build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/python2.7/*" /lib/python2.7/
	rm -rf $(ROMFSDIR)/lib/python2.7/config
	

package: build_$(CONFIGURE_TARGET)-linux/python_for_build build_$(CONFIGURE_TARGET)-linux/python
	# Make the ipkg package
	rm -rf ipkg_python_$(CONFIGURE_TARGET)-linux/home
	mkdir -p ipkg_python_$(CONFIGURE_TARGET)-linux/home/python/bin \
	ipkg_python_$(CONFIGURE_TARGET)-linux/home/python/include \
	ipkg_python_$(CONFIGURE_TARGET)-linux/home/python/lib/python2.7/lib-dynload
	cp -a build_$(CONFIGURE_TARGET)-linux/install-me-here/bin/* ipkg_python_$(CONFIGURE_TARGET)-linux/home/python/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/libpython*.so* ipkg_python_$(CONFIGURE_TARGET)-linux/home/python/lib/
	cp -a build_$(CONFIGURE_TARGET)-linux/build/lib.linux2-arm-2.7/*.so ipkg_python_$(CONFIGURE_TARGET)-linux/home/python/lib/python2.7/lib-dynload/
	$(STRIP) -s ipkg_python_$(CONFIGURE_TARGET)-linux/home/python/bin/python \
	ipkg_python_$(CONFIGURE_TARGET)-linux/home/python/lib/libpython*.so* \
	ipkg_python_$(CONFIGURE_TARGET)-linux/home/python/lib/python2.7/lib-dynload/*.so
	(cd ipkg_python_$(CONFIGURE_TARGET)-linux/home/python/bin/; ln -sf python2.7 python)
	cp -a build_$(CONFIGURE_TARGET)-linux/install-me-here/lib/python2.7 ipkg_python_$(CONFIGURE_TARGET)-linux/home/python/lib/
	cp -a build_$(CONFIGURE_TARGET)-linux/install-me-here/include/python2.7 ipkg_python_$(CONFIGURE_TARGET)-linux/home/python/include/
	# Remove the redundanct files
	find ipkg_python_$(CONFIGURE_TARGET)-linux/home/python -name "test"|xargs rm -rf
	find ipkg_python_$(CONFIGURE_TARGET)-linux/home/python -name "*.a"|xargs rm -rf
	find ipkg_python_$(CONFIGURE_TARGET)-linux/home/python -name "Icons"|xargs rm -rf
	find ipkg_python_$(CONFIGURE_TARGET)-linux/home/python -name "tests"|xargs rm -rf
	find ipkg_python_$(CONFIGURE_TARGET)-linux/home/python -name "lib-tk"|xargs rm -rf
	find ipkg_python_$(CONFIGURE_TARGET)-linux/home/python -name "*.so"|xargs $(STRIP)
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_python_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


