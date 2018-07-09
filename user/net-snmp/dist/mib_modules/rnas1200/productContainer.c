#include "productContainer.h"

/* Define the rnas1200 database container
 *
 * See "productContainer.h" for details
 */
const productContainer rnas1200Container[] = {
/*   id ,table name            ,column name          ,value   ,len,filter    ,filter                     ,filter
 *                                                    type         type       count                             
 */
	{1  ,"systemResources"     ,"hostname"           ,"alnum" ,45 ,"logical" ,1 ,(const char * const []) {"NULL"}},
//	{2  ,"ftpServerResources"  ,"port"               ,"int"   ,6  ,"logical" ,1 ,(const char * const []) {"(1^65535)"}},
	{2  ,"webServerResources"  ,"password"           ,"ascii" ,40 ,"logical" ,1 ,(const char * const []) {"NULL"}},
	{3  ,"webServerResources"  ,"port"               ,"int"   ,6  ,"logical" ,1 ,(const char * const []) {"(1^65535)"}},
	{4  ,"serviceResources"    ,"enable"             ,"int"   ,1  ,"enum"    ,2 ,(const char * const []) {"0", "1"}},
	{5  ,"snmpServerResources" ,"trustHostIp"        ,"ascii" ,16 ,"logical" ,1 ,(const char * const []) {"(0^255).(0^255).(0^255).(0^255)"}},
	{6  ,"snmpServerResources" ,"port"               ,"int"   ,6  ,"logical" ,1 ,(const char * const []) {"(1^65535)"}},
	{7  ,"snmpServerResources" ,"readCommunity"      ,"ascii" ,32 ,"logical" ,1 ,(const char * const []) {"NULL"}}, /* letters & punctuation */
	{8  ,"snmpServerResources" ,"readWriteCommunity" ,"ascii" ,32 ,"logical" ,1 ,(const char * const []) {"NULL"}}, /* same as before */
	{9  ,"snmpTrapResources"   ,"version"            ,"alnum" ,4  ,"enum"    ,2 ,(const char * const []) {"1", "2c"}},
	{10 ,"snmpTrapResources"   ,"serverPort"         ,"int"   ,6  ,"logical" ,1 ,(const char * const []) {"(1^65535)"}},
	{11 ,"snmpTrapResources"   ,"trustTrapIp0"       ,"ascii" ,16 ,"logical" ,1 ,(const char * const []) {"(0^255).(0^255).(0^255).(0^255)"}},
	{12 ,"snmpTrapResources"   ,"trustTrapIp1"       ,"ascii" ,16 ,"logical" ,1 ,(const char * const []) {"(0^255).(0^255).(0^255).(0^255)"}},
	{13 ,"snmpTrapResources"   ,"trustTrapIp2"       ,"ascii" ,16 ,"logical" ,1 ,(const char * const []) {"(0^255).(0^255).(0^255).(0^255)"}},
	{14 ,"snmpTrapResources"   ,"community"          ,"ascii" ,30 ,"logical" ,1 ,(const char * const []) {"NULL"}}, /* letters & punctuation */
	{15 ,"snmpTrapResources"   ,"notifyInterval"     ,"int"   ,0  ,"logical" ,1 ,(const char * const []) {"(1^10000)"}},
	{16 ,"dnsResources"        ,"enableManual"       ,"int"   ,1  ,"enum"    ,2 ,(const char * const []) {"0", "1"}},
	{17 ,"dnsContainer"        ,"ip"                 ,"ascii" ,16 ,"logical" ,1 ,(const char * const []) {"(0^255).(0^255).(0^255).(0^255)"}},
	{18 ,"networkResources"    ,"ip"                 ,"ascii" ,16 ,"logical" ,1 ,(const char * const []) {"(0^255).(0^255).(0^255).(0^255)"}},
	{19 ,"networkResources"    ,"subnet"             ,"ascii" ,16 ,"logical" ,1 ,(const char * const []) {"(0^255).(0^255).(0^255).(0^255)"}},
	{20 ,"networkResources"    ,"gateway"            ,"ascii" ,16 ,"logical" ,1 ,(const char * const []) {"(0^255).(0^255).(0^255).(0^255)"}}, /* related to subnet */
	{21 ,"networkResources"    ,"defaultGateway"     ,"int"   ,1  ,"enum"    ,2 ,(const char * const []) {"0", "1"}},
	{22 ,"networkResources"    ,"enableDHCP"         ,"int"   ,1  ,"enum"    ,2 ,(const char * const []) {"0", "1"}},
	{23 ,"trunkResources"      ,"enableTrunking"     ,"int"   ,1  ,"enum"    ,2 ,(const char * const []) {"0", "1"}},
	{24 ,"trunkResources"      ,"trunkingMode"       ,"int"   ,1  ,"enum"    ,7 ,(const char * const []) {"0", "1", "2", "3", "4", "5", "6"}},
	{25 ,"systemLogResources"  ,"maxRecord"          ,"int"   ,0  ,"logical" ,1 ,(const char * const []) {"(1^65535)"}},
//	{26 ,"bootResources"       ,"commits"            ,"int"   ,1  ,"enum"    ,2 ,(const char * const []) {"0", "1"}},
	{26 ,"peripheralResources" ,"options"            ,"ascii" ,4  ,"enum"    ,2 ,(const char * const []) {"PoE", "PoE+"}},
//	{29 ,"timezoneResources"   ,"timeOffset"         ,"ascii" ,10 ,"logical" ,1 ,(const char * const []) {"(+/-)(00^23):(00^59)"}},
	{27 ,"timezoneResources"   ,"countryDLS"         ,"ascii" ,20 ,"enum"    ,52,(const char * const []) {"GMT+12", "Samoa", "Hawaii", "Alaska", "Pacific", "Arizona", "Mountain", "Saskatchewan", "Central", "Bogota", "Eastern", "Manaus", "Caracas", "Newfoundland", "Montevideo", "Buenos_Aires", "Noronha", "Azores", "Cape_Verde", "Casablanca", "London", "Amsterdam", "Gaborone", "Amman", "Harare", "Baghdad", "Kuwait", "Tehran", "Muscat", "Baku", "Kabul", "Oral", "Karachi", "Kolkata", "Katmandu", "Dhaka", "Almaty", "Rangoon", "Krasnoyarsk", "Bangkok", "Taipei", "Irkutsk", "Yakutsk", "Tokyo", "Darwin", "Adelaide", "Brisbane", "Canberra", "Magadan", "Auckland", "Fiji", "Tongatapu"}},
	{28 ,"timeResources"       ,"enableNTP"          ,"int"   ,1  ,"enum"    ,2 ,(const char * const []) {"0", "1"}},
	{29 ,"timeResources"       ,"displayFormat"      ,"int"   ,1  ,"enum"    ,2 ,(const char * const []) {"0", "1"}},
	{30 ,"timeResources"       ,"updateNTPInterval"  ,"int"   ,3  ,"logical" ,1 ,(const char * const []) {"(1^36)"}},
	{31 ,"timeResources"       ,"ntpServerIp"        ,"ascii" ,16 ,"logical" ,1 ,(const char * const []) {"(0^255).(0^255).(0^255).(0^255)"}}
//	{33 ,"timezoneResources"   ,"tYear"              ,"int"   ,5  ,"logical" ,1 ,(const char * const []) {"(1970^9999)"}},
//	{34 ,"timezoneResources"   ,"tMonth"             ,"int"   ,3  ,"logical" ,1 ,(const char * const []) {"(1^12)"}},
//	{35 ,"timezoneResources"   ,"tDay"               ,"int"   ,3  ,"logical" ,1 ,(const char * const []) {"(1^31)"}},
//	{36 ,"timezoneResources"   ,"tHour"              ,"int"   ,3  ,"logical" ,1 ,(const char * const []) {"(0^23)"}},
//	{37 ,"timezoneResources"   ,"tMin"               ,"int"   ,3  ,"logical" ,1 ,(const char * const []) {"(0^59)"}},
//	{38 ,"timezoneResources"   ,"tSec"               ,"int"   ,3  ,"logical" ,1 ,(const char * const []) {"(0^59)"}},
	/* Lock.Lin	disk control table */
//	{39 ,"diskVolumeResources" ,"diskMode"           ,"alpha" ,10 ,"enum"    ,4 ,(const char * const []) {"single", "linear", "raid 0", "raid 1"}},
//	{40 ,"diskVolumeResources" ,"dataXProMode"       ,"int"   ,1  ,"enum"    ,2 ,(const char * const []) {"0", "1"}},
//	{41 ,"diskVolumeResources" ,"bitmap"             ,"int"   ,1  ,"enum"    ,2 ,(const char * const []) {"0", "1"}}
};

