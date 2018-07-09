#! /bin/bash
#
# bonding       Start the bonding service, bond eth0 and eth1 as slave.
#
#

NAME=bonding
PATH=/bin:/usr/bin:/sbin:/usr/sbin

# to set ethX interfaces as slave the bond0 must have an ip
BOND_IP="192.168.31.166"

case "$1" in
  start)
    echo "Starting bonding service: $NAME."
    # load the module with use_carrier=0 for monitoring the link by ethtool methods
    modprobe bonding use_carrier=0 mode=1 miimon=100

    ifdown eth0					# putting down eth0
    ifdown eth1					# putting down eth1
    #ifdown eth2					# putting down eth2
    #ifdown eth3					# putting down eth3

    #ifconfig bond0 hw ether 00:90:E8:72:20:01
    ifconfig bond0 $BOND_IP netmask 255.255.255.0 up	# set ip address

    ifenslave bond0 eth0			# set eth0 in slave for bond0
    ifenslave bond0 eth1			# set eth1 in slave for bond0
    #ifenslave bond0 eth2			# set eth2 in slave for bond0
    #ifenslave bond0 eth3			# set eth3 in slave for bond0
    ;;

  stop)
    echo "Stopping bonding service: $NAME"
    ifenslave -d bond0 eth0			# release eth0 from bond0
    ifenslave -d bond0 eth1			# release eth1 from bond0
    #ifenslave -d bond0 eth2			# release eth2 from bond0
    #ifenslave -d bond0 eth3			# release eth3 from bond0

    ifconfig bond0 down				# putting down bond0
    # Dont' use modprobe -r bonding. It would be blocked at unloading ipv6 module.
    rmmod bonding				# unload bonding module

    ifup eth0
    ifup eth1
    #ifup eth2
    #ifup eth3
    ;;

  restart)
    $0 stop
    $0 start
    ;;

  *)
    echo "Usage: /etc/init.d/$NAME {start|stop|restart}"
    exit 1
    ;;
esac

exit 0

