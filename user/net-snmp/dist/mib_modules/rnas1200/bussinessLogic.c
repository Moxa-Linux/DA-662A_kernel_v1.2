#include "bussinessLogic.h"

/* check netmask
 *
 * Policy: 
 * 	For each address item must fit "11...100...0"
 *
 * Note:
 * 	Assume each netmask item is >= 0 and <= 255
 */
dataContainer *businessLogicFilterNetmask(dataContainer *head)
{
	dataContainer *curr = NULL;
	char strBuf[BUSINESSLOGIC_IPV4_NETMASK_LEN + 1];
	char *strTmp = NULL;
	char *ptrBuf;
	int countCurr;
	long int netmask;
	int countXor;
	int prevBit, currBit;
	int i;

	curr = head;
	countCurr = 1;
	while (curr != NULL) {
		if (!strcmp(dataGetContainerColName(curr), "subnet")) {
			/* business logic here */
			memset(strBuf, '\0', BUSINESSLOGIC_IPV4_NETMASK_LEN + 1);
			strcpy(strBuf, dataGetContainerValue(curr));
			strTmp = strtok_r(strBuf, BUSINESSLOGIC_IPV4_SEPERATOR, &ptrBuf);

			/* get net mask */
			netmask = 0;
			do {
				netmask = (netmask << 8) | atoi(strTmp);
			} while (strTmp = strtok_r(NULL, BUSINESSLOGIC_IPV4_SEPERATOR, &ptrBuf));

			/* remove not applied for policy */
			countXor = 0;
			prevBit = 0;
			for (i = 0; i < BUSINESSLOGIC_IPV4_LEN; i++) {
				currBit = netmask & 1;
				countXor += (prevBit ^ currBit);
				prevBit = currBit;
				netmask >>= 1;
			}
			if (countXor > 1) {
				curr = curr->next;
				head = dataRemoveContainerNode(head, countCurr);
			} else {
				curr = curr->next;
				countCurr++;
			}
		} else {
			curr = curr->next;
			countCurr++;
		}
	}
	return head;
}

/* check ip and gateway address
 * 
 * Policy:
 * 	1. host id CANNOT be "0..0" or "1..1"
 *
 * Note:
 * 	Assume each item is >= 0 and <= 255
 */
dataContainer *businessLogicFilterIp(dataContainer *head)
{
	dataContainer *curr = NULL;
	char strBuf[BUSINESSLOGIC_IPV4_NETMASK_LEN + 1];
	char *strTmp = NULL;
	char *ptrBuf;
	int countCurr;
	long int hostId;
	int hostIdLen;
	int class;
	int i;

	curr = head;
	countCurr = 1;
	while (curr != NULL) {
		if (!strcmp(dataGetContainerColName(curr), "ip") ||
				!strcmp(dataGetContainerColName(curr), "gateway")) {
			/* business logic here */
			memset(strBuf, '\0', BUSINESSLOGIC_IPV4_NETMASK_LEN + 1);
			strcpy(strBuf, dataGetContainerValue(curr));
			strTmp = strtok_r(strBuf, BUSINESSLOGIC_IPV4_SEPERATOR, &ptrBuf);

			/* Check class A/B/C/Others */
			class = getIpClass(atoi(strTmp));

			/* get host id */
			hostId = 0;
			hostIdLen = 0;
			i = 1;
			while (strTmp = strtok_r(NULL, BUSINESSLOGIC_IPV4_SEPERATOR, &ptrBuf)) {
				if (++i > class) {
					hostId = (hostId << 8) | atoi(strTmp);
					hostIdLen += 8;
				}
			}
			/* remove not applied for policy */
			if (!hostId || !~((-1L << hostIdLen) ^ hostId)) {
				curr = curr->next;
				head = dataRemoveContainerNode(head, countCurr);
			} else {
				curr = curr->next;
				countCurr++;
			}
		} else {
			curr = curr->next;
			countCurr++;
		}
	}
	return head;
}

/* check general netowrk setting
 *
 * Policy:
 * 	1. Gateway should in the LAN by definition of ip and netmask
 *
 * Note:
 * 	Assume each item is >= 0 and <= 255
 */
