/*
 * Copyright (C) MOXA Inc. All rights reserved.
 * This software is distributed under the terms of the
 * MOXA License.  See the file COPYING-MOXA for details.
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include <signal.h>
/* According to POSIX.1-2001 */
#include <sys/select.h>
/* MOXA micro controller */
#include <uc.h>

#ifdef DEBUG 
#define pdebug(fmt, args...) fprintf(stderr, "%s: "fmt, __FUNCTION__, ##args)
#else
#define pdebug(fmt, args...) 
#endif

#define THERMAL_DEV	"/dev/ttyS1"
#define RETRY_TIMES	20
#define BUFFER_SIZE	20

/****************************************
 * Function Name: mdelay
 * action	: delay tick milisecond
 * Arg:
 * 	unsigned long
 * Return Value	: void
 *
 *****************************************/
void mdelay(unsigned long msec)
{
	struct timeval time;

	time.tv_sec = msec / 1000;
	time.tv_usec = (msec % 1000) * 1000;
	select(1, NULL, NULL, NULL, &time);
}

/****************************************
 * Function Name:max
 * action	: compare 2 int and return the biggest
 * Arg:
 * 	argv[1]=integer argv[2]=interger
 * Return Value	: interger
 *
 *****************************************/
int max(int a, int b)
{
	if(a > b) {
		return a ;
	}
	else {
		return b ;
	}
}

/****************************************
 * Function Name: calc_checksum
 * action	: calculate checksum
 * Arg:
 * 	argv[1]=data pointer argv[2]=len
 * Return Value	: checksum (char)
 *
 *****************************************/
char calc_checksum(char *data, char len)
{
	int i;
	char checksum = 0;

	if (len > 25)
		return 0;

	for (i=0; i<len; i++) {
		checksum += data[i];
	}

	return checksum;
}

/****************************************
 * Function Name: body_attach
 * action	: load info to header
 * Arg:
 * 	argv[1]=uc_t pointer argv[2]=cmd_t pointer
 * Return Value	:
 * 		void
 *
 *****************************************/
void body_attach(char *pbuff, cmd_t *cmd)
{
	pbuff[1] = cmd->len;
	pbuff[2] = cmd->type;
	return;
}

/****************************************
 * Function Name: tail_attach
 * action	: load info to header
 * Arg:
 * 	argv[1]=uc_t pointer argv[2]=cmd_t pointer
 * Return Value	:
 * 		void
 *
 *****************************************/
void tail_attach(char *pbuff, char *data)
{
	int i;
	int actual_len = pbuff[LEN_OFFSET];
	int actual_checksum_offset = pbuff[LEN_OFFSET] + CMD_OFFSET;

	/* checksum include command type */
	pbuff[actual_checksum_offset] = pbuff[CMD_OFFSET];

	/* contain data only if command type len > 1 */
	if (actual_len > 1) {
		for (i=0; i<actual_len-1; i++) {
			pbuff[DATA_OFFSET] = *data;
			/* calculate checksum 
			 * checksum offset = header + cmd + len + data...
			 */
			pbuff[actual_checksum_offset] += (*data);
		}
	}
	return;
}

void bit_ctrl_0 (char* pflag, int bit) {
	*pflag &= ~(1 << bit);
}

void bit_ctrl_1 (char* pflag, int bit) {
	*pflag |= (1 << bit);
}

/****************************************
 * Function Name: usage
 * action	: brief usage print usage routines
 * Arg:
 * 	void
 * Return Value
 * 	void
 *
 *****************************************/
