#!/bin/sh

rm Storable.o
make
xscale-none-linux-gnueabi-strip blib/arch/auto/Storable/Storable.so
scp blib/arch/auto/Storable/Storable.so 192.168.27.229:/usr/local/perl/lib/perl5/5.10.1/armv5teb-linux/auto/Storable/

