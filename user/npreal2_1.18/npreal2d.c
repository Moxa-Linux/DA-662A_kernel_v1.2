/*
 *	Copyright (C) 2001-2012  Moxa Inc.
 *	All rights reserved.
 *
 *	Moxa NPort/Async Server UNIX Real TTY daemon program.
 *
 *	Usage: npreal2d [-t reset-time]
 *
 *	Compilation instructions:
 *		LINUX:	cc -O -o npreal2d npreal2d.c
 *
 */

#define		NPREAL_VERSION			"1.18 Build 12020919"

#include	<sys/types.h>
#include	<sys/socket.h>
#include	<sys/time.h>
#include	<sys/param.h>
#include	<netinet/in.h>
#include	<netdb.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<errno.h>
#include	<time.h>
#include	<string.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<signal.h>
#include	<sys/ioctl.h>
#include	<sys/sysinfo.h>
#ifdef	STREAM
#include	<sys/ptms.h>
#endif
#ifdef	SSL_ON
#include	<openssl/ssl.h>
#endif

#include	<arpa/inet.h>
#include	"redund.h"
#include	"npreal2d.h"


#ifdef BIG_ENDIAN_PLATFORM 	//add by Jared 08-26-2005
#define cpu_to_le16(A)  ((((A) & 0xff00) >> 8) | ((A) & 0x00ff) << 8)
#define cpu_to_le32(A) ((((A) & 0xff000000) >> 24) | \
                  (((A) & 0x00ff0000) >> 8) | \
                  (((A) & 0x0000ff00) << 8) | \
                  (((A) & 0x000000ff) << 24))
#endif


#if 0
#undef		FD_SETSIZE
#define		FD_SETSIZE		1024

#define	NPREAL_ASPP_COMMAND_SET		1
#define	NPREAL_LOCAL_COMMAND_SET	2

/* local command set */
#define	LOCAL_CMD_TTY_USED		1
#define	LOCAL_CMD_TTY_UNUSED		2
#define NPREAL_NET_CONNECTED		3
#define NPREAL_NET_DISCONNECTED		4
#define NPREAL_NET_SETTING		5

/*
 * Moxa tty definition
 */
#define	ASPP_CMD_NOTIFY		0x26
#define	ASPP_CMD_POLLING	0x27
#define	ASPP_CMD_ALIVE		0x28

#define	ASPP_CMD_IOCTL		16
#define	ASPP_CMD_FLOWCTRL	17
#define	ASPP_CMD_LSTATUS	19
#define	ASPP_CMD_LINECTRL	18
#define	ASPP_CMD_FLUSH		20
#define	ASPP_CMD_OQUEUE		22
#define	ASPP_CMD_SETBAUD	23
#define	ASPP_CMD_START_BREAK	33
#define	ASPP_CMD_STOP_BREAK	34
#define	ASPP_CMD_START_NOTIFY	36
#define	ASPP_CMD_STOP_NOTIFY	37
#define	ASPP_CMD_HOST		43
#define	ASPP_CMD_PORT_INIT	44
#define	ASPP_CMD_WAIT_OQUEUE 	47

#define	ASPP_CMD_IQUEUE		21
#define	ASPP_CMD_XONXOFF	24
#define	ASPP_CMD_PORT_RESET	32
#define	ASPP_CMD_RESENT_TIME	46
#define	ASPP_CMD_TX_FIFO	48
#define ASPP_CMD_SETXON     51
#define ASPP_CMD_SETXOFF    52


#define	CMD_RETRIEVE		1
#define	CMD_RESPONSE		2
#define	CMD_CONNECTED		3
#define	CMD_DISCONNECTED	4
#define	CMD_NET_SETTING		5
#define	CMD_GET_TTY_STATUS	6


/* The following defines the server type */

#define	DE311	311
#define	DE301	301
#define	DE302	302
#define	DE304	304
#define	DE331	331
#define	DE332	332
#define	DE334	334
#define	DE303	303
#define	DE308	308
#define	DE309	309
#define	CN2100	2100
#define	CN2500	2500

#define 	CMD_BUFFER_SIZE	84	/* temporary cmd buffer size	*/

/*dsci command for ipv6*/
#define EX_VERSION          1
#define DSCI_IPV6           0x79
#define DSCI_IPV6_RESPONS   0xF9
/* CATALOG */
#define KERNEL_FUN	    0x0001
#define NETWORK_CONFIG  0x0002
/* SUBCODE */
#define DSC_ENUMSEARCH       0x0001
#define DSC_GETNETSTAT_V6    0x0004

#define MAX_SOCK_V6     64

struct net_node_setting
{
    int32_t	server_type;
    int32_t	disable_fifo;
};

typedef struct _TTYINFO
{
    char		mpt_name[40];	/* Master pseudo TTY name	*/
    char		ip_addr_s[40];	/* print format of IP address */
	struct	redund_struct redund;
    char		redundant_ip_addr_s[40];	/* print format of IP address for redundant_mode */
    int			redundant_mode;
    u_char	ip6_addr[16];   /* Server IP address		*/
    int		af;             /*IP type*/
	char	scope_id[10];
    int     reconn_flag;      /*1:reconnect 0:not reconnect*/
    int32_t		server_type;
    int32_t		disable_fifo;
    int		tcp_port;	/* Server TCP port for data	*/
    int		cmd_port;	/* Server TCP port  for cmd	*/
    int		local_tcp_port;	/* Client TCP port for data	*/
    int		local_cmd_port;	/* Client TCP port for cmd	*/
    int		mpt_fd; 	/* Master pseudo tty file handle*/
    int		sock_fd;	/* TCP Socket handle		*/
    int		sock_cmd_fd;	/* TCP Socket handlefor cmd	*/
    int		state;		/* TCP connection state 	*/
    char *		mpt_bufptr;	/* Master pseudo TTY data buffer*/
    int		mpt_datakeep;
    int		mpt_dataofs;
    char *		sock_bufptr;	/* TCP socket data buffer	*/
    int		sock_datakeep;
    int		sock_dataofs;
    char 		mpt_cmdbuf[CMD_BUFFER_SIZE];/* TTY cmd buffer*/
    int		mpt_cmdkeep;
    char 		sock_cmdbuf[CMD_BUFFER_SIZE];/* TCP cmd buffer	*/
    int		sock_cmdkeep;
    int		error_flags;
    time_t		time_out;	/* Using for TCP connection check */
    int		serv_index;
    char		tcp_wait_id;
    time_t		tty_used_timestamp;
    time_t      first_servertime;
#ifdef	SSL_ON
    SSL *	pssl;
    int		ssl_enable;
    time_t		ssl_time;
#endif
    int		pipe_port[2];

}
TTYINFO;

typedef struct _ConnMsg
{
    TTYINFO *	infop;
    int		status;
    char		tcp_wait_id;
}
ConnMsg;

typedef struct _SERVERINFO
{
	u_char		ip6_addr[16];
	int			af;
	
    u_short 	dev_type;
    u_short 	serial_no;
    time_t		last_servertime;
    time_t		next_sendtime;

// Below is for DSCI protocol.
    uint32_t        ap_id;
    u_short         hw_id;
    unsigned char   mac[6];
	u_short			dsci_ver;
	u_short			start_item;
}
SERVINFO;


typedef struct _DSCI_HEADER
{
    unsigned char   opcode;
    unsigned char   result;
    u_short         length;
    uint32_t        id;
}
DSCI_HEADER;

typedef struct _DSCI_RET_HEADER
{
    uint32_t        ap_id;
    u_short         hw_id;
    unsigned char   mac[6];
}
DSCI_RET_HEADER;

typedef struct _DSCI_DA_DATA
{
    uint32_t        ap_id;
    u_short         hw_id;
    unsigned char   mac[6];
}
DSCI_DA_DATA;

typedef struct _DSCI_NET_STAT
{
    uint32_t        remote_ip;
    uint32_t        local_ip;
    u_short         remote_port;
    u_short         local_port;
    char            sock_type;
    unsigned char   status;
    unsigned char   reverse[2];
}
DSCI_NET_STAT;

typedef struct _DSCI_NET_STAT_IPV6
{
    u_char          remote_ip[16];
    u_char          local_ip[16];
    u_short         remote_port;
    u_short         local_port;
    char            sock_type;
    unsigned char   status;
    unsigned char   reverse[2];
}
DSCI_NET_STAT_IPV6;

typedef struct _EX_HEADER
{
	u_char		ex_vision;
	char		reservd[3];
	u_short		catalog;
	u_short		subcode;
}EX_HEADER;

union sock_addr
{
	struct sockaddr_in sin;
	struct sockaddr_in6 sin6;
};

#define 	STATE_INIT		0
#define 	STATE_MPT_OPEN		1
#define 	STATE_CONN_FAIL 	2
#define 	STATE_TCP_OPEN		3
#define 	STATE_TCP_CONN		4
#ifdef SSL_ON
#define 	STATE_SSL_CONN	 	5
#define 	STATE_TCP_CLOSE 	6
#define 	STATE_TCP_WAIT		7
#define 	STATE_MPT_REOPEN	8
#define 	STATE_TTY_WAIT		9
#define 	STATE_RW_DATA		10
#define 	STATE_REMOTE_LISTEN	11
#else
#define 	STATE_TCP_CLOSE 	5
#define 	STATE_TCP_WAIT		6
#define 	STATE_MPT_REOPEN	7
#define 	STATE_TTY_WAIT		8
#define 	STATE_RW_DATA		9
#define 	STATE_REMOTE_LISTEN	10
#endif

#define 	CONNECT_OK	0
#define 	CONNECT_FAIL	1
#define 	CLOSE_OK	2

#define 	TCP_LISTEN	1
#define 	TCP_CONNECTED	4

#define 	ERROR_MPT_OPEN	0x0001
#define 	ERROR_TCP_OPEN	0x0002
#define 	ERROR_TCP_CONN	0x0004
#define 	ERROR_FORK	0x0008

#define 	BUFFER_SIZE	1024	/* temporary data buffer size	*/
#define 	MAX_TTYS	256	/* max. handling TTYs number	*/
#define		MAX_PORTS	16	/* max. port of server is 16    */

#define		IS_IPV4		0
#define		IS_IPV6		1
#endif
/* The mode which daemon will be waken up */
int		Graw_mode = 0;
int		Gredund_mode = 0;

int		ttys, servers;
TTYINFO 	ttys_info[MAX_TTYS];
SERVINFO	serv_info[MAX_TTYS];
char		EventLog[160];		/* Event log file name */
int		pipefd[2];
int		maxfd;
int     timeout_time = 0;
int		polling_time=0; 	/* default disable polling function */
int		polling_fd;
int     polling_nport_fd[2];
int		Restart_daemon;
static	int	No_tty_defined;
static	int enable_ipv6=2;  /*2 enable ipv6, 1 disenable ipv6*/
#define EN_IPV6   2
#define DIS_IPV6  1
#ifdef	STREAM
extern	char	*ptsname();
#endif
#ifdef	SSL_ON
static void ssl_init(void);
SSL_CTX *sslc_ctx;
#endif

#ifndef	STREAM
void	restart_handle ();
void	wait_handle ();
void	connect_wait_handle ();
void    config_changed_handle ();
#endif
#ifdef	STREAM
void	restart_handle (int);
void	wait_handle (int);
void	connect_wait_handle (int);
void    config_changed_handle (int);
#endif
int     Gconfig_changed;
char    Gcffile[160];

/*
 *	MOXA TTY daemon main program
 */
