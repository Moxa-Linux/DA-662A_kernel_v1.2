#!/bin/sh
# This script is tested about some communcation module connecting cable is OK not.
# There are included NPLC, LAN BPL, WAN BPL & ZigBee.

# golbal variable setting
testingresult=1	# 1 is means OK, 0 is means fail

# subroutine 
# check mad is running or not
mad_is_running()
{
	echo "To make sure the mad application running or not ?"
	checkcount=0
	while true
	do
		if [ ${checkcount} -ge 60 ]; then
			break
		fi
		no=`ps | grep -v "grep" | grep -c "mad"`
		if [ ${no} -ge 1 ]; then
			echo "The mad application is running."
			return 1
		fi
		checkcount=`expr $checkcount + 1`
		sleep 1
	done
	testingreult=0
	return 0
}

# Test LAN BPL
test_lan_bpl()
{
	echo "To test LAN BPL module."
	tmpfile="/var/dcu/bpl.txt"
	# clean up the tmpfile
	cat /dev/null > ${tmpfile}
	/home/dcu/bin/ds2dis eth3 -c 3 -t 3 > ${tmpfile}
	headno=`grep -c "head" ${tmpfile}`
	if [ ${headno} -eq 0 ]; then
		echo "LAN BPL testing is fail !"
		testingresult=0
	else
		echo "LAN BPL testing is OK."
	fi
}

# Test WAN BPL for BPL DCU model
bpl_model_test_wan_bpl()
{
	echo "To test WAN BPL module."
	tmpfile="/var/dcu/bpl.txt"
	# clean up the tmpfile
	cat /dev/null > ${tmpfile}
	/home/dcu/bin/ds2dis eth3 -c 3 -t 3 > ${tmpfile}
	nodeno=`grep -c "node" ${tmpfile}`
	if [ ${nodeno} -eq 0 ]; then
		echo "WAN BPL testing is fail !"
		testingresult=0
	else
		echo "WAN BPL testing is OK."
	fi
}

# Test WAN BPL for NPL DCU model
npl_model_test_wan_bpl()
{
	echo "To test WAN BPL module."
	ifconfig eth2 192.168.5.127
	tmpfile="/var/dcu/bpl.txt"
	# clean up the tmpfile
	cat /dev/null > ${tmpfile}
	/home/dcu/bin/ds2dis eth2 -c 3 -t 3 > ${tmpfile}
	nodeno=`grep -c "node" ${tmpfile}`
	if [ ${nodeno} -eq 0 ]; then
		echo "WAN BPL testing is fail !"
		testingresult=0
	else
		echo "WAN BPL testing is OK."
	fi
}

# Test ZigBee
test_zigbee()
{
	echo "To test ZigBee module."
	checkcount=0
	waittime=300
	tmpfile="/var/dcu/mad.out"
	while true
	do
		if [ ${checkcount} -ge ${waittime} ]; then
			break
		fi
		# clean up the tmpfile
		/home/dcu/bin/setmadrf ${tmpfile}
		killall -USR1 mad
		zbmeterno=`grep -c "Type=1" ${tmpfile}`
		if [ ${zbmeterno} -eq 0 ]; then
			checkcount=`expr $checkcount + 1`
		else
			break
		fi
		sleep 1
	done
	/home/dcu/bin/dismadr
	if [ ${checkcount} -ge ${waittime} ]; then
		echo "ZigBee testing is fail !"
		testingresult=0
	else
		echo "ZigBee testing is OK."
	fi
}

# Test NPLC
test_nplc()
{
	echo "To test NPLC module."
	checkcount=0
	waittime=300
	tmpfile="/var/dcu/mad.out"
	while true
	do
		if [ ${checkcount} -ge ${waittime} ]; then
			break
		fi
		# clean up the tmpfile
		/home/dcu/bin/setmadrf ${tmpfile}
		killall -USR1 mad
		nplcmeterno=`grep -c "Type=3" ${tmpfile}`
		if [ ${nplcmeterno} -eq 0 ]; then
			checkcount=`expr $checkcount + 1`
		else
			break
		fi
		sleep 1
	done
	/home/dcu/bin/dismadr
	if [ ${checkcount} -ge ${waittime} ]; then
		echo "NPLC testing is fail !"
		testingresult=0
	else
		echo "NPLC testing is OK."
	fi
}

#
# main process
#
echo "Starting to run DCU8620 T3 testing."

# read T3 flag
/usr/sbin/read_t3_flag
t3flag=$?
if [ x"${t3flag}" == x"1" ]; then
	echo "The T3 flag is set."
	exit 0
elif [ x"${t3flag}" != x"0" ]; then
	echo "Read T3 flag is fail [${t3flag}] !"
	testingresult=0
	/usr/sbin/blank_ready_led.sh &
	exit 1
fi
echo "The T3 flag is cleaned. Start to do T3 testing."

# make sure the mad is running
mad_is_running
if [ $? -eq 0 ]; then
	echo "The mad of AMI application isn't run !"
	testingresult=0
	/usr/sbin/blank_ready_led.sh &
	exit 1
fi

# Check it is NPLC model or BPL model
model=`cat /proc/driver/dcu_model`
if [ x"${model}" == x"e" ]; then
	echo "This DCU is a BPL model."
	test_lan_bpl
	bpl_model_test_wan_bpl
elif [ x"${model}" == x"f" ]; then
	echo "This DCU is a NPL model."
	test_nplc
	npl_model_test_wan_bpl
elif [ x"${model}" == x"d" ]; then
	echo "This DCU is for Protecsa."
else
	echo "This DCU is a unknow model [${model}] !"
	testingresult=0
	/usr/sbin/blank_ready_led.sh &
	exit 1
fi

# to test ZigBee module
test_zigbee

# check testing result
# first shutdown the AMI application and clean up all record
/etc/init.d/dcu.sh stop
/home/dcu/bin/dcu_clean.sh
sync;sync;sync

# do testing result action
if [ ${testingresult} -eq 0 ]; then
	echo "The T3 testing is fail !"
	# blank the ready LED
	while true
	do
		/bin/readyled 0
		sleep 1
		/bin/readyled 1
		sleep 1
	done
else
	echo "The T3 testing is OK."
	# set T3 flag on
	/usr/sbin/set_t3_flag
	# beeper continue to on
	while true
	do
		/usr/sbin/beeperon
		sleep 1
	done
fi
exit 0
