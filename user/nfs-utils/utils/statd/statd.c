/* 
 * Copyright (C) 1995, 1997-1999 Jeffrey A. Uphoff
 * Modified by Olaf Kirch, Oct. 1996.
 * Modified by H.J. Lu, 1998.
 * Modified by L. Hohberger of Mission Critical Linux, 2000.
 *
 * NSM for Linux.
 */

#include "config.h"
#include <limits.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <getopt.h>
#include <rpc/rpc.h>
#include <rpc/pmap_clnt.h>
#include <rpcmisc.h>
#include "statd.h"
#include "version.h"

/* Socket operations */
#include <sys/types.h>
#include <sys/socket.h>


short int restart = 0;
int	run_mode = 0;		/* foreground logging mode */

/* LH - I had these local to main, but it seemed silly to have 
 * two copies of each - one in main(), one static in log.c... 
 * It also eliminates the 256-char static in log.c */
char *name_p = NULL;
char *version_p = NULL;

static struct option longopts[] =
{
	{ "foreground", 0, 0, 'F' },
	{ "no-syslog", 0, 0, 'd' },
	{ "help", 0, 0, 'h' },
	{ "version", 0, 0, 'v' },
	{ "outgoing-port", 1, 0, 'o' },
	{ "port", 1, 0, 'p' },
	{ NULL, 0, 0, 0 }
};

extern void sm_prog_1 (struct svc_req *, register SVCXPRT *);
extern int statd_get_socket(int port);

#ifdef SIMULATIONS
extern void simulator (int, char **);
#endif


#ifdef HAVE_TCP_WRAPPER 
#include "tcpwrapper.h"

static void 
sm_prog_1_wrapper (struct svc_req *rqstp, register SVCXPRT *transp)
{
	/* remote host authorization check */
	if (!check_default("statd", svc_getcaller(transp),
				 rqstp->rq_proc, SM_PROG)) {
		svcerr_auth (transp, AUTH_FAILED);
		return;
	}

	sm_prog_1 (rqstp, transp);
}

#define sm_prog_1 sm_prog_1_wrapper
#endif

/*
 * Signal handler.
 */
static void 
killer (int sig)
{
	log (L_FATAL, "Caught signal %d, un-registering and exiting.", sig);
	pmap_unset (SM_PROG, SM_VERS);
	exit (0);
}

/*
 * Startup information.
 */
static void log_modes(void)
{
	char buf[128];		/* watch stack size... */

	/* No flags = no message */
	if (!run_mode) return;

	memset(buf,0,128);
	sprintf(buf,"Flags: ");
	if (run_mode & MODE_NODAEMON)
		strcat(buf,"No-Daemon ");
	if (run_mode & MODE_LOG_STDERR)
		strcat(buf,"Log-STDERR ");
	/* future: IP aliasing
	if (run_mode & MODE_NOTIFY_ONLY)
	{
		strcat(buf,"Notify-Only ");
	} */
	log(L_WARNING,buf);
	/* future: IP aliasing
	if (run_mode & MODE_NOTIFY_ONLY)
	{
		dprintf(L_DEBUG,"Notify IP: %s",svr_addr);
	} */
}

/*
 * Since we do more than standard statd stuff, we might need to
 * help the occasional admin. 
 */
static void 
usage()
{
	fprintf(stderr,"usage: %s [options]\n", name_p);
	fprintf(stderr,"      -h, -?, --help       Print this help screen.\n");
	fprintf(stderr,"      -F, --foreground     Foreground (no-daemon mode)\n");
	fprintf(stderr,"      -d, --no-syslog      Verbose logging to stderr.  Foreground mode only.\n");
	fprintf(stderr,"      -p, --port           Port to listen on\n");
	fprintf(stderr,"      -o, --outgoing-port  Port for outgoing connections\n");
	fprintf(stderr,"      -V, -v, --version    Display version information and exit.\n");
}

/* 
 * Entry routine/main loop.
 */
