# History:
# Date          Author			Comment
# 09-05-2013	ChinFu Yang		First Create
#
# compile:
#	make user/logrotate-3.8.6_only
#

PACKAGE_NAME = logrotate-3.8.6
EXEC = logrotate

all: build_$(CONFIGURE_TARGET)-linux/$(EXEC)

build_$(CONFIGURE_TARGET)-linux/$(EXEC): build_$(CONFIGURE_TARGET)-linux/configured
	$(MAKE) -C build_$(CONFIGURE_TARGET)-linux
	echo "Build $(EXEC) bomplete!"

build_$(CONFIGURE_TARGET)-linux/configured:
	rm -rf build_$(CONFIGURE_TARGET)-linux
	tar xzvf $(PACKAGE_NAME).tar.gz
	mv logrotate-3.8.6 build_$(CONFIGURE_TARGET)-linux
	( cd build_$(CONFIGURE_TARGET)-linux; \
	unset CXX CXXCPP CXXCPP CFLAGS CPPFLAGS LDFLAGS CXXFLAGS PACKAGE_STRING && \
	CC=$(CROSS)gcc \
	STRIP=$(CROSS)strip \
	CXX=$(CROSS)gcc \
	LD=$(CROSS)ld \
	make \
	)
	touch build_$(CONFIGURE_TARGET)-linux/configured

romfs:
	$(ROMFSINST) -v build_$(CONFIGURE_TARGET)-linux/$(EXEC) /bin/$(EXEC)
	$(CROSS)strip $(ROMFSDIR)/bin/$(EXEC)
	$(ROMFSINST) -d 755 /var/lib/logrotate

clean:
	/bin/rm -rf build_$(CONFIGURE_TARGET)-linux \

distclean:
	/bin/rm -rf build_$(CONFIGURE_TARGET)-linux \