main(argc, argv)
int	argc;
char *	argv[];
{
    TTYINFO *	infop;
    char ver[100];
    int		i;
    Restart_daemon = 0;
    No_tty_defined = 0;
	polling_fd = -1; /* Add by Ying */

	for(i=0; i<2; i++)
	    polling_nport_fd[i] = -1;
    while (1)
    {
        if (Restart_daemon)
        {
            /* Add by Ying */
			 if (polling_fd >= 0)
            {
                close(polling_fd);
                polling_fd = -1;
            }
			for(i=0; i<2; i++)
			{
	            if (polling_nport_fd[i] >= 0)
	            {
	                close(polling_nport_fd[i]);
	                polling_nport_fd[i] = -1;
	            }
			}
            /* */

            infop = ttys_info;
            close (pipefd[0]);
            close (pipefd[1]);
            for (i = 0;i < ttys;i++)
            {
                infop->reconn_flag = 1;
                if (infop->sock_fd >= 0)
                {
#ifdef SSL_ON
                    if (infop->ssl_enable)
                    {
                        SSL_shutdown(infop->pssl);
                        SSL_free(infop->pssl);
                        infop->pssl = NULL;
                    }
#endif
                    close(infop->sock_fd);
                }
                if (infop->sock_cmd_fd >= 0)
                    close(infop->sock_cmd_fd);
                if (infop->mpt_fd >= 0)
                    close(infop->mpt_fd);
                infop++;
            }
        }
        if (Restart_daemon == 1)
        {
#ifndef	STREAM
            signal (SIGTERM, ( (void (*)()) wait_handle) );
#endif
#ifdef	STREAM
            signal (SIGTERM, wait_handle);
#endif
            pause();
        }
        /*
         * Read the poll time & the pesudo TTYs configuation file.
        	*/

        if ( (argc > 2) && (strcmp(argv[1], "-t") == 0) )
        {
            timeout_time = 60 * atoi(argv[2]);
            polling_time = timeout_time;
            if ( polling_time >= 60 )
            {
                polling_time = (polling_time - 20) / 4;
            }
        }

        if ( moxattyd_read_config(argv[0]) <= 0 )
        {
            if (!No_tty_defined)
            {
                log_event ("Not any tty defined");
                No_tty_defined = 1;
            }
            break;
//			usleep(1000);
//			continue;
        }
        No_tty_defined = 0;
        /*
         * Initialize this Moxa TTYs daemon process.
         */
        if (!Restart_daemon)
            moxattyd_daemon_start();

        /*
         * Initialize polling async server function.
         */
        if ( polling_time && (poll_async_server_init() < 0) )
        {
            continue;
        }

        /*
         * Open PIPE, set read to O_NDELAY mode.
        	*/
        if ( pipe(pipefd) < 0 )
        {
            log_event("pipe error !");
            continue;
        }
#ifdef	O_NDELAY
        fcntl(pipefd[0], F_SETFL, fcntl(pipefd[0], F_GETFL) | O_NDELAY);
#endif
        signal(SIGCLD, SIG_IGN);

        Restart_daemon = 0;
        sprintf(ver, "MOXA Real TTY daemon program starting (Ver%s)...", NPREAL_VERSION);
        log_event(ver);
#ifndef	STREAM
        signal (SIGTERM, ( (void (*)())restart_handle) );
#endif
#ifdef	STREAM
        signal (SIGTERM, restart_handle);
#endif

        /*
        	* Handle Moxa TTYs data communication.
        	*/
#ifdef SSL_ON
        ssl_init();
#endif
#ifndef STREAM
            signal (SIGUSR1, ( (void (*)()) config_changed_handle) );
#endif
#ifdef  STREAM
            signal (SIGUSR1, config_changed_handle);
#endif
		Gconfig_changed = 0;
		if (Graw_mode)
        	moxattyd_handle_ttys(); /* child process ok */
		else
			return -1;
    }
}
int lib_name2ip6(infop)
TTYINFO *infop;
{
	struct hostent *host;
	struct in_addr ip1, ip2;
	if(inet_pton(AF_INET, infop->ip_addr_s, (u_long*)infop->ip6_addr) <= 0)
	{
		if(inet_pton(AF_INET6, infop->ip_addr_s, infop->ip6_addr) > 0)
		{
			infop->af = AF_INET6;
			return 0;
		}
	}
	else
	{
//printf("[AP] lib_name2ip6  1.2, redund_mode = %d\n", infop->redundant_mode);
		infop->af = AF_INET;
		if (infop->redundant_mode) {
#if 1
			inet_pton(AF_INET, infop->redund.redund_ip, (u_long*)infop->redund.ip6_addr);
			memcpy(&ip2, &infop->redund.ip6_addr[0], 4);
//			printf("[AP]-----------------lib addr 2 = %s-----------------\n", inet_ntoa(ip2));
#endif		
		}
		memcpy(&ip1, &infop->ip6_addr[0], 4);
//			printf("[AP]-----------------lib addr 1 = %s------------------\n",inet_ntoa(ip1));
		return 0;
	}

	host = gethostbyname2(infop->ip_addr_s, AF_INET6);
	if(host == NULL)
		host = gethostbyname2(infop->ip_addr_s, AF_INET);
		
	if(host)
	{
		if(host->h_addrtype == AF_INET)
		{
			*(u_long*)infop->ip6_addr = ((struct in_addr *)host->h_addr)->s_addr;
			infop->af = AF_INET;
			return 0;
		}
		else if(host->h_addrtype == AF_INET6)
		{
			memcpy(infop->ip6_addr, host->h_addr, 16);
			infop->af = AF_INET6;
			return 0;
		}
	}
		
	return -1;
}


/*
 *	Prepare LOG file and read the config TTY records.
 *
 */
