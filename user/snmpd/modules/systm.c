
#include	<sys/param.h>
#include	<sys/types.h>
#include	<sys/time.h>
#include	<stdio.h>
#include	<netdb.h>
#include	<unistd.h>
#include 	<sys/file.h>

#include	"ctypes.h"
#include	"local.h"
#include	"debug.h"
#include	"miv.h"
#include	"asn.h"
#include	"mix.h"
#include	"systm.h"
#include	"oid.h"

#define			systmStringSize		(2 * MAXHOSTNAMELEN)

#if 0
#ifdef NeTtel
#define PRIVATENUM	6492
#else
#define PRIVATENUM	14358 /* SnapGear Pty Ltd */
#endif
#else
#define PRIVATENUM	8691	/* Moxa oid */
#endif

static	CCharType	systmDescrText[systmStringSize];		//*****Change to HOSTANME + version
static	CCharType	systmHostName[(MAXHOSTNAMELEN + 1)];
static	MivStrType	systmDescrStr;
static	CCharType	systmContText[systmStringSize];
static	MivStrType	systmContStr;
static	CCharType	systmNameText[systmStringSize];
static	MivStrType	systmNameStr;
static	CCharType	systmLocText[systmStringSize];
static	MivStrType	systmLocStr;

static	CIntsType	systmServInt;

static	CByteType	systmObjId [ systmStringSize ];
static	MivStrType	systmIdStr;

static	struct	timeval	systmBootTime;


#if defined(ARCH_UC8400) || defined(ARCH_UC8406)
// Added by Jared 11-04-2008
   #include "../../../lib/redboot_config/swid.h" // define the software IDs
#endif // if defined (ARCH_UC8400)

#if 0	// mask by Victor Yu. 07-30-2004
static	MixStatusType	systmTimeRelease (mix)

MixCookieType		mix;

{
	mix = mix;
	return (smpErrorGeneric);
}


static	MixStatusType	systmTimeNoSet (mix, name, namelen, value)

MixCookieType		mix;
MixNamePtrType		name;
MixLengthType		namelen;
AsnIdType		value;

{
	mix = mix;
	name = name;
	namelen = namelen;
	value = value;
	return (smpErrorReadOnly);
}

static	MixStatusType	systmTimeCreate (mix, name, namelen, value)

MixCookieType		mix;
MixNamePtrType		name;
MixLengthType		namelen;
AsnIdType		value;

{
	mix = mix;
	name = name;
	namelen = namelen;
	value = value;
	return (smpErrorGeneric);
}

static	MixStatusType	systmTimeDestroy (mix, name, namelen)

MixCookieType		mix;
MixNamePtrType		name;
MixLengthType		namelen;

{
	mix = mix;
	name = name;
	namelen = namelen;
	return (smpErrorGeneric);
}
#endif

static	AsnIdType	systmTimeGet (mix, name, namelen)

MixCookieType		mix;
MixNamePtrType		name;
MixLengthType		namelen;

{
	struct	timeval		now;
	CUnslType		uptime;

	mix = mix;
	if ((namelen != (MixLengthType) 1) ||
		(*name != (MixNameType) 0)) {
		return ((AsnIdType) 0);
	}
	else {
		(void) gettimeofday (& now, (struct timezone *) 0);
		uptime = (CUnslType) (now.tv_sec - systmBootTime.tv_sec) *
			(CUnslType) 100;
		return (asnUnsl (asnClassApplication, (AsnTagType) 3, uptime));
	}
}

static	AsnIdType	systmTimeNext (mix, name, namelenp)

MixCookieType		mix;
MixNamePtrType		name;
MixLengthPtrType	namelenp;

{
	struct	timeval		now;
	CUnslType		uptime;

	mix = mix;
	if (*namelenp != (MixLengthType) 0) {
		return ((AsnIdType) 0);
	}
	else {
		*namelenp = (MixLengthType) 1;
		*name = (MixNameType) 0;
		DEBUG2 ("systmTimeNext: boot %d %d\n",
			systmBootTime.tv_sec,
			systmBootTime.tv_usec);
		(void) gettimeofday (& now, (struct timezone *) 0);
		DEBUG2 ("systmTimeNext: now %d %d\n",
			now.tv_sec,
			now.tv_usec);
		uptime = (CUnslType) (now.tv_sec - systmBootTime.tv_sec) *
			(CUnslType) 100;
		DEBUG1 ("systmTimeNext: uptime %d\n", uptime);
		return (asnUnsl (asnClassApplication, (AsnTagType) 3, uptime));
	}
}

