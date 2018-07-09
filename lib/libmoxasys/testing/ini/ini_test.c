#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ini.h"

int main()
{
	ini_t *ini;
	char section[1024] = { 0 };
	char key[1024] = { 0 };
	char result[1024] = { 0 };


	ini = mx_ini_init("mxAgent.ini");
	printf("*******\n");
	strcpy(section, "myTable8");
	strcpy(key, "apps");
	mx_ini_find_key(ini, section, key, result, sizeof(key));
	printf("section: %s\n", section);
	printf("key: %s\n", key);
	printf("value: %s\n", result);

	printf("*******\n");
	strcpy(section, "mxWifiTbl");
	strcpy(key, "apps");
	mx_ini_find_key(ini, section, key, result, sizeof(key));
	printf("section: %s\n", section);
	printf("key: %s\n", key);
	printf("value: %s\n", result);
	printf("*******\n");
	strcpy(key, "ttl");
	mx_ini_find_key(ini, section, key, result, sizeof(key));
	printf("section: %s\n", section);
	printf("key: %s\n", key);
	printf("value: %s\n", result);
	printf("*******\n");

	mx_ini_release(ini);
	return 0;
}

