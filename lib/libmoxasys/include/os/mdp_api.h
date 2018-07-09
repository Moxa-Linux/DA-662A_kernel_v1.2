#ifndef OS_HEADER_MDP 
#define OS_HEADER_MDP 

#include <mdp.h>

/** 
 * @brief mdp object construction
 * @param void
 * @return error:NULL success:pointer 
 *
 * mx_mdp_init function used by other object to initialize the memory
 * and prepare internal mdp file descriptor. 
 */
mdp_t *mdp_init(void);              /**< constructor */

/** 
 * @brief mdp object destruction
 * @param this mdp object pointer
 * @return void
 *
 * mx_mdp_release function used by other object to release the memory
 * initialized by construction.
 */
void  mdp_release(struct __MDP__ *this); /**< destructor */

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
int mdp_prepare(mdp_t *this, char *regex, int nmatch, int smatch);


/** 
 * @brief assign buffer to mdp object
 * @param this mdp object pointer
 * @param buffer pointer point to raw data
 * @param buffer length
 * @return success:>0, failure:1
 *
 * mx_mdp_stbuffer function used by other object to assign raw data.
 */
int mdp_setbuffer(mdp_t *this, void *buffer, int length);


/** 
 * @brief processing method mdp object
 * @param this mdp object pointer
 * @return success:>0, failure:1
 *
 * mx_mdp_processor function used by other object to process data.
 */
int mdp_processor(mdp_t *this);


/** 
 * @brief get regular express matching size
 * @param this mdp object pointer
 * @param match number
 * @return success:>0, failure:1
 *
 * mx_mdp_getnmatch function used by other object to get matching line.
 */
int mdp_getnmatch(mdp_t *this, int *nnum);


/** 
 * @brief copying from buffer according to the index
 * @param this mdp object pointer
 * @param buffer pointer point to mdp object data
 * @param index of buffer 
 * @return success:>0, failure:1
 *
 * mx_mdp_getnresult function used by other object retrieve regex result by index.
 */
int mdp_getnresult(mdp_t *this, void **buffer, int nnum);


/** 
 * @brief get regular express matching size
 * @param this mdp object pointer
 * @param match number
 * @param sub match number
 * @return success:>0, failure:1
 *
 * mx_mdp_getsmatch function used by other object to get sub matching line.
 */
int mdp_getsmatch(mdp_t *this, int *snum, int nnum);


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
int mdp_getsresult(mdp_t *this, void **buffer, int nnum, int snum);

#endif
