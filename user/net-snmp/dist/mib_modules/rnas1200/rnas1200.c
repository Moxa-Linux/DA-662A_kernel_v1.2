/*
 *  Template MIB group implementation - example.c
 *
 */

/*
 * include important headers 
 */
#include <net-snmp/net-snmp-config.h>
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif

#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/*
 * header_generic() comes from here 
 */
#include "util_funcs/header_generic.h"

/*
 * include our .h file 
 */
#include "rnas1200.h"
#include "dataContainer.h"
#include "productContainer.h"
#include "filterContainer.h"
#include "sqlDb.h"
#include "misc.h"
#include "bussinessLogic.h"


/*
 *  Certain objects can be set via configuration file directives.
 *  These variables hold the values for such objects, as they need to
 *   be accessible to both the config handlers, and the callback routine.
 */
#define SQL_DB_PATH "/dev/shm/moxa.db"
#define RNAS1200_STR_LEN 300
#define RNAS1200_TIMEOUT 900
#define RNAS1200_COMMITSTR_LEN 3
#define RNAS1200_SETTING_CONTENT "Not avaliable."
static char rnas1200_commit_content[RNAS1200_COMMITSTR_LEN] = "0\0";
static char rnas1200_query_content[RNAS1200_STR_LEN] = "\0";
static dataContainer *rnas1200DataContainer = NULL;
extern productContainer rnas1200Container[];

/*********************
 *
 *  Initialisation & common implementation functions
 *
 *********************/

/*
 * This array structure defines a representation of the
 *  MIB being implemented.
 *
 * The type of the array is 'struct variableN', where N is
 *  large enough to contain the longest OID sub-component
 *  being loaded.  This will normally be the maximum value
 *  of the fifth field in each line.  In this case, the second
 *  and third entries are both of size 2, so we're using
 *  'struct variable2'
 *
 * The supported values for N are listed in <agent/var_struct.h>
 *  If the value you need is not listed there, simply use the
 *  next largest that is.
 *
 * The format of each line is as follows
 *  (using the first entry as an example):
 *      1: EXAMPLESTRING:
 *          The magic number defined in the example header file.
 *          This is passed to the callback routine and is used
 *            to determine which object is being queried.
 *      2: ASN_OCTET_STR:
 *          The type of the object.
 *          Valid types are listed in <snmp_impl.h>
 *      3: NETSNMP_OLDAPI_RONLY (or NETSNMP_OLDAPI_RWRITE):
 *          Whether this object can be SET or not.
 *      4: var_example:
 *          The callback routine, used when the object is queried.
 *          This will usually be the same for all objects in a module
 *            and is typically defined later in this file.
 *      5: 1:
 *          The length of the OID sub-component (the next field)
 *      6: {1}:
 *          The OID sub-components of this entry.
 *          In other words, the bits of the full OID that differ
 *            between the various entries of this array.
 *          This value is appended to the common prefix (defined later)
 *            to obtain the full OID of each entry.
 */
struct variable1 rnas1200_variables[] = {
	{RNAS1200_SETTING, ASN_OCTET_STR, NETSNMP_OLDAPI_RWRITE,
		var_rnas1200, 1, {1}},
	{RNAS1200_COMMIT, ASN_OCTET_STR, NETSNMP_OLDAPI_RWRITE,
		var_rnas1200, 1, {2}},
	{RNAS1200_QUERY, ASN_OCTET_STR, NETSNMP_OLDAPI_RWRITE,
		var_rnas1200, 1, {3}}
};

/*
 * This array defines the OID of the top of the mib tree that we're
 *  registering underneath.
 * Note that this needs to be the correct size for the OID being 
 *  registered, so that the length of the OID can be calculated.
 *  The format given here is the simplest way to achieve this.
 */
oid          rnas1200_variables_oid[] = { 1, 3, 6, 1, 4, 1, 8691, 12, 1200, 1 };


/*
 * This function is called at the time the agent starts up
 *  to do any initializations that might be required.
 *
 * In theory it is optional and can be omitted if no
 *  initialization is needed.  In practise, every module
 *  will need to register itself (or the objects being
 *  implemented will not appear in the MIB tree), and this
 *  registration is typically done here.
 *
 * If this function is added or removed, you must re-run
 *  the configure script, to detect this change.
 */
