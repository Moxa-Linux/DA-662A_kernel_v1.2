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

#ifndef	__STREXT_H__
#define	__STREXT_H__

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_PATH	260


/**
 * @brief allocate a memory space for string
 * @param str string
 * @return error:NULL success:pointer
 */
char * xstrdup(char *str);


/**
 * @brief convert the string to lower case
 * @param str string
 * @return error:NULL success:pointer
 */
char * strlwr(char *str);

/**
 * @brief convert the string to upper case
 * @param str string
 * @return error:NULL success:pointer
 */
char * strupr(char *str);


/**
 * @brief trim the unrequired characters
 * @param str string
 * @param trim characters to be trim
 * @return error:NULL success:pointer
 */
char * strtrim(char *str, const char *trim);

/**
 * @brief trim the unrequired characters from right side
 * @param str string
 * @param trim characters to be trim
 * @return error:NULL success:pointer
 */
char * strrtrim(char *str, const char *trim);

/**
 * @brief trim the unrequired characters from left side
 * @param str string
 * @param trim characters to be trim
 * @return error:NULL success:pointer
 */
char * strltrim(char *str, const char *trim);

#endif
