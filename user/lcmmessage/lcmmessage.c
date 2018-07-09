
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
#include	"id.h"

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

// function pointer for callback function registeration
int (*lcmmessage)(int argc, char **argv);

/*
   following LCM function calls are the same in UC and DA
*/

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

static void LCM_printf(char *str)
{
	int	fd;

	fd = open(DEV_NAME, O_RDWR);
	if ( fd < 0 )
		return;
	write(fd, str, strlen(str));
	close(fd);
}

static void LCM_get_xy(lcm_xy_t *xy)
{
	int	fd;

	fd = open(DEV_NAME, O_RDWR);
	if ( fd < 0 )
		return;
	ioctl(fd, IOCTL_LCM_GET_XY, xy);
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

static void     LCM_cursor_on(void)
{
        int     fd;

        fd = open(DEV_NAME, O_RDWR);
        if ( fd < 0 )
                return;
        ioctl(fd, IOCTL_LCM_CURSOR_ON, NULL);
        close(fd);
}

static void     LCM_cursor_off(void)
{
        int     fd;

        fd = open(DEV_NAME, O_RDWR);
        if ( fd < 0 )
                return;
        ioctl(fd, IOCTL_LCM_CURSOR_OFF, NULL);
        close(fd);
}

static void     LCM_blink_on(void)
{
        int     fd;

        fd = open(DEV_NAME, O_RDWR);
        if ( fd < 0 )
                return;
        ioctl(fd, IOCTL_LCM_BLINK_ON, NULL);
        close(fd);
}

static void     LCM_blink_off(void)
{
        int     fd;

        fd = open(DEV_NAME, O_RDWR);
        if ( fd < 0 )
                return;
        ioctl(fd, IOCTL_LCM_BLINK_OFF, NULL);
        close(fd);
}

/*
   following LCM function calls are different in UC and DA
*/

static void uc7420_LCM_auto_scroll_on(void)
{
	int	fd;

	fd = open(DEV_NAME, O_RDWR);
	if ( fd < 0 )
		return;	
	ioctl(fd, IOCTL_LCM_AUTO_SCROLL_ON, NULL);
	close(fd);
}

static void uc7420_LCM_auto_scroll_off(void)
{
	int	fd;

	fd = open(DEV_NAME, O_RDWR);
	if ( fd < 0 )
		return;	
	ioctl(fd, IOCTL_LCM_AUTO_SCROLL_OFF, NULL);
	close(fd);
}

int uc7420_lcmmessage(int argc, char **argv) {
	int	i;
	int	clean=0, linefeed=0, message=0;

	dbg_printf("%s[%d]\n",__FUNCTION__,__LINE__);
	for ( i=1; i<argc; i++ ) {
		if ( argv[i][0] == '-' ) {
			switch ( argv[i][1] ) {
			case 'c' :	// clean LCM
				clean = 1;
				break;
			case 'l' :	// line feed after show message
				linefeed = 1;
				break;
			case 'm' :	// show message
				i++;
				if ( i < argc )
					message = i;
				break;
			case 's' :	// auto scroll
				i++;
				if ( i < argc ) {
					if ( argv[i][0] == '0' )
						uc7420_LCM_auto_scroll_off();
					else if ( argv[i][0] == '1' )
						uc7420_LCM_auto_scroll_on();
				}
				break;
			case 'h' :	// show help file
				printf("Usage : %s [-c] [-l] [-m message] [-h][-s 0|1]\n", argv[0]);
				printf("\t-c\t\tClear LCM before display message on LCM\n");
				printf("\t-l\t\tAfter display message will be line feed\n");
				printf("\t-m message\tDisplay message context\n");
				printf("\t-h\t\tDisplay this help message\n");
				printf("\t-s\t\tLCM auto scroll on or off\n");
				return 0;
			}
		}
	}
	if ( clean )
		LCM_cls();
	if ( message ) {
		LCM_printf(argv[message]);
		if ( linefeed )
			LCM_printf("\n");
	}

}
/*
  following LCM functions are used in DA
*/

int da66x_lcmmessage(int argc, char **argv) {
	int	i;
	int	clean=0, linefeed=0, message=0;
	lcm_xy_t xy;

	dbg_printf("%s[%d]\n",__FUNCTION__,__LINE__);

	for ( i=1; i<argc; i++ ) {
		if ( argv[i][0] == '-' ) {
			switch ( argv[i][1] ) {
			case 'c' :	// clean LCM
				clean = 1;
				break;
			case 'l' :	// line feed after show message
				linefeed = 1;
				LCM_get_xy(&xy);		
				xy.x = 0x0; // changes to line 1 or line 2, only for DA-660
				xy.y ^= 0x01;
				LCM_goto_xy(&xy);
				break;
			case 'm' :	// show message
				i++;
				if ( i < argc )
					message = i;
				break;
			case 'g' :  // get_XY
				LCM_get_xy(&xy);
				printf("Now position : x=%x;y=%x\n", xy.x, xy.y);
				break;
			case 'a' :
				LCM_clean_line();
				break;
			case 'u' :      // cursor on/off
                                i++;
                                if ( i >= argc ) {
BadCursorParam:
                                        printf("Bad parameter for cursor setting !\n");
                                        return -1;
                                }
                                if ( strcmp(argv[i], "on") == 0 )
                                        LCM_cursor_on();
                                else if ( strcmp(argv[i], "off") == 0 )
                                        LCM_cursor_off();
                                else
                                        goto BadCursorParam;
                                break;
                        case 'b' :      // blink on/off
                                i++;
                                if ( i >= argc ) {
BadBlinkParam:
                                        printf("Bad parameter for blink setting !\n");
                                        return -1;
                                }
                                if ( strcmp(argv[i], "on") == 0 )
                                        LCM_blink_on();
                                else if ( strcmp(argv[i], "off") == 0 )
                                        LCM_blink_off();
                                else
                                        goto BadBlinkParam;
                                break;
			case 'h' :	// show help file
				printf("Usage : %s [-a] [-g] [-c] [-l] [-m message] [-u on|off] [-b on|off] [-h]\n", argv[0]);
				printf("\t-a\t\tClear this line on LCM\n");
				printf("\t-g\t\tGet the current position on LCM\n");
				printf("\t-c\t\tClear LCM before display message on LCM\n");
				printf("\t-l\t\tLine feed\n");
				printf("\t-m message\tDisplay message context\n");
				printf("\t-u on|off\tSet cursor on or off\n");
                                printf("\t-b on|off\tSet blinking on or off\n");
				printf("\t-h\t\tDisplay this help message\n");
				return 0;
			}
		}
	}
	if ( clean ) {
		LCM_cls();	
		usleep(1000);
	}
	if ( message ) {
		LCM_printf(argv[message]);
	}
	return 0;

}

int main(int argc, char *argv[])
{
	unsigned int	swid;
	int		ret=0;

	swid=get_swid();

	// register the lcmmessage callback function for each model
	switch(swid) {
	case UC7402_PLUS_SOFTWARE_ID:
	case UC7408_PLUS_SOFTWARE_ID:
		lcmmessage=NULL;
		break;
	case UC7420_PLUS_SOFTWARE_ID:
	case UC7410_PLUS_SOFTWARE_ID:
		lcmmessage=uc7420_lcmmessage;
		break;
	case DA660_PLUS_SOFTWARE_ID:
	case DA661_SOFTWARE_ID:
	case DA662_SOFTWARE_ID:
	case DA663_SOFTWARE_ID:
	case DA662A_8_SOFTWARE_ID :
	case DA662A_16_SOFTWARE_ID :
		lcmmessage=da66x_lcmmessage;
		break;
	default:
			printf("The software id: %lx does not supported", swid);
	}

	if( lcmmessage )
		ret=lcmmessage(argc, argv);

	return ret;
}
