
.EXPORT_ALL_VARIABLES:

TARGET :=  # $(CROSS:-=)
DEBUG  := 0
TOOLS  := 1

CFLAGS += -fPIC
LDLIBS := -nodefaultlibs -nostdlib -L${ROOTDIR}/lib -lc -lgcc

all:
	@cd upnp && $(MAKE)
	-@if [ ! -d ./upnp/inc/upnp ]; then mkdir ./upnp/inc/upnp; fi
	-@cp -af ./upnp/inc/*.h ./upnp/inc/upnp/

romfs:
	$(ROMFSINST) upnp/bin/libixml.so /lib/libixml.so
	$(ROMFSINST) upnp/bin/libthreadutil.so /lib/libthreadutil.so
	$(ROMFSINST) upnp/bin/libupnp.so /lib/libupno.so
#	$(ROMFSINST) upnp/bin/$(TARGET)/libixml.so /lib 
#	$(ROMFSINST) upnp/bin/$(TARGET)/libthreadutil.so /lib 
#	$(ROMFSINST) upnp/bin/$(TARGET)/libupnp.so /lib

clean:
	@cd upnp && $(MAKE) clean
	-@cd ./upnp/inc; rm -rf ./upnp/* 
	-@rm -rf ./upnp/bin/$(TARGET)

package:
