
/*
 * This program sets the key for application to lock it. The key must be the same as what you
 * encrypte the application on the PC linux platform. 
 * You must set encrypte the application on the PC linux platform with the key value. 
 *
 * History:
 * Date		Author				Comment
 * 12-21-2005	Victor Yu.			Create it.
 */
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include        <fcntl.h>
#include        <errno.h>
#include        <mtd/mtd-user.h>
#include        <sys/types.h>
#include        <sys/ioctl.h>
#include	<sys/socket.h>
#include	<sys/time.h>
#include        <netinet/in.h>
#include        <netdb.h>
#include	<unistd.h>
#include        <arpa/telnet.h>
#include        <signal.h>
#include        <termios.h>
#include        <sys/stat.h>
#include        <sys/reboot.h>
#include	<errno.h>

#ifdef DEBUG
#define dbg_printf(x...)        printf(x)
#else
#define dbg_printf(x...)
#endif


#define VERSION		"1.0"
#define MAX_KEY_LEN	8

#if (defined ARCH_UC8400) || (defined ARCH_UC8000) || (defined ARCH_UC8430) || (defined ARCH_DCU8620)

#include	"redboot_config.h"

int xscale_setkey(void *privkey)
{
	if ( config_init() < 0 ) {
		printf("Configuration initialize fail !\n");
		return(-1);
	}

	if( config_set_swlock(privkey) < 0 ) {
		printf("config_set_swlock() fail\n");
		return(-2);
	}
	
	if ( config_write() < 0 ) {
		printf("config_write() fail !\n");
		return(-3);
	}

	return 0;
}


#else

#define BOOT_MTD	"/dev/mtd0"

int moxaart_setkey(void *privkey)
{
	int		i, fd;
	char		*buf;
	mtd_info_t	meminfo;
	erase_info_t	erase;

	fd = open(BOOT_MTD, O_RDWR);
	if ( fd < 0 ) {
		printf("Open flash fail !\n");
		exit(1);
	}
	if ( ioctl(fd, MEMGETINFO, &meminfo) ) {
		printf("Get flash information fail !\n");
		close(fd);
		exit(1);
	}
	buf = malloc(meminfo.erasesize);
	if ( buf == NULL ) {
		printf("Memory allocate fail !\n");
		close(fd);
		exit(1);
	}
	if ( read(fd, buf, meminfo.erasesize) != meminfo.erasesize ) {
		printf("Backup flash data fail !\n");
		free(buf);
		close(fd);
		exit(1);
	}
	memcpy(&buf[0x68], privkey, MAX_KEY_LEN);
	erase.length = meminfo.erasesize;
	erase.start = 0;
	if ( ioctl(fd, MEMUNLOCK, &erase) < 0 ) {
		if ( errno != EOPNOTSUPP ) {
			printf("Unlock flash fail !\n");
			free(buf);
			close(fd);
			exit(1);
		}
	}
	if ( ioctl(fd, MEMERASE, &erase) ) {
		printf("Erase flash fail !\n");
		free(buf);
		close(fd);
		exit(1);
	}
	lseek(fd, 0, SEEK_SET);
	write(fd, buf, meminfo.erasesize);
	close(fd);
	free(buf);
}

#endif

int	main(int argc, char *argv[])
{
	int		i;
	int (*setkey_fun)(void *privkey);
#if (defined ARCH_UC8400) || (defined ARCH_UC8000) || (defined ARCH_UC8430) || (defined ARCH_DCU8620)
	unsigned char	key[MAX_KEY_LEN]={0, 0, 0, 0, 0, 0, 0, 0};
	setkey_fun=xscale_setkey;
#else
	char		key[MAX_KEY_LEN]={0, 0, 0, 0, 0, 0, 0, 0};
	setkey_fun=moxaart_setkey;
#endif

	if ( argc != 2 )
		goto usage;

	i = strlen(argv[1]);
	
	if ( i > MAX_KEY_LEN || i <= (MAX_KEY_LEN/2) ) {
		printf("Sorry! We only support key length %d ~ %d bytes\n", (MAX_KEY_LEN/2)+1, MAX_KEY_LEN);
		exit(0);
	}

	if ( i <= MAX_KEY_LEN )
		memcpy(key, argv[1], i);
	else
		memcpy(key, argv[1], MAX_KEY_LEN);

	dbg_printf("%x%x%x%x%x%x%x%x\n", key[0], key[1], key[2], key[3], key[4], key[5], key[6], key[7]);

	setkey_fun((void*)key);

// add by Victor Yu. 03-31-2010, read again to flush cache
#if (defined ARCH_UC8400) || (defined ARCH_UC8000) || (defined ARCH_UC8430) || (defined ARCH_DCU8620)
	config_init();
#endif
	exit(0);

usage:
	printf("Usage: %s keystring\n", argv[0]);
	printf("	keystring - You want to set the key string to lock\n");
	printf("		your application. The maximum keystring\n");
	printf("		length is 8.\n");
	exit(0);
}
