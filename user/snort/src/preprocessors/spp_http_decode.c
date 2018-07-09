/*
** Copyright (C) 1998-2002 Martin Roesch <roesch@sourcefire.com>
** Copyright (C) 2002 Sourcefire, Inc.
**                    Chris Green <cmg@sourcefire.com>
** Copyright (C) 2002 Rain Forest Puppy <rfp@wiretrip.net>
** Copyright (C) 2002 Sourcefire, Inc.
**                    Daniel Roelker <droelker@sourcefire.com>
**
** 11.1.02 (DJR): Fixed off-by bugs.  Fixed %25 encoding.  Fixed
**                %u encoding for ascii codepoints.
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
** $Id: spp_http_decode.c,v 1.56 2003/03/26 21:59:47 chrisgreen Exp $
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <stdlib.h>
#include <limits.h>
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include "mstring.h"
#include "log.h"
#include "plugbase.h"
#include "util.h"
#include "decode.h"
#include "debug.h"
#include "generators.h"
#include "detect.h"
#include "spp_http_decode.h"


#define MODNAME "spp_http_decode"

#define OPT_UNICODE	"unicode"
#define OPT_ALTUNICODE	"iis_alt_unicode"
#define OPT_DOUBLE_ENC	"double_encode"
#define OPT_INVALIDHEX	"abort_invalid_hex"
#define OPT_URL_PARAM	"drop_url_param"
#define OPT_FLIP_SLASH	"iis_flip_slash"
#define OPT_WHITESPACE	"full_whitespace"
#define OPT_INTENAL_ALERTS "internal_alerts"

#define PROFILE_APACHE	"apache"
#define PROFILE_IIS 	"iis"

#define UNICODE_REPLACEMENT 0xff

static int      check_iis_unicode = 0;
static int      check_double_encode = 0;
static int      abort_invalid_hex = 0;
static int      end_on_url_param = 0;
static int      iis_flip_slash = 0;
static int      check_alt_unicode = 0;
static int      http_decode_ignore = 0;
static int      internal_alerts = 0;

extern int      do_detect;

static char lookup_whitespace[256];
static char lookup_hexvalue[256];

/* struct in_addr naddr; */

typedef struct _serverNode	/* for keeping track of our network's servers */
{
    IpAddrSet      *address;
    /*
     * u_long address; u_long netmask;
     */
    char            ignoreFlags;
    struct _serverNode *nextNode;
}               ServerNode;

/* For portscan-ignorehosts */
IpAddrSet      *HttpAllocAddrNode();
void            CreateNodeList(u_char *);
IpAddrSet      *HttpIgnoreAllocAddrNode(ServerNode *);
void            HttpIgnoreParseIP(char *, ServerNode *);
int             IsAServer(Packet *);


static inline int CheckDecodeHTTP(Packet * p);
static void     HttpDecodeInit(u_char * args);
static void     HttpDecodeInitIgnore(u_char * args);
static void     PreprocUrlDecode(Packet * p);

static void     SetPorts(char *portlist);
static u_int    unidecode(char *in, u_int len, u_int * overlong_flag);

ServerNode     *serverLst;

extern char    *file_name;
extern int      file_line;
extern HttpUri  UriBufs[URI_COUNT];	/* the set of buffers that we are
					 * using to match against set in
					 * decode.c */

/* array containing info about which ports we care about */
static char     HttpDecodePorts[65536 / 8];


/*
 * Function: SetupHttpDecode()
 *
 * Purpose: Registers the preprocessor keyword and initialization
 *          function into the preprocessor list.
 *
 * Arguments: None.
 *
 * Returns: void function
 *
 */
void
SetupHttpDecode()
{
    /*
     * link the preprocessor keyword to the init function in the preproc
     * list
     */
    RegisterPreprocessor("http_decode", HttpDecodeInit);
    RegisterPreprocessor("http_decode_ignore", HttpDecodeInitIgnore);

    DEBUG_WRAP(DebugMessage(DEBUG_HTTP_DECODE,
                            "Preprocessor: HttpDecode in setup...\n"););
}


