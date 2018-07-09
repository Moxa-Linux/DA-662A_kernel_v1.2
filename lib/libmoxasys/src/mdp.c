#include <stdlib.h>
#include <string.h>
#include "mdp.h"
#include "os/mdp_api.h"

/** 
 * @brief mdp object construction
 * @param void
 * @return error:NULL success:pointer 
 *
 * mx_mdp_init function used by other object to mdp initialize the memory
 * and prepare internal mdp file descriptor. 
 */
mdp_t *mx_mdp_init(void)              /**< constructor */
{
	mdp_t *this;

	this = (mdp_t *)malloc(sizeof(mdp_t));
	if (!this)
		return NULL;
	memset(this, 0x0, sizeof(mdp_t));

	/* init instant */
	this->status = 0;
	this->cflags = REG_EXTENDED | REG_ICASE;
	this->nmatch = 0;
	this->nmatch_r = 0;
	this->smatch = 0;
	memset(this->smatch_r, 0, sizeof(size_t)*MAX_MDP_MATCH);
	memset(&this->pmatch, 0, sizeof(regmatch_t)*MAX_MDP_MATCH*MAX_MDP_SUB_MATCH);
	memset(&this->preg, 0, sizeof(regex_t));
	memset(&this->room, 0, sizeof(char)*MAX_MDP_BUFFER);
	memset(&this->rules, 0, sizeof(char)*MAX_MDP_RULES);
	memset(&this->result, 0, sizeof(char)*MAX_MDP_MATCH*MAX_MDP_SUB_MATCH*MAX_MDP_BUFFER);

	/* method assign */
	this->mdp_prepare	= mdp_prepare;
	this->mdp_setbuffer	= mdp_setbuffer;
	this->mdp_processor	= mdp_processor;
	this->mdp_getnmatch	= mdp_getnmatch;
	this->mdp_getnresult= mdp_getnresult;
	this->mdp_getsmatch	= mdp_getsmatch;
	this->mdp_getsresult= mdp_getsresult;

	return this;
}


/** 
 * @brief mdp object destruction
 * @param this mdp object pointer
 * @return void
 *
 * mx_mdp_release function used by other object to release the memory
 * initialized by construction.
 */
void  mx_mdp_release(mdp_t *this)
{
	if(this)
		free(this);
}

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
__inline int mx_mdp_prepare(mdp_t *this, char *regex, int nmatch, int smatch) {
	return this->mdp_prepare(this, regex, nmatch, smatch);
}

/** 
 * @brief assign buffer to mdp object
 * @param this mdp object pointer
 * @param buffer pointer point to raw data
 * @param buffer length
 * @return success:>0, failure:1
 *
 * mx_mdp_setbuffer function used by other object to assign raw data.
 */
__inline int mx_mdp_setbuffer(mdp_t *this, void *buffer, int length) {
	return this->mdp_setbuffer(this, buffer, length);
}

/** 
 * @brief processing method mdp object
 * @param this mdp object pointer
 * @return success:>0, failure:1
 *
 * mx_mdp_processor function used by other object to process data.
 */
__inline int mx_mdp_processor(mdp_t *this) {
	return this->mdp_processor(this);
}


/** 
 * @brief get regular express matching size
 * @param this mdp object pointer
 * @param match number
 * @return success:>0, failure:1
 *
 * mx_mdp_getnmatch function used by other object to get matching line.
 */
__inline int mx_mdp_getnmatch(mdp_t *this, int *nnum) {
	return this->mdp_getnmatch(this, nnum);
}


/** 
 * @brief copying from buffer according to the index
 * @param this mdp object pointer
 * @param buffer pointer point to mdp object data
 * @param index of buffer 
 * @return success:>0, failure:1
 *
 * mdp_getnresult function used by other object retrieve regex result by index.
 */
__inline int mx_mdp_getnresult(mdp_t *this, void **buffer, int nnum) {
	return this->mdp_getnresult(this, buffer, nnum);
}

/** 
 * @brief get regular express matching size
 * @param this mdp object pointer
 * @param match number
 * @param sub match number
 * @return success:>0, failure:1
 *
 * mx_mdp_getsmatch function used by other object to get matching line.
 */
__inline int mx_mdp_getsmatch(mdp_t *this, int *snum, int nnum) {
	return this->mdp_getsmatch(this, snum, nnum);
}


/** 
 * @brief copying from buffer according to the index
 * @param this mdp object pointer
 * @param buffer pointer point to mdp object data
 * @param index of buffer 
 * @param sub index of buffer 
 * @return success:>0, failure:1
 *
 * mdp_getsresult function used by other object retrieve regex result by index.
 */
__inline int mx_mdp_getsresult(mdp_t *this, void **buffer, int nnum, int snum) {
	return this->mdp_getsresult(this, buffer, nnum, snum);
}
