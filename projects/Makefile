
.EXPORT_ALL_VARIABLES:

-include $(PROJECTS_CONFIG)
export DESTDIR=$(ROMFSDIR)


prj-$(CONFIG_PROJECT_GPSDO)	+= gpsdo


all:
	for i in $(prj-y); do \
		[ ! -d $$i ] || make -C $$i || exit $$? ; \
	done

romfs:
	for i in $(prj-y); do \
		[ ! -d $$i ] || make -C $$i romfs || exit $$? ; \
	done

install:
	for i in $(prj-y); do \
		[ ! -d $$i ] || make -C $$i install || exit $$? ; \
	done

clean:
	for i in $(prj-y); do \
		[ ! -d $$i ] || make -C $$i clean || exit $$? ; \
	done

oldconfig:
	@$(CONFIG_SHELL) -n config.in
	@HELP_FILE=Configure.help \
		AUTOCONF_FILE=autoconf.h \
		$(CONFIG_SHELL) $(SCRIPTSDIR)/Configure -d config.in

config:
	@$(CONFIG_SHELL) -n config.in
	@HELP_FILE=Configure.help \
		AUTOCONF_FILE=autoconf.h \
		$(CONFIG_SHELL) $(SCRIPTSDIR)/Configure config.in

menuconfig:
	@$(CONFIG_SHELL) -n config.in
	$(MAKE) -C $(SCRIPTSDIR)/lxdialog all
	@HELP_FILE=Configure.help \
		AUTOCONF_FILE=autoconf.h \
		$(CONFIG_SHELL) $(SCRIPTSDIR)/Menuconfig config.in


%_only:
	make -C $(@:_only=)

%_romfs %_debianfs %_package %_menuconfig %_clean:
	[ ! -d "$(word 1,$(subst _, ,$@))" ] || \
		$(MAKEARCH) -C $(word 1,$(subst _, ,$@)) \
		$(word 2,$(subst _, ,$@)); \

