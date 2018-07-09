##############################################################################
#
#	Makefile -- Master makefile for all libraries.
#

# Jared, 2010-12-29 add for checking gcc version
GCC_VERSION = $(shell $(CC) -v 2>&1 | grep version | cut -d' ' -f3 )

.EXPORT_ALL_VARIABLES:

##############################################################################
#
# Include architecture specific build rules.
#

ifndef ROOTDIR
ROOTDIR=..
endif

# Define  UCLINUX_BUILD_USER=1 for building glibc libraries. Maksed by Jared 07-10-2008
#UCLINUX_BUILD_LIB=1
# Define  UCLINUX_BUILD_USER=1 for building user libraries. Add by Jared 07-10-2008
UCLINUX_BUILD_USER=1
-include $(LINUX_CONFIG)
-include $(CONFIG_CONFIG)
-include $(ARCH_CONFIG)

##############################################################################
#
# always build these dir_
#
#dir_y  = ../include $(LIBCDIR) ../include libnet
dir_y  = libnet
dir_n  =
dir_   =

ifeq ($(LIBCDIR),libc)
dir_y += libm
endif

#
# and build these if someone needs them
#
dir_$(CONFIG_LIB_ALSA)         	  += alsa-lib
dir_$(CONFIG_LIB_FLEX)         	  += flex
dir_$(CONFIG_LIB_FLEX_FORCE)   	  += flex
dir_$(CONFIG_LIB_LIBSSL)       	  += openssl
dir_$(CONFIG_LIB_LIBSSL_FORCE) 	  += openssl
dir_$(CONFIG_LIB_LIBEDIT)         += libedit
dir_$(CONFIG_LIB_LIBGMP)          += libgmp
dir_$(CONFIG_LIB_LIBGMP_FORCE)    += libgmp
dir_$(CONFIG_LIB_LIBPAM)          += Linux-PAM
dir_$(CONFIG_LIB_LIBPAM_FORCE)    += Linux-PAM
dir_$(CONFIG_LIB_LIBPCAP)         += libpcap
dir_$(CONFIG_LIB_LIBPCAP_FORCE)   += libpcap
dir_$(CONFIG_LIB_ZLIB)            += zlib
dir_$(CONFIG_LIB_ZLIB_FORCE)      += zlib
dir_$(CONFIG_LIB_LIBATM)          += libatm
dir_$(CONFIG_LIB_LIBATM_FORCE)    += libatm
dir_$(CONFIG_LIB_LIBPNG)          += libpng
dir_$(CONFIG_LIB_LIBPNG_FORCE)    += libpng
dir_$(CONFIG_LIB_LIBGPG_ERROR_FORCE) += libgpg-error
dir_$(CONFIG_LIB_LIBGCRYPT_FORCE) += libgcrypt
dir_$(CONFIG_LIB_LIBJPEG)         += jpeg
dir_$(CONFIG_LIB_LIBJPEG_FORCE)   += jpeg
dir_$(CONFIG_LIB_LIBNL)           += libnl
dir_$(CONFIG_LIB_LIBUDEV)         += udev
dir_$(CONFIG_LIB_LIBUPNP)         += libupnp
dir_$(CONFIG_LIB_LIBUPNP_FORCE)   += libupnp
dir_$(CONFIG_LIB_STLPORT)         += STLport
dir_$(CONFIG_LIB_STLPORT_FORCE)   += STLport
dir_$(CONFIG_LIB_EXPAT)           += expat
dir_$(CONFIG_LIB_EXPAT_FORCE)     += expat
dir_$(CONFIG_LIB_FREETYPE_FORCE)  += freetype
dir_$(CONFIG_LIB_LIBLDAP)         += libldap
dir_$(CONFIG_LIB_LIBLDAP_FORCE)   += libldap
dir_$(CONFIG_LIB_TINYTCL)         += ../user/tinytcl
dir_$(CONFIG_LIB_LIBCCMALLOC)     += libccmalloc
dir_$(CONFIG_LIB_ADNS_FORCE)      += adns
dir_$(CONFIG_LIB_LZO)             += lzo
dir_$(CONFIG_LIB_READLINE)        += readline
dir_$(CONFIG_LIB_XML2_FORCE)      += libxml2
dir_$(CONFIG_LIB_GD)              += gd
dir_$(CONFIG_LIB_GD_FORCE)        += gd
dir_$(CONFIG_LIB_REDBOOT_CONFIG)  += redboot_config
dir_$(CONFIG_LIB_NET_1_1_5)       += libnet-1.1.5
dir_$(CONFIG_LIB_ICONV)           += libiconv
dir_$(CONFIG_LIB_NCURSES)         += ncurses
dir_$(CONFIG_LIB_NCURSES_FORCE)   += ncurses
dir_$(CONFIG_LIB_PCRE)            += pcre
dir_$(CONFIG_LIB_PCSC_LITE)       += pcsc-lite
dir_$(CONFIG_LIB_LIBFFI)          += libffi
dir_$(CONFIG_LIB_LIBTIRPC)        += libtirpc
dir_$(CONFIG_LIB_GLIB2)           += glib2
dir_$(CONFIG_LIB_EVENTLOG_0_2_12) += eventlog-0.2.12

