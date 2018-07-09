#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <log.h>
#include <debug.h>


int main()
{
	log_t *mylog = NULL;
	int flag = 0;
	char file[100] = "./logTest ";

	int a = 1;
	float b = 2.2;
	double c = 3.3;
	char d = 'd';
	char e[] = "eeeee";

	int mode;
	int level;
	int shrink;
	int size = 0;

	while (1) {
		for (mode = LOG_DISPLAY_MODE; mode <= LOG_DB_MODE; mode++) {
			for (level = LOG_LEVEL0; level <= LOG_LEVEL1; level++) {
				for (shrink = LOG_DISABLE; shrink <= LOG_ENABLE; shrink++) {
					/* mx_log_init 
					 * return success:pointer error:NULL
					 */
					if (mode == LOG_FILE_MODE) {
						file[9] = '1';
						size = 1024;
					} else {
						file[9] = '2';
						size = 50;
					}
					mylog = mx_log_init(mode, file, size, level, shrink);
					if (!mylog) {
						fprintf(stderr, "init error!\n");
					}

					/* mx_log_save
					 * return success:0 error:1 file_size_overflow:2 
					 */
					flag = mx_log_save(mylog, "hello! %d, %f, %f, %c, %s\n", a, b, c, d ,e);
					if (flag == 1) {
						fprintf(stderr, "save error!\n");
					}
					else if (flag == 2) {
						fprintf(stderr, "Save error! Full!\n");
					}

					mx_log_release(mylog);
				}
			}
		}
	}

	return 0;
}
