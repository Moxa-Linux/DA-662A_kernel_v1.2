
/**
 * @file read_t3_flag.c
 * @brief this program is read the T3 flag on or off at now.
 *
 * History:
 * Date		Author			Comment
 * 2014/1/24	Victor Yu.		Initialize it.
 */
#include <stdio.h>
#include <stdlib.h>
#include <redboot_config.h>

int	main(int argc, char *argv[])
{
	int	t3flag;

	if ( config_init() < 0 ) {
		printf("Initialize redboot configuration fail !\n");
		exit(-1);
	}
	if ( config_get("t3_module_flag", &t3flag) < 0 ) {
		printf("To read T3 flag fail !\n");
		exit(-1);
	}
	if ( t3flag )
		exit(1);
	exit(0);
}