dir_y += 

# And build libraries in the prop directory last
#dir_y += $(ROOTDIR)/prop


##############################################################################
#
# always build these tc_dir_
#
tc_dir_y =
tc_dir_n =
tc_dir_y += flex
tc_dir_y += expat
tc_dir_y += zlib
tc_dir_y += jpeg
tc_dir_y += libpng
tc_dir_y += libpcap
tc_dir_y += lzo
tc_dir_y += freetype
tc_dir_y += gd
tc_dir_y += libxml2
tc_dir_y += ncurses
tc_dir_y += libiconv
tc_dir_y += openssl
tc_dir_y += Linux-PAM
tc_dir_y += readline
tc_dir_y += dbus
tc_dir_y += libcap-ng

#tc_dir_y += adns
#tc_dir_y += alsa-lib
#tc_dir_y += bluez-libs
#tc_dir_y += libatm
#tc_dir_y += libccmalloc
#tc_dir_y += libcipher
#tc_dir_y += libgcrypt
#tc_dir_y += libgmp
#tc_dir_y += libgpg-error
#tc_dir_y += libldap
#tc_dir_y += libm
#tc_dir_y += libnet
#tc_dir_y += libupnp
#tc_dir_y += libusb
#tc_dir_y += redboot_config
#tc_dir_y += STLport


##############################################################################
#PRINCE ADD 08082006 FOR lib/glibc symbolic link
#Jared Masked 07102008 and re-write a new one for rebuilding the libraries
#all:
##	if [ ! -L $(LIBCDIR) ]; then \
##		ln -s ../$(LIBCDIR) $(LIBCDIR); \
##	fi
##	for i in $(dir_y) ; do \
##		[ ! -d $$i ] || make -C $$i || exit $$? ; \
##		$(MAKE) make_links ; \
##			if [ "$$i" == "$(LIBCDIR)" ] ; then \
##				$(MAKE) -C ../include || exit $?; \
##			fi \
##	done
#############################################################################

# Jared, these have moved to define in vendors/PROCUCT/MODULE/config.arch
#ifeq ("$(CROSS)","xscale-linux-")
#	TC_LIBDIR=$(TOOLCHAIN_DIR)/lib/be
#else
#ifeq ("$(CROSS)","arm-linux-")
#	TC_LIBDIR=$(TOOLCHAIN_DIR)/lib
#endif
#endif
#
#ifeq ("$(CROSS)","xscale-none-linux-gnueabi-")
#	TC_LIBDIR=$(TC_INSTALL_PREFIX)/lib
#else
#ifeq ("$(CROSS)","arm-none-linux-gnueabi-")
#	TC_LIBDIR=$(TC_INSTALL_PREFIX)/lib
#endif
#endif

