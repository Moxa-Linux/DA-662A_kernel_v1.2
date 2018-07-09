#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <db.h>
#include <ini.h>

/*
 * @brief usage print usage routines
 */
void usage()
{
    fprintf(stderr, "Usage: db_test [db_name] \n");
    exit(1);
}

/*
 * @brief dump data from DB
 * @param sqlite3 statement structure
 */
int dump_data(sqlite3_stmt *stmt)
{
    unsigned char *name;
    int status;
    int rt_val;
    
    rt_val= sqlite3_step(stmt);

    /* No data found*/
    if (rt_val == SQLITE_DONE) {
    	sqlite3_finalize(stmt);
		return 1;
    }
    	
    /* dump column data from each row */
    while (rt_val == SQLITE_ROW) {
    	name = (unsigned char*)sqlite3_column_text(stmt,0);
		status = sqlite3_column_int(stmt, 1);
		printf("%10d %10s \n", status, name);
		rt_val= sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
    return 0;
}

/*
 * @brief daemon entry point
 * @param command argument
 * @return error:1 success:0
 */
int main(int argc, char *argv[])
{
    char db_name[1024];
    sqlite3_stmt *stmt=NULL;
    int rt_val;
    db_t *mx_db;
    static int options;

    char test1[]="SELECT * from mxDeviceTbl";
    char test2[]="INSERT INTO mxDeviceTbl VALUES('mxDeviceTbl2',1);";
    char test3[]="UPDATE mxDeviceTbl set status=0 where name='mxDeviceTbl2';";
    char test4[]="DELETE from mxDeviceTbl where name='mxDeviceTbl2';";
    char test5[]="SELECT * from XXXX";
    char test6[]="SELECT * from mxDeviceTbl where status=987";

    if (argc == 1) {
    	usage();
    }
    memset(db_name, '\0', sizeof(db_name));
    snprintf(db_name, sizeof(db_name), "%s", argv[1]);

    mx_db = mx_db_init(db_name);
    
    /* select data from database */
    rt_val=mx_db_gettbl(mx_db, test1, &stmt);

    if (rt_val != SQLITE_OK){
    	printf("error\n");
	return 1;
    }

    printf("*** Original DB ***\n");
    /* dump data from db */
    dump_data(stmt);
    /* execute sql command */
    mx_db_exec(mx_db,test2);
    /* select data from database */
    rt_val=mx_db_gettbl(mx_db, test1, &stmt);

    if (rt_val != SQLITE_OK){
    	printf("error\n");
	return 1;
    }

    printf("*** INSERT DB ***\n");
    /* dump data from db */
    dump_data(stmt);
    /* execute sql command */
    mx_db_exec(mx_db,test3);
    /* select data from database */
    rt_val=mx_db_gettbl(mx_db, test1, &stmt);

    if (rt_val != SQLITE_OK){
    	printf("error\n");
	return 1;
    }
    printf("*** UPDATE DB ***\n");
    /* dump data from db */
    dump_data(stmt);

    /* execute sql command */
    mx_db_exec(mx_db,test4);
    /* select data from database */
    rt_val=mx_db_gettbl(mx_db, test1, &stmt);

    if (rt_val != SQLITE_OK){
    	printf("error\n");
	return 1;
    }
    printf("*** DELETE DB ***\n");
    /* dump data from db */
    dump_data(stmt);

    /* select error data from database */
    printf("*** Error table name in DB ***\n");
    rt_val = mx_db_gettbl(mx_db, test5, &stmt);

    if (rt_val == SQLITE_ERROR) {
    	printf("SQL command '%s' execution error\n", test5);
	/* return 1 */;
    }

    /* dump data from db */
    dump_data(stmt);

    /* select error data from database */
    printf("*** Error status in DB ***\n");
    rt_val = mx_db_gettbl(mx_db, test6, &stmt);

    if (rt_val == SQLITE_ERROR) {
    	printf("SQL command '%s' execution error\n", test6);
	/* return 1 */;
    }

    rt_val = dump_data(stmt);

    if (rt_val == SQLITE_ERROR) {
    	printf("Cannot find any data with SQL command '%s'\n", test6);
	/* return 1 */;
    }
    mx_db_release(mx_db);
    return 0;
}
