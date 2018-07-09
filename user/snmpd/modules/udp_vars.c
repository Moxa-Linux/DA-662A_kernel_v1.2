

//#include	<sys/types.h>
#include	<stdio.h>
#include	<netdb.h>
#include	<unistd.h>
#include	<net/if.h>
#include	<linux/sockios.h>

#include	"ctypes.h"
#include	"error.h"
#include	"local.h"
#include	"udp_vars.h"
#include	"mix.h"
#include	"mis.h"
#include	"asn.h"

#define UDP_MAXTYPE 	4
#define IP_ADDR_LEN	4
#define PROC_UDP_NODE	"/proc/net/udp"
#define BUF_LEN		1024

#if defined(ARCH_IA241) || defined(ARCH_IA241_32128) || defined(ARCH_EM1280)
    #define IP_ADDR_NO	3
#elif defined  ARCH_IA240
    #define IP_ADDR_NO	2
#elif defined(ARCH_UC8400) || defined(ARCH_UC8406)
    #define IP_ADDR_NO	3
#elif defined  ARCH_UC8000
    #define IP_ADDR_NO	2
#elif defined  ARCH_W341
    #define IP_ADDR_NO	2
#elif defined  ARCH_DA662A
    #define IP_ADDR_NO	4
#elif defined  ARCH_W345
    #define IP_ADDR_NO	1
#elif defined  ARCH_UC_7112_LX_PLUS
    #define IP_ADDR_NO	2
#elif defined  ARCH_W311
    #define IP_ADDR_NO	1
#elif defined  ARCH_W321
    #define IP_ADDR_NO	1
#elif ARCH_W315
    #define IP_ADDR_NO	1
#elif ARCH_W315_NJKL
    #define IP_ADDR_NO	1
#elif defined  ARCH_W325
    #define IP_ADDR_NO	1
#elif defined(ARCH_W406) || defined(ARCH_SMG1100) || defined(ARCH_W416)
	#define IP_ADDR_NO	1
#elif defined(ARCH_IA261) || defined(ARCH_IA262) || defined(ARCH_IA260) || defined(ARCH_EM2260)|| defined(ARCH_IA261_256)
	#ifdef HOSON_ODM
		#define IP_ADDR_NO	1
	#else
		#define IP_ADDR_NO	2
	#endif	// HOSON_ODM
#endif


#define LNAME_PATTERN   0x81
#define LNAME_VALUE     0x80
#define BYTE_HIGH_BIT	0x80
#define BYTE_LOW_VALUE	0x7F
#define BYTE_LOW_BIT_NO	7

struct udp_mib
{
 	unsigned long	UdpInDatagrams;
 	unsigned long	UdpNoPorts;
 	unsigned long	UdpInErrors;
 	unsigned long	UdpOutDatagrams;
};

union ipaddr_union {
	unsigned long	addrl;
	unsigned char	addrc[IP_ADDR_LEN];
};

