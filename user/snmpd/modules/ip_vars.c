
#include	<stdio.h>
#include	<netdb.h>
#include	<unistd.h>
#include	<net/if.h>
#include	<linux/sockios.h>

#include	"ctypes.h"
#include	"error.h"
#include	"local.h"
#include	"ip_vars.h"
#include	"mix.h"
#include	"mis.h"
#include	"asn.h"

#define IP_MAXTYPE 	19
#if defined(ARCH_IA241) || defined(ARCH_IA241_32128) || defined(ARCH_EM1280)
   #define IP_ADDR_NO	3
#elif defined ( ARCH_IA240 ) 
   #define IP_ADDR_NO	2
#elif defined(ARCH_UC8400) || defined(ARCH_UC8406)
   #define IP_ADDR_NO	3
#elif defined ( ARCH_UC8000 )
   #define IP_ADDR_NO	2
#elif  defined ( ARCH_W341 )
   #define IP_ADDR_NO	2
#elif  defined ( ARCH_W345 )
   #define IP_ADDR_NO	1
#elif  defined ARCH_UC_7112_LX_PLUS
   #define IP_ADDR_NO	2
#elif  defined ARCH_W311
   #define IP_ADDR_NO	2
#elif  defined ARCH_W321
   #define IP_ADDR_NO	2
#elif  defined ARCH_W315
   #define IP_ADDR_NO	1
#elif  defined ARCH_W315_NJKL
   #define IP_ADDR_NO	1
#elif  defined ARCH_W325
   #define IP_ADDR_NO	1
#elif  defined ARCH_RNAS1200
   #define IP_ADDR_NO	2
#elif  defined ARCH_DA662A
   #define IP_ADDR_NO	4
#elif defined(ARCH_W406) || defined(ARCH_SMG1100) || defined(ARCH_W416)
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
#define IP_ROUTE_PROC_NODE	"/proc/net/route"
#define BUF_LEN		1024

struct ip_mib
{
 	unsigned long	IpForwarding;
 	unsigned long	IpDefaultTTL;
 	unsigned long	IpInReceives;
 	unsigned long	IpInHdrErrors;
 	unsigned long	IpInAddrErrors;
 	unsigned long	IpForwDatagrams;
 	unsigned long	IpInUnknownProtos;
 	unsigned long	IpInDiscards;
 	unsigned long	IpInDelivers;
 	unsigned long	IpOutRequests;
 	unsigned long	IpOutDiscards;
 	unsigned long	IpOutNoRoutes;
 	unsigned long	IpReasmTimeout;
 	unsigned long	IpReasmReqds;
 	unsigned long	IpReasmOKs;
 	unsigned long	IpReasmFails;
 	unsigned long	IpFragOKs;
 	unsigned long	IpFragFails;
 	unsigned long	IpFragCreates;
};

union ipaddr_union {
	unsigned long	addrl;
	unsigned char	addrc[IP_ADDR_LEN];
};

struct ipaddr_struct {
	union ipaddr_union	ipaddr;
	union ipaddr_union	netmask;
	unsigned char		hwaddr[6];
};

static struct ipaddr_struct ipinfo[IP_ADDR_NO];

struct iproute_struct {
	union ipaddr_union	destaddr;
	union ipaddr_union	gw;
	int			flag;
	int			metric;
	union ipaddr_union	mask;
	int			mtu;
	int			rtt;
	int			index;
	struct iproute_struct	*next;
};
static struct iproute_struct	*iproutehead=NULL;