void
init_rnas1200(void)
{
	/*
	 * Register ourselves with the agent to handle our mib tree.
	 * The arguments are:
	 *    descr:   A short description of the mib group being loaded.
	 *    var:     The variable structure to load.
	 *                  (the name of the variable structure defined above)
	 *    vartype: The type of this variable structure
	 *    theoid:  The OID pointer this MIB is being registered underneath.
	 */
	REGISTER_MIB("rnas1200", rnas1200_variables, variable1, rnas1200_variables_oid);
}

/*********************
 *
 *  System specific implementation functions
 *
 *********************/

/*
 * Define the callback function used in the example_variables structure.
 * This is called whenever an incoming request refers to an object
 *  within this sub-tree.
 *
 * Four of the parameters are used to pass information in.
 * These are:
 *    vp      The entry from the 'example_variables' array for the
 *             object being queried.
 *    name    The OID from the request.
 *    length  The length of this OID.
 *    exact   A flag to indicate whether this is an 'exact' request
 *             (GET/SET) or an 'inexact' one (GETNEXT/GETBULK).
 *
 * Four of the parameters are used to pass information back out.
 * These are:
 *    name     The OID being returned.
 *    length   The length of this OID.
 *    var_len  The length of the answer being returned.
 *    write_method   A pointer to the SET function for this object.
 *
 * Note that name & length serve a dual purpose in both roles.
 */

u_char         *
var_rnas1200(struct variable *vp,
		oid * name,
		size_t * length,
		int exact, size_t * var_len, WriteMethod ** write_method)
{
	/*
	 *  The result returned from this function needs to be a pointer to
	 *    static data (so that it can be accessed from outside).
	 *  Define suitable variables for any type of data we may return.
	 */
	static char string[RNAS1200_STR_LEN];    /* for RNAS1200STRING   */
	memset(string, '\0', RNAS1200_STR_LEN);

	/*
	 * Before returning an answer, we need to check that the request
	 *  refers to a valid instance of this object.  The utility routine
	 *  'header_generic' can be used to do this for scalar objects.
	 *
	 * This routine 'header_simple_table' does the same thing for "simple"
	 *  tables. (See the AGENT.txt file for the definition of a simple table).
	 *
	 * Both these utility routines also set up default values for the
	 *  return arguments (assuming the check succeeded).
	 * The name and length are set suitably for the current object,
	 *  var_len assumes that the result is an integer of some form,
	 *  and write_method assumes that the object cannot be set.
	 *
	 * If these assumptions are correct, this callback routine simply
	 * needs to return a pointer to the appropriate value (using 'long_ret').
	 * Otherwise, 'var_len' and/or 'write_method' should be set suitably.
	 */
	//DEBUGMSGTL(("rnas1200", "var_rnas1200 entered\n"));
	if (header_generic(vp, name, length, exact, var_len, write_method) ==
			MATCH_FAILED)
		return NULL;


	/*
	 * Many object will need to obtain data from the operating system in
	 *  order to return the appropriate value.  Typically, this is done
	 *  here - immediately following the 'header' call, and before the
	 *  switch statement. This is particularly appropriate if a single 
	 *  interface call can return data for all the objects supported.
	 *
	 * This example module does not rely on external data, so no such
	 *  calls are needed in this case.  
	 */

	/*
	 * Now use the magic number from the variable pointer 'vp' to
	 *  select the particular object being queried.
	 * In each case, one of the static objects is set up with the
	 *  appropriate information, and returned mapped to a 'u_char *'
	 */
	switch (vp->magic) {
	case RNAS1200_SETTING:
		*write_method = rnas1200SetSetting;
		strcpy(string, RNAS1200_SETTING_CONTENT);
		*var_len = strlen(string);
		return (u_char *) string;

	case RNAS1200_COMMIT:
		*write_method = rnas1200SetCommit;
		strcpy(string, rnas1200_commit_content);
		*var_len = strlen(string);
		return (u_char *) string;

	case RNAS1200_QUERY:
		*write_method = rnas1200SetQuery;
		strcpy(string, rnas1200_query_content);
		*var_len = strlen(string);
		return (u_char *) string;

	default:
		/*
		 *  This will only be triggered if there's a problem with
		 *   the coding of the module.  SNMP requests that reference
		 *   a non-existant OID will be directed elsewhere.
		 *  If this branch is reached, log an error, so that
		 *   the problem can be investigated.
		 */
		DEBUGMSGTL(("snmpd", "unknown sub-id %d in rnas1200s/var_rnas1200\n",
					vp->magic));
	}
	/*
	 * If we fall through to here, fail by returning NULL.
	 * This is essentially a continuation of the 'default' case above.
	 */
	return NULL;
}

