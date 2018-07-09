#ifndef _FILTERCONTAINER_H_
#define _FILTERCONTAINER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "basicFunc.h"

/* define global variables */
#define FILTER_MAX_LEN 300

/* declare filter list 
 *
 * variables:
 * 	option: content inside the "()"
 * 	optionCount: the numbers of options
 * 	operator: '/', '^'
 * 	seperator: content outside the "()"
 *
 */
struct filterList {
	char **option;
	int optionCount;
	char operator;
	char *seperator;
	struct filterList *next;
};
typedef struct filterList filterContainer;

/* declare the function prototype */
void filterFreeContainerNode(filterContainer *);
filterContainer *filterCreateContainerNode(char **, int, char, char *);
filterContainer *filterInsertContainerNode(filterContainer *, filterContainer *, int, int *);
int filterGetContainerLen(filterContainer *);
void filterDisplayContainer(filterContainer *);
filterContainer *filterFreeContainer(filterContainer *);
filterContainer *filterAddContainer(filterContainer *, char **, int, char, char *);
filterContainer *filterCreateContainer(const char *);
int filterCompareContainer(filterContainer *, const char *);

#endif