/*
 * Function: HttpDecodeInit(u_char *)
 *
 * Purpose: Processes the args sent to the preprocessor, sets up the
 *          port list, links the processing function into the preproc
 *          function list
 *
 * Arguments: args => ptr to argument string
 *
 * Returns: void function
 *
 */
static void
HttpDecodeInit(u_char * args)
{
    int             idx;
    DEBUG_WRAP(DebugMessage(DEBUG_HTTP_DECODE,
                            "Preprocessor: HttpDecode Initialized\n"););

    /* let's populate the lookup tables with the default values */
    for(idx = 0; idx < 256; idx++)
    {
        lookup_whitespace[idx] = 0;
        lookup_hexvalue[idx] = -1;
    }

    /* the default whitespace characters */
    lookup_whitespace[32] = 1;	/* ' '  */
    lookup_whitespace[10] = 1;	/* '\n' */

    /*
     * the hexvalues table doubles as a isxdigit() replacement as well,
     * allowing us to kill two birds with one stone and cut out a lot of
     * redundant processing time
     */
    lookup_hexvalue[48] = 0;/* 0x30 '0' */
    lookup_hexvalue[49] = 1;/* 0x31 '1' */
    lookup_hexvalue[50] = 2;/* 0x32 '2' */
    lookup_hexvalue[51] = 3;/* 0x33 '3' */
    lookup_hexvalue[52] = 4;/* 0x34 '4' */
    lookup_hexvalue[53] = 5;/* 0x35 '5' */
    lookup_hexvalue[54] = 6;/* 0x36 '6' */
    lookup_hexvalue[55] = 7;/* 0x37 '7' */
    lookup_hexvalue[56] = 8;/* 0x38 '8' */
    lookup_hexvalue[57] = 9;/* 0x39 '9' */
    lookup_hexvalue[65] = 10;	/* 0x41 'A' */
    lookup_hexvalue[66] = 11;	/* 0x42 'B' */
    lookup_hexvalue[67] = 12;	/* 0x43 'C' */
    lookup_hexvalue[68] = 13;	/* 0x44 'D' */
    lookup_hexvalue[69] = 14;	/* 0x45 'E' */
    lookup_hexvalue[70] = 15;	/* 0x46 'F' */
    lookup_hexvalue[97] = 10;	/* 0x61 'a' */
    lookup_hexvalue[98] = 11;	/* 0x62 'b' */
    lookup_hexvalue[99] = 12;	/* 0x63 'c' */
    lookup_hexvalue[100] = 13;	/* 0x64 'd' */
    lookup_hexvalue[101] = 14;	/* 0x65 'e' */
    lookup_hexvalue[102] = 15;	/* 0x66 'f' */

    /*
     * Parse the argument list into a list of ports to normalize. We
     * first reset the entire port list and then make port 80 the
     * default.
     */
    bzero(&HttpDecodePorts, sizeof(HttpDecodePorts));
    HttpDecodePorts[(80 / 8)] |= 1 << (80 % 8);
    SetPorts(args);

    /* Set the preprocessor function into the function list */
    AddFuncToPreprocList(PreprocUrlDecode);
}

void
HttpDecodeInitIgnore(u_char * args)
{
    DEBUG_WRAP(DebugMessage(DEBUG_HTTP_DECODE, "Preprocessor: HttpDecodeIgnore Initialized\n");
               );
    http_decode_ignore = 1;
    DEBUG_WRAP(DebugMessage(DEBUG_HTTP_DECODE, "Hosts: %s\n", args);
               );
    CreateNodeList(args);
}


/*
 * Function: SetPorts(char *)
 *
 * Purpose: Reads the list of port numbers from the argument string and
 *          parses them into the port list data struct
 *
 * Arguments: portlist => argument list
 *
 * Returns: void function
 *
 */
static void
SetPorts(char *portlist)
{
    char            portstr[STD_BUF];
    char          **toks;
    int             is_reset = 0;
    int             num_toks;
    int             num;

    if(portlist == NULL || *portlist == '\0')
    {
        FatalError("%s (%d) => No arguments to http_decode "
                   "preprocessor!\n", file_name, file_line);
    }
    /* default port list string */
    strlcpy(portstr, "80", STD_BUF);

    /* tokenize the argument list */
    toks = mSplit(portlist, " ", 31, &num_toks, '\\');

    LogMessage("http_decode arguments:\n");

    /* convert the tokens and place them into the port list */
    for(num = 0; num < num_toks; num++)
    {
        if(!strcasecmp(OPT_UNICODE, toks[num]))
        {
            check_iis_unicode = 1;
            LogMessage("    Unicode decoding\n");
        }
        else if(!strcasecmp(OPT_ALTUNICODE, toks[num]))
        {
            check_alt_unicode = 1;
            LogMessage("    IIS alternate Unicode decoding\n");
        }
        else if(!strcasecmp(OPT_DOUBLE_ENC, toks[num]))
        {
            check_double_encode = 1;
            LogMessage("    IIS double encoding vuln\n");
        }
        else if(!strcasecmp(OPT_INVALIDHEX, toks[num]))
        {
            abort_invalid_hex = 1;
            LogMessage("    Stop on invalid hex encoding\n");
        }
        else if(!strcasecmp(OPT_URL_PARAM, toks[num]))
        {
            end_on_url_param = 1;
            LogMessage("    Stop on URL parameter\n");
        }
        else if(!strcasecmp(OPT_FLIP_SLASH, toks[num]))
        {
            LogMessage("    Flip backslash to slash\n");
            iis_flip_slash = 1;
        }
        else if(!strcasecmp(OPT_WHITESPACE, toks[num]))
        {
            lookup_whitespace[9] = 1;	/* TAB  */
            lookup_whitespace[13] = 1;	/* '\r' */
            LogMessage("    Include additional whitespace separators\n");
        }
        else if(!strcasecmp(PROFILE_APACHE, toks[num]))
        {
            /* abort_invalid_hex = 1;  */
            lookup_whitespace[9] = 1;	/* TAB  */
            lookup_whitespace[13] = 1;	/* '\r' */
            LogMessage("    Apache profile\n");
        }
        else if(!strcasecmp(PROFILE_IIS, toks[num]))
        {
            check_iis_unicode = 1;
            check_alt_unicode = 1;
            check_double_encode = 1;
            iis_flip_slash = 1;
            LogMessage("    IIS profile\n");
        }
        else if(!strcasecmp(OPT_INTENAL_ALERTS, toks[num]))
        {
            internal_alerts = 1;
            LogMessage("    Internal Alerts enabled\n");
        }
        else if(isdigit((int) toks[num][0]))
        {
            char           *num_p = NULL;	/* used to determine
                                                 * last position in
                                                 * string */
            long            t_num;

            t_num = strtol(toks[num], &num_p, 10);


            if(*num_p != '\0')
            {
                FatalError("Port Number invalid format: %s\n", toks[num]);
            }
            else if(t_num < 0 || t_num > 65335)
            {
                FatalError("Port Number out of range: %ld\n", t_num);
            }
            /*
             * user specified a legal port number and it should
             * override the default port list, so reset it unless
             * already done
             */
            if(!is_reset)
            {
                bzero(&HttpDecodePorts, sizeof(HttpDecodePorts));
                portstr[0] = '\0';
                is_reset = 1;
            }
            /*
             * mark this port as being interesting using some
             * portscan2-type voodoo, and also add it to the port
             * list string while we're at it so we can later
             * print out all the ports with a single LogMessage()
             */
            HttpDecodePorts[(t_num / 8)] |= 1 << (t_num % 8);
            strlcat(portstr, toks[num], STD_BUF - 1);
            strlcat(portstr, " ", STD_BUF - 1);
        }
        else
        {
            FatalError("%s(%d) => Unknown argument to http_decode "
                       "preprocessor: \"%s\"\n",
                       file_name, file_line, toks[num]);
        }
    }

    /* print out final port list */
    LogMessage("    Ports to decode http on: %s\n", portstr);
}


