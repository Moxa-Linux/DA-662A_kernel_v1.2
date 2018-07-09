#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <debug.h>
#include <unistd.h>
#include <net.h>

#define TEST_STR1	"Hello World!!!!"
#define TEST_STR2	"1234567890"

int main(int argc, char *argv[])
{
	char rbuffer[MAX_BUFFER_LEN];
	char sbuffer[MAX_BUFFER_LEN];
	char *ipaddr = "127.0.0.1";
	int fd = 0;
	int ret = 0; 
	int mode = 0;	/* default client */
	int port = 1234;
	int length = MAX_BUFFER_LEN;
	int i, j;
	pid_t pid;
	net_t	*server;
	net_t	*client;

	/* forking */
	if ((pid = fork()) > 0) {
		/* parent act as server */
		server = mx_net_init(SERVER_MODE, NULL, port);
		if (!server) {
			fprintf(stderr, "server network object init error\n");
			exit(1);
		}

		fprintf(stdout, "server waiting accept\n");
		ret = mx_net_accept(server, &fd);
		if (ret < 0) {
			fprintf(stderr, "accept fail %d\n", server->status);
			return 1;
		}

		ret = mx_net_setfd(server, fd);
		if (ret < 0) {
			fprintf(stderr, "setfd fail %d\n", server->status);
			return 1;
		}

		/* testing 100 times */
		for (i=0; i<100; i++){
			/* randomly test block/non-block mode */
			if((i%2)==1)
			       	ret = mx_net_recv(server, (void *)rbuffer, length, BLOCKING, 1000);
			else
			       	ret = mx_net_recv(server, (void *)rbuffer, length, NON_BLOCKING, 1000);
			if (ret < 0) {
				fprintf(stderr, "recv fail %d\n", server->status);
				return 1;
			}
			else {
				/* check data */
				if (strncmp(rbuffer, TEST_STR1, strlen(TEST_STR1))) {
					printf("buffer [%s], TEST_STR1 [%s]\n", rbuffer, TEST_STR1);
					fprintf(stderr, "server error receiving buffer\n");
				}
			}

			/* rubbish data */
			strcpy(sbuffer, TEST_STR2);

			if((i%2)==1)
			       	ret = mx_net_send(server, sbuffer, strlen(sbuffer), BLOCKING, 1000);
			else
			       	ret = mx_net_send(server, sbuffer, strlen(sbuffer), NON_BLOCKING, 1000);

			if (ret <= 0)
				fprintf(stderr, "server error sending packets\n");

			i++;
		}
		
		/* release server instant */
		mx_net_release(server);
		sleep(2);
	}
	else if (pid == 0) {
		/* child act as client */
		sleep(1); /* wait for server process up */
		client = mx_net_init(CLIENT_MODE, ipaddr, port);
		if (!client) {
			fprintf(stderr, "client network object init error\n");
			exit(1);
		}

		strcpy(sbuffer, TEST_STR1);
		while (1) {
		       	ret = mx_net_send(client, sbuffer, strlen(sbuffer), NON_BLOCKING, 10);
			if (ret < 0) {
				fprintf(stderr, "client error sending packets, %d\n", client->status);
			}
			else{
				fprintf(stderr, "server shutdown\n");
				sleep (2);
				exit (0);
			}

		       	ret = mx_net_recv(client, (void *)rbuffer, length, NON_BLOCKING, 10);
			if (ret < 0) {
				fprintf(stderr, "client error recv packets, %d\n", client->status);
			}
			else{
				if (strncmp(rbuffer, TEST_STR2, strlen(TEST_STR2)))
					fprintf(stderr, "client error receiving buffer\n");
				sleep (2);
				exit (0);
			}

		} //while loop
		
		/* server exit, client will be automatically terminate */
		fprintf(stdout, "client testing complete\n");
	}
	else {
		fprintf(stderr, "forking failure\n");
		exit(1);
	}

	printf("\n\n\n");
	printf("network testing complete\n");


	return 0;
}
