# History:
# Date		Author			Comment
# 06-12-2009	Jared Wu.		install at BERoot or LERoot
# 12-29-2011	Aeluin Chen.	do not install at INSTALL_ROOT, instead of TC_INSTALL_PREFIX
#
# compile: 
#	make lib/libnet_only
#

CFLAGS += -isystem$(TC_INSTALL_PREFIX)/include

SRC = net.c
OBJS = openraw.o closeraw.o setifflags.o resetifflags.o setifaddr.o \
	setifdstaddr.o setifbrdaddr.o setifnetmask.o setifmtu.o \
	addroute.o maskaddress.o setifhwaddr.o

LIBNET = libnet.a


all: $(TC_INSTALL_PREFIX)/lib/$(LIBNET)
	@echo "libnet has installed at $(TC_INSTALL_PREFIX)/lib/$(LIBNET)"

$(TC_INSTALL_PREFIX)/lib/$(LIBNET):$(LIBNET)
	mkdir -p build_$(CONFIGURE_TARGET)-linux
	cp -a $(LIBNET) $(TC_INSTALL_PREFIX)/lib
	@echo "Install libnet finish"

$(LIBNET): $(LIBNET)($(OBJS))
	$(RANLIB) $(LIBNET)

$(LIBNET)($(OBJS)): $(SRC)
	$(CC) $(CFLAGS) -DL_$(basename $*) -o $(basename $*).o -c $^
	$(AR) rv $(LIBNET) $(basename $*).o

romfs:

clean:
	-rm -f $(LIBNET) *.o

distclean: clean
	-rm -rf build_*

package:
