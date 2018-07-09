#include <net.h>
#include "debug.h"

/** 
 * @brief privte method setsockopt for net object
 * @param this net object pointer
 * @return success:0, failure=-1
 *
 * mx_net_setsockopt function used by other object to set the socket option
 */
int net_setsockopt(struct __NET__ *this)
{
	char value = 0;
	int on = 0;
	int ret = 0;

	/* open socket */
	if (this->net_mode == CLIENT_MODE) {
		this->client_fd = socket(AF_INET,SOCK_STREAM,0) ;
		this->client_addr.sin_family = AF_INET ;
		this->client_addr.sin_addr.s_addr = inet_addr(this->server_ip) ;
		this->client_addr.sin_port = htons(this->server_port) ;
		this->client_addr_len = sizeof(this->client_addr) ;
		if ( this->client_fd == -1) {
			this->status = SOCKET_OPEN_ERR;
			return -1;
		}
	}
	else {
		this->server_fd = socket(AF_INET,SOCK_STREAM,0) ;
		this->server_addr.sin_family = AF_INET ;
		this->server_addr.sin_addr.s_addr = htons(INADDR_ANY) ;
		this->server_addr.sin_port = htons(this->server_port) ;
		this->server_addr_len = sizeof(this->server_addr) ;
		if ( this->server_fd == -1) {
			this->status = SOCKET_OPEN_ERR;
			return -1;
		}
	}

	/* setsockopt */
	on = 1 ;
	if (this->net_mode == CLIENT_MODE) {
		ret = setsockopt(this->client_fd, IPPROTO_TCP, TCP_NODELAY, &on,\
				sizeof(on)) ;
	}
	else {
		ret = setsockopt(this->server_fd, IPPROTO_TCP, TCP_NODELAY, &on,\
				sizeof(on)) ;
	}
	if (ret == -1) {
		this->status = SOCKET_NODELAY_ERR;
		return -1;
	}
	
	if (this->net_mode == CLIENT_MODE) {
		ret = setsockopt(this->client_fd, SOL_SOCKET,SO_REUSEADDR, &this->client_addr_len,\
				sizeof(this->client_addr_len)) ;
	}
	else {
		ret = setsockopt(this->server_fd, SOL_SOCKET,SO_REUSEADDR, &this->server_addr_len,\
				sizeof(this->server_addr_len)) ;
	}
	if (ret == -1) {
		this->status = SOCKET_REUSEADDR_ERR;
		return -1;
	}

	if (this->net_mode == SERVER_MODE) {
		on = 2 ;
		ret = setsockopt(this->server_fd, IPPROTO_TCP,TCP_KEEPCNT, (char *)&on, sizeof(on)) ;
		if (ret == -1) {
			this->status = SOCKET_KEEPCNT_ERR;
			return -1;
		}

		on = 1 ;
		ret = setsockopt(this->server_fd, IPPROTO_TCP,TCP_KEEPINTVL, (char *)&on, sizeof(on)) ;
		if (ret == -1) {
			this->status = SOCKET_KEEPINTVL_ERR;
			return -1;
		}
	}

	value = 3 ;
	if (this->net_mode == CLIENT_MODE) {
		setsockopt(this->client_fd, IPPROTO_TCP,TCP_KEEPIDLE,(char *)&value, sizeof(value)) ;
	}
	else {
		setsockopt(this->server_fd, IPPROTO_TCP,TCP_KEEPIDLE,(char *)&value, sizeof(value)) ;
	}
	if (ret == -1) {
		this->status = SOCKET_KEEPIDLE_ERR;
		return -1;
	}

	/* connect or listen */
	if (this->net_mode == CLIENT_MODE) {
		/* client mode */
	       	ret = connect(this->client_fd, (struct sockaddr *)&(this->client_addr),\
			       	this->client_addr_len) ;
	}
	else {
		/* server mode */
		ret = bind(this->server_fd, (struct sockaddr *)&(this->server_addr), this->server_addr_len);
	}
	if (ret == -1) {
		this->status = SOCKET_BINDING_ERR;
		return -1;
	}

	/* server listen */
	if (this->net_mode == SERVER_MODE) {
		ret = listen(this->server_fd, 10);
		if (ret == -1) {
			this->status = SOCKET_LISTEN_ERR;
			return -1;
		}
	}

	return 0;
}

