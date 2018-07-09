
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

#if (defined ARCH_UC8400) || (defined ARCH_DCU8620)
#include	"redboot_config.h"
#include	"swid.h"
#else
#define BOOT_MTD	"/dev/mtd0"
#endif

#ifdef DEBUG
#define dbg_printf(x...)        printf(x)
#else
#define dbg_printf(x...)
#endif

#define VERSION		"1.0"
#define MAX_KEY_LEN	8
#define MP_FLAG		1
#define MP_FLAG_OFFSET	0x5c

//hank
#if (defined ARCH_UC8400) || (defined ARCH_DCU8620)

#define USB_STATE	0x11  // First USB
#define USB_STATE1	0x12  // Second USB
#define CF_STATE        0x13
#define SD_STATE	0x14
#define CARDBUS_STATE	0x15

#else

#define CF_STATE        12
#define SD_STATE	13
#define USB_STATE	14
#define CARDBUS_STATE	15

#endif


#define TEST_CHAR	'a'

typedef struct __MOXA_MPTEST_ITEM {
	int (*TestUSB1) (void);
	int (*TestUSB2) (void);
	int (*TestCF) (void);
	int (*TestSD) (void);
	int (*TestCardbus) (void);
} MOXA_MPTEST_ITEM_T;

static int	testingstate;

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
	while ( 1 ) {
		showdebugled(testingstate);
		sleep(1);
		showdebugled(0);
		sleep(1);
	}
}

#define TEST_LEN	1024
static char	buf[TEST_LEN];
// following add by Victor Yu. 10-28-2004
static int	CheckMount(char *dir)
{
	FILE	*file;
	char	m1[64], m2[64];

	dbg_printf("%s[%d]:Check mount %s\n", __FUNCTION__, __LINE__, dir);

	file = fopen("/proc/mounts", "r");
	if ( file == NULL ) {
		printf("Open /proc/mounts fail !\n");
		return -1;
	}
	while ( fgets(buf, TEST_LEN, file) ) {
		sscanf(buf, "%s %s", m1, m2);
		// Add by Jared. Now, check "/dev/sda1", "/dev/sda1"
		if ( strcmp(m1, dir) == 0 ) {
			dbg_printf("%s[%d]: m1=%s, m2=%s\n", __FUNCTION__, __LINE__, m1, m2);
			fclose(file);
			return 0;
		}
	}
	fclose(file);
	printf("%s[%d]: read file: %s fail!\n", __FUNCTION__, __LINE__, file);
	return -1;
}

// Masked by Jared 07-28-2008
int test_USBDisk(char *disk_path, char *mount_point)
{
	int	i, fd1;
	char usb_test_file[64];
	
	// first check USB1 & USB2 are mounted or not
	dbg_printf("umount %s then mount\n", mount_point);
	umount(mount_point);
	
	mount(disk_path, mount_point, "vfat", NULL, NULL);

	if ( CheckMount(disk_path) ) {
		printf("Check mount %s fail!\n", disk_path);
		return -1;
	}
	
	sprintf(usb_test_file, "%s/mptest.txt", mount_point);

	// Remove /mnt/sda/mptest.txt or /mnt/sd[abcd]/mptest.txt
	sprintf(buf, "rm -f %s", usb_test_file);
	system(buf);
	
	fd1 = open(usb_test_file, O_RDWR|O_CREAT|O_TRUNC);
	if ( fd1 < 0 ) {
		printf("%s[%d]: open new file, %s, fail !\r\n", __FUNCTION__, __LINE__, usb_test_file);
		return -1;
	}
	
	memset(buf, TEST_CHAR, TEST_LEN* sizeof(char) );
		
	if ( write(fd1, buf, TEST_LEN) != TEST_LEN ) {
		printf("%s[%d]: write data to %s fail !\r\n", __FUNCTION__, __LINE__, usb_test_file);
		close(fd1);
		return -1;
	}
	close(fd1);

	for ( i=0; i<TEST_LEN; i++ )
		buf[i] = TEST_CHAR + '1';
		
	fd1 = open(usb_test_file, O_RDONLY);
	if ( fd1 < 0 ) {
		printf("%s[%d]: reopen %s fail !\r\n", __FUNCTION__, __LINE__, usb_test_file);
		return -1;
	}
	
	if ( read(fd1, buf, TEST_LEN) != TEST_LEN ) {
		printf("%s[%d]: read %s data length fail !\r\n", __FUNCTION__, __LINE__, usb_test_file);
		close(fd1);
		return -1;
	}
	for ( i=0; i<TEST_LEN; i++ ) {
		if ( buf[i] != TEST_CHAR ) {
			printf("%s[%d]: read %s data context fail !\r\n", __FUNCTION__, __LINE__, usb_test_file);
			close(fd1);
			return -1;
		}
	}
	close(fd1);
	printf("%s[%d]: %s read/write successes !\r\n", __FUNCTION__, __LINE__, usb_test_file);
	
	return 0;
}

