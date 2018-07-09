#.EXPORT_ALL_VARIABLES:

#CXXFLAGS += $(INCSG)
#CFLAGS += $(INCSG)

-include $(ARCH_CONFIG)

CFLAGS := $(VENDOR_CFLAGS)

DIRS = snmplib modules snmpd

all romfs test:
	for i in $(DIRS) ; do make -C $$i $@ || exit $?; done

package:

clean:
	for i in $(DIRS) ; do make -C $$i clean ; done

