//#define CONFIG_JFFS2_V2_1     //mask by Jared Wu. 07-19-2005.  For JFFS2 v.2.1(firmware v1.5.2.4 before ), please define this MACRO

#ifndef _REDBOOT_CONFIG_H
#define _REDBOOT_CONFIG_H

#define __SOFTWARE_ID_PCI_IDENTIFICATION__	1 // Jared: 05-10-2010 create

#define CYGNUM_REDBOOT_FLASH_STRING_SIZE 128
#define CYGNUM_REDBOOT_FLASH_SCRIPT_SIZE 512
#define CYGNUM_REDBOOT_FLASH_CONFIG_SIZE 4096

#define MAX_SCRIPT_LENGTH CYGNUM_REDBOOT_FLASH_SCRIPT_SIZE
#define MAX_STRING_LENGTH CYGNUM_REDBOOT_FLASH_STRING_SIZE
#define MAX_CONFIG_DATA   CYGNUM_REDBOOT_FLASH_CONFIG_SIZE

struct _config {
	unsigned long len;
	unsigned long key1;
	unsigned char config_data[MAX_CONFIG_DATA-(4*4)];
	unsigned long key2;
	unsigned long cksum;
};

#define CONFIG_EMPTY   0
#define CONFIG_BOOL    1
#define CONFIG_INT     2
#define CONFIG_STRING  3
#define CONFIG_SCRIPT  4
#define CONFIG_IP      5
#define CONFIG_ESA     6
#define CONFIG_NETPORT 7

#define CONFIG_OBJECT_TYPE(dp)          (dp)[0]
#define CONFIG_OBJECT_KEYLEN(dp)        (dp)[1]
#define CONFIG_OBJECT_ENABLE_SENSE(dp)  (dp)[2]
#define CONFIG_OBJECT_ENABLE_KEYLEN(dp) (dp)[3]
#define CONFIG_OBJECT_KEY(dp)           ((dp)+4)
#define CONFIG_OBJECT_ENABLE_KEY(dp)    ((dp)+4+CONFIG_OBJECT_KEYLEN(dp))
#define CONFIG_OBJECT_VALUE(dp)         ((dp)+4+CONFIG_OBJECT_KEYLEN(dp)+CONFIG_OBJECT_ENABLE_KEYLEN(dp))

/*  Descriptions: get the value of the specific item
 *  Parameters:
 *    char *nkname: the specific item in the redboot partition
 *  return:
 *    0: successful
 *    -1: fail
 */

#if 1 // Jared, 05-17-2007
int	config_get(char *nkname, void *value);
int	config_length(int type);
#endif

#if defined(__SOFTWARE_ID_PCI_IDENTIFICATION__)  // Jared, 05-10-2010
int	config_get_target_swid(void *value);
#endif

#endif
