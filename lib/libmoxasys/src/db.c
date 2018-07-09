#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "db.h"
#include "debug.h"
#include "os/db_api.h"

/**
 * @brief db object construction
 * @param db file path
 * @return error:NULL success:pointer
 *
 * mx_db_init function used by other object to initialize the memory
 * and prepare internal db file descriptor.
 */
void *mx_db_init(char *path)
{
	if(!path)
		return NULL;

	db_t *this;

	this = (db_t *)malloc(sizeof(db_t));
	if (!this)
		return NULL;

	/* init DB path array */
	memset(this->path,'\0',DB_SIZE);
	this->time = 0;
	strncpy(this->path, path, strlen(path));

	/* init instant */
	if (sqlite3_open_v2(this->path, &this->db, SQLITE_OPEN_READWRITE, 0)) {
		mx_print("Can't open database %s\n", this->path);
		free(this);
		return NULL;
	}

	/* DB busy handler */
	if(SQLITE_OK != sqlite3_busy_timeout(this->db, BUSY_TIMEOUT)){
		mx_print("DB still busy after trying access for 10 seconds\n"); 
	}

	/* method assign */
	return this;
}

int sqlite3Strlen30(const char *z){
	const char *z2 = z;
	if( z==0 ) return 0;
	while( *z2 ){ z2++; }
	return 0x3fffffff & (int)(z2 - z);
}


/*
 ** This routine is called once for each row in the result table.  Its job
 ** is to fill in the TabResult structure appropriately, allocating new
 ** memory as necessary.
 */
static int sqlite3_get_table_cb(void *pArg, int nCol, char **argv, char **colv){
	TabResult *p = (TabResult*)pArg;  /* Result accumulator */
	int need;                         /* Slots needed in p->azResult[] */
	int i;                            /* Loop counter */
	char *z;                          /* A single column of result */

	/* Make sure there is enough space in p->azResult to hold everything
	 ** we need to remember from this invocation of the callback.
	 */
	if( p->nRow==0 && argv!=0 ){
		need = nCol*2;
	}else{
		need = nCol;
	}
	if( p->nData + need > p->nAlloc ){
		char **azNew;
		p->nAlloc = p->nAlloc*2 + need;
		azNew = sqlite3_realloc( p->azResult, sizeof(char*)*p->nAlloc );
		if( azNew==0 ) goto malloc_failed;
		p->azResult = azNew;
	}

	/* If this is the first row, then generate an extra row containing
	 ** the names of all columns.
	 */
	if( p->nRow==0 ){
		p->nColumn = nCol;
		for(i=0; i<nCol; i++){
			z = sqlite3_mprintf("%s", colv[i]);
			if( z==0 ) goto malloc_failed;
			p->azResult[p->nData++] = z;
		}
	}else if( p->nColumn!=nCol ){
		sqlite3_free(p->zErrMsg);
		p->zErrMsg = sqlite3_mprintf(
				"sqlite3_get_table() called with two or more incompatible queries"
				);
		p->rc = SQLITE_ERROR;
		return 1;
	}

	/* Copy over the row data
	 */
	if( argv!=0 ){
		for(i=0; i<nCol; i++){
			if( argv[i]==0 ){
				z = 0;
			}else{
				int n = sqlite3Strlen30(argv[i])+1;
				z = sqlite3_malloc( n );
				if( z==0 ) goto malloc_failed;
				memcpy(z, argv[i], n);
			}
			p->azResult[p->nData++] = z;
		}
		p->nRow++;
	}
	return 0;

malloc_failed:
	p->rc = SQLITE_NOMEM;
	return 1;
}

/*
 * @brief get data from table function
 * @param return code
 * @return error: else  success:0
 */
int mx_db_gettbl(
	struct __DB__ *this,	/* The database on which the SQL executes */
	const char *sqlcmd, 	/* The SQL to be executed */
	sqlite3_stmt **stmt 	/* Write the result table here */
){
	int rt_val;
	rt_val = sqlite3_prepare_v2(this->db, sqlcmd, strlen(sqlcmd), stmt, 0);  
	return rt_val;
}


/*
 * @brief execute sql command function
 * @param DB structure pointer
 * @param SQL command
 * @return error: else success:0
 */
int mx_db_exec(
	struct __DB__ *this, 
	char *sqlcmd
){
	int rt_val;
	TabResult res;
	rt_val = sqlite3_exec(this->db, sqlcmd, sqlite3_get_table_cb, &res, &this->zErrMsg);

	return rt_val;
}


/**
 * @brief db object destruction
 * @param void
 * @return success:0
 *
 * mx_db_release function used by other object to release the memory
 * initialized by construction.
 */
void mx_db_release(db_t *this)
{
	if (this){
		/* Close  database */
		sqlite3_close(this->db);
		free(this);
	}
	return;
}