/*
 * Function: CheckUrlDecode(Packet *p)
 *
 * Purpose: Inspect the packet to see if we should process the packet or not
 *
 * Arguments: p => pointer to the current packet data struct
 *
 */
static inline int
CheckDecodeHTTP(Packet * p)
{

    if(p->frag_flag)
    {
        DEBUG_WRAP(DebugMessage(DEBUG_HTTP_DECODE, "This is a fragmented packet\n");
                   );
        return 0;
    }
    /*
     * check to make sure we're talking TCP and that the TWH has already
     * completed before processing anything
     */
    if(!PacketIsTCP(p))
    {
        DEBUG_WRAP(DebugMessage(DEBUG_HTTP_DECODE, "It isn't TCP session traffic 1\n");
                   );
        return 0;
    }

    /*
     * check to make sure the packet isn't emtpy before we go on.
     */
    if(p->dsize == 0)
    {
        DEBUG_WRAP(DebugMessage(DEBUG_HTTP_DECODE, "Empty packet\n"););
        return 0;
    }

    /* If it has a source address from the http_decode_ignore */
    if(http_decode_ignore)
    {
        if(IsAServer(p))
        {
            DEBUG_WRAP(DebugMessage(DEBUG_HTTP_DECODE, "You said to ignore this one...\n");
                       );
            return 0;
        }
    }
    if(HttpDecodePorts[(p->dp / 8)] & (1 << (p->dp % 8)))
    {
        /* on match, normalize the data */
        DEBUG_WRAP(DebugMessage(DEBUG_HTTP_DECODE,
                                "Got HTTP traffic (%d bytes @ %p)!\n",
                                p->dsize, p->data);
                   DebugMessage(DEBUG_HTTP_DECODE, "%s\n", p->data);
                   );

        return 1;	/* yes we should decode this port */
    }
    DEBUG_WRAP(DebugMessage(DEBUG_HTTP_DECODE,
                            "Not interested in this port (%d)\n", p->dp);
               );
    return 0;
}