int	moxattyd_read_config(cmdpath)
char *	cmdpath;
{
    int		n, data, cmd;
    FILE *		ConfigFd;
    struct hostent *host;
    TTYINFO *	infop;
    char		workpath[160], buf[160];
    char		ttyname[160],tcpport[16],cmdport[16];
	char		ttyname2[160], curname[160], scope_id[10];
    int			redundant_mode;
    int32_t		server_type,disable_fifo;
#ifdef SSL_ON
    int32_t		ssl_enable;
#else
    int32_t            temp;
#endif

    redundant_mode = 0;
// Scott: 2005-10-03
// The original design will lead to an incorrect workpath.
// Use fixed path instead.
    sprintf(workpath, "/usr/lib/npreal2/driver");

    /*
     * Prepare the full-path file names of LOG/Configuration.
     */
#if 0 // Jared, 2013-07-30
    sprintf(buf,"%s/npreal2d.cf", workpath);        /* Config file name */
    sprintf(EventLog,"%s/npreal2d.log", workpath);  /* Log file name */
#else
    sprintf(buf,"/etc/npreal2d/npreal2d.cf");        /* Config file name */
    sprintf(EventLog,"/etc/npreal2d/npreal2d.log");  /* Log file name */
#endif
    strcpy(Gcffile, buf);

	
    /*
     * Open configuration file:
     */
    ConfigFd = fopen(buf, "r");
    if ( ConfigFd == NULL )
    {
        log_event("Can't open configuration file (npreal2d.cf) !");
        return(-1);			/* Can't open file ! */
    }

    /*
        * old configuration file format.
     *"Device Id" "Server IP addr/Name" "data_port" "cmd_port" "Server Type"
     * ex:
     *  0	       192.168.1.1	950	966 2500
     *	1	       tty_server	951	967 303
     *	2	       192.168.1.1	950	966 311
        *
        *
     * Read configuration & the data format of every data line is :
     * [Minor] [ServerIP]	   [Data] [Cmd] [FIFO] [ttyName] [coutName]
     *  0      192.168.1.1     950    966   1      ttyr00    cur00
     *  1      192.168.1.1     951    967   1      ttyr01    cur01
     *  2      192.168.1.1     952    968   1      ttyr02    cur02
     *
     * Security data format
     * [Minor] [ServerIP]	             [Data] [Cmd] [FIFO] [SSL] [ttyName] [coutName] [interface]
     *  0      192.168.1.1               950    966   1      0     ttyr00    cur00
     *  1      192.168.1.1               951    967   1      0     ttyr01    cur01
     *  2      192.168.1.1               952    968   1      1     ttyr02    cur02
	 *  3      fe80::216:d4ff:fe80:63e6  950    966   1      0     ttyr03    cur03       eth0
     */
    ttys = 0;
    infop = ttys_info;
    while ( ttys < MAX_TTYS )
    {
        if ( fgets(buf, sizeof(buf), ConfigFd) == NULL )
            break;				/* end of file */
		memset(&infop->redund, 0, sizeof(struct redund_struct));
        server_type = disable_fifo = 0;
#ifdef SSL_ON
        ssl_enable = 0;
#endif
#if 1
        n = sscanf(buf, "%s%s%s%s%d%d%s%s%s%d%s",
                    ttyname,
                    infop->ip_addr_s,
                    tcpport,
                    cmdport,
                    &disable_fifo,
#ifdef SSL_ON
                    &ssl_enable,
#else
                    &temp,
#endif
                    ttyname2,
                    curname,
                    scope_id,
					&infop->redundant_mode,
					infop->redund.redund_ip);
	#if 0 // Jared, 2013-07-30, I just know these configures, so add n !=8 and n!9 in the condition check
		if(n != 10 && n != 11)
	#else
		if(n !=8 && n !=9 && n != 10 && n != 11)
	#endif
		{
#endif
#if 0
	        n = sscanf(buf, "%s%s%s%s%d%d%s%s%s%d%s",
                    ttyname,
                    infop->ip_addr_s,
                    tcpport,
                    cmdport,
                    &disable_fifo,
#ifdef SSL_ON
                    &ssl_enable,
#else
                    &temp,
#endif
                    ttyname2,
                    curname,
                    scope_id,
		    &infop->redundant_mode,
		    infop->redund.redund_ip);
//		printf("[AP] [ttyname = %s],[tcpport = %s],[ip1 = %s],[ip2 = %s]\n", ttyname, tcpport, 	
//								 infop->ip_addr_s,
//								 infop->redund.redund_ip);
		if(n != 10 && n != 11)
		{
#endif
            continue;
        }

#if 0
        n = sscanf(buf, "%s%s%s%s%d%s%s%s",
                    ttyname,
                    infop->ip_addr_s,
                    tcpport,
                    cmdport,
                    &disable_fifo,
                    ttyname2,
                    curname,
                    scope_id);
		if(n != 7 && n != 8)
		{
            continue;
        }
#endif
        if (ttyname[0]=='#')
            continue;
	
		Graw_mode = 1; 
		Gredund_mode = 1;

        /* in npreal2d.cf, [FIFO] is set to 1 if user is tend to */
        /* enable fifo, so the value of disable_fifo must be set to 0*/
        /* vice versa */
        if (disable_fifo == 1)
        {
            disable_fifo = 0;
        }
        else
        {
            disable_fifo = 1;
        }
//        server_type = CN2500;
        sprintf(infop->mpt_name,"/proc/npreal2/%s",ttyname);
		if(lib_name2ip6(infop) == -1)
		{
			log_event("ip address fail!!");
			break;
		}
		if(infop->af == AF_INET)
		{
			if ( *(u_long*)infop->ip6_addr == (uint32_t)0xFFFFFFFF )
			    continue;
		}
		
		if ( (data = atoi(tcpport)) <= 0 || data >= 10000 )
		    continue;
		if ( (cmd = atoi(cmdport)) <= 0 || cmd >= 10000 )
		    continue;

		if((strncmp(infop->ip_addr_s, "fe80", 4) == 0) || (strncmp(infop->ip_addr_s, "FE80", 4) == 0))
		{
			if(strlen(scope_id) == 0)
			{
			    break;
			}
			strcpy(infop->scope_id, scope_id);
		}
		else
		{
			memset(infop->scope_id, 0, 10);
		}
        infop->tcp_port = data;
        infop->cmd_port = cmd;
        infop->mpt_fd = -1;
        infop->sock_fd = -1;
        infop->sock_cmd_fd = -1;
        infop->state = STATE_INIT;
        infop->mpt_bufptr = (char *)malloc(BUFFER_SIZE * 2);
        if ( infop->mpt_bufptr == (char *)NULL )
        {
            log_event("Alocate memory fail !");
            break;
        }
        infop->sock_bufptr = infop->mpt_bufptr + BUFFER_SIZE;
	infop->alive_check_cnt = 0;
        infop->mpt_datakeep = 0;
        infop->mpt_dataofs = 0;
        infop->mpt_cmdkeep = 0;
        infop->sock_datakeep = 0;
        infop->sock_dataofs = 0;
        infop->sock_cmdkeep = 0;
        infop->error_flags = 0;
        strcpy(infop->ttyname, ttyname);
        strcpy(infop->ttyname2, ttyname2);
        strcpy(infop->curname, curname);
		infop->sameflag = 0;
        infop->server_type = server_type;
        infop->disable_fifo = disable_fifo;
        infop->tcp_wait_id = 0;
        if (!Restart_daemon)
            infop->tty_used_timestamp = 0;
        infop->first_servertime = 0;
#ifdef	SSL_ON
        infop->pssl = NULL;
        infop->ssl_enable = ssl_enable;
#endif
        infop++;
        ttys++;
    }

    /*
     * Close configuration file:
     */
    fclose(ConfigFd);
    if ( ttys == 0 )
        log_event("Have no any TTY configured record !");
    return(ttys);
}

/*
 *	Initialize a daemon process & detach a daemon process from login
 *	session context.
 */
moxattyd_daemon_start()
{
    register int	childpid, fd;

    /*
     * If we were started by init (process 1) from the /etc/inittab file
     * there's no need to detach.
     * This test is unreliable due to an unavoidable ambiguity if the
     * process is started by some other process and orphaned (i.e., if
     * the parent process terminates before we are started).
     */
    if ( getppid() == 1 )
        goto next;

    /*
     * Ignore the terminal stop signals.
     */
#ifdef	SIGTTOU
    signal(SIGTTOU, SIG_IGN);
#endif
#ifdef	SIGTTIN
    signal(SIGTTIN, SIG_IGN);
#endif
#ifdef	SIGTSTP
    signal(SIGTSTP, SIG_IGN);
#endif

    /*
     * If we were not started in the background, fork and let the parent
     * exit. This also guarantees the first child is not a process group
     * leader.
     */
    if ( (childpid = fork()) < 0 )
    {
        log_event("Can't fork first child !");
        exit(0);
    }
    else if ( childpid > 0 )
        exit(0);		/* parent process */

    /*
     * Disassociate from controlling terminal and process group.
     * Ensure the process can't reacquire a new controlling terminal.
     */
#ifdef	TIOCNOTTY

    if ( (fd = open("/dev/tty", O_RDWR)) >= 0 )
    {
        ioctl(fd, TIOCNOTTY, (char *)NULL);
        close(fd);
    }

#else

    if ( setpgrp() == -1 )
    {
        log_event("Can't change process group !");
        exit(0);
    }
    signal(SIGHUP, SIG_IGN);	/* immune from pgrp leader death */
    if ( (childpid = fork()) < 0 )
    {
        log_event("Can't fork second child !");
        exit(0);
    }
    else if ( childpid > 0 )
        exit(0);		/* parent process */

#endif

next:
    /*
     * Close any open files descriptors.
     */
#if 1
    close(0);
    close(1);
    close(2);
#endif
    errno = 0;

    /*
     * Move the current directory to root, to make sure we aren't on a
     * mounted filesystem.
     */
    chdir("/");

    /*
     * Clear any inherited file mode creation mask.
     */
    umask(0);
}

/*
 * Initialize the polling Server UDP socket & server IP table.
 */
poll_async_server_init()
{
    int			i, n, udp_port;
    struct sockaddr_in	sin;
	struct sockaddr_in6	sin6;
    struct sysinfo		sys_info;
	int af;
	
	int family[] = {AF_INET, AF_INET6};
	struct sockaddr * ptr;
	int len;

    servers = 0;
	af = ttys_info[0].af;
    for ( i=0; i<ttys; i++ )
    {
        for ( n=0; n<servers; n++ )
        {
            if ( *(u_long*)serv_info[n].ip6_addr == *(u_long*)ttys_info[i].ip6_addr )
                break;
			else if(memcmp(serv_info[n].ip6_addr, ttys_info[i].ip6_addr, 16) == 0)
				break;
        }
        if ( n == servers )
        {
            sysinfo(&sys_info);
            ttys_info[i].serv_index = servers;
			if(ttys_info[i].af == AF_INET)
	            *(u_long*)serv_info[servers].ip6_addr = *(u_long*)ttys_info[i].ip6_addr;
			else
				memcpy(serv_info[servers].ip6_addr, ttys_info[i].ip6_addr, 16);
			serv_info[servers].af = ttys_info[i].af;
            serv_info[servers].dev_type = 0;
            serv_info[servers].serial_no = 0;
            serv_info[servers].last_servertime = (time_t)((int32_t)(sys_info.uptime - 2));
            serv_info[servers].next_sendtime = (time_t)((int32_t)(sys_info.uptime - 1));
            serv_info[servers].ap_id = 0;
            serv_info[servers].hw_id = 0;
            serv_info[servers].dsci_ver= 0xFFFF;
            serv_info[servers].start_item= 0;
            servers++;
        }
        else
            ttys_info[i].serv_index = n; // Scott added: 2005-03-02
    }

	for(i=0; i<2; i++)
	{
		ptr = (i == IS_IPV4)? (struct sockaddr*)&sin : (struct sockaddr*)&sin6;
		len = (i == IS_IPV4)? sizeof(sin) : sizeof(sin6);
		
	    if ( (polling_nport_fd[i] = socket(family[i], SOCK_DGRAM, 0)) < 0 )
	    {
	        log_event("Can not open the polling_nport_fd socket !");
			if(i == IS_IPV6)
			{
				polling_nport_fd[1] = -1;
				enable_ipv6 = DIS_IPV6;
			    break;
			}
	        return(-1);
	    }
		if(i == IS_IPV4)
		{
		    sin.sin_family = AF_INET;
		    sin.sin_port = 0;
		    sin.sin_addr.s_addr = INADDR_ANY;
		}
		else
		{
			memset(&sin6, 0, sizeof(sin6));
			sin6.sin6_family = AF_INET6;
			sin6.sin6_port = 0;
		}
	    if (bind(polling_nport_fd[i], ptr, len) == 0)
	    {
#ifdef	FIONBIO
	        fcntl(polling_nport_fd[i], FIONBIO);
#endif
	    }
	    else
	    {
	    	for(n=0; n<=i; n++)
	    	{
		        close(polling_nport_fd[n]);
		        polling_nport_fd[n] = -1;
	    	}
	        log_event("Can not bind the polling NPort UDP port !");
	        return(-1);
	    }
	}
    if ( (polling_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
    {
        log_event("Can not open the polling UDP socket !");
        return(-1);
    }

    sin.sin_family = AF_INET;
    sin.sin_port = 0;
    sin.sin_addr.s_addr = INADDR_ANY;

    if (bind(polling_fd, (struct sockaddr*)&sin, sizeof(sin)) == 0)
    {
#ifdef	FIONBIO
        fcntl(polling_fd, FIONBIO);
#endif
    }
	else
	{
	    close(polling_fd);
	    polling_fd = -1; /* Add by Ying */
		for(i=0; i<2; i++)
		{
			close(polling_nport_fd[i]);
		    polling_nport_fd[i] = -1;
		}
	    log_event("Can not bind the polling UDP port !");
	    return(-1);
	}
	return 0;
}

poll_async_server_send(servp)
SERVINFO	*servp;
{
    struct sockaddr_in	to;
    int			len;
    unsigned char		msg[32];
    struct sysinfo		sys_info;
#ifdef BIG_ENDIAN_PLATFORM	//add by Jared 08-26-2005
	u_long tmp_time;
#endif
//printf("[AP] poll_async_server_send\n");
    if (servp->ap_id)
        return;
#ifndef	STREAM
    bzero(msg, 28);
#endif
#ifdef	STREAM
    memset (msg, 0, 28);
#endif
    sysinfo(&sys_info);
    if ( servp->dev_type == 0 )
    {
        servp->next_sendtime = (time_t)((int32_t)(sys_info.uptime + 5 ));
        msg[0] = 1;
        msg[3] = 6;
        len = 6;
    }
    else
    {
        servp->next_sendtime = (time_t)((int32_t)(sys_info.uptime + polling_time));
        msg[0] = 0x71;
        msg[3] = 30;
#ifdef BIG_ENDIAN_PLATFORM	//add by Jared 08-26-2005
        *(u_short *)&msg[22] = cpu_to_le16(servp->dev_type);
        *(u_short *)&msg[24] = cpu_to_le16(servp->serial_no);
        tmp_time= htonl(time(0));
        *(u_char *)&msg[26] = (u_char)((tmp_time&0xFF000000)>>24);
        *(u_char *)&msg[27] = (u_char)((tmp_time&0x00FF0000)>>16);
        *(u_char *)&msg[28] = (u_char)((tmp_time&0x0000FF00)>>8);
        *(u_char *)&msg[29] = (u_char)(tmp_time&0x000000FF);
#else
        *(u_short *)&msg[22] = servp->dev_type;
        *(u_short *)&msg[24] = servp->serial_no;
        *(uint32_t *)&msg[26] = htonl((uint32_t)sys_info.uptime);
#endif
        len = 30;
    }
    to.sin_family = AF_INET;
    to.sin_port = htons(0x405);
    to.sin_addr.s_addr = *(u_long*)servp->ip6_addr;
	sendto(polling_fd, msg, len, 0, (struct sockaddr *)&to, sizeof(to));
}

poll_async_server_recv()
{
    struct sockaddr_in	from;
    int			len, n, m, i, connected, listening;
    int32_t			t;
    SERVINFO *		servp;
    TTYINFO *		infop;
    unsigned char	msg[100];
    struct sysinfo		sys_info;
#ifdef BIG_ENDIAN_PLATFORM	//add by Jared 08-26-2005
	unsigned char *ptime;
#endif

//printf("[AP] poll_async_server_recv\n");
#ifdef	AIX
    if ( recvfrom(polling_fd, msg, 86, 0, (struct sockaddr *)&from, (socklen_t *)&len)
#else
#ifdef	SCO
    if ( recvfrom(polling_fd, msg, 86, 0, (struct sockaddr *)&from, &len)
#endif
#ifndef	SCO
            len = sizeof(from);
            if ( recvfrom(polling_fd, msg, 86, 0, (struct sockaddr *)&from, (socklen_t *)&len)
                        //if ( recvfrom(polling_fd, msg, 86, 0, (struct sockaddr *)&from, (size_t  *)&len)
#endif
#endif
            != 86 )
        return;
    if ( ((msg[0] != 0x81) && (msg[0] != 0xF1)) || (msg[3] != 86) )
        return;
    if ( msg[1] || msg[2] || msg[4] || msg[5] ||
        (from.sin_port != ntohs(0x405)) )
        return;
    for ( n=0, servp=serv_info; n<servers; n++, servp++ )
    {
        if ( from.sin_addr.s_addr == *(u_long*)servp->ip6_addr )
            break;
    }
    if ( n == servers )
        return;

    if ( msg[0] == 0x81 )
    {
        sysinfo(&sys_info);
        n = 0;
        if ( (msg[10]==0x08 && msg[11]==0x21) || (msg[10]==0x16 && msg[11]==0x21) )
        {
            if ( (msg[25] > 1) || (msg[24] > (unsigned char)0x25) )
            {
#ifdef BIG_ENDIAN_PLATFORM	//add by Jared 08-26-2005
                servp->dev_type = cpu_to_le16(*(u_short *)&msg[10]);
                servp->serial_no = cpu_to_le16(*(u_short *)&msg[12]);
#else
                servp->dev_type = *(u_short *)&msg[10];
                servp->serial_no = *(u_short *)&msg[12];
#endif			
                servp->last_servertime = (time_t)((int32_t)(sys_info.uptime - 1));
                n = 1;
            }
        }
        else
        {
#ifdef BIG_ENDIAN_PLATFORM	//add by Jared 08-26-2005
            servp->dev_type = cpu_to_le16(*(u_short *)&msg[10]);
            servp->serial_no = cpu_to_le16(*(u_short *)&msg[12]);
#else
            servp->dev_type = *(u_short *)&msg[10];
            servp->serial_no = *(u_short *)&msg[12];
#endif
            servp->last_servertime = (time_t)((int32_t)(sys_info.uptime - 1));
            n = 1;
        }
        if ( (servp->serial_no == 0) || (n == 1) )
        {
#ifdef BIG_ENDIAN_PLATFORM	//add by Jared 08-26-2005
            servp->serial_no = cpu_to_le16(*(u_short *)&msg[12]);
#else
            servp->serial_no = *(u_short *)&msg[12];
#endif
            *(uint32_t *)(&msg[96]) = *(u_long*)servp->ip6_addr;
            msg[93] = msg[23];
            msg[94] = msg[24];
            msg[95] = msg[25];
            if (msg[93]) /* x.x.[x] */
                sprintf((char *)msg,
                        "IP=%d.%d.%d.%d, Ver=%x.%x.%x[0x%02x%02x%02x] is alive.",
                        (int)(msg[96]), (int)(msg[97]), (int)(msg[98]),
                        (int)(msg[99]), (int)(msg[95]), (int)(msg[94]),
                        (int)(msg[93]), (int)(msg[95]), (int)(msg[94]),
                        (int)(msg[93]));
            else
                sprintf((char *)msg,
                        "IP=%d.%d.%d.%d, Ver=%x.%x(0x%02x%02x) is alive.",
                        (int)(msg[96]), (int)(msg[97]), (int)(msg[98]),
                        (int)(msg[99]), (int)(msg[95]), (int)(msg[94]),
                        (int)(msg[95]), (int)(msg[94]));
            /*
            			if (msg[94] < 0x10)
            				sprintf((char *)msg, "NPort(Async) Server (%d.%d.%d.%d) firmware version is %d.%02x .",
            				(int)(msg[96]), (int)(msg[97]), (int)(msg[98]),
            				(int)(msg[99]), (int)(msg[95]), (int)(msg[94]));
            			else
            				sprintf((char *)msg, "NPort(Async) Server (%d.%d.%d.%d) firmware version is %d.%2x .",
            				(int)(msg[96]), (int)(msg[97]), (int)(msg[98]),
            				(int)(msg[99]), (int)(msg[95]), (int)(msg[94]));
            */
            log_event(msg);
        }
        return;
    }
#ifdef BIG_ENDIAN_PLATFORM	//add by Jared 08-26-2005, the aligment problem
    ptime = (unsigned char*)&t;
    *ptime= msg[18];
    ptime++;
    *ptime= msg[19];
    ptime++;
    *ptime= msg[20];
    ptime++;
    *ptime= msg[21];
#else
    t = ntohl(*(int32_t *)&msg[18]);
#endif
    if (  t - servp->last_servertime  <= 0 )
        return;
#ifdef BIG_ENDIAN_PLATFORM	//add by Jared 08-26-2005
    if ( (servp->dev_type != cpu_to_le16(*(u_short *)&msg[10])) || \
        (servp->serial_no != cpu_to_le16(*(u_short *)&msg[12])) )
#else
    if ( (servp->dev_type != *(u_short *)&msg[10]) ||
            (servp->serial_no != *(u_short *)&msg[12]) )
#endif
    {
        servp->dev_type = 0;
        sysinfo(&sys_info);
        servp->next_sendtime = (time_t)((int32_t)(sys_info.uptime - 1 ));
        return;
    }
    m = 0;
    servp->last_servertime = t;
    for ( n=0, infop=ttys_info; n<ttys; n++, infop++ )
    {
        if ( *(u_long*)infop->ip6_addr != *(u_long*)servp->ip6_addr )
            continue;
        for (i=0, connected=0, listening=0; i<MAX_PORTS; i++)
        {
#ifdef BIG_ENDIAN_PLATFORM	//add by Jared 08-26-2005
            if ( infop->tcp_port != cpu_to_le16(*(u_short *)&msg[22+i*2]) )
#else
            if ( infop->tcp_port != ntohs(*(u_short *)&msg[22+i*2]) )
#endif
                continue;

            if ( msg[54+i] == TCP_CONNECTED )
            {
                connected = 1;
                break;
            }
            if ( msg[54+i] == TCP_LISTEN )
            {
                if ( infop->state == STATE_RW_DATA )
                    listening = 1;
                /* 1-30-02 by William
                		        else if ( infop->state == STATE_REMOTE_LISTEN ) {
                			        infop->state = STATE_RW_DATA;
                		        }
                */
            }
        }
        if ( !connected && listening == 1 )
        {
            m++;
            infop->state = STATE_REMOTE_LISTEN;
            sysinfo(&sys_info);
            infop->time_out = sys_info.uptime;
        }
    }
    if ( m )
    {
        *(uint32_t *)(&msg[96]) = *(u_long*)servp->ip6_addr;
        sprintf((char *)msg, "Ports reset of NPort(Async) Server %d.%d.%d.%d !",
                (int)(msg[96]), (int)(msg[97]), (int)(msg[98]),
                (int)(msg[99]));
        log_event(msg);
    }
}

poll_nport_send(servp)
SERVINFO	*servp;
{
 	union	sock_addr to;
    int			    len;
    unsigned char	msg[32];
    DSCI_HEADER     *dsci_headerp;
    DSCI_DA_DATA    *dscidata_p;
	EX_HEADER		*exheader;
    struct sysinfo		sys_info;
	int af = servp->af;

    if (servp->dev_type)
        return;
    if(af == AF_INET6 && enable_ipv6 == DIS_IPV6)
		return;
#ifndef	STREAM
    bzero(msg, 28);
#endif
#ifdef	STREAM
    memset (msg, 0, 28);
#endif
    sysinfo(&sys_info);
    if ( servp->ap_id == 0 )
    {   // send dsc_search
        servp->next_sendtime = (time_t)((int32_t)(sys_info.uptime + 5 ));
        servp->dsci_ver = 0xFFFF;
        dsci_headerp=(DSCI_HEADER*)&msg[0];
        dsci_headerp->opcode = 0x01; // dsc_search
        dsci_headerp->result = 0;
        dsci_headerp->length = htons(8);
        dsci_headerp->id = 0;
        len = 8;
		if(af == AF_INET6)
		{
			dsci_headerp->opcode = DSCI_IPV6;
			dsci_headerp->length = htons(28);
			exheader = (EX_HEADER*)&msg[20];
			exheader->ex_vision = EX_VERSION;
			memset(exheader->reservd, 0, 3);
			exheader->catalog = htons(KERNEL_FUN);
			exheader->subcode = htons(DSC_ENUMSEARCH);
			len = 28;
		}
    }
	else if (servp->dsci_ver == 0xFFFF)
	{	// send getkernelinfo
        servp->next_sendtime = (time_t)((int32_t)(sys_info.uptime + 5 ));
        dsci_headerp=(DSCI_HEADER*)&msg[0];
        dsci_headerp->opcode = 0x16; // getkernelinfo
        dsci_headerp->result = 0;
        dsci_headerp->length = htons(20);
        dsci_headerp->id = 0;

        dscidata_p=(DSCI_DA_DATA*)&msg[8];
        dscidata_p->ap_id = htonl(servp->ap_id);
        dscidata_p->hw_id = htons(servp->hw_id);
        memcpy((void*)dscidata_p->mac, (void*)servp->mac, 6);
        len = 20;
	}
    else if (servp->dsci_ver == 0)
    {        // send dsc_GetNetstat
        servp->next_sendtime = (time_t)((int32_t)(sys_info.uptime + polling_time ));
        dsci_headerp=(DSCI_HEADER*)&msg[0];
        dsci_headerp->opcode = 0x14; // dsc_GetNetstat
        dsci_headerp->result = 0;
        dsci_headerp->length = htons(22);
        dsci_headerp->id = htonl((uint32_t)sys_info.uptime);

        dscidata_p=(DSCI_DA_DATA*)&msg[8];
        dscidata_p->ap_id = htonl(servp->ap_id);
        dscidata_p->hw_id = htons(servp->hw_id);
        memcpy((void*)dscidata_p->mac, (void*)servp->mac, 6);
        msg[20] = 128;   // max number of sockets
        msg[21] = 0;   // max number of sockets
        len = 22;
    }
	else
	{	// send dsc_GetNetstat_ex
		int addr;
        servp->next_sendtime = (time_t)((int32_t)(sys_info.uptime + polling_time ));
        dsci_headerp=(DSCI_HEADER*)&msg[0];
        dsci_headerp->opcode = (af == AF_INET) ? 0x1D : DSCI_IPV6; // dsc_GetNetstat_ex : DSCI IPv6
        dsci_headerp->result = 0;
        dsci_headerp->length = (af == AF_INET) ? htons(24) : htons(32);
        dsci_headerp->id = htonl((uint32_t)sys_info.uptime);

        dscidata_p=(DSCI_DA_DATA*)&msg[8];
        dscidata_p->ap_id = htonl(servp->ap_id);
        dscidata_p->hw_id = htons(servp->hw_id);
        memcpy((void*)dscidata_p->mac, (void*)servp->mac, 6);
		if(af == AF_INET6)
		{
			exheader = (EX_HEADER*)&msg[20];
			exheader->ex_vision = EX_VERSION;
			memset(exheader->reservd, 0, 3);
			exheader->catalog = htons(NETWORK_CONFIG);
			exheader->subcode = htons(DSC_GETNETSTAT_V6);
		}
		addr = (af == AF_INET) ? 20 : 28;  //max socket address for ipv4 or ipv6

	    msg[addr] = 0x00;   // max number of sockets
	    msg[addr+1] = (af == AF_INET) ? 0xFF : MAX_SOCK_V6;   // max number of sockets
        
        if(servp->af == AF_INET)
        {
            msg[addr+2] = (unsigned char)servp->start_item;	// start item
            msg[addr+3] = 0;   					// start item
        }
        else
        { 
            msg[addr+3] = (unsigned char)servp->start_item;	// start item
            msg[addr+2] = 0;   					// start item
        }
       
        len = (af == AF_INET) ? 24 : 32;
	}
	memset(&to, 0, sizeof(to));
	if(af == AF_INET)
	{
	    to.sin.sin_family = AF_INET;
	    to.sin.sin_port = htons(4800);
	    to.sin.sin_addr.s_addr = *(u_long*)servp->ip6_addr;
	}
	else
	{
		to.sin6.sin6_family = AF_INET6;
		to.sin6.sin6_port = htons(4800);
		memcpy(to.sin6.sin6_addr.s6_addr, servp->ip6_addr, 16);
	}
    sendto(polling_nport_fd[((af == AF_INET) ? 0 : 1)], msg, len, 0, (struct sockaddr *)&to, sizeof(to));
}

poll_nport_recv(af_type)
int af_type;
{
	union sock_addr from;
    int             retlen, len, n, m, i, nstat, connected_tcp, connected_cmd, listening_tcp, listening_cmd;
    int32_t         t;
    SERVINFO *      servp;
    TTYINFO *       infop;
    char   msg[2100];
    DSCI_HEADER     *dsci_headerp;
    DSCI_RET_HEADER *dsci_retp;
    DSCI_NET_STAT   *desc_netstatp;
	DSCI_NET_STAT_IPV6 *desc_netstatp_ipv6;
    struct sysinfo  sys_info;
	u_short			next_item = 0;
	int             addr;

#ifdef	AIX
    if ( (retlen=recvfrom(polling_nport_fd[af_type], msg, sizeof(msg), 0, (struct sockaddr *)&from, (socklen_t *)&len))
#else
#ifdef	SCO
    if ( (retlen=recvfrom(polling_nport_fd[af_type], msg, sizeof(msg), 0, (struct sockaddr *)&from, &len))
#endif
#ifndef	SCO
            len = sizeof(from);
            if ( (retlen=recvfrom(polling_nport_fd[af_type], msg, sizeof(msg), 0, (struct sockaddr *)&from, (socklen_t *)&len))
#endif
#endif
            != 24 && ((retlen-24)%16) && retlen != 36 && ((retlen - 44)%16) && ((retlen-32)%40) )
        return;
    dsci_headerp = (DSCI_HEADER*)&msg[0]; 
    if ( (dsci_headerp->opcode == 0x81 && 
             ( (ntohs(dsci_headerp->length) != 24 ) && (ntohs(dsci_headerp->length) != 40) )) ||
		 (dsci_headerp->opcode == 0x94 && ((ntohs(dsci_headerp->length)-24)%16) != 0) ||
		 (dsci_headerp->opcode == 0x96 && ntohs(dsci_headerp->length) != 36) ||
		 (dsci_headerp->opcode == 0x9d && ((ntohs(dsci_headerp->length)-24)%16) != 0) ||
		 (dsci_headerp->opcode == DSCI_IPV6_RESPONS && 
		     ((((ntohs(dsci_headerp->length)-44)%16) != 0) &&  //dsci ipv6 enum search return 
		      (((ntohs(dsci_headerp->length)-32)%40) != 0))) ) //dsci ipv6 GetNetstat_V6 return
        return;
    if ( dsci_headerp->result!=0 || 
	    ( (from.sin.sin_port != ntohs(4800)) && (from.sin6.sin6_port != htons(4800)) ) )
        return;

    for ( n=0, servp=serv_info; n<servers; n++, servp++ )
    {
    	if(af_type == 0)
    	{
		    if ( from.sin.sin_addr.s_addr == *(u_long*)servp->ip6_addr )
		        break;
        }
        else
        {
        	if(memcmp(from.sin6.sin6_addr.s6_addr, servp->ip6_addr, 16) == 0)
        		break;
        }
    }
    if ( n == servers )
        return;

    sysinfo(&sys_info);
    dsci_retp=(DSCI_RET_HEADER*)&msg[8];
    if ( dsci_headerp->opcode == 0x81 || 
		((dsci_headerp->opcode == DSCI_IPV6_RESPONS) && ((htons(dsci_headerp->length)-44)%16 == 0)))
    {     // dsc_search respons
        char tmpbuf[128];
        servp->ap_id = ntohl(dsci_retp->ap_id);
        servp->hw_id = ntohs(dsci_retp->hw_id);
        memcpy((void*)servp->mac, (void*)dsci_retp->mac, 6);
        servp->last_servertime = (time_t)((int32_t)(sys_info.uptime - 1));

		inet_ntop(servp->af, servp->ip6_addr, (char *)&msg[96], 50);
        sprintf(tmpbuf, "%s is alive", &msg[96]);
		log_event(tmpbuf);
        return;
    }
    else if ( dsci_headerp->opcode == 0x96 )
    {     // getkernelinfo respons
        servp->last_servertime = (time_t)((int32_t)(sys_info.uptime - 1));
		servp->dsci_ver = *(u_short *)(&msg[34]);
        return;
    }

	if (dsci_headerp->opcode == 0x9D 
		|| (dsci_headerp->opcode == DSCI_IPV6_RESPONS && ((ntohs(dsci_headerp->length)-32)%40) == 0) )
	{
        if(servp->af == AF_INET)
        {
	        next_item = (int)msg[23];	/* for big&little endian machine */
	        next_item = (next_item << 8) | ((int)msg[22] & 0xff);
        }
        else
        {
            next_item = msg[30];
            next_item = (next_item << 8) | ((int)msg[31] & 0xff);
        }

		if (next_item)
			servp->start_item = next_item;
		else
			servp->start_item = 0;
	}
	
    t = ntohl(dsci_headerp->id);
    if (  t - servp->last_servertime  <= 0 )
        return;
    if ( (servp->ap_id != ntohl(dsci_retp->ap_id)) ||
            (servp->hw_id != ntohs(dsci_retp->hw_id)) )
    {
        servp->ap_id = 0;
        sysinfo(&sys_info);
        servp->next_sendtime = (time_t)((int32_t)(sys_info.uptime - 1 ));
        return;
    }
    m = 0;
    servp->last_servertime = t;
	addr = (servp->af == AF_INET)? 21 : 29;
    nstat = (int)msg[addr];	/* for big&little endian machine */
    nstat = (nstat << 8) | ((int)msg[addr-1] & 0xff);
	addr = (servp->af == AF_INET) ? 128 : 35;
    if(nstat > addr){            /* the value can not over 128 */
        nstat = addr;            /*for ipv6, the value can not over 35*/
    }
    for ( n=0, infop=ttys_info; n<ttys; n++, infop++ )
    {
    	u_short local_port, remote_port;
		unsigned char status;
    	if(servp->af == AF_INET)
    	{
	        if ( *(u_long*)infop->ip6_addr != *(u_long*)servp->ip6_addr )
	            continue;
    	}
		else if(servp->af == AF_INET6)
		{
			if(memcmp(infop->ip6_addr, servp->ip6_addr, 16) != 0)
				continue;
		}

        for (i=0, connected_tcp = connected_cmd = listening_tcp = listening_cmd = 0; i<nstat; i++)
        {
        	if(servp->af == AF_INET)
    		{
	            desc_netstatp=(DSCI_NET_STAT*)&msg[24+i*16];
    			local_port = desc_netstatp->local_port;
				remote_port = desc_netstatp->remote_port;
				status = desc_netstatp->status;
    		}
			else if(servp->af == AF_INET6)
			{
				unsigned char *buf;
	            desc_netstatp_ipv6 = (DSCI_NET_STAT_IPV6*)&msg[32+i*40];
				buf = (unsigned char *)&desc_netstatp_ipv6->local_port;
				local_port = buf[0]*0x100 + buf[1];
				buf = (unsigned char *)&desc_netstatp_ipv6->remote_ip;
				remote_port = buf[0]*0x100 + buf[1];
				status = desc_netstatp_ipv6->status;
			}

            // Scott: 2005-09-19
            // If either the command port or data port is back to listen,
            //  info state of the port must be set the STATE_REMOTE_LISTEN,
            //  so that the port can be re-opened by the user application.
            // if ( infop->tcp_port != desc_netstatp->local_port )
            if ( !(infop->local_tcp_port && infop->tcp_port == local_port) &&
                    !(infop->local_cmd_port && infop->cmd_port == local_port))
                continue;

#if 0
            if (infop->local_tcp_port && infop->tcp_port == desc_netstatp->local_port)
                printf("hit data port (%d, %d)\n", infop->tcp_port, infop->local_tcp_port);
            else if (infop->local_cmd_port && infop->cmd_port == desc_netstatp->local_port)
                printf("hit command port (%d, %d)\n", infop->cmd_port, infop->local_cmd_port);
#endif

            if (infop->tcp_port == local_port && status == TCP_LISTEN && infop->state == STATE_RW_DATA)
                listening_tcp = 1;
            else if (infop->cmd_port == local_port && status == TCP_LISTEN && infop->state == STATE_RW_DATA)
                listening_cmd = 1;
            else if (infop->local_tcp_port == remote_port && status == TCP_CONNECTED)
                connected_tcp = 1;
            else if (infop->local_cmd_port == remote_port && status == TCP_CONNECTED)
                connected_cmd = 1;
        }

        if ( (listening_tcp == 1 || listening_cmd == 1) && (!connected_tcp || !connected_cmd))
        {
			if (servp->dsci_ver == 0 || (servp->dsci_ver != 0 && next_item == 0))
			{
	            m++;	
				infop->alive_check_cnt++;
				if (infop->alive_check_cnt > 1) {
	            	infop->state = STATE_REMOTE_LISTEN;
					infop->alive_check_cnt = 0;
				}
			}
            sysinfo(&sys_info);
            infop->time_out = sys_info.uptime;
        } else {
			infop->alive_check_cnt = 0;
		}
    }
    if ( m )
    {
    	char ip_buf[50];
		int size;
		size = sizeof(ip_buf);
		sprintf(msg, "Ports reset of NPort(Async) Server %s !", 
		inet_ntop(servp->af, servp->ip6_addr, ip_buf, size));
		log_event(msg);
    }
}

/*
 *	The major function of Moxa pseudo TTYs daemon:
 *	maintain the TCP/IP connection to Async-Server and exchange those
 *	data to/from TCP sockets and master pseudo tty file descriptors.
 */
int moxattyd_change_config() {
    int			data, cmd;
    int         i, j, n, chttys;
    FILE *      ConfigFd;
    TTYINFO     *infop, *orip;
    char        buf[160], showbuf[80];
	char		ttyname[160], tcpport[16], cmdport[16];
	char		ttyname2[160], curname[160], scope_id[10];
    TTYINFO     chttys_info[MAX_TTYS];
	int32_t		server_type, disable_fifo;
	int32_t		temp;
	char		tmp_cmd[1024];
#ifdef SSL_ON
	int32_t		ssl_enable;
#endif
	printf("start moxaddyd_change_config ttys = %d\n", ttys);
	printf("Gcffile = %s\n", Gcffile);
    ConfigFd = fopen(Gcffile, "r");
    if ( ConfigFd == NULL ) {
        sprintf(showbuf,"Can't open configuration file (moxattyd.cf) !");
        log_event(showbuf);
		printf("change_config : open fial\n");
        return(-1);
    }

    chttys = 0;
    infop = chttys_info;
    while ( chttys < MAX_TTYS ) {
        if ( fgets(buf, sizeof(buf), ConfigFd) == NULL )
            break;
		//memset(&infop->redund, 0, sizeof(struct redund_struct));
		server_type = disable_fifo = 0;
#ifdef SSL_ON
	ssl_enable = 0;
#endif
		n = sscanf(buf, "%s%s%s%s%d%d%s%s%s",
                    ttyname,
                    infop->ip_addr_s,
                    tcpport,
                    cmdport,
                    &disable_fifo,
#ifdef SSL_ON
                    &ssl_enable,
#else
                    &temp,
#endif
                    ttyname2,
                    curname,
                    scope_id,
					&infop->redundant_mode);
		printf("\n");
        if(n != 9 && n != 10)
        {
            continue;
        }

		if (ttyname[0] == '#') {
			continue;
		}
		if (disable_fifo == 1) {
			disable_fifo = 0;
		} else {
			disable_fifo = 1;
		}
        if(lib_name2ip6(infop) == -1)
        {
            log_event("ip address fail!!");
            break;
        }
        if(infop->af == AF_INET)
        {
            if ( *(u_long*)infop->ip6_addr == (uint32_t)0xFFFFFFFF )
                continue;
        }
        if ( (data = atoi(tcpport)) <= 0 || data >= 10000 )
            continue;
        if ( (cmd = atoi(cmdport)) <= 0 || cmd >= 10000 )
            continue;

        if((strncmp(infop->ip_addr_s, "fe80", 4) == 0) || (strncmp(infop->ip_addr_s, "FE80", 4) == 0))
        {
            if(strlen(scope_id) == 0)
            {
                break;
            }
            strcpy(infop->scope_id, scope_id);
        }
        else
        {
            memset(infop->scope_id, 0, 10);
        }
		printf("\n");
		sprintf(infop->mpt_name, "/proc/npreal2/%s", ttyname);

		infop->tcp_port = data;	
		infop->cmd_port = cmd;
		infop->mpt_fd = -1;
		infop->sock_fd = -1;
		infop->sock_cmd_fd = -1;
		infop->state = STATE_INIT;
		infop->mpt_bufptr = (char *)malloc (BUFFER_SIZE * 2);
		if (infop->mpt_bufptr == (char *)NULL) {
			log_event("Alocate memory fail !");
			
			break;
		}
 		infop->sock_bufptr = infop->mpt_bufptr + BUFFER_SIZE;
        infop->mpt_datakeep = 0;
        infop->mpt_dataofs = 0;
        infop->mpt_cmdkeep = 0;
        infop->sock_datakeep = 0;
        infop->sock_dataofs = 0;
        infop->sock_cmdkeep = 0;
        infop->error_flags = 0;
        infop->server_type = server_type;
        infop->disable_fifo = disable_fifo;
        infop->tcp_wait_id = 0;
        strcpy(infop->ttyname, ttyname);
        strcpy(infop->ttyname2, ttyname2);
        strcpy(infop->curname, curname);
		printf("ip = %s\n", infop->ip_addr_s);
		printf("name = %s\n", infop->ttyname);
		printf("name2 = %s\n", infop->ttyname2);
		printf("\n");
        if (!Restart_daemon)
            infop->tty_used_timestamp = 0;
        infop->first_servertime = 0;
#ifdef  SSL_ON
        infop->pssl = NULL;
        infop->ssl_enable = ssl_enable;
#endif
        infop++;
        chttys++;
    }

    fclose(ConfigFd);

	orip = ttys_info;
	for (i = 0; i < ttys; i++, orip++) {
		infop = chttys_info;
		for (j = 0; j < chttys; j++, infop++) {
			if (strcmp(orip->mpt_name, infop->mpt_name) == 0) {
				if (strcmp(orip->ip_addr_s, infop->ip_addr_s) == 0) {
					if (orip->tcp_port == infop->tcp_port) {
						memcpy(&chttys_info[j], &ttys_info[i], sizeof(TTYINFO));
						orip->sameflag = 1; /* new_info == old_info */
						break;
					} 
				} 
			}
		}
	}
	orip = ttys_info;
#if 1
	for (i = 0; i < ttys; i++, orip++) {
		if (orip->sameflag != 1) { /* new_info != old_info */
			close(orip->mpt_fd);
			close(orip->sock_fd);
			close(orip->sock_cmd_fd);
		}
	}
#endif
printf("start\n");
printf("start\n");
	for (i = 0; i < chttys; i++) {
		memcpy(&ttys_info[i], &chttys_info[i], sizeof(TTYINFO));
		printf("ip = %s\n", &chttys_info[i].ip_addr_s);
		printf("name = %s\n", &chttys_info[i].ttyname);
		printf("name2 = %s\n", &chttys_info[i].ttyname2);
	}
printf("end\n");
printf("end\n");
	for (i = ttys; i < chttys; i++) {
#if 1
		sprintf(tmp_cmd, "rm -rf /dev/%s", &ttys_info[i].ttyname2);
		system(tmp_cmd);
		sprintf(tmp_cmd, "/usr/lib/npreal2/driver/mxmknod %s 33 %s", 
						&ttys_info[i].ttyname2, &ttys_info[i].ttyname);
//		sprintf(tmp_cmd, "mknod -Z system_u:object_r:tty_device_t:s0 -m 666 /dev/%s c 33 %s", 
//						 &ttys_info[i].ttyname2, &ttys_info[i].ttyname);
		system(tmp_cmd);
		sprintf(tmp_cmd, "rm -rf /dev/%s", &ttys_info[i].curname);
		system(tmp_cmd);
		sprintf(tmp_cmd, "/usr/lib/npreal2/driver/mxmknod %s 38 %s", 
						&ttys_info[i].curname, &ttys_info[i].ttyname);
//		sprintf(tmp_cmd, "mknod -Z system_u:object_r:tty_device_t:s0 -m 666 /dev/%s c 33 %s", 
//						 &ttys_info[i].curname, &ttys_info[i].ttyname);
		system(tmp_cmd);
#endif
	}

    ttys = chttys;
    return(chttys);
}
moxattyd_handle_ttys()
{
    int		i, n, m, maxfd, t0, sndx,len,len1,j;
    TTYINFO *	infop;
    SERVINFO *	servp;
    fd_set		rfd, wfd, efd;
    struct timeval	tm;
    char		cmd_buf[CMD_BUFFER_SIZE], buf[100];
    ConnMsg 	msg;
    int		tcp_wait_count;
    struct sysinfo	sys_info;

    signal(SIGPIPE, SIG_IGN);	/* add for "broken pipe" error */
    while ( 1 )
    {
//printf("---------------------------------\n");
        if (Restart_daemon)
        {
            for ( i=0, infop=ttys_info; i<ttys; i+=1, infop+=1 )
            {
                if (infop->redundant_mode)
                    continue;
                if (infop->tty_used_timestamp)
                {
                    ioctl(infop->mpt_fd,
                          _IOC(_IOC_READ|_IOC_WRITE,'m',CMD_DISCONNECTED,0),
                          0);
                }
				infop->reconn_flag = 1;
            }
            return;
        }
        if (Gconfig_changed > 0) {
            Gconfig_changed = 0;
        }

        tm.tv_sec = 3;
        tm.tv_usec = 0;
        FD_ZERO(&rfd);
        FD_ZERO(&wfd);
        FD_ZERO(&efd);
        maxfd = -1;
        sndx = -1;
        tcp_wait_count = 0;
        for ( i=0, infop=ttys_info; i<ttys; i+=1, infop+=1 )
        {
		    if (infop->redundant_mode)
            	continue;
            if ( infop->state == STATE_INIT ||
                    infop->state == STATE_MPT_OPEN ||
                    infop->state == STATE_MPT_REOPEN )
            {
                OpenTty(infop);
            }

            if ( infop->state == STATE_CONN_FAIL )
            {
                sysinfo(&sys_info);
                if ( (sys_info.uptime - infop->time_out) >= 1 )
                    infop->state = STATE_TCP_OPEN;
            }

            if ( infop->state == STATE_TCP_OPEN )
                OpenTcpSocket(infop);

            if ( infop->state == STATE_TCP_CONN )
                ConnectTcp(infop);

#ifdef SSL_ON
            if ( infop->ssl_enable )
            {
                if ( infop->state == STATE_SSL_CONN )
                    ConnectSSL(infop);
            }
#endif

            if ( infop->state == STATE_TCP_CLOSE )
                CloseTcp(infop);

            if ( infop->state == STATE_TCP_WAIT )
            {
                ConnectCheck();
                if ( infop->state == STATE_TCP_WAIT )
                    tcp_wait_count++;
            }

            if ( infop->state < STATE_TTY_WAIT )
            {
                tm.tv_sec = 1;
            }
            else if ( infop->state == STATE_REMOTE_LISTEN)
            {
                CloseTcp(infop);
                continue;
            }

            if (infop->mpt_fd >= 0)
                FD_SET(infop->mpt_fd, &efd);
            if ( infop->mpt_fd > maxfd )
                maxfd = infop->mpt_fd;

            servp = &serv_info[infop->serv_index];
            if ( (infop->state >= STATE_RW_DATA)&&polling_time )
            {
                if (!infop->first_servertime)
                {
                    sysinfo(&sys_info);
                    infop->first_servertime = sys_info.uptime - 1;
                    servp->last_servertime = (time_t)((int32_t)(sys_info.uptime - 1));
		//printf("[AP3] last_servertime = %d, sys_info.uptime = %d\n", servp->last_servertime, sys_info.uptime);
                }

                if ( sndx < 0 )
                {
                    sysinfo(&sys_info);
                    if ( ((time_t)((int32_t)sys_info.uptime) - servp->next_sendtime) > 0 )
                    {
                        sndx = infop->serv_index;
                        FD_SET(polling_fd, &wfd);
                    }
//		printf("[AP3] last_servertime = %d, sys_info.uptime = %d\n", servp->last_servertime, sys_info.uptime);
#if 1
                    if (((time_t)((int32_t)sys_info.uptime)-servp->last_servertime)>timeout_time)
                    {
                        infop->first_servertime = 0;
                        infop->state = STATE_REMOTE_LISTEN;
                        infop->time_out = sys_info.uptime;
						servp->start_item = 0;
                    }
#endif
                }
    		    
                FD_SET(polling_fd, &rfd);
                if ( polling_fd > maxfd )
                    maxfd = polling_fd;
    			for(n=0; n<enable_ipv6; n++)
    		    {	
                    FD_SET(polling_nport_fd[n], &rfd);
                    if ( polling_nport_fd[n] > maxfd )
                        maxfd = polling_nport_fd[n];
                }
            }

            if ( infop->state >= STATE_RW_DATA )
            {
                if ( infop->mpt_fd > maxfd )
                    maxfd = infop->mpt_fd;
                if ( infop->sock_fd > maxfd )
                    maxfd = infop->sock_fd;
                if ( infop->sock_cmd_fd > maxfd )
                    maxfd = infop->sock_cmd_fd;

                if ( infop->mpt_datakeep )
                {
                    FD_SET(infop->sock_fd, &wfd);
                }
                else
                {
                    FD_SET(infop->mpt_fd, &rfd);
                }

                if ( infop->sock_datakeep )
                {
                    FD_SET(infop->mpt_fd, &wfd);
                }
                else
                {
                    FD_SET(infop->sock_fd, &rfd);
                }

                FD_SET(infop->sock_cmd_fd, &rfd);
            }
        }
        if (tcp_wait_count)
        {
            tm.tv_sec = 0;
            tm.tv_usec = 20000;
        }

        if ((j= select(maxfd+1, &rfd, &wfd, &efd, &tm)) <= 0 )
        {
            continue;
        }

        for ( i=0, infop=ttys_info; i<ttys; i+=1, infop+=1 )
        {
			if (infop->redundant_mode)
				continue;
            if (Gconfig_changed > 0)
                break;
            if ( infop->mpt_fd < 0)
                continue;
            if ( (infop->mpt_fd)&&FD_ISSET(infop->mpt_fd, &efd) )
            { //cmd ready
                if ((n=ioctl(infop->mpt_fd,
                             _IOC(_IOC_READ|_IOC_WRITE,'m',CMD_RETRIEVE,CMD_BUFFER_SIZE),
                             infop->mpt_cmdbuf)) > 0)
                {
                    if (infop->mpt_cmdbuf[0] == NPREAL_ASPP_COMMAND_SET)
                    {
                        write (infop->sock_cmd_fd,
                               infop->mpt_cmdbuf+1,n-1);
                    }
                    else if (infop->mpt_cmdbuf[0] == NPREAL_LOCAL_COMMAND_SET)
                    {
                        switch (infop->mpt_cmdbuf[1])
                        {
                        case LOCAL_CMD_TTY_USED:
                            if (infop->state != STATE_TTY_WAIT)
                            {
#ifdef SSL_ON
                                if (infop->ssl_enable)
                                {
                                    SSL_shutdown(infop->pssl);
                                    SSL_free(infop->pssl);
                                    infop->pssl = NULL;
                                }
#endif
                                shutdown(infop->sock_fd, 2);
                                shutdown(infop->sock_cmd_fd, 2);
                                close(infop->sock_fd);
                                close(infop->sock_cmd_fd);
                                infop->sock_fd = -1;
                                infop->sock_cmd_fd = -1;
                                infop->local_tcp_port = 0;
                                infop->local_cmd_port = 0;
                                sprintf(cmd_buf, "Repeat connection!, %d, %s\n", infop->tcp_port, infop->ip_addr_s);
                                log_event(cmd_buf);
                                sleep(1);
                            }
                            infop->state = STATE_TCP_OPEN;
                            sysinfo(&sys_info);
                            infop->tty_used_timestamp = sys_info.uptime;
                            continue;
                        case LOCAL_CMD_TTY_UNUSED:
#ifdef SSL_ON
                            if (infop->ssl_enable)
                            {
                                SSL_shutdown(infop->pssl);
                                SSL_free(infop->pssl);
                                infop->pssl = NULL;
                            }
#endif
                            shutdown(infop->sock_fd, 2);
                            shutdown(infop->sock_cmd_fd, 2);
                            close(infop->sock_fd);
                            close(infop->sock_cmd_fd);
                            infop->sock_fd = -1;
                            infop->sock_cmd_fd = -1;
                            infop->local_tcp_port = 0;
                            infop->local_cmd_port = 0;
                            cmd_buf[0] = NPREAL_LOCAL_COMMAND_SET;
                            cmd_buf[1] = LOCAL_CMD_TTY_UNUSED;
                            ioctl(infop->mpt_fd,
                                  _IOC(_IOC_READ|_IOC_WRITE,'m',CMD_RESPONSE,2),
                                  cmd_buf);
                            infop->sock_datakeep = 0;
                            infop->sock_dataofs = 0;
                            infop->mpt_datakeep = 0;
                            infop->mpt_dataofs = 0;
                            if ((infop->state < STATE_RW_DATA) && !(infop->error_flags & ERROR_TCP_CONN))
                            {
                                sprintf(cmd_buf, "Socket connect fail (%s,TCP port %d) !",
                                        infop->ip_addr_s,
                                        infop->tcp_port);
                                log_event(cmd_buf);
                            }
                            infop->state = STATE_TTY_WAIT;
                            infop->tty_used_timestamp = 0;
                            infop->first_servertime = 0;
                            continue;
                        }
                    }
                }
            }
            if ( infop->state < STATE_RW_DATA )
                continue;

            if ( FD_ISSET(infop->sock_cmd_fd, &rfd) )
            { //cmd resp
                if ((len = read(infop->sock_cmd_fd,
                                infop->sock_cmdbuf,
                                CMD_BUFFER_SIZE)) <= 0)
                {
#ifdef SSL_ON
                    if (infop->ssl_enable)
                    {
                        SSL_shutdown(infop->pssl);
                        SSL_free(infop->pssl);
                        infop->pssl = NULL;
                    }
#endif
                    close(infop->sock_fd);
                    close(infop->sock_cmd_fd);
                    infop->sock_fd = -1;
                    infop->sock_cmd_fd = -1;
                    infop->local_tcp_port = 0;
                    infop->local_cmd_port = 0;
                    infop->state = STATE_TCP_OPEN;
                    ioctl(infop->mpt_fd,
                          _IOC(_IOC_READ|_IOC_WRITE,'m',CMD_DISCONNECTED,0),
                          0);
                    continue;
                }
                n = 0;
                while (len > 0)
                {
                    if (infop->sock_cmdbuf[n]
                            == ASPP_CMD_POLLING)
                    {
                        if (len < 3)
                        {
                            len = 0;
                            continue;
                        }
                        cmd_buf[0] = ASPP_CMD_ALIVE;
                        cmd_buf[1] = 1;
                        cmd_buf[2] = infop->sock_cmdbuf[n+2];
                        len1 = 3;
                        write(infop->sock_cmd_fd,cmd_buf,len1);
                        n += len1;
                        len -= len1;
                        continue;
                    }
                    switch (infop->sock_cmdbuf[n])
                    {
                    case ASPP_CMD_NOTIFY :
                    case ASPP_CMD_WAIT_OQUEUE :
                    case ASPP_CMD_OQUEUE :
                    case ASPP_CMD_IQUEUE :
                        len1 = 4;
                        break;
                    case ASPP_CMD_LSTATUS :
                    case ASPP_CMD_PORT_INIT :
                        len1 = 5;
                        break;
                    case ASPP_CMD_FLOWCTRL:
                    case ASPP_CMD_IOCTL:
                    case ASPP_CMD_SETBAUD:
                    case ASPP_CMD_LINECTRL:
                    case ASPP_CMD_START_BREAK:
                    case ASPP_CMD_STOP_BREAK:
                    case ASPP_CMD_START_NOTIFY:
                    case ASPP_CMD_STOP_NOTIFY:
                    case ASPP_CMD_FLUSH:
                    case ASPP_CMD_HOST:
                    case ASPP_CMD_TX_FIFO:
                    case ASPP_CMD_XONXOFF:
                    case ASPP_CMD_SETXON:
                    case ASPP_CMD_SETXOFF:
                        len1 = 3;
                        break;
                    default :
                        len1 = len;
                        break;
                    }

                    if ((len1 > 0)&&((n+len1) < CMD_BUFFER_SIZE))
                    {
                        cmd_buf[0] = NPREAL_ASPP_COMMAND_SET;
                        memcpy(&cmd_buf[1],&infop->sock_cmdbuf[n],len1);
                        ioctl(infop->mpt_fd,
                              _IOC(_IOC_READ|_IOC_WRITE,'m',CMD_RESPONSE,len1+1),
                              cmd_buf);
                    }
                    n += len1;
                    len -= len1;
                }

            }

            if ( FD_ISSET(infop->mpt_fd, &rfd) )
            {
                m = infop->mpt_datakeep + infop->mpt_dataofs;
                n = read(infop->mpt_fd,
                         infop->mpt_bufptr + m,
                         BUFFER_SIZE - m);
                if ( n > 0 )
                    infop->mpt_datakeep += n;
            }
            if ( FD_ISSET(infop->sock_fd, &rfd) )
            {
                m = infop->sock_datakeep + infop->sock_dataofs;
#ifdef	SSL_ON
                if (infop->ssl_enable)
                {
                    n = SSL_read(infop->pssl,
                                 infop->sock_bufptr + m,
                                 BUFFER_SIZE - m);
                }
                else
                {
                    n = read(infop->sock_fd,
                             infop->sock_bufptr + m,
                             BUFFER_SIZE - m);
                }
#else
                n = read(infop->sock_fd,
                         infop->sock_bufptr + m,
                         BUFFER_SIZE - m);
#endif
                if ( n > 0 )
                {
                    infop->sock_datakeep += n;
                    infop->state = STATE_RW_DATA;
                    sysinfo(&sys_info);
                    servp = &serv_info[infop->serv_index];
                    servp->last_servertime = (time_t)((int32_t)(sys_info.uptime - 1));
		//printf("[AP4] last_servertime = %d, sys_info.uptime = %d\n", servp->last_servertime, sys_info.uptime);
                }
                else if (n <= 0)
                {
#ifdef SSL_ON
                    if (infop->ssl_enable)
                    {
                        SSL_shutdown(infop->pssl);
                        SSL_free(infop->pssl);
                        infop->pssl = NULL;
                    }
#endif
                    close(infop->sock_fd);
                    close(infop->sock_cmd_fd);
                    infop->sock_fd = -1;
                    infop->sock_cmd_fd = -1;
                    infop->local_tcp_port = 0;
                    infop->local_cmd_port = 0;
                    infop->state = STATE_TCP_OPEN;
                    ioctl(infop->mpt_fd,
                          _IOC(_IOC_READ|_IOC_WRITE,'m',CMD_DISCONNECTED,0),
                          0);
                    continue;
                }
            }
            if ( FD_ISSET(infop->mpt_fd, &wfd) )
            {
                n = write(infop->mpt_fd,
                          infop->sock_bufptr+infop->sock_dataofs,
                          infop->sock_datakeep);
                if ( n > 0 )
                {
                    infop->sock_datakeep -= n;
                    if ( infop->sock_datakeep )
                        infop->sock_dataofs += n;
                    else
                        infop->sock_dataofs = 0;
                }
            }
            if ( FD_ISSET(infop->sock_fd, &wfd) )
            {
#ifdef	SSL_ON
                if (infop->ssl_enable)
                {
                    n = SSL_write(infop->pssl,
                                  infop->mpt_bufptr+infop->mpt_dataofs,
                                  infop->mpt_datakeep);
                }
                else
                {
                    n = write(infop->sock_fd,
                              infop->mpt_bufptr+infop->mpt_dataofs,
                              infop->mpt_datakeep);
                }
#else
                n = write(infop->sock_fd,
                          infop->mpt_bufptr+infop->mpt_dataofs,
                          infop->mpt_datakeep);
#endif
                if ( n > 0 )
                {
                    sysinfo(&sys_info);
                    servp = &serv_info[infop->serv_index];
                    servp->last_servertime = (time_t)((int32_t)(sys_info.uptime - 1));
		//printf("[AP5] last_servertime = %d, sys_info.uptime = %d\n", servp->last_servertime, sys_info.uptime);
                    infop->mpt_datakeep -= n;
                    if ( infop->mpt_datakeep )
                        infop->mpt_dataofs += n;
                    else
                        infop->mpt_dataofs = 0;
                }
                else if (n < 0)
                {
                    log_event("Can not write data");
                }
            }
        }

        if ( polling_time == 0 )
            continue;
#if 1
        
        if ( (sndx >= 0) && FD_ISSET(polling_fd, &wfd))
        {
            poll_async_server_send(&serv_info[sndx]);
            poll_nport_send(&serv_info[sndx]);
        } 
		if ( FD_ISSET(polling_fd, &rfd) )
        {
            poll_async_server_recv();
        }
		for(n=0; n<enable_ipv6; n++)
        {
           
            if ( FD_ISSET(polling_nport_fd[n], &rfd) )
            {
                poll_nport_recv(n);
            }
        }
#endif
    } 
}

OpenTty(infop)
TTYINFO *	infop;
{
    int	flag;
    char	buf[80];

    flag = 0;
    if ( infop->mpt_fd >= 0 )
    {
        /*	close(infop->mpt_fd);  05-02-2001 Jming */
        flag = 1;
    }
    else
    {
        flag = 0;
        infop->mpt_fd = open(infop->mpt_name, O_RDWR);
    }

    if ( infop->mpt_fd >= 0 )
    {
        struct	net_node_setting nd_settings;
        int	tty_status = 0;
        infop->reconn_flag = 1;
        nd_settings.server_type = infop->server_type;
        nd_settings.disable_fifo = infop->disable_fifo;
        ioctl(infop->mpt_fd,
              _IOC(_IOC_READ|_IOC_WRITE,'m',CMD_NET_SETTING,
                   sizeof(struct net_node_setting)),
              &nd_settings);
        ioctl(infop->mpt_fd,
              _IOC(_IOC_READ|_IOC_WRITE,'m',CMD_GET_TTY_STATUS,
                   sizeof(int)),&tty_status);
        if (infop->tty_used_timestamp == 0)
        {
            if (!tty_status)
            {
                infop->state = STATE_TTY_WAIT;
            }
            else
            {
                ioctl(infop->mpt_fd,
                      _IOC(_IOC_READ|_IOC_WRITE,'m',CMD_DISCONNECTED,0),
                      0);
                infop->state = STATE_TCP_OPEN;

            }
        }
        else
            infop->state = STATE_TCP_OPEN;

    }
    else if ( flag == 0 )
    {
        if ( !(infop->error_flags & ERROR_MPT_OPEN) )
        {
            sprintf(buf, "Master tty open fail (%s) !",
                    infop->mpt_name);
            log_event(buf);
            infop->error_flags |= ERROR_MPT_OPEN;
        }
    }
}
#define SOCK_BUF 1048
OpenTcpSocket(infop)
TTYINFO *	infop;
{
    char	buf[80];
    int	on=1;
    int af;
    af = infop->af;

    infop->sock_fd = socket(af, SOCK_STREAM, 0);
    if ( infop->sock_fd >= 0 )
    {
        if ( setsockopt(infop->sock_fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof(on)) < 0 )
            log_event("Set TCP keep alive fail !");
		if(strlen(infop->scope_id) > 0)
		{
			if ( setsockopt(infop->sock_fd, SOL_SOCKET, SO_BINDTODEVICE, infop->scope_id, strlen(infop->scope_id)) < 0)
			    log_event("Set TCP bind to device fail !");		
		}
        infop->state = STATE_TCP_CONN;
    }
    infop->sock_cmd_fd = socket(af, SOCK_STREAM, 0);
    if ( infop->sock_cmd_fd >= 0 )
    {
        if ( setsockopt(infop->sock_cmd_fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof(on)) < 0 )
        	log_event("Set TCP keep alive fail !");
		if(strlen(infop->scope_id) > 0)
		{
			if ( setsockopt(infop->sock_cmd_fd, SOL_SOCKET, SO_BINDTODEVICE, infop->scope_id, strlen(infop->scope_id)) < 0)
			    log_event("Set TCP bind to device fail !");		
		}
        infop->state = STATE_TCP_CONN;
    }

    if ((infop->sock_fd < 0) || (infop->sock_cmd_fd < 0))
    {
        close(infop->sock_fd);
        close(infop->sock_cmd_fd);
        if ( !(infop->error_flags & ERROR_TCP_OPEN) )
        {
            if (infop->sock_fd < 0)
            {
                sprintf(buf, "Socket open fail (%s, TCP port %d) !",
                        infop->ip_addr_s,
                        infop->tcp_port);
                log_event(buf);
            }
            if (infop->sock_fd < 0)
            {
                sprintf(buf, "Socket open fail (%s, TCP port %d) !",
    	                infop->ip_addr_s,
        	            infop->cmd_port);
	            log_event(buf);
            }
            infop->error_flags |= ERROR_TCP_OPEN;
        }
        infop->sock_fd = -1;
        infop->sock_cmd_fd = -1;
        infop->state = STATE_TCP_OPEN; // Scott: 2005-09-20
    }
}

ConnectTcp(infop)
TTYINFO *	infop;
{
    int			childpid, n;
    ConnMsg 		msg;
	union sock_addr sock;

    if(infop->af == AF_INET6 && enable_ipv6 == DIS_IPV6)
		return;
    infop->state = STATE_TCP_WAIT;
    infop->tcp_wait_id++;
    if ( (childpid = fork()) == 0 )
    {	/* child process */
        msg.tcp_wait_id = infop->tcp_wait_id;
        close(pipefd[0]);
        msg.status = CONNECT_FAIL;

		if(infop->af == AF_INET)
		{
			sock.sin.sin_family = AF_INET;
			sock.sin.sin_addr.s_addr = *(u_long*)infop->ip6_addr;
			sock.sin.sin_port = htons(infop->cmd_port);
		}
		else
		{
			memset(&sock.sin6, 0, sizeof(sock));
			sock.sin6.sin6_family = AF_INET6;
			sock.sin6.sin6_port = htons(infop->cmd_port);
			memcpy(sock.sin6.sin6_addr.s6_addr, infop->ip6_addr, 16);
		}
        if ( connect(infop->sock_cmd_fd, (struct sockaddr*)&sock, sizeof(sock)) >= 0 )
        {
			if(infop->af == AF_INET)
			{
			    sock.sin.sin_family = AF_INET;
			    sock.sin.sin_addr.s_addr = *(u_long*)infop->ip6_addr;
			    sock.sin.sin_port = htons(infop->tcp_port);
			}
			else
			{
				sock.sin6.sin6_family = AF_INET6;
				sock.sin6.sin6_port = htons(infop->tcp_port);
				memcpy(sock.sin6.sin6_addr.s6_addr, infop->ip6_addr, 16);
			}
            if ( connect(infop->sock_fd, (struct sockaddr*)&sock, sizeof(sock)) >= 0)
            {	
                if(infop->af == AF_INET6)
               	{
                    int rand[16];
        			if ( write(infop->sock_cmd_fd, rand, 16) >= 0)
        			{
        			    if(read(infop->sock_cmd_fd, rand, 16) != 16)
        		    	{
        		    	    msg.infop = infop;
                            //write(pipefd[1], (char *)&msg, sizeof(ConnMsg));
                            write(pipefd[1], (char *)&msg, sizeof(ConnMsg));
                            close(pipefd[1]);
                            //close(pipefd[1]);
                            exit(0);
        		    	}	
        			}
    			}
                msg.status = CONNECT_OK;
            }
        }
        msg.infop = infop;
        write(pipefd[1], (char *)&msg, sizeof(ConnMsg));
        close(pipefd[1]);
        /*		close(infop->sock_fd);
        		close(infop->sock_cmd_fd);
        */
        exit(0);
        /*
        	msg.tcp_wait_id = infop->tcp_wait_id;
        	close(pipefd[0]);
        	msg.status = CONNECT_FAIL;
        	sin.sin_family = AF_INET;
        	sin.sin_addr.s_addr = infop->ip_addr;
        	sin.sin_port = htons(infop->tcp_port);
        	if ( connect(infop->sock_fd, (struct sockaddr *)&sin,
        		sizeof(sin)) >= 0 ) {
        		sin.sin_family = AF_INET;
        		sin.sin_addr.s_addr = infop->ip_addr;
        		sin.sin_port = htons(infop->cmd_port);
        		if ( connect(infop->sock_cmd_fd,
        			 (struct sockaddr *)&sin,
        			 sizeof(sin)) >= 0 ) {
        			msg.status = CONNECT_OK;
        		}
        	}
        	msg.infop = infop;
        	write(pipefd[1], (char *)&msg, sizeof(ConnMsg));
        	close(pipefd[1]);
        	close(infop->sock_fd);
        	close(infop->sock_cmd_fd);
        	exit(0);
        	*/
    }
    else if ( childpid < 0 )
    {
        infop->state = STATE_TCP_CONN;
        if ( !(infop->error_flags & ERROR_FORK) )
        {
            log_event("Can't fork child process !");
            infop->error_flags |= ERROR_FORK;
        }
    }
}

#ifdef SSL_ON
ConnectSSL(infop)
TTYINFO *	infop;
{
    fd_set		rfd, wfd;
    struct timeval	tm;
    int	fd, flags, ret;
    struct sysinfo	sys_info;
    char buf[100];

    fd = infop->sock_fd;

    tm.tv_sec = 0;
    tm.tv_usec = 1000;

    FD_ZERO(&rfd);
    FD_ZERO(&wfd);

    FD_SET(fd, &wfd);
    FD_SET(fd, &rfd);

    if ((flags = fcntl(infop->sock_fd, F_GETFL, 0)) < 0)
        log_event("fcntl F_GETFL fail!");
    if (fcntl(infop->sock_fd, F_SETFL, flags | O_NONBLOCK) < 0)
        log_event("fcntl F_SETFL fail!");

    if ( select(fd+1, &rfd, &wfd, 0, &tm) > 0 )
    {
        if ( FD_ISSET(fd, &wfd) || FD_ISSET(fd, &rfd))
        {
            if ((ret = SSL_connect(infop->pssl)) > 0)
            {
                fcntl(fd, F_SETFL, flags);
                infop->state = STATE_RW_DATA;
            }
            else
            {
                ret = SSL_get_error(infop->pssl, ret);
                switch (ret)
                {
                case SSL_ERROR_WANT_READ:
                case SSL_ERROR_WANT_WRITE:
                    infop->state = STATE_SSL_CONN;
                    log_event("SSL_ERROR_WANT_WRITE");
                    break;
                case SSL_ERROR_ZERO_RETURN:
                case SSL_ERROR_WANT_CONNECT:
                case SSL_ERROR_WANT_X509_LOOKUP:
                case SSL_ERROR_SYSCALL:
                case SSL_ERROR_SSL:
                    infop->state = STATE_TCP_CLOSE;
					infop->reconn_flag = 0;
                    sprintf(buf, "SSL_connect Other Error %d", ret);
                    log_event(buf);
                    break;
                }
            }
        }
    }
    sysinfo(&sys_info);
    if ((sys_info.uptime - infop->ssl_time) > 5 )
    {
        infop->state = STATE_TCP_CLOSE;
		infop->reconn_flag = 0;
        log_event("Your target machine might not be set secure mode.");
    }
}
#endif

CloseTcp(infop)
TTYINFO *	infop;
{
    struct sockaddr_in	sin;
    int			childpid;
    ConnMsg 		msg;

    infop->state = STATE_TCP_WAIT;
    infop->tcp_wait_id++;
    if ( (childpid = fork()) == 0 )
    {	/* child process */
        msg.tcp_wait_id = infop->tcp_wait_id;
        close(pipefd[0]);
#ifdef SSL_ON
        if (infop->ssl_enable)
        {
            SSL_shutdown(infop->pssl);
            SSL_free(infop->pssl);
            infop->pssl = NULL;
        }
#endif
        close(infop->sock_fd);
        close(infop->sock_cmd_fd);
        sleep(1);
        msg.status = CLOSE_OK;
        msg.infop = infop;
        write(pipefd[1], (char *)&msg, sizeof(ConnMsg));
        close(pipefd[1]);
        exit(0);
    }
    else if ( childpid < 0 )
    {
        infop->state = STATE_TCP_CLOSE;
        if ( !(infop->error_flags & ERROR_FORK) )
        {
            log_event("Can't fork child process !");
            infop->error_flags |= ERROR_FORK;
        }
    }

    if ( infop->state != STATE_TCP_CLOSE )
    {
#ifdef SSL_ON
        if (infop->ssl_enable)
        {
            SSL_shutdown(infop->pssl);
            SSL_free(infop->pssl);
            infop->pssl = NULL;
        }
#endif
        close(infop->sock_fd);
        close(infop->sock_cmd_fd);
        infop->local_tcp_port = 0;
        infop->local_cmd_port = 0;
    }
}

ConnectCheck()
{
    ConnMsg 	msg;
    TTYINFO *	infop;
    char		buf[80];
    int ret;
    struct sysinfo	sys_info;
    struct sockaddr_in	local_sin;
	struct sockaddr_in6	local_sin6;
    socklen_t		socklen = sizeof(local_sin);
	struct sockaddr * ptr;

    if ((ret=read(pipefd[0], (char *)&msg, sizeof(ConnMsg))) == sizeof(ConnMsg))
    {
        infop = msg.infop;
        if ( (infop->state == STATE_TCP_WAIT)&&(infop->tcp_wait_id == msg.tcp_wait_id) )
        {
        	ptr = (infop->af == AF_INET) ? (struct sockaddr*)&local_sin : (struct sockaddr*)&local_sin6;
			socklen = (infop->af == AF_INET) ? sizeof(local_sin) : sizeof(local_sin6);
            if ( msg.status == CONNECT_OK )
            {
				infop->alive_check_cnt = 0;
                getsockname(infop->sock_fd, ptr, &socklen);
				if(infop->af == AF_INET)
	                infop->local_tcp_port = ntohs(local_sin.sin_port);
				else
					infop->local_tcp_port = ntohs(local_sin6.sin6_port);
                getsockname(infop->sock_cmd_fd, ptr, &socklen);
				if(infop->af == AF_INET)
					infop->local_cmd_port = ntohs(local_sin.sin_port);
				else
					infop->local_cmd_port = ntohs(local_sin6.sin6_port);
                
				infop->state = STATE_RW_DATA;
                infop->error_flags = 0;
                buf[0] = NPREAL_LOCAL_COMMAND_SET;
                buf[1] = LOCAL_CMD_TTY_USED;
                ioctl(infop->mpt_fd,
                      _IOC(_IOC_READ|_IOC_WRITE,'m',CMD_CONNECTED,0),
                      0);
                ioctl(infop->mpt_fd,
                      _IOC(_IOC_READ|_IOC_WRITE,'m',CMD_RESPONSE,2),
                      buf);
#ifdef SSL_ON
                if (infop->ssl_enable)
                {
                    infop->pssl = SSL_new(sslc_ctx);
                    if (infop->pssl != NULL)
                    {
                        if (SSL_set_fd(infop->pssl, infop->sock_fd))
                        {
                            SSL_set_connect_state(infop->pssl);
                        }
                        else
                        {
                            log_event("SSL_set_fd() error!");
                        }
                    }
                    else
                    {
                        log_event("SSL_new() error!");
                    }
                    sysinfo(&sys_info);
                    infop->ssl_time = sys_info.uptime;
                    infop->state = STATE_SSL_CONN;
                    /*if (SSL_connect(infop->pssl) < 0){
                    				printf("SSL_connect() error.\n");
                    	SSL_free(infop->pssl);
                    }*/
                }
#endif
            }
            else if ( msg.status == CLOSE_OK )
            {
                infop->error_flags = 0;
                infop->sock_fd = -1;
                infop->sock_cmd_fd = -1;
                if(infop->reconn_flag == 1)       /*reconnect or not*/
                    infop->state = STATE_TCP_OPEN;
                else if(infop->reconn_flag == 0)
                    infop->state = STATE_TTY_WAIT;
                ioctl(infop->mpt_fd,
                      _IOC(_IOC_READ|_IOC_WRITE,'m',CMD_DISCONNECTED,0),
                      0);
            }
            else
            {
                close(infop->sock_fd);
                close(infop->sock_cmd_fd);
                infop->sock_fd = -1;
                infop->sock_cmd_fd = -1;
                infop->local_tcp_port = 0;
                infop->local_cmd_port = 0;
                infop->state = STATE_CONN_FAIL;
                sysinfo(&sys_info);
                infop->time_out = sys_info.uptime;
                ioctl(infop->mpt_fd,
                      _IOC(_IOC_READ|_IOC_WRITE,'m',CMD_DISCONNECTED,0),
                      0);
                if ( !(infop->error_flags & ERROR_TCP_CONN) && ((sys_info.uptime - infop->tty_used_timestamp) > 60))
                {
                    sprintf(buf, "ConnectCheck> Socket connect fail (%s,TCP port %d) !",
                            infop->ip_addr_s,
                            infop->tcp_port);
                    log_event(buf);
                    infop->error_flags |= ERROR_TCP_CONN;
                }
            }
        }
    }
}

void log_event(char *msg)
{
    FILE *		fd;
    time_t		t;
    struct tm	*tt;
    char		tmp[80];

    if (Restart_daemon)
        return;

    t = time(0);
    tt = localtime(&t);
    /*
     * Open Log file as append mode.
     */
    fd = fopen(EventLog, "a+");
    if ( fd )
    {
        sprintf(tmp, "%02d-%02d-%4d %02d:%02d:%02d  ",
                tt->tm_mon + 1, tt->tm_mday, tt->tm_year+1900,
                tt->tm_hour, tt->tm_min, tt->tm_sec);
        fputs(tmp, fd);
        fputs(msg, fd);
        fputs("\n", fd);
        fclose(fd);
    }
}

#ifndef	STREAM
void	restart_handle ()
#endif
#ifdef	STREAM
void	restart_handle (int sig)
#endif
{
    Restart_daemon = 1;
#ifndef	STREAM
    signal (SIGTERM, ( (void (*)()) wait_handle) );
#endif
#ifdef	STREAM
    sig = sig;
    signal (SIGTERM, wait_handle);
#endif
}

#ifndef	STREAM
void	wait_handle ()
#endif
#ifdef	STREAM
void	wait_handle (int sig)
#endif
{
    Restart_daemon = 2;
#ifndef	STREAM
    signal (SIGTERM, ( (void (*)()) wait_handle) );
#endif
#ifdef	STREAM
    sig = sig;
    signal (SIGTERM, wait_handle);
#endif
}

#ifndef	STREAM
void	connect_wait_handle ()
#endif
#ifdef	STREAM
void	connect_wait_handle (int sig)
#endif
{
#ifndef	STREAM
    signal (SIGUSR1, ( (void (*)()) connect_wait_handle) );
#endif
#ifdef	STREAM
    sig = sig;
    signal (SIGUSR1, connect_wait_handle);
#endif
}

#ifdef	SSL_ON
static void ssl_init(void)
{
    SSLeay_add_ssl_algorithms();

#ifdef SSL_VER2
    sslc_ctx = SSL_CTX_new(SSLv2_client_method());
#else
#ifdef SSL_VER3
    sslc_ctx = SSL_CTX_new(SSLv3_client_method());
    ;
#else
    sslc_ctx = SSL_CTX_new(SSLv23_client_method());
#endif
#endif

    /* For blocking mode: cause read/write operations to only return after the handshake and successful completion. */
    SSL_CTX_set_mode(sslc_ctx, SSL_MODE_AUTO_RETRY);
}
#endif
#ifndef STREAM
void    config_changed_handle ()
#endif
#ifdef  STREAM
void    config_changed_handle (int sig)
#endif
{
    moxattyd_change_config();
    Gconfig_changed = 1;
#ifndef STREAM
    signal (SIGUSR1, ( (void (*)()) config_changed_handle) );
#endif
#ifdef  STREAM
    sig = sig;
    signal (SIGUSR1, config_changed_handle);
#endif
}

