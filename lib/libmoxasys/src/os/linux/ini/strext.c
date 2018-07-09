/**
 * Copyright (C) 1999-2000 Open System Telecom Corporation.
 *  
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <strext.h>

/**
 * @brief allocate a memory space for string
 * @param str string
 * @return error:NULL success:pointer
 */
char * xstrdup(char *str)
{
	char *dest;


	if (NULL == str)
		return NULL;
	dest = malloc(strlen(str) + 1);
	if (dest)
		strcpy(dest, str);
	return dest;
}


/**
 * @brief trim the unrequired characters
 * @param str string
 * @param trim characters to be trim
 * @return error:NULL success:pointer
 */
char * strtrim(char *str, const char *trim)
{
	return strltrim(strrtrim(str, trim), trim);
}

/**
 * @brief trim the unrequired characters from right side
 * @param str string
 * @param trim characters to be trim
 * @return error:NULL success:pointer
 */
char * strrtrim(char *str, const char *trim)
{
	char *end;
	

	if (!str)
		return NULL;

	if (!trim)
		trim = " \t\n\r";
		
	end = str + strlen(str);
	while (end-- > str)
	{
		if (NULL == strchr(trim, *end))
			return str;
		*end = 0;
	}
	return str;
}

/**
 * @brief trim the unrequired characters from left side
 * @param str string
 * @param trim characters to be trim
 * @return error:NULL success:pointer
 */
char *strltrim(char *str, const char *trim)
{
	char *head = str;


	if (!str)
		return NULL;
	
	if (!trim)
		trim = " \t\r\n";
	
	while (*str)
	{
		if (NULL == strchr(trim, *str))
		{
			strcpy(head, str);
			return str;
		}
		*str = 0;
		str++;
	}
	return str;
}


/**
 * @brief convert the string to lower case
 * @param str string
 * @return error:NULL success:pointer
 */
char * strlwr(char *str)
{
	char *old;

	if(!str)
		return NULL;
		
	old = str;
	while ((*str = (char)tolower(*str)))
		++str;
	
	return old;
}

/**
 * @brief convert the string to upper case
 * @param str string
 * @return error:NULL success:pointer
 */
char * strupr(char *str)
{
	char    *old;

	if(!str)
		return NULL;
		
	old = str;
	while ((*str = (char)toupper(*str)))
		++str;
	return old;
}

