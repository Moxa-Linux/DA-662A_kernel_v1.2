

CONFIGURE_SETTINGS := --build=i686-linux \
		--host=$(CONFIGURE_TARGET) \
		--target=$(CONFIGURE_TARGET) \
		--enable-nfsv3

all: Makefile
	unset CFLAGS CC CPPFLAGS LDFLAGS CXXFLAGS && make -f Makefile all
	make -f Makefile install

Makefile:	Makefile
	unset CFLAGS CC CPPFLAGS LDFLAGS CXXFLAGS && \
	CC=$(CROSS)gcc BUILD_CC=gcc ./configure $(CONFIGURE_SETTINGS)

romfs:
	$(STRIP) -s utils/statd/statd
	$(STRIP) -s utils/lockd/lockd
	$(STRIP) -s utils/nfsd/nfsd
	$(STRIP) -s utils/mountd/mountd
	$(STRIP) -s utils/exportfs/exportfs
	$(ROMFSINST) utils/statd/statd /usr/sbin/rpc.statd
	$(ROMFSINST) utils/lockd/lockd /usr/sbin/rpc.lockd
	$(ROMFSINST) utils/nfsd/nfsd /usr/sbin/rpc.nfsd
	$(ROMFSINST) utils/mountd/mountd /usr/sbin/rpc.mountd
	$(ROMFSINST) utils/exportfs/exportfs /usr/sbin/exportfs

package:

clean distclean:
	make -f Makefile distclean

%:
	make -f Makefile $@
