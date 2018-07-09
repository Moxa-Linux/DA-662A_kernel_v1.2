#!/bin/sh

DISKNAME=`echo $1|cut -c1-3`
DISKNAME2=`echo $1|cut -c1-2`
MPATHLOG=/dev/shm/mountpath.log

#echo "$1 action" > $MPATHLOG

if [ "$1" == "" ]; then
	echo .automount.sh parameter is none.
	exit 1
fi

if [ "$DISKNAME2" == "cf" ]; then
	MPATH=/var/$DISKNAME2
else
	MPATH=/var/$DISKNAME
fi
if [ -e "$MPATH" ]; then
	#echo "umount $MPATH" > $MPATHLOG
	fuser -m -k $MPATH
	umount $MPATH
	rm -rf $MPATH
else
	#echo "mount $MPATH" > $MPATHLOG
	mkdir -p $MPATH
	mount /dev/$1 $MPATH
fi
exit 0
