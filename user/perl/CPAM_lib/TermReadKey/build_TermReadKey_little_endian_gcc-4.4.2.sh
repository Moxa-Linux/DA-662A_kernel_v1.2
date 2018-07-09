#!/bin/sh

# This Perl Device::SerialPort cross-compiling script is used on Debian 5.0
# autoconf v.2.61
# arm-linux-gcc v4.4.2
# perl v5.10.1 libraries for xscale platform

export PATH=/usr/local/bin/:$PATH
SOURCE=TermReadKey-2.30

if [ ! -d $SOURCE ]; then
	tar xzvf $SOURCE.tar.gz
fi

cd $SOURCE

[ -f Makefile ] && make distclean

perl Makefile.PL

##### Patch for cross-compiling #####
sed -i 's/AR = ar/AR = arm-linux-ar/g' Makefile
sed -i 's/CC = cc/CC = arm-linux-gcc/g' Makefile
sed -i 's/LD = cc/LD = arm-linux-gcc/g' Makefile
#sed -i 's/OPTIMIZE =/#OPTIMIZE =/g' Makefile
sed -i 's/-fstack-protector//g' Makefile
sed -i 's/usr\/local\/lib/usr\/local\/arm-linux\/lib/g' Makefile
sed -i 's/usr\/local\/include/usr\/local\/arm-linux\/include/g' Makefile
sed -i 's/SITEARCHEXP = \/usr\/local\/arm-linux\/lib/perl5\/site_perl\/5.10.1\/i686-linux/SITEARCHEXP = \/usr\/local\/arm-linux\/lib\/perl5\/site_perl\/5.10.1\/armv4tl/' Makefile
sed -i 's/usr\/local\/arm-linux\/lib\/perl5\/5.10.1\/i686-linux/usr\/local\/lib\/perl5\/5.10.1\/i686-linux/g' Makefile
sed -i 's/INSTALLSITEARCH = \/usr\/local\/arm-linux\/lib\/perl5\/site_perl\/5.10.1\/i686-linux/INSTALLSITEARCH = \/usr\/local\/arm-linux\/lib\/perl5\/site_perl\/5.10.1\/armv4tl/g' Makefile

make
make install DESTDIR="install_me_here"

cd ..
