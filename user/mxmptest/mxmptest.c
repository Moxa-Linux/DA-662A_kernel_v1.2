
/*
 * This program is only for Moxa to manufacture program to test the hardware OK or fail.
 *
 * History:
 * Date		Author				Comment
 * 03-16-2006	Victor Yu.			Create it.
 */
#include	<stdio.h>
#include	<stdlib.h>
#include        <fcntl.h>
#include        <errno.h>
#include        <mtd/mtd-user.h>
#include        <sys/types.h>
#include        <sys/ioctl.h>
#include	<sys/socket.h>
#include	<sys/time.h>
#include	<sys/select.h>
#include	<linux/if.h>
#include        <netinet/in.h>
#include        <netdb.h>
#include	<unistd.h>
#include        <arpa/telnet.h>
#include        <arpa/inet.h>
#include	<netpacket/packet.h>
#include	<net/ethernet.h>
#include        <signal.h>
#include        <termios.h>
#include        <sys/stat.h>
#include        <sys/reboot.h>
#include	<errno.h>
#include	<linux/kd.h>
#include	<string.h>

#ifdef DEBUG
#define dbg_printf(x...)        printf(x)
#else
#define dbg_printf(x...)
#endif

#define VERSION		"1.1"

#if defined(ARCH_IA261) || defined(ARCH_IA262) || defined(ARCH_IA260) || defined(ARCH_EM2260) || defined(ARCH_IA261_256)
#define BOOT_MTD	"/dev/mtdblock0"
#define MP_FLAG_OFFSET	(0x40000+0x18)
#define USB1_STATE	0x21
#define USB2_STATE	0x22
#define CF_STATE	0x23

//Added by wade 02-19-2009
#elif defined(ARCH_W406) || defined(ARCH_SMG1100) || defined(ARCH_W416)
#define BOOT_MTD	"/dev/mtdblock0"
#define MP_FLAG_OFFSET	(0x40000+0x18)
#define USB1_STATE	0x21
#define SD_STATE	0x22

#elif defined(ARCH_UC8400)
#include        "redboot_config.h"
#define USB_STATE       0x11  // First USB
#define USB_STATE1      0x12  // Second USB
#define CF_STATE        0x13
#define SD_STATE        0x14
#define CARDBUS_STATE   0x15
#else
#define BOOT_MTD	"/dev/mtd0"
#define MP_FLAG		1
#define MP_FLAG_OFFSET	0x5c
#define USB_STATE	14
#define CARDBUS_STATE	15
#endif	// ARCH_IA261

static int	testingstate;
static int buzzer_fd= 0;

static void readyledon(void)
{
	int	fd;

	fd = open("/dev/mxmisc", O_RDWR);
	if ( fd < 0 )
		return;
	ioctl(fd, 1, NULL);
	close(fd);
}

static void readyledoff(void)
{
	int	fd;

	fd = open("/dev/mxmisc", O_RDWR);
	if ( fd < 0 )
		return;
	ioctl(fd, 2, NULL);
	close(fd);
}

static void showdebugled(int value)
{
	int	fd;

	fd = open("/dev/mxmisc", O_RDWR);
	if ( fd < 0 )
		return;
	ioctl(fd, 0x100, &value);
	close(fd);
}

static void testingfail(void)
{
	readyledon();
	while ( 1 ) {
		showdebugled(testingstate);
		sleep(1);
		showdebugled(0);
		sleep(1);
	}
}


int BuzzerInit()
{
        buzzer_fd= open("/dev/console", O_RDWR);
        return buzzer_fd;
}


int BuzzerSound( int time)
{
        if( buzzer_fd <= 0)
                if( BuzzerInit() < 0)
                        return -1;

        ioctl( buzzer_fd, KDMKTONE, (time<<16) | 100);
}