void usage()
{
	fprintf(stderr, "Usage: -c [command] -s [led state] -t [watchdog timer]\n");
	fprintf(stderr, "\tcommand:\n");
	fprintf(stderr, "\t1: get hdd's first temperature\n");
	fprintf(stderr, "\t2: get hdd's second temperature\n");
	fprintf(stderr, "\t3: set system led on/off (MUST use with -s option)\n");
	fprintf(stderr, "\t4: get temperature sensor status\n");
	fprintf(stderr, "\t5: set first POE on/off (MUST use with -s option)\n");
	fprintf(stderr, "\t6: set second POE on/off (MUST use with -s option)\n");
	fprintf(stderr, "\t7: set first HDD power on/off (MUST use with -s option)\n");
	fprintf(stderr, "\t8: set second HDD power on/off (MUST use with -s option)\n");
	fprintf(stderr, "\t9: get uc version\n");
	fprintf(stderr, "\t-s 0/1 (off/on)\n");
	fprintf(stderr, "\t10: set factory flag \n");
	fprintf(stderr, "\t11: get factory flag\n");
	fprintf(stderr, "\t-s register value. Bit 0: Burnin flag \n");
	fprintf(stderr, "\t                   Bit 1: EOT flag \n");
	fprintf(stderr, "\t                   Bit 2: MAC flag \n");
	fprintf(stderr, "\t                   Bit 3: MP flag \n");
	fprintf(stderr, "\t-t timer value (secs) [default is 10s. value > 1\n");
	fprintf(stderr, "\t-h help\n");
	//fprintf(stderr, "\t-f 1: Burnin_flag 2: EOT_flag 3: Mac_flag 4: MP_flag\n");
	return;
}

static cmd_t command_type[] = {
	{/*get_hdd_temperature1*/ 0xC0, 0x1},
	{/*get_hdd_temperature2*/ 0xC1, 0x1},
	{/*set_sys_cmd_state*/ 	 0xC2, 0x2},
	{/*get_hdd_temperature_sensor_status*/	 0xC3, 0x1},
	{/* set_poe1_mode*/	 0xC4, 0x2},
	{/* set_poe2_mode*/	 0xC5, 0x2},
	{/* set_hdd1_power*/	 0xC6, 0x2},
	{/* set_hdd2_power*/	 0xC7, 0x2},
	{/*get_version*/	 0xCA, 0x1},
	{/* set factory flag*/   0xCD, 0x2},
	{/* get factory flag*/   0xCE, 0x2},
};

