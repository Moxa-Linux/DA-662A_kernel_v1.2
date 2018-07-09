#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <debug.h>
#include <log.h>
#include <db.h>
#include "os/log_api.h"

/**<
  @brief stamp time information on log string 
  @param this ini object instant
  @return success:0 error:1

  This function stamp time information on log string 
  before writing into the file.
  The format will be following:
  old style:
  	"YYYY/MM/DD DAY HH:MM:SS : <log string>"
  	"2012/02/15 Wed 15:00:29 : hello world!"
  new style:
	"YYYYMMDDHHMMSS : <log string>"
	"20120215150029 : hello world!"
 */
static int log_add_timestamp(char *buf, const int size)
{
	const char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

	time_t ltime;
	struct tm *now;
	char *new = NULL;

	/* initialize the current time */
	time(&ltime);
	now = localtime(&ltime);

	new = (char *)malloc(size);
	if (!new)
		return 1;

	memset(new, '\0', size);
#if 0
	if (snprintf(new, size, "%04d/%02d/%02d %s %02d:%02d:%02d : %s"
#endif
	if (snprintf(new, size, "%04d%02d%02d%02d%02d%02d : %s"
				, (1900 + now->tm_year)
				, (1 + now->tm_mon)
				, now->tm_mday
//				, wday[now->tm_wday]
				, now->tm_hour
				, now->tm_min
				, now->tm_sec
				, buf) == -1) {
		mx_debug("timestamp error");
		free(new);
		return 1;
	}
	
	strcpy(buf, new);
	free(new);
	return 0;
}

static int log_format(log_t *this, char *fmt, va_list args, char *buf, int size)
{
	int log_len = 0;

	/* get log data */
	memset(buf, '\0', size);
	log_len = vsnprintf(buf, size, fmt, args);
	if (log_len < 0 || log_len > LOG_MAX_SIZE - 2)
		return -1;
	strcat(buf, "\0");

	/* attach the timestamp for a log */
	switch (this->level) {
		case 0:
			break;
		case 1:
			if(log_add_timestamp(buf, size))
				return -1;
			break;
		default:
			break;
	}

	return log_len;
}

static int sql_format(int sql_op, char *buf, int size, char *log_buf)
{
	if (sql_op < LOG_DB_SQL_DROP || sql_op > LOG_DB_SQL_DELETE
		|| (sql_op == LOG_DB_SQL_INSERT && NULL == log_buf)
		|| NULL == buf)
		return -1;

	int sql_len = 0;
	char timestamp[LOG_TIMESTAMP_LEN];

	memset(buf, '\0', size);

	/* get sql command */
	switch (sql_op) {
		case LOG_DB_SQL_DROP:
			sql_len = snprintf(buf, size, "%s", LOG_DB_SQL_CMD_DROP);
			if (sql_len < 0 || sql_len > (size - 2)) {
				mx_debug("get sql cmd drop error");
				return -1;
			}
			break;
		case LOG_DB_SQL_CREATE:
			sql_len = snprintf(buf, size, "%s", LOG_DB_SQL_CMD_CREATE);
			if (sql_len < 0 || sql_len > (size - 2)) {
				mx_debug("get sql cmd create error");
				return -1;
			}
			break;
		case LOG_DB_SQL_INSERT:
			if(log_add_timestamp(timestamp, sizeof(timestamp))) {
				mx_debug("get sql cmd create error");
				return -1;
			}
			timestamp[LOG_TIMESTAMP_LEN - 3] = '\0';
			sql_len = snprintf(buf, size, "%s(\"%s\", \"%s\");", LOG_DB_SQL_CMD_INSERT, timestamp, log_buf);
			if (sql_len < 0 || sql_len > (size - 2)) {
				mx_debug("get sql cmd create error");
				return -1;
			}
			break;
		case LOG_DB_SQL_COUNT:
			sql_len = snprintf(buf, size, "%s", LOG_DB_SQL_CMD_COUNT);
			if (sql_len < 0 || sql_len > (size - 2)) {
				mx_debug("get sql cmd create error");
				return -1;
			}
			break;
		case LOG_DB_SQL_DELETE:
			sql_len = snprintf(buf, size, "%s", LOG_DB_SQL_CMD_DELETE);
			if (sql_len < 0 || sql_len > (size - 2)) {
				mx_debug("get sql cmd create error");
				return -1;
			}
			break;
		default:
			break;
	}

	return sql_len;
}

/**<
 * @brief log verify resource function private method
 * @param this log_len
 * @return success:0 error:1
 */
int log_verify(log_t *this, int log_len)
{
	return this->verify(this, log_len);
}

/**<
 * @brief log verify resource function private method of mode "display"
 * @param this log_len
 * @return success:0 error:1
 */
int log_verify_display(log_t *this, int log_len)
{
	return 0;
}

/**<
 * @brief log verify resource function private method of mode "file"
 * @param this log_len
 * @return success:0 error:1 log file full:2
 */
int log_verify_file(log_t *this, int log_len)
{
	int log_fd = 0;
	int tmp_fd = 0;
	struct stat file_stat;  /* file stat */

	int cut = 0;
	int extra = 0;
	char buf[1024] = { 0 };
	char file_tmp[1024];
	
	memset(file_tmp, '\0', sizeof(file_tmp));
	snprintf(file_tmp, sizeof(file_tmp), "%s.tmp", this->file_path);

	/* open log file */
	log_fd = open(this->file_path, O_RDONLY | O_CREAT, 0666);
	if (log_fd < 0)
		return 1;

	/* 1. If the file size is in control, do nothing */
	fstat(log_fd, &file_stat);
	if (file_stat.st_size + log_len * sizeof(char) <= this->file_size) {
		close(log_fd);
		return 0;

	/* 2. If the file size exceed the setting, return and do nothing */
	} else if (!this->file_shrink) {
		close(log_fd);
		return 2;

	/* 3. If file size exceed the setting, delete the very old one */
	/*    Open an temp file for adjust the log */
	} else {
		tmp_fd = open(file_tmp, O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (tmp_fd < 0) {
			close(log_fd);
			return 1;
		}

		extra = file_stat.st_size + log_len - this->file_size;
		while (cut < extra) {
			while (read(log_fd, buf, 1) > 0 && '\n' != buf[0])
				cut++;
		}

		/* write to the temp file */
		while (read(log_fd, buf, sizeof(buf) - 1) > 0) {
			write(tmp_fd, buf, strlen(buf) * sizeof(char));
			memset(buf, '\0', sizeof(buf));
		}
	}
	close(log_fd);
	close(tmp_fd);
	remove(this->file_path);
	if(rename(file_tmp, this->file_path))
		perror("rename:");

	return 0;
}

/**<
 * @brief log verify resource function private method of mode "db"
 * @param this log_len
 * @return success:0 error:1 log db full:2
 */
int log_verify_db(log_t *this, int log_len)
{
	db_t *log_db = NULL;
	int log_fd = 0;
	int sql_len = 0;
	int sql_count = 0;
	char sql_cmd[LOG_MAX_SIZE + LOG_TIMESTAMP_LEN * 2 + 36];
	sqlite3_stmt *stmt = NULL;
	
	/* If there's no db file ,create it. */
	log_fd = open(this->db_path, O_RDONLY | O_CREAT, 0666);
	if (log_fd < 0)
		return 1;
	close(log_fd);

	/* open db */
	log_db = mx_db_init(this->db_path);
	if (!log_db)
		return 1;

	/* format the sql cmd: count */
	sql_len = sql_format(LOG_DB_SQL_COUNT, sql_cmd, sizeof(sql_cmd), NULL);
	if (-1 == sql_len)
		return 1;

	/* 0. If there's no db table ,create it. */
	mx_db_gettbl(log_db, sql_cmd, &stmt);
	if (sqlite3_step(stmt) != SQLITE_ROW) {
		/* format the sql cmd: CREATE */
		sql_len = sql_format(LOG_DB_SQL_CREATE, sql_cmd, sizeof(sql_cmd), NULL);
		if (-1 == sql_len)
			return 1;

		mx_db_exec(log_db, sql_cmd);
		sql_count = 0;
	} else {
		sql_count = atoi((char *)sqlite3_column_text(stmt,0));
	}
	sqlite3_finalize(stmt);

	/* 1. If the db size is in control, do nothing */
	if (sql_count < this->db_size) {
		mx_db_release(log_db);
		return 0;
	/* 2. If the db size exceed the setting, return and do nothing */
	} else if (!this->db_shrink) {
		mx_db_release(log_db);
		return 2;
	/* 3. If  db exceed the setting, delete the oldest one */
	} else {
		/* format the sql cmd: DELETE */
		sql_len = sql_format(LOG_DB_SQL_DELETE, sql_cmd, sizeof(sql_cmd), NULL);
		if (-1 == sql_len)
			return 1;
		
		mx_db_exec(log_db, sql_cmd);
	}
	mx_db_release(log_db);
	return 0;
}

/**<
 * @brief log save function public method of mode "display"
 * @param this ini object instant
 * @param message a bunch of string, support by ANSI C99 format
 * @return success:0 error:1
 *
 * log_save function used by other object to save log messege.
 */
int log_save_display(log_t *this, char *fmt, va_list args)
{
	char log_buf[LOG_MAX_SIZE + LOG_TIMESTAMP_LEN];
	int log_len = 0;
	int retval = 0;

	if (NULL == this || NULL == fmt)
		return 1;

	/* format the log data */
	log_len = log_format(this, fmt, args, log_buf, sizeof(log_buf));
	if (-1 == log_len)
		return 1;

	/* If db doesn't exist, then create one.
	 * If file size exceed the setting,
	 * delete the very old one or just return*/
	retval = log_verify(this, log_len);
	if (retval)
		return retval;
	
	/* print to stderr */
	mx_debug("start to print log string\n");
	fprintf(stderr, "%s", log_buf);

	return 0;
}

/**<
 * @brief log save function public method of mode "file"
 * @param this ini object instant
 * @param message a bunch of string, support by ANSI C99 format
 * @return success:0 error:1 file full:2
 *
 * log_save function used by other object to save log messege.
 */
int log_save_file(log_t *this, char *fmt, va_list args)
{
	int retval = 0;             /* return value */
	char log_buf[LOG_MAX_SIZE + LOG_TIMESTAMP_LEN];
	int log_fd = 0;
	int log_len = 0;

	if (NULL == this || NULL == fmt)
		return 1;

	/* format the log data */
	log_len = log_format(this, fmt, args, log_buf, sizeof(log_buf));
	if (-1 == log_len)
		return 1;

	/* if file size exceed the setting,
	 * delete the very old one or just return*/
	retval = log_verify(this, log_len);
	if (retval)
		return retval;

	/* open file */
	log_fd = open(this->file_path, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (log_fd < 0) 
		return 1;
	
	/* put log data */
	mx_debug("start to print log string.\n");
	if (write(log_fd, log_buf, strlen(log_buf) * sizeof(char)) < 0)
		retval = 1;
	
	/* close file */
	close(log_fd);
	return retval;
}

/**<
 * @brief log save function public method of mode "db"
 * @param this ini object instant
 * @param message a bunch of string, support by ANSI C99 format
 * @return success:0 error:1 db full:2
 *
 * log_save function used by other object to save log messege.
 */
int log_save_db(log_t *this, char *fmt, va_list args)
{
	int retval = 0;             /* return value */
	char log_buf[LOG_MAX_SIZE + LOG_TIMESTAMP_LEN];
	int log_len = 0;
	char sql_cmd[LOG_MAX_SIZE + LOG_TIMESTAMP_LEN * 2 + 36];
	int sql_len = 0;
	db_t *log_db = NULL;

	if (NULL == this || NULL == fmt)
		return 1;

	/* format the log data */
	log_len = log_format(this, fmt, args, log_buf, sizeof(log_buf));
	if (-1 == log_len)
		return 1;

	/* format the sql cmd */
	sql_len = sql_format(LOG_DB_SQL_INSERT, sql_cmd, sizeof(sql_cmd), log_buf);
	if (-1 == sql_len)
		return 1;
	/* If db doesn't exist, then create one.
	 * If db size exceed the setting,
	 * delete the very old one or just return*/
	retval = log_verify(this, log_len);
	if (retval)
		return retval;

	/* open database */
	log_db = mx_db_init(this->db_path);
	if (!log_db) 
		return 1;
	
	/* put log data */
	mx_debug("start to print log string.\n%s\n", sql_cmd);
	mx_db_exec(log_db, sql_cmd);
	
	/* close database */
	mx_db_release(log_db);
	return retval;
}