static void testingok(void)
{
	showdebugled(testingstate);

	while ( 1 ) {
		BuzzerSound(250);
		readyledon();
		sleep(1);
		BuzzerSound(250);
		readyledoff();
		sleep(1);
	}
}

#define	BUF_LEN		256
#define	BUF_LEN		256
static void	testfile(char *filename)
{
	int	fd, i;
	char	buffer[BUF_LEN];

	for (i=0; i<BUF_LEN; i++ )
		buffer[i] = (i & 0xf) + '0';
	fd = open(filename, O_TRUNC | O_CREAT | O_RDWR);
	if ( fd < 0 ) {
		dbg_printf("First time write [%s] file open fail !\n", filename);
		testingfail();
	}
	i = write(fd, buffer, BUF_LEN);
	close(fd);
	if ( i != BUF_LEN ) {
		dbg_printf("Frist time write file [%s] length return fail [%d] !\n", filename, i);
		testingfail();
	}

	fd = open(filename, O_RDONLY);
	if ( fd < 0 ) {
		dbg_printf("Open file [%s] to read fail !\n", filename);
		testingfail();
	}
	if ( read(fd, buffer, BUF_LEN) != BUF_LEN ) {
		dbg_printf("Read file [%s] length bad !\n", filename);
		close(fd);
		testingfail();
	}
	close(fd);
	for ( i=0; i<BUF_LEN; i++ ) {
		if ( buffer[i] != ((i & 0xf) + '0') )
			break;
	}
	if ( i == BUF_LEN )
		return;

	dbg_printf("File [%s] read data error !\n", filename);
	testingfail();
}

#if !defined(ARCH_IA261) && !defined(ARCH_IA262) && !defined(ARCH_IA260) && !defined(ARCH_EM2260)&& !defined(ARCH_W406) && !defined(ARCH_SMG1100) && !defined(ARCH_W416) && !defined(ARCH_IA261_256)
#define TEST_STRING	"1234567890"
#define TEST_STRING_LEN	10
#if defined(ARCH_W345)
#define TEST_FILE	"/mnt/usbstorage1/mp.txt"
#else
#define TEST_FILE	"/mnt/usbstorage/mp.txt"
#endif
static void testusb(void)
{
	int	usbfd;
	char	buf[128], str[128];
	FILE	*f;

	// first show debug LED
	testingstate = USB_STATE;
	showdebugled(testingstate);

	// check USB exist or not
	f = fopen("/proc/mounts", "r");
	if ( f == NULL )
		testingfail();
	while ( fgets(buf, 128, f) ) {
		sscanf(buf, "%s", str);
		if ( strcmp(str, "/dev/sda1") == 0 )
			goto found_usb;
	}
	fclose(f);
	testingfail();
found_usb:
	fclose(f);

	// write data to USB storage
	usbfd = open(TEST_FILE, O_CREAT|O_TRUNC|O_RDWR);
	if ( usbfd < 0 )
		testingfail();
	write(usbfd, TEST_STRING, TEST_STRING_LEN);
	close(usbfd);
	
	// read data from USB storage
	usbfd = open(TEST_FILE, O_RDONLY);
	if ( usbfd < 0 )
		testingfail();
	read(usbfd, buf, TEST_STRING_LEN);
	buf[TEST_STRING_LEN] = 0;
	if ( strcmp(buf, TEST_STRING) )
		testingfail();
	close(usbfd);
}
#endif

