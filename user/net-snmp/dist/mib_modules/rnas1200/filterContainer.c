#include "filterContainer.h"

/* compare the filterContainer to string
 *
 * output:
 * 	0: correct
 * 	1: not correct
 * 	2: arguments error
 */
int filterCompareContainer(filterContainer *head, const char *s)
{
	filterContainer *curr = NULL;
	char filterStr[FILTER_MAX_LEN + 1];
	char *filterContent = NULL;
	char option[FILTER_MAX_LEN + 1];
	
	int count;
	int seperatorIndex;
	int seperatorLen;
	int contentLen;
	int retval;
	int max, min;

	if (NULL == head || NULL == s || FILTER_MAX_LEN < strlen(s)) {
		printf("Error!\n");
		return 2;
	}

	curr = head;
	memset(filterStr, '\0', sizeof(filterStr));
	strcpy(filterStr, s);
	filterContent = filterStr;

	while (curr != NULL) {
		/* calculate seperatorLen */
		if (curr->seperator == '\0') 
			seperatorLen = 0;
		else
			seperatorLen = strlen(curr->seperator);

		/* calculate the next seperatorIndex */
		seperatorIndex = 0;
		if (NULL == curr->next) {
			seperatorIndex = strlen(filterContent) + 1;
		} else if (curr->next->seperator[0] != '\0') {
			seperatorIndex = strstrnum(&filterContent[seperatorLen], curr->next->seperator);
			seperatorIndex += seperatorLen;
		} else {
			/* if seperator is empty:
			 * 	Assume that two options are NOT the same type(number/word/symbol).*/
			if (isalpha(filterContent[seperatorLen]))
				while (isalpha(filterContent[seperatorLen + seperatorIndex++]));
			if (isdigit(filterContent[seperatorLen]))
				while (isdigit(filterContent[seperatorLen + seperatorIndex++]));
			if (!isalnum(filterContent[seperatorLen]))
				while (!isalnum(filterContent[seperatorLen + seperatorIndex++]));
			seperatorIndex += seperatorLen;
		}
	
		/* 1) check seperator */
		if (strncmp(curr->seperator, filterContent, seperatorLen) != 0)
			return 1;

		/* 2) check option */
		filterContent = &filterContent[seperatorLen];
		contentLen = seperatorIndex - seperatorLen - 1;
		count = 0;
		retval = 1;
		/* check for '/' */
		if (curr->operator == '/') {
			while (count < curr->optionCount) {
				if (strncmp(filterContent, curr->option[count], contentLen) == 0) {
					retval = 0;
					break;
				}
				count++;
			}
			if (retval)
				return 1;
		/* check for '^' */
		} else if (curr->operator == '^') {
			max = atoi(curr->option[0]);
			min = atoi(curr->option[1]);
			if (max < min)
				swap(&max, &min);
			memset(option, '\0', sizeof(option));
			strncpy(option, filterContent, contentLen);
			if (atoi(option) < min || atoi(option) > max) 
				return 1;
		/* check for '' */
		} else {
			memset(option, '\0', sizeof(option));
			strncpy(option, filterContent, contentLen);
			if (strcmp(option, curr->option[0]) != 0) 
				return 1;
		}

		filterContent = &filterContent[contentLen];
		curr = curr->next;
	}
		
	return 0;
}


/* create a filter container.
 *
 * Note:
 * 	1. There is no "()" inside another "()"
 * 	2. It cannot be empty in "()".
 *	3. There is only one type operator in "()", the second type will be ignored.
 * 	4. There must be at least one character between operators.
 *	5. There cannot be Any character after the last "()".
 *	6. There can be characters before the first "()".
 *	*7. For operator '^':
 *			* Only accept two number.
 *	*8. If the seperator is NULL, then two option can't be the same type. (symbol/number/word)
 *
 */
filterContainer *filterCreateContainer(const char *filter)
{
	filterContainer *head = NULL;
	char **option = NULL;
	char operator;
	char seperator[FILTER_MAX_LEN + 1];
	int optionCount;
	
	char filterStr[FILTER_MAX_LEN + 1];
	char *filterContent = NULL;
	int index;
	char *ptrBuf;

	if (NULL == filter || FILTER_MAX_LEN < strlen(filter)) {
		printf("Error!\n");
		return NULL;
	}

	memset(filterStr, '\0', sizeof(filterStr));
	strncpy(filterStr, filter, sizeof(filterStr));
	
	filterContent = strtok_r(filterStr, ")", &ptrBuf);
	
	/* check whether there's no ')' */
	if (strlen(filterContent) == strlen(filter)) {
		printf("Error!\n");
		return NULL;
	}
	
	do {
		/* 1) get seperator and content */
		index = strcspn(filterContent, "(");
		
		/* check whether there's no '(' or no char between '(' and ')' */
		if (strlen(filterContent) <= index + 1) {
			printf("Error!\n");
			filterFreeContainer(head);
			return NULL;
		}
		memset(seperator, '\0', sizeof(seperator));
		strncpy(seperator, filterContent, index);
		filterContent = &filterContent[index + 1];
	
		/* 2) get operator */
		/* check whether there's any '(' and ')' between '(' and ')' */
		if (strpbrk(filterContent, "()")) {
			printf("Error!\n");
			filterFreeContainer(head);
			return NULL;
		}
		index = strcspn(filterContent, "/^");
		operator = filterContent[index];
		
		/* initialize option and optionCount*/
		option = (char **) malloc((strchrcount(filterContent, operator) + 1) * sizeof(char *));
		optionCount = 1;
	
		/* 3) get options */
		while (strlen(filterContent) != index) {
			option[optionCount - 1] = (char *) malloc((index + 1) * sizeof(char));
			memset(option[optionCount - 1], '\0', (index + 1) * sizeof(char));
			strncpy(option[optionCount - 1], filterContent, index);
			
			filterContent = &filterContent[index + 1];
			index = strcspn(filterContent, &operator);
			optionCount++;
		}
		option[optionCount - 1] = (char *) malloc((index + 1) * sizeof(char));
		memset(option[optionCount - 1], '\0', (index + 1) * sizeof(char));
		strncpy(option[optionCount - 1], filterContent, index);

		/* 4) add content to filterContainer */
		head = filterAddContainer(head, option, optionCount, operator, seperator);
		
		/* 5) free option */
		while (optionCount) {
			if (option[--optionCount] != NULL)
				free(option[optionCount]);
		}
		if (option != NULL)
			free(option);

	} while (filterContent = strtok_r(NULL, ")", &ptrBuf));

	return head;
}



