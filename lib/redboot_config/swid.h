#ifndef __MOXA_SWID__
#define __MOXA_SWID__

#if ( defined ARCH_W341 )
// Defined W341 Device ID
#define W341_ID               0x0341
#define W341_AP_ID            0x80000341
#define W341_SOFTWARE_ID      0x0341

#elif ( defined ARCH_W345 )
// Defined W341 Device ID
#define W345_ID               0x0345
#define W345_AP_ID            0x80000345
#define W345_SOFTWARE_ID      0x0345

#elif ( defined ARCH_W345_IMP1 )
// Defined W341_16128 Device ID
#define W345_IMP1_ID               0x2345
#define W345_IMP1_AP_ID            0x80002345
#define W345_IMP1_SOFTWARE_ID      0x2345

//leo 2007.08.20
#elif ( defined ARCH_UC_7101_LX )
#define UC_7101_LX_ID               0x7101
#define UC_7101_LX_AP_ID            0x80007101
#define UC_7101_LX_SOFTWARE_ID      0x7101

#elif ( defined ARCH_UC_7112_LX_PLUS )
#define UC_7112_LX_PLUS_ID               0x7112
#define UC_7112_LX_PLUS_AP_ID            0x80017112
#define UC_7112_LX_PLUS_SOFTWARE_ID 	 0x7112

#elif ( defined ARCH_UC_7112_LX_PLUS_LITON )
#define UC_7112_LX_PLUS_LITON_ID               0x7112
#define UC_7112_LX_PLUS_LITON_AP_ID            0x80027112
#define UC_7112_LX_PLUS_LITON_SOFTWARE_ID 	 0x7112

#elif ( defined ARCH_W311 )
#define W311_ID               0x0311
#define W311_AP_ID            0x80000311
#define W311_SOFTWARE_ID 	 0x0311

#elif ( defined ARCH_W321 )
#define W321_ID               0x0321
#define W321_AP_ID            0x80000321
#define W321_SOFTWARE_ID 	 0x0321

#elif ( defined ARCH_W315 )
#define W315_ID               0x0315
#define W315_AP_ID            0x80000315
#define W315_SOFTWARE_ID 	 0x0315

//johnson add for CV
#elif ( defined ARCH_W315_NJKL )
#define W315_NJKL_ID               0x3315
#define W315_NJKL_AP_ID            0x80003315
#define W315_NJKL_SOFTWARE_ID 	 0x3315

//johnson add for CV
#elif ( defined ARCH_W325 )
#define W325_ID               0x0325
#define W325_AP_ID            0x80000325
#define W325_SOFTWARE_ID 	 0x0325

#elif ( defined ARCH_IA240 )
#define IA240_ID		0x0240
#define IA240_AP_ID		0x80000240
#define IA240_SOFTWARE_ID	0x0240

#elif (defined ARCH_UC8400) || (defined ARCH_UC8400LE)|| (defined ARCH_DCU8620)
// For UC-8410/8411/8416/8418/8481/8482/8483, DCU-8620 identification
#define UC8410_ID		0x8410
#define UC8410_AP_ID		0x80008410
#define UC8410_SOFTWARE_ID	0x8410
#define UC8416_ID		0x8416
#define UC8416_AP_ID		0x80008416
#define UC8416_SOFTWARE_ID	0x8416
#define UC8418_ID		0x8418
#define UC8418_AP_ID		0x80008418
#define UC8418_SOFTWARE_ID	0x8418
#define UC8430_ID		0x8430
#define UC8430_AP_ID		0x80008430
#define UC8430_SOFTWARE_ID	0x8430
#define UC8411_ID		0x8411
#define UC8411_AP_ID		0x80008411
#define UC8411_SOFTWARE_ID	0x8411
#define UC8481_ID		0x8481
#define UC8481_AP_ID		0x80008481
#define UC8481_SOFTWARE_ID	0x8481
#define UC8482_ID		0x8482
#define UC8482_AP_ID		0x80008482
#define UC8482_SOFTWARE_ID	0x8482
#define UC8483_ID		0x8483
#define UC8483_AP_ID		0x80008483
#define UC8483_SOFTWARE_ID	0x8483
#define DCU8620_ID		0x8620
#define DCU8620_AP_ID		0x80008620
#define DCU8620_SOFTWARE_ID	0x8620

#define BUF_LEN		1024
static char proc_buf[BUF_LEN];

#include <unistd.h>
ssize_t read_procfs(char *procfs, char *buf) {
	int	fh;
	char	*ptmp;

	fh=open(procfs, O_RDONLY);
	for ( ptmp=buf; read(fh,  ptmp, BUF_LEN) != 0 ; ptmp++ );
	close(fh);

	return (ptmp-buf);
}

