#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <debug.h>
#include <net.h>

int net_dispatch(net_t *net, void *buffer, int length)
{
	static int ret = 0;
	char buf[17];

	memset(buf, '\0', 17);
	mx_net_getip(net, buf, 17);
	fprintf(stdout, "Be connected by IP:(%s)\n", buf);

	/* call up recv method */
	while (ret >= 0) {
		fprintf(stdout, "waiting to receive again\n");
		ret = mx_net_recv(net, (void *)buffer, length, BLOCKING, 1000);
		if (ret < 0) {
			fprintf(stderr, "error receiving packets\n");
		}
		else if (ret == 0){
			fprintf(stderr, "fd error, connection could be terminated\n");
		}
		else
			fprintf(stderr, "receiving data %d\n", ret);

		fprintf(stdout, "get total %d , data %s from network\n", ret, (char *)buffer);


		strcpy(buffer, "World Hello to you!!!");

		/* call up send method */
		ret = mx_net_send(net, buffer, strlen(buffer), BLOCKING, 1000);
		if (ret <= 0)
			fprintf(stderr, "error sending packets\n");
		else
			fprintf(stderr, "sending data %d\n", ret);
	}

}

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
	char buffer[MAX_BUFFER_LEN];
	char ipaddr[MAX_IP_STR];
	int length = MAX_BUFFER_LEN;
	int client_fd = 0;
	int server_fd = 0;
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
	net = mx_net_init(SERVER_MODE, NULL, port);
	if (!net) {
		fprintf(stderr, "network object init error\n");
		exit(1);
	}

	/* get server fd */	
	server_fd = mx_net_getserverfd(net);
	
	while (1) {
		pid_t pid;

		/* call up accept method, keep the fd by yourself */
		ret = mx_net_accept(net, &client_fd);
		if (ret < 0) {
			fprintf(stderr, "accept return %d\n", ret);
		}
#if 0
		/* mx_net_accept already do this */
		/* set up fd in order to receive data */
		ret = mx_net_setfd(net, client_fd);
#endif
		if ((pid=fork() == 0)) {
			/* child */
			close(server_fd);
			fprintf(stdout, "child running\n");
			ret = net_dispatch(net, buffer, length);
		} else if (pid < 0) {
			fprintf(stderr, "dispatch process failure\n");
		} else {
			close(client_fd);
			fprintf(stdout, "dispatching success\n");
		}
	}

	/* release net */
	mx_net_release(net);

	return 0;
}
