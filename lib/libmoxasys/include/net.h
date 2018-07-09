/*! \file net.h
 *     \brief A header file for moxa network declare.
 *         
 *             Details.
 */
#ifndef _NET_H
#define _NET_H

#include        <stdlib.h>
#include        <stdio.h>
#include        <errno.h>
#include        <string.h>
#include        <unistd.h>
#include        <fcntl.h>
#include 	<termios.h>
#include 	<time.h>
#include	<signal.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<pthread.h>
#include	<sys/socket.h>
#include	<sys/ioctl.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<linux/tcp.h>
#include	<sys/un.h>
#include	<asm/types.h>
#include 	<string.h>

#define MAX_IP_STR	16
#define	MAX_BUFFER_LEN 8192
#define CLIENT_MODE	0
#define SERVER_MODE	1
#define BLOCKING	0
#define NON_BLOCKING	1
#define SOCKET_OPEN_ERR		10
#define SOCKET_REUSEADDR_ERR	11
#define SOCKET_KEEPCNT_ERR	12
#define SOCKET_KEEPINTVL_ERR	13
#define SOCKET_KEEPIDLE_ERR	14
#define SOCKET_BINDING_ERR	15
#define SOCKET_SEND_ERR		16
#define SOCKET_RECV_ERR		17
#define SOCKET_LISTEN_ERR	18
#define SOCKET_ACCEPT_ERR	19
#define SOCKET_MODE_ERR		20
#define SOCKET_SELECT_ERR	21
#define SOCKET_SELECT_TIMEOUT	22
#define SOCKET_NODELAY_ERR	23

#define NET_OBJ \
	/* private instant */ \
	int status;         /** status of current net object */ \
	int net_mode;         	/** CLIENT_MODE / SERVER_MODE */ \
	int block_mode;        	/** BLOCKING / NON_BLOCKING */ \
	int server_fd;         /** file descriptor of server mode */ \
	int client_fd;         /** file descriptor of client mode */ \
	int server_addr_len;   /** server address size */ \
	int client_addr_len;   /** client address size */ \
	struct sockaddr_in server_addr;   /** server sockaddr */ \
	struct sockaddr_in client_addr;   /** client sockaddr */ \
	int server_port;       /** port use by server to listen */ \
	int client_port;       /** port use by client to connect */ \
	char server_ip[MAX_IP_STR];        /** user input ip address */ \
	char transmitBuffer[MAX_BUFFER_LEN];        /** send buffer container */ \
	char receiveBuffer[MAX_BUFFER_LEN];        /** recv buffer container */ \
	unsigned long transmitLength;       /** length to send buffer */ \
	unsigned long receiveLength;       /** length buffer receive from network */ \
	struct timeval tv;       /** time waiting for send/recv */ \
	fd_set fdsr;       		/** select fd */ \
	int maxsock;       		/** select max fd */ \
\
	/* public instant */ \
\
	/* public method */ \
	int (*net_accept)	\
		(struct __NET__ *, int *fd); \
	int (*net_send)	\
		(struct __NET__ *, void *buffer, int length, int mode, int time); \
	int (*net_recv)	\
		(struct __NET__ *, void *buffer, int length, int mode, int time); \
	int (*net_getfd)	\
		(struct __NET__ *); \
	int (*net_getserverfd)	\
		(struct __NET__ *); \
	int (*net_getip)	\
		(struct __NET__ *, void *buffer, int length); \
	int (*net_setfd)	\
		(struct __NET__ *, int fd);

typedef struct __NET__
{

	NET_OBJ;

}net_t ;

/** 
 * @brief net object construction
 * @param options (CLIENT_MODE, SERVER_MODE)
 * @param ip address, effect only CLIENT_MODE
 * @param port connect or listen
 * @return error:NULL success:pointer 
 *
 * mx_net_init function used by other object to nettialize the memory
 * and prepare internal net file descriptor. 
 */
net_t *mx_net_init(int mode, char *ipaddr, int port);              /**< constructor */

/** 
 * @brief net object destruction
 * @param this net object pointer
 * @return void
 *
 * mx_net_release function used by other object to release the memory
 * nettialized by construction.
 */
void  mx_net_release(struct __NET__ *this); /**< destructor */


/** 
 * @brief set file descriptor to net object
 * @param this net object pointer
 * @param file descriptor
 * @return success:0
 *
 * mx_net_setfd function used by other object to set the client fd
 */
__inline int mx_net_setfd(struct __NET__ *this, int fd);


/** 
 * @brief set file descriptor to net object
 * @param this net object pointer
 * @param file descriptor
 * @return success: file descriptor, failure: -1
 *
 * mx_net_getfd function used by other object to get the client fd
 */
__inline int mx_net_getfd(struct __NET__ *this);


/** 
 * @brief set file descriptor to net object
 * @param this net object pointer
 * @param file descriptor
 * @return success: file descriptor, failure: -1
 *
 * mx_net_getserverfd function used by other object to get the server fd
 */
__inline int mx_net_getserverfd(struct __NET__ *this);

/** 
 * @brief get client ip from net object
 * @param this net object pointer
 * @return success: 0, failure: -1
 *
 * mx_net_getip function used by other object to get the client ip
 */
__inline int mx_net_getip(struct __NET__ *this, void *buffer, int length);

/** 
 * @brief send method for net object
 * @param this net object pointer
 * @param mode:(BLOCK, NON_BLOCK)
 * @param time: blocking time in milisecond
 * @return success:>0, failure:-1, peer disconnect:0
 *
 * mx_net_send function used by other object to send the network data.
 */
__inline int mx_net_send(struct __NET__ *this, void *buffer, int length, int mode, int time);


/** 
 * @brief receive method net object
 * @param this net object pointer
 * @param length: size to receive
 * @param buffer: pointer to user buffer
 * @param mode:(BLOCK, NON_BLOCK)
 * @param time: blocking time in milisecond
 * @return success:>receive len, failure:-1, peer disconnect:0
 *
 * mx_net_recv function used by other object to receive the network data.
 */
__inline int mx_net_recv(struct __NET__ *this, void *buffer, int length, int mode, int time);



/** 
 * @brief incoming handler method net object
 * @param this net object pointer
 * @param *fd assign incoming client file descriptor
 * @return success:0, failure:-1
 *
 * mx_net_accept function used by other object to handler incomming network.
 */
__inline int mx_net_accept(net_t *this, int *fd); 
#endif
