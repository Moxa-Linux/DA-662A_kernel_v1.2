#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <debug.h>
#include <net.h>

/*
 * @brief usage print usage routines
 */
void usage()
{
	fprintf(stderr, "Usage: -cs -p port\n");
	fprintf(stderr, "\t-c client mode\n");
	fprintf(stderr, "\t-s server mode\n");
	fprintf(stderr, "\t-p port number\n");
	return;
}

/* 
 * @brief daemon entry point
 * @param command argument
 * @return error:1 success:0
 */
int main(int argc, char *argv[])
{

	static int options;
 	extern char *optarg;
	int mode = 0;	/* default client */
	int port = 0;
	char buffer[MAX_BUFFER_LEN] = "Hello World!!!!";
	char ipaddr[MAX_IP_STR];
	int length = MAX_BUFFER_LEN;
	int ret = 0; 

	net_t	*net;

	/* token analysis */
	while((options=getopt(argc, argv, "hsp:c:")) != -1) {
		   	switch(options) {
			case 'c':
				mode = 0;
				strcpy(ipaddr, optarg);
				break; 
			case 's':
				mode = 1;
				break; 
			case 'p':
				port = atoi(optarg);
				break; 
			case 'h':
				usage(); 
				return 1;
				break; 
			default:
				printf("Incorrect paramter: %c\n", options);
				usage() ;
				return 1 ; /* print help without process others opts */
		}
	}


	/* initial net */
	net = mx_net_init(CLIENT_MODE, ipaddr, port);
	if (!net) {
		fprintf(stderr, "network object init error\n");
		exit(1);
	}


	/* call up send method */
	while (ret >= 0) {
		fprintf(stdout, "please input string:");
		scanf("%s", buffer);
		ret = mx_net_send(net, buffer, strlen(buffer), BLOCKING, 1000);
		if (ret <= 0)
			fprintf(stderr, "error sending packets\n");
		else
			fprintf(stderr, "sending data %d\n", ret);

		/* call up recv method */
		ret = mx_net_recv(net, (void *)buffer, length, BLOCKING, 1000);
		if (ret <= 0)
			fprintf(stderr, "error receiving packets\n");
		else
			fprintf(stderr, "receiving data %d\n", ret);

		fprintf(stdout, "get total %d , data %s from network\n", ret, buffer);
	}

	/* release net */
	mx_net_release(net);

	return 0;
}
