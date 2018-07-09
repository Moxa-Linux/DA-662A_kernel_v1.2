/*! \file ini.h
 *     \brief A header file for moxa ini parser to get specified values.
 *         
 *             Details.
 */
#ifndef _INI_H
#define _INI_H


#define INI_OBJ \
	/* private instant */ \
	void *data;         /** contents of an ini */ \
\
	/* public instant */ \
\
	/* public method */ \
	int (*ini_findkey)      \
	(struct __INI__ *, const char *, char *, char *, size_t); \
;

typedef struct __INI__
{

	INI_OBJ;

}ini_t ;


/** 
 * @brief ini object construction
 * @param ini file path
 * @return error:NULL success:pointer 
 *
 * mx_ini_init function used by other object to initialize the memory
 * and prepare internal ini file descriptor. 
 */
ini_t *mx_ini_init(char *file);              /**< constructor */

/** 
 * @brief ini object destruction
 * @param this ini object pointer
 * @return success:0
 *
 * mx_ini_release function used by other object to release the memory
 * initialized by construction.
 */
void  mx_ini_release(struct __INI__ *this); /**< destructor */

/**
 * @brief find ttl value with a specified table name from ini object
 * @param this ini object pointer
 * @param section name in ini file
 * @param key name in ini file
 * @param return data in result
 * @return success:0
 *
 * mx_ini_findttl function used by other object to get the ttl value
 * in a specified table.
 */
__inline int mx_ini_find_key(struct __INI__ *this, const char *section,
                char *key, char *result, size_t size);
#endif