/*********************
 *
 *  Writeable object SET handling routines
 *
 *********************/
int
rnas1200SetSetting(int action,
		u_char * var_val,
		u_char var_val_type,
		size_t var_val_len,
		u_char * statP, oid * name, size_t name_len)
{
	/* define data container variable/buffer */
	char tableName[RNAS1200_STR_LEN + 1];
	char colName[RNAS1200_STR_LEN + 1];
	char rowId[RNAS1200_STR_LEN + 1];
	char value[RNAS1200_STR_LEN + 1];
	static dataContainer *node = NULL;
	/* define sql database variable */
	static sqlite3 *sqlDb = NULL;
	/* define misc variable */
	int errNum;


	switch (action) {
	case RESERVE1:
		DEBUGMSGTL(("rnas1200", "-----> %s, %s, RESERVE1:%s\n", __FILE__, __FUNCTION__, var_val));
		/*
		 *  Check that the value being set is acceptable
		 */
		if (var_val_type != ASN_OCTET_STR) {
			DEBUGMSGTL(("rnas1200", "%x not string type\n", var_val_type));
			return SNMP_ERR_WRONGTYPE;
		}
		if (var_val_len > RNAS1200_STR_LEN) {
			DEBUGMSGTL(("rnas1200", "wrong length %u\n", var_val_len));
			return SNMP_ERR_WRONGLENGTH;
		}

		break;

	case RESERVE2:
		//DEBUGMSGTL(("rnas1200", "-----> %s, %s, RESERVE2:\n", __FILE__, __FUNCTION__));
		/*
		 *  This is conventially where any necesary
		 *   resources are allocated (e.g. calls to malloc)
		 *  Here, we are using static variables
		 *   so don't need to worry about this.
		 */

		/* check rnas1200_commit_content */
		if (atoi(rnas1200_commit_content) != 0) {
			DEBUGMSGTL(("rnas1200", "rnas1200 is busy.\n"));
			return SNMP_ERR_RESOURCEUNAVAILABLE;
		}
		/* get data from string */
		if (getData(var_val, tableName, colName, rowId, value, &isSetFormat)) {
			DEBUGMSGTL(("rnas1200", "wrong string format, it should be [*.*.*=*]\n"));
			return SNMP_ERR_BADVALUE;
		}
		/* create data container node */
		node = dataCreateContainerNode(tableName, colName, rowId, value);
		if (NULL == node) {
			DEBUGMSGTL(("rnas1200", "not enough memory\n"));
			return SNMP_ERR_RESOURCEUNAVAILABLE;
		}
		/* check sql database and data container node */
		sqlDb = sqlOpenDb(SQL_DB_PATH);
		if (NULL == sqlDb) {
			DEBUGMSGTL(("rnas1200", "failed to open database\n"));
			dataFreeContainerNode(node);
			return SNMP_ERR_RESOURCEUNAVAILABLE;
		}
		if (sqlAccessDbItem(sqlDb, node) != 0 || checkData(rnas1200Container, node) != 0) {
			DEBUGMSGTL(("rnas1200", "data is not avaiable\n"));
			sqlCloseDb(sqlDb);
			dataFreeContainerNode(node);
			return SNMP_ERR_WRONGVALUE;
		}
		sqlCloseDb(sqlDb);

		break;

	case FREE:
		//DEBUGMSGTL(("rnas1200", "-----> %s, %s, FREE:\n", __FILE__, __FUNCTION__));
		/*
		 *  This is where any of the above resources
		 *   are freed again (because one of the other
		 *   values being SET failed for some reason).
		 *  Again, since we are using static variables
		 *   we don't need to worry about this either.
		 */
		break;

	case ACTION:
		//DEBUGMSGTL(("rnas1200", "-----> %s, %s, ACTION:\n", __FILE__, __FUNCTION__));
		/*
		 *  Set the variable as requested.
		 *   Note that this may need to be reversed,
		 *   so save any information needed to do this.
		 */

		/* insert data container node to the last */
		rnas1200DataContainer = dataInsertContainerNode(rnas1200DataContainer, node, dataGetContainerLen(rnas1200DataContainer) + 1, &errNum);
		if (errNum) {
			DEBUGMSGTL(("rnas1200", "wrong dataContainer node or insert index\n"));
			dataFreeContainerNode(node);
			return SNMP_ERR_WRONGVALUE;
		}
		/* Remove timeout and duplicate data */
		rnas1200DataContainer = dataRemoveContainerTimeout(rnas1200DataContainer, RNAS1200_TIMEOUT);
		rnas1200DataContainer = dataRemoveContainerDuplicate(rnas1200DataContainer, DATA_TABLENAME | DATA_COLNAME | DATA_ROWID);

		break;

	case UNDO:
		//DEBUGMSGTL(("rnas1200", "-----> %s, %s, UNDO:\n", __FILE__, __FUNCTION__));
		/*
		 *  Something failed, so re-set the
		 *   variable to its previous value
		 *  (and free any allocated resources).
		 */
		break;

	case COMMIT:
		//DEBUGMSGTL(("rnas1200", "-----> %s, %s, COMMIT:\n", __FILE__, __FUNCTION__));
		/*
		 *  Everything worked, so we can discard any
		 *   saved information, and make the change
		 *   permanent (e.g. write to the config file).
		 *  We also free any allocated resources.
		 *
		 *  In this case, there's nothing to do.
		 */
		break;

	}
	return SNMP_ERR_NOERROR;
}

