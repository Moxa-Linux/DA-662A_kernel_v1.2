

#include	<stdio.h>
#include	<netdb.h>
#include	<unistd.h>
#include	<net/if.h>
#include	<linux/sockios.h>
#include	<linux/types.h>
/*
typedef __u16	__le16;
typedef __u16	__be16;
typedef __u32	__le32;
typedef __u32	__be32;
typedef __u64	__le64;
typedef __u64	__be64;
#include	<linux/tcp.h>
*/
#include	<netinet/tcp.h>

#include	"ctypes.h"
#include	"error.h"
#include	"local.h"
#include	"tcp_vars.h"
#include	"mix.h"
#include	"mis.h"
#include	"asn.h"

#define TCP_MAXTYPE	15
#define IP_ADDR_LEN     4
#define PROC_TCP_NODE   "/proc/net/tcp"
#define BUF_LEN         1024
#if defined(ARCH_IA241) || defined(ARCH_IA241_32128) || defined(ARCH_EM1280) || defined (ARCH_UC8400) || defined(ARCH_UC8406)
   #define IP_ADDR_NO      3
#elif defined ARCH_IA240
   #define IP_ADDR_NO      2
#elif defined ARCH_UC8000
   #define IP_ADDR_NO	2
#elif defined ARCH_DA662A
   #define IP_ADDR_NO	4
#elif defined ARCH_W341
   #define IP_ADDR_NO      2
#elif defined ARCH_W345
   #define IP_ADDR_NO      1
#elif defined ARCH_UC_7112_LX_PLUS
   #define IP_ADDR_NO      2
#elif defined ARCH_W311
   #define IP_ADDR_NO      1
#elif defined ARCH_W321
   #define IP_ADDR_NO      1
#elif defined ARCH_W315
   #define IP_ADDR_NO      1
#elif defined ARCH_W315_NJKL
   #define IP_ADDR_NO      1
#elif defined ARCH_W325
   #define IP_ADDR_NO      1
#elif defined ARCH_RNAS1200
   #define IP_ADDR_NO      2
#elif defined(ARCH_W406) || defined(ARCH_SMG1100) || defined(ARCH_W416) //Added by Wade ,03-09-2009 
	#define IP_ADDR_NO      1
#elif defined(ARCH_IA261) || defined(ARCH_IA262) || defined(ARCH_IA260) || defined(ARCH_EM2260) || defined(ARCH_IA261_256)
	#ifdef HOSON_ODM
		#define IP_ADDR_NO      1
	#else
		#define IP_ADDR_NO      2
	#endif	// HOSON_ODM
#endif
#define LNAME_PATTERN   0x81
#define LNAME_VALUE     0x80
#define BYTE_HIGH_BIT   0x80
#define BYTE_LOW_VALUE  0x7F
#define BYTE_LOW_BIT_NO 7

union ipaddr_union {
	unsigned long   addrl;
	unsigned char   addrc[IP_ADDR_LEN];
};

struct tcpaddr_struct {
	union ipaddr_union      lipaddr;
	int                     lport;
	union ipaddr_union      ripaddr;
	int                     rport;
	int			connstate;
	struct tcpaddr_struct   *next;
};
static struct tcpaddr_struct    *tcpaddrhead=NULL;
#if defined(ARCH_IA241) || defined(ARCH_IA241_32128) || defined(ARCH_EM1280) || defined(ARCH_UC8400) || defined(ARCH_UC8406)
static char                     *ifname[IP_ADDR_NO]={"eth0", "eth1", "eth2"};
#elif defined(ARCH_IA261) || defined(ARCH_IA262) || defined(ARCH_IA260) || defined(ARCH_EM2260) || defined(ARCH_IA261_256)
	#ifdef HOSON_ODM
static char                     *ifname[IP_ADDR_NO]={"eth0"};
	#else
static char                     *ifname[IP_ADDR_NO]={"eth0", "eth1"};
	#endif	// HOSON_ODM
#elif defined(ARCH_W406) || defined(ARCH_SMG1100) || defined(ARCH_W416) //Added by Wade ,03-09-2009 
static char                     *ifname[IP_ADDR_NO]={"eth0"};
#elif defined(ARCH_DA662A)
static char                     *ifname[IP_ADDR_NO]={"eth0", "eth1", "eth2", "eth3"};
#else
static char                     *ifname[IP_ADDR_NO]={"eth0", "eth1"};
#endif
static union ipaddr_union       ifipaddr[IP_ADDR_NO];

