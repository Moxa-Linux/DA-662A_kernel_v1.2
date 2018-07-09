MIB_MODULE=uc1260
SHAREDOBJS=$(MIB_MODULE).so
DLFLAGS=-fPIC
LDFLAGS=-L$(TC_INSTALL_PREFIX)/lib
CFLAGS=-I../../../build_$(CONFIGURE_TARGET)-linux/include \
	   -I../../../build_$(CONFIGURE_TARGET)-linux/agent \
	   -I../../../build_$(CONFIGURE_TARGET)-linux/agent/mibgroup \
	   -I../../../build_$(CONFIGURE_TARGET)-linux/snmplib \
	   -I. -g -O2 -Ulinux -Dlinux=linux -Wall -fno-strict-aliasing

all: $(MIB_MODULE).o
	$(CC) -shared -o $(SHAREDOBJS) $(MIB_MODULE).o $(LDFLAGS)
	$(STRIP) -s $(SHAREDOBJS)

$(MIB_MODULE): $(MIB_MODULE).c $(MIB_MODULE).h
	$(CC) $(CFLAGS) $(DLFLAGS) -c ${MIB_MODULE}.c

clean:
	@rm -f *.o *.so