static void
PreprocUrlDecode(Packet * p)
{
    char           *url;	/* this is where the converted data will be
				 * written */
    char           *index;	/* this is the index pointer to walk thru the
				 * data */
    char           *end;	/* points to the end of the payload, for loop
				 * control */
    char           *cur;	/* current decode ptr */
    u_int16_t       psize;	/* payload size */

    /* register */ int hex1 = 0, hex2 = 0;	/* hex lookup values */
    u_int           overlong_flag;
    int             junk;
    Event           event;

    DEBUG_WRAP(DebugMessage(DEBUG_HTTP_DECODE,
                            "http decoder init on %d bytes\n", p->dsize););


    /* reset the uri count for each packet */
    p->uri_count = 0;

    if(!CheckDecodeHTTP(p))
    {
        /* Don't normalize this packet */
        return;
    }
    /* setup the pointers */
    url = (u_int8_t *) UriBufs[0].uri;

    index = (char *) p->data;	/* index into the data portion of the
                                 * packet */
    end = (char *) p->data + p->dsize;
    psize = (u_int16_t) (p->dsize);

    /* first skip past the HTTP method */
    while(index < end && !lookup_whitespace[(u_int) (*index)])
        index++;

    /* skip over whitespace seperator */
    while(index < end && lookup_whitespace[(u_int) (*index)])
        index++;

    /* evilness check */
    if((index - (char *) (p->data)) > 10)
    {
        /*
         * the request had an abnormally-large HTTP method and/or
         * amount of separating whitespace.  This is only typical of
         * attacks: - attempting a buffer overflow - attempting IDS
         * evasion - attempting to screw with the HTTP server logs
         */
        if(internal_alerts)
        {
            SetEvent(&event, GENERATOR_SPP_HTTP_DECODE,
                     HTTP_DECODE_LARGE_METHOD, 1, 0, 0, 0);
            CallAlertFuncs(p, HTTP_DECODE_LARGE_METHOD_STR, NULL, &event);
            CallLogFuncs(p, HTTP_DECODE_LARGE_METHOD_STR, NULL, &event);
        }
        
        DEBUG_WRAP(DebugMessage(DEBUG_HTTP_DECODE,
                                "A large HTTP method was received\n"););
        return;
    }
    if(index == end)
    {
        /*
         * gross...malformed request with no URI? I guess a very
         * weird HTTP 0.9 request could trigger this, but odds are
         * something evil is happening.
         */
        if(internal_alerts)
        {
            SetEvent(&event, GENERATOR_SPP_HTTP_DECODE,
                     HTTP_DECODE_MISSING_URI, 1, 0, 0, 0);
            CallAlertFuncs(p, HTTP_DECODE_MISSING_URI_STR, NULL, &event);
            CallLogFuncs(p, HTTP_DECODE_MISSING_URI_STR, NULL, &event);
        }
        DEBUG_WRAP(DebugMessage(DEBUG_HTTP_DECODE,
                                "HTTP request with no URI!\n");
                   );

        return;
    }
    DEBUG_WRAP(DebugMessage(DEBUG_HTTP_DECODE, "set URI at %p\n", UriBufs[0].uri);
               );

    /* walk thru each char in the payload */

    /*
     * this is a speed hack to avoid unnecessary *cur=*index copying
     * found in the next loop.  Think of it as a precursor strchr,
     * without having to readjust pointers and run strlen's
     *
     * Would be included if we could munge packets in place but we need the
     * original packet data to stay around
     *
     * do { if(*index == '%' || lookup_whitespace[(u_int)(*index)]) break;
     * if(*index == '?'  && end_on_url_param) break; if(*index == '\\' &&
     * iis_flip_slash) (*index) = '/'; } while(index++ < end);
     *
     */

    cur = UriBufs[0].uri;

    /* Main Decode Loop */
    while(index < end && ((cur - (char *) UriBufs[0].uri) < URI_LENGTH))
    {
        if(*index == '%')
        {
            if(check_alt_unicode && ((index + 5) < end) &&
               ((*(index + 1)) == 'u' || (*(index + 1)) == 'U'))
            {
                /*
                **  alternate IIS unicode encoding.
                **
                **  (DJR: 11.1.02)
                **  %u encoding references the unicode
                **  codepoints, which are not encoded in utf-8
                **  (e.g. %c0%af = codepoint 0x2f).  In the
                **  case of IIS, it maps multiple codepoints
                **  to ascii characters and this changes
                **  according to the IIS codepoint maps.
                **  Right now we are fixing the %u to work
                **  with %u00xx (ascii) codepoints.
                **
                **  In a future version, we will map the
                **  appropriate codepoints for the default IIS
                **  servers.
                */

                junk = 0;
                hex1 = lookup_hexvalue[(u_int) (*(index + 2))];
                hex2 = lookup_hexvalue[(u_int) (*(index + 3))];
                if(hex1 != -1 && hex2 != -1)
                {
                    junk = ((hex1 & 0x0f) << 12) + ((hex2 & 0x0f) << 8);

                    hex1 = lookup_hexvalue[(u_int) (*(index + 4))];
                    hex2 = lookup_hexvalue[(u_int) (*(index + 5))];
                    if(hex1 != -1 && hex2 != -1)
                    {
                        junk += (((hex1 & 0x0f) << 4) + (hex2 & 0x0f));

                        if(junk <= 0x7f)
                        {
                            *cur = (char) junk;
                            cur++;
                            index += 6;
                            continue;
                        }
                    }
                }
            }

            /*
            **  IIS Double Encoding
            **  There are different ways to do double encoding.
            **  This way covers the %25xx variant.  I'll cover
            **  the other variants in a future version.
            */
            if(check_double_encode && ((index + 2) < end) && 
                (*(index + 1) == '2') && (*(index + 2) == '5'))
            {
                /*
                 * double encoded URL, which is a problem
                 * amongst IIS servers
                 */
                if(internal_alerts)
                {
                    SetEvent(&event, GENERATOR_SPP_HTTP_DECODE,
                             HTTP_DECODE_DOUBLE_ENC, 1, 0, 0, 0);
                    CallAlertFuncs(p, HTTP_DECODE_DOUBLE_ENC_STR,
                                   NULL, &event);
                    CallLogFuncs(p, HTTP_DECODE_DOUBLE_ENC_STR, NULL, &event);
                    return;
                }
                DEBUG_WRAP(DebugMessage(DEBUG_HTTP_DECODE,
                                        "Double hex encoding received\n"););

                /*
                **  Move index ahead to real bytes to decode,
                **  if this isn't a triple double decode.
                **  %25%xx%xx
                */
                if(((index + 3) < end) && (*(index + 3) != 0x25))
                {
                    index += 2;
                }
            }

            if((index + 2) < end)
            {
                hex1 = lookup_hexvalue[(u_int) (*(index + 1))];
                hex2 = lookup_hexvalue[(u_int) (*(index + 2))];
            }
            else
            {
                hex1 = -1;
                hex2 = -1;
            }

            if(hex1 != -1 && hex2 != -1)
            {
                *cur = ((hex1 & 0x0f) << 4) + (hex2 & 0x0f);
                cur++;
                index += 3;
                psize -= 2;
                continue;
            }
            else
            {
                /*
                 * illegal character...they tried to submit a
                 * non-valid hex character, with is typical
                 * of an anti-IDS subversion or DoS attack
                 */
                if(internal_alerts)
                {
                    SetEvent(&event, GENERATOR_SPP_HTTP_DECODE,
                             HTTP_DECODE_ILLEGAL_HEX, 1, 0, 0, 0);
                    CallAlertFuncs(p, HTTP_DECODE_ILLEGAL_HEX_STR, NULL, &event);
                    CallLogFuncs(p, HTTP_DECODE_ILLEGAL_HEX_STR, NULL, &event);
                    return;
                }
                DEBUG_WRAP(DebugMessage(DEBUG_HTTP_DECODE,
                                        "Illegal URL hex encoding received\n"););

                /*
                 * Apache will not continue processing the
                 * request, so there's no reason we should
                 * either.
                 */
                if(abort_invalid_hex)
                {
                    /* what should we do with the URI? */

                    *cur = '\0';
                    return;
                }

                /* otherwise we continue going through chars */
                *cur = *index;
                cur++;
                index++;
                continue;
            }
        }
        else if(lookup_whitespace[(u_int) (*index)])
        {
            /* we've reached the delimiting whitespace */
            /* UriBufs[0].http_version = (u_int8_t *) index; */
            break;

        }
        else
        {
            if((*index == '?') && end_on_url_param)
            {
                /*
                 * time to just start copying bytes into the
                 * URI buffer
                 */
                *cur = *index;            
                while(index < end && ((cur - (char *) UriBufs[0].uri) < URI_LENGTH))
                {
                    if(lookup_whitespace[(u_int) (*index)])
                    {
                        /*
                         * we've reached the
                         * delimiting whitespace
                         */
                        goto end_decode_loop;
                        /* breaks out of outer while */
                    }
                    *cur = *index;
                    cur++;
                    index++;
                    
                    if(index >= end ||
                       ((cur - (char *) UriBufs[0].uri) >= URI_LENGTH))
                    {
                        goto end_decode_loop;
                    }
                }

            }

            if(iis_flip_slash && *index == '\\')
            {
                *cur = '/';
            }
            else
            {
                *cur = *index;
            }

            cur++;
            index++;
        }
    }			/* Main Decode Loop */

 end_decode_loop:
    *cur = '\0';

    if(check_iis_unicode)
    {
        overlong_flag = 0;

        UriBufs[0].length = unidecode((char *) UriBufs[0].uri,
                                      (cur - (char *) UriBufs[0].uri),
                                      &overlong_flag);
        
        /*
         * overlong characters are technically illegal in the UTF-8
         * spec, and receiving any means that odds are the person is
         * up to no good.  Keep in mind, this is just overlong
         * characters, as opposed to valid, non-overlong Unicode
         * characters.  Finding valid, non-overlong characters
         * depends on the mapping/locale of the target server.
         */
        if(overlong_flag)
        {
            if(internal_alerts)
            {
                DEBUG_WRAP(DebugMessage(DEBUG_HTTP_DECODE,
                                        "Overlong Unicode character received\n");
                           );
                SetEvent(&event, GENERATOR_SPP_HTTP_DECODE,
                         HTTP_DECODE_OVERLONG_CHAR, 1, 0, 0, 0);
                CallAlertFuncs(p, HTTP_DECODE_OVERLONG_CHAR_STR, NULL, &event);
                CallLogFuncs(p, HTTP_DECODE_OVERLONG_CHAR_STR, NULL, &event);
                return;
            }
        }
    }
    else
    {
        UriBufs[0].length = (cur - (char *) UriBufs[0].uri);
    }

    if(UriBufs[0].length)
    {
        p->uri_count = 1;
    }

#ifdef DEBUG
    DebugMessage(DEBUG_HTTP_DECODE, "URI length: %d\n",
                 UriBufs[0].length);
    DebugMessage(DEBUG_HTTP_DECODE, "URI data (%s):\n",
                 UriBufs[0].uri);
    ClearDumpBuf();
    /* PrintNetData(stdout, (char *) UriBufs[0].uri, UriBufs[0].length); */
    ClearDumpBuf();
#endif
    return;
}