struct tcp_mib
{
 	unsigned long	TcpRtoAlgorithm;
 	unsigned long	TcpRtoMin;
 	unsigned long	TcpRtoMax;
 	unsigned long	TcpMaxConn;
 	unsigned long	TcpActiveOpens;
 	unsigned long	TcpPassiveOpens;
 	unsigned long	TcpAttemptFails;
 	unsigned long	TcpEstabResets;
 	unsigned long	TcpCurrEstab;
 	unsigned long	TcpInSegs;
 	unsigned long	TcpOutSegs;
 	unsigned long	TcpRetransSegs;
        unsigned long   TcpInErrs;
        unsigned long   TcpOutRsts;
};

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

static struct tcpaddr_struct *find_match(struct tcpaddr_struct *tcpaddr)
{
	struct tcpaddr_struct	*head;

	head = tcpaddrhead;
	while ( head ) {
		if ( head->lipaddr.addrl == tcpaddr->lipaddr.addrl &&
		     head->lport == tcpaddr->lport &&
		     head->ripaddr.addrl == tcpaddr->ripaddr.addrl &&
		     head->rport == tcpaddr->rport )
			return head;
		head = head->next;
	}
	return head;
}

static void get_tcpaddr_info(void)
{
	struct tcpaddr_struct	*tcpaddrptr, *tail;
	int			fd, i, ipaddrno;
	struct ifreq		ifr;
	FILE			*in;
	char			line[BUF_LEN];

	// first free has got udp address information
	while ( tcpaddrhead ) {
		tcpaddrptr = tcpaddrhead;
		tcpaddrhead = tcpaddrhead->next;
		free(tcpaddrptr);
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

	// now to get the TCP local & remote address with port information
	in = fopen(PROC_TCP_NODE, "r");
	if ( !in )
		return;
	tail = tcpaddrhead;
	while ( line == fgets(line, BUF_LEN, in) ) {
		struct tcpaddr_struct	tcpaddr;
		tcpaddr.next = NULL;
		if ( 6 == sscanf(line, "%4d: %08X:%04X %08X:%04X %02X", &i, &tcpaddr.lipaddr.addrl, &tcpaddr.lport, &tcpaddr.ripaddr.addrl, &tcpaddr.rport, &tcpaddr.connstate) ) {
			switch ( tcpaddr.connstate ) {
			case TCP_CLOSE :	tcpaddr.connstate = 1; break;
			case TCP_LISTEN :	tcpaddr.connstate = 2; break;
			case TCP_SYN_SENT :	tcpaddr.connstate = 3; break;
			case TCP_SYN_RECV :	tcpaddr.connstate = 4; break;
			case TCP_ESTABLISHED :	tcpaddr.connstate = 5; break;
			case TCP_FIN_WAIT1 :	tcpaddr.connstate = 6; break;
			case TCP_FIN_WAIT2 :	tcpaddr.connstate = 7; break;
			case TCP_CLOSE_WAIT :	tcpaddr.connstate = 8; break;
			case TCP_LAST_ACK :	tcpaddr.connstate = 9; break;
			case TCP_CLOSING :	tcpaddr.connstate = 10; break;
			case TCP_TIME_WAIT :	tcpaddr.connstate = 11; break;
			default :		tcpaddr.connstate = 1; break;
			}
			if ( tcpaddr.lipaddr.addrl == 0 ) {
				for ( i=0; i<ipaddrno; i++ ) {
					tcpaddr.lipaddr = ifipaddr[i];
					if ( find_match(&tcpaddr) )
						continue;
					tcpaddrptr = malloc(sizeof(struct tcpaddr_struct));
					if ( tcpaddrptr == NULL )
						break;
					*tcpaddrptr = tcpaddr;
					if ( tcpaddrhead == NULL )
						tcpaddrhead = tcpaddrptr;
					else
						tail->next = tcpaddrptr;
					tail = tcpaddrptr;
				}
				if ( i != ipaddrno )
					break;
			} else {
				tcpaddrptr = malloc(sizeof(struct tcpaddr_struct));
				if ( tcpaddrptr == NULL )
					break;
				*tcpaddrptr = tcpaddr;
				if ( tcpaddrhead == NULL ) 
					tcpaddrhead = tcpaddrptr;
				else
					tail->next = tcpaddrptr;
				tail = tcpaddrptr;
			}
		}
	}
	fclose(in);
}

static	AsnIdType	tcpRetrieve (CIntfType item)
{
	struct tcp_mib	tcpstat;
	AsnIdType	asnresult;
        FILE 		*in;
        char		line[BUF_LEN];

	in = fopen ("/proc/net/snmp", "r");
	if (! in) {
    		printf("tcpRetrieve() Error opening /proc/net/snmp\n");	
		return 0;
	}

	while (line == fgets (line, BUF_LEN, in)) {
		if (12 == sscanf (line, 
				"Tcp: %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu\n",
	&tcpstat.TcpRtoAlgorithm,&tcpstat.TcpRtoMin,&tcpstat.TcpRtoMax, &tcpstat.TcpMaxConn,
	&tcpstat.TcpActiveOpens,&tcpstat.TcpPassiveOpens,&tcpstat.TcpAttemptFails,
	&tcpstat.TcpEstabResets,&tcpstat.TcpCurrEstab, &tcpstat.TcpInSegs,
	&tcpstat.TcpOutSegs,&tcpstat.TcpRetransSegs))
			break;
	}
	fclose (in);
	
	switch (item) {
	case TCPRTOALGORITHM: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType) 2,tcpstat.TcpRtoAlgorithm);
		break;
	case TCPRTOMIN: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType) 2,tcpstat.TcpRtoMin);
		break;
	case TCPRTOMAX: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType) 2,tcpstat.TcpRtoMax);
		break;
	case TCPMAXCONN: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType) 2,tcpstat.TcpMaxConn);
		break;
	case TCPACTIVEOPENS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,tcpstat.TcpActiveOpens);
		break;
	case TCPPASSIVEOPENS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,tcpstat.TcpPassiveOpens);
		break;
	case TCPATTEMPTFAILS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,tcpstat.TcpAttemptFails);
		break;
	case TCPESTABRESETS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,tcpstat.TcpEstabResets);
		break;
	case TCPCURRESTAB: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 2,tcpstat.TcpCurrEstab);
		break;
	case TCPINSEGS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,tcpstat.TcpInSegs);
		break;
	case TCPOUTSEGS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,tcpstat.TcpOutSegs);
		break;
	case TCPRETRANSSEGS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,tcpstat.TcpRetransSegs);
		break;
	case TCPINERRS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,tcpstat.TcpInErrs);
		break;
	case TCPOUTRSTS: 
		asnresult = asnUnsl (asnClassApplication, (AsnTagType) 1,tcpstat.TcpOutRsts);
		break;
	case TCPCONNTABLE:
	default:
		break;
	}	
	
	return (asnresult);
}

