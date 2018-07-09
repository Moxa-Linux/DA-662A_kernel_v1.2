/*! \file ini_parser.h
 *     \brief A header file for ini parser.
 *         
 *             Details.
 */
#ifndef __INI_PARSER_H__
#define __INI_PARSER_H__


#define INI_ERROR_NULL_POINTER      "null pointer"
#define INI_ERROR_OUT_OF_MEMORY     "memory not enough"
#define INI_ERROR_OPEN_FAILED       "file open failed: %s"
#define INI_ERROR_INVALID_SYNTAX    "syntax invalid: %s"



#define INI_DATA \
	/* private instant */ \
	int num;            /** number of keys */ \
	int size;           /** storage size */ \
	char **key;         /** list of string keys */ \
	char **val;         /** list of string values */ \
	unsigned int *hash; /** list of hash values for keys */ \
\
	/* public instant */ \
\
	/* public method */

typedef struct __INI_DATA__
{

	INI_DATA;

}ini_data_t;


/**
 * @brief initialize the ini object
 * @param this ini object pointer
 * @param file ini file path
 * @return success:0
 *
 * ini_init function used by other object to initialize the memory and load
 * the ini file into prepared memory.
 */
ini_data_t *ini_init(char *file);

/**
 * @brief free the ini contents (key/value/hash)
 * @param this ini object pointer
 * @return success:0
 *
 * ini_release function used by other object to release the memory initialized
 * by ini_init.
 */
void ini_free(ini_data_t *this);

/**
 * @brief get a key value
 * @param key the specified key name
 * @return success:0
 *
 * ini_get function is used to get a key value from an ini object.
 * The format of the key should be 
 *	"<section>:<key>".
 */
char * ini_get(ini_data_t *this, char *key);

/**
 * @brief add or update a key value
 * @param this ini object pointer
 * @param key key name
 * @param value the key value
 * @return success:0
 *
 * ini_set function is used to add or update an ini key value in a ini object.
 * The format of the key should be 
 *	"<section>:<key>".
 */
int ini_set(ini_data_t *this, char *key, char *value);

#endif
