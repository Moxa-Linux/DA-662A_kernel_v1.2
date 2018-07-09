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
    sqlite3_stmt *stmt=NULL;
    int rt_val;
    db_t *mx_db;

    unsigned char *street;
    int id;
    if (argc == 1) {
        fprintf(stderr, "Usage: db_test [db_name] \n");
        exit(1);
    }
    memset(db_name, '\0', sizeof(db_name));
    snprintf(db_name, sizeof(db_name), "%s", argv[1]);
    mx_db = mx_db_init(db_name);
    
    char tmp[]="select * from mxDeviceTbl";
    rt_val=mx_db_gettbl(mx_db, tmp, &stmt);

    if (rt_val != SQLITE_OK)
	printf("error\n");

	    while(sqlite3_step(stmt)==SQLITE_ROW ) {
		    id = sqlite3_column_int(stmt, 1);
		    street = (unsigned char*)sqlite3_column_text(stmt,0);
		    printf("%10d %10s \n", id, street);
	    }


    sqlite3_finalize(stmt);

    char tmp3[]="UPDATE mxDeviceTbl set status=0 where name='mxDeviceTbl2';";
    char tmp2[]="INSERT INTO mxDeviceTbl VALUES('mxDeviceTbl2',1);";
    mx_db_exec(mx_db,tmp2);
    rt_val=mx_db_gettbl(mx_db, tmp, &stmt);

    if (rt_val != SQLITE_OK)
	printf("error\n");

	    while(sqlite3_step(stmt)==SQLITE_ROW ) {
		    id = sqlite3_column_int(stmt, 1);
		    street = (unsigned char*)sqlite3_column_text(stmt,0);
		    printf("%10d %10s \n", id, street);
	    }


    sqlite3_finalize(stmt);
    mx_db_exec(mx_db,tmp3);
    rt_val=mx_db_gettbl(mx_db, tmp, &stmt);

    if (rt_val != SQLITE_OK)
	printf("error\n");

	    while(sqlite3_step(stmt)==SQLITE_ROW ) {
		    id = sqlite3_column_int(stmt, 1);
		    street = (unsigned char*)sqlite3_column_text(stmt,0);
		    printf("%10d %10s \n", id, street);
	    }


    sqlite3_finalize(stmt);
    rt_val=mx_db_gettbl(mx_db, tmp, &stmt);

    mx_db_release(mx_db);
    return 0;
}
