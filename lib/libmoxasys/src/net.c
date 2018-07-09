#include <stdlib.h>
#include <string.h>
#include "net.h"
#include "os/net_api.h"


/** 
 * @brief net object construction
 * @param net file path
 * @return error:NULL success:pointer 
 *
 * mx_net_init function used by other object to nettialize the memory
 * and prepare internal net file descriptor. 
 */
net_t *mx_net_init(int mode, char *ipaddr, int port)              /**< constructor */
{
	net_t *this;


	this = (net_t *)malloc(sizeof(net_t));
	if (!this)
		return NULL;
	memset(this, 0x0, sizeof(net_t));

	/* init instant */
	this->status = 0;
	this->net_mode = mode;
	this->block_mode = BLOCKING;
	this->server_fd = -1;
	this->client_fd = -1;
	this->server_addr_len = 0;
	this->client_addr_len = 0;
	memset(&this->server_addr, 0, sizeof(struct sockaddr_in));
	memset(&this->client_addr, 0, sizeof(struct sockaddr_in));
	memset(&this->server_ip, '\0', sizeof(char)*MAX_IP_STR);
	if (this->net_mode == CLIENT_MODE) {
		strcpy(this->server_ip, ipaddr);
	}
	this->server_port = port;
	memset(&this->transmitBuffer, '\0', sizeof(char)*MAX_BUFFER_LEN);
	memset(&this->receiveBuffer, '\0', sizeof(char)*MAX_BUFFER_LEN);
	this->transmitLength = 0;
	this->receiveLength = 0;

	/* method assign */
	this->net_accept      = net_accept;
	this->net_send        = net_send;
	this->net_recv        = net_recv;
	this->net_setfd       = net_setfd;
	this->net_getfd       = net_getfd;
	this->net_getserverfd = net_getserverfd;
	this->net_getip       = net_getip;

	/* check validation */
	if (this->server_port == 0) {
		fprintf(stderr, "No port setting\n");
		goto error;
	}
	if ((this->net_mode == CLIENT_MODE) && (this->server_ip[0] == '\0')) {
		fprintf(stderr, "No ip address setting\n");
		goto error;
	}

	/* os depend */
	if (net_setsockopt(this) != 0) {
		fprintf(stderr, "net setsockopt error %d\n", this->status);
		goto error;
	}

	return this;

error:
	mx_net_release(this);
	return NULL;
}


/** 
 * @brief net object destruction
 * @param this net object pointer
 * @return void
 *
 * mx_net_release function used by other object to release the memory
 * nettialized by construction.
 */
void mx_net_release(net_t *this)
{
	if (this != NULL) {
		if (this && this->server_fd)
			close(this->server_fd);
		if (this && this->client_fd)
			close(this->client_fd);
		free(this);
	}
}

/** 
 * @brief set file descriptor to net object
 * @param this net object pointer
 * @param file descriptor
 * @return success:0
 *
 * mx_net_setfd function used by other object to set the client fd
 */
__inline int mx_net_setfd(net_t *this, int fd) {
	return this->net_setfd(this, fd);
}


/** 
 * @brief get file descriptor from net object
 * @param this net object pointer
 * @return success: file descriptor, failure: -1
 *
 * mx_net_getfd function used by other object to get the client fd
 */
__inline int mx_net_getfd(net_t *this) {
	return this->net_getfd(this);
}

/** 
 * @brief get file descriptor from net object
 * @param this net object pointer
 * @return success: file descriptor, failure: -1
 *
 * mx_net_getserverfd function used by other object to get the server fd
 */
__inline int mx_net_getserverfd(net_t *this)
{
	return this->net_getserverfd(this);
}

/** 
 * @brief get client ip from net object
 * @param this net object pointer
 * @return success: 0, failure: -1
 *
 * mx_net_getip function used by other object to get the client ip
 */
__inline int mx_net_getip(net_t *this, void *buffer, int length)
{
	return this->net_getip(this, buffer, length);
}

/** 
 * @brief send method for net object
 * @param this net object pointer
 * @param mode:(BLOCK, NONE_BLOCK)
 * @param time: blocking time in milisecond
 * @return success:>0, failure:-1, peer disconnect:0
 *
 * mx_net_send function used by other object to send the network data.
 */
__inline int mx_net_send(net_t *this, void *buffer, int length, int mode, int time) {
	return this->net_send(this, buffer, length, mode, time);
}


/** 
 * @brief receive method net object
 * @param this net object pointer
 * @param buffer: pointer to user buffer
 * @param length: size to receive
 * @param mode:(BLOCK, NONE_BLOCK)
 * @param time: blocking time in milisecond
 * @return success:>0, failure:-1, peer disconnect:0
 *
 * mx_net_recv function used by other object to receive the network data.
 */
__inline int mx_net_recv(net_t *this, void *buffer, int length, int mode, int time) {
	return this->net_recv(this, buffer, length, mode, time);
}


/** 
 * @brief incoming handler method net object
 * @param this net object pointer
 * @param *fd assign incoming client file descriptor
 * @return success:0, failure:-1
 *
 * mx_net_accept function used by other object to handler incomming network.
 */
__inline int mx_net_accept(net_t *this, int *fd) {
	return this->net_accept(this, fd);
}