struct udpaddr_struct {
	union ipaddr_union	ipaddr;
	int			port;
	struct udpaddr_struct	*next;
};
static struct udpaddr_struct	*udpaddrhead=NULL;
#if defined(ARCH_IA241) || defined(ARCH_IA241_32128) || defined(ARCH_EM1280)
static char     		*ifname[IP_ADDR_NO]={"eth0", "eth1", "eth2"};
#elif defined ARCH_IA240 
static char     		*ifname[IP_ADDR_NO]={"eth0", "eth1"};
#elif defined(ARCH_UC8400) || defined(ARCH_UC8406)
static char     		*ifname[IP_ADDR_NO]={"eth0", "eth1", "eth2"};
#elif defined(ARCH_DA662A)
static char     		*ifname[IP_ADDR_NO]={"eth0", "eth1", "eth2", "eth3"};
#elif defined ARCH_UC8000
static char     		*ifname[IP_ADDR_NO]={"eth0", "eth1"};
#elif defined ARCH_W341
static char     		*ifname[IP_ADDR_NO]={"eth0", "eth1"};
#elif defined ARCH_W345
static char     		*ifname[IP_ADDR_NO]={"eth0"};
#elif defined ARCH_UC_7112_LX_PLUS 
static char     		*ifname[IP_ADDR_NO]={"eth0", "eth1"};
#elif defined ARCH_W311
static char     		*ifname[IP_ADDR_NO]={"eth0"};
#elif defined ARCH_W321
static char     		*ifname[IP_ADDR_NO]={"eth0"};
#elif defined ARCH_W315
static char     		*ifname[IP_ADDR_NO]={"eth0"};
#elif defined ARCH_W315_NJKL
static char     		*ifname[IP_ADDR_NO]={"eth0"};
#elif defined ARCH_W325
static char     		*ifname[IP_ADDR_NO]={"eth0"};
#elif defined(ARCH_W406) || defined(ARCH_SMG1100) || defined(ARCH_W416)
static char     		*ifname[IP_ADDR_NO]={"eth0"};
#elif defined(ARCH_IA261) || defined(ARCH_IA262) || defined(ARCH_IA260) || defined(ARCH_EM2260)|| defined(ARCH_IA261_256)
#ifdef HOSON_ODM
static char                     *ifname[IP_ADDR_NO]={"eth0"};
#else
static char                     *ifname[IP_ADDR_NO]={"eth0", "eth1"};
#endif	// HOSON_ODM
#endif
static union ipaddr_union	ifipaddr[IP_ADDR_NO];

static int setipaddrname(union ipaddr_union *ipaddr, MixNamePtrType name, MixLengthPtrType namelenp)
{
        int     i, ret;

	for ( i=0, ret=0; i<IP_ADDR_LEN; i++ ) {
		if ( ipaddr->addrc[i] >= LNAME_VALUE ) {
			*name++ = LNAME_PATTERN;
			*name++ = ipaddr->addrc[i] - LNAME_VALUE;
			ret += 2;
		} else {
			*name++ = ipaddr->addrc[i];
			ret++;
		}
	}
	*namelenp = *namelenp + ret;
	return ret;
}

static int setportname(int port, MixNamePtrType name, MixLengthPtrType namelenp)
{
	int		i, ret=0;
	MixNameType	buf[8];

	do {
		buf[ret++] = port & BYTE_LOW_VALUE;
		port >>= BYTE_LOW_BIT_NO;
	} while ( port );
	i = ret - 1;
	while ( i ) {
		*name++ = buf[i--] | BYTE_HIGH_BIT;
	}
	*name++ = buf[i];

	*namelenp = *namelenp + ret;
	return ret;
}

static int getipaddr(union ipaddr_union *ipaddr, MixNamePtrType name)
{
	int     i, ret;

	for ( i=0, ret=0; i<IP_ADDR_LEN; i++, name++ ) {
		if ( *name == LNAME_PATTERN ) {
			ipaddr->addrc[i] = LNAME_VALUE;
			name++;
			ipaddr->addrc[i] += *name;
			ret += 2;
		} else {
			ipaddr->addrc[i] = *name;
			ret++;
		}
	}
	return ret;
}

static int getport(int *port, MixNamePtrType name)
{
	int	ret=0, p=0;

	while ( *name & BYTE_HIGH_BIT ) {
		p |= (*name & BYTE_LOW_VALUE);
		p <<= BYTE_LOW_BIT_NO;
		name++;
		ret++;
	}
	p |= *name;
	ret++;
	*port = p;
	return ret;
}

static struct udpaddr_struct *find_match(struct udpaddr_struct *udpaddr)
{
	struct udpaddr_struct	*head;

	head = udpaddrhead;
	while ( head ) {
		if ( head->ipaddr.addrl == udpaddr->ipaddr.addrl &&
		     head->port == udpaddr->port )
			return head;
		head = head->next;
	}
	return head;
}

