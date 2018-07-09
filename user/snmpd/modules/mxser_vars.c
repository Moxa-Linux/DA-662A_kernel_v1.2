
#include	<net/if.h>
#include	<stdio.h>
#include	<netdb.h>
#include	<unistd.h>
#include	<time.h>
#include	<linux/sockios.h>

#include	"ctypes.h"
#include	"error.h"
#include	"local.h"
#include	"mix.h"
#include	"mis.h"
#include	"asn.h"

#define PATH_PROC_MXSER		"/proc/driver/mxser"
#define BUF_LEN			256

static CIntsType	mxserNumberInt=0;

struct mxser_info {
	u_long	inbytes;
	u_long	outbytes;
	int	speed;
	int	opened;
	int	interface;
	int	databits;
	int	stopbits;
	int	parity;
	int	ctsstate;
	int	dsrstate;
	int	dcdstate;
	int	rtsstate;
	int	dtrstate;
};

/* 
 * The inpurt port number from 0.
 */
static void	mxser_get_info(struct mxser_info *info, int port)
{
	FILE			*in;
	char			line[BUF_LEN];

	memset(info, 0, sizeof(struct mxser_info));
	in = fopen(PATH_PROC_MXSER, "r");
	if ( in ) {
		// skip to get total port numbers
		fgets(line, BUF_LEN, in);
		
		// skip each port information until I want port number information
		while ( port-- ) fgets(line, BUF_LEN, in);

		// to get the object port information
		if ( line == fgets(line, BUF_LEN, in) ) {
			sscanf(line, "%lu %lu %d %d %d %d %d %d %d %d %d %d %d", &info->inbytes, &info->outbytes, &info->speed, &info->opened, &info->interface, &info->databits, &info->stopbits, &info->parity, &info->ctsstate, &info->dsrstate, &info->dcdstate, &info->rtsstate, &info->dtrstate);
		}

		fclose(in);
	}
}

#define MXSER_GEN_INDEX		1
#define MXSER_GEN_TYPE		2
#define MXSER_GEN_IN_SIG_NO	3
#define MXSER_GEN_OUT_SIG_NO	4
#define MXSER_GEN_IN_SPEED	5
#define MXSER_GEN_OUT_SPEED	6
#define MXSER_GEN_MAX_ITEM	6
static	AsnIdType	mxserGenRetrieve (CIntfType port, CIntfType item)
{
	AsnIdType		asnresult=0;
	struct mxser_info	info;

	mxser_get_info(&info, port-1);
	switch (item) {
	case MXSER_GEN_INDEX: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, port);
		break;
	case MXSER_GEN_TYPE: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, info.interface);
		break;
	case MXSER_GEN_IN_SIG_NO: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, 3);
		break;
	case MXSER_GEN_OUT_SIG_NO: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, 2);
		break;
	case MXSER_GEN_IN_SPEED: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, info.speed);
		break;
	case MXSER_GEN_OUT_SPEED: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, info.speed);
	default:
		break;
	}
	return (asnresult);
}

#define MXSER_ASYNC_INDEX	1
#define MXSER_ASYNC_BITS	2
#define MXSER_ASYNC_STOP_BITS	3
#define MXSER_ASYNC_PARITY	4
#define MXSER_ASYNC_MAX_ITEM	4
static	AsnIdType	mxserAsyncRetrieve (CIntfType port, CIntfType item)
{
	AsnIdType		asnresult=0;
	struct mxser_info	info;

	mxser_get_info(&info, port-1);
	switch (item) {
	case MXSER_ASYNC_INDEX: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, port);
		break;
	case MXSER_ASYNC_BITS: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, info.databits);
		break;
	case MXSER_ASYNC_STOP_BITS: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, info.stopbits);
		break;
	case MXSER_ASYNC_PARITY: 
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, info.parity);
		break;
	default:
		break;
	}
	return (asnresult);
}

#define MXSER_IN_SIG_INDEX	1
#define MXSER_IN_SIG_NAME	2
#define MXSER_IN_SIG_STATE	3
#define MXSER_IN_SIG_MAX_ITEM	3
#define M_RTS			1
#define M_CTS			2
#define M_DSR			3
#define M_DTR			4
#define M_RI			5
#define M_DCD			6
#define MAX_SIG			6
static	AsnIdType	mxserInSigRetrieve (CIntfType port, CIntfType sig, CIntfType item)
{
	AsnIdType		asnresult=0;
	struct mxser_info	info;

	mxser_get_info(&info, port-1);
	switch ( item ) {
	case MXSER_IN_SIG_INDEX :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, port);
		break;
	case MXSER_IN_SIG_NAME :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, sig);
		break;
	case MXSER_IN_SIG_STATE :
		switch ( sig ) {
		case M_CTS :
			asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, info.ctsstate);
			break;
		case M_DSR :
			asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, info.dsrstate);
			break;
		case M_DCD :
			asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, info.dcdstate);
			break;
		default :
			asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, 1);
			break;
		}
		break;
	default:
		break;
	}
	return (asnresult);
}