dataContainer *businessLogicFilterNetwork(sqlite3 *sqlDb, dataContainer *head)
{
	dataContainer *network = NULL;
	dataContainer *node = NULL;
	dataContainer *curr = NULL;
	int countCurr;
	
	char *rowId = NULL;
	char *ip = NULL;
	char *gateway = NULL;
	char *netmask = NULL;
	long int ipValue;
	long int gatewayValue;
	long int netmaskValue;

	char strBuf[BUSINESSLOGIC_IPV4_IP_LEN + 1];
	char *strTmp = NULL;
	char *ptrBuf;
	int lanBitIndex;
	int class;
	
	if (NULL == sqlDb) {
		printf("Error!\n");
		return head;
	}

	curr = head;
	countCurr = 1;
	/* 1. get "networkResources.ip/netmask/gateway" data from dataContainer to a new dataContainer(network) */
	while (curr != NULL) {
		if (!strcmp(dataGetContainerTableName(curr), "networkResources") && (
				!strcmp(dataGetContainerColName(curr), "ip") ||
				!strcmp(dataGetContainerColName(curr), "gateway") ||
				!strcmp(dataGetContainerColName(curr), "subnet"))) {
			network = dataAddContainer(
					network,
					dataGetContainerTableName(curr),
						dataGetContainerColName(curr),
						dataGetContainerRowId(curr),
						dataGetContainerValue(curr));
				curr = curr->next;
				head = dataRemoveContainerNode(head, countCurr); 
			} else {
				curr = curr->next;
				countCurr++;
			}
	}

	/* 2. Obtain ip/netmask/gateway
	 *
	 *	Use the rowId of first data in dataContainer(network) to 
	 *	check general network setting for this rowId.
	 *	If each ip/netmask/gateway isn't in dataContainer(network), then find it from database.
	 */
	while (network != NULL) {
		rowId = strdup(dataGetContainerRowId(network));
		curr = network;
		countCurr = 1;
		ip = NULL;
		gateway = NULL;
		netmask = NULL;

		while (curr != NULL) {
			if (!strcmp(dataGetContainerRowId(curr), rowId)) {
				/* get index of ip */
				if (!strcmp(dataGetContainerColName(curr), "ip"))
					ip = strdup(dataGetContainerValue(curr));
				/* get index of gateway */
				if (!strcmp(dataGetContainerColName(curr), "gateway"))
					gateway = strdup(dataGetContainerValue(curr));
				/* get index of netmask */
				if (!strcmp(dataGetContainerColName(curr), "subnet"))
					netmask = strdup(dataGetContainerValue(curr));

				curr = curr->next;
				network = dataRemoveContainerNode(network, countCurr);
			} else {
				curr = curr->next;
				countCurr++;
			}
		}
		/* find ip from database */
		if (NULL == ip) {
			node = dataCreateContainerNode("networkResources", "ip", rowId, "");
			ip = sqlGetDbItemValue(sqlDb, node);
			dataFreeContainerNode(node);
		}
		/* find gateway from database */
		if (NULL == gateway) {
			node = dataCreateContainerNode("networkResources", "gateway", rowId, "");
			gateway = sqlGetDbItemValue(sqlDb, node);
			dataFreeContainerNode(node);
		}
		/* find netmask from database */
		if (NULL == netmask) {
			node = dataCreateContainerNode("networkResources", "subnet", rowId, "");
			netmask = sqlGetDbItemValue(sqlDb, node);
			dataFreeContainerNode(node);
		}

		/* 3. check ip/netmask/gateway
		 *	
		 *	Gateway must in the lan by definition of ip/netmask.
		 */
	
		/* get ip value and ip class */
		memset(strBuf, '\0', sizeof(strBuf));
		strcpy(strBuf, ip);
		strTmp = strtok_r(strBuf, BUSINESSLOGIC_IPV4_SEPERATOR, &ptrBuf);
		class = getIpClass(atoi(strTmp));
		ipValue = 0;
		do {
			ipValue = (ipValue << 8) | atoi(strTmp);
		} while (strTmp = strtok_r(NULL, BUSINESSLOGIC_IPV4_SEPERATOR, &ptrBuf));
		/* get netmask value */
		memset(strBuf, '\0', sizeof(strBuf));
		strcpy(strBuf, netmask);
		strTmp = strtok_r(strBuf, BUSINESSLOGIC_IPV4_SEPERATOR, &ptrBuf);
		netmaskValue = 0;
		do {
			netmaskValue = (netmaskValue << 8) | atoi(strTmp);
		} while (strTmp = strtok_r(NULL, BUSINESSLOGIC_IPV4_SEPERATOR, &ptrBuf));
		/* get gateway value */
		memset(strBuf, '\0', sizeof(strBuf));
		strcpy(strBuf, gateway);
		strTmp = strtok_r(strBuf, BUSINESSLOGIC_IPV4_SEPERATOR, &ptrBuf);
		gatewayValue = 0;
		do {
			gatewayValue = (gatewayValue << 8) | atoi(strTmp);
		} while (strTmp = strtok_r(NULL, BUSINESSLOGIC_IPV4_SEPERATOR, &ptrBuf));
		
		/* get lan bit index */
		lanBitIndex = 0;
		while (!(netmaskValue & 1)) {
			lanBitIndex++;
			netmaskValue >>= 1;
		}
		lanBitIndex = lanBitIndex < (BUSINESSLOGIC_IPV4_LEN - class * 8) ? lanBitIndex : (BUSINESSLOGIC_IPV4_LEN - class * 8);

		/* 4. check gateway and add ip/netmask/gateway to dataContainer */
		ipValue >>= lanBitIndex;
		gatewayValue >>= lanBitIndex;
		if (ipValue == gatewayValue) {
			head = dataAddContainer(head, "networkResources", "ip", rowId, ip);
			head = dataAddContainer(head, "networkResources", "gateway", rowId, gateway);
			head = dataAddContainer(head, "networkResources", "subnet", rowId, netmask);
		}

		/* 5. free ip/gateway/netmask if get it from database */
		free(rowId);
		free(ip);
		free(gateway);
		free(netmask);
	}

	return head;
}

