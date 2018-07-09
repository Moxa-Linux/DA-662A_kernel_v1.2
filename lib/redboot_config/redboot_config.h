//#define CONFIG_JFFS2_V2_1     //mask by Jared Wu. 07-19-2005.  For JFFS2 v.2.1(firmware v1.5.2.4 before ), please define this MACRO

#ifndef _REDBOOT_CONFIG_H
#define _REDBOOT_CONFIG_H

//#define CONFIG_JFFS2_V2_1     // mask by Jared Wu. 07-19-2005.  
                                // For JFFS2 v.2.1, please define this MACRO

#ifdef DEBUG
#define dbg_printf(x...)        printf(x)
#else
#define dbg_printf(x...)
#endif

#define CONFIG_EMPTY   0
#define CONFIG_BOOL    1
#define CONFIG_INT     2
#define CONFIG_STRING  3
#define CONFIG_SCRIPT  4
#define CONFIG_IP      5
#define CONFIG_ESA     6
#define CONFIG_NETPORT 7

#define MAX_SWLOCK_LENGTH 8

/*
 *  Descriptions: Open the redboot partition and read all the data 
 *  from flash to memory buffer
 *  Parameters:
 *    none
 *  return:
 *    0: successful
 *    -1: fail
 */
int     config_init(void);

/*  Descriptions: Flush the redboot partition from memory buffer
 *    to flash.
 *  Parameters:
 *    none
 *  return:
 *    0: successful
 *    -1: fail
 */
int     config_write(void);

/*  Descriptions: Return the data type of the specific item
 *  Parameters:
 *    char *nkname: the specific item in the redboot partition
 *  return:
 *    fail: -1
 *    else:
 *      CONFIG_EMPTY   0
 *      CONFIG_BOOL    1
 *      CONFIG_INT     2
 *      CONFIG_STRING  3
 *      CONFIG_SCRIPT  4
 *      CONFIG_IP      5
 *      CONFIG_ESA     6
 *      CONFIG_NETPORT 7
 */
int     config_type(char *nkname);

/*  Descriptions: get the value of the specific item
 *  Parameters:
 *    char *nkname: the specific item in the redboot partition
 *  return:
 *    0: successful
 *    -1: fail
 */
int     config_get(char *nkname, void *value);

/*  Descriptions: set the value of the specific item
 *  Parameters:
 *    char *nkname: the specific item in the redboot partition
 *    void *value: the value of the item
 *  return:
 *    0: successful
 *    -1: fail. If the item can not be found, return -1.
 *      
 */
int     config_set(char *nkname, void *value);

/*  Descriptions: add the value of the specific item
 *  Parameters:
 *    char *nkname: the specific item in the redboot partition
 *    int type: it accept following values
 *      CONFIG_EMPTY   0
 *      CONFIG_BOOL    1
 *      CONFIG_INT     2
 *      CONFIG_STRING  3
 *      CONFIG_SCRIPT  4
 *      CONFIG_IP      5
 *      CONFIG_ESA     6
 *      CONFIG_NETPORT 7
 *    void *value: the value of the item
 *  return:
 *    0: successful
 *    -1: fail. If the item can not be found, return -1.
 */
int	config_add(char *nkname, int type, void *value);

/*  Descriptions: remove the specific item
 *  Parameters:
 *    char *nkname: the specific item in the redboot partition
 *  return:
 *    0: successful
 *    -1: fail
 */

int	config_remove(char *nkname);

/*
 * Descriptions: Add a 32 bytes software lock value into the memory buffer.
 *   Remember to call config_write() to write back the data to flash
 * Parameters:
 *   void *value: the 32 bytes value of this item
 * return:
 *   0: successful
 *   negative: fail.
 */
int     config_set_swlock(void *value);

/*
 * Descriptions: get the software lock value from memory buffer.
 *   Remember to call config_init() in first.
 * Parameters:
 *   void *value: return the 32 bytes value of this item
 * return:
 *   0: successful
 *   negative: fail.
 */
int	config_get_swlock(void *value);

#endif
