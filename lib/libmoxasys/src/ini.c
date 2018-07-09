#include <stdlib.h>
#include <string.h>
#include "ini.h"
#include "os/ini_api.h"


/** 
 * @brief ini object construction
 * @param ini file path
 * @return error:NULL success:pointer 
 *
 * mx_ini_init function used by other object to initialize the memory
 * and prepare internal ini file descriptor. 
 */
ini_t *mx_ini_init(char *file)
{
	ini_t *this;


	this = (ini_t *)malloc(sizeof(ini_t));
	if (!this)
		return NULL;
	memset(this, 0x0, sizeof(ini_t));

	/* init instant */
	this->data = ini_init(file);
	if (NULL == this->data)
	{
		mx_ini_release(this);
		return NULL;
	}
	this->ini_findkey = ini_findkey;

	return this;
}


/** 
 * @brief ini object destruction
 * @param void
 * @return success:0
 *
 * mx_ini_release function used by other object to release the memory
 * initialized by construction.
 */
void mx_ini_release(ini_t *this)
{
	/* close ini file descriptor before free memory */
	if (this)
	{
		ini_free(this->data);
		free(this);
	}
	return;
}

/** 
 * @brief find value with a specified table name from ini object
 * @param this ini object pointer
 * @param tb_name the specified table name
 * @param app_names buffer to record the application names
 * @param size size of the buffer
 * @return success:0
 *
 * mx_ini_findapp function used by other object to get the application
 * names in a specified table.
 */
__inline int mx_ini_find_key(struct __INI__ *this, const char *section,
                char *key, char *result,  size_t size) {
	return this->ini_findkey(this, section, key, result, size);
};
