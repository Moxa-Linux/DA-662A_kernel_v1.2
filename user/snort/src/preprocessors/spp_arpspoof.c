/*
** Copyright (C) 2001 Jeff Nathan <jeff@wwti.com>
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

/* $Id: spp_arpspoof.c,v 1.19 2002/10/30 19:18:36 andrewbaker Exp $ */
/* Snort ARPspoof Preprocessor Plugin
*   by Jeff Nathan <jeff@wwti.com>
*   Version 0.1.2
*
* Purpose:
*
* This preprocessor looks for anomalies in ARP traffic and attempts to 
* maliciously overwrite  ARP cache information on hosts.
*
* Arguments:
*
* To check for unicast ARP requests use:
* arpspoof: -unicast
*
* WARNING: this can generate false positives as Linux systems send unicast 
* ARP requests repetatively for entries in their cache.
*
* This plugin also takes a list of IP addresses and MAC address in the form:
* arpspoof_detect_host: 10.10.10.10 29:a2:9a:29:a2:9a
* arpspoof_detect_host: 192.168.40.1 f0:0f:00:f0:0f:00
* and so forth...
*
* Effect:
* By comparing information in the Ethernet header to the ARP frame, obvious
* anomalies are detected.  Also, utilizing a user supplied list of IP addresses 
* and MAC addresses, ARP traffic appearing to have originated from any IP in 
* that list is carefully examined by comparing the source hardware address to 
* the user supplied hardware address.  If there is a mismatch, an alert is 
* generated as either an ARP request or REPLY can be used to overwrite cache 
* information on a remote host.  This should only be used for hosts/devices
* on the **same layer 2 segment** !!
*
* Bugs:
* This is a proof of concept ONLY.  It is clearly not complete.  Also, the 
* lookup function LookupIPMacEntryByIP is in need of optimization.  The
* arpspoof_detect_host functionality may false alarm in redundant environments. * Also, see the comment above pertaining to Linux systems.
*
* Thanks:
*
* First and foremost Patrick Mullen who sat beside me and helped every step of
* the way.  Andrew Baker for graciously supplying the tougher parts of this 
* code.  W. Richard Stevens for readable documentation, Kevin DePeugh for 
* his incredible depth of knowledge and dedication to research. And finally 
* Marty for being a badass.  All your packets are belong to Marty.
*
*/

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifndef WIN32
    #include <sys/time.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
#else
    #include <time.h>
#endif


#include "generators.h"
#include "log.h"
#include "detect.h"
#include "decode.h"
#include "event.h"
#include "plugbase.h"
#include "parser.h"
#include "mstring.h"
#include "debug.h"
#include "util.h"


#define MODNAME "spp_arpspoof"

#define WITHUNICAST "-unicast"

int check_unicast_arp = 0;



typedef struct _IPMacEntry
{
    u_int32_t ipv4_addr;
    u_int8_t  mac_addr[6];
    u_int8_t  pad[2];
} IPMacEntry;

typedef struct _IPMacEntryListNode
{
    IPMacEntry *ip_mac_entry;
    struct _IPMacEntryListNode *next;
} IPMacEntryListNode;

typedef struct _IPMacEntryList
{
    int size;
    IPMacEntryListNode *head;
    IPMacEntryListNode *tail;
} IPMacEntryList;

int AddIPMacEntryToList(IPMacEntryList *ipmel, IPMacEntry *ipme);
IPMacEntry *LookupMacEntryByIP(IPMacEntryList *ipmel, u_int32_t ipv4_addr);
void FreeIPMacEntryList(IPMacEntryList *ipmel);

/* list of function prototypes for this preprocessor */
void ARPspoofInit(u_char *);
void ParseARPspoofArgs(char *);
void ARPspoofPreprocFunction(Packet *);
void ARPspoofRestartFunction(int);
void ARPspoofCleanExitFunction(int);
void ARPwatchArg(u_char *);



struct spp_timeval
{
    time_t tv_sec;
    time_t tv_usec;
};

/* from tcpdump source (where ARP frames are fully evaluated) */
u_int8_t bcast[6];

