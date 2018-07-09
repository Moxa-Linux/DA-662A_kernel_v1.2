#ifndef OS_HEADER_LOG
#define OS_HEADER_LOG

#include <log.h>

/**<
 * @brief log save function public method of mode "display"
 * @param this ini object instant
 * @param message a bunch of string, support by ANSI C99 format
 * @return success:0 error:1
 * 
 * log_save function used by other object to save log messege.
 */
int log_save_display(log_t *this, char *fmt, va_list args);

/**<
 * @brief log save function public method of mode "file"
 * @param this ini object instant
 * @param message a bunch of string, support by ANSI C99 format
 * @return success:0 error:1
 * 
 * log_save function used by other object to save log messege.
 */
int log_save_file(log_t *this, char *fmt, va_list args);

/**<
 * @brief log save function public method of mode "db"
 * @param this ini object instant
 * @param message a bunch of string, support by ANSI C99 format
 * @return success:0 error:1
 * 
 * log_save function used by other object to save log messege.
 */
int log_save_db(log_t *this, char *fmt, va_list args);

/**<
 * @brief log verify resource function private method of mode "display"
 * @param this log_len
 * @return success:0 error:1
 */
int log_verify_display(log_t *this, int log_len);

/**<
 * @brief log verify resource function private method of mode "file"
 * @param this log_len
 * @return success:0 error:1
 */
int log_verify_file(log_t *this, int log_len);
/**<
 * @brief log verify resource function private method of mode "db"
 * @param this log_len
 * @return success:0 error:1
 */
int log_verify_db(log_t *this, int log_len);
#endif
