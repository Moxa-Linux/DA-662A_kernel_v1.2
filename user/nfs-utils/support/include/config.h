/* support/include/config.h.  Generated automatically by configure.  */
/* Define this if you have standard C headers
 */
/* #undef  STDC_HEADERS */

/* Define this if you have string.h */
/* #undef  HAVE_STRING_H */

/* Define this if you have netgroup support
 */
#define  HAVE_INNETGR 1

/* Define this if you want NFSv3 support compiled in
 */
#define  NFS3_SUPPORTED 1

/* This defines the location of the NFS state files
 * Warning: these must match definitions in config.mk!
 */
#define NFS_STATEDIR		"/var/lib/nfs"

/* Define this if you want to enable various security
 * checks in statd. These checks basically keep anyone
 * but lockd from using this service.
 */
/* #undef RESTRICTED_STATD */
