## $Id: Makefile.am,v 1.22 2003/05/19 18:08:02 chrisgreen Exp $
AUTOMAKE_OPTIONS=foreign no-dependencies

bin_PROGRAMS = snort

snort_SOURCES = cdefs.h \
event.h \
fatal.h \
generators.h \
prototypes.h \
plugin_enum.h \
rules.h \
sys_include.h \
checksum.h \
codes.c codes.h \
debug.c debug.h \
decode.c decode.h \
log.c log.h \
mstring.c mstring.h \
parser.c parser.h \
plugbase.c plugbase.h \
snort.c snort.h \
snprintf.c snprintf.h \
strlcatu.c strlcatu.h \
strlcpyu.c strlcpyu.h \
tag.c tag.h \
ubi_BinTree.c ubi_BinTree.h \
ubi_SplayTree.c ubi_SplayTree.h \
util.c util.h \
detect.c detect.h \
signature.c signature.h \
mempool.c mempool.h \
sf_sdlist.c sf_sdlist.h \
threshold.c threshold.h \
bitop.h \
fpcreate.c fpcreate.h \
fpdetect.c fpdetect.h \
mwm.c mwm.h \
mpse.c mpse.h \
acsmx.c acsmx.h \
pcrm.c pcrm.h \
sfksearch.c sfksearch.h \
bounds.h \
byte_extract.c \
byte_extract.h \
timersub.h \
spo_plugbase.h

snort_LDADD = output-plugins/libspo.a \
detection-plugins/libspd.a            \
preprocessors/libspp.a                \
parser/libparser.a

SUBDIRS = win32 output-plugins detection-plugins preprocessors parser

INCLUDES = @INCLUDES@