/* get product container length
 *
 * retval:
 * 	-1: product container is empty.
 * 	n: n length
 */
int productGetContainerLen(const productContainer *productObj)
{
	int count = 0;

	if (NULL == productObj) {
		printf("Error!\n");
		return -1;
	}

	while (productObj[count].id == count + 1)
		count++;

	return count;
}

/* get product container id for data container
 *
 * retval:
 *	-1: container is empty
 * 	0: can't find the data container
 * 	n: id number
 */
int productGetContainerId(const productContainer *productObj, dataContainer *node)
{
	int length;
	int count = 0;
	
	if (NULL == productObj || NULL == node) {
		printf("Error!\n");
		return -1;
	}

	length = productGetContainerLen(productObj);
	while (count < length) {
		if (strcmp(productObj[count].tableName, dataGetContainerTableName(node)) == 0)
			if (strcmp(productObj[count].colName, dataGetContainerColName(node)) == 0)
				break;
		count++;
	}

	if (count == length)
		return 0;

	return ++count;
}

/* get valueType from id
 *
 * retval:
 * 	NULL: container is empty or id is not available
 * 	char *: address of string of "valueType"
 */
const char *productGetContainerValueType(const productContainer *productObj, int id)
{
	if (NULL == productObj || id <= 0 || id > productGetContainerLen(productObj)) {
		printf("Error!\n");
		return NULL;
	}
	return productObj[--id].valueType;
}
/* get valueLengh from id
 *
 * retval:
 * 	-1: container is empty or id is not available
 * 	n: value of "valueLengh"
 */
