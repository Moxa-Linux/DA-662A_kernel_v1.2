#include "basicFunc.h"

/* swap two integer */
void swap(int *a, int *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

/* Search the string "needle" to string "s",
 * and get the first index.
 *
 * retval:
 * 	-1: data error.
 * 	0: none
 * 	n: index number
 */
int strstrnum(char *s, char *needle)
{
	char *lable = NULL;

	if (NULL == s || NULL == needle)
		return -1;

	lable = strstr(s, needle);
	if (NULL == lable)
		return 0;

	return abs(&lable[0] - &s[0])/sizeof(char) + 1;
}

/* Count the number of 'c' in string "s"
 *
 * retval:
 * 	-1: data error.
 * 	n: count number
 */
int strchrcount(char *s, char c)
{
	int length = 0;
	int count = 0;

	if (NULL == s)
		return -1;

	length = strlen(s);
	do {
		if (s[--length] == c)
			count++;
	} while(length);

	return count;
}
