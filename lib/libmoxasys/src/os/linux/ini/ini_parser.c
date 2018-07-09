#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ini_debug.h>
#include <strext.h>
#include <ini_parser.h>


/* minimal ini item size */
#define INI_MINSIZE     64

/* status of the ini line */
enum INI_LINE_STATUS {
	INI_LINE_UNKNOWN,
	INI_LINE_EMPTY, 
	INI_LINE_COMMENT,
	INI_LINE_SECTION,
	INI_LINE_VALUE,
	INI_LINE_ERROR = -1
};


/**
 * @brief reallocate the memory size
 * @param obj object pointer
 * @param current_size current size of the object
 * @param extend_size size to be extended
 * @param size slot size
 * @return error:NULL success:pointer
 */
static void * _extend_mem (void *obj, int current_size, int extend_size, 
                           size_t size)
{
	void * new;


	new = calloc(current_size + extend_size, size);
	if (NULL == new)
		return NULL;
	memcpy(new, obj, current_size);
	free(obj);

	return new;
}

/**
 * @brief allocate or extend ini memory storage
 * @param this ini object pointer
 * @return error:-1 success:0
 */
static int ini_alloc(ini_data_t *this)
{
	int size;


	if (NULL == this)
		return -1;

	/* extend the memory for key */
	size = this->size + INI_MINSIZE;
	this->key = (char **) _extend_mem(
			this->key, 
			this->size, 
			size, sizeof(char*));
	this->val = (char **) _extend_mem(
			this->val, 
			this->size, 
			size, sizeof(char*));
	this->hash = (unsigned int *) _extend_mem(
			this->hash, 
			this->size, 
			size, 
			sizeof(unsigned int));
	if (NULL == this->key || NULL == this->val || NULL == this->hash)
	{
		mx_ini_debug(INI_ERROR_OUT_OF_MEMORY);
		return -1;
	}

	this->size = size;
	return 0;
}

/**
 * @brief calculate a hash value by the given key
 * @param this ini object pointer
 * @param key key name
 * @return success:hash
 */