/* check default gateway setting
 *
 * Policy:
 * 	1. Default gateway should be on only one LAN port.
 *
 */
dataContainer *businessLogicFilterDefaultGateway(sqlite3 *sqlDb, dataContainer *head)
{
	dataContainer *defGateway = NULL;
	dataContainer *node = NULL;
	dataContainer *curr = NULL;
	int countCurr;
	int errNum;
	
	if (NULL == sqlDb) {
		printf("Error!\n");
		return head;
	}

	curr = head;
	countCurr = 1;
	/* 1. get "networkResources.defaultGateway" data from dataContainer to a new dataContainer(defGateway) */
	while (curr != NULL) {
		if (!strcmp(dataGetContainerTableName(curr), "networkResources") &&
			!strcmp(dataGetContainerColName(curr), "defaultGateway")) {
			defGateway = dataAddContainer(
					defGateway,
					dataGetContainerTableName(curr),
					dataGetContainerColName(curr),
					dataGetContainerRowId(curr),
					dataGetContainerValue(curr));
			curr = curr->next;
			head = dataRemoveContainerNode(head, countCurr); 
		} else {
			curr = curr->next;
			countCurr++;
		}
	}
	/* leave if there is no data about defaultGateway */
	if (NULL == defGateway)
		return head;

	/* 2. Keep the latest setting */
	defGateway = dataRemoveContainerDuplicate(defGateway, DATA_TABLENAME | DATA_COLNAME);
	
	/* 3. add defGateway to dataContainer */
	head = dataInsertContainerNode(head, defGateway, dataGetContainerLen(head) + 1, &errNum);
	if (errNum) {
		printf("Error!\n");
		dataFreeContainerNode(defGateway);
		return head;
	}
	/* 4. Unset default gateway of all LAN ports to 0 */
	node =  dataCreateContainerNode(dataGetContainerTableName(defGateway), dataGetContainerColName(defGateway), dataGetContainerRowId(defGateway), "0");
	sqlSetDbColValue(sqlDb, node);
	dataFreeContainerNode(node);

	return head;
}

int getIpClass(int firstNetId)
{
	int class;

	if (!(firstNetId & 128))
		class = 1;
	else if (!(firstNetId & 64))
		class = 2;
	else if (!(firstNetId & 32))
		class = 3;
	else if (!(firstNetId & 16))
		class = 4;
	else if (!(firstNetId & 8))
		class = 5;
	else if (!(firstNetId & 4))
		class = 6;
	else 
		class = 7;

	return class;
}
