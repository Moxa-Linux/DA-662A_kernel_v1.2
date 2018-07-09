#include "misc.h"

/* write data and commit in database 
 *
 * retval:
 * 	the drop list
 * 	
 */
dataContainer *writeData(sqlite3 *sqlDb, dataContainer *head)
{
	dataContainer *curr = NULL;
	dataContainer *dataCommit = NULL;
	dataContainer *controlCommit = NULL;
	dataContainer *controlCommitList = NULL;
	char *controlCommitValue = NULL;
	int errNum;
	int countCurr;

	if (NULL == sqlDb) {
		printf("Error!\n");
		return head;
	}
	
	/* drop or write data if controlManagement is busy or not */
	curr = head;
	countCurr = 1;
	while (curr != NULL) {
		dataCommit = NULL;
		controlCommit = NULL;
		controlCommitValue = NULL;

		if (getCommit(sqlDb, curr, &dataCommit, &controlCommit)) {
			curr = curr->next;
			countCurr++;
			continue;
		}
		controlCommitValue = sqlGetDbItemValue(sqlDb, controlCommit);
		if (!strcmp(controlCommitValue, "0")) {
			sqlSetDbItemValue(sqlDb, curr);
			sqlSetDbItemValue(sqlDb, dataCommit);
			controlCommitList = dataInsertContainerNode(controlCommitList, controlCommit, dataGetContainerLen(controlCommitList) + 1, &errNum);
			curr = curr->next;
			head = dataRemoveContainerNode(head, countCurr);
		} else {
			dataFreeContainerNode(controlCommit);
			curr = curr->next;
			countCurr++;
		}
		dataFreeContainerNode(dataCommit);
		free(controlCommitValue);
	}

	/* remove duplicate in controlManagement */
	controlCommitList = dataRemoveContainerDuplicate(controlCommitList, DATA_TABLENAME | DATA_ROWID);

	/* commit controlManagement */
	curr = controlCommitList;
	while (curr != NULL) {
		sqlSetDbItemValue(sqlDb, curr);
		curr = curr->next;
	}

	/* release resources */
	controlCommitList = dataFreeContainer(controlCommitList);

	return head;
}

/* get commit data from table and controlManagement */
int getCommit(sqlite3 *sqlDb, dataContainer *node, dataContainer **dataCommit, dataContainer **controlManagementCommit)
{
	char *id = NULL;
	dataContainer *controlManagementAction = NULL;

	if (NULL == sqlDb || NULL == node) {
		printf("Error!\n");
		return 1;
	}
	
	/* only for rnas1200 dnsContainer table */
	if (!strcmp(dataGetContainerTableName(node), "dnsContainer")) {
		controlManagementAction = dataCreateContainerNode("controlManagement", "actions", "", "dnsResources");
	} else {
		controlManagementAction = dataCreateContainerNode("controlManagement", "actions", "", dataGetContainerTableName(node));
	}
	if (NULL == controlManagementAction) {
		printf("Error!\n");
		goto err;
	}

	id = sqlGetDbItemId(sqlDb, controlManagementAction);
	if (NULL == id) {
		printf("Error!\n");
		goto err;
	}

	*controlManagementCommit = dataCreateContainerNode("controlManagement", "commits", id, "1");
	if (NULL == *controlManagementCommit) {
		printf("Error!\n");
		goto err;
	}

	*dataCommit = dataCreateContainerNode(dataGetContainerTableName(node), "commits", dataGetContainerRowId(node), "1");
	if (NULL == *dataCommit) {
		printf("Error!\n");
		goto err;
	}
		
	free(id);
	dataFreeContainerNode(controlManagementAction);
	return 0;

err:
	if (id != NULL)
		free(id);
	if (*dataCommit != NULL)
		dataFreeContainerNode(*dataCommit);
	if (*controlManagementCommit != NULL)
		dataFreeContainerNode(*controlManagementCommit);
	if (controlManagementAction != NULL)
		dataFreeContainerNode(controlManagementAction);
	return 1;
}