int main(int argc, char *argv[])
{
	static int options;
	extern char *optarg;
	struct termios tbuff;
	int write_len = 0;
	char buffer[BUFFER_SIZE];
	char response[BUFFER_SIZE];
	int response_len;
	int response_checksum;
	int ret;
	fd_set readfds;
	int maxfd;
	struct timeval wait_time;		/* Timer for release FD */
	int status;
	int fd;
	int retry;
	int j;
	int checksum_index;
	int rc = 0;
	/* user options */
	char data = 0;
	char flag = 0;
	int command = 0;
	char cmd_state = 0;
	int watchdog_timer = 10;
	/* uc command */
	char header = 0xF0;
	uc_t uc_request;
	uc_t uc_respond;

	//pdebug("Thermal app.\n");

	if (argc < 2) {
		usage();
		return 1;
	}

	/* token analysis */
	while ((options=getopt(argc,argv,"c:s:t:f:")) != -1) {
		switch(options) {
		case 'c':
			command = atoi(optarg);
			break;
		case 's':
			cmd_state = atoi(optarg);
			data = (char)cmd_state;
			break;
			//case 'f':
			//	flag = atoi(optarg);
			//flag = (char)cmd_state;
		case 't':
			watchdog_timer = atoi(optarg);
			data = (char)watchdog_timer;
			break; 
		case 'h':
			usage(); 
			return 1;
			break; 
		default:
			fprintf(stderr, "Incorrect parameter: %c\n", options);
			usage();
			return 1; /* print help without process others opts */
		}
	}
	/* check options */
	if ((command == 0) || (command > sizeof(command_type)/sizeof(cmd_t))) {
		fprintf(stderr, "command %d exceed !!\n", command);
		usage();
		return 1;
	}

	if ((watchdog_timer == 0) || (watchdog_timer > 254)) {
		fprintf(stderr, "watchdog_timer %d exceed !!\n", watchdog_timer);
		usage();
		return 1;
	}

	fd = open(THERMAL_DEV,O_RDWR|O_NONBLOCK);
	if (fd < 0) {
		perror("open\n");
		return 1;
	}
	if (!flock(fd,LOCK_EX)) {
		//pdebug("the file was locked.\n");
	}

	/* tty setting */
	tcgetattr(fd, &tbuff);
	//tbuff.c_lflag &= ~(ICANON|ECHO);
	tbuff.c_lflag = 0;
	tbuff.c_oflag = 0;
	tbuff.c_cc[VMIN] = 000;
	tbuff.c_cc[VTIME] = 000;
	tbuff.c_cflag = CS8|CREAD|CLOCAL;
	//tbuff.c_iflag &= ~(IXON|IXOFF|ICRNL);
	tbuff.c_iflag = 0;

#if (defined ARCH_UC1260)
	if(cfsetspeed(&tbuff, B115200) < 0) {
		perror("baudrate");
	}
#else
	if(cfsetspeed(&tbuff, B9600) < 0) {
		perror("baudrate");
	}
#endif

	/* apply current setting right now */
	tcsetattr(fd, TCSANOW, &tbuff);
	tcflush(fd, TCIOFLUSH);

	/* select */
	wait_time.tv_usec = 100000;
	wait_time.tv_sec = 0;

	FD_ZERO(&readfds);
	FD_SET(fd,&readfds);
	/* biggest fd should assign for select fn use */
	maxfd = max(fd,2) + 1;

	/* command combination */
	memset((void *)&uc_request, '\0', sizeof(uc_t));
	memset((void *)&uc_respond, '\0', sizeof(uc_t));

	/* header attach */
	buffer[HEADER_OFFSET] = header;

	/* body attach */
	body_attach(buffer, &command_type[command-1]);

	/* tail attach */
	//	if (flag != 0)
	//		data = flag;
	//	data = 1 << (flag -1);
	//	printf("flag=%d\n",flag);
	//	bit_ctrl_1(&data, flag-1);


	pdebug("%02x", data);

	tail_attach(buffer, &data);

	/* now, reconstruct buffer */
	write_len = (buffer[LEN_OFFSET]-1) + 4 /* HEADER + LEN + CMD + CHECKSUM */ ;

	pdebug("writing len %d, %02x %02x %02x ", write_len, buffer[HEADER_OFFSET], buffer[LEN_OFFSET], buffer[CMD_OFFSET]);

	if (write_len >= 5) {
		for (j=0; j<(write_len-3); j++) {
			pdebug("%02x ", buffer[DATA_OFFSET+j]);
		}
	} else {
		pdebug("%02x", buffer[DATA_OFFSET]);
	}

	ret = write(fd, &buffer, write_len);
	if (ret < 0) {
		perror("write\n");
		return 1;
	}

	retry = RETRY_TIMES;
	memset(&response, '\0', BUFFER_SIZE);
	response_len = 0;
	while (retry > 0) {
		status = select(maxfd, &readfds, NULL, NULL, &wait_time);
		if (status > 0) {
			if (FD_ISSET(fd, &readfds)) {

				memset(&buffer, '\0', BUFFER_SIZE);
				ret = read(fd, &buffer, BUFFER_SIZE);
				if (ret < 0) {
					perror("read");
				} else {
					memcpy(response + response_len*sizeof(char), buffer, ret);
					response_len += ret;

					/* debug */
					//fprintf(stderr, "----> response ");
					//for (j=0; j<response_len; j++) {
					//	fprintf(stderr, "%x:", response[j]);
					//}
					//fprintf(stderr, "\n");

					/* simple data confirmation */
					if (response_len > 0 && response[HEADER_OFFSET] != header) {
						rc = 1;
						goto exit;
					}
					if (response_len > 1 && response[LEN_OFFSET] == 0) {
						rc = 1;
						goto exit;
					}
					if (response_len >= (response[LEN_OFFSET] + 3)) {
						/* finish receive response */
						rc = 1;

						/* veryify checksum */
						checksum_index = CMD_OFFSET + response[LEN_OFFSET];
						response_checksum = calc_checksum(response + CMD_OFFSET, response[LEN_OFFSET]);
						if (response_checksum == response[checksum_index]) {
							/* print data */
							for (j=DATA_OFFSET; j<(DATA_OFFSET+response[LEN_OFFSET]-1); j++) {
								/* 2's complement */
								int c;
								if (response[j] >= 128) {
									c = (int)response[j] % 128;
									c -= 127;
								} else {
									c = response[j];
								}
								printf("%d", c);
							}
							printf("\n");
							rc = 0;
						}

						break;
					}
				}
			}
		} else if (status < 0) {
			pdebug("select error\n");
			rc = 1;
			goto exit;
		} else {
			pdebug("wait timeout\n");
		}

		retry--;
	}

	/* according to sun.lee@moxa.com spec, each loop need
	 * at least 5 second to send request again.
	 * sleep(5);
	 */

exit:
	if (!flock(fd, LOCK_UN)) {
		//pdebug("the file was unlocked./n");
	}
	close(fd);

	return rc;
}
