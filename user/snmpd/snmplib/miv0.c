#include	"ctypes.h"
#include	"local.h"
#include	"error.h"
#include	"miv.h"
#include	"mix.h"
#include	"mis.h"
#include	"asn.h"

#define		mivNonZero(asn)		\
			asnNonZero (asnValue ((asn)), asnLength ((asn)))

#define		mivNumber(asn)		\
			asnNumber (asnValue ((asn)), asnLength ((asn)))


#if 0	// mask by Victor Yu. 08-02-2004
static	MixStatusType	mivRelease (MixCookieType mix)
{
	mix = mix;
	return (smpErrorGeneric);
}


static	MixStatusType	mivNoSet (MixCookieType mix, MixNamePtrType name, MixLengthType namelen, AsnIdType value)
{
	mix = mix;
	name = name;
	namelen = namelen;
	value = value;
	return (smpErrorReadOnly);
}

static	MixStatusType	mivCreate (MixCookieType mix, MixNamePtrType name, MixLengthType namelen, AsnIdType value)
{
	mix = mix;
	name = name;
	namelen = namelen;
	value = value;
	return (smpErrorGeneric);
}

static	MixStatusType	mivDestroy (MixCookieType mix, MixNamePtrType name, MixLengthType namelen)
{
	mix = mix;
	name = name;
	namelen = namelen;
	return (smpErrorGeneric);
}
#endif

static	AsnIdType	mivGet (MixCookieType mix, MixNamePtrType name, MixLengthType namelen)
{
	if ((namelen != (MixLengthType) 1) ||
		(*name != (MixNameType) 0)) {
		return ((AsnIdType) 0);
	}
	else {
		return (asnIntl (asnClassUniversal, (AsnTagType) 2,
			*((CIntlPtrType) mix)));
	}
}

static	AsnIdType	mivNext (MixCookieType mix, MixNamePtrType name, MixLengthPtrType namelenp)
{
	if (*namelenp != (MixLengthType) 0) {
		return ((AsnIdType) 0);
	}
	else {
		*namelenp = (MixLengthType) 1;
		*name = (MixNameType) 0;
		return (asnIntl (asnClassUniversal, (AsnTagType) 2,
			*((CIntlPtrType) mix)));
	}
}

static	MixStatusType	mivSet (MixCookieType mix, MixNamePtrType name, MixLengthType namelen, AsnIdType asn)
{
	if ((namelen != (MixLengthType) 1) ||
		(*name != (MixNameType) 0)) {
		SnmpMib.snmp_mibs.InNoSuchNames++;	// add by Victor Yu. 08-02-2004
		return (smpErrorNoSuch);
	}
	else if (asnLength (asn) > (AsnLengthType) sizeof (CIntlType)) {
		SnmpMib.snmp_mibs.InBadValues++;	// add by Victor Yu. 08-02-2004
		SnmpMib.snmp_mibs.OutBadValues++;	// add by Victor Yu. 08-02-2004
		return (smpErrorBadValue);
	}
	else if (asnTag (asn) != (AsnTagType) 2) {
		SnmpMib.snmp_mibs.InBadValues++;	// add by Victor Yu. 08-02-2004
		SnmpMib.snmp_mibs.OutBadValues++;	// add by Victor Yu. 08-02-2004
		return (smpErrorBadValue);
	}
	else if (asnClass (asn) != asnClassUniversal) {
		SnmpMib.snmp_mibs.InBadValues++;	// add by Victor Yu. 08-02-2004
		SnmpMib.snmp_mibs.OutBadValues++;	// add by Victor Yu. 08-02-2004
		return (smpErrorBadValue);
	}
	else {
		*((CIntlPtrType) mix) = (CIntlType) mivNumber (asn);
		return (smpErrorNone);
	}
}

#if 0	// mask by Victor Yu. 08-02-2004
static	MixOpsType	mivOpsRW	= {
	mivRelease,
	mivCreate,
	mivDestroy,
	mivNext,
	mivGet,
	mivSet
};
#else	// add by Victor Yu. 08-02-2004
static	MixOpsType	mivOpsRW	= {
	GeneralRelease,
	GeneralCreate,
	GeneralDestroy,
	mivNext,
	mivGet,
	mivSet
};
#endif

MisStatusType		mivIntlRW (MixNamePtrType name, MixLengthType namelen, CIntlPtrType address)
{
	return (misExport (name, namelen, & mivOpsRW,
		(MixCookieType) address));
}

#if 0	// mask by Victor Yu. 08-02-2004
static	MixOpsType	mivOpsRO	= {
	mivRelease,
	mivCreate,
	mivDestroy,
	mivNext,
	mivGet,
	mivNoSet
};
#else	// add by Victor Yu. 08-02-2004
static	MixOpsType	mivOpsRO	= {
	GeneralRelease,
	GeneralCreate,
	GeneralDestroy,
	mivNext,
	mivGet,
	GeneralSet
};
#endif

MisStatusType		mivIntlRO (MixNamePtrType name, MixLengthType namelen, CIntlPtrType address)
{
	return (misExport (name, namelen, & mivOpsRO,
		(MixCookieType) address));
}

