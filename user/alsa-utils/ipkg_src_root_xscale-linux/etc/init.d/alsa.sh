#! /bin/sh
# /etc/init.d/alsa.sh: start ALSA drivers.
#
PATH=/bin:/usr/bin:/sbin:/usr/sbin

case "$1" in
  start)
    echo -n "Initialze the ALSA drivers"
    insmod /lib/modules/2.6.23.1/kernel/sound/core/snd-page-alloc.ko
    insmod /lib/modules/2.6.23.1/kernel/sound/soundcore.ko
    insmod /lib/modules/2.6.23.1/kernel/sound/core/snd.ko
    insmod /lib/modules/2.6.23.1/kernel/sound/core/snd-hwdep.ko
    insmod /lib/modules/2.6.23.1/kernel/sound/core/snd-timer.ko
    insmod /lib/modules/2.6.23.1/kernel/sound/core/snd-rawmidi.ko
    insmod /lib/modules/2.6.23.1/kernel/sound/usb/snd-usb-lib.ko
    insmod /lib/modules/2.6.23.1/kernel/sound/core/snd-pcm.ko
    insmod /lib/modules/2.6.23.1/kernel/sound/usb/snd-usb-audio.ko
    insmod /lib/modules/2.6.23.1/kernel/drivers/usb/class/cdc-acm.ko
    rm -rf /dev/ttyS[1-3]
    mkdir /dev/snd
    mknod /dev/snd/controlC0 c 116 0
    mknod /dev/snd/midiC0D0 c 116 8
    mknod /dev/snd/pcmC0D0p c 116 16
    mknod /dev/snd/pcmC0D0c c 116 24
    mknod /dev/snd/timer c 116 33
    mknod /dev/snd/controlC0 c 116 0
    mknod /dev/ttyACM0 c 166 0
    ;;
  stop)
    rm -rf /dev/snd
    rmmod cdc_acm
    rmmod snd_usb_audio
    rmmod snd_pcm 
    rmmod snd_usb_lib
    rmmod snd_rawmidi
    rmmod snd_timer
    rmmod snd_hwdep
    rmmod snd
    rmmod soundcore
    rmmod snd_page_alloc
    echo "."
    ;;
  *)
    echo "Usage: /etc/init.d/alsa.sh {start|stop}"
    exit 1
esac

exit 0