void
CreateNodeList(u_char * servers)
{
    char          **toks;
    int             num_toks;
    int             num_servers = 0;
    ServerNode     *currentServer;
    int             i;

    currentServer = NULL;
    serverLst = NULL;

    if(servers == NULL)
    {
        FatalError("%s (%d)=> No arguments to "
                   "http_decode_ignore preprocessor!\n", file_name, file_line);
    }
    /* tokenize the argument list */
    toks = mSplit(servers, " ", 31, &num_toks, '\\');

    /* convert the tokens and place them into the server list */
    for(num_servers = 0; num_servers < num_toks; num_servers++)
    {
        if(currentServer != NULL)
        {
            currentServer->nextNode = (ServerNode *) calloc(sizeof(ServerNode),
                                                            sizeof(char));
            currentServer = currentServer->nextNode;
        }
        else
        {
            currentServer = (ServerNode *) calloc(sizeof(ServerNode),
                                                  sizeof(char));
            serverLst = currentServer;
        }

        HttpIgnoreParseIP(toks[num_servers], currentServer);

        currentServer->nextNode = NULL;
    }

    for(i = 0; i < num_toks; i++)
    {
        free(toks[i]);
    }
}

void
HttpIgnoreParseIP(char *addr, ServerNode * server)
{
    char          **toks;
    int             num_toks;
    int             i;
    IpAddrSet      *tmp_addr;
    int             global_negation_flag;
    char           *tmp;

    if(*addr == '!')
    {
        global_negation_flag = 1;
        addr++;
    }
    if(*addr == '$')
    {
        if((tmp = VarGet(addr + 1)) == NULL)
        {
            FatalError("%s (%d) => Undefined variable %s\n", file_name,
                       file_line, addr);
        }
    }
    else
    {
        tmp = addr;
    }

    if(*tmp == '[')
    {
        *(strrchr(tmp, (int) ']')) = 0;	/* null out the en-bracket */

        toks = mSplit(tmp + 1, ",", 128, &num_toks, 0);

        for(i = 0; i < num_toks; i++)
        {
            tmp_addr = HttpIgnoreAllocAddrNode(server);

            ParseIP(toks[i], tmp_addr);
        }

        for(i = 0; i < num_toks; i++)
            free(toks[i]);
    }
    else
    {
        tmp_addr = HttpIgnoreAllocAddrNode(server);

        ParseIP(tmp, tmp_addr);
    }
}

