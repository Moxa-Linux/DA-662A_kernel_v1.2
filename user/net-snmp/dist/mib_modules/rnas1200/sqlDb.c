#include "sqlDb.h"

/* check whether db exist. */
int sqlAccessDb(char *path)
{
	if (NULL == path) {
		printf("Error!\n");
		return 1;
	}
	if (access(path, R_OK)) {
		printf("Error!\n");
		return 1;
	}

	return 0;
}

/* open sql database */
sqlite3 *sqlOpenDb(char *path)
{
	sqlite3 *sqlDb = NULL;

	if (sqlAccessDb(path))
		return NULL;

	if (sqlite3_open(path, &sqlDb)) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(sqlDb));
		sqlite3_close(sqlDb);
		return NULL;
	}

	if (sqlite3_busy_timeout(sqlDb, SQL_BUSY_TIMEOUT)) {
		fprintf(stderr, "Can't set busy timeout\n");
		return NULL;
	}

	return sqlDb;
}

/* check whether the item exist */
int sqlAccessDbItem(sqlite3 *sqlDb, dataContainer *node)
{
	char *retval = NULL;

	if (NULL == sqlDb || NULL == node) {
		printf("Error!\n");
		return 1;
	}

	retval = sqlGetDbItemValue(sqlDb, node);
	if (NULL == retval) {
		printf("Error!\n");
		return 1;
	}
	free(retval);
	return 0;
}

/* get the item id */
char *sqlGetDbItemId(sqlite3 *sqlDb, dataContainer *node)
{
	char *zErrMsg = NULL;
	int rc;
	char sqlCmd[SQL_MAX_LEN] = {};
	char *sqlResult = NULL;
	char **res = NULL;
	int nrow = 0, ncol = 0;

	if (NULL == sqlDb || NULL == node) {
		printf("Error!\n");
		return NULL;
	}

	snprintf(sqlCmd, sizeof(sqlCmd), "SELECT id FROM %s WHERE %s='%s';", dataGetContainerTableName(node), dataGetContainerColName(node), dataGetContainerValue(node));
	rc = sqlite3_get_table(sqlDb, sqlCmd, &res, &nrow, &ncol, &zErrMsg);
	if (rc != SQLITE_OK || nrow == 0 || ncol == 0) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return NULL;
	} else if (nrow == 0 || ncol == 0) {
		sqlite3_free_table(res);
		return NULL;
	}

	sqlResult = (char *) malloc((strlen(res[1]) + 1) * sizeof(char));
	if (NULL == sqlResult) {
		printf("Error!\n");
		sqlite3_free_table(res);
		return NULL;
	}
	memset(sqlResult, '\0', (strlen(res[1]) + 1) * sizeof(char));
	strcpy(sqlResult, res[1]);
	sqlite3_free_table(res);

	return sqlResult;
}

/* get the item value */
char *sqlGetDbItemValue(sqlite3 *sqlDb, dataContainer *node)
{
	char *zErrMsg = NULL;
	int rc;
	char sqlCmd[SQL_MAX_LEN] = {};
	char *sqlResult = NULL;
	char **res = NULL;
	int nrow = 0, ncol = 0;

	if (NULL == sqlDb || NULL == node) {
		printf("Error!\n");
		return NULL;
	}

	snprintf(sqlCmd, sizeof(sqlCmd), "SELECT %s FROM %s WHERE id=%s;", dataGetContainerColName(node), dataGetContainerTableName(node), dataGetContainerRowId(node));
	rc = sqlite3_get_table(sqlDb, sqlCmd, &res, &nrow, &ncol, &zErrMsg);
	if(rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return NULL;
	} else if (nrow == 0 || ncol == 0) {
		sqlite3_free_table(res);
		return NULL;
	}
	
	sqlResult = (char *) malloc((strlen(res[1]) + 1) * sizeof(char));
	if (NULL == sqlResult) {
		printf("Error!\n");
		sqlite3_free_table(res);
		return NULL;
	}
	memset(sqlResult, '\0', (strlen(res[1]) + 1) * sizeof(char));
	strcpy(sqlResult, res[1]);
	sqlite3_free_table(res);

	return sqlResult;
}

/* set the item value */
int sqlSetDbItemValue(sqlite3 *sqlDb, dataContainer *node)
{
	char *zErrMsg = NULL;
	int rc;
	char sqlCmd[SQL_MAX_LEN] = {};

	if (NULL == sqlDb || NULL == node) {
		printf("Error!\n");
		return 1;
	}

	snprintf(sqlCmd, sizeof(sqlCmd), "UPDATE %s SET %s='%s' WHERE id=%s;", dataGetContainerTableName(node), dataGetContainerColName(node), dataGetContainerValue(node), dataGetContainerRowId(node));
	rc = sqlite3_exec(sqlDb, sqlCmd, NULL, NULL, &zErrMsg);
	if(rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return 1;
	}

	return 0;
}

/* close sql database */
void sqlCloseDb(sqlite3 *sqlDb)
{
	if (NULL == sqlDb)
		return;

	sqlite3_close(sqlDb);
}

/* count the number of entry of the table */
int sqlCountDbTableEntry(sqlite3 *sqlDb, dataContainer *node)
{
	char *zErrMsg = NULL;
	int rc;
	char sqlCmd[SQL_MAX_LEN] = {};
	char **res = NULL;
	int nrow = 0, ncol = 0;

	if (NULL == sqlDb || NULL == node) {
		printf("Error!\n");
		return -1;
	}

	snprintf(sqlCmd, sizeof(sqlCmd), "SELECT id FROM %s;", dataGetContainerTableName(node));
	rc = sqlite3_get_table(sqlDb, sqlCmd, &res, &nrow, &ncol, &zErrMsg);
	if(rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}
	sqlite3_free_table(res);

	return nrow;
}

/* set the value of all the items in one column */
int sqlSetDbColValue(sqlite3 *sqlDb, dataContainer *node)
{
	char *zErrMsg = NULL;
	int rc;
	char sqlCmd[SQL_MAX_LEN] = {};

	if (NULL == sqlDb || NULL == node) {
		printf("Error!\n");
		return 1;
	}

	snprintf(sqlCmd, sizeof(sqlCmd), "UPDATE %s SET %s='%s';", dataGetContainerTableName(node), dataGetContainerColName(node), dataGetContainerValue(node));
	rc = sqlite3_exec(sqlDb, sqlCmd, NULL, NULL, &zErrMsg);
	if(rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return 1;
	}

	return 0;
}

