# History:
# Date		Author			Comment
# 01-6-2010	Jared Wu.		install at BERoot or LERoot
# 12-29-2011    Aeluin Chen.    do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# Pre-requirement:
# 	1. Set the Perl v5.10.1 search PATH "/usr/local/bin"
#		$ export PATH=/usr/local/bin:$PATH
#	2. Build perl for i686 the same version.
#	  	$ tar xjvf perl-5.10.1.tar.bz2
#		$ mv perl-5.10.1 build_x86-linux
#		$ cd build_x86-linux
#		$ ./configure.gnu
#		$ make
#	3 Backup the original perl and install the latest perl
#		$ mv /usr/local/bin/perl /usr/local/bin/perl.org
#		$ mv /usr/local/bin/perl5.10.1 /usr/local/bin/perl5.10.1.org
#		$ make install
#
# Cross-compile perl:
#
#	make user/perl_only
#
ARCH_PATCH=`echo $(TOOLCHAIN_DIR)|cut -d'/' -f4`

#all: /usr/local/bin/perl build_$(CONFIGURE_TARGET)-linux/install_me_here/usr/bin/perl
all: build_$(CONFIGURE_TARGET)-linux/install_me_here/usr/bin/perl
	echo "Build Complete!"

build_$(CONFIGURE_TARGET)-linux/install_me_here/usr/bin/perl:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xjvf perl-5.10.1.tar.bz2
	mv perl-5.10.1 build_$(CONFIGURE_TARGET)-linux
	chmod a+w build_$(CONFIGURE_TARGET)-linux/Cross/*
	### Rename the miniperl    ###
	rm -rf miniperl
	cp -a miniperl.$(shell uname -m) miniperl
	### patch for signal issue ###
	( cd build_$(CONFIGURE_TARGET)-linux/Cross; \
	cp -a ../../patches/$(ARCH_PATCH)/config.$(CONFIGURE_TARGET)-linux ./config; \
	cp -a ../../patches/$(ARCH_PATCH)/Makefile.$(CONFIGURE_TARGET)-linux ./Makefile; \
	export PATH=$(shell pwd)/usr/local/bin:${PATH}; \
	make patch; \
	make perl; \
	)

/usr/local/bin/perl:
	rm -rf build_perl_i686
	tar xjvf perl-5.10.1.tar.bz2
	mv perl-5.10.1 build_perl_i686
	(cd build_perl_i686; \
	unset CC LD STRIP CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && ./configure.gnu; \
	make; \
	make install-strip \
	)

romfs:

package: build_$(CONFIGURE_TARGET)-linux/install_me_here/usr/bin/perl
	mkdir -p ipkg_perl_$(CONFIGURE_TARGET)-linux/home/perl/bin
	mkdir -p ipkg_perl_$(CONFIGURE_TARGET)-linux/home/perl/lib
	find build_$(CONFIGURE_TARGET)-linux/install_me_here -name .packlist |xargs rm -rf
	cp -a build_$(CONFIGURE_TARGET)-linux/install_me_here/usr/bin/* ipkg_perl_$(CONFIGURE_TARGET)-linux/home/perl/bin/
	cp -a build_$(CONFIGURE_TARGET)-linux/install_me_here/usr/lib/* ipkg_perl_$(CONFIGURE_TARGET)-linux/home/perl/lib/
	cp -a CPAM_lib/install_me_here_$(CONFIGURE_TARGET)-linux/usr/bin/* ipkg_perl_$(CONFIGURE_TARGET)-linux/home/perl/bin/
	cp -a CPAM_lib/install_me_here_$(CONFIGURE_TARGET)-linux/usr/lib/* ipkg_perl_$(CONFIGURE_TARGET)-linux/home/perl/lib/
	find ipkg_perl_$(CONFIGURE_TARGET)-linux -name libperl.so |xargs $(STRIP) -s
	find ipkg_perl_$(CONFIGURE_TARGET)-linux -name "*.so" |xargs $(STRIP) -s
	$(STRIP) -s ipkg_perl_$(CONFIGURE_TARGET)-linux/home/perl/bin/perl5.10.1 \
	ipkg_perl_$(CONFIGURE_TARGET)-linux/home/perl/bin/perl
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_perl_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux

clean distclean: 
	rm -rf build_$(CONFIGURE_TARGET)-linux \
	ipkg_perl_$(CONFIGURE_TARGET)-linux/home \
	miniperl

