#ifndef _DBG_H
#define _DBG_H
#include <stdio.h>

#ifdef DEBUG 
#define mx_debug(fmt, args...) fprintf(stderr, "%s: "fmt, __FUNCTION__, ##args)
#else
#define mx_debug(fmt, args...) 
#endif

#define mx_print(fmt, args...) fprintf(stderr, fmt, ##args)

#endif

