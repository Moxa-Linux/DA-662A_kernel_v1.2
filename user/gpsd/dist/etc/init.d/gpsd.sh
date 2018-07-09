#!/bin/sh
#
# gpsd	This shell script takes care of starting and stopping the GPS daemon
#
# description: gpsd is GPS daemon.

# Source function library.
#. /etc/rc.d/init.d/functions

name="gpsd"
prog="/usr/sbin/gpsd"
args="-n /dev/ttyACM0"
#pidf="/var/run/gpsd.pid"

[ -x $prog ] || exit 0

start() {
	# Start daemons.
	echo -n $"Starting $name: "
	start-stop-daemon -S -b -x $prog -- $args
	RETVAL=$?
	echo
	sleep 1
	/usr/local/bin/mxgps > /dev/null 2>&1 &
	return $RETVAL
}

stop() {
	killall mxgps
	# Stop daemons.
	echo -n $"Shutting down $name: "
	start-stop-daemon -K -n $name
	RETVAL=$?
	echo
	return $RETVAL
}

# See how we were called.
case "$1" in
  start)
	start
        ;;
  stop)
	stop
        ;;
  restart)
	stop
	start
	RETVAL=$?
	;;
  *)
        echo $"Usage: $0 {start|stop|restart}"
        exit 1
esac

exit $RETVAL
