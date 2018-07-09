/*			Jimmy_chen@moxa.com.tw:
 *			kversion: get version from flash
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <getopt.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/termios.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <errno.h>

#ifdef DEBUG
#define dbg_printf(x...)	printf(x)
#else
#define dbg_printf(x...)
#endif

#if (defined ARCH_IA241)
#define PNAME	"IA241-LX"
#elif (defined ARCH_RNAS1200)
#define PNAME	"RNAS1200-LX"
#elif (defined ARCH_IA241_32128)
#define PNAME	"IA241-32128-LX"
#elif (defined ARCH_IA241_NSPS)
#define PNAME   "IA241-NSPS-LX"
#elif (defined ARCH_IA240)
#define PNAME	"IA240-LX"
#elif (defined ARCH_IA240_MARI)
#define PNAME   "IA240_MARI-LX"
#elif (defined ARCH_W341)
#define PNAME	"W341-LX"
#elif (defined ARCH_W321)
#define PNAME	"W321-LX"
#elif (defined ARCH_W311)
#define PNAME	"W311-LX"
#elif (defined ARCH_W345)
#define PNAME	"W345-LX"
#elif (defined ARCH_W345_IMP1)
#define PNAME	"W345(IMP1)-LX"
#elif (defined ARCH_W325)
#define PNAME	"W325-LX"
#elif (defined ARCH_W315)
#define PNAME	"W315-LX"
#elif (defined ARCH_W315_NJKL)
#define PNAME	"W315_NJKL-LX"
#elif (defined ARCH_UC_7112_LX_PLUS)
#define PNAME	"UC7112-Plus-LX"
#elif (defined ARCH_UC_7112_LX_PLUS_LITON)
#define PNAME	"UC7112-Plus-Liton-LX"
#elif (defined ARCH_IA261)
	#ifdef HOSON_ODM
#define PNAME	"Q3300-LX"
	#else
#define PNAME	"IA261-LX"
	#endif	// HOSON_ODM
#elif (defined ARCH_IA261_256)
#define PNAME   "IA261(256M)-LX"
#elif (defined ARCH_W406)
#define PNAME   "W406-LX"
#elif (defined ARCH_SMG1100)
#define PNAME   "SMG-1100"
#elif (defined ARCH_W416)
#define PNAME   "W416-LX"
#elif (defined ARCH_IA260)
#define PNAME   "IA260-LX"
#elif (defined ARCH_IA262)
#define PNAME   "IA262-LX"
#elif (defined ARCH_EM2260)
#define PNAME   "EM2260-LX"
#elif (defined ARCH_UC8406)
#define PNAME	"UC-8406-LX"
#elif (defined ARCH_UC1260)
#define PNAME	"FLI-1260"
#elif (defined ARCH_UC8400) || (defined ARCH_DCU8620)
#include	"redboot_config.h"
#include "swid.h" // define the software IDs
#define PNAME	"UC-8410-LX"
#elif (defined ARCH_DA662A)
	#define DA662A_8_SOFTWARE_ID    0x60046620
        #define DA662A_16_SOFTWARE_ID   0x60046621
        #define DA662A_8_ID             0x6620
        #define DA662A_16_ID            0x6621
        #define DA662A_8_LX_AP_ID       0xE0006620
        #define DA662A_16_LX_AP_ID      0xE0006621
	#define PNAME			"DA-662A"
#else
#define PNAME	""
#endif

int	main(int argc, char *argv[])
{

	if ( argc==1 || (argc == 2 && (!strcmp(argv[1],"-a")))){
#if defined (ARCH_UC8400) || (defined ARCH_DCU8620)
		/* For reading the DCU Model */
		int	model_fd;
		char	DCUModelStr[40];
		char	DCUModel;

		read_procfs("/proc/bus/pci/devices", proc_buf);
		switch(get_swid()) {
		case DCU8620_ID:
			/* Read the DCU Model */
			model_fd=open("/proc/driver/dcu_model", O_RDONLY);
			read(model_fd, &DCUModel, sizeof(DCUModel));
			close(model_fd);
			if( DCUModel == 'f' )
				sprintf(DCUModelStr, "DCU-8620-T(%s)","NPLC");
			else if( DCUModel == 'e' )
				sprintf(DCUModelStr, "DCU-8620-T(%s)","BPL");
			else if( DCUModel == 'd' )
				sprintf(DCUModelStr, "DCU-8620-T(%s)","Protecsa");
			else
				sprintf(DCUModelStr, "DCU-8620-T");
			printf("%s version %s", DCUModelStr, FIRM_VER );
	    		break;
		case UC8416_ID:
			printf("%s version %s","UC-8416-LX", FIRM_VER);
			break;
		case UC8411_ID:
			printf("%s version %s","UC-8411-LX", FIRM_VER);
			break;
		case UC8418_ID:
			printf("%s version %s","UC-8418-LX", FIRM_VER);
			break;
		case UC8430_ID:
			printf("%s version %s","UC-8430-LX", FIRM_VER);
			break;
		case UC8481_ID:
		case UC8482_ID:
		case UC8483_ID:
			printf("%s version %s","UC-8481-LX", FIRM_VER);
			break;
		default:
			printf("%s version %s",PNAME, FIRM_VER);
		}
#elif (defined ARCH_DA662A)
		int	fd, len;
		char	buf[32];
		fd = open("/proc/driver/swid", O_RDONLY);
		if ( fd < 0 ) {
			printf("Open /proc/driver/swid fail !\n");
			return -1;
		}
		len = read(fd, buf, 32);
		buf[8] = 0;
		switch ( strtol(buf, NULL, 16) ) {
		case DA662A_8_SOFTWARE_ID :
			printf("%s-8-LX version %s", PNAME, FIRM_VER);
			break;
		case DA662A_16_SOFTWARE_ID :
			printf("%s-16-LX version %s", PNAME, FIRM_VER);
			break;
		default :
			printf("Can't identifile model name !\n");
		}
		close(fd);
#else
		printf("%s version %s",PNAME, FIRM_VER);
#endif
		if( argc == 2 ) 
			printf(" Build %s",DATE) ;
	} else
		printf("Usage: kversion\n  -a: show the build time");
	printf("\n") ;
	return 0;
}