#define MXSER_OUT_SIG_INDEX	1
#define MXSER_OUT_SIG_NAME	2
#define MXSER_OUT_SIG_STATE	3
#define MXSER_OUT_SIG_MAX_ITEM	3
static	AsnIdType	mxserOutSigRetrieve (CIntfType port, CIntfType sig, CIntfType item)
{
	AsnIdType		asnresult=0;
	struct mxser_info	info;

	mxser_get_info(&info, port-1);
	switch ( item ) {
	case MXSER_OUT_SIG_INDEX :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, port);
		break;
	case MXSER_OUT_SIG_NAME :
		asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, sig);
		break;
	case MXSER_OUT_SIG_STATE :
		switch ( sig ) {
		case M_RTS :
			asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, info.rtsstate);
			break;
		case M_DTR :
			asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, info.dtrstate);
			break;
		default :
			asnresult = asnUnsl (asnClassUniversal, (AsnTagType)2, 1);
			break;
		}
		break;
	default:
		break;
	}
	return (asnresult);
}

#if 0	// mask by Victor Yu. 07-30-2004
static	MixStatusType	mxserGenRelease (MixCookieType cookie)
{
	cookie = cookie;
	return (smpErrorGeneric);
}

static	MixStatusType	mxserGenCreate (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen, AsnIdType asn)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	asn = asn;
	return (smpErrorGeneric);
}

static	MixStatusType	mxserGenDestroy (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	return (smpErrorGeneric);
}
#endif

static	AsnIdType	mxserGenGet (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen)
{
	CIntfType		port, item;

	cookie = cookie;
	if ((namelen != (MixLengthType) 2) ||
		((port=(CIntfType)*(name+1)) < (CIntfType) 1) ||
		(port > (CIntfType)(mxserNumberInt)) ||
		((item = (CIntfType) *name) < (CIntfType) 1) ||
		(item > (CIntfType) (MXSER_GEN_MAX_ITEM)) ) {
		return ((AsnIdType) 0);
	} else {
		return (mxserGenRetrieve (port, item));
	}
}

static	AsnIdType	mxserAsyncGet (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen)
{
	CIntfType	port, item;

	cookie = cookie;
	if ((namelen != (MixLengthType) 2) ||
		((port=(CIntfType)*(name+1)) < (CIntfType) 1) ||
		(port > (CIntfType)(mxserNumberInt)) ||
		((item = (CIntfType) *name) < (CIntfType) 1) ||
		(item > (CIntfType) (MXSER_ASYNC_MAX_ITEM)) ) {
		return ((AsnIdType) 0);
	} else {
		return (mxserAsyncRetrieve (port, item));
	}
}

static	AsnIdType	mxserInSigGet (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen)
{
	CIntfType	item, port, sig;

	cookie = cookie;
	if ( namelen != 3 )
		return 0;
	port = *(name + 1);
	if ( port < 1 || port > mxserNumberInt )
		return 0;
	sig = *(name + 2);
	if ( sig < 1 || (sig != M_CTS && sig != M_DSR && sig != M_DCD) )
		return 0;
	item = *name;
	if ( item < 1 || item > MXSER_IN_SIG_MAX_ITEM )
		return 0;
	return mxserInSigRetrieve(port, sig, item);
}

static	AsnIdType	mxserOutSigGet (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen)
{
	CIntfType	item, port, sig;

	cookie = cookie;
	if ( namelen != 3 )
		return 0;
	port = *(name + 1);
	if ( port < 1 || port > mxserNumberInt )
		return 0;
	sig = *(name + 2);
	if ( sig < 1 || (sig != M_RTS && sig != M_DTR) )
		return 0;
	item = *name;
	if ( item < 1 || item > MXSER_OUT_SIG_MAX_ITEM )
		return 0;
	return mxserOutSigRetrieve(port, sig, item);
}

#if 0	// mask by Victor Yu. 07-30-2004
static	MixStatusType	mxserGenSet (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen, AsnIdType asn)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	asn = asn;
	return (smpErrorReadOnly);
}
#endif