/* chech whether if string is the correct format.
 * The format is "xxx.xxx.xxx".
 *
 * retval:
 * 	0: correct
 * 	1: not correct
 * 	2: argument error
 */
int isQueryFormat(char *string)
{
	int length;
	int dotCount = 0;
	int curr = 0;
	int prev = -1;

	if (NULL == string) {
		printf("Error!\n");
		return 2;
	}

	length = strlen(string);
	if (length == 0 || length > MISC_STR_LEN) {
		printf("Error!\n");
		return 1;
	}

	for (curr = 0; curr < length; curr++) {
		/* 1. check whether there's a string between two '.' */
		if (string[curr] == '.') {
			if ((curr - prev) > 1) {
				prev = curr;
				dotCount++;
			} else {
				printf("Error!\n");
				return 1;
			}
		}
	}
	/* 2. check total number of '.' 
	 * 3. check if there is no string after the last '.'
	 */
	if (dotCount != 2 || prev == --curr) {
		printf("Error!\n");
		return 1;
	}

	return 0;
}

/* chech whether if string is the correct format.
 * The format is "xxx.xxx.xxx=xxx".
 *
 * retval:
 * 	0: correct
 * 	1: not correct
 * 	2: argument error
 */
int isSetFormat(char *string) {
	int length;
	int dotCount = 0;
	int equalCount = 0;
	int curr = 0;
	int prev = -1;

	if (NULL == string) {
		printf("Error!\n");
		return 2;
	}

	length = strlen(string);
	if (length == 0 || length > MISC_STR_LEN) {
		printf("Error!\n");
		return 1;
	}

	for (curr = 0; curr < length; curr++) {
		/* 1. check whether there's a string between two '.' */
		if (string[curr] == '.' && equalCount == 0) {
			if ((curr - prev) > 1) {
				prev = curr;
				dotCount++;
			} else {
				printf("Error!\n");
				return 1;
			}
		}
		/* 2. check number of '.' before '=' */
		if (string[curr] == '=') {
			if (dotCount == 2) {
				prev = curr;
				equalCount++;
			} else {
				printf("Error!\n");
				return 1;
			}
		}
	}

	/* 3. check total number of '.' and '=' 
	 * 4. check whether there's no string after '='
	 */
	if (dotCount != 2 || equalCount != 1 || prev == --curr) {
		printf("Error!\n");
		return 1;
	}

	return 0;
}

/* get the data from string 
 *
 * retval:
 * 	0: success
 * 	1: fail
 */
int getData(char *string, char *tableName, char *colName, char *rowId, char *value, int (*checkStrFormat)(char *))
{
	int length;
	char strBuf[MISC_STR_LEN + 1];
	char dataBuf[MISC_STR_LEN + 1];
	char *ptrBuf;

	if (checkStrFormat(string)) {
		printf("Error!\n");
		return 1;
	}
	length = strlen(string);

	/* use buffer since string from heap memory is not available to "strtok_r" */
	memset(strBuf, '\0', sizeof(strBuf));
	strcpy(strBuf, string);

	/* get item */
	memset(dataBuf, '\0', sizeof(dataBuf));
	strcpy(dataBuf, strtok_r(strBuf, "=", &ptrBuf));
	/* get value (available for NULL value) */
	memset(value, '\0', MISC_STR_LEN + 1);
	if (length > strlen(dataBuf) + 1)
		strcpy(value, strtok_r(NULL, "=", &ptrBuf));
	/* get tableName */
	memset(tableName, '\0', MISC_STR_LEN + 1);
	strcpy(tableName, strtok_r(dataBuf, ".", &ptrBuf));
	/* get colName */
	memset(colName, '\0', MISC_STR_LEN + 1);
	strcpy(colName, strtok_r(NULL, ".", &ptrBuf));
	/* get rowId */
	memset(rowId, '\0', MISC_STR_LEN + 1);
	strcpy(rowId, strtok_r(NULL, ".", &ptrBuf));

	return 0;
}

