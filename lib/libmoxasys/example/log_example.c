#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <debug.h>
#include <log.h>

/*
 * @brief usage print usage routines
 */
void usage()
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "\t-p path  [default is ./logTest\n");
	fprintf(stderr, "\t-m mode  [default is display\n");
	fprintf(stderr, "\t-l level [default is 0\n");
	fprintf(stderr, "\t-t shrink[default is 0\n");
	fprintf(stderr, "\t-s size  [default is 1024\n");
	fprintf(stderr, "\t-h help\n");
	return;
}

/* 
 * @brief daemon entry point
 * @param command argument
 * @return error:1 success:0
 */
int main(int argc, char *argv[])
{
	static int options;
 	extern char *optarg;
	char path[100] = "./logTest"; 
	int mode = 0;
	int level = 0;
	int shrink = 0;
	int size = 50; 

	log_t *log = NULL;

	/* token analysis */
	while((options=getopt(argc,argv,"hl:p:m:t:s:"))!=-1) {
		switch(options) {
			case 'p':
				strcpy(path, optarg);
				break;
			case 'm':
				mode = atoi(optarg);
				break;
			case 'l':
				level = atoi(optarg);
				break;
			case 't':
				shrink = atoi(optarg);
				break;
			case 's':
				size = atoi(optarg);
				break;
			case 'h':
				usage(); 
				exit(1);
				break; 
			default:
				usage();
				exit(1); /* print help without process others opts */
		}
	}

	log = mx_log_init(mode, path, size, level, shrink);
	if (log == NULL) {
		printf("init error.\n");
		exit(1);
	}
	if (mx_log_save(log, "hello world!\n")) {
		printf("save error.\n");
		exit(1);
	}
	mx_log_release(log);

	return 0;
}
