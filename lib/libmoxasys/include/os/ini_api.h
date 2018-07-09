#ifndef __MXINI_H__
#define __MXINI_H__

#include <ini.h>


/**
 * @brief initialize the ini object
 * @param this ini object pointer
 * @param file ini file path
 * @return success:0
 *
 * ini_init function used by other object to initialize the memory and load
 * the ini file into prepared memory.
 */
void *ini_init(char *file);

/**
 * @brief free the ini contents (key/value/hash)
 * @param this ini object pointer
 * @return success:0
 *
 * ini_release function used by other object to release the memory initialized
 * by ini_init.
 */
void ini_free(void *this);

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
int ini_findkey(ini_t *this, const char *section, char *key, char *result, size_t size);         

#endif