#if 0	// mask by Victor Yu. 07-30-2004
static	MixOpsType	systmTimeOps	= {
			systmTimeRelease,
			systmTimeCreate,
			systmTimeDestroy,
			systmTimeNext,
			systmTimeGet,
			systmTimeNoSet
};
#else
static	MixOpsType	systmTimeOps	= {
	GeneralRelease,
	GeneralCreate,
	GeneralDestroy,
	systmTimeNext,
	systmTimeGet,
	GeneralSet
};
#endif

CVoidType		systmInit ()
{
	long		hostid;
	unsigned char *ep;
	static int nr = 0;
	FILE *in;
 	char tmp [256];
	CIntfType	k;
	 
 if ((in = fopen ("/etc/version", "r")) == 0) {
   in = fopen ("/proc/version", "r");
 }
 if (in)
    {
	if (fgets (tmp, 256, in) > 0)
	  { 
	      tmp [strlen (tmp) - 1] = 0;
	      strcpy (systmDescrText, tmp);
	  }
	fclose (in);
    }
  else
    {
	strcpy (systmDescrText, "Unknown");
	printf ("snmpd: cannot open /etc/version or /proc/version.\n");
    }
	systmDescrStr.mivStrData = (CBytePtrType) systmDescrText;
	systmDescrStr.mivStrMaxLen = (CUnsfType) systmStringSize;
	systmDescrStr.mivStrLen = (CUnsfType) strlen (systmDescrText);

	(void) mivStringRO ((MixNamePtrType) "\53\6\1\2\1\1\1",
		(MixLengthType) 7, & systmDescrStr);

	/* REVISIT: Do we have a different MIB for each device type?
                    If so, this is probably not the way to find out */
	// 12 is for Moxa Systems Embedded Linux platform
	// 13 is for Moxa Systems Embedded WinCE platform, 03-08-2006
        hostid = 12;
#ifdef NEVER
	ep = (unsigned char *) (0xf0006000);
	if ((ep[0] == 0xff) && (ep[1] == 0xff) && (ep[2] == 0xff) &&
	    (ep[3] == 0xff) && (ep[4] == 0xff) && (ep[5] == 0xff))
		hostid = 0x00;
	else
		//hostid = (ep[2]<<24)+(ep[3]<<16)+(ep[4]<<8)+ep[5];
		hostid = (ep[4]<<8)+ep[5];
	DEBUG1 ("systmInit: Hostid %d\n", hostid);
#endif
#if defined(ARCH_IA241) 
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.241", PRIVATENUM, hostid);
#elif defined(ARCH_IA240)
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.240", PRIVATENUM, hostid);
#elif defined(ARCH_IA241_32128)
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.1241", PRIVATENUM, hostid);
#elif defined(ARCH_IA260)
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.260", PRIVATENUM, hostid);
#elif defined(ARCH_IA261)
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.261", PRIVATENUM, hostid);
#elif defined(ARCH_IA261_256)
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.1261", PRIVATENUM, hostid);
#elif defined(ARCH_IA262)
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.262", PRIVATENUM, hostid);
#elif  defined(ARCH_EM2260)
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.2260", PRIVATENUM, hostid);
#elif  defined(ARCH_W406)
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.406", PRIVATENUM, hostid);
#elif  defined(ARCH_W416)
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.416", PRIVATENUM, hostid);
#elif  defined(ARCH_SMG1100)
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.1100", PRIVATENUM, hostid);
#elif ( defined ( ARCH_W341 )) 
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.341", PRIVATENUM, hostid);
#elif ( defined ( ARCH_W345 )) 
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.345", PRIVATENUM, hostid);
#elif ( defined ARCH_UC_7112_LX_PLUS )
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.17112", PRIVATENUM, hostid);
#elif ( defined ARCH_W311 )
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.311", PRIVATENUM, hostid);
#elif ( defined ARCH_W321 )
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.321", PRIVATENUM, hostid);
#elif defined ARCH_W315
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.315", PRIVATENUM, hostid);
#elif defined ARCH_W315_NJKL
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.315", PRIVATENUM, hostid);
#elif ( defined ARCH_W325 )
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.325", PRIVATENUM, hostid);
#elif ( defined ARCH_RNAS1200 )
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.1200", PRIVATENUM, hostid);
#elif ( defined ARCH_UC8400 )
	read_procfs("/proc/bus/pci/devices", proc_buf);
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.%x", PRIVATENUM, hostid, get_swid());
#elif ( defined ARCH_UC8000 )
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.8000", PRIVATENUM, hostid);
#elif defined(ARCH_UC8406 )
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.8406", PRIVATENUM, hostid);
#elif defined(ARCH_DA662A )
	(void) sprintf (tmp, "1.3.6.1.4.1.%u.%lu.6622", PRIVATENUM, hostid);
#endif
	k = oidEncode (systmObjId, (CIntfType) systmStringSize, tmp);
	if (k > (CIntfType) 0) {
		systmIdStr.mivStrData = systmObjId;
		systmIdStr.mivStrMaxLen = (CUnsfType) systmStringSize;
		systmIdStr.mivStrLen = (CUnsfType) k;

		(void) mivObjectIdRO ((MixNamePtrType) "\53\6\1\2\1\1\2",
			(MixLengthType) 7, & systmIdStr);
	}
	
	(void) gettimeofday (&systmBootTime, (struct timezone *) 0);	
	(void) misExport ((MixNamePtrType) "\53\6\1\2\1\1\3",
		(MixLengthType) 7, & systmTimeOps, (MixCookieType) 0);

	/**Contact**/
	//strcpy(systmContText, "Unknown"); // mask by Victor Yu. 07-26-2004
#if 0 // 10-17-2008. Jared. Company address
	strcpy(systmContText, "Moxa systems Co., LDT."); // add by Victor Yu. 07-26-2004
#else
	strcpy(systmContText, "Moxa Inc., Embedded Computing Business.");
#endif
	systmContStr.mivStrData = (CBytePtrType) systmContText;
	systmContStr.mivStrMaxLen = (CUnsfType) systmStringSize;
	systmContStr.mivStrLen = (CUnsfType) strlen (systmContText);
	(void) mivStringRW ((MixNamePtrType) "\53\6\1\2\1\1\4",
		(MixLengthType) 7, & systmContStr);
	/**End Contact**/

	/**Name**/
	if (gethostname((char *)systmHostName,(int)MAXHOSTNAMELEN) < 0) 
		{
		(void)sprintf(systmDescrText,"Unknown Hostname");
		}
	else 
		{
		systmHostName[MAXHOSTNAMELEN]=(CCharType)0;
		(void)sprintf(systmNameText, "%s",systmHostName);
		}
	systmNameStr.mivStrData = (CBytePtrType) systmNameText;
	systmNameStr.mivStrMaxLen = (CUnsfType) systmStringSize;
	systmNameStr.mivStrLen = (CUnsfType) strlen (systmNameText);
	(void) mivStringRW ((MixNamePtrType) "\53\6\1\2\1\1\5",
		(MixLengthType) 7, &systmNameStr);
	/**End Name**/

	/**Location**/
#if 0 // 10-17-2008. Jared. Company address
	strcpy(systmLocText, "Unknown");
#else
	strcpy(systmLocText, "Fl.8 No.6, Alley 6, Lane 235, Pao-Chiao Rd., Shing Tien City, Taipei, Taiwan, R.O.C.");
#endif
	systmLocStr.mivStrData = (CBytePtrType) systmLocText;
	systmLocStr.mivStrMaxLen = (CUnsfType) systmStringSize;
	systmLocStr.mivStrLen = (CUnsfType) strlen (systmLocText);
	(void) mivStringRW ((MixNamePtrType) "\53\6\1\2\1\1\6",
		(MixLengthType) 7, &systmLocStr);
	/**End Location**/

	/**Services**/
	systmServInt=(CIntsType)6;
	(void) mivIntlRO ((MixNamePtrType) "\53\6\1\2\1\1\7",
		(MixLengthType) 7, (CIntlPtrType)&systmServInt);
	/**End Services**/

}
