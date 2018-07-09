#
# hostname.sh	Set hostname.
#
# Version:	@(#)hostname.sh  1.00  22-Jun-1998  miquels@cistron.nl
#
# chkconfig: S 40 0
#

echo -n "Hostname: "

if [ -f /etc/hostname ]
then
    hostname -F /etc/hostname
fi

echo $(hostname)"."

