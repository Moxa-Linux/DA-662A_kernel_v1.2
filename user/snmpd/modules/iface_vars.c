
#include	<net/if.h>
#include	<stdio.h>
#include	<netdb.h>
#include	<unistd.h>
#include	<time.h>
#include	<linux/sockios.h>

#include	"ctypes.h"
#include	"error.h"
#include	"local.h"
#include	"iface_vars.h"
#include	"mix.h"
#include	"mis.h"
#include	"asn.h"

#if 1 /* Jared, 06-08-2016, for retrive the link status */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
#include	<linux/ethtool.h>
#include	<linux/sockios.h>
#endif

#if defined(ARCH_IA241) || defined(ARCH_IA241_32128) || defined(ARCH_EM1280)
	#define IFACE_ETH_NUMBER	3
#elif defined ARCH_IA240
	#define IFACE_ETH_NUMBER	2
#elif defined(ARCH_UC8400) || defined(ARCH_UC8406)
	#define IFACE_ETH_NUMBER	3
#elif defined ARCH_UC8000
	#define IFACE_ETH_NUMBER	2
#elif defined ARCH_DA662A
	#define IFACE_ETH_NUMBER	4
#elif defined ARCH_W341
	#define IFACE_ETH_NUMBER	2
#elif defined ARCH_W345
	#define IFACE_ETH_NUMBER	1
#elif defined ARCH_UC_7112_LX_PLUS
	#define IFACE_ETH_NUMBER	2
#elif defined ARCH_W311
	#define IFACE_ETH_NUMBER	2
#elif defined ARCH_W321
	#define IFACE_ETH_NUMBER	2
#elif defined ARCH_W315
	#define IFACE_ETH_NUMBER	1
#elif defined ARCH_W315_NJKL
	#define IFACE_ETH_NUMBER	1
#elif defined ARCH_W325
	#define IFACE_ETH_NUMBER	1
#elif defined ARCH_RNAS1200
	#define IFACE_ETH_NUMBER	2
#elif  defined(ARCH_W406) || defined(ARCH_SMG1100) || defined(ARCH_W416)
	#define IFACE_ETH_NUMBER	1
#elif defined(ARCH_IA261) || defined(ARCH_IA262) || defined(ARCH_IA260) || defined(ARCH_EM2260) || defined(ARCH_IA261_256)
	#ifdef HOSON_ODM
		#define IFACE_ETH_NUMBER	1
	#else
		#define IFACE_ETH_NUMBER	2
	#endif
#endif
#define IFACE_MAXTYPE		22
#define PATH_PROC_NET_DEV	"/proc/net/dev"
#define PATH_PROC_MXSER		"/proc/driver/mxser"
#define IFACE_MAX_STR_LEN	32

static CIntsType	ifaceNumberInt;

struct iface_dev {
	u_long	rx_bytes;
	u_long	rx_packets;
	u_long	rx_errs;
	u_long	rx_drops;
	u_long	rx_fifo;
	u_long	rx_frames;
	u_long	rx_compressed;
	u_long	rx_multicast;

	u_long	tx_bytes;
	u_long	tx_packets;
	u_long	tx_errs;
	u_long 	tx_drops;
	u_long	tx_fifo;
	u_long	tx_colls;
	u_long	tx_carrier;
	u_long	tx_compressed;

	u_long	speed;
	int	mtu;
	int	qlen;
	char	hwaddr[6];
	short	flags;
};