const int productGetContainerValueLengh(const productContainer *productObj, int id)
{
	if (NULL == productObj || id <= 0 || id > productGetContainerLen(productObj)) {
		printf("Error!\n");
		return -1;
	}
	return productObj[--id].valueLengh;
}
/* get valueFilterFormat from id
 *
 * retval:
 * 	NULL: container is empty or id is not available
 * 	char *: address of string of "valueFilterFormat"
 */
const char *productGetContainerValueFilterFormat(const productContainer *productObj, int id)
{
	if (NULL == productObj || id <= 0 || id > productGetContainerLen(productObj)) {
		printf("Error!\n");
		return NULL;
	}
	return productObj[--id].valueFilterFormat;
}
/* get valueFilterCount from id
 *
 * retval:
 * 	-1: container is empty or id is not available
 * 	n: value of "valueFilterCount"
 */
const int productGetContainerValueFilterCount(const productContainer *productObj, int id)
{
	if (NULL == productObj || id <= 0 || id > productGetContainerLen(productObj)) {
		printf("Error!\n");
		return -1;
	}
	return productObj[--id].valueFilterCount;
}
/* get valueFilter from id
 *
 * retval:
 * 	NULL: container is empty or id is not available
 * 	char *: address of string of "valueFilter"
 */
const char * const *productGetContainerValueFilter(const productContainer *productObj, int id)
{
	if (NULL == productObj || id <= 0 || id > productGetContainerLen(productObj)) {
		printf("Error!\n");
		return NULL;
	}
	return productObj[--id].valueFilter;
}

