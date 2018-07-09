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
#include "uc1260.h"


/*
 *  Certain objects can be set via configuration file directives.
 *  These variables hold the values for such objects, as they need to
 *   be accessible to both the config handlers, and the callback routine.
 */
#define UC1260_STR_LEN 300
#define UC1260_COMMITSTR_LEN 3
#define UC1260_MANUFACTURER_NAME_CONTENT "MOXA"
#define UC1260_VERSION_CONTENT "2.0"
#define UC1260_MODEL_NUMBER_CONTENT "FLI-1260"
#define UC1260_TRAP_SERVER_EXCEPTION_CONTENT "FLI server exceptions."
#define UC1260_TRAP_TOPOLOGY_MAPPING_EXCEPTION_CONTENT "Network topology mapping exceptions."

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
struct variable2 uc1260_variables[] = {
	{UC1260_MANUFACTURER_NAME, ASN_OCTET_STR, NETSNMP_OLDAPI_RONLY,
		var_uc1260, 2, { 1, 1}},
	{UC1260_VERSION, ASN_OCTET_STR, NETSNMP_OLDAPI_RONLY,
		var_uc1260, 2, { 1, 2}},
	{UC1260_MODEL_NUMBER, ASN_OCTET_STR, NETSNMP_OLDAPI_RONLY,
		var_uc1260, 2, { 1, 5}},
	{UC1260_TRAP_SERVER_EXCEPTION, ASN_OCTET_STR, NETSNMP_OLDAPI_RONLY,
		var_uc1260, 2, { 2, 1}},
	{UC1260_TRAP_TOPOLOGY_MAPPING_EXCEPTION, ASN_OCTET_STR, NETSNMP_OLDAPI_RONLY,
		var_uc1260, 2, { 2, 2}}
};

/*
 * This array defines the OID of the top of the mib tree that we're
 *  registering underneath.
 * Note that this needs to be the correct size for the OID being 
 *  registered, so that the length of the OID can be calculated.
 *  The format given here is the simplest way to achieve this.
 */
oid          uc1260_variables_oid[] = { 1, 3, 6, 1, 4, 1, 8691, 404, 1 };


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
init_uc1260(void)
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
	REGISTER_MIB("uc1260", uc1260_variables, variable2, uc1260_variables_oid);
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
var_uc1260(struct variable *vp,
		oid * name,
		size_t * length,
		int exact, size_t * var_len, WriteMethod ** write_method)
{
	/*
	 *  The result returned from this function needs to be a pointer to
	 *    static data (so that it can be accessed from outside).
	 *  Define suitable variables for any type of data we may return.
	 */
	static char string[UC1260_STR_LEN];    /* for UC1260STRING   */
	memset(string, '\0', UC1260_STR_LEN);

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
	DEBUGMSGTL(("uc1260", "var_uc1260 entered\n"));
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
	case UC1260_MANUFACTURER_NAME:
		strcpy(string, UC1260_MANUFACTURER_NAME_CONTENT);
		*var_len = strlen(string);
		return (u_char *) string;

	case UC1260_VERSION:
		strcpy(string, UC1260_VERSION_CONTENT);
		*var_len = strlen(string);
		return (u_char *) string;

	case UC1260_MODEL_NUMBER:
		strcpy(string, UC1260_MODEL_NUMBER_CONTENT);
		*var_len = strlen(string);
		return (u_char *) string;

	case UC1260_TRAP_SERVER_EXCEPTION:
		strcpy(string, UC1260_TRAP_SERVER_EXCEPTION_CONTENT);
		*var_len = strlen(string);
		return (u_char *) string;

	case UC1260_TRAP_TOPOLOGY_MAPPING_EXCEPTION:
		strcpy(string, UC1260_TRAP_TOPOLOGY_MAPPING_EXCEPTION_CONTENT);
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
		DEBUGMSGTL(("snmpd", "unknown sub-id %d in uc1260s/var_uc1260\n",
					vp->magic));
	}
	/*
	 * If we fall through to here, fail by returning NULL.
	 * This is essentially a continuation of the 'default' case above.
	 */
	return NULL;
}

