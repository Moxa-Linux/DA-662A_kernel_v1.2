#CC = arm-linux-gcc
CFLAGS = -g -O2 -I.. -I../include
#LDFLAGS = -fPIC
#CPP = xscale-linux-gcc -E
#CPPFLAGS = 
#CXX = arm-linux-g++
#CXXCPP = arm-linux-g++ -E
#RANLIB = arm-linux-ranlib
#AR= arm-linux-ar
VERSION = 0.1
LIB=libcellular_modem.a
MODULES=exec.o debug.o serial.o misc.o cellular_modem.o
PRESHARELIB=libsms.so
SONAME=${PRESHARELIB}.1
SHARELIB=${PRESHARELIB}.1.0
STATICLIB=libsms.1.0.a

all: ${MODULES}
	${AR} -cvq ${STATICLIB} ${MODULES}
	#cp ${STATICLIB} $(INSTALL_ROOT)/lib/${STATICLIB}
	${CC} -shared -Wl,-soname,${SONAME} -o ${PRESHARELIB} ${MODULES}
	cp ${PRESHARELIB} $(INSTALL_ROOT)/lib/${PRESHARELIB}
	#cp ${PRESHARELIB} ${STATICLIB} /usr/local/arm-linux/lib
	#ln -sf ${PRESHARELIB} ${SHARELIB}
	#ln -sf ${SHARELIB} ${SONAME}

cellular_modem.o: cellular_modem.c
	${CC} ${CFLAGS} ${LDFLAGS} -c cellular_modem.c

debug.o: debug.c ../include/debug.h
	${CC} ${CFLAGS} ${LDFLAGS} -c debug.c

misc.o: misc.c 
	${CC} ${CFLAGS} ${LDFLAGS} -c misc.c

serial.o: serial.c 
	${CC} ${CFLAGS} ${LDFLAGS} -c serial.c

exec.o: exec.c 
	${CC} ${CFLAGS} ${LDFLAGS} -c exec.c

clean:
	rm -rf ${MODULES}
	rm -rf ${STATICLIB}
	rm -rf ${SHARELIB}
	rm -rf ${SONAME}
	rm -rf ${PRESHARELIB}

romfs:
	$(ROMFSINST) $(INSTALL_ROOT)/lib/$(PRESHARELIB) /lib/$(PRESHARELIB)
	$(STRIP) -s $(ROMFSDIR)/lib/$(PRESHARELIB)
	(cd $(ROMFSDIR)/lib/; \
	ln -sf ${PRESHARELIB} ${SHARELIB}; \
	ln -sf ${SHARELIB} ${SONAME}; \
	)

package:

