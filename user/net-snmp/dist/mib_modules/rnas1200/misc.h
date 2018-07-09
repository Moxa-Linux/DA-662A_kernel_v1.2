#ifndef _MISC_H_
#define _MISC_H_


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dataContainer.h"
#include "productContainer.h"
#include "filterContainer.h"
#include "sqlDb.h"

#define MISC_STR_LEN 300

/* declare function prototypes */
int isQueryFormat(char *);
int isSetFormat(char *);
int getData(char *, char *, char *, char *, char *, int (*)(char *));
int getCommit(sqlite3 *, dataContainer *, dataContainer **, dataContainer **);
dataContainer *writeData(sqlite3 *, dataContainer *);
int checkData(productContainer *, dataContainer *);
int checkValueType(const char *, char *);
int checkValueLengh(const int , char *);
int checkValueFilter(const char * const *, const char *, const int, char *);

#endif
