# History:
# Date		Author			Comment
# 10-02-2012	Jared Wu.		First create.
#
# pre-requirement:
#	python-2.6.6
#
# compile: 
#	make user/python-2.6.6_only


all: build_$(CONFIGURE_TARGET)-linux/hostpython build_$(CONFIGURE_TARGET)-linux/python
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux install \
	HOSTPYTHON="./hostpython" \
	BLDSHARED="$(CC) -shared" \
	CROSS_COMPILE="$(CROSS)" \
	CROSS_COMPILE_TARGET="yes" \
	prefix="$(TC_INSTALL_PREFIX)/home/python"



build_$(CONFIGURE_TARGET)-linux/python: build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux \
	HOSTPYTHON="./hostpython" \
	HOSTPGEN="./Parser/hostpgen" \
	BLDSHARED="$(CC) -shared" \
	CROSS_COMPILE="$(CROSS)" \
	CROSS_COMPILE_TARGET="yes" \
	HOSTARCH="$(CONFIGURE_TARGET)-linux" 


build_$(CONFIGURE_TARGET)-linux/configured:
	#sed -i '12764,12767s/^/#/' build_$(CONFIGURE_TARGET)-linux/configure
	( cd build_$(CONFIGURE_TARGET)-linux; \
	patch -p1 < ../Python-2.6.6-xcompile.patch; \
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	CC="$(CC)" \
	CXX="$(CXX)" \
	LD="$(CC)" \
	CPP="$(CPP)" \
	AR="$(AR)" \
	RANLIB="$(RANLIB)" \
	BASECFLAGS="-I$(TC_INSTALL_PREFIX)/include/ncurses" \
	CPPFLAGS="-I$(TC_INSTALL_PREFIX)/include/ncurses" \
	LDFLAGS="-L$(TC_LIBDIR)" \
	sh ./configure \
	--host="$(CONFIGURE_TARGET)-linux" \
	--build=i686 \
	--prefix="$(TC_INSTALL_PREFIX)/home/python" \
	)
	sed -i 's/-DSVNVERSION=\"\\"`LC_ALL=C\ $$(SVNVERSION)`\\"\"\ //g' build_$(CONFIGURE_TARGET)-linux/Makefile
	touch build_$(CONFIGURE_TARGET)-linux/configured

build_$(CONFIGURE_TARGET)-linux/hostpython:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf python2.6-2.6.6.tar.bz2
	mv python2.6-2.6.6 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CC CXX LD CPP CFLAGS AR RANLIB CPPFLAGS LDFLAGS CXXFLAGS && \
	sh ./configure \
	)
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux python Parser/pgen
	cp -a build_$(CONFIGURE_TARGET)-linux/python build_$(CONFIGURE_TARGET)-linux/hostpython
	cp -a build_$(CONFIGURE_TARGET)-linux/Parser/pgen build_$(CONFIGURE_TARGET)-linux/Parser/hostpgen
	make -C build_$(CONFIGURE_TARGET)-linux distclean


clean distclean:
	rm -rf build_$(CONFIGURE_TARGET)-linux \
	ipkg_python_$(CONFIGURE_TARGET)-linux/home \

romfs: #build_$(CONFIGURE_TARGET)-linux/python
#	$(STRIP) build_$(CONFIGURE_TARGET)-linux/python
#	$(ROMFSINST) build_$(CONFIGURE_TARGET)-linux/python /usr/sbin/python

package: build_$(CONFIGURE_TARGET)-linux/hostpython build_$(CONFIGURE_TARGET)-linux/python
	# Make the ipkg package
	rm -rf ipkg_python_$(CONFIGURE_TARGET)-linux/home
	mkdir -p ipkg_python_$(CONFIGURE_TARGET)-linux/home/python/bin \
	ipkg_python_$(CONFIGURE_TARGET)-linux/home/python/include \
	ipkg_python_$(CONFIGURE_TARGET)-linux/home/python/lib
	cp -a $(TC_INSTALL_PREFIX)/home/python/bin/* ipkg_python_$(CONFIGURE_TARGET)-linux/home/python/bin/
	$(STRIP) -s ipkg_python_$(CONFIGURE_TARGET)-linux/home/python/bin/python
	(cd ipkg_python_$(CONFIGURE_TARGET)-linux/home/python/bin/; ln -sf python2.6 python)
	cp -a $(TC_INSTALL_PREFIX)/home/python/lib/python2.6 ipkg_python_$(CONFIGURE_TARGET)-linux/home/python/lib/
	cp -a $(TC_INSTALL_PREFIX)/home/python/include/python2.6 ipkg_python_$(CONFIGURE_TARGET)-linux/home/python/include/
	# Remove the redundanct files
	find ipkg_python_$(CONFIGURE_TARGET)-linux/home/python -name "test"|xargs rm -rf
	find ipkg_python_$(CONFIGURE_TARGET)-linux/home/python -name "*.a"|xargs rm -rf
	find ipkg_python_$(CONFIGURE_TARGET)-linux/home/python -name "Icons"|xargs rm -rf
	find ipkg_python_$(CONFIGURE_TARGET)-linux/home/python -name "tests"|xargs rm -rf
	find ipkg_python_$(CONFIGURE_TARGET)-linux/home/python -name "lib-tk"|xargs rm -rf
	find ipkg_python_$(CONFIGURE_TARGET)-linux/home/python -name "*.so"|xargs $(STRIP)
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_python_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux


