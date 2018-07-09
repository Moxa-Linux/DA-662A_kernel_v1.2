#! /bin/sh
# /etc/init.d/alsa.sh: start ALSA drivers.
#
PATH=/bin:/usr/bin:/sbin:/usr/sbin

case "$1" in
  start)
    echo -n "Starting smcroute"
    smcroute -d
    ;;
  stop)
    killall smcroute
    echo "."
    ;;
  *)
    echo "Usage: /etc/init.d/smcroute.sh {start|stop}"
    exit 1
esac

exit 0
