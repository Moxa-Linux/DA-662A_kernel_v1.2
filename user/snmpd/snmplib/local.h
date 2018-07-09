#ifndef		_LOCAL_H_
#define		_LOCAL_H_

#include <string.h>
#include <stdlib.h>

// follwing add by Victor Yu. 07-30-2004
#include "ctypes.h"
#include "mix.h"

extern MixStatusType   GeneralRelease (MixCookieType cookie);
extern MixStatusType   GeneralCreate (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen, AsnIdType asn);
extern MixStatusType   GeneralDestroy (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen);
extern MixStatusType   GeneralSet (MixCookieType cookie, MixNamePtrType name, MixLengthType namelen, AsnIdType asn);
extern AsnIdType GeneralGet(MixCookieType cookie, MixNamePtrType name, MixLengthType namelen);
extern AsnIdType GeneralNext(MixCookieType cookie, MixNamePtrType name, MixLengthPtrType namelenp);
// above add by Victor Yu. 07-30-2004

#endif		/*	_LOCAL_H_	*/
