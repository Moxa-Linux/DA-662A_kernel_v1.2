#ifndef _PRODUCTCONTAINER_H_
#define _PRODUCTCONTAINER_H_

#include <stdio.h>
#include "dataContainer.h"

/*
 * Declare MOXA product database stucture.
 *
 * This is the first version.
 * Only fullfill editing database directly.
 * Features:
 * 	1. Only editing is available.
 * 	2. Need nas.sql to know which parameter is available to edit.
 *	3. Use the same word/term as nas.sql used. 
 *
 * Need to be improved:
 * 	1. Fullfill deleting/adding functions.
 *
 */
struct productList {
	const int id;
	const char *tableName;
	const char *colName;
	const char *valueType; /* int, alnum, alpha, ascii */
	const int valueLengh;
	const char *valueFilterFormat;	/* enum:
									 * 	list valid value.
									 *
									 * logical: 
									 * 	infer valid value.
									 * 	rules:
									 * 		a/b: a or b
									 * 		1^10: 1 to 10 (Only for number)
									 * 		(): stand for a position
									 * 	example:
									 * 		ip (0^255).(0^255).(0^255).(0^255)
									 * 		timeOffset (+/-)(00^24):(00^60)
									 */
	const int valueFilterCount;
	const char * const *valueFilter;
};
typedef struct productList productContainer;

/* declare function prototype */
int productGetContainerLen(const productContainer *);
int productGetContainerId(const productContainer *, dataContainer *);
const char *productGetContainerValueType(const productContainer *, int);
const int productGetContainerValueLengh(const productContainer *, int);
const char *productGetContainerValueFilterFormat(const productContainer *, int);
const int productGetContainerValueFilterCount(const productContainer *, int);
const char * const *productGetContainerValueFilter(const productContainer *, int);

#endif
