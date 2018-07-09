#######
# X86 #
#######
#CC=gcc
#LD=ld
#CFLAGS=-Wall -fPIC


###########
# Marvell #
###########
CROSS=arm-mv5sft-linux-gnueabi-
CC=$(CROSS)gcc
LD=$(CROSS)ld
CFLAGS=-Wall -fPIC 
#-I/usr/local/arm-mv5sft-linux-gnueabi/arm-mv5sft-linux-gnueabi/include


##########
# IXP425 (incomplete)#
##########
#CC=arm-none-linux-gnueabi-gcc
#LD=arm-none-linux-gnueabi-ld
#CFLAGS=-Wall -fPIC -DWORDS_BIGENDIAN -Icommon/include -I/usr/local/arm-linux-4.4.2-v4/arm-none-linux-gnueabi/include
