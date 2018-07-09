
/**
 * @file set_t3_flag.c
 * @brief this program is set the T3 flag on.
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
	int	t3flag=0;

	if ( config_init() < 0 ) {
		printf("Initialize redboot configuration fail !\n");
		return -1;
	}
	if ( config_set("t3_module_flag", &t3flag) < 0 ) {
		printf("To clear T3 flag fail !\n");
		return -1;
	}
	if ( config_write() < 0 ) {
		printf("Save redboot configuration fail !\n");
		return -1;
	}
	return 0;
}
