/*! \file mdp.h
 *     \brief A header file for moxa meter data parser declare.
 *         
 *             Details.
 */
#ifndef _MDP_H
#define _MDP_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <regex.h>

#define	MAX_MDP_BUFFER		(4096)
#define	MAX_MDP_RULES		(1024)
#define MAX_MDP_MATCH		(20)
#define MAX_MDP_SUB_MATCH	(10)
#define MDP_SETBUFFER_ERR	1
#define MDP_PREPARE_ERR		2
#define MDP_PROCESSOR_ERR	3
#define MDP_NMATCH_ERR		4

#define MDP_OBJ \
	/* private instant */ \
	int status;     	    				/** status of current mdp object */ \
	char room[MAX_MDP_BUFFER];       		/** data buffer */ \
	char rules[MAX_MDP_RULES];       		/** regular expression rules */ \
	char result[MAX_MDP_MATCH][MAX_MDP_SUB_MATCH][MAX_MDP_BUFFER];  /** data buffer after regex */ \
	regex_t preg;			       			/** pointer to a pattern buffer storage area */ \
	regmatch_t pmatch[MAX_MDP_MATCH][MAX_MDP_SUB_MATCH];      		/** pattern parsing data buffer array */ \
	size_t nmatch;			       			/** desire number of pattern match */ \
	size_t nmatch_r;			       		/** return number of pattern match */ \
	size_t smatch;			       			/** desire number of sub pattern match */ \
	size_t smatch_r[MAX_MDP_MATCH];			/** return number of sub pattern match */ \
	int cflags;			       				/** regex flag */ \
\
	/* public instant */ \
\
	/* public method */ \
	int (*mdp_prepare)	\
		(struct __MDP__ *, char *regex, int nmatch, int smatch); \
	int (*mdp_setbuffer)	\
		(struct __MDP__ *, void *buffer, int length); \
	int (*mdp_getnmatch)	\
		(struct __MDP__ *, int *nnum); \
	int (*mdp_getnresult)	\
		(struct __MDP__ *, void **buffer, int nnum); \
	int (*mdp_getsmatch)	\
		(struct __MDP__ *, int *snum, int nnum); \
	int (*mdp_getsresult)	\
		(struct __MDP__ *, void **buffer, int nnum, int snum); \
	int (*mdp_processor)	\
		(struct __MDP__ *);

typedef struct __MDP__
{

	MDP_OBJ;

}mdp_t ;

/** 
 * @brief mdp object construction
 * @param void
 * @return error:NULL success:pointer 
 *
 * mx_mdp_init function used by other object to initialize the memory
 * and prepare internal mdp file descriptor. 
 */
mdp_t *mx_mdp_init(void);              /**< constructor */

/** 
 * @brief mdp object destruction
 * @param this mdp object pointer
 * @return void
 *
 * mx_mdp_release function used by other object to release the memory
 * initialized by construction.
 */
void  mx_mdp_release(struct __MDP__ *this); /**< destructor */


/** 
 * @brief prepare regex parsing method for mdp object
 * @param this mdp object pointer
 * @param regular expression rule
 * @param max number of matching result
 * @param max number of sub matching result
 * @return success:>0, failure:-1
 *
 * mx_mdp_prepare function used by other object to describe regular express rules.
 */
__inline int mx_mdp_prepare(struct __MDP__ *this, char *regex, int nmatch, int smatch);


/** 
 * @brief assign buffer to mdp object
 * @param this mdp object pointer
 * @param buffer pointer point to raw data
 * @param buffer length
 * @return success:>0, failure:1
 *
 * mx_mdp_stbuffer function used by other object to assign raw data.
 */
__inline int mx_mdp_setbuffer(struct __MDP__ *this, void *buffer, int length);


/** 
 * @brief processing method mdp object
 * @param this mdp object pointer
 * @return success:>0, failure:1
 *
 * mx_mdp_processor function used by other object to process data.
 */
__inline int mx_mdp_processor(struct __MDP__ *this);


/** 
 * @brief get regular express matching size
 * @param this mdp object pointer
 * @param match number
 * @return success:>0, failure:1
 *
 * mx_mdp_getnmatch function used by other object to get matching line.
 */
__inline int mx_mdp_getnmatch(struct __MDP__ *this, int *nnum);


/** 
 * @brief copying from buffer according to the index
 * @param this mdp object pointer
 * @param buffer pointer point to mdp object data
 * @param index of buffer 
 * @return success:>0, failure:1
 *
 * mx_mdp_getnresult function used by other object retrieve regex result by index.
 */
__inline int mx_mdp_getnresult(struct __MDP__ *this, void **buffer, int nnum);


/** 
 * @brief get regular express matching size
 * @param this mdp object pointer
 * @param match number
 * @param sub match number
 * @return success:>0, failure:1
 *
 * mx_mdp_getsmatch function used by other object to get sub matching line.
 */
__inline int mx_mdp_getsmatch(struct __MDP__ *this, int *snum, int nnum);


/** 
 * @brief copying from buffer according to the index
 * @param this mdp object pointer
 * @param buffer pointer point to mdp object data
 * @param index of buffer 
 * @param sub index of buffer 
 * @return success:>0, failure:1
 *
 * mx_mdp_getsresult function used by other object retrieve regex result by index.
 */
__inline int mx_mdp_getsresult(struct __MDP__ *this, void **buffer, int nnum, int snum);


#endif
