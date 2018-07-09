/*
 *  Template MIB group interface - example.h
 *
 */

/*
 * Don't include ourselves twice 
 */
#ifndef _MIBGROUP_RNAS1200_H
#define _MIBGROUP_RNAS1200_H

#ifdef __cplusplus
extern "C" {
#endif

	/*
	 * We use 'header_generic' from the util_funcs/header_generic module,
	 *  so make sure this module is included in the agent.
	 */
	config_require(util_funcs/header_generic)


	/*
	 * Declare our publically-visible functions.
	 * Typically, these will include the initialization and shutdown functions,
	 *  the main request callback routine and any writeable object methods.
	 *
	 * Function prototypes are provided for the callback routine ('FindVarMethod')
	 *  and writeable object methods ('WriteMethod').
	 */
	void init_rnas1200(void);
	FindVarMethod var_rnas1200;
	WriteMethod rnas1200SetSetting;
	WriteMethod rnas1200SetCommit;
	WriteMethod rnas1200SetQuery;


	/*
	 * Magic number definitions.
	 * These must be unique for each object implemented within a
	 *  single mib module callback routine.
	 *
	 * Typically, these will be the last OID sub-component for
	 *  each entry, or integers incrementing from 1.
	 *  (which may well result in the same values anyway).
	 *
	 * Here, the second and third objects are form a 'sub-table' and
	 *   the magic numbers are chosen to match these OID sub-components.
	 * This is purely for programmer convenience.
	 * All that really matters is that the numbers are unique.
	 */
#define	RNAS1200_SETTING 1
#define	RNAS1200_COMMIT 2
#define	RNAS1200_QUERY 3


#ifdef __cplusplus
}
#endif

#endif                          /* _MIBGROUP_RNAS1200_H */