static IPMacEntryList *ipmel;

int AddIPMacEntryToList(IPMacEntryList *ip_mac_entry_list, 
        IPMacEntry *ip_mac_entry)
{
    IPMacEntryListNode *newNode;

    if (ip_mac_entry == NULL || ipmel == NULL)
        return 1;

    newNode = (IPMacEntryListNode *)malloc(sizeof(IPMacEntryListNode));
    /* XXX: OOM Check */
    newNode->ip_mac_entry = ip_mac_entry;
    newNode->next = NULL;

    if (ipmel->head == NULL)
    {
        ipmel->head = newNode;
        ipmel->size = 1;
    }
    else
    {
        ipmel->tail->next = newNode;
        ipmel->size += 1;
    }
    ipmel->tail = newNode;
    return 0;
}


IPMacEntry *LookupIPMacEntryByIP(IPMacEntryList *ipmel, u_int32_t ipv4_addr)
{
    IPMacEntryListNode *current;
#ifdef DEBUG
    struct in_addr ina, inb;
    char *cha, *chb;
#endif

    if (ipmel == NULL)
        return NULL;

    for (current = ipmel->head; current != NULL; current = current->next)
    {
#ifdef DEBUG
        ina.s_addr = ipv4_addr;
        inb.s_addr = current->ip_mac_entry->ipv4_addr;
        cha = strdup(inet_ntoa(ina));
        chb = strdup(inet_ntoa(inb));

        DEBUG_WRAP(DebugMessage(DEBUG_PLUGIN, 
            "MODNAME: LookupIPMacEntryByIP() comparing %s to %s\n", cha, chb););

#endif
        if (current->ip_mac_entry->ipv4_addr == ipv4_addr)
        {
            DEBUG_WRAP(DebugMessage(DEBUG_PLUGIN, 
                    "MODNAME: LookupIPMecEntryByIP() match!"););

            return current->ip_mac_entry;
        }
    }
    return NULL;
}

void FreeIPMacEntryList(IPMacEntryList *ipmel)
{
    IPMacEntryListNode *prev;
    IPMacEntryListNode *current;

    if (ipmel == NULL)
        return;

    current = ipmel->head;
    while (current != NULL)
    {
        if (current->ip_mac_entry != NULL)
            free(current->ip_mac_entry);

        prev = current;
        current = current->next;
        free(prev);
    }
    ipmel->head = NULL;
    ipmel->size = 0;
}

void SetupARPspoof()
{
    RegisterPreprocessor("arpspoof", ARPspoofInit);
    RegisterPreprocessor("arpspoof_detect_host", ARPwatchArg);

    DEBUG_WRAP(DebugMessage(DEBUG_INIT, 
            "MODNAME is setup...\n"););
}

void ParseARPspoofArgs(char *args)
{
    char **toks;
    int num_toks;
    int num;

    if (!args)
        return;

    toks = mSplit(args, " ", 2, &num_toks, '\\');

        if (num_toks > 1)
        {      
            FatalError(MODNAME ": ERROR: %s (%d) => ARPspoof configuration "
                    "format: -unicast\n", file_name, file_line);
        } 

    for (num = 0; num < num_toks; num++)
    {
        if (!strncasecmp(WITHUNICAST, toks[num], sizeof WITHUNICAST))
            check_unicast_arp = 1;
    }
}

