/*
 * The watchdog daemon will ack the watchdog timer in 20 seconds to keep the system working.
 * History:
 * Date		Author			Comment
 * 05-24-2012	Jared Wu.		Create for RNAS-1200 platform
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<fcntl.h>
#include	<string.h>
#include	<sys/stat.h>
#include	<sys/ioctl.h>
#include	<sys/select.h>
#include	<sys/time.h>
#include        <signal.h>

#define WATCHDOG_IOCTL_BASE     'W'
#define WDIOC_GETSTATUS         _IOR(WATCHDOG_IOCTL_BASE, 1, int)
#define WDIOC_GETBOOTSTATUS     _IOR(WATCHDOG_IOCTL_BASE, 2, int)
#define WDIOC_GETTEMP           _IOR(WATCHDOG_IOCTL_BASE, 3, int)
#define WDIOC_SETOPTIONS        _IOR(WATCHDOG_IOCTL_BASE, 4, int)
#define WDIOC_KEEPALIVE         _IOR(WATCHDOG_IOCTL_BASE, 5, int)
#define WDIOC_SETTIMEOUT        _IOWR(WATCHDOG_IOCTL_BASE, 6, int)
#define WDIOC_GETTIMEOUT        _IOR(WATCHDOG_IOCTL_BASE, 7, int)
#define WDIOC_SETPRETIMEOUT     _IOWR(WATCHDOG_IOCTL_BASE, 8, int)
#define WDIOC_GETPRETIMEOUT     _IOR(WATCHDOG_IOCTL_BASE, 9, int)
#define WDIOC_GETTIMELEFT       _IOR(WATCHDOG_IOCTL_BASE, 10, int)

#define WDT_MIN_TIME 0
#define WDT_MAX_TIME 24

#ifdef DEBUG
#define dbg_printf(x...)	printf(x)
#else
#define dbg_printf(x...)
#endif

void showHelp(void) {
	printf("Usage twatchdog -[flag]\n");
		printf("flag is following :\n");
		printf("	h - show this message\n");
		printf("	n - does not ACK watchdog\n");
		printf("	g - get the watchdog time\n");
		printf("	l - get the seconds of watchdog to timeout.\n");
		printf("	t - set the time to ACK the watchdog timer in x seconds. Default x is 24\n", WDT_MAX_TIME);
		printf("	m - do memory fault\n");
		printf("	v - verbose mode\n");
}

extern int optind, opterr, optopt;
extern char *optarg, **environ;
static int bGetWDTTime = 0;
static int bGetWDTLeftTime = 0;
static int bSetTime = 0;
static int bNotAck = 0;
static int bShowHelp = 0;
static int bDoMemFail = 0;
static int bVerbose = 0;
static unsigned long AckTime = WDT_MAX_TIME;
static unsigned long WDTTime;
static unsigned long WDTTimeLeft;
static int wdt_fd; // For watchdog file descriptor
static int bNormalTerm = 0; // For normal termination

void signal_watchdog (int sig) {
	bNormalTerm = 1;
}

int main(int argc, char *argv[])
{
	int		mode, i, c;
	char		flag=0;
	char     optstring[] = "nglhmt:";

	while ((c = getopt(argc, argv, optstring)) != -1)
		switch (c) {
		case 'n':
			bNotAck = 1;
			printf("Not to ack the watchdog timer.\n");
			break;
		case 'g':
			bGetWDTTime = 1;
			printf("Get the seconds of watchdog to timeout.\n");
			break;
		case 'l':
			bGetWDTLeftTime = 1;
			printf("Get the seconds of watchdog to timeout.\n");
			break;
		case 't':
			bSetTime = 1;
			AckTime = atoi(optarg);
			if ( AckTime < WDT_MIN_TIME || AckTime > WDT_MAX_TIME ) {
				showHelp();
				return 0;
}
			printf("Set the watchdog time %d.\n", AckTime);
			break;
		case 'h':
			bShowHelp = 1;
			printf("Show the help message\n");
			break;
		case 'm':
			bDoMemFail = 1;
			printf("Do memory fault\n");
			break;
		case 'v':
			bVerbose = 1;
			printf("Verbose mode\n");
			break;
		case '?':
			printf("The option is not %c recognized\n", optopt);
			break;
		}

	if ( bShowHelp == 1 ) {
		showHelp();
		return 0;
	}

	if ( bSetTime == 1 ) {
		printf("Set ACK time = %d\n", AckTime);
	}

	// For normal stop condition, we should close the watchdog
	signal(SIGTERM, signal_watchdog);
	signal(SIGKILL, signal_watchdog);

	wdt_fd = open("/dev/watchdog", O_RDWR);
	if ( wdt_fd < 0 ) {
		printf("Open watchdog devicd node fail !\n");
		exit(1);
	}

	write(wdt_fd, "V", 1); // Set WDT_OK_TO_CLOSE bit

	if ( bGetWDTTime ) {
		ioctl(wdt_fd, WDIOC_GETTIMEOUT, &WDTTime);
		printf("Now watchdog timeout in %d seconds\n",WDTTime);
	}

	/* Do memory fault */
	if ( bDoMemFail == 1 ) {
		printf("Do memory fault\n");
		*(int *)0 = 1;
	}

	while ( 1 ) {

		if ( bGetWDTLeftTime ) {
			ioctl(wdt_fd, WDIOC_GETTIMELEFT, &WDTTimeLeft);
			printf("Watchdog timer left %d seconds to timeout\n",WDTTimeLeft);
		}

		sleep(AckTime);

		if ( bNormalTerm ) {
			printf("Exit the loop, to terminal the watchdog\n");
			break;
		}

		if ( bNotAck == 0 ) {
			if ( bVerbose )
				printf("Ack the watchdog timer\n");
			ioctl(wdt_fd, WDIOC_KEEPALIVE, 0);
		}
	}

	close(wdt_fd);

	return (0);
}
