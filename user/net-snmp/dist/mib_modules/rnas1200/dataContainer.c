#include "dataContainer.h"

/* duplicate a dataContainer */
dataContainer *dataDuplicateContainer(dataContainer *head, dataContainer *dup)
{
	dataContainer *curr = NULL;

	if (dup != NULL) {
		printf("Error!");
		return NULL;
	}

	curr = head;
	while (curr != NULL) {
		dup = dataAddContainer(dup,
				dataGetContainerTableName(curr),
				dataGetContainerColName(curr),
				dataGetContainerRowId(curr),
				dataGetContainerValue(curr));

		curr = curr->next;
	}

	return dup;
}

/* remove timeout data node 
 *
 * arguments:
 * 	timeout: the unit is "sec"
 *
 */
dataContainer *dataRemoveContainerTimeout(dataContainer *head, int timeout)
{
	dataContainer *curr = NULL;
	int countCurr;

	if (timeout < 1)
		return head;

	curr = head;
	countCurr = 1;
	while (curr != NULL) {
		if ((time((time_t *) NULL) - curr->timestamp) > timeout) {
			curr = curr->next;
			head = dataRemoveContainerNode(head, countCurr);
		} else {
			curr = curr->next;
			countCurr++;
		}
	}

	return head;
}

/* remove duplicate data node */
dataContainer *dataRemoveContainerDuplicate(dataContainer *head, int flags)
{
	dataContainer *curr = NULL;
	dataContainer *comp = NULL;
	int length;
	int currCount;
	int compCount;

	/* check flags */
	if (flags < 1 || flags >= (int) pow(2, NUM_OF_FLAGS)) {
		printf("Error!\n");
		return head;
	}

	/* put curr to the last node */
	curr = head;
	while (curr != NULL) {
		if (curr->next == NULL)
			break;
		curr = curr->next;
	}
	
	/* Check duplicate from the last to the first,
	 * since we can keep the newest data */
	length = dataGetContainerLen(head);
	currCount = 1;
	while (curr != NULL) {
		comp = curr->prev;
		compCount = currCount + 1;
		while (comp != NULL) { 
			if ((IS_TABLENAME(flags) || !strcmp(dataGetContainerTableName(curr), dataGetContainerTableName(comp))) &&
				(IS_COLNAME(flags) || !strcmp(dataGetContainerColName(curr), dataGetContainerColName(comp))) &&
				(IS_ROWID(flags) || !strcmp(dataGetContainerRowId(curr), dataGetContainerRowId(comp))) &&
				(IS_VALUE(flags) || !strcmp(dataGetContainerValue(curr), dataGetContainerValue(comp)))) 
			{
				comp = comp->prev;
				head = dataRemoveContainerNode(head, length - compCount + 1);
				length--;
			} else {
				comp = comp->prev;
				compCount++;
			}
		}
		curr = curr->prev;
		currCount++;
	}

	return head;
}

/* remove dataContainer node */
dataContainer *dataRemoveContainerNode(dataContainer *head, int nodeNum)
{
	int nodeCount = 1;
	dataContainer *curr = NULL;
	dataContainer *prev = NULL;

	if (nodeNum < 1 || nodeNum > dataGetContainerLen(head) || NULL == head) {
		printf("Error!\n");
		return head;
	}
	
	curr = head;
	if (nodeNum == 1) {
		head=curr->next;
		if (head != NULL)
			head->prev=NULL;
		dataFreeContainerNode(curr);
		return head;
	}

	while (curr != NULL && nodeCount < nodeNum) {
		prev = curr;
		curr = curr->next;
		nodeCount++;
	}
	prev->next = curr->next;
	if (curr->next != NULL)
		curr->next->prev = prev;
	dataFreeContainerNode(curr);

	return head;
}

/* free dataContainer node */
void dataFreeContainerNode(dataContainer *node)
{
	if (NULL == node)
		return;

	if (node->tableName != NULL)
		free(node->tableName);
	if (node->colName != NULL)
		free(node->colName);
	if (node->rowId != NULL)
		free(node->rowId);
	if (node->value != NULL)
		free(node->value);

	free(node);
}

