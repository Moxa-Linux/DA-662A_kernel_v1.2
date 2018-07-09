
#include	<stdio.h>
#include	<stdlib.h>
#include	<fcntl.h>
#include	<errno.h>
#include	<termios.h>
#include	<time.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<sys/ioctl.h>
#include	<linux/if.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	"moxadevice.h"

//#define DEBUG
#ifdef DEBUG
#define dbg_printf(x...)	printf(x)
#else
#define dbg_printf(x...)
#endif

#ifdef ARCH_DA662A
#define DEV_NAME	"/dev/sds_lcm"
#else
#define DEV_NAME	"/dev/lcm"
#endif

static void LCM_get_xy(lcm_xy_t *xy)
{
	int	fd;

	fd = open(DEV_NAME, O_RDWR);
	if ( fd < 0 )
		return;
	ioctl(fd, IOCTL_LCM_GET_XY, xy);
	close(fd);
}

static void LCM_clean_line(void)
{
	int 	fd;

	fd = open(DEV_NAME, O_RDWR);
	if ( fd < 0 )
		return;
	ioctl(fd, IOCTL_LCM_CLEAN_LINE, NULL);
	close(fd);
}

static void LCM_cls(void)
{
	int 	fd;

	fd = open(DEV_NAME, O_RDWR);
	if ( fd < 0 )
		return;
	ioctl(fd, IOCTL_LCM_CLS, NULL);
	close(fd);
}

static void LCM_goto_xy(lcm_xy_t *xy)
{
	int	fd;

	fd = open(DEV_NAME, O_RDWR);
	if ( fd < 0 )
		return;	
	ioctl(fd, IOCTL_LCM_GOTO_XY, xy);
	close(fd);
}

static void LCM_printf(char *str)
{
	int	fd;

	fd = open(DEV_NAME, O_RDWR);
	if ( fd < 0 )
		return;
	write(fd, str, strlen(str));
	close(fd);
}

#define PORT_NO			2
#define INTERFACE_PREFIX_NAME	"eth"
#define DEFAULT_IP_ADDR		"0.0.0.0"
struct interface {
	struct sockaddr	addr;		/* ip address */
	struct sockaddr	netmask;	/* ip netmask */
	int		has_ip;		/* has ip flag */
	char		name[IFNAMSIZ];	/* iterface name */
	short		flags;		/* various flags */
} ;

static struct interface	ife[PORT_NO];

static void getip(void)
{
	int		i, fd, rlen;
	struct ifreq	ifr;

	dbg_printf("%s,%d:PORT_NO=%d\n", __FILE__, __LINE__, PORT_NO);
	// initial variable
	for ( i=0; i<PORT_NO; i++ ) {
		ife[i].flags = 0;
		memset(&ife[i].addr, 0, sizeof(struct sockaddr));
		memset(&ife[i].netmask, 0, sizeof(struct sockaddr));
		ife[i].has_ip = 0;
		sprintf(ife[i].name, "%s%d", INTERFACE_PREFIX_NAME, i);
	}

	// create a socket to get IP */
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if ( fd < 0 ) {
		dbg_printf("Open socket fail [%d] !\n", errno);
		return;
	}
	dbg_printf("%s,%d\n", __FILE__, __LINE__);

	// to get interface setting
	for ( i=0; i<PORT_NO; i++ ) {
		strcpy(ifr.ifr_name, ife[i].name);
		if ( ioctl(fd, SIOCGIFFLAGS, &ifr) < 0 ) {
			dbg_printf("Port [%d] get socket flag fail [%d] !\n", i, errno);
			continue;
		}
		ife[i].flags = ifr.ifr_flags;

		strcpy(ifr.ifr_name, ife[i].name);
		ifr.ifr_addr.sa_family = AF_INET;
		if ( ioctl(fd, SIOCGIFADDR, &ifr) == 0 ) {
			ife[i].has_ip = 1;
			memcpy(&ife[i].addr, &ifr.ifr_addr, sizeof(struct sockaddr));

			strcpy(ifr.ifr_name, ife[i].name);
			if ( ioctl(fd,  SIOCGIFNETMASK, &ifr) == 0 ) {
				ife[i].netmask = ifr.ifr_netmask;
			} else {
				dbg_printf("Port [%d] get ip netmask fail [%d] !\n", i, errno);
				continue;
			}
		} else {
			dbg_printf("Port [%d] get ip address fail [%d] !\n", i, errno);
			continue;
		}
	}
	dbg_printf("%s,%d\n", __FILE__, __LINE__);

exit_getip:
	close(fd);
}

static void display(void)
{
	lcm_xy_t	pos;
	int		i, l;
	char		str[32];

	for ( i=0, l=0; i<PORT_NO; i++ ) {
		pos.x = 0;
		pos.y = l++;
		LCM_goto_xy(&pos);
		LCM_clean_line();
		sprintf(str, "LAN%d IP config:", i+1);
		LCM_printf(str);

		pos.x = 0;
		pos.y = l++;
		LCM_goto_xy(&pos);
		LCM_clean_line();
		if ( ife[i].flags & IFF_UP ) {
			if ( ife[i].addr.sa_family == 0xFFFF || ife[i].addr.sa_family == 0 ) {
				strcpy(str, "None set");
			} else {
				strcpy(str, inet_ntoa(((struct sockaddr_in *)&ife[i].addr)->sin_addr));
			}
		} else {
			strcpy(str, "Disable");
		}

		LCM_printf(str);

		pos.x = 0;
		pos.y = l++;
		LCM_goto_xy(&pos);
		LCM_clean_line();
		if ( ife[i].flags & IFF_UP ) {
			if ( ife[i].netmask.sa_family == 0xFFFF || ife[i].netmask.sa_family == 0 ) {
				strcpy(str, "None set");
			} else {
				strcpy(str, inet_ntoa(((struct sockaddr_in *)&ife[i].netmask)->sin_addr));
			}
		} else {
			strcpy(str, "Disable");
		}
		LCM_printf(str);
	}

	pos.x = 0;
	pos.y = l++;
	LCM_goto_xy(&pos);
	LCM_clean_line();
	LCM_printf("System ready !!!");
	pos.x = 0;
	pos.y = l++;
	LCM_goto_xy(&pos);
	LCM_clean_line();
}

int main(int argc, char *argv[])
{
	int	i;

	dbg_printf("%s,%d\n", __FILE__, __LINE__);
	LCM_cls();

loop_get:
	dbg_printf("%s,%d\n", __FILE__, __LINE__);
	getip();
	dbg_printf("%s,%d\n", __FILE__, __LINE__);
	display();
	dbg_printf("%s,%d\n", __FILE__, __LINE__);
	for ( i=0; i<PORT_NO; i++ ) {
		if ( ife[i].flags & IFF_UP ) {
			if ( ife[i].netmask.sa_family == 0xFFFF || ife[i].netmask.sa_family == 0 ) {
				sleep(1);
				dbg_printf("%s,%d\n", __FILE__, __LINE__);
				goto loop_get;
			}
		}
	}

	return 0;
}
