/*
** Copyright (C) 1998-2002 Martin Roesch <roesch@sourcefire.com>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

/* $Id: sp_icmp_code_check.c,v 1.12 2003/05/13 20:53:52 chrisgreen Exp $ */

#include <stdlib.h>
#include <ctype.h>

#include "rules.h"
#include "decode.h"
#include "plugbase.h"
#include "parser.h"
#include "util.h"
#include "debug.h"
#include "plugin_enum.h"

typedef struct _IcmpCodeCheckData
{
    /* the icmp code number */
    int icmp_code;

} IcmpCodeCheckData;

void IcmpCodeCheckInit(char *, OptTreeNode *, int);
void ParseIcmpCode(char *, OptTreeNode *);
int IcmpCodeCheck(Packet *, struct _OptTreeNode *, OptFpList *);





/****************************************************************************
 * 
 * Function: SetupIcmpCodeCheck()
 *
 * Purpose: Register the icode keyword and configuration function
 *
 * Arguments: None.
 *
 * Returns: void function
 *
 ****************************************************************************/
void SetupIcmpCodeCheck()
{
    /* map the keyword to an initialization/processing function */
    RegisterPlugin("icode", IcmpCodeCheckInit);
    DEBUG_WRAP(DebugMessage(DEBUG_PLUGIN, "Plugin: IcmpCodeCheck Initialized\n"););
}


/****************************************************************************
 * 
 * Function: IcmpCodeCheckInit(char *, OptTreeNode *)
 *
 * Purpose: Initialize the rule data structs and parse the rule argument
 *          data, then link in the detection function
 *
 * Arguments: data => rule arguments/data
 *            otn => pointer to the current rule option list node
 *
 * Returns: void function
 *
 ****************************************************************************/
void IcmpCodeCheckInit(char *data, OptTreeNode *otn, int protocol)
{
    if(protocol != IPPROTO_ICMP)
    {
        FatalError( "%s(%d): ICMP Options on non-ICMP rule\n", file_name, file_line);
    }

    /* multiple declaration check */ 
    if(otn->ds_list[PLUGIN_ICMP_CODE])
    {
        FatalError("%s(%d): Multiple icmp code options in rule\n", file_name,
                file_line);
    }

    /* allocate the data structure and attach it to the
       rule's data struct list */

    otn->ds_list[PLUGIN_ICMP_CODE] = (IcmpCodeCheckData *)
            SnortAlloc(sizeof(IcmpCodeCheckData));

    /* this is where the keyword arguments are processed and placed into the 
       rule option's data structure */
    ParseIcmpCode(data, otn);

    /* finally, attach the option's detection function to the rule's 
       detect function pointer list */
    AddOptFuncToList(IcmpCodeCheck, otn);
}



/****************************************************************************
 * 
 * Function: ParseIcmpCode(char *, OptTreeNode *)
 *
 * Purpose: Process the icode argument and stick it in the data struct
 *
 * Arguments: data => argument data
 *            otn => pointer to the current rule's OTN
 *
 * Returns: void function
 *
 ****************************************************************************/
void ParseIcmpCode(char *data, OptTreeNode *otn)
{
    char *code;
    IcmpCodeCheckData *ds_ptr;  /* data struct pointer */

    /* set the ds pointer to make it easier to reference the option's
       particular data struct */
    ds_ptr = otn->ds_list[PLUGIN_ICMP_CODE];

    /* set a pointer to the data so to leave the original unchanged */
    code = data;

    /* get rid of whitespace before the data */
    while(isspace((int)*data))
        data++;

    /* make sure this is a number (maybe someday this can handle ICMP code
       strings as well) */
    if(isdigit((int)*data))
    {
        /* set the icmp code value */
        ds_ptr->icmp_code = atoi(data);

        /* all done */
        return;
    }
    else  /* uh oh */
    {
        FatalError("%s(%d): Bad ICMP code: %s\n", file_name, 
                   file_line, data);
    }  
}


/****************************************************************************
 * 
 * Function: IcmpCodeCheck(char *, OptTreeNode *)
 *
 * Purpose: Test the packet's ICMP code field value against the option's
 *          ICMP code
 *
 * Arguments: data => argument data
 *            otn => pointer to the current rule's OTN
 *
 * Returns: void function
 *
 ****************************************************************************/
int IcmpCodeCheck(Packet *p, struct _OptTreeNode *otn, OptFpList *fp_list)
{
    if(!p->icmph)
        return 0; /* if error occured while icmp header
                   * was processed, return 0 automagically.
               */
    if(((IcmpCodeCheckData *) otn->ds_list[PLUGIN_ICMP_CODE])->icmp_code == p->icmph->code)
    {
        DEBUG_WRAP(DebugMessage(DEBUG_PLUGIN, "Got icmp code match!\n"););
        /* call the next function in the function list recursively */
        return fp_list->next->OptTestFunc(p, otn, fp_list->next);
    }
    else
    {
        DEBUG_WRAP(DebugMessage(DEBUG_PLUGIN, "icmp code mismatch!\n"););
    }

    /* return 0 on failed test */
    return 0;
}