#if ( defined ARCH_IA241 ) || (defined ARCH_IA241_32128) || defined(ARCH_EM1280)
static char	*ifname[IP_ADDR_NO]={"eth0", "eth1", "eth2"};
#elif ( defined ARCH_IA240 )
static char	*ifname[IP_ADDR_NO]={"eth0", "eth1"};
#elif defined(ARCH_UC8400) || defined(ARCH_UC8406)
static char	*ifname[IP_ADDR_NO]={"eth0", "eth1", "eth2"};
#elif ( defined ARCH_UC8000 )
static char	*ifname[IP_ADDR_NO]={"eth0", "eth1"};
#elif ( defined ARCH_DA662A )
static char	*ifname[IP_ADDR_NO]={"eth0", "eth1", "eth2", "eth3"};
#elif  ( defined ARCH_W341)
static char	*ifname[IP_ADDR_NO]={"eth0", "eth1"};
#elif  ( defined ARCH_W345)
static char	*ifname[IP_ADDR_NO]={"eth0"};
#elif ( defined ARCH_UC_7112_LX_PLUS )
static char	*ifname[IP_ADDR_NO]={"eth0", "eth1"};
#elif  ( defined ARCH_W311)
static char	*ifname[IP_ADDR_NO]={"eth0"};
#elif  ( defined ARCH_W321)
static char	*ifname[IP_ADDR_NO]={"eth0"};
#elif  defined ARCH_W315
static char	*ifname[IP_ADDR_NO]={"eth0"};
#elif  defined ARCH_W315_NJKL
static char	*ifname[IP_ADDR_NO]={"eth0"};
#elif  ( defined ARCH_W325)
static char	*ifname[IP_ADDR_NO]={"eth0"};
#elif ( defined ARCH_RNAS1200 )
static char	*ifname[IP_ADDR_NO]={"eth0", "eth1"};
#elif defined(ARCH_W406) || defined(ARCH_SMG1100) || defined(ARCH_W416)//Added by Wade ,03-09-2009 
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
		if ( ioctl(fd, SIOCGIFNETMASK, &ifr) >= 0 )
			memcpy(ipinfo[i].netmask.addrc, &ifr.ifr_netmask.sa_data[2], IP_ADDR_LEN);

		strcpy(ifr.ifr_name, ifname[i]);
		if ( ioctl(fd, SIOCGIFHWADDR, &ifr) >= 0 )
			memcpy(ipinfo[i].hwaddr, ifr.ifr_hwaddr.sa_data, 6);
	}
	close(fd);
}

static struct iproute_struct *find_iproute_match(struct iproute_struct *iproute)
{
	struct iproute_struct	*head;

	head = iproutehead;
	while ( head ) {
		if ( head->destaddr.addrl == iproute->destaddr.addrl )
			break;
		head = head->next;
	}
	return head;
}

static void get_iproute_info(void)
{
	struct iproute_struct	*iprouteptr, *tail;
	FILE			*in;
	char			line[BUF_LEN];

	// first free all has got ip routing information
	while ( iproutehead ) {
		iprouteptr = iproutehead;
		iproutehead = iproutehead->next;
		free(iprouteptr);
	}

	// now to get new ip routing information
	get_ip_addr_info();
	in = fopen(IP_ROUTE_PROC_NODE, "r");
	if ( !in )
		return;
	tail = iproutehead;
	while ( line == fgets(line, BUF_LEN, in) ) {
		struct iproute_struct	iproute;
		char			node[32];
		int			i;
		iproute.next = NULL;
		if ( 11 == sscanf(line, "%s\t%08X\t%08X\t%04X\t%d\t%u\t%d\t%08X\t%d\t%u\t%u", node, &iproute.destaddr.addrl, &iproute.gw.addrl, &iproute.flag, &i, &i, &iproute.metric, &iproute.mask.addrl, &iproute.mtu, &i, &iproute.rtt) ) {
#if 0
			if ( iproute.destaddr.addrl == 0x7F000000 )
				continue;
#endif
			if ( iproute.gw.addrl == 0 ) {
				if ( strcmp(ifname[0], node) == 0 )
					iproute.gw = ipinfo[0].ipaddr;
				else if ( strcmp(ifname[1], node) == 0 )
					iproute.gw = ipinfo[1].ipaddr;
				else
					continue;
			}
			if ( strcmp(ifname[0], node) == 0 )
				iproute.index = 1;
			else if ( strcmp(ifname[1], node) == 0 )
				iproute.index = 2;
			else
				continue;
			iprouteptr = malloc(sizeof(struct iproute_struct));
			if ( iprouteptr == NULL )
				break;
			*iprouteptr = iproute;
			if ( iproutehead == NULL )
				iproutehead = iprouteptr;
			else
				tail->next = iprouteptr;
			tail = iprouteptr;
		}
	}
	fclose(in);
}

