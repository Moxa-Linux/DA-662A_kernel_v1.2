#ifndef _THERMAL_H
#define _THERMAL_H

#define MAX_CMD_STR	1024
/* packet format
 *
 * |header|len|cmd|data1|data2|data3|data...|checksum|
 *
 */ 
#define HEADER_OFFSET	0
#define LEN_OFFSET	1
#define CMD_OFFSET	2
#define DATA_OFFSET	3


typedef struct __CMD__ {
	char type;
	char len;
}cmd_t;

typedef struct __UC__ {
	char header;
	cmd_t p;
	char data;
	char checksum;
}uc_t;

#endif /* _THERMAL_H */