/** 
 * @brief set file descriptor to net object
 * @param this net object pointer
 * @param file descriptor
 * @return success:0
 *
 * mx_net_setfd function used by other object to set the client fd
 */
int net_setfd(struct __NET__ *this, int fd)
{
	/* only used by server situation */
	if (this->net_mode == SERVER_MODE)
	       	this->client_fd = fd;

	return 0;
}

/** 
 * @brief get file descriptor from net object
 * @param this net object pointer
 * @return success: file descriptor, failure: -1
 *
 * mx_net_getfd function used by other object to get the client fd
 */
int net_getfd(struct __NET__ *this)
{
	/* only used by server situation */
	if (this->net_mode == SERVER_MODE)
		return this->client_fd;
	else return -1;
}

/** 
 * @brief get file descriptor from net object
 * @param this net object pointer
 * @return success: file descriptor, failure: -1
 *
 * mx_net_getserverfd function used by other object to get the server fd
 */
int net_getserverfd(struct __NET__ *this)
{
	/* only used by server situation */
	if (this->net_mode == SERVER_MODE)
		return this->server_fd;
	else return -1;
}

/** 
 * @brief get client ip from net object
 * @param this net object pointer
 * @return success: 0, failure: -1
 *
 * mx_net_getip function used by other object to get the client ip
 */
int net_getip(struct __NET__ *this, void *buffer, int length)
{
	/* only used by server situation */
	if ((this->net_mode == SERVER_MODE) && (length >= MAX_IP_STR) && (buffer != NULL)) {
		strcpy((char *)buffer, inet_ntoa(this->client_addr.sin_addr));
		return 0;
	}
	else return -1;
}

/** 
 * @brief send method for net object
 * @param this net object pointer
 * @param mode:(BLOCK, NON_BLOCK)
 * @param time: blocking time in milisecond
 * @return success:>0, failure:-1, peer disconnect:0
 *
 * mx_net_send function used by other object to send the network data.
 */
int net_send(struct __NET__ *this, void *buffer, int length, int mode, int time)
{
	int ret = 0;
	int status = 0;

	/* check fd */
	if (this->client_fd <= 0) {
		this->status = SOCKET_BINDING_ERR;
		return -1;
	}

	/* if mode choose NON-BLOCKING, do it here */
	if (mode == NON_BLOCKING) {
		fcntl(this->client_fd, F_SETFL, O_NONBLOCK);
		this->tv.tv_usec = (time % 1000) * 1000;
		this->tv.tv_sec = time / 1000;
		FD_ZERO(&this->fdsr) ;
		this->maxsock = this->client_fd + 1;
	    FD_SET(this->client_fd, &this->fdsr) ;
		status = select(this->maxsock, NULL, &this->fdsr, NULL, &this->tv);
		if (status > 0) {
		    if(FD_ISSET(this->client_fd, &this->fdsr)) {
				/* real case */
				ret = write(this->client_fd, buffer, length);
			}
		}
		else if (status < 0){
			/* select error */
			this->status = SOCKET_SELECT_ERR;
			return -1;
		}
		else {
			/* nothing happend, just time out */
			this->status = SOCKET_SELECT_TIMEOUT;
			return 0;
		}
	}
	else {

		/* real case */
		//ret = send(this->client_fd, this->transmitBuffer, this->transmitLength, 0);
		ret = write(this->client_fd, buffer, length);
	}

	if (ret <= 0) {
		this->status = SOCKET_SEND_ERR;
		return -1;
	}
	return ret;
}

/** 
 * @brief receive method net object
 * @param this net object pointer
 * @param buffer: pointer to user buffer
 * @param length: size to receive
 * @param mode:(BLOCK, NON_BLOCK)
 * @param time: blocking time in milisecond
 * @return success:>0, failure:-1, peer disconnect:0
 *
 * mx_net_recv function used by other object to receive the network data.
 */