static	AsnIdType	ipRetrieve (CIntfType item)
{
	struct ip_mib	ipstat;
	AsnIdType	asnresult=0;
        FILE 		*in;
        char 		line [BUF_LEN];
	
	in = fopen ("/proc/net/snmp", "r");
	if (! in) {
    		printf("ipRetrieve() Error opening /proc/net/snmp\n");	
		return 0;
	}

	while (line == fgets (line, BUF_LEN, in)) {
		if ((IP_MAXTYPE) == sscanf (line, 
						"Ip: %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
			&ipstat.IpForwarding, &ipstat.IpDefaultTTL, &ipstat.IpInReceives, 
     			&ipstat.IpInHdrErrors, &ipstat.IpInAddrErrors, &ipstat.IpForwDatagrams, 
			&ipstat.IpInUnknownProtos, &ipstat.IpInDiscards, &ipstat.IpInDelivers, 
			&ipstat.IpOutRequests, &ipstat.IpOutDiscards, &ipstat.IpOutNoRoutes, 
			&ipstat.IpReasmTimeout, &ipstat.IpReasmReqds, &ipstat.IpReasmOKs, 
			&ipstat.IpReasmFails, &ipstat.IpFragOKs, &ipstat.IpFragFails, 
			&ipstat.IpFragCreates))
			break;
	}
	fclose (in);

	switch (item){
	case IPFORWARDING: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType) 2,ipstat.IpForwarding);
		break;
	case IPDEFAULTTTL: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType) 2,ipstat.IpDefaultTTL);
		break;
	case IPINRECEIVES: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,ipstat.IpInReceives);
		break;
	case IPINHDRERRORS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,ipstat.IpInHdrErrors);
		break;
	case IPINADDRERRORS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,ipstat.IpInAddrErrors);
		break;
	case IPFORWDATAGRAMS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,ipstat.IpForwDatagrams);
		break;
	case IPINUNKNOWNPROTOS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,ipstat.IpInUnknownProtos);
		break;
	case IPINDISCARDS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,ipstat.IpInDiscards);
		break;
	case IPINDELIVERS:
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,ipstat.IpInDelivers);
		break;
	case IPOUTREQUESTS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,ipstat.IpOutRequests);
		break;
	case IPOUTDISCARDS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,ipstat.IpOutDiscards);
		break;
	case IPOUTNOROUTES: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,ipstat.IpOutNoRoutes);
		break;
	case IPREASMTIMEOUT: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType) 2,ipstat.IpReasmTimeout);
		break;
	case IPREASMREQDS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,ipstat.IpReasmReqds);
		break;
	case IPREASMOKS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,ipstat.IpReasmOKs);
		break;
	case IPREASMFAILS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,ipstat.IpReasmFails);
		break;
	case IPFRAGOKS:
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,ipstat.IpFragOKs);
		break;
	case IPFRAGFAILS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,ipstat.IpFragFails);
		break;
	case IPFRAGCREATES: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,ipstat.IpFragCreates);
		break;
	case IPROUTEDISCARD:
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, 0);
		break;
	case IPADENTADDR :
	case IPROUTETABLE :
	case IPNETTOMEDIA :
	default:
		break;
	}
	
	return (asnresult);
}

