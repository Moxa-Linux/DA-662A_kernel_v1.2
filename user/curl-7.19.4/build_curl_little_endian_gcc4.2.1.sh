#/bin/sh
#
# This script is created by jared.wu@moxa.com
# For cross compiling this package automatically on Debian Etch

# Define the parameters for cross compiling
SOURCE=curl-7.19.4
INSTALL_ROOT=`pwd`/../installroot_little_endian
PACKAGE_DIR=package_root_little_endian
ARCH=arm-unknown-linux
CC=arm-linux-gcc
CPP=arm-linux-cpp
LD=arm-linux-ld
STRIP=arm-linux-strip

configure() {

	CC=${CC} \
	CPP=${CPP} \
	LD=${LD} \
	./configure \
	--build=i686\
	--host=${ARCH} \
	--with-static \
	--prefix=${INSTALL_ROOT} \
	--without-ssl \
	--without-libidn
 
#	--with-ipkglibdir="/home/ipkg"

}

build_package() {

	[ ! -d ${PACKAGE_DIR}/usr/bin ] && mkdir -p ${PACKAGE_DIR}/usr/bin
	[ ! -d ${PACKAGE_DIR}/lib ] && mkdir -p ${PACKAGE_DIR}/lib
	cp  ${SOURCE}/src/.libs/curl ${PACKAGE_DIR}/usr/bin/
	${STRIP} ${PACKAGE_DIR}/usr/bin/curl
	cp -a ${SOURCE}/lib/.libs/libcurl.so* ${PACKAGE_DIR}/lib/
	${STRIP} ${PACKAGE_DIR}/lib/*

	(cd ${PACKAGE_DIR}; tar czvf ../${SOURCE}.tgz * )

	cat moxa-install.header ${SOURCE}.tgz > ${SOURCE}_install.sh
	chmod 755 ${SOURCE}_install.sh

}

if [ ! -d ${SOURCE} ] ; then
	[ -f ${SOURCE}.tar.gz ] && tar xzvf ${SOURCE}.tar.gz
	[ -f ${SOURCE}.tar.bz2 ] && tar xjvf ${SOURCE}.tar.bz2
fi

cd ${SOURCE}
configure
make
cd ..
build_package

