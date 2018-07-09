#!/bin/bash 

TUN_INTF=tun6to4  # The name of the 6to4 tunnel link
WAN_LOCAL=eth0
WAN_IP=$(ifconfig $WAN_LOCAL |grep 'inet addr'|awk '{print $2}'|cut -d':' -f 2)
WAN_IPV6_IP=$(printf "2002:%02x%02x:%02x%02x::1/16" $(echo "${WAN_IP}"| tr '.' ' '))
TUN_RELAY=192.88.99.1  # 6to4 relay address 

mname=
module_up()
{
	oIFS=$IFS
	IFS='
	'
	FOUND="no"
	for LINE in `lsmod`
	do
		TOK=`echo $LINE | cut -d' ' -f1`
		if [ "$TOK" = "$mname" ]; then
			FOUND="yes";
			break;
		fi
	done
	IFS=$oIFS
	
	if [ "$FOUND" = "no" ]; then
		modprobe $mname 
	fi
}

case "$1" in
	start)
		# Load the module "ipv6"
		mname=ipv6
		module_up
		
		# Configure the 6to4 sit tunnel (Simple Internet Transition) 
		echo "Creating tunnel interface..."
		ip tunnel add ${TUN_INTF} mode sit remote any local ${WAN_IP} 
		# IPv6 in IP has an additional IPv4 header (20 bytes). 8 bytes for PPPoE. 
		echo "Setting tunnel interface up..."
		ip link set dev ${TUN_INTF} mtu 1472 up 

		# Add the 6to4 IPv6 address 
		echo "Assigning ${WAN_IPV6_IP} address to tunnel interface..."
		ip -6 addr add ${WAN_IPV6_IP} dev ${TUN_INTF} 

		# Add the routing. 
		echo "Assigning ::96 address to $lanif (local lan interface)..."

		ip -6 route add ::96 dev ${TUN_INTF} metric 1 
		echo "Adding route to IPv6 internet on tunnel interface via relay..."
		ip -6 route add 2000::/3 via ::${TUN_RELAY} dev ${TUN_INTF} metric 1

		;;
	stop)
		# Clean up before adding the tunnel 
		echo "Removing routes to 6to4 tunnel interface..."
		#ip -6 route flush dev ${TUN_INTF} 
		ip -6 route del 2000::/3 via ::${TUN_RELAY} dev ${TUN_INTF}
		ip -6 route del ::96 dev ${TUN_INTF}
		ip -6 addr del ${WAN_IPV6_IP} dev ${TUN_INTF}
		echo "Setting tunnel interface down..."
		ip link set dev ${TUN_INTF} down 
		echo "Removing tunnel interface..."
		ip tunnel del ${TUN_INTF}
		echo "Done."
		;;
	*)
		echo "Usage: /etc/init.d/tun6to4 {start|stop}"
		exit 1
		;;
esac

