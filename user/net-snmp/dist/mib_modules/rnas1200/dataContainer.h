#ifndef _DATACONTAINER_H_
#define _DATACONTAINER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* define flags */
#define NUM_OF_FLAGS 4
#define DATA_TABLENAME 1
#define DATA_COLNAME 2
#define DATA_ROWID 4
#define DATA_VALUE 8
#define IS_TABLENAME(x) ((x & DATA_TABLENAME) ^ DATA_TABLENAME)
#define IS_COLNAME(x) ((x & DATA_COLNAME) ^ DATA_COLNAME)
#define IS_ROWID(x) ((x & DATA_ROWID) ^ DATA_ROWID)
#define IS_VALUE(x) ((x & DATA_VALUE) ^ DATA_VALUE)


/* declare data container list */
struct dataList {
	char *tableName;
	char *colName;
	char *rowId;
	char *value;
	int timestamp;
	struct dataList *next;
	struct dataList *prev;
};
typedef struct dataList dataContainer;


/* declare function prototype */
dataContainer *dataDuplicateContainer(dataContainer *, dataContainer *);
dataContainer *dataRemoveContainerTimeout(dataContainer *, int);
dataContainer *dataRemoveContainerDuplicate(dataContainer *, int);
dataContainer *dataRemoveContainerNode(dataContainer *, int);
void dataFreeContainerNode(dataContainer *);
dataContainer *dataCreateContainerNode(char *, char *, char *, char *);
dataContainer *dataInsertContainerNode(dataContainer *, dataContainer *, int, int *);
int dataGetContainerLen(dataContainer *);
void dataDisplayContainer(dataContainer *);
dataContainer *dataFreeContainer(dataContainer *);
dataContainer *dataAddContainer(dataContainer *, char *, char *, char *, char *);
char *dataGetContainerTableName(dataContainer *);
char *dataGetContainerColName(dataContainer *);
char *dataGetContainerRowId(dataContainer *);
char *dataGetContainerValue(dataContainer *);
int dataGetContainerTimestamp(dataContainer *);

#endif
