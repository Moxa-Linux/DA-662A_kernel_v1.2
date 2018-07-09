#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include <mxhw_crypto_io.h>

#define MXCIPHER_DEV_NODE		"/dev/mxcrypto"	/* device node */

int
mxcrypto_uio_register(u_int algo, u_int mode, u_char *keys, u_int bits, int type)
{
	CIPHER 	info;
	int 	fd, i;
	
	DBG("mxhw_userio: mxcrypto_uio_register %d %d %d\n", 11,algo,mode);
	
	/* open a new context */
	if ( (fd = open(MXCIPHER_DEV_NODE, O_RDWR, 0)) < 0 )
		return fd;

	/* register this context with parameters */
	info.type = (type)? 1:0;
	info.algo = algo;
	info.mode = mode;
	info.bits = bits;
	memcpy(info.keys, keys, MAX_MXCIPHER_LEN_KEYS);
	if ( (i = ioctl(fd, IOCTLSET_MXCIPHER_INFO, &info)) < 0 )
	{
		close(fd);
		fd = i;
	}
	DBG("mxhw_userio: mxcrypto_uio_register %d\n", 99);
	return fd;
}

__inline__ int
mxcrypto_uio_perform(int ctrl, const u_char *input, u_char *output, u_long length, 
					u_char *ivec, u_int ilen)
{
	int		n=0;
	u_char	data[MAX_CIPHER_PACKET], *ibuf, *obuf;

	DBG("mxhw_userio: mxcrypto_uio_perform %d %ld %d\n", 11, length, ilen);
	if (ctrl<=0)
		return -1;
		
	if (ivec!=NULL && ilen>0)
	{
		if ((length+ilen)>MAX_CIPHER_PACKET)
			return -2;
		obuf = ibuf = data;
		memcpy(ibuf, input, length);
		memcpy(ibuf+length, ivec, ilen);
		length += ilen;
	}	
	else
	{
		ibuf = (u_char*) input;
		obuf = output;
	}
	DBG("mxhw_userio: mxcrypto_uio_perform %d %ld %d\n", 22, length, ilen);
	/* send request to the engine and wait for response */
	if ((n=write(ctrl, ibuf, length)) < 0)
	{
		DBG("error: write %d\n", n);
		return n;
	}
	DBG("mxhw_userio: mxcrypto_uio_perform %d %ld %d\n", 33, length, ilen);
	if ((n=read(ctrl, obuf, length)) < 0)
	{
		DBG("error: read %d\n", n);
		return n;
	}
	DBG("mxhw_userio: mxcrypto_uio_perform %d %ld %d\n", 44, length, ilen);
	if (ivec!=NULL && ilen>0) 
	{
		length -= ilen;
		memcpy(output,	obuf, length);
		memcpy(ivec,	obuf+length, ilen);
	}
	DBG("mxhw_userio: mxcrypto_uio_perform %d\n", 99);
	return 0;
}

int
mxcrypto_uio_close(int fd)
{
	int size=0;
	
	if ( ioctl(fd, 99, &size) < 0 )
		return -1;
	close(fd);
	return size;
}

#ifdef OVERFLOW_TEST
int
mxcrypto_uio_write(int ctxId, const u_char *input, u_long len, u_char *ivec, int ilen)
{
	int n=0;
	
	/* send request to the engine and wait for response */
	if ((n=write(ctxId, input, len+ilen)) < 0)
		DBG("error: write/read %d\n", n);
	return n;
}

int
mxcrypto_uio_read(int ctxId, u_char *output, u_long len, u_char *ivec, int ilen)
{
	int n=0;
	
	/* send request to the engine and wait for response */
	if ((n=read(ctxId, output, len+ilen)) < 0)
		DBG("error: write/read %d\n", n);
	return n;
}

int
mxcrypto_uio_queue(int ctxId)
{
	int n=0;
	
	if (ioctl(ctxId, 100, &n)<0)
		return -1;
	return n;
}
#endif