IpAddrSet      *
HttpIgnoreAllocAddrNode(ServerNode * server)
{
    IpAddrSet      *idx;	/* IP struct indexing pointer */

    if(server->address == NULL)
    {
        server->address = (IpAddrSet *) calloc(sizeof(IpAddrSet), sizeof(char));

        if(server->address == NULL)
        {
            FatalError("Unable to allocate space for "
                       "http_decode IP addr\n");
        }
        return server->address;
    }
    idx = server->address;

    while(idx->next != NULL)
    {
        idx = idx->next;
    }

    idx->next = (IpAddrSet *) calloc(sizeof(IpAddrSet), sizeof(char));

    idx = idx->next;

    if(idx == NULL)
    {
        FatalError("Unable to allocate space for"
                   " http_decode IP address\n");
    }
    return idx;
}

/*
 * Check if packet originated from a machine we have been told to ignore SYN
 * and UDP "scans" from, presumably because it's a server.
 */
int
IsAServer(Packet * p)
{
    ServerNode     *currentServer = serverLst;

    while(currentServer)
    {
        /*
         * Return 1 if the source addr is in the serverlist, 0 if nothing is
         * found.
         */
        if(CheckAddrPort(currentServer->address, 0, 0, p,
                          (ANY_SRC_PORT | currentServer->ignoreFlags), CHECK_SRC))
        {
            return (1);
        }
        currentServer = currentServer->nextNode;
    }

    return (0);
}




