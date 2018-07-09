/*! \file db.h
 *     \brief A header file.
 *         
 *             Details.
 */
#ifndef _DB_H
#define _DB_H

#include <sqlite3.h>
#include <time.h>

#define POLL_DETECT	0
#define EVENT_DETECT	1
#define DB_SIZE		1024
#define BUSY_TIMEOUT 10000 /* Default 10s */

#define TABLE_NORMAL 0
#define TABLE_MODIFIED 1
#define TABLE_BUSY 2

#if defined(__PTRDIFF_TYPE__)  /* This case should work for GCC */
# define SQLITE_INT_TO_PTR(X)  ((void*)(__PTRDIFF_TYPE__)(X))
# define SQLITE_PTR_TO_INT(X)  ((int)(__PTRDIFF_TYPE__)(X))
#elif !defined(__GNUC__)       /* Works for compilers other than LLVM */
# define SQLITE_INT_TO_PTR(X)  ((void*)&((char*)0)[X])
# define SQLITE_PTR_TO_INT(X)  ((int)(((char*)X)-(char*)0))
#elif defined(HAVE_STDINT_H)   /* Use this case if we have ANSI headers */
# define SQLITE_INT_TO_PTR(X)  ((void*)(intptr_t)(X))
# define SQLITE_PTR_TO_INT(X)  ((int)(intptr_t)(X))
#else                          /* Generates a warning - but it always works */
# define SQLITE_INT_TO_PTR(X)  ((void*)(X))
# define SQLITE_PTR_TO_INT(X)  ((int)(X))
#endif


#define DB_OBJ \
\
	/* private instant */ \
	sqlite3 *db;						/**< db node */ \
	char **result;						/**< result */ \
	char *zErrMsg;						/**< error code */ \
	char path[DB_SIZE];					/**< db path */ \
	char table_name[DB_SIZE];					/**< table list from ini file */ \
	time_t time;						/**< db file access time */ \
	int errCode;						/**< error return code */ \
\
	/* public method */ \

typedef struct __DB__
{
	DB_OBJ
}db_t ;


/*
 ** @brief pass data from sqlite3_get_table() through
 ** to the callback function is uses to build the result.
 */
typedef struct TabResult {
	char **azResult;   /**< Accumulated output */
	char *zErrMsg;     /**< Error message text, if an error occurs */
	int nAlloc;        /**< Slots allocated for azResult[] */
	int nRow;          /**< Number of rows in the result */
	int nColumn;       /**< Number of columns in the result */
	int nData;         /**< Slots used in azResult[].  (nRow+1)*nColumn */
	int rc;            /**< Return code from sqlite3_exec() */
} TabResult;


/*
 * @brief db object construction
 * @param path database file full path
 * @return error:NULL success:pointer
 */
void *mx_db_init(char *);					/**< constructor */
/*
 * @brief db object destruction
 * @param DB struct
 * @return success:0
 */
void mx_db_release(struct __DB__ *);				/**< destructor */

/*
 * @brief get data from table function
 * @param DB struct
 * @param sql command
 * @param sqlite3 statement
 * @return success:0 error: else 
 */
__inline int mx_db_gettbl(struct __DB__ *, const char *, sqlite3_stmt **);
/*
 * @brief execute SQL function
 * @param DB struct
 * @param sql command
 * @return success:0 error: else
 */
__inline int mx_db_exec(struct __DB__ *, char *);

#endif

