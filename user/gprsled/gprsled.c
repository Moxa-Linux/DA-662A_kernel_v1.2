
/*
 * This is a program to show the GPRS signal strength.
 *
 * History:
 * 04-20-2007	Victor Yu.		Create it.
 */
#include	<stdio.h>
#include	<stdlib.h>
#include	<fcntl.h>
#include	<unistd.h>
#include	<termios.h>

#ifdef DEBUG
#define dbg_printf(x...)	printf(x)
#else
#define dbg_printf(x...)
#endif

#define PIPE_NODE	"/dev/gsled"
#define GPRS_TTY	"/dev/ttyS1"
#define LED_CONTROL	"/proc/driver/dio"

#define LED1_ON_STR	"8 1 0\n"
#define LED2_ON_STR	"9 1 0\n"
#define LED3_ON_STR	"20 1 0\n"
#define LED4_ON_STR	"21 1 0\n"
#define LED5_ON_STR	"28 1 0\n"
#define LED1_OFF_STR	"8 1 1\n"
#define LED2_OFF_STR	"9 1 1\n"
#define LED3_OFF_STR	"20 1 1\n"
#define LED4_OFF_STR	"21 1 1\n"
#define LED5_OFF_STR	"28 1 1\n"
#define LED_STR_LEN	7
#define LED_STR2_LEN	6

#define LED1_ON(f)	{write((f), LED1_ON_STR, LED_STR2_LEN);}
#define LED2_ON(f)	{write((f), LED2_ON_STR, LED_STR2_LEN);}
#define LED3_ON(f)	{write((f), LED3_ON_STR, LED_STR_LEN);}
#define LED4_ON(f)	{write((f), LED4_ON_STR, LED_STR_LEN);}
#define LED5_ON(f)	{write((f), LED5_ON_STR, LED_STR_LEN);}
#define LED1_OFF(f)	{write((f), LED1_OFF_STR, LED_STR2_LEN);}
#define LED2_OFF(f)	{write((f), LED2_OFF_STR, LED_STR2_LEN);}
#define LED3_OFF(f)	{write((f), LED3_OFF_STR, LED_STR_LEN);}
#define LED4_OFF(f)	{write((f), LED4_OFF_STR, LED_STR_LEN);}
#define LED5_OFF(f)	{write((f), LED5_OFF_STR, LED_STR_LEN);}

#define BUF_LEN		128
char			buf[BUF_LEN+1];

static int	open_tty(void)
{
	int		fd;
	struct termios	termios;

	fd = open(GPRS_TTY, O_RDWR | O_NONBLOCK);
	if ( fd < 0 )
		return fd;
	tcgetattr(fd, &termios);
	termios.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
	termios.c_lflag = 0;
	termios.c_oflag = 0;
	termios.c_iflag = 0;
	termios.c_cc[VMIN] = 0;
	termios.c_cc[VTIME] = 0;
	tcsetattr(fd, TCSANOW, &termios);
	return fd;
}

int	main(int argc, char *argv[])
{
	int	fd_gsled, fd_tty, fd_dio;
	int	stop_flag, len;

	fd_gsled = open(PIPE_NODE, O_RDWR | O_NONBLOCK);
	if ( fd_gsled < 0 ) {
		dbg_printf("Open pipe node [%s] fail !\n", PIPE_NODE);
		return 0;
	}
	fd_tty = open_tty();
	if ( fd_tty < 0 ) {
		dbg_printf("Open GPRS control tty fail !\n");
		close(fd_gsled);
		return 0;
	}
	fd_dio = open(LED_CONTROL, O_RDWR | O_NONBLOCK);
	if ( fd_dio < 0 ) {
		dbg_printf("Open LED control file fail !\n");
		close(fd_gsled);
		close(fd_tty);
		return 0;
	}

	stop_flag = 0;
	while ( 1 ) {
		if ( read(fd_gsled, buf, 1) > 0 ) {
			if ( buf[0] == '1' && stop_flag == 1 ) {
				dbg_printf("Start to send AT command\n");
				stop_flag = 0;
				fd_tty = open_tty();
			}
			if ( buf[0] == '0' && stop_flag == 0 ) {
stop_at_command:
				dbg_printf("Stop to send AT command\n");
				stop_flag = 1;
				tcflush(fd_tty,TCIOFLUSH);
				close(fd_tty);
			}
		}
		if ( stop_flag == 1 ) {
			sleep(1);
			continue;
		}

		dbg_printf("Send AT command.\n");
		write(fd_tty, "AT+CSQ\r", 7);
		len = read(fd_tty, buf, BUF_LEN);
		if ( len > 0 ) {
			int 	i;
			buf[len] = 0;
			dbg_printf("Read data [%s][%d]\n", buf, len);
			for ( i=0; i<len; i++ ) {
				if ( buf[i] == '+' && strncmp(&buf[i+1], "CSQ:", 4) == 0 ) 
					break;
			}
			i++;
			if ( strncmp(&buf[i], "CSQ:", 4) == 0 ) {
				int	j, v;
				i += 4;
				if ( i >= len )
					goto main_loop;
				for ( j=i; j<len; j++ ) {
					if ( buf[j] == ',' )
						break;
				}
				buf[j] = 0;
				v = strtol(&buf[i], NULL, 0);
				dbg_printf("Read value = %d\n", v);
				if ( v <= 0 || v == 99 ) {	// LED all off
				} else if ( v > 24 ) {		// 5 LED
					LED1_ON(fd_dio);
					LED2_ON(fd_dio);
					LED3_ON(fd_dio);
					LED4_ON(fd_dio);
					LED5_ON(fd_dio);
				} else if ( v > 18 ) {		// 4 LED
					LED5_OFF(fd_dio);
					LED1_ON(fd_dio);
					LED2_ON(fd_dio);
					LED3_ON(fd_dio);
					LED4_ON(fd_dio);
				} else if ( v > 12 ) {		// 3 LED
					LED5_OFF(fd_dio);
					LED4_OFF(fd_dio);
					LED1_ON(fd_dio);
					LED2_ON(fd_dio);
					LED3_ON(fd_dio);
				} else if ( v > 6 ) {		// 2 LED
					LED5_OFF(fd_dio);
					LED4_OFF(fd_dio);
					LED3_OFF(fd_dio);
					LED1_ON(fd_dio);
					LED2_ON(fd_dio);
				} else {			// 1 LED
					LED5_OFF(fd_dio);
					LED4_OFF(fd_dio);
					LED3_OFF(fd_dio);
					LED2_OFF(fd_dio);
					LED1_ON(fd_dio);
				}
			}
		}
main_loop:
		sleep(1);
	}

	close(fd_tty);
	close(fd_gsled);
	close(fd_dio);
	return 0;
}