static void get_udpaddr_info(void)
{
	struct udpaddr_struct	*udpaddrptr, *tail;
	int			fd, i, ipaddrno;
	struct ifreq		ifr;
	FILE			*in;
	char			line[BUF_LEN];

	// first free has got udp address information
	while ( udpaddrhead ) {
		udpaddrptr = udpaddrhead;
		udpaddrhead = udpaddrhead->next;
		free(udpaddrptr);
	}

	// now to get my IP address for each interface
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if ( fd < 0 )
		return;
	for ( i=0, ipaddrno=0; i<IP_ADDR_NO; i++ ) {
		strcpy(ifr.ifr_name, ifname[i]);
		ifr.ifr_addr.sa_family = AF_INET;
		if ( ioctl(fd, SIOCGIFADDR, &ifr) >= 0 ) {
			memcpy(ifipaddr[ipaddrno++].addrc, &ifr.ifr_addr.sa_data[2], IP_ADDR_LEN);
		}
	}
	close(fd);
	if ( ipaddrno == 0 )
		return;

	// now to get the UDP address with port information
	in = fopen(PROC_UDP_NODE, "r");
	if ( !in )
		return;
	tail = udpaddrhead;
	while ( line == fgets(line, BUF_LEN, in) ) {
		struct udpaddr_struct	udpaddr;
		udpaddr.next = NULL;
		if ( 3 == sscanf(line, "%4d: %08X:%04X", &i, &udpaddr.ipaddr.addrl, &udpaddr.port) ) {
			if ( udpaddr.ipaddr.addrl == 0 ) {
				for ( i=0; i<ipaddrno; i++ ) {
					udpaddr.ipaddr = ifipaddr[i];
					if ( find_match(&udpaddr) )
						continue;
					udpaddrptr = malloc(sizeof(struct udpaddr_struct));
					if ( udpaddrptr == NULL )
						break;
					*udpaddrptr = udpaddr;
					if ( udpaddrhead == NULL )
						udpaddrhead = udpaddrptr;
					else
						tail->next = udpaddrptr;
					tail = udpaddrptr;
				}
				if ( i != ipaddrno )
					break;
			} else {
				udpaddrptr = malloc(sizeof(struct udpaddr_struct));
				if ( udpaddrptr == NULL )
					break;
				*udpaddrptr = udpaddr;
				if ( udpaddrhead == NULL ) 
					udpaddrhead = udpaddrptr;
				else
					tail->next = udpaddrptr;
				tail = udpaddrptr;
			}
		}
	}
	fclose(in);
}

static	AsnIdType	udpRetrieve (CIntfType item)
{
	struct udp_mib	udpstat;
	AsnIdType	asnresult;
   	unsigned long	result;
        FILE 		*in;
        char 		line[1024];

	in = fopen ("/proc/net/snmp", "r");
	if (! in) {
    		printf("udpRetrieve() Error opening /proc/net/snmp\n");	
		return 0;
	}

	while (line == fgets (line, 1024, in)) {
		if (4 == sscanf (line, "Udp: %lu %lu %lu %lu\n",
			&udpstat.UdpInDatagrams, &udpstat.UdpNoPorts,
			&udpstat.UdpInErrors, &udpstat.UdpOutDatagrams))
			break;
	}
	fclose (in);
	switch ( item ){
	case UDPINDATAGRAMS: 
		result=udpstat.UdpInDatagrams;
		break;
	case UDPNOPORTS: 
		result=udpstat.UdpNoPorts;
		break;
	case UDPINERRORS: 
		result=udpstat.UdpInErrors;
		break;
	case UDPOUTDATAGRAMS: 
		result=udpstat.UdpOutDatagrams;
		break;
	default:
		break;
	}		
	asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,
			result);
	return (asnresult);
}

#define UDP_ADDR_IP		1
#define UDP_ADDR_PORT		2
#define UDP_ADDR_MAX_ITEM	2
static	AsnIdType	udpaddrRetrieve (struct udpaddr_struct *udpaddr, CIntfType item)
{
	AsnIdType       asnresult=0;

	switch ( item ) {
	case UDP_ADDR_IP :
		asnresult = asnOctetString (asnClassApplication, (AsnTagType)0, udpaddr->ipaddr.addrc, 4);
		break;
	case UDP_ADDR_PORT :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, udpaddr->port);
		break;
	default :
		break;
	}
	return asnresult;
}

