/*! \file log.h
 *     \brief Log header file.
 *         
 *      Details.
 */
#ifndef _LOG_H
#define _LOG_H

#include <stdarg.h>

#define LOG_DISPLAY_MODE       0   /**< print out via stderr */
#define LOG_FILE_MODE          1   /**< save to file */
#define LOG_DB_MODE            2   /**< save into database */
#define LOG_LEVEL0             0   /**< save original messege */
#define LOG_LEVEL1             1   /**< save with timestamp attached */

#define LOG_DISABLE            0   /**< value of disable */
#define LOG_ENABLE             1   /**< value of enable */

#define LOG_TIMESTAMP_LEN      17         /**< length of the timestamp */
#define LOG_MAX_SIZE           256        /**< max lengh of log string*/
#define LOG_FILE_MIN_SIZE      1*1024     /**< min size of log file */
#define LOG_FILE_MAX_SIZE      1*1024*1024/**< max size of log file */
#define LOG_DB_MIN_SIZE        50         /**< min size of row numbers */
#define LOG_DB_MAX_SIZE        1000       /**< max size of row numbers */

#define LOG_DB_SQL_DROP        0
#define LOG_DB_SQL_CREATE      1
#define LOG_DB_SQL_INSERT      2
#define LOG_DB_SQL_COUNT       3
#define LOG_DB_SQL_DELETE      4

#define LOG_DB_SQL_CMD_DROP \
"DROP TABLE IF EXISTS concentrator_log;"
#define LOG_DB_SQL_CMD_CREATE \
"CREATE TABLE concentrator_log(time VARCHAR(17), ext_info VARCHAR(273));"
#define LOG_DB_SQL_CMD_INSERT \
"INSERT INTO concentrator_log VALUES"
#define LOG_DB_SQL_CMD_COUNT \
"SELECT COUNT(time) FROM concentrator_log;"
#define LOG_DB_SQL_CMD_DELETE \
"DELETE FROM concentrator_log WHERE time IN (SELECT time FROM concentrator_log LIMIT 1);"



#define LOG_OBJ \
	/* private instant */ \
	int level;				/**< log level */ \
	int mode;				/**< log mode */ \
	/* LOG_DISPLAY_MODE: print out */ \
	/* LOG_FILE_MODE: save to file */ \
	char *file_path;		/**< path of file */ \
	int file_size;			/**< size of file */ \
	int file_shrink;        /**< delete the oldest log file or not */ \
	/* LOG_DB_MODE: save into database */ \
	char *db_path;			/**< path of db */ \
	int db_size;			/**< size of db */ \
	int db_shrink;          /**< delete the oldest log db or not */ \
\
	/* private method */ \
	int (*verify) (struct __LOG__ *, int); \
\
	/* public method */ \
	int (*save) (struct __LOG__ *, char *, va_list);


typedef struct __LOG__
{
	LOG_OBJ;
} log_t;



/* API */

/**<
 * @brief apps object construction
 * @param void
 * @return error:NULL success:pointer 
 *
 * log_save function used by object saving a printable information.
 * User can choose the level depend on how the printable info either
 * is save inside the file or print on the screen.
 */
log_t *mx_log_init(int, char *, int, int, int); /**< constrctor */

/**< 
 * @brief apps object destruction
 * @param void
 * @return success:0
 */
void  mx_log_release(struct __LOG__ *);	/**< destructor */

/**<
 * @brief log save function public method
 * @param this ini object instant
 * @param message a bunch of string, support by ANSI C99 format
 * @return success:0 error:1	
 */
int mx_log_save(log_t *this, char *fmt, ...);

#endif