static int testusb(void)
{	int nUSBMountCount=0;

	testingstate = USB_STATE;
	showdebugled(testingstate);
#if (defined ARCH_UC8400) || (defined ARCH_DCU8620)
	if ( CheckMount("/dev/sdb1") == 0 ) {
		nUSBMountCount ++;
		if ( test_USBDisk("/dev/sdb1","/mnt/sdb") != 0 )
			testingfail();
	}

	if ( CheckMount("/dev/sdc1") == 0 ) {
		nUSBMountCount ++;
		if ( test_USBDisk("/dev/sdc1","/mnt/sdc") != 0 )
			testingfail();
	}
		//hank
	testingstate = USB_STATE1;
	
	if ( CheckMount("/dev/sdd1") == 0 ) {
		nUSBMountCount ++;
		if ( test_USBDisk("/dev/sdd1","/mnt/sdd") != 0 )
			testingfail();
	}

	if ( nUSBMountCount < 2 ) {
		printf("usb mount fail\n");
		testingfail();
	}
#else
	if ( test_USBDisk("/dev/sda1","/mnt/sda") != 0 )
		testingfail();
#endif
}

static void testcf(void)
{
	testingstate = CF_STATE;
	showdebugled(testingstate);
#if (defined ARCH_UC8400) || (defined ARCH_DCU8620)
	if ( test_USBDisk("/dev/sda1","/mnt/sda") != 0 )
		testingfail();
#endif
}

static void testsd(void)
{
	testingstate = USB_STATE;
	showdebugled(testingstate);
#if (defined ARCH_UC8400) || (defined ARCH_DCU8620)
	if ( test_USBDisk("/dev/sdb1","/mnt/sdb") != 0 )
		testingfail();
#endif
}

#if defined(ARCH_IA241) || defined(ARCH_IA241_32128) || defined(ARCH_IA241_NSPS)
#define UDP_TEST_PORT	0x404
#define CARDBUS_TEST_LEN	128
static void testcardbus(void)
{
	int			i, fd;
	struct ifreq		ifr;
	struct timeval		tm;
	fd_set			readfds;
	struct sockaddr_ll	ll;
	char			sbuf[CARDBUS_TEST_LEN], rbuf[CARDBUS_TEST_LEN];

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
	for ( i=0; i<CARDBUS_TEST_LEN; i++ )
		sbuf[i] =  i;

	// send data
	ll.sll_family = AF_PACKET;
	ll.sll_protocol = htons(ETH_P_ALL);
	ll.sll_ifindex = ifr.ifr_ifindex;
	ll.sll_pkttype = htons(PACKET_BROADCAST);
	ll.sll_halen = 6;
	memset(ll.sll_addr, 0xff, 8);
	if ( sendto(fd, sbuf, CARDBUS_TEST_LEN, 0, (struct sockaddr *)&ll, sizeof(ll)) <= 0 )
		testingfail();

	// receive data
	FD_ZERO(&readfds);
	FD_SET(fd, &readfds);
	tm.tv_sec = 0;
	tm.tv_usec = 100 * 1000;
	if ( select(fd+1, &readfds, NULL, NULL, &tm) <= 0 )
		testingfail();
	if ( recvfrom(fd, rbuf, CARDBUS_TEST_LEN, 0, (struct sockaddr *)&ll, &i) != CARDBUS_TEST_LEN )
		testingfail();
	for ( i=0; i<CARDBUS_TEST_LEN; i++ ) {
		if ( sbuf[i] != rbuf[i] )
			break;
	}
	if ( i != CARDBUS_TEST_LEN )
		testingfail();

	close(fd);
}
#endif