#if defined(ARCH_IA241) || defined(ARCH_IA241_32128)
#define UDP_TEST_PORT	0x404
#define TEST_LEN	128
static void testcardbus(void)
{
	int			i, fd;
	struct ifreq		ifr;
	struct timeval		tm;
	fd_set			readfds;
	struct sockaddr_ll	ll;
	char			sbuf[TEST_LEN], rbuf[TEST_LEN];

	testingstate = CARDBUS_STATE;
	showdebugled(testingstate);
	fd = socket(AF_PACKET, SOCK_DGRAM, 0);
	if ( fd < 0 )
		testingfail();

	// set this socket file handle to force to send out by ixp1 interface
	memset(&ifr, 0, sizeof(struct ifreq));
	strcpy(ifr.ifr_name, "eth2");
        if ( setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, &ifr, sizeof(ifr)) < 0 )
		testingfail();
	i = 1;
	setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &i, sizeof(int));

	// bind the socket file handle local port number
	strcpy(ifr.ifr_name, "eth2");
	ioctl(fd, SIOCGIFINDEX, &ifr);
	ll.sll_family = AF_PACKET;
	ll.sll_protocol = htons(ETH_P_ALL);
	ll.sll_ifindex = ifr.ifr_ifindex;
	ll.sll_pkttype = htons(PACKET_BROADCAST);
	ll.sll_halen = 6;
	memset(ll.sll_addr, 0xff, 8);
	if ( bind(fd, (struct sockaddr *)&ll, sizeof(ll)) < 0 )
		testingfail();

	// initialize the data buffer
	for ( i=0; i<TEST_LEN; i++ )
		sbuf[i] =  i;

	// send data
	ll.sll_family = AF_PACKET;
	ll.sll_protocol = htons(ETH_P_ALL);
	ll.sll_ifindex = ifr.ifr_ifindex;
	ll.sll_pkttype = htons(PACKET_BROADCAST);
	ll.sll_halen = 6;
	memset(ll.sll_addr, 0xff, 8);
	if ( sendto(fd, sbuf, TEST_LEN, 0, (struct sockaddr *)&ll, sizeof(ll)) <= 0 )
		testingfail();

	// receive data
	FD_ZERO(&readfds);
	FD_SET(fd, &readfds);
	tm.tv_sec = 0;
	tm.tv_usec = 100 * 1000;
	if ( select(fd+1, &readfds, NULL, NULL, &tm) <= 0 )
		testingfail();
	if ( recvfrom(fd, rbuf, TEST_LEN, 0, (struct sockaddr *)&ll, &i) != TEST_LEN )
		testingfail();
	for ( i=0; i<TEST_LEN; i++ ) {
		if ( sbuf[i] != rbuf[i] )
			break;
	}
	if ( i != TEST_LEN )
		testingfail();

	close(fd);
}
#endif

