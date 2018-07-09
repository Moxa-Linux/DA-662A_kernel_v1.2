#CC = arm-linux-gcc
#CFLAGS = -g -O2
#LDFLAGS += -L./include
#CPP = xscale-linux-gcc -E
#CPPFLAGS = 
#CXX = arm-linux-g++
#CXXCPP = arm-linux-g++ -E
#RANLIB = arm-linux-ranlib
#VERSION = 0.1

INSTALL_ROOT=$(TC_INSTALL_PREFIX)

all:
	make -C src INSTALL_ROOT=$(INSTALL_ROOT)
	make -C example/testing INSTALL_ROOT=$(INSTALL_ROOT)
	make -C example/utility INSTALL_ROOT=$(INSTALL_ROOT)
	make -C daemon INSTALL_ROOT=$(INSTALL_ROOT)

clean:
	make -C src clean
	make -C example/testing clean
	make -C example/utility clean
	make -C daemon  clean

romfs:
	make -C src romfs INSTALL_ROOT=$(INSTALL_ROOT)
	make -C example/utility romfs INSTALL_ROOT=$(INSTALL_ROOT)
	make -C daemon romfs INSTALL_ROOT=$(INSTALL_ROOT)

package:
