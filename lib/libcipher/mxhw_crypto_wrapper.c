#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

#include <mxhw_crypto_userio.h>
#include <mxhw_crypto_wrapper.h>

static SSNTAB global_sessions[MAX_CIPHER_CLIENTS];

#define SSN_FREE 			0
#define SSN_CREATE 			1
#define SSN_ENCRYPT 		(1<<1)
#define SSN_DECRYPT 		(1<<2)

#define SET_SSN_FREE(s) 	memset(s,0,sizeof(SSNTAB))
#define CHK_SSN_ENCRYPT(s) 	(s->flag&SSN_ENCRYPT)
#define CHK_SSN_DECRYPT(s) 	(s->flag&SSN_DECRYPT)

#define SSNFIND(s,k) \
{ int i; s = NULL;\
	for (i=0; i < MAX_CIPHER_CLIENTS; i++) \
		if (k && global_sessions[i].bkey==k)	{ s = &global_sessions[i]; break; } \
}

static SSNTAB*
mxcrypto_find(void *key)
{
	SSNTAB *s;
	
	SSNFIND(s,key);
	return s;
}

SSNTAB*
mxcrypto_create(const void *userKey, u_int klen, void *key, u_int blen, u_int is_aes_key)
{
	SSNTAB *s=NULL;
	static int first=1;
	int i;

	if (first)
	{
		memset(global_sessions, 0, MAX_CIPHER_CLIENTS*sizeof(SSNTAB));
		first = 0;
	}
	/* find a session by binary key */
	if ((s=mxcrypto_find(key)))
		mxcrypto_close(key);

	for (i=0; i < MAX_CIPHER_CLIENTS; i++)
	{
		s = &global_sessions[i];
		if (s->flag==SSN_FREE)
		{
			s->enId = s->deId = 0; 
			s->flag = SSN_CREATE;
			s->bkey = key;
			s->blen = blen;
			s->klen = (is_aes_key)? (klen>>3):klen; /* bytes for both of the DES and AES */
			memcpy(s->ukey, userKey, s->klen);
			return s;
		}
	}
	return NULL;	
}

static disable_hw=0;
void
mxcrypto_set_software(int flag)
{
	disable_hw = flag;
}

/* 
	This is an over-all interface performing cipher tasks for all kinds of algorithms 
	in 4/5 modes.

	note: typeless keys and ivec to offer a maximum room for any application.

	returns:	> 0, use software approach. 
				= 0, hardware ok. 
				< 0, hardware failure.
*/
int
mxcrypto_perfrom(u_int algo, u_int mode, const u_char *input, u_char *output, u_long length, 
					void *key1, void *key2, void *key3, void *ivec, u_int numbits, int enc)
{
	SSNTAB	*s;
	int		fd;
	
	DBG("mxcrypto_perfrom=%d %d %lu %d\n", algo, mode, length, enc);
	
	if (disable_hw)
	{
		DBG("SW %s\n", enc? "ENCRYPT":"DECRYPT");
		return 1;
	}
	/* find a session by binary key */
	SSNFIND(s,key1);
	if (s==NULL)	
		return 1;
	DBG("HW %s\n", enc? "ENCRYPT":"DECRYPT");
	
	fd = (enc!=0)? s->enId:s->deId;
	if (fd==0)
	{
		SSNTAB	*s2=NULL, *s3=NULL;

		DBG("--------mxcrypto_perfrom %d\n", 22);
		if (s->flag==SSN_CREATE)
		{
			DBG("--------mxcrypto_perfrom %d\n", 33);
			/* if it is triple des */
			if (algo==MXCIPHER_ALGO_3DES)
			{
				if (key2==NULL || key3==NULL)
				{
					SET_SSN_FREE(s);
					return 2;
				}
				s2 = mxcrypto_find(key2);
				s3 = mxcrypto_find(key3);
				if (s2==NULL || s3==NULL)
				{
					SET_SSN_FREE(s);
					return 2;
				}
				/* put keys together */
				memcpy(s->ukey+s->klen, s2->ukey, s2->klen);
				s->klen += s2->klen;
				memcpy(s->ukey+s->klen, s3->ukey, s3->klen);
				s->klen += s3->klen;
				DBG("--------mxcrypto_perfrom %d\n", 44);
			}
		}
		if (algo==MXCIPHER_ALGO_AES)
		{
				int klen = s->klen<<3;
				
				DBG("--------mxcrypto_perfrom aes klen %d\n", s->klen);
				if (klen==128) 	   	algo = MXCIPHER_ALGO_AES128;
				else if (klen==192) algo = MXCIPHER_ALGO_AES192;
				else if (klen==256) algo = MXCIPHER_ALGO_AES256;
				else
				{
					SET_SSN_FREE(s);
					return 3;
				}
		}
		/* after keys have been put together */
		if (mode==MXCIPHER_MODE_ECB)
			s->blen = 0;
			
		DBG("--------register hardware approache=%d %d %d %d\n", algo, mode, s->blen, s->klen);
		
		/* register a contexts and return a file descriptor */
		fd = mxcrypto_uio_register(algo, mode, s->ukey, numbits, enc);
		/* no matterwhat, free up 2 tmp sessions */
		if (s2 && s2!=s) SET_SSN_FREE(s2);
		if (s3 && s3!=s) SET_SSN_FREE(s3);
		if (fd < 0)
		{
			SET_SSN_FREE(s);
			return 4;
		}		
		if (enc){ s->flag|=SSN_ENCRYPT; s->enId=fd; }
		else	{ s->flag|=SSN_DECRYPT; s->deId=fd; }
	}

	if (mxcrypto_uio_perform(fd, input, output, length, ivec, s->blen)==0)
	{
		DBG("--------%s ok hardware approache=%d %d %d %d\n", enc? "ENCRYPT":"DECRYPT", 
					algo, mode, s->blen, s->klen);
		return 0;
	}
	DBG("--------fial try hardware approache = %d\n", fd);
	if (fd>0) mxcrypto_close(key1); /* since we turn hw to sw, so free it now */
	return 5; 
}

/* before free up the memory for keys, close the file */
int
mxcrypto_close(void *key)
{
	SSNTAB	*s = mxcrypto_find(key);
	int 	size=0;

	/* find a session by binary key */
	if (s)
	{
		
		if (s->enId>0) 
			size += mxcrypto_uio_close(s->enId);
		if (s->deId>0) 
			size += mxcrypto_uio_close(s->deId);
		SET_SSN_FREE(s);
	}
	return size;
}