#define TCP_CONN_STATE			1
#define TCP_CONN_LOCAL_ADDRESS		2
#define TCP_CONN_LOCAL_PORT		3
#define TCP_CONN_REM_ADDRESS		4
#define TCP_CONN_REM_PORT		5
#define TCP_CONN_MAX_ITEM		5
static	AsnIdType	tcpconnRetrieve (struct tcpaddr_struct *tcpaddr, CIntfType item)
{
	AsnIdType       asnresult=0;

	switch ( item ) {
	case TCP_CONN_STATE :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, tcpaddr->connstate);
		break;
	case TCP_CONN_LOCAL_ADDRESS :
		asnresult = asnOctetString (asnClassApplication, (AsnTagType)0, tcpaddr->lipaddr.addrc, 4);
		break;
	case TCP_CONN_LOCAL_PORT :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, tcpaddr->lport);
		break;
	case TCP_CONN_REM_ADDRESS :
		asnresult = asnOctetString (asnClassApplication, (AsnTagType)0, tcpaddr->ripaddr.addrc, 4);
		break;
	case TCP_CONN_REM_PORT :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, tcpaddr->rport);
		break;
	default :
		break;
	}
	return asnresult;
}

#if 0	// mask by Victor Yu. 07-30-2004
static	MixStatusType	tcpRelease (MixCookieType cookie)
{
	cookie = cookie;
	return (smpErrorGeneric);
}