#if defined(ARCH_IA241) || defined(ARCH_IA241_32128) || defined(ARCH_EM1280) || defined(ARCH_UC8400) || defined(ARCH_UC8406)
static CCharType	ifaceName[IFACE_ETH_NUMBER][IFACE_MAX_STR_LEN] = {"eth0", "eth1", "eth2"};
#elif defined ARCH_IA240
static CCharType	ifaceName[IFACE_ETH_NUMBER][IFACE_MAX_STR_LEN] = {"eth0", "eth1"};
#elif defined ARCH_DA662A
static CCharType	ifaceName[IFACE_ETH_NUMBER][IFACE_MAX_STR_LEN] = {"eth0", "eth1", "eth2", "eth3"};
#elif defined ARCH_UC8000
static CCharType	ifaceName[IFACE_ETH_NUMBER][IFACE_MAX_STR_LEN] = {"eth0", "eth1"};
#elif defined ARCH_W341
static CCharType	ifaceName[IFACE_ETH_NUMBER][IFACE_MAX_STR_LEN] = {"eth0", "eth1"};
#elif defined ARCH_W345
static CCharType	ifaceName[IFACE_ETH_NUMBER][IFACE_MAX_STR_LEN] = {"eth0"};
#elif defined ARCH_UC_7112_LX_PLUS
static CCharType	ifaceName[IFACE_ETH_NUMBER][IFACE_MAX_STR_LEN] = {"eth0", "eth1"};
#elif defined ARCH_W311
static CCharType	ifaceName[IFACE_ETH_NUMBER][IFACE_MAX_STR_LEN] = {"eth0"};
#elif defined ARCH_W321
static CCharType	ifaceName[IFACE_ETH_NUMBER][IFACE_MAX_STR_LEN] = {"eth0"};
#elif defined ARCH_W315
static CCharType	ifaceName[IFACE_ETH_NUMBER][IFACE_MAX_STR_LEN] = {"eth0"};
#elif defined ARCH_W315_NJKL
static CCharType	ifaceName[IFACE_ETH_NUMBER][IFACE_MAX_STR_LEN] = {"eth0"};
#elif defined ARCH_W325
static CCharType	ifaceName[IFACE_ETH_NUMBER][IFACE_MAX_STR_LEN] = {"eth0"};
#elif defined ARCH_RNAS1200
static CCharType	ifaceName[IFACE_ETH_NUMBER][IFACE_MAX_STR_LEN] = {"eth0", "eth1"};
#elif defined(ARCH_W406) || defined(ARCH_SMG1100) || defined(ARCH_W416)
static CCharType	ifaceName[IFACE_ETH_NUMBER][IFACE_MAX_STR_LEN] = {"eth0"};
#elif defined(ARCH_IA261) || defined(ARCH_IA262) || defined(ARCH_IA260) || defined(ARCH_EM2260)|| defined(ARCH_IA261_256)

#ifdef HOSON_ODM
static CCharType	ifaceName[IFACE_ETH_NUMBER][IFACE_MAX_STR_LEN] = {"eth0"};
#else
static CCharType	ifaceName[IFACE_ETH_NUMBER][IFACE_MAX_STR_LEN] = {"eth0", "eth1"};
#endif	// HOSON_ODM

