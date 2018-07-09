#
# Configuration stuff for nfs-utils
#

VERSION		= nfs-utils 0.3.2

prefix		= $(install_prefix)/usr
exec_prefix	= $(install_prefix)/usr
BINDIR		= ${exec_prefix}/bin
SBINDIR		= ${exec_prefix}/sbin
MANDIR		= ${prefix}/man
STATEDIR	= $(install_prefix)/var/lib/nfs

##################################################################
# This is the prefix that will be used for nfsd and mountd. Leave this
# empty, or set to `k'.
KPREFIX		= 

# This define will turn NFSv3 support on or off one day. Not functional yet.
NFSV3		= yes

# Where and how to install manpages
MAN1EXT		= 1
MAN5EXT		= 5
MAN8EXT		= 8
MAN9EXT		= 9
MANOWNER	= root
MANGROUP	= root

# Various libs
LIBBSD		= 
LIBNSL		= -lnsl
LIBWRAP		= 

################# END OF USER SERVICEABLE PARTS ##################
ALLTARGETS	= all clean distclean install installman \
		  depend dep postscript indent

ifndef ARCHFLAGS
  ARCH		= $(shell uname -m)
  FLAGS_alpha	= -mno-fp-regs -ffixed-8
  ARCHFLAGS	= $(FLAGS_$(ARCH))
.EXPORT:	ARCHFLAGS
endif

BUILD_CC	= gcc
ifdef BUILD
CC		= $(BUILD_CC)
else
CC		= $(CROSS)gcc
endif
AR		= $(CROSS)ar
LD		= $(CROSS)ld
RM		= rm -f
MKDIR		= mkdir -p
LN_S		= ln -sf
RANLIB		= $(CROSS)ranlib
INDENT		= indent
RPCGEN		= $(TOP)bin/rpcgen
GETKVER		= $(TOP)tools/getkversion
INSTALL		= install
MAN2PS		= groff -Tps -man

AFLAGS		= -I$(TOP)support/include \
		  -Wall $(ARCHFLAGS) -pipe
ifdef KERNEL_INCDIR
AFLAGS	       += -I$(KERNEL_INCDIR)
endif

CFLAGS		= -g -O2 -D_GNU_SOURCE $(AFLAGS) $(CCOPTS) -DVERSION="\"$(VERSION)\""
LDFLAGS		=  $(LDOPTS) -L$(TOP)support/lib

ifdef NFSV3
 CFLAGS	+= -DNFS3_SUPPORTED
endif

k		= $(KPREFIX)

INSTALLSCRIPT	= $(INSTALL) -m 755
INSTALLBIN	= $(INSTALL) -m 755
INSTALLSUID	= $(INSTALL) -m 4755
INSTALLMOD	= $(INSTALL) -m 600
INSTALLMAN	= $(INSTALL) -m 644
MAN1DIR		= $(MANDIR)/man$(MAN1EXT)
MAN5DIR		= $(MANDIR)/man$(MAN5EXT)
MAN8DIR		= $(MANDIR)/man$(MAN8EXT)
MAN9DIR		= $(MANDIR)/man$(MAN9EXT)
