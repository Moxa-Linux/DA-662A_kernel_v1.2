
PATH1="."
##############################################################
# Linux Kernel 2.6 and 3.0
##############################################################
CC:=$(CC)
LD:=$(LD)
STRIP:=$(STRIP)

all: module npreal2d npreal2d_redund tools
#SP1: module npreal2d npreal2d_redund tools
#ssl: module SSLnpreal2d npreal2d_redund tools
#SP1_ssl: module SSLnpreal2d npreal2d_redund tools
#ssl64: module SSL64npreal2d npreal2d_redund tools
#SP1_ssl64: module SSL64npreal2d npreal2d_redund tools
#ppc64: module ppc64npreal2d npreal2d_redund tools

npreal2d: npreal2d.o
	$(CC) npreal2d.o -o npreal2d
	$(STRIP)	npreal2d

npreal2d.o : npreal2d.c npreal2d.h
	#$(CC) -c npreal2d.c
	$(CC) -DBIG_ENDIAN_PLATFORM -c npreal2d.c

npreal2d_redund: 	redund_main.o redund.o
	$(CC)	redund_main.o redund.o -lpthread -o npreal2d_redund
	$(STRIP)	npreal2d_redund

redund_main.o:	redund_main.c npreal2d.h redund.h
	$(CC) -c redund_main.c

redund.o:	redund.c redund.h npreal2d.h
	$(CC) -c redund.c

SSLnpreal2d: 	SSLnpreal2d.o
	$(CC)	npreal2d.o -o npreal2d libssl.so 
	$(STRIP)	npreal2d

SSLnpreal2d.o:	npreal2d.c
	$(CC) -c -DSSL_ON -DOPENSSL_NO_KRB5 npreal2d.c -I$(PATH1)/include
	
SSL64npreal2d: 	SSL64npreal2d.o
	$(CC)	-m64 npreal2d.o -o npreal2d libssl.so 
	$(STRIP)	npreal2d

SSL64npreal2d.o:	npreal2d.c
	$(CC) -c -m64 -DSSL_ON -DOPENSSL_NO_KRB5 npreal2d.c -I$(PATH1)/include
	
ppc64npreal2d: 	ppc64npreal2d.o
	$(CC)	-mpowerpc64 npreal2d.o -o npreal2d libssl.so 
	$(STRIP)	npreal2d

ppc64npreal2d.o:	npreal2d.c
	$(CC) -c -mpowerpc64 -DSSL_ON -DOPENSSL_NO_KRB5 npreal2d.c -I$(PATH1)/include

ifeq	"$(findstring 2.6.15-1.2054, $(shell uname -r))" "2.6.15-1.2054"
CFLAGS  += -DFEDORA
endif

ifneq ($(KERNELRELEASE),)
obj-m := npreal2.o
else
#KDIR	:= /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)
KDIR	= $(PWD)/../../$(LINUXDIR)
KERNEL_VER = `cat $(ROOTDIR)/$(LINUXDIR)/include/config/kernel.release`

module:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
	#cp -p npreal2.ko /lib/modules/$(shell uname -r)/kernel/drivers/char/
	#depmod -a

endif

tools: mxaddsvr mxdelsvr mxcfmat mxloadsvr mxsetsec

mxaddsvr: mxaddsvr.c
	$(CC) -o mxaddsvr mxaddsvr.c
	$(STRIP) mxaddsvr

mxdelsvr: mxdelsvr.c
	$(CC) -o mxdelsvr mxdelsvr.c
	$(STRIP) mxdelsvr

mxcfmat: mxcfmat.c
	$(CC) -o mxcfmat mxcfmat.c
	$(STRIP) mxcfmat

mxloadsvr: mxloadsvr.c
	$(CC) -o mxloadsvr mxloadsvr.c
	$(STRIP) mxloadsvr
	
mxsetsec: mxsetsec.c
	$(CC) -o mxsetsec mxsetsec.c
	$(STRIP) mxsetsec
	
clean:
	rm -rf *.o \
	./.tmp_versions \
	npreal2.mod* \
	.npreal2* \
	npreal2.ko \
	npreal2d \
	npreal2d_redund \
	/lib/modules/$(shell uname -r)/kernel/drivers/char/npreal2.ko \
	/lib/modules/$(shell uname -r)/misc/npreal2.ko \
	mxaddsvr \
	mxdelsvr \
	mxcfmat \
	mxloadsvr \
	mxsetsec \
	Module.symvers \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin \
	ipkg_$(CONFIGURE_TARGET)-linux/lib

package: module npreal2d npreal2d_redund tools
	$(STRIP) -s npreal2d \
	npreal2d_redund \
	mxaddsvr \
	mxdelsvr \
	mxcfmat \
	mxloadsvr \
	mxsetsec
	mkdir -p ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin \
	ipkg_$(CONFIGURE_TARGET)-linux/lib/modules/$(KERNEL_VER)/kernel/drivers/char 
	cp -a npreal2d \
	npreal2d_redund \
	mxmknod \
	mxrmnod \
	mxaddsvr \
	mxdelsvr \
	mxcfmat \
	mxloadsvr \
	mxsetsec \
	ipkg_$(CONFIGURE_TARGET)-linux/usr/sbin/
	cp -a npreal2.ko ipkg_$(CONFIGURE_TARGET)-linux/lib/modules/$(KERNEL_VER)/kernel/drivers/char/ 
	mkdir -p $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
	$(IPKGBUILD) ipkg_$(CONFIGURE_TARGET)-linux $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)-linux