int
rnas1200SetCommit(int action,
		u_char * var_val,
		u_char var_val_type,
		size_t var_val_len,
		u_char * statP, oid * name, size_t name_len)
{
	/* defile variables */
	static sqlite3 *sqlDb = NULL;
	dataContainer *curr = NULL;

	switch (action) {
	case RESERVE1:
		DEBUGMSGTL(("rnas1200", "-----> %s, %s, RESERVE1:%s\n", __FILE__, __FUNCTION__, var_val));
		/*
		 *  Check that the value being set is acceptable
		 */
		if (var_val_type != ASN_OCTET_STR) {
			DEBUGMSGTL(("rnas1200", "%x not string type\n", var_val_type));
			return SNMP_ERR_WRONGTYPE;
		}
		if (var_val_len > RNAS1200_COMMITSTR_LEN - 1) {
			DEBUGMSGTL(("rnas1200", "wrong length %u\n", var_val_len));
			return SNMP_ERR_WRONGLENGTH;
		}
		break;

	case RESERVE2:
		//DEBUGMSGTL(("rnas1200", "-----> %s, %s, RESERVE2:\n", __FILE__, __FUNCTION__));
		/*
		 *  This is conventially where any necesary
		 *   resources are allocated (e.g. calls to malloc)
		 *  Here, we are using static variables
		 *   so don't need to worry about this.
		 */

		/* check rnas1200_commit_content */
		if (atoi(rnas1200_commit_content) != 0) {
			DEBUGMSGTL(("rnas1200", "rnas1200 is busy.\n"));
			return SNMP_ERR_RESOURCEUNAVAILABLE;
		}
		/* check var_val */
		if (atoi(var_val) != 1) {
			DEBUGMSGTL(("rnas1200", "wrong value\n"));
			return SNMP_ERR_BADVALUE;
		}
		/* open sql database */
		sqlDb = sqlOpenDb(SQL_DB_PATH);
		if (NULL == sqlDb) {
			DEBUGMSGTL(("rnas1200", "failed to open database\n"));
			return SNMP_ERR_RESOURCEUNAVAILABLE;
		}

		break;

	case FREE:
		//DEBUGMSGTL(("rnas1200", "-----> %s, %s, FREE:\n", __FILE__, __FUNCTION__));
		/*
		 *  This is where any of the above resources
		 *   are freed again (because one of the other
		 *   values being SET failed for some reason).
		 *  Again, since we are using static variables
		 *   we don't need to worry about this either.
		 */
		break;

	case ACTION:
		//DEBUGMSGTL(("rnas1200", "-----> %s, %s, ACTION:\n", __FILE__, __FUNCTION__));
		/*
		 *  Set the variable as requested.
		 *   Note that this may need to be reversed,
		 *   so save any information needed to do this.
		 */

		/* set rnas1200_commit_content */
		sprintf(rnas1200_commit_content, "%d\0", var_val);
		/* business logic
		 *  1) Remove timeout data
		 *  2) Remove duplicate data (choose the last data user insert, then remove others)
		 *  3) networkResources ("ip"/"gateway"/"subnet" can't be any value from 0 to 255)
		 *  4) networkResources ("ip"/"gateway"/"subnet" should match)
		 *  5) networkResources ("defaultGateway" should on only one port)
		 */
		rnas1200DataContainer = dataRemoveContainerTimeout(rnas1200DataContainer, RNAS1200_TIMEOUT);
		rnas1200DataContainer = dataRemoveContainerDuplicate(rnas1200DataContainer, DATA_TABLENAME | DATA_COLNAME | DATA_ROWID);
		rnas1200DataContainer = businessLogicFilterNetmask(rnas1200DataContainer);
		rnas1200DataContainer = businessLogicFilterIp(rnas1200DataContainer);
		rnas1200DataContainer = businessLogicFilterNetwork(sqlDb, rnas1200DataContainer);
		rnas1200DataContainer = businessLogicFilterDefaultGateway(sqlDb, rnas1200DataContainer);
		/* Write data into sql database 
		 * If database is busy then keep data in rnas1200DataContainer which become drop list */
		rnas1200DataContainer = writeData(sqlDb, rnas1200DataContainer);
		/* free resources */
		sqlCloseDb(sqlDb);
		/* clear rnas1200_commit_content */
		sprintf(rnas1200_commit_content, "0\0");
		/* report drop list */
		if (dataGetContainerLen(rnas1200DataContainer)) {
			DEBUGMSGTL(("rnas1200", "database is busy for some table\n"));
			curr = rnas1200DataContainer;
			while (curr != NULL) {
				DEBUGMSGTL(("rnas1200", "Table '%s' is dropped\n", dataGetContainerTableName(curr)));
				curr = curr->next;
			}
			rnas1200DataContainer = dataFreeContainer(rnas1200DataContainer);
			return SNMP_ERR_RESOURCEUNAVAILABLE;
		} 

		break;

	case UNDO:
		//DEBUGMSGTL(("rnas1200", "-----> %s, %s, UNDO:\n", __FILE__, __FUNCTION__));
		/*
		 *  Something failed, so re-set the
		 *   variable to its previous value
		 *  (and free any allocated resources).
		 */
		break;

	case COMMIT:
		//DEBUGMSGTL(("rnas1200", "-----> %s, %s, COMMIT:\n", __FILE__, __FUNCTION__));
		/*
		 *  Everything worked, so we can discard any
		 *   saved information, and make the change
		 *   permanent (e.g. write to the config file).
		 *  We also free any allocated resources.
		 *
		 *  In this case, there's nothing to do.
		 */
		break;

	}
	return SNMP_ERR_NOERROR;
}