GLIBC_LIB= \
	$(TC_LIBDIR)/ld-*.so \
	$(TC_LIBDIR)/libc-*.so \
	$(TC_LIBDIR)/ld-linux.so.* \
	$(TC_LIBDIR)/libc.so* \
	$(TC_LIBDIR)/libcrypt*.so* \
	$(TC_LIBDIR)/libcrypt-*.so \
	$(TC_LIBDIR)/libdl*.so* \
	$(TC_LIBDIR)/libdl-*.so \
	$(TC_LIBDIR)/libm-*.so \
	$(TC_LIBDIR)/libm.so* \
	$(TC_LIBDIR)/libnsl.so* \
	$(TC_LIBDIR)/libnsl-*.so \
	$(TC_LIBDIR)/libnss_dns.so* \
	$(TC_LIBDIR)/libnss_dns-*.so \
	$(TC_LIBDIR)/libnss_files.so* \
	$(TC_LIBDIR)/libnss_files-*.so \
	$(TC_LIBDIR)/libpthread.so.* \
	$(TC_LIBDIR)/libpthread-*.so \
	$(TC_LIBDIR)/libresolv.so* \
	$(TC_LIBDIR)/libresolv-*.so \
	$(TC_LIBDIR)/librt-*.so \
	$(TC_LIBDIR)/librt.so* \
	$(TC_LIBDIR)/libthread_db*.so* \
	$(TC_LIBDIR)/libutil.so* \
	$(TC_LIBDIR)/libutil-*.so \
	$(TC_LIBDIR)/libncurses.so.5* \
	$(TC_LIBDIR)/liblzo2.so*

GNUC_LIB= \
	$(TC_LIBDIR)/libgcc_s.so.1 \
	$(TC_LIBDIR)/libstdc++.so*


#Jared, add for rebuilding all the libraries defined in dir_y. 07-10-2008
# only build those not in toolchain
all: 
	for i in $(dir_y); do \
		if [ "`echo $(tc_dir_y) | grep $$i`" = "" ]; then \
			[ ! -d $$i ] || make -C $$i || exit $$? ; \
		fi \
	done

##############################################################################
# build all required
build:
	for i in $(dir_y) ; do \
		[ ! -d $$i ] || make -C $$i || exit $$? ; \
	done

##############################################################################
# copy libraries from builded 
romfs-builded:
	for i in $(dir_y) ; do \
		[ ! -d $$i ] || make -C $$i romfs || exit $$? ; \
	done
	for i in $(GLIBC_LIB); do \
		cp -a $$i $(ROMFSDIR)/lib; \
	done
	find $(ROMFSDIR)/lib/ -regex "\S+\.so\(\.[0-9]*\)*" | xargs file | \
		grep 'ELF' | cut -d: -f1 | xargs $(STRIP)
	#$(STRIP) $(ROMFSDIR)/lib/*.so.*

##############################################################################
# build and install into toolchain
toolchain:
	for i in $(tc_dir_y) ; do \
		if [ -d $$i ]; then \
			make -C $$i && make -C $$i install || exit $$? ; \
			echo ----- $$i: finished -----------------------------------; \
		fi \
	done

##############################################################################
# copy from toolchain if any, build if none
romfs:
	for i in $(dir_y); do \
		if [ -n "`echo $(tc_dir_y) | grep $$i`" ]; then \
			make -C $$i tc2romfs; \
		else \
			make -C $$i romfs; \
		fi \
	done
	for i in $(GLIBC_LIB); do \
		cp -a $$i $(ROMFSDIR)/lib; \
	done
	for i in $(GNUC_LIB); do \
		cp -a $$i $(ROMFSDIR)/lib; \
	done
	find $(ROMFSDIR)/lib/ -regex "\S+\.so\(\.[0-9]*\)*" | xargs file | \
		grep 'ELF' | cut -d: -f1 | xargs $(STRIP)
	#$(STRIP) $(ROMFSDIR)/lib/*.so.*

##############################################################################

clean:
	-for i in $(dir_y) $(dir_n) $(dir_); do \
		[ ! -d $$i ] || make -C $$i clean ; \
	done

distclean: 
	-for i in $(dir_y) $(dir_n) $(dir_); do \
		[ ! -d $$i ] || make -C $$i distclean ; \
	done

tc-distclean: 
	-for i in $(tc_dir_y); do \
		[ ! -d $$i ] || make -C $$i distclean ; \
	done

#clean: clean_links
#	-for i in $(dir_y) $(dir_n) $(dir_); do \
#		[ ! -d $$i ] || make -C $$i clean ; \
#	done
#	rm -rf $(LIBCDIR)
#	rm -rf *.so
#	rm -rf *.so.*
#	rm -rf *.a

##############################################################################
#
# The libs we link into this directory
#

