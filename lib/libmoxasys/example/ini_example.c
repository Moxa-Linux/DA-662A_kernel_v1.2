#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <debug.h>
#include <ini.h>

/*
 * @brief usage print usage routines
 */
void usage()
{
    fprintf(stderr, "Usage: ini_example [ini_file] \n");
    exit(1);
}

/* 
 * @brief daemon entry point
 * @param command argument
 * @return error:1 success:0
 */
int main(int argc, char *argv[])
{
	ini_t *ini;
	char ini_name[1024];
	char section[1024] = { 0 };
	char key[1024] = { 0 };
	char result[1024] = { 0 };
	int rt_val=0;

	if (argc == 1) {
		usage();
	}
	memset(ini_name, '\0', sizeof(ini_name));
	snprintf(ini_name, sizeof(ini_name), "%s", argv[1]);

	/* initialize ini object */
	ini = mx_ini_init(ini_name);

	/* assign section & key */
	strcpy(section, "myTable8");
	strcpy(key, "apps");

	/* find and get mapping value */
	mx_ini_find_key(ini, section, key, result, sizeof(result));

	/* show the result */
	printf("*******************\n");
	printf("section: %s, key:%s\n", section, key);
	printf("value: %s\n", result);
	printf("*******************\n");


	/* assign section & key */
	strcpy(section, "mxDeviceTbl");
	strcpy(key, "ttl");

	/* find and get mapping value */
	mx_ini_find_key(ini, section, key, result, sizeof(result));

	/* show the result */
	printf("*******************\n");
	printf("section: %s, key:%s\n", section, key);
	printf("value: %s\n", result);
	printf("*******************\n");


	/* assign section & key */
	strcpy(section, "ggg");
	strcpy(key, "apps");

	/* find and get mapping value */
	rt_val = mx_ini_find_key(ini, section, key, result, sizeof(result));

	if (rt_val ==0) {
		/* show the result */
		printf("*******************\n");
		printf("section: %s, key:%s\n", section, key);
		printf("value: %s\n", result);
		printf("*******************\n");
	}
	else
		printf("Error in section name\n");

	/* release ini */
	mx_ini_release	(ini);

	return 0;
}