int
rnas1200SetQuery(int action,
		u_char * var_val,
		u_char var_val_type,
		size_t var_val_len,
		u_char * statP, oid * name, size_t name_len)
{
	/* define data container variable/buffer */
	char tableName[RNAS1200_STR_LEN + 1];
	char colName[RNAS1200_STR_LEN + 1];
	char rowId[RNAS1200_STR_LEN + 1];
	char value[RNAS1200_STR_LEN + 1];
	static dataContainer *node = NULL;
	/* define sql database variable */
	static sqlite3 *sqlDb = NULL;
	/* define misc variable */
	char *res = NULL;

	switch (action) {
	case RESERVE1:
		DEBUGMSGTL(("rnas1200", "-----> %s, %s, RESERVE1:%s\n", __FILE__, __FUNCTION__, var_val));
		/*
		 *  Check that the value being set is acceptable
		 */
		if (var_val_type != ASN_OCTET_STR) {
			DEBUGMSGTL(("rnas1200", "%x not string type\n", var_val_type));
			return SNMP_ERR_WRONGTYPE;
		}
		if (var_val_len > RNAS1200_STR_LEN) {
			DEBUGMSGTL(("rnas1200", "wrong length %u\n", var_val_len));
			return SNMP_ERR_WRONGLENGTH;
		}

		break;

	case RESERVE2:
		//DEBUGMSGTL(("rnas1200", "-----> %s, %s, RESERVE2:\n", __FILE__, __FUNCTION__));
		/*
		 *  This is conventially where any necesary
		 *   resources are allocated (e.g. calls to malloc)
		 *  Here, we are using static variables
		 *   so don't need to worry about this.
		 */


		/* get data from string */
		if (getData(var_val, tableName, colName, rowId, value, &isQueryFormat)) {
			DEBUGMSGTL(("rnas1200", "wrong string format, it should be [*.*.*]\n"));
			return SNMP_ERR_BADVALUE;
		}
		/* create data container node */
		node = dataCreateContainerNode(tableName, colName, rowId, value);
		if (NULL == node) {
			DEBUGMSGTL(("rnas1200", "not enough memory\n"));
			return SNMP_ERR_RESOURCEUNAVAILABLE;
		}
		/* open sql database */
		sqlDb = sqlOpenDb(SQL_DB_PATH);
		if (NULL == sqlDb) {
			DEBUGMSGTL(("rnas1200", "failed to open database\n"));
			dataFreeContainerNode(node);
			return SNMP_ERR_RESOURCEUNAVAILABLE;
		}

		break;

	case FREE:
		//DEBUGMSGTL(("rnas1200", "-----> %s, %s, FREE:\n", __FILE__, __FUNCTION__));
		/*
		 *  This is where any of the above resources
		 *   are freed again (because one of the other
		 *   values being SET failed for some reason).
		 *  Again, since we are using static variables
		 *   we don't need to worry about this either.
		 */
		break;

	case ACTION:
		//DEBUGMSGTL(("rnas1200", "-----> %s, %s, ACTION:\n", __FILE__, __FUNCTION__));
		/*
		 *  Set the variable as requested.
		 *   Note that this may need to be reversed,
		 *   so save any information needed to do this.
		 */

		/* Read item from sql database
		 * If there's no this item it will clear "rnas1200_query_content" */
		memset(rnas1200_query_content, '\0', sizeof(rnas1200_query_content));
		res = sqlGetDbItemValue(sqlDb, node);
		if (NULL == res) {
			DEBUGMSGTL(("rnas1200", "wrong value or rnas1200 is busy\n"));
			dataFreeContainerNode(node);
			sqlCloseDb(sqlDb);
			return SNMP_ERR_BADVALUE;
		}
		strcpy(rnas1200_query_content, res);
		/* free resources */
		free(res);
		dataFreeContainerNode(node);
		sqlCloseDb(sqlDb);

		break;

	case UNDO:
		//DEBUGMSGTL(("rnas1200", "-----> %s, %s, UNDO:\n", __FILE__, __FUNCTION__));
		/*
		 *  Something failed, so re-set the
		 *   variable to its previous value
		 *  (and free any allocated resources).
		 */
		break;

	case COMMIT:
		//DEBUGMSGTL(("rnas1200", "-----> %s, %s, COMMIT:\n", __FILE__, __FUNCTION__));
		/*
		 *  Everything worked, so we can discard any
		 *   saved information, and make the change
		 *   permanent (e.g. write to the config file).
		 *  We also free any allocated resources.
		 *
		 *  In this case, there's nothing to do.
		 */
		break;

	}
	return SNMP_ERR_NOERROR;
}