#endif
static	AsnIdType	ifaceRetrieve (CIntfType interface, CIntfType item)
{
	AsnIdType		asnresult;
	struct iface_dev	ifdev;
	FILE			*in;
	char			line[1024], *ptr1, *ptr2;
	int			fd;
	struct ifreq		ifr;
#if 1 /* Jared, 06-08-2016, for retrive the link status */
	struct ethtool_value edata ;		/* Struct network tool,support only by
						 * exiting device driver */
#endif

	memset(&ifdev, 0, sizeof(ifdev));
	if ( interface > IFACE_ETH_NUMBER )
		goto get_serial_info;
	ifdev.speed = 100000000;
	in = fopen(PATH_PROC_NET_DEV, "r");
	if ( in ) {
		while ( line == fgets(line, 1024, in) ) {
			ptr1 = line;
			ptr2 = ifaceName[interface-1];
			while ( *ptr1 && *ptr2 ) {
				if ( *ptr1 == *ptr2 )
					ptr2++;
				else
					ptr2 = ifaceName[interface-1];
				ptr1++;
			}
			if ( !*ptr1 )
				continue;
			while ( *ptr1 && *ptr1 != ' ' ) ptr1++;
			if ( !*ptr1 )
				continue;
			while ( *ptr1 && *ptr1 == ' ' ) ptr1++;
			if ( !*ptr1 )
				continue;
			if ( 16 == sscanf(ptr1, "%Lu %Lu %lu %lu %lu %lu %lu %lu %Lu %Lu %lu %lu %lu %lu %lu %lu", &ifdev.rx_bytes, &ifdev.rx_packets, &ifdev.rx_errs, &ifdev.rx_drops, &ifdev.rx_fifo, &ifdev.rx_frames, &ifdev.rx_compressed, &ifdev.rx_multicast, &ifdev.tx_bytes, &ifdev.tx_packets, &ifdev.tx_errs, &ifdev.tx_drops, &ifdev.tx_fifo, &ifdev.tx_colls, &ifdev.tx_carrier, &ifdev.tx_compressed) ) {
					break;
			}
		}
		fclose(in);
	}

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if ( fd > 0 ) {
		strcpy(ifr.ifr_name, ifaceName[interface-1]);
		if ( ioctl(fd, SIOCGIFFLAGS, &ifr) >= 0 ) {
			ifdev.flags = ifr.ifr_flags;
		}

		strcpy(ifr.ifr_name, ifaceName[interface-1]);
		if ( ioctl(fd, SIOCGIFHWADDR, &ifr) >= 0 ) {
			memcpy(ifdev.hwaddr, ifr.ifr_hwaddr.sa_data, 6);
		}

		strcpy(ifr.ifr_name, ifaceName[interface-1]);
		if ( ioctl(fd, SIOCGIFTXQLEN, &ifr) >= 0 ) {
			ifdev.qlen = ifr.ifr_qlen;
		}

		strcpy(ifr.ifr_name, ifaceName[interface-1]);
		if ( ioctl(fd, SIOCGIFMTU, &ifr) >= 0 ) {
			ifdev.mtu = ifr.ifr_mtu;
		}
#if 1 /* Jared, 06-08-2016, get the current operational state of the interface */
		edata.cmd = ETHTOOL_GLINK ;
		ifr.ifr_data = (char *)&edata ;
		if (ioctl(fd, SIOCETHTOOL, &ifr) < 0)  {
		       	printf("ETHTOOL_GLINK failed\n");
		}

		ifdev.tx_carrier = edata.data;

		if ( ifdev.tx_carrier )
			ifdev.flags |= IFF_RUNNING | IFF_UP;
		else
			ifdev.flags &= ~(IFF_RUNNING | IFF_UP);
		
#endif
		close(fd);
	}
	goto output_to_client;

get_serial_info:
	in = fopen(PATH_PROC_MXSER, "r");
	if ( in ) {
		int	p;

		// skip to get total port numbers
		if ( line != fgets(line, 1024, in) ) {
			fclose(in);
			goto output_to_client;
		}
		
		// skip each port information until I want port number information
		p = interface - 1 - IFACE_ETH_NUMBER;
		while ( p-- ) fgets(line, 1024, in);

		// to get the object port information
		if ( line == fgets(line, 1024, in) ) {
#if 1 /* Jared, 06-08-2016, we should not overwrite the ifdev.flags */
			sscanf(line, "%lu %lu %d", &ifdev.rx_bytes, &ifdev.tx_bytes, &ifdev.speed);
#else
			if ( 4 == sscanf(line, "%lu %lu %d %d", &ifdev.rx_bytes, &ifdev.tx_bytes, &ifdev.speed, &ifdev.flags) ) {
				if ( ifdev.flags == 1 )
					ifdev.flags = IFF_RUNNING | IFF_UP;
				else
					ifdev.flags = 0;
			}
#endif
		}

		fclose(in);
	}

output_to_client:
	switch (item) {
	case IFINDEX: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, interface);
		break;
	case IFDESCR: 
		if ( interface <= IFACE_ETH_NUMBER ) 
			sprintf(line, "eth%d", interface-1);
		else
			sprintf(line, "Serial port %d", interface-1-IFACE_ETH_NUMBER);
		asnresult = asnOctetString(asnClassUniversal, (AsnTagType)4, line, strlen(line));
		break;
	case IFTYPE: 
		if ( interface <= IFACE_ETH_NUMBER )
			asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, 6);
		else
			asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, 1);
		break;
	case IFMTU: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, ifdev.mtu);
		break;
	case IFSPEED: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType)2, ifdev.speed);
		break;
	case IFPHYSADDRESS: 
		if ( interface <= IFACE_ETH_NUMBER )
			asnresult = asnOctetString(asnClassUniversal, (AsnTagType)4, ifdev.hwaddr, 6);
		else
			asnresult = asnUnsl(asnClassUniversal, (AsnTagType)4, 0);
		break;
	case IFADMINSTATUS: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, ifdev.flags & IFF_RUNNING ? 1 : 2);
		break;
	case IFOPERSTATUS: 
#if 1 /* Jared, 06-08-2016, return the link status of the available Ethernet interfaces. */
		asnresult = 0;
		if ( interface <= IFACE_ETH_NUMBER )
			asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, ifdev.flags & IFF_UP ? 1 : 2);