#if 0	// mask by Victor Yu. 07-30-2004
static	MixStatusType	udpRelease (MixCookieType cookie)
{
	cookie = cookie;
	return (smpErrorGeneric);
}

static	MixStatusType	udpCreate (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen, AsnIdType asn)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	asn = asn;
	return (smpErrorGeneric);
}

static	MixStatusType	udpDestroy (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	return (smpErrorGeneric);
}
#endif

static	AsnIdType	udpGet (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen)
{
	CIntfType		item;

	cookie = cookie;
	if ( namelen > 2 && *name == UDPADDRPORT && (*(name+1) == 1) && ((item=*(name+2)) > 1) && item <= UDP_ADDR_MAX_ITEM ) {
		struct udpaddr_struct	udpaddr;
		int			len;

		get_udpaddr_info();
		if ( udpaddrhead == NULL )
			return 0;
		name += 3;
		len = getipaddr(&udpaddr.ipaddr, name);
		getport(&udpaddr.port, name+len);
		if ( find_match(&udpaddr) == NULL )
			return 0;
		return udpaddrRetrieve(&udpaddr, item);
	}

	if ((namelen != (MixLengthType) 2) ||
		((item = (CIntfType) *name) < (CIntfType) 1) ||
		(item > (CIntfType) (UDP_MAXTYPE)) || (*(name + 1) != (MixNameType) 0)) {
		return ((AsnIdType) 0);
	}
	else {
		return (udpRetrieve (item));
	}
}

#if 0	// mask by Victor Yu. 07-30-2004
static	MixStatusType	udpSet (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen, AsnIdType asn)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	asn = asn;
	return (smpErrorReadOnly);
}
#endif

