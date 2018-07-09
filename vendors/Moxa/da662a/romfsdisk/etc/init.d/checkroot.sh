#
# checkroot.sh	Check to root file system.
#
# Version:	@(#)checkroot.sh  2.78-4  25-Jun-2000  miquels@cistron.nl
#
# chkconfig: S 10 0
#

. /etc/default/rcS

#
# Set SULOGIN in /etc/default/rcS to yes if you want a sulogin to be spawned
# from this script *before anything else* with a timeout, like SCO does.
#
#[ "$SULOGIN" = yes ] && sulogin -t 30 $CONSOLE

#
# Read /etc/fstab.
#
#exec 9>&0 </etc/fstab
mount -a
