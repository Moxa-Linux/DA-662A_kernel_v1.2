
/*
 * History:
 * Date		Author				Comment
 * 02-22-2006	Victor Yu.			Create it.
 */
#include	<stdio.h>
#include	<stdlib.h>
#include        <fcntl.h>
#include        <sys/ioctl.h>
#include	<unistd.h>
#include        <signal.h>

#ifdef ARCH_DA662A
#define IOCTL_LCM_BASE		0x100
#define IOCTL_SW_READY_ON	(11+IOCTL_LCM_BASE)
#define IOCTL_SW_READY_OFF	(12+IOCTL_LCM_BASE)
#else
#define MOXA_MISC_MINOR		105
#define IOCTL_SW_READY_ON	_IO(MOXA_MISC_MINOR,1)
#define IOCTL_SW_READY_OFF	_IO(MOXA_MISC_MINOR,2)
#endif

#ifdef DEBUG
#define dbg_printf(x...)        printf(x)
#else
#define dbg_printf(x...)
#endif

#define VERSION			"1.1"

int	main(int argc, char *argv[])
{
	int	fd, highlow;

	if ( argc < 2 ) 
		exit(1);
	sscanf(argv[1], "%d", &highlow);
	dbg_printf("Set ready LED high or low = %d\n", highlow);
#ifdef ARCH_DA662A
	fd = open("/dev/sw_ready", O_RDWR);
#else
	fd = open("/dev/mxmisc", O_RDWR);
#endif
	if ( fd < 0 )
		exit(1);
	if ( highlow )
		ioctl(fd, IOCTL_SW_READY_ON, NULL);
	 else 
		ioctl(fd, IOCTL_SW_READY_OFF, NULL);
	close(fd);
}
