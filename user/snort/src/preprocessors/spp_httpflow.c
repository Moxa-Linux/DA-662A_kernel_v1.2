/* $Id: spp_httpflow.c,v 1.2 2002/10/30 19:18:42 andrewbaker Exp $ 
**
**  spp_httpflow.c
**
**  Copyright (C) 2002 Sourcefire, Inc.
**  Dan Roelker <droelker@sourcefire.com>
**  Marc Norton <mnorton@sourcefire.com>
**
**  DESCRIPTION
**    This is the httpflow preprocessor that optimizes http flow
**    inspection.  The assumption here is that http server-side data
**    is 'mostly' harmless, and few rules are written for server-side
**    data.  Yet, server-side data accounts for 95% of web traffic.
**    A majority of this server-side traffic can be ignored which
**    significantly enhances performance opportunity.
**
**  NOTES
**  6.3.02 - Initial Source Code.  Norton/Roelker
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
**
*/

#include <stdlib.h>
#include <ctype.h>

#include "plugbase.h"
#include "debug.h"
#include "mstring.h"
#include "util.h"
#include "http-resp.h"
#include "spp_httpflow.h"
#include "parser.h"

typedef struct _HTTPFLOW
{
    int iType;
    int iDepth;
    int aiPorts[65536];

} HTTPFLOW;

#define HTTPFLOW_FULL_TOKEN  "full"
#define HTTPFLOW_QUICK_TOKEN "quick"

#define HTTPFLOW_FULL  1
#define HTTPFLOW_QUICK 2

#define HTTPFLOW_SS_BYTE_CNT_TOKEN "depth"
#define HTTPFLOW_SS_BYTE_CNT       200

#define HTTPFLOW_PORTS_TOKEN "ports"

#define HTTPFLOW_MAX_TOKENS 20

/*
**  Functions
*/
static void InitHttpFlow(u_char *args);
static void ParseHttpFlowArgs(char *args);
static void HttpFlow(Packet *p);

/*
 * Function: SetupHttpFlow()
 *
 * Purpose: Registers the preprocessor keyword and initialization 
 *          function into the preprocessor list.  This is the function that
 *          gets called from InitPreprocessors() in plugbase.c.
 *
 * Arguments: None.
 *
 * Returns: void function
 *
 */
void SetupHttpFlow()
{
    /* link the preprocessor keyword to the init function in 
       the preproc list */
    RegisterPreprocessor("HttpFlow", InitHttpFlow);

    DEBUG_WRAP(DebugMessage(DEBUG_PLUGIN,"Preprocessor: Template is setup...\n"););
}


/*
 * Function: InitHttpFlow(u_char *)
 *
 * Purpose: Calls the argument parsing function, performs final setup on data
 *          structs, links the preproc function into the function list.
 *
 * Arguments: args => ptr to argument string
 *
 * Returns: void function
 *
 */
static void InitHttpFlow(u_char *args)
{
    DEBUG_WRAP(DebugMessage(DEBUG_PLUGIN,"Preprocessor: httpflow Initialized\n"););

    /* parse the argument list from the rules file */
    ParseHttpFlowArgs(args);

    /* Set the preprocessor function into the function list */
    AddFuncToPreprocList(HttpFlow);
}

/*
**  This function verifies that the input is a positive number.
*/
static int VerifyResponseBytes(char *pcBytes)
{
    int iCtr;

    for(iCtr = 0; iCtr < strlen(pcBytes); iCtr++)
    {
        if(!isdigit(pcBytes[iCtr]))
        {
            return 1;
        }
    }

    return 0;
}

/*
**  VerifyHttpFlowPort verifies that the input is a port and not 0.
*/
int VerifyHttpFlowPort(char *pcPort)
{
    int iCtr;
    int iPort;

    for(iCtr = 0; iCtr < strlen(pcPort); iCtr++)
    {
        if(!isdigit(pcPort[iCtr]))
        {
            return 1;
        }
    }

    iPort = atoi(pcPort);

    if(iPort < 1 || iPort > 65535)
    {
        return 1;
    }

    return 0;
}

static int DisplayConfig(HTTPFLOW *hfInfo)
{
    int iCtr;
    int iPorts = 0;

    LogMessage("HttpFlow config:\n");
    LogMessage("    Depth: %d\n", hfInfo->iDepth);
    LogMessage("    Ports: ");

    for(iCtr = 0; iCtr < 65536; iCtr++)
    {
        if(hfInfo->aiPorts[iCtr])
        {
            LogMessage("%d ", iCtr);
            iPorts++;
        }

        if(iPorts > 20)
        {
            LogMessage("...\n");
            return 0;
        }
    }

    LogMessage("\n");

    return 0;
}

