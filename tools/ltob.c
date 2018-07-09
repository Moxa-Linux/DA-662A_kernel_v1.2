/*
 * This is a program to do transfer from little endian binary file to big endian
 * file.
 *
 * History:
 * Date		Author			Comment
 * 04-25-2005	Victor Yu.		Create it.
 */
#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>

#define BUF_LEN		1024
#define BOUNDARY_BYTE	4
#define BOUNDARY_MASK	(BOUNDARY_BYTE-1)

static inline unsigned short swab16(unsigned short v)
{
        unsigned short  ret;

        ret = ((v & (unsigned short)0x00ffU) << 8) |
                ((v & (unsigned short)0xff00U) >> 8);
        return ret;
}

static inline unsigned int swab32(unsigned int v)
{
        unsigned int    ret;

        ret = ((v & (unsigned int)0x000000ffUL) << 24) |
                ((v & (unsigned int)0x0000ff00UL) << 8) |
                ((v & (unsigned int)0x00ff0000UL) >> 8) |
                ((v & (unsigned int)0xff000000UL) >> 24);
        return ret;
}

static char		buf[BUF_LEN];

int	main(int argc, char *argv[])
{
	int	infd, outfd, len, i;

	if ( argc != 3 ) {
		printf("Usage: %s input-file-name output-file-name\n", argv[0]);
		printf("	input-file-name		- the file which want to do\n");
		printf("	output-file-name	- the output file name\n");
		exit(1);
	}

	infd = open(argv[1], O_RDONLY);
	if ( infd < 0 ) {
		printf("Open file %s to read failed !\n", argv[1]);
		exit(1);
	}

	outfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if ( outfd < 0 ) {
		printf("Open file %s failed !\n", argv[2]);
		close(infd);
		exit(1);
	}

	while ( (len=read(infd, buf, BUF_LEN)) > 0 ) {
		if ( len & BOUNDARY_MASK ) {
			printf("Read the file boudary length is error !\n");
			break;
		}
		for ( i=0; i<len; i+=BOUNDARY_BYTE ) {
			*(unsigned int *)&buf[i] = swab32(*(unsigned int *)&buf[i]);
		}
		write(outfd, buf, len);
	}

	close(infd);
	close(outfd);
	exit(0);
}
