
all: build/ethtool

build/ethtool: build/Makefile
	$(MAKE) -C build

build/Makefile:
	rm -rf build
	mkdir build
	(cd build; \
	CC="$(CC) \
	$(CFLAGS)" \
	LDFLAGS="$(LDFLAGS)" \
	LIBS="$(LDLIBS)" \
	../configure \
	--host=$(CONFIGURE_TARGET) \
	--build=i686 \
	)

romfs:
	$(STRIP) -s build/ethtool
	$(ROMFSINST) build/ethtool /bin/ethtool

package:
	echo "do nothing"

clean:
	rm -rf build