/*
**  Function: ParseHttpFlowArgs(char *)
**
**  Purpose: Process the preprocessor arguements from the rules file and 
**           initialize the preprocessor's data struct.  This function doesn't
**           have to exist if it makes sense to parse the args in the init 
**           function.
**
**           The HttpFlow arguments will consist of the type of mode that
**           HttpFlow should run in.  This mode can be either a full or
**           quick mode.  If the mode is full, then there is no additional
**           configuration options.  The full args would look like this:
**             full
**           That is it.  :)  For quick processing there are additional
**           configuration options of the server-side byte count and the
**           http ports to inspect.  The quick args would look like this:
**             quick server_side_byte_count 200 ports 80 3128 8080
**
**  Arguments: args => argument list
**
**  Returns: void function
**
*/
static void ParseHttpFlowArgs(char *args)
{
    HTTPFLOW hfInfo;
    char **Toks;
    int iTokNum;
    int iCtr;
    int iPortsSet = 0;
    int iPort;

    memset(&hfInfo, 0x00, sizeof(hfInfo));
    
    if(args == NULL)
    {
        FatalError("%s(%d)=> No arguments to HttpFlow preprocessor"
                   "!\n", file_name, file_line);
    }

    Toks = mSplit(args, " \t", HTTPFLOW_MAX_TOKENS, &iTokNum, '\\');
    
    /*
    **  We look for two exclusive parameters that signify
    **  full analysis (which means that the whole server-side
    **  HTTP stream is inspected, and quick analysis (which
    **  means that the specified number of bytes is inspected in
    **  the server-side HTTP stream.
    */

    hfInfo.iType = HTTPFLOW_QUICK;

    /*
    **  Set default byte count
    */
    SetupHttpFlowByteCount(HTTPFLOW_SS_BYTE_CNT);
    hfInfo.iDepth = HTTPFLOW_SS_BYTE_CNT;

    InitHttpFlowPorts();

    for(iCtr =0; iCtr < iTokNum; iCtr++)
    {
        if(!strcmp(HTTPFLOW_SS_BYTE_CNT_TOKEN, Toks[iCtr]))
        {
            iCtr++;
            if(iCtr >= iTokNum)
            {
                FatalError("%s (%d)=> Invalid depth value for "
                           "HttpFlow preprocessor.  Must be zero "
                           "or positive integer.\n", file_name, file_line);
            }

            if(VerifyResponseBytes(Toks[iCtr]))
            {
                FatalError("%s (%d)=> Invalid response byte value "
                           "for HttpFlow preprocessor.  Must be zero or "
                           "positive integer.\n", file_name, file_line);
            }
                
            hfInfo.iDepth = atoi(Toks[iCtr]);
            SetupHttpFlowByteCount(hfInfo.iDepth);
        }
        else if(!strcmp(HTTPFLOW_PORTS_TOKEN, Toks[iCtr]))
        {
            for(iCtr++; iCtr < iTokNum; iCtr++)
            {
                if(VerifyHttpFlowPort(Toks[iCtr]))
                {
                    /*
                    **  Look ahead token.  Go back so loop
                    **  can iterate correctly.
                    */
                    iCtr--;
                    break;
                }

                iPort = atoi(Toks[iCtr]);

                hfInfo.aiPorts[iPort] = 1;
                AddHttpFlowPort(iPort);

                if(!iPortsSet)
                {
                    iPortsSet = 1;
                }
            }
        }
	    
        else
        {
            FatalError("%s(%d)=> Invalid parameter '%s' for "
                       "HttpFlow preprocessor.\n", file_name, file_line,
                       Toks[iCtr]);
        }
    }
    
    /*
    **  Check to see if any ports were set, if not then we set port
    **  80.
    */
    if(!iPortsSet)
    {
        hfInfo.aiPorts[80] = 1;
        AddHttpFlowPort(80);
    }

    DisplayConfig(&hfInfo);

    return;
}

/*
 * Function: HttpFlow(Packet *)
 *
 * Purpose: Perform the preprocessor's intended function.  This can be
 *          simple (statistics collection) or complex (IP defragmentation)
 *          as you like.  Try not to destroy the performance of the whole
 *          system by trying to do too much....
 *
 * Arguments: p => pointer to the current packet data struct 
 *
 * Returns: void function
 *
 */
static void HttpFlow(Packet *p)
{
    extern int do_detect;

    /*
    **  Check to see if the data on this packet is Http server-side
    **  data.  If it is, then we don't want to process it.
    */    
    if(IsHttpServerData(p))
    {
        /*
        **  Don't do anymore inspection/preprocessing on this
        **  packet, process portscan though.
        */
        do_detect = 0;
        p->preprocessors = 0;

        /*
        **  We turn on these flags for additional preprocessing.
        */
        p->preprocessors |= PP_PORTSCAN;
        p->preprocessors |= PP_STREAM4;
    }

    return;
}
