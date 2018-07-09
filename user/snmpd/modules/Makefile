OBJS = icmp_vars.o ip_vars.o tcp_vars.o udp_vars.o systm.o iface_vars.o mxser_vars.o at_vars.o

LIBRARY = libmodules.a

CFLAGS += -I../snmplib

all: $(LIBRARY)

test:

romfs:

$(LIBRARY): $(OBJS)
	$(AR) rcv $@ $(OBJS)
	-$(RANLIB) $@

clean::
	$(RM) $(OBJS) $(LIBRARY)
