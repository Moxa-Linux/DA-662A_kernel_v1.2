#include <stdio.h>
#include <ini.h>
#include <ini_debug.h>
#include <strext.h>
#include <ini_parser.h>


/** 
 @brief search relation between table name & application name function
 @param this ini object instant
 @param table_name specified table name
 @param app_names buffer to store the app names
 @param size size of the app_names
 @return error:1 success:0

 This method use to read the application name according to table key. 
 assign application name to instant app_name if found, and return 0.
 assign application name to NULL if not found, and return 1. 
 app_name instant contain all application names register with ini in string
 format with white space splitter. For the example "/usr/bin/apps.sh /bin/apps2.perl"
 */
int ini_findkey(ini_t *this, const char *section, char *key, char *result, size_t size)
{
	char tmp[1024] = { 0 };
	char *value;

	if (NULL == this || NULL == this->data || NULL == section
		|| NULL == key)
		return -1;

	/* format the key */
	snprintf(tmp, sizeof(tmp), "%s:%s", section, key);
	strtrim(tmp, " ");

	memset(result, 0, size);
	value = ini_get((ini_data_t*) this->data, tmp);
	if (value)
		strncpy(result, value, size);
	else
		return -1;

	return 0;
}