/*
 * unidecode
 *
 * Basically, a UTF-8 Decoder that does overlong decodings
 *
 * Good article on UTF8
 * http://www.sun.com/developers/gadc/technicalpublications/articles/utf8.html
 *
 * This function attempts to decode a string containing unicode values; since
 * snort internally uses single-character strings, we have issues when we
 * attempt to store a multi-byte character; so, instead, we substitute
 * legitimate multi-byte characters with the character defined as
 * UNICODE_REPLACEMENT.  This is *much* better than dropping the character(s)
 * altogether, since that could potentially result in nastiness (DoS and
 * false-positives, and potentially another anti-IDS tactic).
 *
 */
static u_int unidecode(char *in, u_int len, u_int * overlong_flag)
{
    unsigned char lead;            /* lead byte */
    unsigned char not_overlong = 0; /* is this an overlong encoding? */
    unsigned char not_unicode = 0;  /* is this even unicode? */
    unsigned char tmp;     /* storage character for normalization */
    unsigned int ptr = 0;      /* current offset into in */
    unsigned int count;    /* length of the current encoding */
    unsigned int idx;      /* temporary index for normalization */
    unsigned int cur = 0; /* current offset into in */

    *overlong_flag = 0;	/* number of found overlong sequences */

    while(ptr < len)
    {
        if((unsigned char) in[ptr] >= 0xc0 && (unsigned char) in[ptr] <= 0xfd)
        {
            lead = in[ptr];

            count = 0;
            lead <<= 1;

            while((lead & 0x80) == 0x80)  /* How long is this encoding */
            {
                count++;
                lead <<= 1;
                if(count >= 5)	/* assumption: values <=32 bits */
                    break;
            }

            if(ptr + count >= len)
            {
                break;
            }
            
            /* figure out what kind of code mappings we have */
            for(idx = 1; idx < count; idx++)
            {
                tmp = in[ptr + idx];
                if(tmp != 0x80)
                    not_overlong = 1;
                if(tmp < 0x80 || tmp > 0xbf)
                    not_unicode = 1;
            }

            /* grab the last byte */
            tmp = in[ptr + count];
            
            if(tmp < 0x80 || tmp > 0xbf)
            {
                not_unicode = 1;
            }

            if(not_unicode)
            {
                in[cur] = in[ptr];
            }
            else
            {
                if(not_overlong)
                {
                    in[cur] = UNICODE_REPLACEMENT;
                }
                else
                {
                    (*overlong_flag)++;
                    /* high two bits in previous byte */
                    tmp = in[ptr + count - 1] << 6;
                    in[cur] = (in[ptr + count] & 0x7f) + tmp;
                }
            }

            cur++;
            
            if(count)
            {
                ptr += count;
            }
            else
            {
                ptr++;
            }
                    
            not_overlong = 0;
            not_unicode = 0;

        }
        else
        {

            in[cur] = in[ptr];
            cur++;
            ptr++;

        }
    } /* while */

    return cur;
}
