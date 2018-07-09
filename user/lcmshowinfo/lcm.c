/*---------------------------------------------------------------------------*/
/**
  @file		lcm.c
  @brief	This program demo how to code lcm program on UC.

  The lcm screen can display 16*8 words (one word is 8*8 pix).
  Usage :
	- Compile this file and execute on UC7000 series.
	- The Progarm will show a menu with number,choose one to control lcm.

  History :
	Version		Author		Date		Comment
	1.0		Hank		01-15-2004	Wrote it.
	2.0		AceLan		01-12-2006	Librarize
 */
/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "lcm.h"

#ifdef ARCH_DA662A
#define DEV_NAME	"/dev/sds_lcm"
#else
#define DEV_NAME	"/dev/lcm"
#endif

// ioctl() return 0 & -1, 0 ==> OK, -1 ==> FAIL
int LCMGetXY( int *x, int *y)
{
	lcm_xy_t lcm_xy;

	int fd= open(DEV_NAME, O_RDWR);
	if ( fd < 0 )
		return -1;

	ioctl( fd, IOCTL_LCM_GET_XY, &lcm_xy);
	*x= lcm_xy.x, *y= lcm_xy.y;

	close(fd);
}

int LCMCleanLine()
{
	int fd= open(DEV_NAME, O_RDWR);
        if ( fd < 0 )
                return -1;

	ioctl( fd, IOCTL_LCM_CLEAN_LINE, NULL);

	close(fd);
}

int LCMClear()
{
        int fd= open(DEV_NAME, O_RDWR);
        if ( fd < 0 )
                return -1;

        ioctl( fd, IOCTL_LCM_CLS, NULL);
	usleep(1000);

        close(fd);
}

int LCMGotoXY( int x, int y)
{
	lcm_xy_t lcm_xy;

        int fd= open(DEV_NAME, O_RDWR);
        if ( fd < 0 )
                return -1;

	lcm_xy.x= x, lcm_xy.y= y;
	ioctl( fd, IOCTL_LCM_GOTO_XY, &lcm_xy);

	close(fd);
}

// write() - to display message by sequence
int LCMPrint( char* str)
{
        int fd= open(DEV_NAME, O_RDWR);
        if ( fd < 0 )
                return -1;

	write( fd, str, strlen(str));

	close( fd);
}

/* Following io controls are only avaliable for UC-7410/7420-LX and UC-7410/7420-LX Plus */
int LCMBackLightOn()
{
        int fd= open(DEV_NAME, O_RDWR);
        if ( fd < 0 )
                return -1;

	ioctl( fd, IOCTL_LCM_BACK_LIGHT_ON, NULL);

	close( fd);
}

int LCMBackLightOff()
{
        int fd= open(DEV_NAME, O_RDWR);
        if ( fd < 0 )
                return -1;

	ioctl( fd, IOCTL_LCM_BACK_LIGHT_OFF, NULL);

	close( fd);
}
/* Following io controls are only avaliable for DA-660/661/662/663-LX */
int LCM_blink_off(void)
{
    int ret, fd;

    fd=open(DEV_NAME, O_RDWR);
    if (fd < 0)
        return -1;

    ret=ioctl(fd, IOCTL_LCM_BLINK_OFF, NULL);

    if (ret)
        return -1;

    return 0;
}

int LCM_blink_on(void)
{
    int ret, fd;

    fd=open(DEV_NAME, O_RDWR);
    if (fd < 0)
        return -1;
    ret=ioctl(fd, IOCTL_LCM_BLINK_ON, NULL);

    if (ret)
        return -1;

    return 0;
}

int LCM_cursor_off(void)
{
    int ret, fd;

    fd=open(DEV_NAME, O_RDWR);
    if (fd < 0)
        return -1;

    ret=ioctl(fd, IOCTL_LCM_CURSOR_OFF, NULL);

    if (ret)
        return -1;

    return 0;
}

int LCM_cursor_on(void)
{
    int ret, fd;

    fd=open(DEV_NAME, O_RDWR);
    if (fd < 0)
        return -1;
    ret=ioctl(fd, IOCTL_LCM_CURSOR_ON, NULL);

    if (ret)
        return -1;

    return 0;
}
