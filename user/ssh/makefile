# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make user/ssh_only

all: $(TC_INSTALL_PREFIX)/bin/ssh

$(TC_INSTALL_PREFIX)/bin/ssh: build_$(CONFIGURE_TARGET)/ssh
	sed -i "s/STRIP_OPT=-s/STRIP_OPT=/g" build_$(CONFIGURE_TARGET)/Makefile
	make -C build_$(CONFIGURE_TARGET) DESTDIR=$(TC_INSTALL_PREFIX) install-files

build_$(CONFIGURE_TARGET)/ssh: build_$(CONFIGURE_TARGET)/build
	sed -i "s/-O2//g" build_$(CONFIGURE_TARGET)/Makefile
	sed -i "s/-Wuninitialized//g" build_$(CONFIGURE_TARGET)/Makefile
	unset LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET) \
	LD=$(CC)

build_$(CONFIGURE_TARGET)/build:build_$(CONFIGURE_TARGET)/patched
	( cd build_$(CONFIGURE_TARGET); \
	unset LIBS LDFLAGS CFLAGS CPPFLAGS CXXFLAGS && \
	ac_cv_path_AR="$(AR)" \
	CC="$(CC)" \
	LD="$(CC)" \
	CXX="$(CXX)" \
	CPP="$(CPP)" \
	STRIP="$(STRIP)" \
	RANLIB="$(RANLIB)" \
	./configure \
	--build=i686-linux \
	--host=$(CONFIGURE_TARGET)-linux \
	--without-xauth \
	--exec-prefix="/" \
	--sysconfdir="/etc/ssh" \
	--with-cflags="-I $(ROOTDIR)/lib/openssl/build_$(CONFIGURE_TARGET)/include -D_FORTIFY_SOURCE=0" \
	--with-ldflags="-L $(ROOTDIR)/lib/openssl/build_$(CONFIGURE_TARGET)" \
	--prefix="$(TC_INSTALL_PREFIX)" \
	)
	touch build_$(CONFIGURE_TARGET)/build

	#--with-cflags="-g" \
	#--with-cflags="-I$(TC_INSTALL_PREFIX)/include -g" \
	#--with-zlib="$(ROOTDIR)/lib/zlib" \
	#--with-ldflags="-L$(TC_INSTALL_PREFIX)/lib" \

build_$(CONFIGURE_TARGET)/patched:
	rm -rf build_$(CONFIGURE_TARGET)
	#tar xzvf openssh-5.2p1.tar.gz
	#mv  openssh-5.2p1 build_$(CONFIGURE_TARGET)
	tar xzvf openssh_6.7p1.orig.tar.gz
	mv openssh-6.7p1 build_$(CONFIGURE_TARGET)
	#( cd build_$(CONFIGURE_TARGET); patch -p1 < ../cipher_no_rc4-5.2p1.patch; )
	( cd build_$(CONFIGURE_TARGET); patch -p1 < ../cipher_no_rc4-6.7p1.patch; )
	touch build_$(CONFIGURE_TARGET)/patched

romfs:
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/ssh /bin/ssh
	$(STRIP) -s $(ROMFSDIR)/bin/ssh
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/sbin/sshd /bin/sshd
	$(STRIP) -s $(ROMFSDIR)/bin/sshd
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/scp /bin/scp
	$(STRIP) -s $(ROMFSDIR)/bin/scp
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/sftp /bin/sftp
	$(STRIP) -s $(ROMFSDIR)/bin/sftp
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/libexec/sftp-server /bin/sftp-server
	$(STRIP) -s $(ROMFSDIR)/bin/sftp-server
	$(ROMFSINST) $(TC_INSTALL_PREFIX)/bin/ssh-keygen /bin/ssh-keygen
	$(STRIP) -s $(ROMFSDIR)/bin/ssh-keygen

package: $(TC_INSTALL_PREFIX)/bin/ssh

clean distclean:
	rm -fr build_$(CONFIGURE_TARGET);
