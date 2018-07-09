
#include	<stdio.h>
#include	<netdb.h>
#include	<unistd.h>
#include	<net/if.h>
#include	<linux/sockios.h>

#include	"ctypes.h"
#include	"error.h"
#include	"local.h"
#include	"mix.h"
#include	"mis.h"
#include	"asn.h"

#if defined(ARCH_IA241) || defined(ARCH_IA241_32128) || defined(ARCH_EM1280) ||  defined(ARCH_UC8400) || defined(ARCH_UC8406)
    #define IP_ADDR_NO	3
#elif defined ARCH_IA240 || defined(ARCH_W311) || defined(ARCH_W321) || defined(ARCH_W341) || defined(ARCH_UC_7112_LX_PLUS) || defined (ARCH_UC8000) || defined (ARCH_RNAS1200) 
     #define IP_ADDR_NO	2
#elif defined ARCH_DA662A
     #define IP_ADDR_NO	4
#elif defined ARCH_W315 || defined ARCH_W315_NJKL || defined ARCH_W325 || defined ARCH_W345  || defined(ARCH_W406) || defined(ARCH_SMG1100) || defined(ARCH_W416)
     #define IP_ADDR_NO	1
#elif defined(ARCH_IA261) || defined(ARCH_IA262) || defined(ARCH_IA260) || defined(ARCH_EM2260)|| defined(ARCH_IA261_256)
	#ifdef HOSON_ODM
		#define IP_ADDR_NO	1
	#else
		#define IP_ADDR_NO	2
	#endif	// HOSON_ODM
#endif


#define IP_ADDR_LEN	4
#define LNAME_PATTERN	0x81
#define LNAME_VALUE	0x80

union ipaddr_union {
	unsigned long	addrl;
	unsigned char	addrc[IP_ADDR_LEN];
};

struct ipaddr_struct {
	union ipaddr_union	ipaddr;
	unsigned char		hwaddr[6];
};
static struct ipaddr_struct ipinfo[IP_ADDR_NO];

#if defined(ARCH_IA241) || defined(ARCH_IA241_32128) || defined(ARCH_EM1280)
static char	*ifname[IP_ADDR_NO]={"eth0", "eth1", "eth2"};
#elif defined ARCH_IA240
static char	*ifname[IP_ADDR_NO]={"eth0", "eth1"};
#elif defined ARCH_UC8400 || defined(ARCH_UC8406) 
static char	*ifname[IP_ADDR_NO]={"eth0", "eth1", "eth2"};
#elif defined ARCH_DA662A 
static char	*ifname[IP_ADDR_NO]={"eth0", "eth1", "eth2", "eth3"};
#elif defined ARCH_UC8000
static char	*ifname[IP_ADDR_NO]={"eth0", "eth1"};
#elif defined ARCH_W341
static char	*ifname[IP_ADDR_NO]={"eth0", "eth1"};
#elif defined ARCH_W345
static char	*ifname[IP_ADDR_NO]={"eth0"};
#elif defined ARCH_UC_7112_LX_PLUS
static char	*ifname[IP_ADDR_NO]={"eth0", "eth1"};
#elif defined ARCH_W311
static char	*ifname[IP_ADDR_NO]={"eth0"};
#elif defined ARCH_W321
static char	*ifname[IP_ADDR_NO]={"eth0"};
#elif defined ARCH_W315
static char	*ifname[IP_ADDR_NO]={"eth0"};
#elif defined ARCH_W315_NJKL
static char	*ifname[IP_ADDR_NO]={"eth0"};
#elif defined ARCH_W325
static char	*ifname[IP_ADDR_NO]={"eth0"};
#elif defined ARCH_RNAS1200
static char	*ifname[IP_ADDR_NO]={"eth0", "eth1"};
#elif defined(ARCH_W406) || defined(ARCH_SMG1100) || defined(ARCH_W416)
static char	*ifname[IP_ADDR_NO]={"eth0"};
#elif defined(ARCH_IA261) || defined(ARCH_IA262) || defined(ARCH_IA260) || defined(ARCH_EM2260)|| defined(ARCH_IA261_256)
#ifdef HOSON_ODM
static char	*ifname[IP_ADDR_NO]={"eth0"};
#else
static char	*ifname[IP_ADDR_NO]={"eth0", "eth1"};
#endif	// HOSON_ODM
#endif
static void get_ip_addr_info(void)
{
	int		fd, i;
	struct ifreq	ifr;

	memset(&ipinfo, 0, sizeof(ipinfo));
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if ( fd < 0 )
		return;
	for ( i=0; i<IP_ADDR_NO; i++ ) {
		strcpy(ifr.ifr_name, ifname[i]);
		ifr.ifr_addr.sa_family = AF_INET;
		if ( ioctl(fd, SIOCGIFADDR, &ifr) >= 0 )
			memcpy(ipinfo[i].ipaddr.addrc, &ifr.ifr_addr.sa_data[2], IP_ADDR_LEN);

		strcpy(ifr.ifr_name, ifname[i]);
		if ( ioctl(fd, SIOCGIFHWADDR, &ifr) >= 0 )
			memcpy(ipinfo[i].hwaddr, ifr.ifr_hwaddr.sa_data, 6);
	}
	close(fd);
}