ssize_t pattern_match_count(const char *buf, char *pattern) {
	int	i;
	ssize_t	count=0;
  
	for ( i=0; i<strlen(buf); i++) {
		if ( strncmp( (buf+i), pattern, strlen(pattern)) == 0 )
			count++;
	}

	return count;
}

unsigned int get_swid() {
	int	nRTL8139=0, nCAN=0, nMU860=0, nSM502=0, nATH9K=0;

	nRTL8139=pattern_match_count(proc_buf, "10ec8139");
	nCAN=pattern_match_count(proc_buf, "13930100");
	nMU860=pattern_match_count(proc_buf, "13930001");
	nSM502=pattern_match_count(proc_buf, "126f0501");
	nATH9K=pattern_match_count(proc_buf, "168c0029");

	if ( nRTL8139 ==0 && nMU860 == 1 && nCAN == 0 && nATH9K == 1 )
		return UC8481_ID;
	else if ( nRTL8139 ==0 && nMU860 == 1 && nCAN == 0 && nATH9K == 2 )
		return UC8482_ID;
	else if ( nRTL8139 ==0 && nMU860 == 1 && nCAN == 0 && nATH9K == 3 )
		return UC8483_ID;
	else if ( nRTL8139 ==0 && nMU860 == 1 && nCAN == 0 && nSM502 == 0 )
		return UC8411_ID;
	else if ( nRTL8139 ==2 && nMU860 == 1 && nCAN == 0 && nSM502 == 0 )
	#if (defined ARCH_DCU8620 )
		return DCU8620_ID;
	#else
		return UC8416_ID;
	#endif
	else if ( nRTL8139 ==1 && nMU860 == 1 && nCAN == 1 && nSM502 == 0 )
		return UC8418_ID;
	else if ( nRTL8139 ==1 && nMU860 == 1 && nCAN == 0 && nSM502 == 1 )
		return UC8430_ID;
	else if ( nRTL8139 == 1 && nMU860 == 1 && nCAN == 0 && nSM502 == 0 )
		return UC8410_ID;
	else
		return UC8410_ID;
}

#elif ( defined ARCH_UC8000 )
	#define UC8000_ID		0x8000
	#define UC8000_AP_ID		0x80008000
	#define UC8000_SOFTWARE_ID	0x8000

#elif defined(ARCH_UC8406)
#define UC8406_ID		0x8406
#define UC8406_AP_ID		0x80008406
#define UC8406_SOFTWARE_ID	0x8406

#elif ( defined ARCH_IA240_MARI )
	#define IA240_MARI_ID		0x0240
	#define IA240_MARI_AP_ID	0x80000240
	#define IA240_MARI_SOFTWARE_ID	0x0240

#elif ( defined ARCH_IA241 )
	#define IA241_ID		0x0241
	#define IA241_AP_ID		0x80000241
	#define IA241_SOFTWARE_ID	0x0241

#elif ( defined ARCH_IA241_32128 )
	#define IA241_32128_ID		0x1241
	#define IA241_32128_AP_ID	0x80001241
	#define IA241_32128_SOFTWARE_ID	0x1241

#elif ( defined ARCH_IA241_NSPS )
	#define IA241_NSPS_ID          0x2241
	#define IA241_NSPS_AP_ID       0x80002241
	#define IA241_NSPS_SOFTWARE_ID 0x2241

#elif defined(ARCH_IA261)
	#define ID		0x0269
	#define APID		0x80000269

#elif defined(ARCH_IA261_256)
	#define ID		0x1261
	#define APID		0x80001261

#elif defined(ARCH_IA262)
	#define ID		0x026a
	#define APID		0x8000026a

#elif defined(ARCH_EM2260)
	#define ID		0x026b
	#define APID		0x8000026b

#elif defined(ARCH_IA260)
	#define ID		0x026c
	#define APID		0x8000026c

#elif defined(ARCH_W406)
	#define ID		0x0406
	#define APID		0x80001406

#elif defined(ARCH_W416)
	#define ID		0x0416
	#define APID		0x80001416

#elif defined(ARCH_RNAS1200)
	#define ID		0x1200
	#define APID		0x80001200

#elif defined(ARCH_DA662A)
	#define DA662A_8_SOFTWARE_ID    0x60040020
	#define DA662A_16_SOFTWARE_ID   0x60040021
	#define DA662A_8_ID             0x6620
	#define DA662A_16_ID            0x6621
	#define DA662A_8_LX_AP_ID       0xE0006620
	#define DA662A_16_LX_AP_ID      0xE0006621
#endif

#endif //#ifndef __MOXA_SWID__
