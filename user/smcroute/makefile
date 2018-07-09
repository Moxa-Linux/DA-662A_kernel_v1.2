# History:
# Date		Author			Comment
# 09-03-2010	Jared Wu.		
#
# compile: 
#	make user/smcroute_only

CONFIGURE_TARGET := $(strip $(subst -, ,$(CROSS)))
CONFIGURE_TARGET := $(word 1,$(CONFIGURE_TARGET))-$(word 2,$(CONFIGURE_TARGET))

all: build_$(CONFIGURE_TARGET)/src/smcroute

build_$(CONFIGURE_TARGET)/src/smcroute:
	rm -rf build_$(CONFIGURE_TARGET)
	tar xzvf smcroute-0.93d.tar.gz
	mv smcroute-0.93d build_$(CONFIGURE_TARGET)
	unset CFLAGS CPPFLAGS LDFLAGS CXXFLAGS && \
	make -C build_$(CONFIGURE_TARGET)/src CC=$(CC) LD=$(CC) -j 2;

romfs:
	echo 'nothing'

package:
	mkdir -p ipkg_src_root_$(CONFIGURE_TARGET)/usr/sbin
	cp -a build_$(CONFIGURE_TARGET)/src/smcroute ipkg_src_root_$(CONFIGURE_TARGET)/usr/sbin/
	$(STRIP) ipkg_src_root_$(CONFIGURE_TARGET)/usr/sbin/*
	$(IPKGBUILD) ipkg_src_root_$(CONFIGURE_TARGET) $(IPKGDIR)/ipkg_$(CONFIGURE_TARGET)


clean distclean:
	rm -fr build_$(CONFIGURE_TARGET)
	rm -fr ipkg_src_root_$(CONFIGURE_TARGET)/usr/sbin/*