void ARPspoofPreprocFunction(Packet *p)
{
    Event event;
    IPMacEntry *ipme;
    u_int32_t addr;

    if(!(p->preprocessors & PP_ARPSPOOF))
    {
        return;
    }

    memset(&addr, 0, sizeof(u_int32_t));
    if (p && (p->eh != NULL && p->ah != NULL))
    {
        memset(bcast, 0xff, 6);

        if ((ntohs(p->ah->ea_hdr.ar_hrd) != 0x0001) || 
                (ntohs(p->ah->ea_hdr.ar_pro) != ETHERNET_TYPE_IP))
        {
            /* hardware type is not Ethernet or protocol type is not IP */
            return;
        }
        switch(ntohs(p->ah->ea_hdr.ar_op))
        {
            case ARPOP_REQUEST:
                if (check_unicast_arp) 
                {
                    if (memcmp((u_char *)p->eh->ether_dst, 
                            (u_char *)bcast, 6) != 0)
                    {
                        SetEvent(&event, GENERATOR_SPP_ARPSPOOF, 
                                ARPSPOOF_UNICAST_ARP_REQUEST, 1, 0, 0, 0);

                        CallAlertFuncs(p, ARPSPOOF_UNICAST_ARP_REQUEST_STR, 
                                NULL, &event);

                        DEBUG_WRAP(DebugMessage(DEBUG_PLUGIN, 
                                "MODNAME: unicast ARP request\n"););
                    }
                }
                else if (memcmp((u_char *)p->eh->ether_src, 
                        (u_char *)p->ah->arp_sha, 6) != 0) 
                {
                    SetEvent(&event, GENERATOR_SPP_ARPSPOOF, 
                            ARPSPOOF_ETHERFRAME_ARP_MISMATCH_SRC, 1, 0, 0, 0);

                    CallAlertFuncs(p, ARPSPOOF_ETHERFRAME_ARP_MISMATCH_SRC_STR, 
                            NULL, &event);
                    DEBUG_WRAP(DebugMessage(DEBUG_PLUGIN, 
                            "MODNAME: Ethernet/ARP mismatch request\n"););
                }
                break;
            case ARPOP_REPLY:
                if (memcmp((u_char *)p->eh->ether_src, 
                        (u_char *)p->ah->arp_sha, 6) != 0)
                {
                    SetEvent(&event, GENERATOR_SPP_ARPSPOOF, 
                            ARPSPOOF_ETHERFRAME_ARP_MISMATCH_SRC, 1, 0, 0, 0);

                    CallAlertFuncs(p, ARPSPOOF_ETHERFRAME_ARP_MISMATCH_SRC_STR, 
                            NULL, &event);
                    DEBUG_WRAP(DebugMessage(DEBUG_PLUGIN, 
                            "MODNAME: Ethernet/ARP mismatch reply src\n"););
                }
                else if (memcmp((u_char *)p->eh->ether_dst, 
                        (u_char *)p->ah->arp_tha, 6) != 0)
                {
                    SetEvent(&event, GENERATOR_SPP_ARPSPOOF, 
                            ARPSPOOF_ETHERFRAME_ARP_MISMATCH_DST, 1, 0, 0, 0);

                    CallAlertFuncs(p, ARPSPOOF_ETHERFRAME_ARP_MISMATCH_DST_STR,
                            NULL, &event);
                    DEBUG_WRAP(DebugMessage(DEBUG_PLUGIN,
                            "MODNAME: Ethernet/ARP mismatch reply dst\n"););
                }
                break;
        }
        /* LookupIPMacEntryByIP() is too slow, will be fixed later */
        memcpy(&addr, p->ah->arp_spa, 4);

        if ((ipme = LookupIPMacEntryByIP(ipmel, addr)) == NULL)
        {
            DEBUG_WRAP(DebugMessage(DEBUG_PLUGIN, 
                    "MODNAME: LookupIPMacEntryByIp returned NULL\n"););
            return;
        }
        else
        {
            DEBUG_WRAP(DebugMessage(DEBUG_PLUGIN, 
                    "MODNAME: LookupIPMacEntryByIP returned %p\n", ipme););
            if ((memcmp((u_int8_t *)p->eh->ether_src, 
               (u_int8_t *)ipme->mac_addr, 6) != 0) || 
               (memcmp((u_int8_t *)p->ah->arp_sha, 
               (u_int8_t *)ipme->mac_addr, 6) != 0))
            {
                SetEvent(&event, GENERATOR_SPP_ARPSPOOF, 
                        ARPSPOOF_ARP_CACHE_OVERWRITE_ATTACK, 1, 0, 0, 0);

                CallAlertFuncs(p, ARPSPOOF_ARP_CACHE_OVERWRITE_ATTACK_STR,
                        NULL, &event);

                DEBUG_WRAP(DebugMessage(DEBUG_PLUGIN, 
                        "MODNAME: Attempted ARP cache overwrite attack\n"););
            }
       } 
    }
    else
    {
        /* p, eh or ah was NULL, either way it's a non-happy packet */
        return;
    }
}

