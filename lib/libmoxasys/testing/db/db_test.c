#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "db.h"
#include "ini.h"

int main(int argc, char *argv[])
{
    char db_name[1024];
    int rt_val = 0;
    db_t *mx_db;
    char *tmp = NULL;
    char *fetch_tbname= NULL;
    char *delim = " \t";

    if (argc == 1) {
        fprintf(stderr, "Usage: db_test [db_name] \n");
        exit(1);
    }
    memset(db_name, '\0', sizeof(db_name));
    snprintf(db_name, sizeof(db_name), "%s", argv[1]);
    mx_db = mx_db_init(db_name);
    while(1){
    	/* passive/pull mode to listen DB event */
	tmp = mx_db_listener(mx_db, POLL_DETECT, NULL);
	if (tmp == NULL)
		continue;
	fetch_tbname = strtok(tmp,delim);
	while(fetch_tbname != NULL){
		printf("\n*** Found Table name=%s modified...***\n", fetch_tbname);
		}
		fetch_tbname = strtok (NULL, delim);
	}
	free (tmp);
    }
    mx_db_release(mx_db);
    return 0;
}