LINKLIBS=\
	$(ROOTDIR)/lib/$(LIBCDIR)/build/lib/crt*.o \
	$(ROOTDIR)/lib/$(LIBCDIR)/build/lib/*.gdb \
	$(ROOTDIR)/lib/$(LIBCDIR)/build/lib/*.so* \
	$(ROOTDIR)/lib/$(LIBCDIR)/build/lib/*.a \
	$(ROOTDIR)/lib/$(LIBCDIR)/build/crt*.o \
	$(ROOTDIR)/lib/$(LIBCDIR)/build/*.gdb \
	$(ROOTDIR)/lib/$(LIBCDIR)/build/*.so* \
	$(ROOTDIR)/lib/$(LIBCDIR)/build/*.a \
	$(ROOTDIR)/lib/$(LIBCDIR)/lib/crt*.o \
	$(ROOTDIR)/lib/$(LIBCDIR)/lib/*.gdb \
	$(ROOTDIR)/lib/$(LIBCDIR)/lib/*.so* \
	$(ROOTDIR)/lib/$(LIBCDIR)/lib/*.a \
	$(ROOTDIR)/lib/$(LIBCDIR)/crt*.o \
	$(ROOTDIR)/lib/$(LIBCDIR)/*.gdb \
	$(ROOTDIR)/lib/$(LIBCDIR)/*.so* \
	$(ROOTDIR)/lib/$(LIBCDIR)/*.a \
	$(ROOTDIR)/lib/libm/*.a \
	$(ROOTDIR)/lib/libnet/*.a \
	$(ROOTDIR)/lib/libpcap/*.a \
	$(ROOTDIR)/lib/zlib/*.a $(ROOTDIR)/lib/zlib/*.so* \
	$(ROOTDIR)/lib/libatm/src/lib/*.so* $(ROOTDIR)/lib/libatm/src/lib/*.a \
	$(ROOTDIR)/lib/libcrypt_old/libcrypt_old.a \
	$(ROOTDIR)/lib/libldap/builddir/libraries/libldap/.libs/*.a \
	$(ROOTDIR)/lib/libldap/builddir/libraries/liblber/.libs/*.a \
	$(ROOTDIR)/prop/libsnapgear++/*.a $(ROOTDIR)/prop/libsnapgear++/*.so* \
	$(ROOTDIR)/prop/libsnapgear/*.a $(ROOTDIR)/prop/libsnapgear/*.so* \
	$(ROOTDIR)/user/tinytcl/*.a $(ROOTDIR)/user/tinytcl/*.so* \
	$(ROOTDIR)/prop/config/*.a $(ROOTDIR)/prop/config/*.so* \
	$(ROOTDIR)/prop/uberconfig/*.a $(ROOTDIR)/prop/uberconfig/*.so* \
	$(ROOTDIR)/prop/gccmapping/*.a $(ROOTDIR)/prop/gccmapping/*.so* \
	$(ROOTDIR)/lib/expat/*.a $(ROOTDIR)/lib/expat/*.so* \
	$(ROOTDIR)/lib/libpng/*.a $(ROOTDIR)/lib/libpng/*.so* \
	$(ROOTDIR)/lib/libjpeg/*.a $(ROOTDIR)/lib/libjpeg/*.so* \
	$(ROOTDIR)/lib/libupnp/upnp/bin/*.so* $(ROOTDIR)/lib/adns/src/libadns.a \
	$(ROOTDIR)/lib/libgmp/*.a \
	$(ROOTDIR)/lib/libssl/*.so* \
	$(ROOTDIR)/lib/libiconv.so* \
#	$(ROOTDIR)/lib/libssl/*.a \


##############################################################################
make_links: clean_links
	@echo "Making symlinks in lib/"
	@for p in $(LINKLIBS); do \
		for i in $$p; do \
			[ ! -e "$$i" ] && continue; \
			[ "`basename $$i`" = CVS ] && continue; \
			[ -L "./`basename $$i`" ] && continue; \
			ln -s $$i .; \
		done ; \
	done

clean_links:
	@-for i in *; do \
		[ "$$i" = "Makefile" ] && continue; \
		[ -L "$$i" ] || continue ; \
		case "`file -L $$i`" in \
		*directory*) ;; \
		*) rm -f $$i ;; \
		esac ; \
	done

##############################################################################
package:
	for i in $(dir_y) ; do \
		[ ! -d $$i ] || make -C $$i package || exit $$? ; \
	done


%_only:
	make -C $(@:_only=)

%_romfs %_debianfs %_package %_menuconfig %_clean:
	[ ! -d "$(word 1,$(subst _, ,$@))" ] || \
		$(MAKEARCH) -C $(word 1,$(subst _, ,$@)) \
		$(word 2,$(subst _, ,$@)); \