int net_recv(struct __NET__ *this, void *buffer, int length, int mode, int time)
{
	int ret = 0;
	int status = 0;


	/* check fd */
	if (this->client_fd <= 0) {
		this->status = SOCKET_BINDING_ERR;
		return -1;
	}

#if 0
	/* check if length over running */
	if (length > MAX_BUFFER_LEN) {
		this->status = SOCKET_BINDING_ERR;
		return 0;
	}
#endif

	/* if mode choose NON-BLOCKING, do it here */
	if (mode == NON_BLOCKING) {
		fcntl(this->client_fd, F_SETFL, O_NONBLOCK);
		this->tv.tv_usec = (time % 1000) * 1000;
		this->tv.tv_sec = time / 1000;
		FD_ZERO(&this->fdsr) ;
		this->maxsock = this->client_fd + 1;
	    FD_SET(this->client_fd, &this->fdsr) ;
		status = select (this->maxsock, &this->fdsr, NULL, NULL, &this->tv);
		if (status > 0) {
		    if(FD_ISSET(this->client_fd, &this->fdsr)) {
				/* real case */
				ret = read(this->client_fd, buffer, length);
			}
		}
		else if (status < 0){
			/* select error */
			this->status = SOCKET_SELECT_ERR;
			return -1;
		}
		else {
			/* nothing happend, just time out */
			this->status = SOCKET_SELECT_TIMEOUT;
			return 0;
		}
	}
	else {

		/* real case */
		ret = read(this->client_fd, buffer, length);
	}

	if (ret < 0) {
		this->status = SOCKET_RECV_ERR;
		return -1;
	}
	else if(ret == 0) {
		this->status = SOCKET_BINDING_ERR;
		return 0;
	}
	else
	       	this->receiveLength = ret;
	return ret;
}


/** 
 * @brief copying to buffer method for net object
 * @param this net object pointer
 * @param buffer pointer point to user data
 * @param length of buffer need to be send
 * @return success:>0, failure:-1, peer disconnect:0
 *
 * mx_net_setbuffer function used to copy the buffer data.
 */
int net_setbuffer(struct __NET__ *this, void *buffer, int length)
{
	this->transmitLength = length;
	memset(&this->transmitBuffer, '\0', sizeof(char)*MAX_BUFFER_LEN);
	if (memcpy(&this->transmitBuffer, buffer, length) != NULL)
	       	return length;
	else
	       	return -1;
}


/** 
 * @brief copying from buffer method net object
 * @param this net object pointer
 * @param buffer pointer point to net object data
 * @param length of buffer receive by net_recv
 * @return success:>0, failure:-1, peer disconnect:0
 *
 * mx_net_getbuffer function used to receive the buffer data.
 */
int net_getbuffer(struct __NET__ *this, void **buffer, int *length)
{
	if (this->receiveLength < 0)
	       	return -1;
	*length = this->receiveLength;
	*buffer = this->receiveBuffer;
	return this->receiveLength;
}


/** 
 * @brief incoming handler method net object
 * @param this net object pointer
 * @param *fd assign incoming client file descriptor
 * @return success:0, failure:-1
 *
 * mx_net_accept function used by other object to handler incomming network.
 */
int net_accept(struct __NET__ *this, int *fd)
{
	int ret = 0;

	if (this->net_mode == CLIENT_MODE) {
		this->status = SOCKET_MODE_ERR;
		return -1;
	}

	/* check fd */
	if (this->server_fd <= 0) {
		this->status = SOCKET_ACCEPT_ERR;
		return -1;
	}

	/* real case */
	this->client_addr_len = sizeof(struct sockaddr_in);
	*fd = accept(this->server_fd, (struct sockaddr *)&this->client_addr, (socklen_t *)&this->client_addr_len) ;

	//fprintf(stdout, "accept incomming ip:%s\n", inet_ntoa(((struct sockaddr_in *)&this->client_addr)->sin_addr));

	if (*fd <= 0) {
		this->status = SOCKET_ACCEPT_ERR;
		return -1;
	}
	else
		this->client_fd = *fd;

	return ret;
}
