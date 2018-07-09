# History:
# Date		Author			Comment
# 11-7-2011	Jared Wu.		Build the gdp host utility
#
# Pre-requirement:
# apt-get install libncurses5-dev
#	None

TARGET_SRC=gdb-6.8
HOST=arm-linux

all: $(TARGET_SRC)/gdb/gdb
	echo "Build gdb Complete!"

install:
	make -C $(TARGET_SRC) install
	echo "Install gdb Complete!"

$(TARGET_SRC)/gdb/gdb: $(TARGET_SRC)/Makefile
	make -C $(TARGET_SRC)
	echo "Build gdb Complete!"

$(TARGET_SRC)/Makefile:
	rm -rf $(TARGET_SRC)
	tar xjvf $(TARGET_SRC).tar.bz2
	( cd $(TARGET_SRC); \
	patch -p1 -i ../gdb-6.8_eval.patch; \
	./configure \
	--build=i686 \
	--host=i686 \
	--target="${HOST}" \
	--prefix="$(INSTALL_ROOT)" \
	--program-prefix="${HOST}-" \
	)
	touch $(TARGET_SRC)/configured


clean distclean: 
	rm -rf $(TARGET_SRC)