/* free filterContainer node */
void filterFreeContainerNode(filterContainer *node)
{
	int count = 0;

	if (NULL == node)
		return;
	
	while (count < node->optionCount) {
		if (node->option[count] != NULL)
			free(node->option[count++]);
	}
	if (node->option != NULL)
		free(node->option);
	if (node->seperator != NULL)
		free(node->seperator);

	free(node);
}

/* create filterContainer node */
filterContainer *filterCreateContainerNode(char **option, int optionCount, char operator, char *seperator)
{
	filterContainer *node = NULL;
	int i;

	if (NULL == option || 0 == optionCount || NULL == seperator) {
		printf("Error\n");
		return NULL;
	}

	/* create new filterContainer node */
	node = (filterContainer *) malloc(sizeof(filterContainer));
	if (node == NULL) {
		printf("Error!\n");
		return NULL;
	}
	
	/* set optionCount/operaotr */
	node->optionCount = optionCount;
	node->operator = operator;

	/* set option */
	node->option = (char **) malloc(optionCount * sizeof(char *));
	if (node->option == NULL) {
		printf("Error!\n");
		filterFreeContainerNode(node);
		return NULL;
	}
	for (i = 0; i < optionCount; i++) {
		node->option[i] = (char *) malloc((strlen(option[i]) + 1) * sizeof(char));
		if (node->option[i] == NULL) {
			printf("Error!\n");
			filterFreeContainerNode(node);
			return NULL;
		}
		memset(node->option[i], '\0', (strlen(option[i]) + 1) * sizeof(char));
		strcpy(node->option[i], option[i]);
	}

	/* set seperator */
	node->seperator = (char *) malloc((strlen(seperator) + 1) * sizeof(char));
	if (node->seperator == NULL) {
		printf("Error!\n");
		filterFreeContainerNode(node);
		return NULL;
	}
	memset(node->seperator, '\0', (strlen(seperator) + 1) * sizeof(char));
	strcpy(node->seperator, seperator);

	return node;
}

/* insert filterContainer node
 * 
 * errNum:
 *   1: Inserted node is empty.
 *   2: The position of the inserted node is out of filter.
 *   0: success
 *
 */
filterContainer *filterInsertContainerNode(filterContainer *head, filterContainer *node, int nodeNum, int *errNum)
{
	filterContainer *prev = NULL;
	int nodeCount = 1;
	*errNum = 0;

	if (NULL == node) {
		printf("Error!\n");
		*errNum = 1;
		return head;
	}

	if (nodeNum < 1 || (nodeNum - filterGetContainerLen(head)) > 1) {
		printf("Error!\n");
		*errNum = 2;
		return head;
	}

	/* move prev to (nodeNum-1)-th node */
	prev = head;
	while (nodeCount < (nodeNum - 1)) {
		prev = prev->next;
		nodeCount++;
	}

	if (nodeNum == 1) {
		node->next = head;
		head = node;
	} else {
		node->next = prev->next;
		prev->next = node;
	}

	return head;
}

/* show filterContainer length  */
int filterGetContainerLen(filterContainer *head)
{
	filterContainer *curr = NULL;
	int nodeCount = 0;

	curr = head;
	while (curr != NULL) {
		nodeCount++;
		curr = curr->next;
	}

	return nodeCount;
}

/* display filterContainer */
void filterDisplayContainer(filterContainer *head)
{
	filterContainer *curr = NULL;
	int i;

	curr = head;
	while (curr != NULL) {
		for (i = 0; i < curr->optionCount; i++) {
			printf("option[%d]: %s\n", i, curr->option[i]);
		}
		printf("optionCount: %d, operator: %c, seperator: %s\n", curr->optionCount, curr->operator, curr->seperator);
		curr = curr->next;
	}
}

/* free filterContainer */
filterContainer *filterFreeContainer(filterContainer *head)
{
	filterContainer *curr = NULL;
	filterContainer *prev = NULL;

	curr = head;
	while (curr != NULL) {
		prev = curr;
		curr = curr->next;
		filterFreeContainerNode(prev);
	}
	head = NULL;

	return head;
}

/* add data to filterContainer */
filterContainer *filterAddContainer(filterContainer *head, char **option, int optionCount, char operator, char *seperator)
{
	filterContainer *node = NULL;
	int errNum = 0;

	if (NULL == option || 0 == optionCount || NULL == seperator) {
		printf("Error\n");
		return head;
	}

	/* create a node */
	node = filterCreateContainerNode(option, optionCount, operator, seperator);
	if (NULL == node) {
		printf("Error!\n");
		return head;
	}
	/* insert node to the last */
	head = filterInsertContainerNode(head, node, filterGetContainerLen(head) + 1, &errNum);
	if (errNum) {
		printf("Error!\n");
		filterFreeContainerNode(node);
		return head;
	}

	return head;
}