void ARPspoofCleanExitFunction(int signal)
{
    FreeIPMacEntryList(ipmel);
    free(ipmel);
    ipmel = NULL;
}

void ARPspoofRestartFunction(int signal)
{
       /* restart code goes here */
}

void ARPspoofInit(u_char *args)
{
    DEBUG_WRAP(DebugMessage(DEBUG_INIT, 
            "MODNAME Initialized\n"););

    /* parse the argument list from the rules file */
    ParseARPspoofArgs(args);
    AddFuncToPreprocList(ARPspoofPreprocFunction);

    /* Set the preprocessor function into the function list
    AddFuncToCleanExitList(ARPspoofCleanExitFunction);
    AddFuncToRestartList(ARPspoofRestartFunction); */

    ipmel = (IPMacEntryList *)malloc(sizeof(IPMacEntryList));
    memset(ipmel, 0, sizeof(IPMacEntryList));
}

void PrintIPMacEntryList(IPMacEntryList *ipmel)
{
    IPMacEntryListNode *current;
    int i;
    struct in_addr in;
    if (ipmel == NULL)
        return;

    current = ipmel->head;
    printf("MODNAME: IPMacEntry List");
    printf("  Size: %i\n", ipmel->size);
    while (current != NULL)
    {
        in.s_addr = current->ip_mac_entry->ipv4_addr;
        printf("%s -> ", inet_ntoa(in));
        for (i = 0; i < 6; i++)
        {
            printf("%02x", current->ip_mac_entry->mac_addr[i]);
            if (i != 5)
                printf(":");
        }
        printf("\n");
        current = current->next;
    }    
}


void ARPwatchArg(u_char *args)
{
    char **toks;
    char **macbytes; 
    int num_toks, num_macbytes;
    int i;
    struct in_addr IP_struct;
    IPMacEntry *ipme = NULL;

    if (ipmel == NULL)
    {
        FatalError("%s %s(%d) => Cannot initialize"
                   "arpspoof_detect_host without arpspoof\n", MODNAME, file_name,
                   file_line);
    }

    toks = mSplit(args, " ", 2, &num_toks, '\\');

    if (num_toks != 2)
    {
        FatalError("%s %s(%d) => Invalid arguments to "
                   "arpspoof_detect_host\n", MODNAME, file_name, 
                   file_line);
    }

    /* Add entries */
    ipme = (IPMacEntry *)malloc(sizeof(IPMacEntry));
    /* XXX: OOM Check */
    memset(ipme, 0, sizeof(IPMacEntry));


    if ((IP_struct.s_addr = inet_addr(toks[0])) == -1)
    {
        FatalError("%s %s(%d) => Non IP as first argument of "
                   "IP/MAC pair to arpspoof_detect_host ... ignoring\n", MODNAME, 
                   file_name, file_line);
    }

    ipme->ipv4_addr = (u_int32_t)IP_struct.s_addr;

    macbytes = mSplit(toks[1], ":", 6, &num_macbytes, '\\');

    if (num_macbytes < 6)
    {
        FatalError("%s %s(%d) => Invalid MAC address as second "
                   "argument of IP/MAC pair to arpspoof_detect_host ... "
                   , MODNAME, file_name, file_line);
    }
    else
    {
        for (i = 0; i < 6; i++)
            ipme->mac_addr[i] = (u_int8_t) strtoul(macbytes[i], NULL, 16);
    }

    AddIPMacEntryToList(ipmel, ipme);

    for (i = 0; i < num_toks; i++)
        free(toks[i]);

    for (i = 0; i < num_macbytes; i++)
        free(macbytes[i]);

#ifdef DEBUG
    PrintIPMacEntryList(ipmel);
#endif
}
