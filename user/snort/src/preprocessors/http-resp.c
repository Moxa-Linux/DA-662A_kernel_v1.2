/*
**
*  $Id: http-resp.c,v 1.2 2003/03/05 03:33:18 chris_reid Exp $
**
** http-resp.c
**
** Copyright (C) 2002 Sourcefire,Inc
** Marc Norton <mnorton@sourcefire.com>
** Dan Roelker <droelker@sourcefire.com>
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
**
**
*/
#include "decode.h"
#include "rules.h"
#include "http-resp.h"
#include <string.h>


static int iHttpFlowByteCount = 0;
static int HttpFlowPorts[65536];

/*
*    
*/
static inline int IsHttpProtocol( Packet * p )
{
    if(HttpFlowPorts[p->sp] && !HttpFlowPorts[p->dp])
    {
        return 1;
    }
    
    return 0;
}

int SetupHttpFlowByteCount(int iByteCount)
{
    iHttpFlowByteCount = iByteCount;
    return 1;
}

int InitHttpFlowPorts()
{
    memset(&HttpFlowPorts, 0x00, sizeof(HttpFlowPorts));
    return 0;
}

/*
**  
**  NAME
**    AddHttpFlowPort
**    
**  DESCRIPTION
**    Add Http port to HttpFlow port list
**
**  FORMAL INPUTS
**    int - the port number
**
**  FORMAL OUTPUT
**    int - 0 is successful, 1 is invalid port.
**
*/
int AddHttpFlowPort(int iPort)
{
    if(iPort < 1 || iPort > 65535)
    {
        return 1;
    }

    HttpFlowPorts[iPort] = 1;

    return 0;
}

int IsHttpServerData( Packet * p )
{
    /* 
    ** HTTP:Server-Side-Session-Performance-Optimization
    ** This drops Server->Client packets which are not part of the 
    ** HTTP Response header. It can miss part of the response header 
    ** if the header is sent as multiple packets.
    */

    if(p->tcph && (p->packet_flags & PKT_FROM_SERVER))
    {        
        if( IsHttpProtocol( p ) )
        {
            if( p->dsize > 4 )
	        {
                if( (p->data[0]!='H') || (p->data[1]!='T') || 
                    (p->data[2]!='T') || (p->data[3]!='P') )
                {
                    return 1;
                }
	       
                /*
                **  OK its an HTTP response header.
                */

                /*       
                **  Now, limit the amount we inspect, 
                **  we could just examine this whole packet, 
                **  since it's usually full of HTTP Response info.
                **  For protocol analysis purposes we probably ought to 
                **  let the whole thing get processed, or have a 
                **  different pattern match length and protocol inspection 
                **  length.
                */

                if(p->dsize > iHttpFlowByteCount)
                {
                    p->dsize = iHttpFlowByteCount;  
                }

                return 0;
            }   
            else
            {
                return 1;
            } 
        }
    }
   
    return 0;
}

