#ifndef OS_HEADER_NET 
#define OS_HEADER_NET 

#include <net.h>

/** 
 * @brief net object construction
 * @param options (CLIENT_MODE, SERVER_MODE)
 * @param ip address, effect only CLIENT_MODE
 * @param port connect or listen
 * @return error:NULL success:pointer 
 *
 * net_init function used by other object to nettialize the memory
 * and prepare internal net file descriptor. 
 */
net_t *net_init(int mode, char *ipaddr, int port);              /**< constructor */

/** 
 * @brief net object destruction
 * @param this net object pointer
 * @return void
 *
 * net_release function used by other object to release the memory
 * nettialized by construction.
 */
void  net_release(net_t *this); /**< destructor */


/** 
 * @brief set file descriptor to net object
 * @param this net object pointer
 * @param file descriptor
 * @return success:0
 *
 * mx_net_setfd function used by other object to set the client fd
 */
int net_setfd(net_t *this, int fd);


/** 
 * @brief set file descriptor to net object
 * @param this net object pointer
 * @param file descriptor
 * @return success: file descriptor, failure: -1
 *
 * mx_net_getfd function used by other object to get the client fd
 */
int net_getfd(net_t *this);


/** 
 * @brief set file descriptor to net object
 * @param this net object pointer
 * @param file descriptor
 * @return success: file descriptor, failure: -1
 *
 * mx_net_getserverfd function used by other object to get the server fd
 */
int net_getserverfd(net_t *this);

/** 
 * @brief get client ip from net object
 * @param this net object pointer
 * @return success: 0, failure: -1
 *
 * mx_net_getip function used by other object to get the client ip
 */
int net_getip(net_t *this, void *buffer, int length);

/** 
 * @brief send method for net object
 * @param this net object pointer
 * @param mode:(BLOCK, NONE_BLOCK)
 * @param time: blocking time in milisecond
 * @return success:>0, failure:-1, peer disconnect:0
 *
 * net_send function used by other object to send the network data.
 */
int net_send(net_t *this, int mode, int time);


/** 
 * @brief receive method net object
 * @param this net object pointer
 * @param buffer: pointer to user buffer
 * @param length: size to receive
 * @param mode:(BLOCK, NONE_BLOCK)
 * @param time: blocking time in milisecond
 * @return success:>0, failure:-1, peer disconnect:0
 *
 * net_recv function used by other object to receive the network data.
 */
int net_recv(net_t *this, void *buffer, int length, int mode, int time);


/** 
 * @brief copying to buffer method for net object
 * @param this net object pointer
 * @param buffer pointer point to user data
 * @param length of buffer need to be send
 * @return success:>0, failure:-1, peer disconnect:0
 *
 * net_setbuffer function used to copy the buffer data.
 */
int net_setbuffer(net_t *this, void *buffer, int length);


/** 
 * @brief copying from buffer method net object
 * @param this net object pointer
 * @param buffer pointer point to net object data
 * @param length of buffer receive by net_recv
 * @return success:>0, failure:-1, peer disconnect:0
 *
 * net_getbuffer function used to receive the buffer data.
 */
int net_getbuffer(net_t *this, void **buffer, int *length);


/** 
 * @brief incoming handler method net object
 * @param this net object pointer
 * @param *fd assign incoming client file descriptor
 * @return success:0, failure:-1
 *
 * mx_net_accept function used by other object to handler incomming network.
 */
int net_accept(net_t *this, int *fd); 

/** 
 * @brief privte method setsockopt for net object
 * @param this net object pointer
 * @return success:0, failure=-1
 *
 * mx_net_setsockopt function used by other object to set the socket option
 */
int net_setsockopt(net_t *this);

#endif