static	MixStatusType	tcpCreate (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen, AsnIdType asn)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	asn = asn;
	return (smpErrorGeneric);
}

static	MixStatusType	tcpDestroy (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	return (smpErrorGeneric);
}
#endif

static	AsnIdType	tcpGet (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen)
{
	CIntfType		item;

	cookie = cookie;
	if ( namelen > 2 && *name == TCPCONNTABLE && (*(name+1) == 1) && ((item=*(name+2)) > 1) && item <= TCP_CONN_MAX_ITEM ) {
		struct tcpaddr_struct	tcpaddr;
		int			len;

		get_tcpaddr_info();
		if ( tcpaddrhead == NULL )
			return 0;
		name += 3;
		len = getipaddr(&tcpaddr.lipaddr, name);
		len += getport(&tcpaddr.lport, name+len);
		len += getipaddr(&tcpaddr.ripaddr, name+len);
		len += getport(&tcpaddr.rport, name+len);
		if ( find_match(&tcpaddr) == NULL )
			return 0;
		return tcpconnRetrieve(&tcpaddr, item);
	}

	if ((namelen != (MixLengthType) 2) ||
		((item = (CIntfType) *name) < (CIntfType) 1) ||
		(item > (CIntfType) TCP_MAXTYPE) || 
		item == TCPCONNTABLE || (*(name + 1) != (MixNameType) 0)) {
		return ((AsnIdType) 0);
	}
	else {
		return (tcpRetrieve (item));
	}
}

#if 0	// mask by Victor Yu. 07-30-3004
static	MixStatusType	tcpSet (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen, AsnIdType asn)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	asn = asn;
	return (smpErrorReadOnly);
}
#endif

