#!/bin/sh

# This Perl Device::SerialPort cross-compiling script is used on Debian 5.0
# autoconf v.2.61
# arm-linux-gcc v4.4.1
# perl v5.10.1 libraries for xscale platform

SOURCE=Device-SerialPort-1.04

if [ ! -d $SOURCE ]; then
	tar xzvf $SOURCE.tar.gz
fi

cd $SOURCE

make distclean

/usr/local/bin/perl Makefile.PL
export 

##### Patch for cross-compiling #####
sed -i 's/AR = ar/AR = arm-linux-ar/g' Makefile
sed -i 's/CC = cc/CC = arm-linux-gcc/g' Makefile
sed -i 's/LD = cc/LD = arm-linux-gcc/g' Makefile
#sed -i 's/OPTIMIZE =/#OPTIMIZE =/g' Makefile
sed -i 's/-fstack-protector//g' Makefile
sed -i 's/usr\/local\/lib/usr\/local\/arm-linux-4.4.2\/lib/g' Makefile
sed -i 's/usr\/local\/include/usr\/local\/arm-linux-4.4.2\/include/g' Makefile
sed -i 's/SITEARCHEXP = \/usr\/local\/arm-linux-4.4.2\/lib\/perl5\/site_perl\/5.10.1\/i686-linux/SITEARCHEXP = \/usr\/local\/arm-linux-4.4.2\/lib\/perl5\/site_perl\/5.10.1\/arm-linux/' Makefile
sed -i 's/INSTALLSITEARCH = \/usr\/local\/arm-linux-4.4.2\/lib\/perl5\/site_perl\/5.10.1\/i686-linux/INSTALLSITEARCH = \/usr\/local\/arm-linux-4.4.2\/lib\/perl5\/site_perl\/5.10.1\/arm-linux/g' Makefile
sed -i 's/usr\/local\/lib\/perl5\/5.10.1\/i686-linux/usr\/local\/arm-linux-4.4.2\/lib\/perl5\/5.10.1\/arm-linux/g' Makefile
sed -i 's/i686-linux/arm-linux/g' Makefile

make
make install DESTDIR="install_me_here"
cd ..