static MOXA_MPTEST_ITEM_T testing_items = { 
#if (defined ARCH_UC8400) || (defined ARCH_DCU8620)
	.TestUSB1 = testusb,
#endif
#if (defined ARCH_UC8400) || (defined ARCH_DCU8620)
	.TestCF = testcf,
#endif
#if (defined ARCH_UC8400) || (defined ARCH_DCU8620)
	.TestSD = NULL,  // 09-09-2008 Jared: The SD reader is removed.
#endif
#if defined(ARCH_IA241) || defined(ARCH_IA241_32128) || defined(ARCH_IA241_NSPS)
	.TestCardbus = testcardbus,
#endif
};

extern int optind, opterr, optopt;
extern char *optarg, **environ;

int	main(int argc, char *argv[])
{
	int		fd, c, bFlagChecked=0;
	char		*buf;
	mtd_info_t	meminfo;
	erase_info_t	erase;
	char     optstring[] = "d";
#if (defined ARCH_UC8400) || (defined ARCH_DCU8620)
	int	flag;
	unsigned int swid;

	read_procfs("/proc/bus/pci/devices", proc_buf);

	swid =  get_swid();
	switch(swid) {
	  case UC8410_ID:
	    break;
	  case UC8416_ID:
	    break;
	  case UC8411_ID:
	  	// Because UC-8411 doesn't equip with USB and CF cards
		testing_items.TestUSB1=NULL;
		testing_items.TestCF=NULL;
	    break;
	  case UC8418_ID:
	    break;
	  default:
	    break;
	}
#else
	unsigned int	flag;
#endif
	
	if ( argc > 2 ) {
		printf("Usage : %s [-n]\n", argv[0]);
		printf("\t-n: ignore the burning flags\n");
		exit(1);
	}

	while ((c = getopt(argc, argv, optstring)) != -1)
		switch(c) {
		case 'd':
			bFlagChecked = 1;  // check the "mp_test" flag
			dbg_printf("Don't check the mp test flag.\n");
			break;
		case '?':
			dbg_printf("Found an option that was not in optstring. %c\n", optopt);
			break;
		}

#if (defined ARCH_UC8400) || (defined ARCH_DCU8620)
	if ( bFlagChecked == 0 ) {
		if ( config_init() < 0 ) {
			printf("Flash configuraiton initialize fail !\r\n");
			return -1;
		}

		if ( config_get("mp_flag", &flag) < 0 ) {
			printf("Get MP flag form flash fail !\r\n");
			return -1;
		} else {
			dbg_printf("Get MP flag %d.\n", flag);
			if ( flag ) {
				dbg_printf("mp_flag is true. Stop the MP testing.\n");
				return 0;
			}
			dbg_printf("Get MP flag is false.\n");
			flag=1;
		}
	}
#else	
	if ( bFlagChecked == 0 ) {
		fd = open(BOOT_MTD, O_RDWR);
		if ( fd < 0 ) {
			printf("Open flash fail !\n");
			exit(1);
		}
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
	}
#endif

	printf("Starting moxa MP test.\n");

	// start to testing USB storage
	if ( testing_items.TestUSB1 )
		testing_items.TestUSB1();

	if ( testing_items.TestCF )
		testing_items.TestCF();

	if ( testing_items.TestSD )
		testing_items.TestSD();
	
	if ( testing_items.TestCardbus )
		testing_items.TestCardbus();

	// testing OK, enable the MP flag
	showdebugled(0x1F);
#if (defined ARCH_UC8400) || (defined ARCH_DCU8620)
	if ( bFlagChecked == 0 ) {
		if ( config_set("mp_flag", &flag) < 0 ) {
			printf("%s[%d]mp_flag:%d set vaule fail\n",flag,__FUNCTION__,__LINE__);
			return -1;
		}
		if ( config_write() < 0 ) {
			printf("%s[%d]Write flash configuration fail !\n",__FUNCTION__,__LINE__);
		return -1;
		}
	}
#else

	if ( bFlagChecked == 0 ) {
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
	}
#endif
	// let the beeper on
	fd = open("/dev/console", O_WRONLY);
	ioctl(fd, KDMKTONE, 100<<16);
	close(fd);

	exit(0);

exit_l2:	// writel MP flag error to blank the ready led
	close(fd);
#if ! ( ( defined ARCH_UC8400) || (defined ARCH_DCU8620) )
	free(buf);
#endif
	while ( 1 ) {
		readyledon();
		sleep(1);
		readyledoff();
		sleep(1);
	}
}