#define IP_ADDR_ADDR		1
#define IP_ADDR_INDEX		2
#define IP_ADDR_NETMASK		3
#define IP_ADDR_BCAST		4
#define IP_ADDR_RESAM		5
#define IP_ADDR_MAX_ITEM	5
static	AsnIdType	ipaddrRetrieve (int ipno, CIntfType item)
{
	AsnIdType	asnresult=0;

	switch ( item ) {
	case IP_ADDR_ADDR :
		asnresult = asnOctetString (asnClassApplication, (AsnTagType)0, ipinfo[ipno-1].ipaddr.addrc, 4);
		break;
	case IP_ADDR_INDEX :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, ipno);
		break;
	case IP_ADDR_NETMASK :
		asnresult = asnOctetString (asnClassApplication, (AsnTagType)0, ipinfo[ipno-1].netmask.addrc, 4);
		break;
	case IP_ADDR_BCAST :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, 1);
		break;
	case IP_ADDR_RESAM :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, 65535);
		break;
	default :
		break;
	}
	return asnresult;
}

#define IP_NET_INDEX		1
#define IP_NET_PHYS_ADDR	2
#define IP_NET_ADDR		3
#define IP_NET_TYPE		4
#define IP_NET_MAX_ITEM		4
static	AsnIdType	ipnetRetrieve (int ipno, CIntfType item)
{
	AsnIdType		asnresult=0;

	switch ( item ) {
	case IP_NET_INDEX :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, ipno);
		break;
	case IP_NET_PHYS_ADDR :
		asnresult = asnOctetString(asnClassUniversal, (AsnTagType)4, ipinfo[ipno-1].hwaddr, 6);
		break;
	case IP_NET_ADDR :
		asnresult = asnOctetString (asnClassApplication, (AsnTagType)0, ipinfo[ipno-1].ipaddr.addrc, 4);
		break;
	case IP_NET_TYPE :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, 4);
		break;
	default :
		break;
	}
	return asnresult;
}

#define IP_ROUTE_DEST		1
#define IP_ROUTE_INDEX		2
#define IP_ROUTE_METRIC1	3
#define IP_ROUTE_METRIC2	4
#define IP_ROUTE_METRIC3	5
#define IP_ROUTE_METRIC4	6
#define IP_ROUTE_NEXT_HOP	7
#define IP_ROUTE_TYPE		8
#define IP_ROUTE_PROTO		9
#define IP_ROUTE_AGE		10
#define IP_ROUTE_MASK		11
#define IP_ROUTE_METRIC5	12
#define IP_ROUTE_INFO		13
#define IP_ROUTE_MAX_ITEM	13
static	AsnIdType	iprouteRetrieve (struct iproute_struct *iproute, CIntfType item)
{
	AsnIdType		asnresult=0;

	switch ( item ) {
	case IP_ROUTE_DEST :
		asnresult = asnOctetString (asnClassApplication, (AsnTagType)0, iproute->destaddr.addrc, 4);
		break;
	case IP_ROUTE_INDEX :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, iproute->index);
		break;
	case IP_ROUTE_METRIC1 :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, iproute->metric);
		break;
	case IP_ROUTE_METRIC2 :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, -1);
		break;
	case IP_ROUTE_METRIC3 :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, -1);
		break;
	case IP_ROUTE_METRIC4 :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, -1);
		break;
	case IP_ROUTE_NEXT_HOP :
		asnresult = asnOctetString (asnClassApplication, (AsnTagType)0, iproute->gw.addrc, 4);
		break;
	case IP_ROUTE_TYPE :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, 3);
		break;
	case IP_ROUTE_PROTO :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, 2);
		break;
	case IP_ROUTE_AGE :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, iproute->mtu);
		break;
	case IP_ROUTE_MASK :
		asnresult = asnOctetString (asnClassApplication, (AsnTagType)0, iproute->mask.addrc, 4);
		break;
	case IP_ROUTE_METRIC5 :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, -1);
		break;
	case IP_ROUTE_INFO :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)6, 0);
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