int main (int argc, char **argv)
{
	extern char *optarg;
	int pid;
	int arg;
	int port = 0, out_port = 0;

	/* Default: daemon mode, no other options */
	run_mode = 0;

	/* Set the basename */
	if ((name_p = strrchr(argv[0],'/')) != NULL) {
		name_p ++;
	} else {
		name_p = argv[0];
	}

	/* Get the version */
	if ((version_p = strrchr(VERSION,' ')) != NULL) {
		version_p++;
	} else {
		version_p = VERSION;
	}
	
	/* Process command line switches */
	while ((arg = getopt_long(argc, argv, "h?vVFdp:o:", longopts, NULL)) != EOF) {
		switch (arg) {
		case 'V':	/* Version */
		case 'v':
			printf("%s version %s\n",name_p,version_p);
			exit(0);
		case 'F':	/* Foreground/nodaemon mode */
			run_mode |= MODE_NODAEMON;
			break;
		case 'd':	/* No daemon only - log to stderr */
			run_mode |= MODE_LOG_STDERR;
			break;
		case 'o':
			out_port = atoi(optarg);
			if (out_port < 1 || out_port > 65535) {
				fprintf(stderr, "%s: bad port number: %s\n",
					argv[0], optarg);
				usage();
				exit(1);
			}
			break;
		case 'p':
			port = atoi(optarg);
			if (port < 1 || port > 65535) {
				fprintf(stderr, "%s: bad port number: %s\n",
					argv[0], optarg);
				usage();
				exit(1);
			}
			break;
		case '?':	/* heeeeeelllllllpppp? heh */
		case 'h':
			usage();
			exit (0);
		default:	/* oh dear ... heh */
			usage();
			exit(-1);
		}
	}

	if (port == out_port && port != 0) {
		fprintf(stderr, "Listening and outgoing ports cannot be the same!\n");
		exit(-1);
	}

	if (!(run_mode & MODE_NODAEMON)) {
		run_mode &= ~MODE_LOG_STDERR;	/* Never log to console in
						   daemon mode. */
	}

	log_init (name_p,version_p);

	log_modes();

#ifdef SIMULATIONS
	if (argc > 1)
		/* LH - I _really_ need to update simulator... */
		simulator (--argc, ++argv);	/* simulator() does exit() */
#endif
	
	if (!(run_mode & MODE_NODAEMON)) {
		int filedes, fdmax, tempfd;

		if ((pid = fork ()) < 0) {
			perror ("Could not fork");
			exit (1);
		} else if (pid != 0) {
			/* Parent. */
			exit (0);
		}
		/* Child.	*/
		setsid ();
		chdir (DIR_BASE);

		tempfd = open("/dev/null", O_RDWR);
		close(0); dup2(tempfd, 0);
		close(1); dup2(tempfd, 1);
		close(2); dup2(tempfd, 2);
		fdmax = sysconf (_SC_OPEN_MAX);
		for (filedes = 3; filedes < fdmax; filedes++) {
			close (filedes);
		}
	}

	/* Child. */
	signal (SIGHUP, killer);
	signal (SIGINT, killer);
	signal (SIGTERM, killer);
	/* WARNING: the following works on Linux and SysV, but not BSD! */
	signal(SIGCHLD, SIG_IGN);

	/* initialize out_port */
	statd_get_socket(out_port);

	for (;;) {
		pmap_unset (SM_PROG, SM_VERS);
		change_state ();
		shuffle_dirs ();	/* Move directory names around */
		notify_hosts ();	/* Send out notify requests */
		++restart;

		/* future: IP aliasing 
		if (!(run_mode & MODE_NOTIFY_ONLY)) {
			rpc_init("statd", SM_PROG, SM_VERS, sm_prog_1, port);
		} */
		/* this registers both UDP and TCP services */
		rpc_init("statd", SM_PROG, SM_VERS, sm_prog_1, port);

		/*
		 * Handle incoming requests:  SM_NOTIFY socket requests, as
		 * well as callbacks from lockd.
		 */
		my_svc_run();	/* I rolled my own, Olaf made it better... */
	}
	return 0;
}