#endif
		break;
	case IFLASTCHANGE: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType)3, 0);
		break;
    	case IFINOCTETS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType)1, ifdev.rx_bytes);
		break;
	case IFINUCASTPKTS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType)1, ifdev.rx_packets);
		break;
	case IFINNUCASTPKTS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType)1, 0);
		break;
	case IFINDISCARDS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType)1, ifdev.rx_drops);
		break;
	case IFINERRORS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType)1, ifdev.rx_errs);
		break;
	case IFINUNKNOWNPROTOS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType)1, 0);
		break;
	case IFOUTOCTETS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType)1, ifdev.tx_bytes);
		break;
	case IFOUTUCASTPKTS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType)1, ifdev.tx_packets);
		break;
	case IFOUTNUCASTPKTS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType)1, 0);
		break;
	case IFOUTDISCARDS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType)1, ifdev.tx_drops);
		break;
	case IFOUTERRORS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType)1, ifdev.tx_errs);
		break;
	case IFOUTQLEN: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType)2, ifdev.qlen);
		break;
	case IFSPECIFIC: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)6, 0);
		break;
	default:
		break;
	}
	return (asnresult);
}

#if 0	// mask by Victor Yu. 07-30-2004
static	MixStatusType	ifaceRelease (MixCookieType cookie)
{
	cookie = cookie;
	return (smpErrorGeneric);
}

static	MixStatusType	ifaceCreate (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen, AsnIdType asn)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	asn = asn;
	return (smpErrorGeneric);
}

static	MixStatusType	ifaceDestroy (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	return (smpErrorGeneric);
}
#endif

static	AsnIdType	ifaceGet (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen)
{
	CIntfType		interface, item;

	cookie = cookie;
	if ((namelen != (MixLengthType) 2) ||
		((interface=(CIntfType)*(name+1)) < (CIntfType) 1) ||
		(interface > (CIntfType)(ifaceNumberInt)) ||
		((item = (CIntfType) *name) < (CIntfType) 1) ||
		(item > (CIntfType) (IFACE_MAXTYPE)) ) {
		return ((AsnIdType) 0);
	}
	else {
		return (ifaceRetrieve (interface, item));
	}
}

#if 0	// mask by Victor Yu. 07-30-2004
static	MixStatusType	ifaceSet (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen, AsnIdType asn)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	asn = asn;
	return (smpErrorReadOnly);
}
#endif

static	AsnIdType	ifaceNext (MixCookieType cookie, MixNamePtrType name, MixLengthPtrType namelenp)
{
	CIntfType		interface, item;

	cookie = cookie;
	if (*namelenp == (MixLengthType) 0) {
		*namelenp = (MixLengthType) 2;
		*name++ = (MixNameType) 1;
		*name = 1;
		return (ifaceRetrieve (1, 1));
	}
	else if (*namelenp == (MixLengthType) 1) {
		item = (CIntfType)*name;
		if ( item > IFACE_MAXTYPE )
			return ((AsnIdType) 0);
		*namelenp = (MixLengthType) 2;
		*(++name) = (MixNameType)1;
		return (ifaceRetrieve (1, item));
	}
	else {
		item = (CIntfType)*name;
		interface = (CIntfType)*(name + 1);
		if ( interface >= ifaceNumberInt && item >= IFACE_MAXTYPE )
			return ((AsnIdType) 0);
		*namelenp = (MixLengthType)2;
		if ( interface >= ifaceNumberInt ) {
			interface = 1;
			item++;
		} else {
			interface++;
		}
		*name++ = item;
		*name = interface;
		return (ifaceRetrieve (interface, item));
	}
}

#if 0	// mask by Victor Yu. 07-30-2004
static	MixOpsType	ifaceOps = {
		ifaceRelease,
		ifaceCreate,
		ifaceDestroy,
		ifaceNext,
		ifaceGet,
		ifaceSet
};
#else	// add by Victor Yu. 07-30-2004
static	MixOpsType	ifaceOps = {
	GeneralRelease,
	GeneralCreate,
	GeneralDestroy,
	ifaceNext,
	ifaceGet,
	GeneralSet
};
#endif

CVoidType		ifaceInit (void)
{
	FILE	*in;
	char	line[256];

	ifaceNumberInt = (CIntsType)IFACE_ETH_NUMBER;
	in = fopen(PATH_PROC_MXSER, "r");
	if ( in ) {
		int	ports;

		if ( line == fgets(line, 256, in) ) {
			if ( 1 == sscanf(line, "%d", &ports) ) {
				ifaceNumberInt += ports;
			}
		}
		fclose(in);
	}
	(void)mivIntlRO ((MixNamePtrType) "\53\6\1\2\1\2\1",
		(MixLengthType)7, (CIntlPtrType)&ifaceNumberInt);
	(void) misExport ((MixNamePtrType) "\53\6\1\2\1\2\2\1",
		(MixLengthType)8, &ifaceOps, (MixCookieType) 0);
}