static	AsnIdType	ipaddrNext (MixCookieType cookie, MixNamePtrType name, MixLengthPtrType namelenp)
{
	CIntsType	item, ipno;

	cookie = cookie;
#if 0
	if ( *namelenp == 0 ) {	// 1.3.6.1.2.1.4
		get_ip_addr_info();
		*name++ = IPADENTADDR;
		*name++ = 1;
		*name++ = 1;
		ipno = item = 1;
		goto ipaddrNextl1;
	} else if ( *namelenp == 1 ) { // 1.3.6.1.2.1.4.20
#else
	if ( *namelenp == 1 ) { // 1.3.6.1.2.1.4.20
#endif
		item = *name;
		if ( item != IPADENTADDR )
			return 0;
		get_ip_addr_info();
		name++;
		*name++ = 1;
		*name++ = 1;
		*name++ = 1; // index
		ipno = item = 1;
ipaddrNextl1:
		//*namelenp = 7;
		*namelenp = 8;
		setipaddrname(&ipinfo[ipno-1].ipaddr, name, namelenp);
		return ipaddrRetrieve(ipno, item);
	} else if ( *namelenp == 2 ) {	// 1.3.6.1.2.1.4.20.1
		item = *name;
		if ( item != IPADENTADDR )
			return 0;
		item = *(name + 1);
		if ( item != 1 )
			return 0;
		get_ip_addr_info();
		name += 2;
		*name++ = 1;
		*name++ = 1;	// index
		ipno = item = 1;
		goto ipaddrNextl1;
	} else if ( *namelenp == 3 ) {	// 1.3.6.1.2.1.4.20.1.item
		item = *name;
		if ( item != IPADENTADDR )
			return 0;
		item = *(name + 1);
		if ( item != 1 )
			return 0;
		item = *(name + 2);
		if ( item < 1 || item > IP_ADDR_MAX_ITEM )
			return 0;
		get_ip_addr_info();
		name += 3;
		*name++ = 1;	// index
		ipno = 1;
		goto ipaddrNextl1;
	//} else {	// 1.3.6.1.2.1.4.20.1.item.ipaddr
	} else {	// 1.3.6.1.2.1.4.20.1.item.ifindex.ipaddr
		union ipaddr_union	ipaddr;

		item = *name;
		if ( item != IPADENTADDR )
			return 0;
		item = *(name + 1);
		if ( item != 1 )
			return 0;
		item = *(name + 2);
		/*
		if ( item < 1 || item > IP_ADDR_MAX_ITEM )
			return 0;
		*/
		//getipaddr(&ipaddr, name+3);
		getipaddr(&ipaddr, name+4);
		for ( ipno=0; ipno < IP_ADDR_NO; ipno++ ) {
			if ( ipaddr.addrl == ipinfo[ipno].ipaddr.addrl )
				break;
		}
		if ( ipno == IP_ADDR_NO )
			return 0;
		ipno++;
		if ( ipno >= IP_ADDR_NO ) {
			if ( item >= IP_ADDR_MAX_ITEM )
				return 0;
			ipno = 1;
			item++;
		}  else {
			ipno++;
		} 
		name += 2;
		*name++ = item;
		*name++ = ipno;	// index
		goto ipaddrNextl1;
	}
	return 0;
}

static	AsnIdType	ipnetNext (MixCookieType cookie, MixNamePtrType name, MixLengthPtrType namelenp)
{
	CIntsType	item;
	int		ipno;

	cookie = cookie;
#if 0
	if ( *namelenp == 0 ) {	// 1.3.6.1.2.1.4
		get_ip_addr_info();
		*name++ = IPNETTOMEDIA;
		*name++ = 1;
		*name++ = 1;
		*name++ = 1;	// index
		ipno = item = 1;
		goto ipnetNextl1;
	} else if ( *namelenp == 1 ) { // 1.3.6.1.2.1.4.22
#else
	if ( *namelenp == 1 ) { // 1.3.6.1.2.1.4.22
#endif
		item = *name;
		if ( item != IPNETTOMEDIA )
			return 0;
		get_ip_addr_info();
		name++;
		*name++ = 1;
		*name++ = 1;
		*name++ = 1;	// index
		ipno = item = 1;
ipnetNextl1:
		*namelenp = 8;
		setipaddrname(&ipinfo[ipno-1].ipaddr, name, namelenp);
		return ipnetRetrieve(ipno, item);
	} else if ( *namelenp == 2 ) {	// 1.3.6.1.2.1.4.22.1
		item = *name;
		if ( item != IPNETTOMEDIA )
			return 0;
		item = *(name + 1);
		if ( item != 1 )
			return 0;
		get_ip_addr_info();
		name += 2;
		*name++ = 1;
		*name++ = 1;	// index
		ipno = item = 1;
		goto ipnetNextl1;
	} else if ( *namelenp == 3 ) {	// 1.3.6.1.2.1.4.22.1.item
		item = *name;
		if ( item != IPNETTOMEDIA )
			return 0;
		item = *(name + 1);
		if ( item != 1 )
			return 0;
		item = *(name + 2);
		if ( item < 1 || item > IP_NET_MAX_ITEM )
			return 0;
		get_ip_addr_info();
		name += 3;
		*name++ = 1;	// index
		ipno = 1;
		goto ipnetNextl1;
	} else {	// 1.3.6.1.2.1.4.22.1.item.ifindex.ipaddr
		union ipaddr_union	ipaddr;

		item = *name;
		if ( item != IPNETTOMEDIA )
			return 0;
		item = *(name + 1);
		if ( item != 1 )
			return 0;
		item = *(name + 2);
		/*
		if ( item < 1 || item > IP_NET_MAX_ITEM )
			return 0;
		*/
		getipaddr(&ipaddr, name+4);
		for ( ipno=0; ipno < IP_ADDR_NO; ipno++ ) {
			if ( ipaddr.addrl == ipinfo[ipno].ipaddr.addrl )
				break;
		}
		if ( ipno == IP_ADDR_NO )
			return 0;
		ipno++; // because I find from 0, but index from 1
		if ( ipno >= IP_ADDR_NO ) {
			if ( item >= IP_NET_MAX_ITEM )
				return 0;
			ipno = 1;
			item++;
		} else {
			ipno++;
		}
		name += 2;
		*name++ = item;
		*name++ = ipno;	// index
		goto ipnetNextl1;
	}
	return 0;
}

static	AsnIdType	iprouteNext (MixCookieType cookie, MixNamePtrType name, MixLengthPtrType namelenp)
{
	CIntsType		item;
	struct iproute_struct	*iprouteptr;
	static int		index;

	cookie = cookie;
#if 0
	if ( *namelenp == 0 ) {	// 1.3.6.1.2.1.4
		get_iproute_info();
		if ( iproutehead == NULL )
			return 0;
		*name++ = IPROUTETABLE;
		*name++ = 1;
		*name++ = 1;
		item = 1;
		iprouteptr = iproutehead;
		goto iprouteNextl1;
	} else if ( *namelenp == 1 ) { // 1.3.6.1.2.1.4.21
#else
	if ( *namelenp == 1 ) { // 1.3.6.1.2.1.4.21
#endif
		item = *name;
		if ( item != IPROUTETABLE )
			return 0;
		get_iproute_info();
		if ( iproutehead == NULL )
			return 0;
		name++;
		*name++ = 1;
		*name++ = 1;
		*name++ = 1;	// index
		index = 1;
		item = 1;
		iprouteptr = iproutehead;
iprouteNextl1:
		//*namelenp = 7;
		*namelenp = 8;
		setipaddrname(&iprouteptr->destaddr, name, namelenp);
		return iprouteRetrieve(iprouteptr, item);
	} else if ( *namelenp == 2 ) {	// 1.3.6.1.2.1.4.21.1
		item = *name;
		if ( item != IPROUTETABLE )
			return 0;
		item = *(name + 1);
		if ( item != 1 )
			return 0;
		get_iproute_info();
		if ( iproutehead == NULL )
			return 0;
		name += 2;
		*name++ = 1;
		*name++ = 1;	// index
		item = 1;
		index = 1;
		iprouteptr = iproutehead;
		goto iprouteNextl1;
	} else if ( *namelenp == 3 ) {	// 1.3.6.1.2.1.4.21.1.item
		item = *name;
		if ( item != IPROUTETABLE )
			return 0;
		item = *(name + 1);
		if ( item != 1 )
			return 0;
		item = *(name + 2);
		if ( item < 1 || item > IP_ROUTE_MAX_ITEM )
			return 0;
		get_iproute_info();
		if ( iproutehead == NULL )
			return 0;
		name += 3;
		*name++ = 1;	// index
		index = 1;
		iprouteptr = iproutehead;
		goto iprouteNextl1;
	//} else {	// 1.3.6.1.2.1.4.21.1.item.destipaddr
	} else {	// 1.3.6.1.2.1.4.21.1.item.ifindex.ipaddr
		struct iproute_struct	iproute;

		item = *name;
		if ( item != IPROUTETABLE )
			return 0;
		item = *(name + 1);
		if ( item != 1 )
			return 0;
		item = *(name + 2);
		/*
		if ( item < 1 || item > IP_ROUTE_MAX_ITEM )
			return 0;
		*/
		//getipaddr(&iproute.destaddr, name+3);
		getipaddr(&iproute.destaddr, name+4);
		iprouteptr = find_iproute_match(&iproute);
		if ( iprouteptr == NULL )
			return 0;
		iprouteptr = iprouteptr->next;
		//index++;
		if ( iprouteptr == NULL ) {
			if ( item >= IP_ROUTE_MAX_ITEM ) 
				return 0;
			item++;
			index = 1;
			iprouteptr = iproutehead;
		} else
			index++;
		name += 2;
		*name++ = item;
		*name++ = index;
		goto iprouteNextl1;
	}
	return 0;
}

#if 0	// mask by Victor Yu. 07-30-2004
static	MixStatusType	ipRelease (MixCookieType cookie)
{
	cookie = cookie;
	return (smpErrorGeneric);
}

static	MixStatusType	ipCreate (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen, AsnIdType asn)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	asn = asn;
	return (smpErrorGeneric);
}

static	MixStatusType	ipDestroy (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	return (smpErrorGeneric);
}
#endif

static	AsnIdType	ipGet (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen)
{
	CIntfType		item;

	cookie = cookie;
	if ( namelen == 2 ) {
		item = *name;
		if ( item == IPROUTEDISCARD && *(name+1) == 0 )
			return ipRetrieve(*name);
	}

	if ( namelen > 2 && *name == IPROUTETABLE && (*(name+1) == 1) && ((item=*(name+2)) >= 1) && item <= IP_ROUTE_MAX_ITEM ) {
		struct iproute_struct	iproute, *iprouteptr;
		get_iproute_info();
		if ( iproutehead == NULL )
			return 0;
		getipaddr(&iproute.destaddr, name+3);
		iprouteptr = find_iproute_match(&iproute);
		if ( iprouteptr == NULL )
			return 0;
		return iprouteRetrieve(iprouteptr, item);
	}

	if ( namelen > 2 && *name == IPADENTADDR && (*(name+1) == 1) && ((item=*(name+2)) >= 1) && item <= IP_ADDR_MAX_ITEM ) {
		int			ipno;
		union ipaddr_union	ipaddr;
		get_ip_addr_info();
		getipaddr(&ipaddr, name+3);
		for ( ipno=0; ipno < IP_ADDR_NO; ipno++ ) {
			if ( ipaddr.addrl == ipinfo[ipno].ipaddr.addrl )
				break;
		}
		if ( ipno == IP_ADDR_NO )
			return 0;
		ipno++;
		return ipaddrRetrieve(ipno, item);
	}

	if ( namelen > 2 && ((item=*name) == IPNETTOMEDIA) && (*(name+1) == 1 ) && ((item=*(name+2)) >= 1) && item <= IP_NET_MAX_ITEM ) {
		int			ipno;
		union ipaddr_union	ipaddr;
		get_ip_addr_info();
		getipaddr(&ipaddr, name+4);
		for ( ipno=0; ipno < IP_ADDR_NO; ipno++ ) {
			if ( ipaddr.addrl == ipinfo[ipno].ipaddr.addrl )
				break;
		}
		if ( ipno == IP_ADDR_NO )
			return 0;
		ipno++;
		return ipnetRetrieve(ipno, item);
	}

	if ((namelen != (MixLengthType) 2) ||
		((item = (CIntfType) *name) < (CIntfType) 1) ||
		(item > (CIntfType)IP_MAXTYPE) || (*(name + 1) != (MixNameType) 0)) {
		return ((AsnIdType) 0);
	}

	return (ipRetrieve (item));
}

#if 0	// mask by Victor Yu. 07-30-2004
static	MixStatusType	ipSet (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen, AsnIdType asn)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	asn = asn;
	return (smpErrorReadOnly);
}
#endif

static	AsnIdType	ipNext (MixCookieType cookie, MixNamePtrType name, MixLengthPtrType namelenp)
{
	CIntfType	item;
	AsnIdType	type;

	cookie = cookie;
	if (*namelenp == (MixLengthType) 0) {
		*namelenp = (MixLengthType) 2;
		*name++ = (MixNameType) 1;
		*name = (MixNameType) 0;
		return (ipRetrieve ((CIntfType) 1));
	}
	else if (*namelenp == (MixLengthType) 1) {
		item = *name;
		if ( item == IPROUTEDISCARD )
			goto ipnextl1;
		if ( item == IPROUTETABLE )
			return iprouteNext(cookie, name, namelenp);
		if ( item == IPADENTADDR )
			return ipaddrNext(cookie, name, namelenp);
		if ( item == IPNETTOMEDIA )
			return ipnetNext(cookie, name, namelenp);
		if (item <= (CIntfType) IP_MAXTYPE) {
ipnextl1:
			*namelenp = (MixLengthType) 2;
			*(++name) = (MixNameType) 0;
			return (ipRetrieve (item));
		}
		else {
			return ((AsnIdType) 0);
		}
	}
	else {
		item = *name;
		if ( item == IPROUTEDISCARD )
			return 0;
		if ( item == IP_MAXTYPE ) {
			*name = IPADENTADDR;
			*(name+1) = 1;
			*namelenp = 2;
			return ipaddrNext(cookie, name, namelenp);
		} else if ( item == IPADENTADDR ) {
			type = ipaddrNext(cookie, name, namelenp);
			if ( type == 0 ) {
				*name = IPROUTETABLE;
				*(name+1) = 1;
				*namelenp = 2;
				return iprouteNext(cookie, name, namelenp);
			} else {
				return type;
			}
		} else if ( item == IPROUTETABLE ) {
			type = iprouteNext(cookie, name, namelenp);
			if ( type == 0 ) {
				*name = IPNETTOMEDIA;
				*(name+1) = 1;
				*namelenp = 2;
				return ipnetNext(cookie, name, namelenp);
			} else {
				return type;
			}
		} else if ( item == IPNETTOMEDIA ) {
			type = ipnetNext(cookie, name, namelenp);
			if ( type == 0 )
				item = IPROUTEDISCARD;
			else
				return type;
		} else {
			item++;
		}
		*namelenp = (MixLengthType) 2;
		*name++ = (MixNameType) item;
		*name = (MixNameType) 0;
		return (ipRetrieve (item));
	}
	return 0;
}

#if 0	// mask by Victor Yu. 07-30-2004
static	MixOpsType	ipOps = {
	ipRelease,
	ipCreate,
	ipDestroy,
	ipNext,
	ipGet,
	ipSet
};
#else 	// add by Victor Yu. 07-30-2004
static	MixOpsType	ipOps = {
	GeneralRelease,
	GeneralCreate,
	GeneralDestroy,
	ipNext,
	ipGet,
	GeneralSet
};
#endif

CVoidType		ipInit (void)
{
	(void) misExport ((MixNamePtrType) "\53\6\1\2\1\4",
		(MixLengthType)6, &ipOps, (MixCookieType) 0);
}
