#include <mdp.h>
#include "debug.h"


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
int mdp_prepare(struct __MDP__ *this, char *regex, int nmatch, int smatch)
{
	if (this->status || nmatch < 0 || nmatch > MAX_MDP_MATCH || smatch < 0 || smatch > MAX_MDP_SUB_MATCH)
		return 1;

	memset(&this->preg, 0, sizeof(regex_t));
	memset(&this->rules, 0, sizeof(char)*MAX_MDP_RULES);
	/* copy the regular expression rules into instant */
	strcpy(this->rules, regex);

	/* set matching result number */
	this->nmatch = nmatch;
	if (!smatch) smatch++;
	this->smatch = smatch;

	/* preprocessing */
	if( regcomp(&this->preg, this->rules, this->cflags) != 0 ) {
		fprintf(stderr, "regex preprocessing error!\n");
		this->status = MDP_PREPARE_ERR;
		return 1;
	}
	return 0;
}



/** 
 * @brief assign buffer to mdp object
 * @param this mdp object pointer
 * @param buffer pointer point to raw data
 * @param buffer length
 * @return success:>0, failure:1
 *
 * mx_mdp_stbuffer function used by other object to assign raw data.
 */
int mdp_setbuffer(struct __MDP__ *this, void *buffer, int length)
{
	/* status initial */
	this->status = 0;

	/* we don't want to make user buffer dirty
	 * instead, copy the data to our own buffer
	 */
	if (length > MAX_MDP_BUFFER) {
		fprintf(stderr, "mdp buffer limit to %d\n", MAX_MDP_BUFFER);
		this->status = MDP_SETBUFFER_ERR;
		return 1;
	}
	memset(&this->room, '\0', sizeof(char)*MAX_MDP_BUFFER);
	memcpy(&this->room, buffer, length);
	return 0;
}


/** 
 * @brief processing method mdp object
 * @param this mdp object pointer
 * @return success:>0, failure:1
 *
 * mx_mdp_processor function used by other object to process data.
 */
int mdp_processor(struct __MDP__ *this)
{
	static int rc = 0;
	int i, len, n = 0;
	char tmp[MAX_MDP_BUFFER];
	char buf[MAX_MDP_BUFFER];

	if (this->status)
		return 1;

	/* deep copy */
	memset(buf, '\0', sizeof(buf));
	strcpy(buf, this->room);

	memset(&this->result, 0, sizeof(char)*MAX_MDP_BUFFER*MAX_MDP_MATCH*MAX_MDP_SUB_MATCH);

	while (rc = regexec(&this->preg, buf, this->smatch, this->pmatch[n], 0) == 0 && n < this->nmatch) {
		/* make it "deep copy" style, so that
		 * we don't pollutted our result
		 */
		for (i = 0; i < this->smatch && this->pmatch[n][i].rm_so >= 0; ++i) {
			len = this->pmatch[n][i].rm_eo - this->pmatch[n][i].rm_so;
			strncpy(this->result[n][i], buf + this->pmatch[n][i].rm_so, len);
			this->result[n][i][len] = '\0';
		}

		/* shift string */
		memset(tmp, '\0', sizeof(tmp));
		strcpy(tmp, buf + this->pmatch[n][0].rm_eo);
		memset(buf, '\0', sizeof(buf));
		strcpy(buf, tmp);

		this->smatch_r[n] = i;
		n++;
	}
	this->nmatch_r = n;
	regfree(&this->preg);

	return 0;
}


/** 
 * @brief get regular express matching size
 * @param this mdp object pointer
 * @param match number
 * @return success:>0, failure:1
 *
 * mx_mdp_getnmatch function used by other object to get matching line.
 */
int mdp_getnmatch(struct __MDP__ *this, int *nnum) {
	if (this->status)
		return 1;

	if (!this->nmatch_r)
		this->status = MDP_NMATCH_ERR;

	*nnum = this->nmatch_r;
	return 0;
}

/** 
 * @brief copying from buffer according to the index
 * @param this mdp object pointer
 * @param buffer pointer point to mdp object data
 * @param index of buffer 
 * @return success:>0, failure:1
 *
 * mx_mdp_getnresult function used by other object retrieve regex result by index.
 */
int mdp_getnresult(struct __MDP__ *this, void **buffer, int nnum)
{
	if (nnum > this->nmatch_r)
		return 1;

	if (this->status)
		return 1;

	*buffer = this->result[nnum][0];
	return 0;
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
int mdp_getsmatch(struct __MDP__ *this, int *snum, int nnum) {
	if (this->status)
		return 1;

	if (!this->nmatch_r || !this->smatch_r)
		this->status = MDP_NMATCH_ERR;

	*snum = this->smatch_r[nnum];
	return 0;
}

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
int mdp_getsresult(struct __MDP__ *this, void **buffer, int nnum, int snum)
{
	if (nnum > this->nmatch_r)
		return 1;
	
	if (snum > this->smatch_r[nnum])
		return 1;

	if (this->status)
		return 1;

	*buffer = this->result[nnum][snum];
	return 0;
}