/* check database whether the data from node is available to edit.
 * 
 * retval:
 *	0: correct
 *	1: not correct
 *	2: data is not available to edit.
 * 	3: argument error
 */
int checkData(productContainer *productObj, dataContainer *node)
{
 	int id;

	if (NULL == productObj || NULL == node) {
		printf("Error!\n");
		return 3;
	}

	id = productGetContainerId(productObj, node);
	if (id <= 0) {
		printf("Error!\n");
		return 2;
	}

	/* check type of value */
	if (checkValueType(productGetContainerValueType(productObj, id), dataGetContainerValue(node))) {
		printf("Error!\n");
		return 1;
	}
	/* check length of value */
	if (checkValueLengh(productGetContainerValueLengh(productObj, id), dataGetContainerValue(node))) {
		printf("Error!\n");
		return 1;
	}
	/* check filter of value */
	if (checkValueFilter(productGetContainerValueFilter(productObj, id), productGetContainerValueFilterFormat(productObj, id), productGetContainerValueFilterCount(productObj, id), dataGetContainerValue(node))) {
		printf("Error!\n");
		return 1;
	}

	return 0;
}

/* check type of value
 * 
 * param:
 *	valueType: "int", "char"
 *
 * retval:
 * 	0: correct
 * 	1: not correct
 * 	2: argument error
 *
 */
int checkValueType(const char *type, char *value)
{
	int count = 0;
	int (*checkFunc)(int);

	if (NULL == type || NULL == value) {
		printf("Error!\n");
		return 2;
	}

	if (!strcmp(type, "int"))
		checkFunc = &isdigit;
	else if (!strcmp(type, "alnum"))
		checkFunc = &isalnum;
	else if (!strcmp(type, "alpha"))
		checkFunc = &isalpha;
	else if (!strcmp(type, "ascii"))
		checkFunc = &isascii;
	else
		return 1;

	while (value[count] != '\0') {
		if (checkFunc(value[count]) == 0)
			return 1;
		count++;
	}

	return 0;
}

/* check length of value
 *
 * retval:
 * 	0: correct
 * 	1: not correct
 * 	2: argument error
 *
 */
int checkValueLengh(const int length, char *value)
{
	if (NULL == value || 0 > length) {
		printf("Error!\n");
		return 2;
	}

	if (length == 0)
		return 0;
	if (strlen(value) > length)
		return 1;

	return 0;
}

/* check filter of data
 * 
 *  param:
 *  	valueFilterFormat: "enum", "logical"
 * 
 *  functionality:
 *   enum:
 *     list valid data.
 * 
 *   logical:
 *     infer valid data.
 *     rules:
 *       a/b: a or b
 *       1^10: 1 to 10 (Only for number)
 *       (): stand for a position
 * 
 *     example:
 *       ip (0^255).(0^255).(0^255).(0^255)
 *       timeOffset (+/-)(00^23):(00^59)
 * 
 *  Output:
 *   0: correct
 *   1: not correct
 *   2: argument error
 */
int checkValueFilter(const char * const *filter, const char *format, int count, char *value)
{
	int retval = 1;
	filterContainer *filterObj = NULL;

	if (NULL == filter || NULL == format || NULL == value || 0 == count) {
		printf("Error!\n");
		retval = 2;
		return retval;
	}

	/* check for "enum" */
	if (strcmp(format, "enum") == 0) {
		while (count--) {
			if (strcmp(value, filter[count]) == 0) {
				retval = 0;
				break;
			}
		}

	/* check for "logical" */
	} else {
		/* case: filter==NULL */
		if (strcmp(filter[0], "NULL") == 0) {
			retval = 0;

			/* case: filter!=NULL */
		} else {
			filterObj = filterCreateContainer(filter[0]);
			if (NULL == filterObj) {
				printf("Error!\n");
				return 2;
			}
			retval = filterCompareContainer(filterObj, value);
			filterObj = filterFreeContainer(filterObj);
		}
	}

	return retval;
}

