include ../nasl.tmpl
NESSUS_CFLAGS= -I../include $(OPTIMIZE) -DNESSUS_ON_SSL -DHAVE_SSL -DNESSUS_EXTENSIONS
NESSUS_CFLAGS+= -I$(ROOTDIR)/user/nessus/nessus-libraries/include
NESSUS_LIBS := $(LIBS) -lnessus -lhosts_gatherer $(LIBPCAP) -lutil -lnsl $(LIBRESOLV) $(LIBSSL) $(LIBCRYPTO) -ldl
NESSUS_LIBS += -L$(ROOTDIR)/user/nessus/lib

NESSUS_INCLUDE=`sh ./cflags`

YACC = bison 
YFLAGS	= -d -v -t -p nasl

OBJS = \
	nasl_packet_forgery.o \
	nasl_socket.o \
	nasl_crypto.o \
	nasl_http.o \
	nasl_host.o \
	nasl_text_utils.o \
	nasl_nessusd_glue.o \
	nasl_misc_funcs.o \
	capture_packet.o \
	nasl_grammar.tab.o \
	nasl_tree.o \
	nasl_var.o \
	exec.o \
	nasl_lex_ctxt.o \
	nasl_func.o \
	nasl_init.o \
	strutils.o \
	regex.o \
	lfind.o \
	smb_crypt.o \
	smb_crypt2.o \
	md5.o \
	hmacmd5.o \
	nasl_debug.o
LO_OBJS = \
	nasl_packet_forgery.lo \
	nasl_socket.lo \
	nasl_crypto.lo \
	nasl_http.lo \
	nasl_host.lo \
	nasl_text_utils.lo \
	nasl_nessusd_glue.lo \
	nasl_misc_funcs.lo \
	capture_packet.lo \
	nasl_grammar.tab.lo \
	nasl_tree.lo \
	nasl_var.lo \
	exec.lo \
	nasl_lex_ctxt.lo \
	nasl_func.lo \
	nasl_init.lo \
	strutils.lo \
	regex.lo \
	lfind.lo \
	smb_crypt.lo \
	smb_crypt2.lo \
	md5.lo \
	hmacmd5.lo \
	nasl_debug.lo

CSRCS = \
	nasl_packet_forgery.c \
	nasl_socket.c \
	nasl_crypto.c \
	nasl_http.c \
	nasl_host.c \
	nasl_text_utils.c \
	nasl_nessusd_glue.c \
	nasl_misc_funcs.c \
	capture_packet.c \
	nasl_grammar.tab.c \
	nasl_tree.c \
	nasl_var.c \
	exec.c \
	nasl_lex_ctxt.c \
	nasl_func.c \
	nasl_init.c \
	strutils.c \
	regex.c \
	lfind.c \
	smb_crypt.c \
	smb_crypt2.c \
	md5.c \
	hmacmd5.c \
	nasl_debug.c

COMPILE = $(LIBTOOL) $(CC) $(CFLAGS) $(NESSUS_INCLUDE)
all : cflags libnasl.la nasl

.c.o:
	$(COMPILE) -c $<
cflags : 
	@echo "echo \"$(NESSUS_CFLAGS) $(include) $(DEFS) \"" > cflags
	@chmod +x cflags

nasl : cflags $(OBJS) nasl.c
	$(CC) $(CFLAGS)  $(NESSUS_INCLUDE) $(include) -c -DVERSION=\"$(VERSION)\" nasl.c
	$(CC) $(LDFLAGS) -o nasl nasl.o $(OBJS) $(NESSUS_LIBS) $(LDLIBS)
	

libnasl.la : cflags $(OBJS)
	$(COMPILE) $(NESSUS_LIBS) -o libnasl.la $(LO_OBJS) -rpath ${libdir} \
	-version-info ${PACKAGE_VERSION}

install : libnasl.la nasl
	test -d $(DESTDIR)${libdir} || $(INSTALL_DIR) -m 755 $(DESTDIR)${libdir}
	$(LIBTOOL) --mode install $(INSTALL) libnasl.la $(DESTDIR)${libdir}/libnasl.la
	$(LIBTOOL) --finish $(DESTDIR)${libdir}
	test -d $(DESTDIR)${bindir} || $(INSTALL_DIR) -m 755 $(DESTDIR)${bindir}
	$(INSTALL) nasl $(DESTDIR)${bindir}/


nasl_grammar.tab.o: nasl_grammar.tab.h nasl_grammar.tab.c nasl_tree.h
	$(COMPILE) -c nasl_grammar.tab.c

nasl_grammar.tab.h: nasl_grammar.tab.c nasl_grammar.y

nasl_grammar.tab.c: nasl_grammar.y
	$(YACC) $(YFLAGS) nasl_grammar.y

nasl_init.o: nasl_init.c
	 $(COMPILE) -c -DVERSION=\"$(VERSION)\" nasl_init.c

nasl_func.o: nasl_func.c nasl_var.h

nasl_var.o: nasl_var.c nasl_var.h

exec.o: exec.c exec.h nasl_tree.h nasl_global_ctxt.h nasl_func.h nasl_var.h nasl_lex_ctxt.h nasl_init.h strutils.h

distclean : clean
	@rm -f strutils.h

clean :
	@rm -f OBJ/*.*
	@rm -f *~
	@rm -f *.o *.lo *.la
	@rm -rf .libs
	@rm -f nasl *core cflags
	@rm -f y.tab.h y.tab.c nasl_grammar.tab.c nasl_grammar.tab.h lex.yy.clex.yy.h y.output nasl_grammar.output