#define AT_INDEX	1
#define AT_PHYS_ADDR	2
#define AT_ADDR		3
#define AT_MAX_ITEM	3
static	AsnIdType	atRetrieve (int ipno, CIntfType item)
{
	AsnIdType		asnresult=0;

	switch ( item ) {
	case AT_INDEX :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, ipno);
		break;
	case AT_PHYS_ADDR :
		asnresult = asnOctetString(asnClassUniversal, (AsnTagType)4, ipinfo[ipno-1].hwaddr, 6);
		break;
	case AT_ADDR :
		asnresult = asnOctetString (asnClassApplication, (AsnTagType)0, ipinfo[ipno-1].ipaddr.addrc, 4);
		break;
	default :
		break;
	}
	return asnresult;
}

static int setipaddrname(union ipaddr_union *ipaddr, MixNamePtrType name, MixLengthPtrType namelenp)
{
	int	len, i, ret;

	len = *namelenp;
	for ( i=0, ret=0; i<IP_ADDR_LEN; i++ ) {
		if ( ipaddr->addrc[i] >= LNAME_VALUE ) {
			*name++ = LNAME_PATTERN;
			*name++ = ipaddr->addrc[i] - LNAME_VALUE;
			len++;
			ret += 2;
		} else {
			*name++ = ipaddr->addrc[i];
			ret++;
		}
	}
	*namelenp = len;
	return ret;
}

static void getipaddr(union ipaddr_union *ipaddr, MixNamePtrType name)
{
	int	i;

	for ( i=0; i<IP_ADDR_LEN; i++, name++ ) {
		if ( *name == LNAME_PATTERN ) {
			ipaddr->addrc[i] = LNAME_VALUE;
			name++;
			ipaddr->addrc[i] += *name;
		} else {
			ipaddr->addrc[i] = *name;
		}
	}
}

static	AsnIdType	atNext (MixCookieType cookie, MixNamePtrType name, MixLengthPtrType namelenp)
{
	CIntsType	item;

	cookie = cookie;
	if ( *namelenp == 0 ) {	// 1.3.6.1.2.1.3.1.1
		get_ip_addr_info();
		*namelenp = 6;
		*name++ = 1;	// item
		*name++ = 1;	// index
		setipaddrname(&ipinfo[0].ipaddr, name, namelenp);
		return atRetrieve(1, 1);
	} else if ( *namelenp == 1 ) { // 1.3.6.1.2.1.3.1.1.x
		item = *name;
		if ( item < 1 || item > AT_MAX_ITEM )
			return 0;
		get_ip_addr_info();
		*namelenp = 6;
		name++;
		*name++ = 1;	// index
		setipaddrname(&ipinfo[0].ipaddr, name, namelenp);
		return atRetrieve(1, item);
	} else {	// 1.3.6.1.2.1.3.1.1.item.index.ip
		int			ipno;
		union ipaddr_union	ipaddr;

		item = *name;
		if ( item < 1 || item > AT_MAX_ITEM )
			return 0;
		getipaddr(&ipaddr, name+2);
		for ( ipno=0; ipno < IP_ADDR_NO; ipno++ ) {
			if ( ipaddr.addrl == ipinfo[ipno].ipaddr.addrl )
				break;
		}
		if ( ipno == IP_ADDR_NO )
			return 0;
		ipno++;
		if ( ipno >= IP_ADDR_NO ) {
			ipno = 1;
			item++;
		} else {
			ipno++;
		}
		*namelenp = 6;
		*name++ = item;
		*name++ = ipno;	// index
		setipaddrname(&ipinfo[ipno-1].ipaddr, name, namelenp);
		return atRetrieve(ipno, item);
	}
	return 0;
}

#if 0	// mask by Victor Yu. 07-30-2004
static	MixStatusType	atRelease (MixCookieType cookie)
{
	cookie = cookie;
	return (smpErrorGeneric);
}

static	MixStatusType	atCreate (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen, AsnIdType asn)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	asn = asn;
	return (smpErrorGeneric);
}

static	MixStatusType	atDestroy (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	return (smpErrorGeneric);
}
#endif

static	AsnIdType	atGet (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen)
{
	CIntfType		item;
	int			ipno;
	union ipaddr_union	ipaddr;

	cookie = cookie;
	if ( namelen < 6 )
		return 0;
	item = *name;
	if ( item < 1 || item > AT_MAX_ITEM )
		return 0;
	ipno = *(name+1);
	if ( ipno < 1 || ipno > IP_ADDR_NO )
		return 0;
	get_ip_addr_info();
	getipaddr(&ipaddr, name+2);
	for ( ipno=0; ipno < IP_ADDR_NO; ipno++ ) {
		if ( ipaddr.addrl == ipinfo[ipno].ipaddr.addrl )
			break;
	}
	if ( ipno == IP_ADDR_NO )
		return 0;
	ipno++;
	return atRetrieve(ipno, item);
}

#if 0	// mask by Victor Yu. 07-30-2004
static	MixStatusType	atSet (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen, AsnIdType asn)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	asn = asn;
	return (smpErrorReadOnly);
}
#endif

#if 0	// mask by Victor Yu. 07-30-2004
static	MixOpsType	atOps = {
	atRelease,
	atCreate,
	atDestroy,
	atNext,
	atGet,
	atSet
};
#else	// add by Victor Yu. 07-30-2004
static	MixOpsType	atOps = {
	GeneralRelease,
	GeneralCreate,
	GeneralDestroy,
	atNext,
	atGet,
	GeneralSet
};
#endif

CVoidType		atInit (void)
{
	(void) misExport ((MixNamePtrType) "\53\6\1\2\1\3\1\1",
		(MixLengthType)8, &atOps, (MixCookieType) 0);
}
