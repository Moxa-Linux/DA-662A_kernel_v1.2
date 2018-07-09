#ifndef _SQLDB_H_
#define _SQLDB_H_

#include <stdio.h>
#include <unistd.h>
#include <sqlite3.h>
#include "dataContainer.h"

#define SQL_MAX_LEN 300
#define SQL_BUSY_TIMEOUT 3000	//ms

int sqlAccessDb(char *);
sqlite3 *sqlOpenDb(char *);
int sqlAccessDbItem(sqlite3 *, dataContainer *);
char *sqlGetDbItemId(sqlite3 *, dataContainer *);
char *sqlGetDbItemValue(sqlite3 *, dataContainer *);
int sqlSetDbItemValue(sqlite3 *, dataContainer *);
void sqlCloseDb(sqlite3 *);
int sqlCountDbTableEntry(sqlite3 *, dataContainer *);
int sqlSetDbColValue(sqlite3 *, dataContainer *);

#endif
