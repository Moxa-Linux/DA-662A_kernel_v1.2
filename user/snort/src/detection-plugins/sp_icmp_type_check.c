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

/* $Id: sp_icmp_type_check.c,v 1.13 2003/05/13 20:53:53 chrisgreen Exp $ */

#include <stdlib.h>
#include <ctype.h>

#include "rules.h"
#include "decode.h"
#include "plugbase.h"
#include "parser.h"
#include "debug.h"
#include "util.h"
#include "plugin_enum.h"
#include "sp_icmp_type_check.h"


 
void IcmpTypeCheckInit(char *, OptTreeNode *, int);
void ParseIcmpType(char *, OptTreeNode *);
int IcmpTypeCheck(Packet *, struct _OptTreeNode *, OptFpList *);



/****************************************************************************
 * 
 * Function: SetupIcmpTypeCheck()
 *
 * Purpose: Register the itype keyword and configuration function
 *
 * Arguments: None.
 *
 * Returns: void function
 *
 ****************************************************************************/
void SetupIcmpTypeCheck()
{
    /* map the keyword to an initialization/processing function */
    RegisterPlugin("itype", IcmpTypeCheckInit);
    DEBUG_WRAP(DebugMessage(DEBUG_PLUGIN,"Plugin: IcmpTypeCheck Initialized\n"););
}


/****************************************************************************
 * 
 * Function: IcmpTypeCheckInit(char *, OptTreeNode *)
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
void IcmpTypeCheckInit(char *data, OptTreeNode *otn, int protocol)
{
    if(protocol != IPPROTO_ICMP)
    {
        FatalError("%s(%d): ICMP Options on non-ICMP rule\n", file_name, file_line);
    }
    
    /* multiple declaration check */ 
    if(otn->ds_list[PLUGIN_ICMP_TYPE])
    {
        FatalError("%s(%d): Multiple ICMP type options in rule\n", file_name,
                file_line);
    }
        
    /* allocate the data structure and attach it to the
       rule's data struct list */
    otn->ds_list[PLUGIN_ICMP_TYPE] = (IcmpTypeCheckData *)
            SnortAlloc(sizeof(IcmpTypeCheckData));

    /* this is where the keyword arguments are processed and placed into the 
       rule option's data structure */
    ParseIcmpType(data, otn);

    /* finally, attach the option's detection function to the rule's 
       detect function pointer list */
    AddOptFuncToList(IcmpTypeCheck, otn);
}



/****************************************************************************
 * 
 * Function: ParseIcmpType(char *, OptTreeNode *)
 *
 * Purpose: Process the itype argument and stick it in the data struct
 *
 * Arguments: data => argument data
 *            otn => pointer to the current rule's OTN
 *
 * Returns: void function
 *
 ****************************************************************************/
void ParseIcmpType(char *data, OptTreeNode *otn)
{
    char *type;
    IcmpTypeCheckData *ds_ptr;  /* data struct pointer */

    /* set the ds pointer to make it easier to reference the option's
       particular data struct */
    ds_ptr = otn->ds_list[PLUGIN_ICMP_TYPE];

    /* set a pointer to the data so to leave the original unchanged */
    type = data;

    if(!data)
    {
        FatalError("%s(%d): No ICMP Type Specified %s\n" );
    }
    
    /* get rid of spaces before the data */
    while(isspace((int)*data))
        data++;

    if(data[0] == '\0')
    {
        FatalError( "%s(%d): No ICMP Type Specified %s\n" );
    }

    /* make sure this is a number (maybe someday this can handle ICMP type
       strings as well */
    if(isdigit((int)*data))
    {
        /* set the icmp type value */
        ds_ptr->icmp_type = atoi(data);

        /* all done */
        return;
    }
    else  /* uh oh */
    {
        FatalError("%s(%d): Bad ICMP type: %s\n", file_name, 
                   file_line, data);

    }  
}


/****************************************************************************
 * 
 * Function: IcmpTypeCheck(char *, OptTreeNode *)
 *
 * Purpose: Test the packet's ICMP type field value against the option's
 *          ICMP type
 *
 * Arguments: data => argument data
 *            otn => pointer to the current rule's OTN
 *
 * Returns: void function
 *
 ****************************************************************************/
int IcmpTypeCheck(Packet *p, struct _OptTreeNode *otn, OptFpList *fp_list)
{
    if(!p->icmph)
        return 0; /* if error occured while icmp header
                   * was processed, return 0 automagically.
               */
    if(((IcmpTypeCheckData *) otn->ds_list[PLUGIN_ICMP_TYPE])->icmp_type == p->icmph->type)
    {
        DEBUG_WRAP(DebugMessage(DEBUG_PLUGIN,"Got icmp type match!\n"););
        /* call the next function in the function list recursively */
        return fp_list->next->OptTestFunc(p, otn, fp_list->next);
    }
    else
    {
        DEBUG_WRAP(DebugMessage(DEBUG_PLUGIN, "icmp type mismatch!\n"););
    }

    /* return 0 on failed test */
    return 0;
}