static	AsnIdType	mxserGenNext (MixCookieType cookie, MixNamePtrType name, MixLengthPtrType namelenp)
{
	CIntfType		port, item;

	cookie = cookie;
	if (*namelenp == (MixLengthType) 0) {
		*namelenp = (MixLengthType) 2;
		*name++ = (MixNameType) 1;
		*name = 1;
		return (mxserGenRetrieve (1, 1));
	}
	else if (*namelenp == (MixLengthType) 1) {
		item = (CIntfType)*name;
		if ( item > MXSER_GEN_MAX_ITEM )
			return ((AsnIdType) 0);
		*namelenp = (MixLengthType) 2;
		*(++name) = (MixNameType)1;
		return (mxserGenRetrieve (1, item));
	}
	else {
		item = (CIntfType)*name;
		port = (CIntfType)*(name + 1);
		if ( port >= mxserNumberInt && item >= MXSER_GEN_MAX_ITEM )
			return ((AsnIdType) 0);
		*namelenp = (MixLengthType)2;
		if ( port >= mxserNumberInt ) {
			port = 1;
			item++;
		} else {
			port++;
		}
		*name++ = item;
		*name = port;
		return (mxserGenRetrieve (port, item));
	}
	return 0;
}

static	AsnIdType	mxserAsyncNext (MixCookieType cookie, MixNamePtrType name, MixLengthPtrType namelenp)
{
	CIntfType		port, item;

	cookie = cookie;
	if (*namelenp == (MixLengthType) 0) {
		*namelenp = (MixLengthType) 2;
		*name++ = (MixNameType) 1;
		*name = 1;
		return (mxserAsyncRetrieve (1, 1));
	}
	else if (*namelenp == (MixLengthType) 1) {
		item = (CIntfType)*name;
		if ( item > MXSER_ASYNC_MAX_ITEM )
			return ((AsnIdType) 0);
		*namelenp = (MixLengthType) 2;
		*(++name) = (MixNameType)1;
		return (mxserAsyncRetrieve (1, item));
	}
	else {
		item = (CIntfType)*name;
		port = (CIntfType)*(name + 1);
		if ( port >= mxserNumberInt && item >= MXSER_ASYNC_MAX_ITEM )
			return ((AsnIdType) 0);
		*namelenp = (MixLengthType)2;
		if ( port >= mxserNumberInt ) {
			port = 1;
			item++;
		} else {
			port++;
		}
		*name++ = item;
		*name = port;
		return (mxserAsyncRetrieve (port, item));
	}
	return 0;
}

static	AsnIdType	mxserInSigNext (MixCookieType cookie, MixNamePtrType name, MixLengthPtrType namelenp)
{
	CIntfType	port, sig, item;

	cookie = cookie;
	if ( *namelenp == 0 ) {
		*namelenp = 3;
		*name++ = 1;	// item
		*name++ = 1;	// port number
		*name++ = M_CTS;	// signal
		return mxserInSigRetrieve(1, M_CTS, 1);
	} else if ( *namelenp == 1 ) {
		item = *name;
		if ( item < 1 || item > MXSER_IN_SIG_MAX_ITEM )
			return 0;
		*namelenp = 3;
		*(++name) = 1;	// port number
		*(++name) = M_CTS;	// signal
		return mxserInSigRetrieve(1, M_CTS, item);
	} else if ( *namelenp == 2 ) {
		item = *name;
		if ( item < 1 || item > MXSER_IN_SIG_MAX_ITEM )
			return 0;
		port = *(name + 1);
		if ( port < 1 || port > mxserNumberInt )
			return 0;
		*namelenp = 3;
		*(name+2) = M_CTS;	// signal;
		return mxserInSigRetrieve(port, M_CTS, item);
	} else {
		item = *name;
		port = *(name + 1);
		sig = *(name + 2);
		if ( item >= MXSER_IN_SIG_MAX_ITEM && port >= mxserNumberInt && sig >= MAX_SIG )
			return 0;
		if ( port >= mxserNumberInt ) {
			port = 1;
			switch ( sig ) {
			case M_CTS :
				sig = M_DSR;
				break;
			case M_DSR :
				sig = M_DCD;
				break;
			case M_DCD :
				sig = M_CTS;
				item++;
				break;
			default :
				break;
			}
		} else {
			port++;
		}
		*namelenp = 3;
		*name++ = item;
		*name++ = port;
		*name++ = sig;
		return mxserInSigRetrieve(port, sig, item);
	}
	return 0;
}

