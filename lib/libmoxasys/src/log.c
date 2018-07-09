#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <log.h>
#include "os/log_api.h"

/**<
 * @brief apps object construction
 * @param void
 * @return error:NULL success:pointer
 *
 * log_save function used by object saving a printable information.
 * User can choose the level depend on how the printable info eithe
 * is save inside the file or print on the screen.
 */
log_t *mx_log_init(int mode, char *path, int size, int level, int shrink)
{
	if (level < LOG_LEVEL0 || level > LOG_LEVEL1
		|| mode < LOG_DISPLAY_MODE || mode > LOG_DB_MODE
		|| shrink < LOG_DISABLE || shrink > LOG_ENABLE
		|| (mode == LOG_FILE_MODE && (size < LOG_FILE_MIN_SIZE || size > LOG_FILE_MAX_SIZE || NULL == path))
		|| (mode == LOG_DB_MODE && (size < LOG_DB_MIN_SIZE || size > LOG_DB_MAX_SIZE || NULL == path)))
		return NULL;

	/* local parameter declare */
	log_t *this = NULL;
	this = (log_t *)malloc(sizeof(log_t));
	if (!this)	
		return NULL;
	memset(this, '\0', sizeof(log_t));

	/* init instant */
	this->mode              = mode;
	this->level             = level;
	switch(mode) {
	case LOG_DISPLAY_MODE:
		this->verify        = log_verify_display;
		this->save          = log_save_display;
		break;
	case LOG_FILE_MODE:
		this->verify        = log_verify_file;
		this->save          = log_save_file;
		this->file_shrink   = shrink;
		this->file_size     = size;
		this->file_path     = (char *)malloc(strlen(path) * sizeof(char));
		if (!this->file_path) {
			free(this);
			return NULL;
		}
		memset(this->file_path, '\0', strlen(path) * sizeof(char));
		strcpy(this->file_path, path);
		break;
	case LOG_DB_MODE:
		this->verify        = log_verify_db;
		this->save          = log_save_db;
		this->db_shrink     = shrink;
		this->db_size       = size;
		this->db_path     = (char *)malloc(strlen(path) * sizeof(char));
		if (!this->db_path) {
			free(this);
			return NULL;
		}
		memset(this->db_path, '\0', strlen(path) * sizeof(char));
		strcpy(this->db_path, path);
		break;
	default:
		break;
	}
	
	return this;
}

/**<
 * @brief apps object destruction
 * @param void
 * @return success:
 */
void mx_log_release(log_t *this)
{
	if (this)
	{
		if (this->file_path)
			free(this->file_path);
		if (this->db_path)
			free(this->db_path);
		free(this);
	}
}


/**<
 * @brief log save function public method
 * @param this ini object instant
 * @param message a bunch of string, support by ANSI C99 format
 * @return success:0 error:1
 */
int mx_log_save(log_t *this, char *fmt, ...)
{
	va_list args;
	int retval;

	va_start(args, fmt);
	retval = this->save(this, fmt, args);
	va_end(args);
	return retval;
}
