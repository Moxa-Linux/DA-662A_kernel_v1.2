
/**
 * @file beeperon.c
 * @brief To let the beeper continue on.
 *
 * History:
 * Date		Author			Comment
 * 2014/1/24	Victor Yu.		Initialize it.
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/kd.h>

int	main(int argc, char *argv[])
{
	int	fd;

	fd = open("/dev/console", O_WRONLY);
        ioctl(fd, KDMKTONE, 100<<16);
        close(fd);
	return 0;
}
