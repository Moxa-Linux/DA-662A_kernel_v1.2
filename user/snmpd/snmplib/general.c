
/*
 * This program is general used for SNMPD. 
 * Created by Victor Yu. 07-30-2004
 */
#include                "ctypes.h"
#include                "local.h"
#include                "debug.h"
#include                "smp.h"
#include                "mix.h"
#include                "avl.h"
#include                "mis.h"
#include                "aps.h"
#include                "asn.h"

MixStatusType   GeneralRelease(MixCookieType cookie)
{
	cookie = cookie;
	return (smpErrorGeneric);
}

MixStatusType   GeneralCreate(MixCookieType cookie, MixNamePtrType name, MixLengthType namelen, AsnIdType asn)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	asn = asn;
	return (smpErrorGeneric);
}

MixStatusType   GeneralDestroy(MixCookieType cookie, MixNamePtrType name, MixLengthType namelen)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	return (smpErrorGeneric);
}

MixStatusType   GeneralSet(MixCookieType cookie, MixNamePtrType name, MixLengthType namelen, AsnIdType asn)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	asn = asn;
	SnmpMib.snmp_mibs.InTotalSetVars++;
	return (smpErrorReadOnly);
}

AsnIdType GeneralGet(MixCookieType cookie, MixNamePtrType name, MixLengthType namelen)
{
	cookie = cookie;
	name = name;
	namelen = namelen;
	return (smpErrorGeneric);
}

AsnIdType GeneralNext(MixCookieType cookie, MixNamePtrType name, MixLengthPtrType namelenp)
{
	cookie = cookie;
	name = name;
	namelenp = namelenp;
	return (smpErrorGeneric);
}
