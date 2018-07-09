/* $Id: threshold.c,v 1.3 2002/10/23 02:48:11 chrisgreen Exp $ */
/*
** Copyright (C) 2002 Martin Roesch <roesch@sourcefire.com>
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


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "mstring.h"
#include "decode.h"
#include "event.h"
#include "rules.h"
#include "util.h"
#include "threshold.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define THRESHOLD_BUCKETS 1023

/**  D A T A  S T R U C T U R E S  ******************************/
typedef struct _ThresholdData
{
    u_int32_t start_time;  /* when this entry was inserted */
    
    u_int32_t sip;  /* src ip */
    u_int32_t dip;  /* dst ip */
    u_int16_t dp;   /* dst port */
    
    u_int32_t sig_id;  /* snort ID of event */

    u_int32_t limit; /* count remaining */

    u_int32_t window; /* time threshold can be active for */

    u_int8_t type;  /* event, ip or port */

    struct _ThresholdData *next;
    
} ThresholdData;


/**  G L O B A L S  ******************************************/
ThresholdData *thresh_hash_table[THRESHOLD_BUCKETS];

extern int file_line;
extern char *file_name;

/**  P R O T O T Y P E S  ************************************/
int ThresholdHashFunc(Packet *, Event *, u_int8_t);
void *ThreshAlloc(u_int32_t);
ThresholdData *InsertEvent(Packet *, Event *, int, u_int32_t, u_int32_t, 
        u_int8_t);
void DeleteEvent(ThresholdData *, int);
ThresholdData *FindThreshold(Packet *, Event *, int, u_int8_t);


int ThresholdHashFunc(Packet *p, Event *event, u_int8_t type)
{
    int hash = 0;
    
    switch(type)
    {
        case THRESHOLD_TYPE_PORT: 
            hash = event->sig_id ^ p->iph->ip_dst.s_addr ^ p->dp ^ 
                p->iph->ip_src.s_addr;
            break;

        case THRESHOLD_TYPE_IP:
            hash = event->sig_id ^ p->iph->ip_dst.s_addr ^ 
                p->iph->ip_src.s_addr;
            break;

        case THRESHOLD_TYPE_EVENT:
            hash = event->sig_id ^ p->iph->ip_src.s_addr; 
            break; 
    }

    return hash % THRESHOLD_BUCKETS;
}


void *ThreshAlloc(u_int32_t size)
{
    void *tmp;

    tmp = (void *) calloc(size, sizeof(char));

    if(tmp == NULL)
    {
        FatalError("Threshold: Unable to allocate memory!\n");
    }

    return tmp;
}


ThresholdData *InsertEvent(Packet *p, Event *event, int bucket, 
        u_int32_t threshold, u_int32_t window, u_int8_t type)
{
    ThresholdData *idx = NULL;  /* indexing pointer */
    
    /* find a place to put it */
    if(thresh_hash_table[bucket] == NULL)
    {
        thresh_hash_table[bucket] =
            (ThresholdData *) ThreshAlloc(sizeof(ThresholdData));

        idx = thresh_hash_table[bucket];
    }
    else
    {
        idx = thresh_hash_table[bucket];

        while(idx->next != NULL) idx = idx->next;

        idx->next = (ThresholdData *) ThreshAlloc(sizeof(ThresholdData));

        idx = idx->next;
    }

    idx->sip = p->iph->ip_src.s_addr;
    idx->dip = p->iph->ip_dst.s_addr;
    idx->dp = p->dp;
    idx->start_time = p->pkth->ts.tv_sec;
    idx->sig_id = event->sig_id;
    idx->limit = threshold;
    idx->window = window;
    idx->type = type;

    return idx;
}


void DeleteEvent(ThresholdData *td, int bucket)
{
    ThresholdData *tmp;
    
    tmp = thresh_hash_table[bucket];

    if(tmp == td && tmp->next == NULL)
    {
        free(td);
        thresh_hash_table[bucket] = NULL;
        return;
    }
    else if(tmp == td)
    {
        thresh_hash_table[bucket] = td->next;
        free(td);
        return;
    }

    while(tmp->next != td) tmp = tmp->next;

    tmp->next = td->next;

    free(td);
}


ThresholdData *FindThreshold(Packet *p, Event *event, int bucket, u_int8_t type)
{
    ThresholdData *tmp;

    tmp = thresh_hash_table[bucket];

    while(tmp != NULL)
    {
        switch(type)
        {
            case THRESHOLD_TYPE_PORT:
                if(tmp->sip == p->iph->ip_src.s_addr &&
                        tmp->dip == p->iph->ip_dst.s_addr &&
                        tmp->dp == p->dp &&
                        tmp->sig_id == event->sig_id)
                {
                    return tmp;
                }

            case THRESHOLD_TYPE_IP:
                if(tmp->sip == p->iph->ip_src.s_addr &&
                        tmp->dip == p->iph->ip_dst.s_addr &&
                        tmp->sig_id == event->sig_id)
                {
                    return tmp;
                }

            case THRESHOLD_TYPE_EVENT:
                if(tmp->sip == p->iph->ip_src.s_addr &&
                        tmp->sig_id == event->sig_id)
                {
                    return tmp;
                }
        }

        tmp = tmp->next;
    }

    return NULL;
}


int Threshold(Packet *p, Event *event, u_int32_t limit, u_int32_t window, 
        u_int8_t type)
{
    int bucket;
    ThresholdData *thd;

    /* generate a hash bucket for this event */
    bucket = ThresholdHashFunc(p, event, type);

    /* lookup the threshold tracker */
    thd = FindThreshold(p, event, bucket, type);

    if(thd == NULL)
    {
        /* no threshold tracker found, make a new one */
        thd = InsertEvent(p, event, bucket, limit, window, type);
        thd->limit--;
    }
    else
    {
        if(--thd->limit == 0)
        {
            /* threshold reached, delete the tracker and return a 
             * threshold reached response
             */
            DeleteEvent(thd, bucket);
            return 1;
        }

        if((p->pkth->ts.tv_sec  - thd->start_time) > thd->window)
        {
            /* time limit reached, drop the tracker and return no success */
            DeleteEvent(thd, bucket);
            return 0;
        }
    }

    return 0;
}


void ParseThreshold(char *args, u_int32_t *threshold, u_int32_t *window, 
        u_int8_t *type)
{
    int num_toks;
    char **toks;

    toks = mSplit(args, ",", 3, &num_toks, 0);

    
    errno = 0;
    
    *threshold = strtol(toks[0], NULL, 10);

    if(errno == ERANGE)
    {
        perror("ParseThreshold()");
        FatalError("ERROR %s(%d) => bad Threshold/window value\n", 
                file_name, file_line);
    }

    errno = 0;
    
    *window = strtol(toks[1], NULL, 10);

    if(errno == ERANGE)
    {
        perror("ParseThreshold()");
        FatalError("ERROR %s(%d) => bad Threshold/window value\n", 
                file_name, file_line);
    }

    if(!strcasecmp(toks[2], "port"))
    {
        *type = THRESHOLD_TYPE_PORT;
    }
    else if(!strcasecmp(toks[2], "ip"))
    {
        *type = THRESHOLD_TYPE_IP;
    }
    else if(!strcasecmp(toks[2], "event"))
    {
        *type = THRESHOLD_TYPE_EVENT;
    }

    return;
}