static	AsnIdType	mxserOutSigNext (MixCookieType cookie, MixNamePtrType name, MixLengthPtrType namelenp)
{
	CIntfType	port, sig, item;

	cookie = cookie;
	if ( *namelenp == 0 ) {
		*namelenp = 3;
		*name++ = 1;	// item
		*name++ = 1;	// port number
		*name++ = M_RTS;	// signal
		return mxserOutSigRetrieve(1, M_RTS, 1);
	} else if ( *namelenp == 1 ) {
		item = *name;
		if ( item < 1 || item > MXSER_OUT_SIG_MAX_ITEM )
			return 0;
		*namelenp = 3;
		*(++name) = 1;	// port number
		*(++name) = M_RTS;	// signal
		return mxserOutSigRetrieve(1, M_RTS, item);
	} else if ( *namelenp == 2 ) {
		item = *name;
		if ( item < 1 || item > MXSER_OUT_SIG_MAX_ITEM )
			return 0;
		port = *(name + 1);
		if ( port < 1 || port > mxserNumberInt )
			return 0;
		*namelenp = 3;
		*(name+2) = M_RTS;	// signal;
		return mxserOutSigRetrieve(port, M_RTS, item);
	} else {
		item = *name;
		port = *(name + 1);
		sig = *(name + 2);
		if ( item >= MXSER_OUT_SIG_MAX_ITEM && port >= mxserNumberInt && sig >= M_DTR )
			return 0;
		if ( port >= mxserNumberInt ) {
			port = 1;
			switch ( sig ) {
			case M_RTS :
				sig = M_DTR;
				break;
			case M_DTR :
				sig = M_RTS;
				item++;
				break;
			default :
				break;
			}
		} else {
			port++;
		}
		*namelenp = 3;
		*name++ = item;
		*name++ = port;
		*name++ = sig;
		return mxserOutSigRetrieve(port, sig, item);
	}
	return 0;
}

#if 0	// mask by Victor Yu. 07-30-2004
static	MixOpsType	mxserGenOps = {
	mxserGenRelease,
	mxserGenCreate,
	mxserGenDestroy,
	mxserGenNext,
	mxserGenGet,
	mxserGenSet
};
#else	// add by Victor Yu. 07-30-2004
static	MixOpsType	mxserGenOps = {
	GeneralRelease,
	GeneralCreate,
	GeneralDestroy,
	mxserGenNext,
	mxserGenGet,
	GeneralSet
};
#endif

#if 0	// mask by Victor Yu. 07-30-2004
static	MixOpsType	mxserAsyncOps = {
	mxserGenRelease,
	mxserGenCreate,
	mxserGenDestroy,
	mxserAsyncNext,
	mxserAsyncGet,
	mxserGenSet
};
#else	// add by Victor Yu. 07-30-2004
static	MixOpsType	mxserAsyncOps = {
	GeneralRelease,
	GeneralCreate,
	GeneralDestroy,
	mxserAsyncNext,
	mxserAsyncGet,
	GeneralSet
};
#endif

#if 0	// mask by Victor Yu. 07-30-2004
static	MixOpsType	mxserInSigOps = {
	mxserGenRelease,
	mxserGenCreate,
	mxserGenDestroy,
	mxserInSigNext,
	mxserInSigGet,
	mxserGenSet
};
#else	// add by Victor Yu. 07-30-2004
static	MixOpsType	mxserInSigOps = {
	GeneralRelease,
	GeneralCreate,
	GeneralDestroy,
	mxserInSigNext,
	mxserInSigGet,
	GeneralSet
};
#endif

#if 0	// mask by Victor Yu. 07-30-2004
static	MixOpsType	mxserOutSigOps = {
	mxserGenRelease,
	mxserGenCreate,
	mxserGenDestroy,
	mxserOutSigNext,
	mxserOutSigGet,
	mxserGenSet
};
#else	// add by Victor Yu. 07-30-2004
static	MixOpsType	mxserOutSigOps = {
	GeneralRelease,
	GeneralCreate,
	GeneralDestroy,
	mxserOutSigNext,
	mxserOutSigGet,
	GeneralSet
};
#endif

CVoidType		mxserInit (void)
{
	FILE	*in;
	char	line[BUF_LEN];

	in = fopen(PATH_PROC_MXSER, "r");
	if ( in ) {
		if ( line == fgets(line, BUF_LEN, in) ) {
			sscanf(line, "%d", &mxserNumberInt);
		}
		fclose(in);
	} else
		return;
	if ( !mxserNumberInt )
		return;
	(void)mivIntlRO ((MixNamePtrType) "\53\6\1\2\1\12\41\1",
		(MixLengthType)8, (CIntlPtrType)&mxserNumberInt);
	(void) misExport ((MixNamePtrType)"\53\6\1\2\1\12\41\2\1",
		(MixLengthType)9, &mxserGenOps, (MixCookieType) 0);
	(void) misExport ((MixNamePtrType)"\53\6\1\2\1\12\41\3\1",
		(MixLengthType)9, &mxserAsyncOps, (MixCookieType) 0);
	(void) misExport ((MixNamePtrType)"\53\6\1\2\1\12\41\5\1",
		(MixLengthType)9, &mxserInSigOps, (MixCookieType) 0);
	(void) misExport ((MixNamePtrType)"\53\6\1\2\1\12\41\6\1",
		(MixLengthType)9, &mxserOutSigOps, (MixCookieType) 0);
}