int	main(int argc, char *argv[])
{
	int		fd;
	char		*buf;
#if defined(ARCH_UC8400)
	int flag;
#else
	unsigned int	flag;
#endif
	mtd_info_t	meminfo;
	erase_info_t	erase;

	fd = open(BOOT_MTD, O_RDWR);
	if ( fd < 0 ) {
		printf("Open flash fail !\n");
		testingfail();
	}
#if defined(ARCH_IA261) || defined(ARCH_IA262) || defined(ARCH_IA261) || defined(ARCH_EM2260) || defined(ARCH_IA260)|| defined(ARCH_W406) || defined(ARCH_SMG1100) || defined(ARCH_W416)|| defined(ARCH_IA261_256)

	lseek(fd, MP_FLAG_OFFSET,SEEK_SET);
	read(fd, &flag, sizeof(flag));
	if ( flag ) {
		dbg_printf("MP flag is set. So exit testing.\n");
		close(fd);
		exit(0);
	}
#else
	if ( ioctl(fd, MEMGETINFO, &meminfo) ) {
		printf("Get flash information fail !\n");
exit_l1:
		close(fd);
		exit(1);
	}
	buf = malloc(meminfo.erasesize);
	if ( buf == NULL ) {
		printf("Memory allocate fail !\n");
		goto exit_l1;
	}
	if ( read(fd, buf, meminfo.erasesize) != meminfo.erasesize ) {
		printf("Backup flash data fail !\n");
		free(buf);
		close(fd);
		exit(1);
	}
	flag = *(unsigned int *)&buf[MP_FLAG_OFFSET];
	if ( flag & MP_FLAG ) {
		free(buf);
		close(fd);
		exit(0);
	}
#endif	// ARCH_IA261

	// start to testing USB storage
#if defined(ARCH_IA261) || defined(ARCH_IA262) || defined(ARCH_IA260) || defined(ARCH_EM2260)|| defined(ARCH_IA261_256)

	sleep(3); 	// to wait USB scan finished

	// mount USB storage for testing
	setenv("ACTION", "add", 1);
	setenv("DISK", "sda", 1);
	system("/bin/usbstpnp");
	setenv("DISK", "sdb", 1);
	system("/bin/usbstpnp");

	// testing first USB with storage
	testingstate = USB1_STATE;
	showdebugled(testingstate);
	testfile("/var/usbstorage1/mptf.txt");

	// testing second USB with storage
	testingstate = USB2_STATE;
	showdebugled(testingstate);
	testfile("/var/usbstorage2/mptf.txt");

	// testing CF card
	testingstate = CF_STATE;
	showdebugled(testingstate);
	testfile("/mnt/cf/mptf.txt");

//Added by wade 02-19-2009
//only test 1 usb, and 1 sd card
#elif defined(ARCH_W406) || defined(ARCH_SMG1100) || defined(ARCH_W416)
	sleep(3); 	// to wait USB scan finished

	// mount USB storage for testing
	setenv("ACTION", "add", 1);
	setenv("DISK", "sda", 1);
	system("/bin/usbstpnp");
	setenv("DISK", "sdb", 1);
	system("/bin/usbstpnp");

	// testing first USB with storage
	testingstate = USB1_STATE;
	showdebugled(testingstate);
	testfile("/var/usbstorage1/mptf.txt");
	dbg_printf("test first usb ok\n");

	// testing SD state
	testingstate = SD_STATE;
	showdebugled(testingstate);
	testfile("/var/usbstorage2/mptf.txt");
	dbg_printf("test sd card ok\n");

#else	// ARCH_IA261
	testusb();
#endif	// ARCH_IA261

#if defined(ARCH_IA241) || defined(ARCH_IA241_32128)
	testcardbus();
#endif

#if defined(ARCH_IA261) || defined(ARCH_IA262) || defined(ARCH_IA260) || defined(ARCH_EM2260)\
|| defined(ARCH_W406) || defined(ARCH_SMG1100) || defined(ARCH_W416)|| defined(ARCH_IA261_256)

	flag = 1;
	lseek(fd, MP_FLAG_OFFSET,SEEK_SET);
	write(fd, &flag, sizeof(flag));
	close(fd);

	sync();
	//Added by Wade ,03-19-2009, print message and add buzzer
	printf("MP Flag Set OK\n");
	fflush(stdout);
	testingok();
#else
	// testing OK, enable the MP flag
	showdebugled(0);

	flag |= MP_FLAG;
	*(unsigned int *)&buf[MP_FLAG_OFFSET] = flag;
	erase.length = meminfo.erasesize;
	erase.start = 0;
	if ( ioctl(fd, MEMUNLOCK, &erase) < 0 ) {
		if ( errno != EOPNOTSUPP ) {
			printf("Unlock flash fail !\n");
			goto exit_l2;
		}
	}
	if ( ioctl(fd, MEMERASE, &erase) ) {
		printf("Erase flash fail !\n");
		goto exit_l2;
	}
	lseek(fd, 0, SEEK_SET);
	write(fd, buf, meminfo.erasesize);
	close(fd);
	free(buf);

	// let the beeper on
	fd = open("/dev/console", O_WRONLY);
	ioctl(fd, KDMKTONE, 100<<16);
	close(fd);

exit_l2:	// writel MP flag error to blank the ready led
	close(fd);
	free(buf);
	while ( 1 ) {
		readyledon();
		sleep(1);
		readyledoff();
		sleep(1);
	}
#endif

	exit(0);
}
