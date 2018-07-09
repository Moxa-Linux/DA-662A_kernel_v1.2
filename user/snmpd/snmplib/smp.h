#ifndef		_SMP_H_
#define		_SMP_H_

#include	"ctypes.h"
#include	"error.h"
#include	"aps.h"

typedef		CUnswType		SmpIdType;

typedef		CUnswType		SmpSocketType;

typedef		ErrStatusType		SmpStatusType;

typedef		enum			SmpErrorTag {

		smpErrorNone,
		smpErrorTooBig,
		smpErrorNoSuch,
		smpErrorBadValue,
		smpErrorReadOnly,
		smpErrorGeneric

		}			SmpErrorType;

typedef		enum			SmpCommandTag {

		smpCommandGet,
		smpCommandNext,
		smpCommandRsp,
		smpCommandSet,
		smpCommandTrap

		}			SmpCommandType;

typedef		enum			SmpKindTag {

		smpKindNone,
		smpKindInteger,
		smpKindOctetString,
		smpKindIPAddr,
		smpKindOpaque,
		smpKindCounter,
		smpKindGuage,
		smpKindTimeTicks,
		smpKindObjectId,
		smpKindNull

		}			SmpKindType;


typedef		enum			SmpTrapTag {

		smpTrapColdStart,
		smpTrapWarmStart,
		smpTrapLinkDown,
		smpTrapLinkUp,
		smpTrapAuthenticationFailure,
		smpTrapEgpNeighborLoss,
		smpTrapEnterpriseSpecific

		}			SmpTrapType;

typedef		CIntlType		SmpSequenceType;

typedef		CUnssType		SmpIndexType;

typedef		CUnsfType		SmpLengthType;

typedef		CBytePtrType		SmpNameType;

typedef		CBytePtrType		SmpValueType;

typedef		CUnslType		SmpNumberType;

typedef		struct			SmpBindTag {

		SmpLengthType		smpBindNameLen;
		SmpNameType		smpBindName;
		SmpKindType		smpBindKind;
		SmpLengthType		smpBindValueLen;
		SmpValueType		smpBindValue;
		SmpNumberType		smpBindNumber;

		}			SmpBindType;

typedef		SmpBindType		*SmpBindPtrType;

typedef		struct			SmpRequestTag {

		SmpCommandType		smpRequestCmd;
		ApsIdType		smpRequestCommunity;
		SmpSequenceType		smpRequestId;
		SmpErrorType		smpRequestError;
		SmpIndexType		smpRequestIndex;
		SmpLengthType		smpRequestEnterpriseLen;
		SmpNameType		smpRequestEnterprise;
		SmpLengthType		smpRequestAgentLen;
		SmpValueType		smpRequestAgent;
		SmpTrapType		smpRequestGenericTrap;
		SmpNumberType		smpRequestSpecificTrap;
		SmpNumberType		smpRequestTimeStamp;
		SmpIndexType		smpRequestCount;
		SmpBindPtrType		smpRequestBinds;

		}			SmpRequestType;

typedef		SmpRequestType		*SmpRequestPtrType;

typedef		SmpStatusType		(*SmpHandlerType) (SmpIdType smp, SmpRequestPtrType req);

typedef		SmpStatusType		(*SmpSendFnType) (SmpSocketType udp, CBytePtrType bp, CIntfType n);

SmpIdType		smpNew (SmpSocketType peer, SmpSendFnType sendFn, SmpHandlerType upcall);
SmpIdType		smpFree (SmpIdType smp);

CVoidType		smpInit (void);
SmpStatusType		smpInput (SmpIdType smp, CByteType x);
SmpStatusType		smpRequest (SmpIdType smp, SmpRequestPtrType req);

// following add by Victor Yu. 08-02-2004
#define SNMP_IN_PKTS			1
#define SNMP_OUT_PKTS			2
#define SNMP_IN_BAD_VERSIONS		3
#define SNMP_IN_BAD_COMMUNITY_NAMES	4
#define SNMP_IN_BAD_COMMUNITY_USES	5
#define SNMP_IN_ASN_PARSE_ERRS		6
#define SNMP_ITEM7			7	// not used
#define SNMP_IN_TOO_BIGS		8
#define SNMP_IN_NO_SUCH_NAMES		9
#define SNMP_IN_BAD_VALUES		10
#define SNMP_IN_READ_ONLYS		11
#define SNMP_IN_GEN_ERRS		12
#define SNMP_IN_TOTAL_REQ_VARS		13
#define SNMP_IN_TOTAL_SET_VARS		14
#define SNMP_IN_GET_REQUESTS		15
#define SNMP_IN_GET_NEXTS		16
#define SNMP_IN_SET_REQUESTS		17
#define SNMP_IN_GET_RESPONSES		18
#define SNMP_IN_TRAPS			19
#define SNMP_OUT_TOO_BIGS		20
#define SNMP_OUT_NO_SUCH_NAMES		21
#define SNMP_OUT_BAD_VALUES		22
#define SNMP_ITEM23			23	// not used
#define SNMP_OUT_GEN_ERRS		24
#define SNMP_OUT_GET_REQUESTS		25
#define SNMP_OUT_GET_NEXTS		26
#define SNMP_OUT_SET_REQUESTS		27
#define SNMP_OUT_GET_RESPONSES		28
#define SNMP_OUT_TRAPS			29
#define SNMP_ENABLE_AUTHEN_TRAPS	30
#define SNMP_MAX_ITEM			30
struct snmp_mib_struct {
	unsigned long	InPkts;			// 1, Counter
	unsigned long	OutPkts;		// 2, Counter
	unsigned long	InBadVersions;		// 3, Counter
	unsigned long	InBadCommunityNames;	// 4, Counter
	unsigned long	InBadCommunityUses;	// 5, Counter
	unsigned long	InASNParseErrs;		// 6, Counter
	unsigned long	Item7;			// 7, not used
	unsigned long	InTooBigs;		// 8, Counter
	unsigned long	InNoSuchNames;		// 9, Counter
	unsigned long	InBadValues;		// 10, Counter
	unsigned long	InReadOnlys;		// 11, Counter
	unsigned long	InGenErrs;		// 12, Counter
	unsigned long	InTotalReqVars;		// 13, Counter
	unsigned long	InTotalSetVars;		// 14, Counter
	unsigned long	InGetRequests;		// 15, Counter
	unsigned long	InGetNexts;		// 16, Counter
	unsigned long	InSetRequests;		// 17, Counter
	unsigned long	InGetResponses;		// 18, Counter
	unsigned long	InTraps;		// 19, Counter
	unsigned long	OutTooBigs;		// 20, Counter
	unsigned long	OutNoSuchNames;		// 21, Counter
	unsigned long	OutBadValues;		// 22, Counter
	unsigned long	Item23;			// 23, not used
	unsigned long	OutGenErrs;		// 24, Counter
	unsigned long	OutGetRequests;		// 25, Counter
	unsigned long	OutGetNexts;		// 26, Counter
	unsigned long	OutSetRequests;		// 27, Counter
	unsigned long	OutGetResponses;	// 28, Counter
	unsigned long	OutTraps;		// 29, Counter
	unsigned long	EnableAuthenTraps;	// 30, Integer, 1 - enable, 2 - disable
};
union snmp_mib_union {
	struct snmp_mib_struct	snmp_mibs;
	unsigned long		snmp_mibl[sizeof(struct snmp_mib_struct)];
};
extern union snmp_mib_union	SnmpMib;
// above add by Victor Yu. 08-02-2004

#endif		/*	_SMP_H_	*/