static	AsnIdType	udpaddrNext (MixCookieType cookie, MixNamePtrType name, MixLengthPtrType namelenp)
{
	CIntsType	item;
	static int	index;

	cookie = cookie;
#if 0
	if ( *namelenp == 0 ) {	// 1.3.6.1.2.1.7
		get_udpaddr_info();
		if ( udpaddrhead == NULL )
			return 0;
		*namelenp = 3;
		*name++ = UDPADDRPORT;
		*name++ = 1;
		*name++ = 1;	// first item;
		item = 1;
		goto udpaddrNextl1;
	} else if ( *namelenp == 1 ) { // 1.3.6.1.2.1.7.5
#else
	if ( *namelenp == 1 ) { // 1.3.6.1.2.1.7.5
#endif
printf("%s,%s,%d\n", __FILE__, __FUNCTION__, __LINE__);
		item = *name;
		if ( item != UDPADDRPORT )
			return 0;
		get_udpaddr_info();
		if ( udpaddrhead == NULL )
			return 0;
		//*namelenp = 3;
		name++;
		*name++ = 1;
		*name++ = 1;	// first item;
		*name++ = 1;	// index
		index = 1;
		item = 1;
udpaddrNextl1:
		*namelenp = 4;	// add a index
		name += setipaddrname(&udpaddrhead->ipaddr, name, namelenp);
		name += setportname(udpaddrhead->port, name, namelenp);
		return udpaddrRetrieve(udpaddrhead, item);
	} else if ( *namelenp == 2 ) { // 1.3.6.1.2.1.7.5.1
		item = *name;
		if ( item != UDPADDRPORT )
			return 0;
		item = *(name + 1);
		if ( item != 1 )
			return 0;
		get_udpaddr_info();
		if ( udpaddrhead == NULL )
			return 0;
		name += 2;
		*name++ = 1;	// first item;
		*name++ = 1;	// index
		index = 1;
		//*namelenp = 3;
		item = 1;
		goto udpaddrNextl1;
	} else if ( *namelenp == 3 ) { // 1.3.6.1.2.1.7.5.1.item
		item = *name;
		if ( item != UDPADDRPORT )
			return 0;
		item = *(name + 1);
		if ( item != 1 )
			return 0;
		item = *(name + 2);
		if ( item < 1 || item > UDP_ADDR_MAX_ITEM )
			return 0;
		get_udpaddr_info();
		if ( udpaddrhead == NULL )
			return 0;
		name += 3;
		*name++ = 1;	// index
		index = 1;
		goto udpaddrNextl1;
	//} else {	// 1.3.6.1.2.1.7.5.1.item.ip.port
	} else {	// 1.3.6.1.2.1.7.5.1.item.index.ip.port
		struct udpaddr_struct	udpaddr, *udpaddrptr;
		int			len;

		if ( udpaddrhead == NULL )
			return 0;
		item = *name;
		if ( item != UDPADDRPORT )
			return 0;
		item = *(name + 1);
		if ( item != 1 )
			return 0;
		item = *(name + 2);
		if ( item < 1 || item > UDP_ADDR_MAX_ITEM )
			return 0;
		name += 2;
		/*
		len = getipaddr(&udpaddr.ipaddr, name+1);
		getport(&udpaddr.port, name+len+1);
		*/
		len = getipaddr(&udpaddr.ipaddr, name+2);
		getport(&udpaddr.port, name+len+2);
		udpaddrptr = find_match(&udpaddr);
		if ( udpaddrptr == NULL )
			return 0;
		udpaddrptr = udpaddrptr->next;
		if ( udpaddrptr == NULL ) {
			item++;
			index = 1;
			udpaddrptr = udpaddrhead;
		} else
			index++;
		*name++ = item;
		*name++ = index;
		//*namelenp = 3;
		*namelenp = 4;
		name += setipaddrname(&udpaddrptr->ipaddr, name, namelenp);
		name += setportname(udpaddrptr->port, name, namelenp);
		return udpaddrRetrieve(udpaddrptr, item);
	}
	return 0;
}

static	AsnIdType	udpNext (MixCookieType cookie, MixNamePtrType name, MixLengthPtrType namelenp)
{
	CIntfType	item;

	cookie = cookie;
	if (*namelenp == (MixLengthType) 0) {
		*namelenp = (MixLengthType) 2;
		*name++ = (MixNameType) 1;
		*name = (MixNameType) 0;
		return (udpRetrieve ((CIntfType) 1));
	}
	else if (*namelenp == (MixLengthType) 1) {
		item = *name;
		if ( item == UDPADDRPORT )
			return udpaddrNext(cookie, name, namelenp);
		if ( item <= UDP_MAXTYPE ) {
			*namelenp = (MixLengthType) 2;
			*(++name) = (MixNameType) 0;
			return (udpRetrieve (item));
		}
		else {
			return ((AsnIdType) 0);
		}
	}
	else {
		item = *name;
		if ( item < UDP_MAXTYPE ) {
			*namelenp = (MixLengthType) 2;
			*name++ = (MixNameType) (++item);
			*name = (MixNameType) 0;
			return (udpRetrieve (item));
		} else if ( item == UDP_MAXTYPE ) {
			*name = UDPADDRPORT;
			*(name+1) = 1;
			*namelenp = 2;
			return udpaddrNext(cookie, name, namelenp);
		} else if ( item == UDPADDRPORT ) {
			return udpaddrNext(cookie, name, namelenp);
		} else {
			return 0;
		}
	}
	return 0;
}

#if 0	// mask by Victor Yu. 07-30-2004
static	MixOpsType	udpOps = {
	udpRelease,
	udpCreate,
	udpDestroy,
	udpNext,
	udpGet,
	udpSet
};
#else	// add by Victor Yu. 07-30-2004
static	MixOpsType	udpOps = {
	GeneralRelease,
	GeneralCreate,
	GeneralDestroy,
	udpNext,
	udpGet,
	GeneralSet
};
#endif

CVoidType		udpInit (void)
{
	(void) misExport ((MixNamePtrType) "\53\6\1\2\1\7",
		(MixLengthType) 6, & udpOps, (MixCookieType) 0);
}