/* create dataContainer node */
dataContainer *dataCreateContainerNode(char *tableName, char *colName, char *rowId, char *value)
{
	dataContainer *node = NULL;

	if (NULL == tableName || NULL == colName || NULL == rowId || NULL == value) {
		printf("Error!\n");
		return NULL;
	}

	/* create new dataContainer node */
	node = (dataContainer *) malloc(sizeof(dataContainer));
	if (node == NULL) {
		printf("Error!\n");
		return NULL;
	}

	/* initialize pointer */
	node->timestamp = time((time_t *) NULL);
	node->tableName = NULL;
	node->colName = NULL;
	node->rowId = NULL;
	node->value = NULL;
	node->prev = NULL;
	node->next = NULL;

	/* set tableName */
	node->tableName = (char *) malloc((strlen(tableName) + 1) * sizeof(char));
	if (node->tableName == NULL) {
		printf("Error!\n");
		dataFreeContainerNode(node);
		return NULL;
	}
	memset(node->tableName, '\0', (strlen(tableName) + 1) * sizeof(char));
	strcpy(node->tableName, tableName);

	/* set colName */
	node->colName = (char *) malloc((strlen(colName) + 1) * sizeof(char));
	if (node->colName == NULL) {
		printf("Error!\n");
		dataFreeContainerNode(node);
		return NULL;
	}
	memset(node->colName, '\0', (strlen(colName) + 1) * sizeof(char));
	strcpy(node->colName, colName);

	/* set rowId */
	node->rowId = (char *) malloc((strlen(rowId) + 1) * sizeof(char));
	if (node->rowId == NULL) {
		printf("Error!\n");
		dataFreeContainerNode(node);
		return NULL;
	}
	memset(node->rowId, '\0', (strlen(rowId) + 1) * sizeof(char));
	strcpy(node->rowId, rowId);

	/* set value */
	node->value = (char *) malloc((strlen(value) + 1) * sizeof(char));
	if (node->value == NULL) {
		printf("Error!\n");
		dataFreeContainerNode(node);
		return NULL;
	}
	memset(node->value, '\0', (strlen(value) + 1) * sizeof(char));
	strcpy(node->value, value);

	return node;
}

/* insert dataContainer node 
 *
 * errNum:
 * 	1: Inserted node is empty.
 * 	2: The position of the inserted node is out of filter.
 * 	0: success
 *	
 * */
dataContainer *dataInsertContainerNode(dataContainer *head, dataContainer *node, int nodeNum, int *errNum)
{
	dataContainer *prev = NULL;
	int nodeCount = 1;
	*errNum = 0;

	if (NULL == node) {
		printf("Error!\n");
		*errNum = 1;
		return head;
	}

	if (nodeNum < 1 || (nodeNum - dataGetContainerLen(head)) > 1) {
		printf("Error!\n");
		*errNum = 2;
		return head;
	}

	/* move prev to (nodeNum-1)th node */
	prev = head;
	while (nodeCount < (nodeNum - 1)) {
		prev = prev->next;
		nodeCount++;
	}

	if (1 == nodeNum) {
		node->next = head;
		if (head != NULL)
			head->prev = node; 
		node->prev = NULL;
		head = node;
	} else {
		node->next = prev->next;
		if (prev->next != NULL)
			prev->next->prev = node;
		node->prev = prev;
		prev->next = node;
	}

	return head;
}

/* show dataContainer length  */
int dataGetContainerLen(dataContainer *head)
{
	dataContainer *curr = NULL;
	int nodeCount = 0;

	curr = head;
	while (curr != NULL) {
		nodeCount++;
		curr = curr->next;
	}

	return nodeCount;
}

/* display dataContainer */
void dataDisplayContainer(dataContainer *head)
{
	dataContainer *curr = NULL;

	curr = head;
	while (curr != NULL) {
		printf("timestamp: %d: table name: %s, column name: %s, row id: %s, value: %s\n", curr->timestamp, curr->tableName, curr->colName, curr->rowId, curr->value);
		curr = curr->next;
	}
}

/* free dataContainer */
dataContainer *dataFreeContainer(dataContainer *head)
{
	dataContainer *curr = NULL;
	dataContainer *prev = NULL;
	
	curr = head;
	while (curr != NULL) {
		prev = curr;
		curr = curr->next;
		dataFreeContainerNode(prev);
	}
	head = NULL;

	return head;
}

/* add data to dataContainer */
dataContainer *dataAddContainer(dataContainer *head, char *tableName, char *colName, char *rowId, char *value)
{
	dataContainer *node = NULL;
	int errNum;

	if (NULL == tableName || NULL == colName || NULL == rowId || NULL == value) {
		printf("Error!\n");
		return head;
	}

	/* create a node */
	node = dataCreateContainerNode(tableName, colName, rowId, value);
	if (NULL == node) {
		printf("Error!\n");
		return head;
	}
	/* insert node to the last */
	head = dataInsertContainerNode(head, node, dataGetContainerLen(head) + 1, &errNum);
	if (errNum) {
		printf("Error!\n");
		dataFreeContainerNode(node);
		return head;
	}
	
	return head;
}

/* get tableName */
char *dataGetContainerTableName(dataContainer *node)
{
	if (NULL == node) {
		printf("Error!\n");
		return NULL;
	}
	return node->tableName;
}

/* get colName */
char *dataGetContainerColName(dataContainer *node)
{
	if (NULL == node) {
		printf("Error!\n");
		return NULL;
	}
	return node->colName;
}

/* get rowId */
char *dataGetContainerRowId(dataContainer *node)
{
	if (NULL == node) {
		printf("Error!\n");
		return NULL;
	}
	return node->rowId;
}

/* get value */
char *dataGetContainerValue(dataContainer *node)
{
	if (NULL == node) {
		printf("Error!\n");
		return NULL;
	}
	return node->value;
}

/* get timestamp */
int dataGetContainerTimestamp(dataContainer *node)
{
	if (NULL == node) {
		printf("Error!\n");
		return -1;
	}
	return node->timestamp;
}