static unsigned int ini_hash(char *key)
{
	int i;
	int len;
	unsigned int hash;


	len = strlen(key);
	for (hash = 0, i = 0; i < len; i++)
	{
		hash += (unsigned int) key[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}

/**
 * @brief find the index of the given key
 * @param this ini object pointer
 * @param key key name
 * @return error:-1 success:index
 */
static int ini_find(ini_data_t *this, char *key)
{
	int i;
	unsigned int hash;


	if (NULL == this || NULL == key || this->num <= 0)
		return -1;

	/* hash for this key */
	hash = ini_hash(key);

	/* find duplicated value */
	for (i = 0; i < this->size; i++)
	{
		if (NULL == this->key[i])
			continue;
		/* hash & key value was the same */
		if (hash == this->hash[i] && 0 == strcmp(key, this->key[i]))
			return i;
	}
	return -1;
}

/**
 * @brief add or update a key value
 * @param this ini object pointer
 * @param key key name
 * @param value the key value
 * @return success:0
 *
 * ini_set function is used to add or update an ini key value in a ini object.
 * The format of the key should be 
 *	"<section>:<key>".
 */
int ini_set(ini_data_t *this, char *key, char *value)
{
	int i;
	unsigned int hash;


	if (NULL == this || NULL == key)
	{
		mx_ini_debug(INI_ERROR_NULL_POINTER);
		return -1;
	}

	/* hash for this key */
	hash = ini_hash(key);

	/* find duplicated value */
	i = ini_find(this, key);
	if (i >= 0)
	{
		/* modified the value */
		if (this->val[i])
			free(this->val[i]);
		this->val[i] = value ? xstrdup(value) : NULL;
		return 0;
	}

	/* add a new value */
	if (this->num == this->size)
		if (-1 == ini_alloc(this))
			return -1;

	/* find the first empty slot */
	for (i = 0; i < this->size; i++)
		if (NULL == this->key[i])
			break;

	this->key[i] = xstrdup(key);
	this->val[i] = value ? xstrdup(value): NULL;
	this->hash[i] = hash;
	this->num++;
	return 0;
}

/**
 * @brief get a key value
 * @param key the specified key name
 * @return success:0
 *
 * ini_get function is used to get a key value from an ini object.
 * The format of the key should be 
 *	"<section>:<key>".
 */
char * ini_get(ini_data_t *this, char *key)
{
	int i;
	unsigned int hash;


	if (NULL == this || NULL == key)
	{
		mx_ini_debug(INI_ERROR_NULL_POINTER);
		return NULL;
	}

	hash = ini_hash(key);
	for (i = 0; i < this->size; i++)
	{
		if (NULL == this->key[i])
			continue;

		if (hash == this->hash[i])
		{
			if (0 == strcmp(key, this->key[i]))
				return this->val[i];
		}
	}
	return NULL;
}

/**
 * @brief parse one line of the ini file
 * @param line line to be parsed
 * @param section record the section name
 * @param key record the key name
 * @param value record the key value
 * @return success:0
 */
static int ini_parse_line(char * line, char *section, char *key, char *value)
{
	int status = INI_LINE_UNKNOWN;

	int len;
	char temp[MAX_PATH] = { 0 };


	strncpy(temp, strtrim(line, " "), sizeof(temp));
	len = strlen(temp);

	if (len < 1)
	{
		status = INI_LINE_EMPTY;
	}
	else if ('#' == line[0] || ';' == line[0])
	{
		status = INI_LINE_COMMENT;
	}
	else if ('[' == line[0] && ']' == line[len - 1])
	{
		/* [section] */
		status = INI_LINE_SECTION;
		sscanf(line, "[%[^]]", section);
		strcpy(section, strtrim(section, " "));
#if 0
		strcpy(section, strlwr(section));
#endif
	}
	else if (2 == sscanf(line, "%[^=] = \"%[^\"]\"", key, value)
              || 2 == sscanf(line, "%[^=] = '%[^\']'", key, value)
              || 2 == sscanf(line, "%[^=] = %[^;#]", key, value))
	{
		status = INI_LINE_VALUE;
		/* key = value */
		strcpy(key, strtrim(key, " "));
#if 0
		strcpy(key, strlwr(key));
#endif
		strcpy(value, strtrim(value, " "));

		/* for empty value of '' or "" */
		if (!strcmp(value, "\"\"") || (!strcmp(value, "''")))
			value[0] = 0;
	}
	else if (2 == sscanf(line, "%[^=] = %[;#]", key, value)
              || 2 == sscanf(line, "%[^=] %[=]", key, value))
	{
		/* special cases:
		 * key = 
		 * key = ;
		 * key = #
		 */
		status = INI_LINE_VALUE;
		strcpy(key, strtrim(key, " "));
#if 0
		strcpy(key, strlwr(key));
#endif
		value[0] = 0;
	}
	else
	{
		status = INI_LINE_ERROR;
	}

	return status;
}


/**
 * @brief initialize the ini object
 * @param this ini object pointer
 * @param file ini file path
 * @return success:ini object pointer
 *
 * ini_init function used by other object to initialize the memory and load
 * the ini file into prepared memory.
 */
ini_data_t * ini_init(char *file)
{
	FILE *in;
	ini_data_t *this;

	int len = 0;
	int last = 0;
	char line[MAX_PATH];
	char section[MAX_PATH];
	char sec_key[MAX_PATH];
	char key[MAX_PATH];
	char val[MAX_PATH];


	/* initialize the ini data object */
	this = (ini_data_t *) calloc(1, sizeof(ini_data_t));
	if (NULL == this || ini_alloc(this))
	{
		mx_ini_debug(INI_ERROR_OUT_OF_MEMORY);
		ini_free(this);
		return NULL;
	}

	/* open the ini file to parse */
	in = fopen(file, "r");
	if (NULL == in)
	{
		mx_ini_debug(INI_ERROR_OPEN_FAILED, file);
		ini_free(this);
		return NULL;
	}

	/* read line by line */
	while (fgets(line + last, sizeof(line) - last, in))
	{
		len = strlen(line) - 1;
		if (0 == len)
			continue;

		/* discard \n and spaces at end the line */
		while ((len > 0) && ('\n' == line[len] || isspace(line[len])))
			line[len--] = 0;

		/* multi-line value */
		if ('\\' == line[len])
		{
			last = len;
			continue;
		}
		last = 0;

		switch (ini_parse_line(line, section, key, val))
		{
			case INI_LINE_SECTION:
				ini_set(this, section, NULL);
				break;

			case INI_LINE_VALUE:
				snprintf(sec_key, sizeof(sec_key), "%s:%s", 
                                         section, key);
				ini_set(this, sec_key, val);
				break;

			case INI_LINE_ERROR:
				mx_ini_debug(INI_ERROR_INVALID_SYNTAX, line);
				break;

			case INI_LINE_EMPTY:
			case INI_LINE_COMMENT:
			default:
				break;
		}
		memset(line, 0x0, sizeof(line));
	}
	fclose(in);
	return this;
}

/**
 * @brief free the ini contents (key/value/hash)
 * @param this ini object pointer
 * @return success:0
 *
 * ini_release function used by other object to release the memory initialized
 * by ini_init.
 */
void ini_free(ini_data_t *this)
{
	int i;
	int num;


	if (NULL == this)
	    return ;

	num = this->num;
	for (i = 0; i < this->size && num > 0; i++)
	{
		if (this->key[i])
			free(this->key[i]);
		if (this->val[i])
			free(this->val[i]);
		num--;
	}
	free(this->key);
	free(this->val);
	free(this->hash);
	free(this);
}

/**
 * @brief print out the whole ini file
 * @param this ini object pointer
 * @return success:0
 *
 * ini_print function used by other object to print the whole ini content.
 */
int ini_print(ini_data_t *this)
{
	int i;
	int num;


	num = this->num;
	printf("num: %d\n", num);
	for (i = 0; i < this->size && num > 0; i++)
	{
		if (this->key[i])
		{
			if (strchr(this->key[i], ':'))
				printf("%s = %s\n", this->key[i], 
					this->val[i] ? (char*) this->val[i] : "");
			else
				printf("[%s]\n", this->key[i]);
		}
		num--;
	}
	return 0;
}

