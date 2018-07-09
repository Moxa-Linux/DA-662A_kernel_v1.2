#ifndef _BUSINESSLOGIC_H
#define _BUSINESSLOGIC_H

#include <stdio.h>
#include <string.h>
#include "dataContainer.h"
#include "sqlDb.h"

#define BUSINESSLOGIC_IPV4_IP_LEN 15 
#define BUSINESSLOGIC_IPV4_NETMASK_LEN 15 
#define BUSINESSLOGIC_IPV4_GATEWAY_LEN 15 
#define BUSINESSLOGIC_IPV4_LEN 32
#define BUSINESSLOGIC_IPV4_SEPERATOR "."
#define BUSINESSLOGIC_IPV6_LEN 48
#define BUSINESSLOGIC_IPV6_SEPERATOR ":"

dataContainer *businessLogicFilterNetmask(dataContainer *);
dataContainer *businessLogicFilterIp(dataContainer *);
dataContainer *businessLogicFilterNetwork(sqlite3 *, dataContainer *);
dataContainer *businessLogicFilterDefaultGateway(sqlite3 *, dataContainer *);
int getIpClass(int);


#endif
