#!/bin/sh
CC="arm-linux-gcc -isystem /usr/local/arm-linux-4.4.2/include" CXX=arm-linux-g++ AR=arm-linux-ar STRIP=arm-linux-strip RANLIB=arm-linux-ranlib LD=arm-linux-ld AS=arm-linux-as NM=arm-linux-nm ARCH=arm ./configure --host=arm-linux --with-kernel-headers=`pwd`/../../linux-2.6.x/include --without-readline --with-flexlib=`pwd`/flex/build/libfl.a --prefix=`pwd`/install_me_here
