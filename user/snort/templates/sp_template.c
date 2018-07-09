/* $Id: sp_template.c,v 1.3 2002/05/15 01:28:35 chrisgreen Exp $ */
/* Snort Detection Plugin Source File Template */

/* sp_template 
 * 
 * Purpose:
 *
 * Detection engine plugins test an aspect of the current packet and report
 * their findings.  The function may be called many times per packet with 
 * different arguments.  These functions are acccessed from the rules file
 * as standard rule options.  When adding a plugin to the system, be sure to 
 * add the "Setup" function to the InitPlugins() function call in 
 * plugbase.c!
 *
 * Arguments:
 *   
 * This is the type of arguements that the detection plugin can take when
 * referenced as a rule option
 *
 * Effect:
 *
 * What the plugin does.  
 *
 * Comments:
 *
 * Any comments?
 *
 */

#include <sys/types.h>
#include <stdlib.h>
#include <ctype.h>

#include "rules.h"
#include "decode.h"
#include "plugbase.h"
#include "parser.h"
#include "debug.h"
#include "util.h"
#include "plugin_enum.h"

/* don't forget to include the name of this file in plugbase.h! */

extern char *file_name;  /* this is the file name from rules.c, generally used
                            for error messages */

extern int file_line;    /* this is the file line number from rules.c that is
                            used to indicate file lines for error messages */

typedef struct _TemplateData
{
    /* your data structure info goes here */

} TemplateData;

void TemplateInit(char *, OptTreeNode *, int);
void TemplateRuleParseFunction(char *, OptTreeNode *);
int TemplateDetectorFunction(Packet *, struct _OptTreeNode *, OptFpList *);

/****************************************************************************
 * 
 * Function: SetupTemplate()
 *
 * Purpose: Generic detection engine plugin template.  Registers the
 *          configuration function and links it to a rule keyword.  This is
 *          the function that gets called from InitPlugins in plugbase.c.
 *
 * Arguments: None.
 *
 * Returns: void function
 *
 ****************************************************************************/
void SetupTemplate()
{
    /* map the keyword to an initialization/processing function */
    RegisterPlugin("keyword", TemplateInit);

    DebugMessage(DEBUG_PLUGIN,"Plugin: TemplateName Setup\n");
}


/****************************************************************************
 * 
 * Function: TemplateInit(char *, OptTreeNode *)
 *
 * Purpose: Generic rule configuration function.  Handles parsing the rule 
 *          information and attaching the associated detection function to
 *          the OTN.
 *
 * Arguments: data => rule arguments/data
 *            otn => pointer to the current rule option list node
 *
 * Returns: void function
 *
 ****************************************************************************/
void TemplateInit(char *data, OptTreeNode *otn, int protocol)
{
    /* allocate the data structure and attach it to the
       rule's data struct list */
    otn->ds_list[PLUGIN_TEMPLATE_NUMBER] = (TemplateData *) calloc(sizeof(TemplateData), sizeof(char));

    /* be sure to check that the protocol that is passed in matches the
       transport layer protocol that you're using for this rule! */

    /* this is where the keyword arguments are processed and placed into the 
       rule option's data structure */
    TemplateRuleParseFunction(data, otn);

    /* finally, attach the option's detection function to the rule's 
       detect function pointer list */
    AddOptFuncToList(TemplateDetectorFunction, otn);
}



/****************************************************************************
 * 
 * Function: TemplateRuleParseFunction(char *, OptTreeNode *)
 *
 * Purpose: This is the function that is used to process the option keyword's
 *          arguments and attach them to the rule's data structures.
 *
 * Arguments: data => argument data
 *            otn => pointer to the current rule's OTN
 *
 * Returns: void function
 *
 ****************************************************************************/
void TemplateRuleParseFunction(char *data, OptTreeNode *otn)
{
    TemplateData *ds_ptr;  /* data struct pointer */

    /* set the ds pointer to make it easier to reference the option's
       particular data struct */
    ds_ptr = otn->ds_list[PLUGIN_TEMPLATE_NUMBER];

    /* manipulate the option arguments here */

    /* set the final option arguments here */
}


/****************************************************************************
 * 
 * Function: TemplateDetectorFunction(char *, OptTreeNode *)
 *
 * Purpose: Use this function to perform the particular detection routine
 *          that this rule keyword is supposed to encompass.
 *
 * Arguments: data => argument data
 *            otn => pointer to the current rule's OTN
 *
 * Returns: If the detection test fails, this function *must* return a zero!
 *          On success, it calls the next function in the detection list 
 *
 ****************************************************************************/
int TemplateDetectorFunction(Packet *p, struct _OptTreeNode *otn, OptFpList *fp_list)
{
    /* reference the option's data struct by looking it up on the data
       struct list like this:
   
           otn->ds_list[PLUGIN_TEMPLATE_NUMBER];
 
       Try to make this function as quick as possible, the faster the 
       detection plugins are, the less packet loss the program will
       experience!  Avoid doing things like declaring variables or anything 
       other than just doing the test and moving on...
     */

    /* your detection function tests go here */
    if (the_test_is_successful)
    {
        /* call the next function in the function list recursively */
        /* THIS CALL *MUST* BE IN THE PLUGIN, OTHERWISE YOU WILL BREAK
           SNORT'S DETECTION ENGINE!!! */
        return fp_list->next->OptTestFunc(p, otn, fp_list->next);
    }
#ifdef DEBUG
    else
    {
        /* you can put debug comments here or not */
        DebugMessage(DEBUG_PLUGIN,"No match\n");
    }
#endif

    /* if the test isn't successful, this function *must* return 0 */
    return 0;
}
