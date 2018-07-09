#!/bin/sh

SCRIPT="$0"
case "${SCRIPT}" in
/*)
	;;
*)
	if [ -f "${SCRIPT}" ]
	then
		SCRIPT="`pwd`/${SCRIPT}"
	else
		SCRIPT="`which ${SCRIPT}`"
	fi
	;;
esac

cd /

if [ ! -f "${SCRIPT}" ]
then
	echo "Cannot find the location of the install script ($SCRIPT) ?"
	exit 1
fi

SKIP=`awk '/^__ARCHIVE_FOLLOWS__/ { print NR + 1; exit 0; }' ${SCRIPT}`

if id | grep root > /dev/null
then
	:
else
	echo "You must be root to install these tools."
	exit 1
fi

clear
echo ##################################################################
echo Welcome to MOXA ARM Linux platform toolchain installer.
echo This toolchain built with arm-linux compiler v4.4.2 and glibc v2.10.1.
echo Any problem please contact support@moxa.com.tw
echo ##################################################################

echo Press the number:
echo 1.Install Linux cross compiler tool.
echo 2.Uninstall Linux cross compiler tool.
echo 3.Exit or CTRL+C

read A

if [ "$A" = "1" ] ; then
	if [ -d /usr/local/arm-linux-4.4.2-v4 ]; then
		echo "Warning! We have detected that you had installed the old version of Linux Tool Chain on your computer. If you continue the installation, the old Tool Chain will be immediately overwrriten and the program compiled by this new Tool Chain may not be used on your old models."
		echo "We strongly suggest you rename the directory of the older version before installing the new one if you would like to use the old Linux Tool Chain for other embedded computers.  Click N to quit. Click Y to continue installation."
		read Q
		if [ "$Q" = "n" -o "$Q" = "N" ]; then
			echo "Exit from the installation."
			exit 0
		fi
	fi
	tail -n +${SKIP} ${SCRIPT} | gunzip | tar xvf -
	echo "----------------------------------------------"
	echo "arm-linux-gcc install complete"
	echo "Please export these environment variables before using toolchain:"
	echo "export PATH=\$PATH:/usr/local/arm-linux-4.4.2-v4/bin"
	exit 0
fi

if [ "$A" = "2" ] ; then
{
	clear
	echo "WARNING!!!!!"
	echo "Uninstall Tool-chain will remove file:"
	echo "/usr/local/arm-linux-4.4.2-v4"
	echo "press any key to continue or Ctrl+C to exit"
	read B
	rm -rf /usr/local/arm-linux-4.4.2-v4
	echo "Uninstall complete!"
	exit 0
}
fi
if [ "$A" = "3" ] ; then
	exit 0
fi

exit 0
__ARCHIVE_FOLLOWS__
