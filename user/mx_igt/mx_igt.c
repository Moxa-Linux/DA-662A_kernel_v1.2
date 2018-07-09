/* History:
 * Date		Author			Comment
 * 03-15-2012	Jared Wu.		Read IGT status, bit 7, and if IGT keep open status over 10 sec, shutdown the system.
 *
 * compile: 
	make user/mx_igt_read_only
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define IOCTL_GET_IGT	0x101
#define IGT_BIT_MASK	0x80
#define MIN_SEC			1
#define MAX_SEC			60
#define TIME4SHUTDOWN	10 // Default seconds for IGT polling shutdown

#define MISC_DEV "/dev/mxmisc"

int shutdown_in_seconds = TIME4SHUTDOWN;

void do_shutdown(void)
{
	char *args[8];
	int i = 0;

	args[i++] = "halt";
	args[i++] = NULL;

	execv("/sbin/halt", args);

	perror("shutdown");
	exit(1);
}

void usage(char *program_name) {
	printf("Shutdown if IGT keep open over %d seconds\n", shutdown_in_seconds);
	printf("Usage: %s -s seconds\n", program_name);
	printf("	Seconds	for IGT polling shutdown - 1 ~ 60\n");
	printf("	-d Entering debug mode and not shutdown\n");
	printf("	-h help\n");
	exit(0);
}

extern int optind, opterr, optopt;
extern char *optarg, **environ;

int main(int argc, char *argv[]) {
	int fd, c ;
	unsigned int bHaltCount=0; // if ( 1 == shutdown_in_seconds ), system halt.
	unsigned int bDebug=0;
	unsigned int val;
	char optstring[] = "hds:";

	while ((c = getopt(argc, argv, optstring)) != -1)
		switch (c) {
		case 's':
			shutdown_in_seconds = atoi(optarg);
			if ( shutdown_in_seconds < MIN_SEC || \
				shutdown_in_seconds > MAX_SEC )
				usage(argv[0]);
			break;
		case 'd':
			bDebug = 1;
			break;
		case 'h':
		default:
			usage(argv[0]);
		}

	fd = open(MISC_DEV, O_RDONLY);
	if ( fd < 0 ) {
		printf("Open file %s fail !\n", MISC_DEV);
		exit(1);
	}

	while ( 1 ) {

		ioctl(fd, IOCTL_GET_IGT, &val);

		/* Jared, 03-19-2012, for IGT OPEN status, bit 7 is zero */
		if ( val & IGT_BIT_MASK )
			bHaltCount=0;
		else
			bHaltCount++;

		if ( bDebug )
			printf("System shutdown in %d seconds, IOCTL_GET_IGT: val=%x\n", (shutdown_in_seconds-bHaltCount), val);

		// shutdown if IGT reach the seconds
		if ( bHaltCount >= shutdown_in_seconds ) {
			printf("System shutdown now\n");
			if( !bDebug ) // Shutdown in non-debug mode
				do_shutdown();
			bHaltCount=shutdown_in_seconds;
		}
		sleep (1);
	} 

	close(fd);
}