static	AsnIdType	tcpconnNext (MixCookieType cookie, MixNamePtrType name, MixLengthPtrType namelenp)
{
	CIntsType		item;
	struct tcpaddr_struct	*tcpaddrptr;
	static int		index;

	cookie = cookie;
#if 0
	if ( *namelenp == 0 ) {	// 1.3.6.1.2.1.6
		get_tcpaddr_info();
		if ( tcpaddrhead == NULL )
			return 0;
		*name++ = TCPCONNTABLE;
		*name++ = 1;
		*name++ = 1;	// first item;
		item = 1;
		tcpaddrptr = tcpaddrhead;
		goto tcpconnNextl1;
	} else if ( *namelenp == 1 ) { // 1.3.6.1.2.1.6.13
#else
	if ( *namelenp == 1 ) { // 1.3.6.1.2.1.6.13
#endif
		item = *name;
		if ( item != TCPCONNTABLE )
			return 0;
		get_tcpaddr_info();
		if ( tcpaddrhead == NULL )
			return 0;
		name++;
		*name++ = 1;
		*name++ = 1;	// first item;
		*name++ = 1;	// index
		index = 1;
		item = 1;
		tcpaddrptr = tcpaddrhead;
tcpconnNextl1:
		//*namelenp = 3;
		*namelenp = 4;	// add a index
		name += setipaddrname(&tcpaddrptr->lipaddr, name, namelenp);
		name += setportname(tcpaddrptr->lport, name, namelenp);
		name += setipaddrname(&tcpaddrptr->ripaddr, name, namelenp);
		name += setportname(tcpaddrptr->rport, name, namelenp);
		return tcpconnRetrieve(tcpaddrptr, item);
	} else if ( *namelenp == 2 ) { // 1.3.6.1.2.1.6.13.1
		item = *name;
		if ( item != TCPCONNTABLE )
			return 0;
		item = *(name + 1);
		if ( item != 1 )
			return 0;
		get_tcpaddr_info();
		if ( tcpaddrhead == NULL )
			return 0;
		name += 2;
		*name++ = 1;	// first item;
		*name++ = 1;	// index
		index = 1;
		item = 1;
		tcpaddrptr = tcpaddrhead;
		goto tcpconnNextl1;
	} else if ( *namelenp == 3 ) { // 1.3.6.1.2.1.6.13.1.item
		item = *name;
		if ( item != TCPCONNTABLE )
			return 0;
		item = *(name + 1);
		if ( item != 1 )
			return 0;
		item = *(name + 2);
		if ( item < 1 || item > TCP_CONN_MAX_ITEM )
			return 0;
		get_tcpaddr_info();
		if ( tcpaddrhead == NULL )
			return 0;
		name += 3;
		*name++ = 1;	// index
		index = 1;
		tcpaddrptr = tcpaddrhead;
		goto tcpconnNextl1;
	//} else {	// 1.3.6.1.2.1.6.13.1.item.lip.lport.rip.rport
	} else {	// 1.3.6.1.2.1.6.13.1.item.index.lip.lport.rip.rport
		struct tcpaddr_struct	tcpaddr;
		int			len;

		if ( tcpaddrhead == NULL )
			return 0;
		item = *name;
		if ( item != TCPCONNTABLE )
			return 0;
		item = *(name + 1);
		if ( item != 1 )
			return 0;
		item = *(name + 2);
		if ( item < 1 || item > TCP_CONN_MAX_ITEM )
			return 0;
		name += 2;
		/*
		len = getipaddr(&tcpaddr.lipaddr, name+1);
		len += getport(&tcpaddr.lport, name+len+1);
		len += getipaddr(&tcpaddr.ripaddr, name+1+len);
		len += getport(&tcpaddr.rport, name+len+1);
		*/
		len = getipaddr(&tcpaddr.lipaddr, name+2);
		len += getport(&tcpaddr.lport, name+len+2);
		len += getipaddr(&tcpaddr.ripaddr, name+2+len);
		len += getport(&tcpaddr.rport, name+len+2);
		tcpaddrptr = find_match(&tcpaddr);
		if ( tcpaddrptr == NULL ) {
			return 0;
		}
		tcpaddrptr = tcpaddrptr->next;
		if ( tcpaddrptr == NULL ) {
			item++;
			index = 1;
			tcpaddrptr = tcpaddrhead;
		} else
			index++;
		*name++ = item;
		*name++ = index;
		goto tcpconnNextl1;
	}
	return 0;
}

static	AsnIdType	tcpNext (MixCookieType cookie, MixNamePtrType name, MixLengthPtrType namelenp)
{
	CIntfType		item;

	cookie = cookie;

	if (*namelenp == (MixLengthType) 0) {
		*namelenp = (MixLengthType) 2;
		*name++ = (MixNameType) 1;
		*name = (MixNameType) 0;
		return (tcpRetrieve ((CIntfType) 1));
	}
	else if (*namelenp == (MixLengthType) 1) {
		item = *name;
		if ( item == TCPCONNTABLE )
			return tcpconnNext(cookie, name, namelenp);
		if ( item <= TCP_MAXTYPE ) {
			*namelenp = (MixLengthType) 2;
			*(++name) = (MixNameType) 0;
			return (tcpRetrieve (item));
		}
		else {
			return ((AsnIdType) 0);
		}
	}
	else {
		AsnIdType	type;

		item = *name;
		if ( item >= TCP_MAXTYPE )
			return 0;
		if ( item == TCPRETRANSSEGS ) {
			*name = TCPCONNTABLE;
			*(name+1) = 1;
			*namelenp = 2;
			return tcpconnNext(cookie, name, namelenp);
		}
		if ( item == TCPCONNTABLE ) {
			type = tcpconnNext(cookie, name, namelenp);
			if ( type )
				return type;
		}
		item++;
		*name++ = item;
		*name++ = 0;
		*namelenp = 2;
		return tcpRetrieve(item);
	}
	return 0;
}

#if 0	// mask by Victor Yu. 07-30-2004
static	MixOpsType	tcpOps = {
	tcpRelease,
	tcpCreate,
	tcpDestroy,
	tcpNext,
	tcpGet,
	tcpSet
};
#else
static	MixOpsType	tcpOps = {
	GeneralRelease,
	GeneralCreate,
	GeneralDestroy,
	tcpNext,
	tcpGet,
	GeneralSet
};
#endif

CVoidType		tcpInit (void)
{
	(void) misExport ((MixNamePtrType) "\53\6\1\2\1\6",
		(MixLengthType) 6, & tcpOps, (MixCookieType) 0);
}
