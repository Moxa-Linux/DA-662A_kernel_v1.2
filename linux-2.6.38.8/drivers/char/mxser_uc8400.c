/* || CONFIG_MACH_MOXA_UC8400
 *          mxser.c  -- MOXA Smartio/Industio family multiport serial driver.
 *
 *      Copyright (C) 1999-2001  Moxa Technologies (support@moxa.com.tw).
 *
 *      This code is loosely based on the Linux serial driver, written by
 *      Linus Torvalds, Theodore T'so and others.
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *	
 *	Original release	10/26/00
 *
 *	Date		Author			Comment
 *	02/06/01				Support MOXA Industio family boards.
 *	02/06/01				Support TIOCGICOUNT.
 *	02/06/01				Fix the problem for connecting to serial mouse.
 *	02/06/01				Fix the problem for H/W flow control.
 *	02/06/01				Fix the compling warning when CONFIG_PCI 	
 *						don't be defined.
 *	08/12/04				Add to supprt special baud rate
 *	09/17/04				To support by pass flip, and directly send to ldisc
 *	12/28/04				Add to support TIOCSBRK, TIOCCBRK ioctl.
 *	12/31/04				Add TICOM_BRK, TICOM_PARITY & TIOCM_FRAME on TIOCMIWAIT input argument for 
 *						Moxa private RealCOM used.
 *	01/04/05				Add TICOM_XOFFHOLD and use buf_overrun to note xoff hold to support RealCOM
 *	02/04/05				Add to support for Q-Free specical used.
 *	11/22/05	Victor Yu.		Porting to Moxa Embedded system.
 *	12/13/2007	Victor Yu.		Add to support new embedded computer IA261 with MU150 chipset
 *						Moxa IA261 embedded computer does not use interrupt vector register.
 *						So I need to poll each UART when has interrupt came in.
 *	01-25-2008	Victor Yu.		Add to support Intel XScale 435 mother board with CP168U card.
 *      10/22/10	Add ier, interface parameters for disabling MCR interrupt, Wade.
 */

//#define VICTOR_DEBUG
#ifdef VICTOR_DEBUG
#define dbg_printk(x...)	printk(x)
#else
#define dbg_printk(x...)
#endif

#if !defined(CONFIG_MACH_MOXA_IA261) && !defined(CONFIG_MACH_KIXRP435)	// add by Victor Yu. 01-25-2008
#include <asm/arch/cpe/cpe.h>
#endif

#include <linux/module.h>

#if !defined(CONFIG_MACH_MOXA_IA261) && !defined(CONFIG_MACH_KIXRP435)	// add by Victor Yu. 01-25-2008
#include <asm/arch/cpe_int.h>
#endif

#include <linux/version.h>
#include <linux/errno.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial.h>
#include <linux/serial_reg.h>

#include <linux/major.h>
#include <linux/string.h>
#include <linux/fcntl.h>
#include <linux/ptrace.h>
#include <linux/ioport.h>
#include <linux/mm.h>
#include <linux/smp_lock.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/proc_fs.h>

#include <asm/system.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/segment.h>
#include <asm/bitops.h>

#ifdef CONFIG_MACH_MOXA_IA261	// add by Victor Yu. 12-13-2007
#include <asm/arch/moxa-ia261.h>
#endif

#define	VERSION_CODE(ver,rel,seq)	((ver << 16) | (rel << 8) | seq)

#define	MXSER_VERSION	"2.2"
#define	MXSERMAJOR	 30	
#define	MXSERCUMAJOR	 35	

#ifdef CONFIG_PCI
#include <linux/pci.h>
#endif /* ENABLE_PCI */

#include <asm/uaccess.h>
#define put_to_user(arg1, arg2) put_user(arg1, (unsigned long *)arg2)
#define get_from_user(arg1, arg2) get_user(arg1, (unsigned int *)arg2)

#if !defined(CONFIG_ARCH_IA241) && !defined(CONFIG_ARCH_IA241_32128) && !defined(CONFIG_MACH_MOXA_IA261) && !defined(CONFIG_MACH_KIXRP435)
#undef UART_RX
#undef UART_TX
#undef UART_DLL
#undef UART_TRG
#undef UART_DLM
#undef UART_IER
#undef UART_FCTR
#undef UART_IIR
#undef UART_FCR
#undef UART_EFR
#undef UART_LCR
#undef UART_MCR
#undef UART_LSR
#undef UART_MSR
#undef UART_SCR
#undef UART_EMSR
#define UART_RX		0
#define UART_TX		0
#define UART_DLL	0
#define UART_TRG	0
#define UART_DLM	4
#define UART_IER	4
#define UART_FCTR	4
#define UART_IIR	8
#define UART_FCR	8
#define UART_EFR	8
#define UART_LCR	12
#define UART_MCR	16
#define UART_LSR	20
#define UART_MSR	24
#define UART_SCR	28
#define UART_EMSR	28
#endif	// !CONFIG_ARCH_IA241

#define	MXSER_EVENT_TXLOW	 1
#define	MXSER_EVENT_HANGUP	 2

#define SERIAL_DO_RESTART

#define MXSER_BOARDS		4	/* Max. boards */
#define MXSER_PORTS		32	/* Max. ports */
#define MXSER_PORTS_PER_BOARD	8	/* Max. ports per board*/
#define MXSER_ISR_PASS_LIMIT	256	// mask by Victor Yu. 02-04-2005

#define	MXSER_ERR_IOADDR	-1
#define	MXSER_ERR_IRQ		-2
#define	MXSER_ERR_IRQ_CONFLIT	-3
#define	MXSER_ERR_VECTOR	-4

#define SERIAL_TYPE_NORMAL	1
#define SERIAL_TYPE_CALLOUT	2

#define WAKEUP_CHARS		256

#define UART_MCR_AFE		0x20
#define UART_LSR_SPECIAL	0x1E

#if 0
#define PORTNO(x)	(MINOR((x)->device) - (x)->driver.minor_start)
#else
#define PORTNO(x)	((x)->index)
#endif

#define RELEVANT_IFLAG(iflag)	(iflag & (IGNBRK|BRKINT|IGNPAR|PARMRK|INPCK))

#ifndef MIN
#define MIN(a,b)	((a) < (b) ? (a) : (b))
#endif

enum	{
	IA240_BOARD=1,
#ifdef CONFIG_MACH_KIXRP435	// add by Victor Yu. 01-25-2008
	CP168U_BOARD=2,
#endif
};

static char *mxser_brdname[] = {
	"MU860 UART",
#ifdef CONFIG_MACH_KIXRP435	// add by Victor Yu. 01-25-2008
	#if defined(CONFIG_MACH_MOXA_UC8000) || defined(CONFIG_MACH_MOXA_UC8400)	 || defined(CONFIG_MACH_MOXA_UC8406_NGC) // add by Victor Yu. 05-16-2008
	"MU860 UART",
	#else
	"CP168U UART",
	#endif	// CONFIG_MACH_MOXA_UC8000 || CONFIG_MACH_MOXA_UC8400
#endif
};

static int mxser_numports[] = {
#if defined(CONFIG_ARCH_UC_7112_LX_PLUS) || defined(CONFIG_ARCH_W321) || defined(CONFIG_ARCH_W325)
	2,
#elif defined(CONFIG_ARCH_W311) || defined(CONFIG_ARCH_W315)
	1,
#else
	4,
#endif
#ifdef CONFIG_MACH_KIXRP435	// add by Victor Yu. 01-25-2008
	8,
#endif
};

typedef struct {
        unsigned short  vendor_id;
        unsigned short  device_id;
        unsigned short  board_type;
}       mxser_pciinfo;

#if defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_IA241_32128) || defined(CONFIG_MACH_KIXRP435) // add by Victor Yu. 05-22-2007
static mxser_pciinfo    mxser_pcibrds[] = {
#ifdef CONFIG_MACH_KIXRP435	// add by Victor Yu. 01-25-2008
	#if defined(CONFIG_MACH_MOXA_UC8000) || defined(CONFIG_MACH_MOXA_UC8400) || defined(CONFIG_MACH_MOXA_UC8406_NGC)	// add by Victor Yu. 05-16-2008
        {0x1393,0x0001,CP168U_BOARD},
	#else
        {0x1393,0x1681,CP168U_BOARD},
	#endif	// CONFIG_MACH_MOXA_UC8000 || CONFIG_MACH_MOXA_UC8400
#else
        {0x1393,0x0001,IA240_BOARD},
#endif
};
#endif

/*
 *	MOXA ioctls
 */
#define MOXA			0x400
#define MOXA_GETDATACOUNT     (MOXA + 23)
#define	MOXA_GET_CONF         (MOXA + 35)
#define MOXA_DIAGNOSE         (MOXA + 50)
#define MOXA_CHKPORTENABLE    (MOXA + 60)
#define MOXA_HighSpeedOn      (MOXA + 61)
#define MOXA_GET_MAJOR        (MOXA + 63)
#define MOXA_GET_CUMAJOR      (MOXA + 64)
#define MOXA_GETMSTATUS       (MOXA + 65)

// following add by Victor Yu. 01-05-2004
#define MOXA_SET_OP_MODE      (MOXA + 66)
#define MOXA_GET_OP_MODE      (MOXA + 67)

#if 1	// add by Victor Yu. 01-26-2005
#define MOXA_UNWAIT		(MOXA+200)
#define MXSER_PROC_NODE		"driver/mxser"  // add by Victor Yu. 07-27-2004
#endif

#define RS232_MODE		0
#define RS485_2WIRE_MODE	1
#define RS422_MODE		2
#define RS485_4WIRE_MODE	3
#define OP_MODE_MASK		3

#ifdef CONFIG_MACH_MOXA_IA261	// add by Victor Yu. 12-14-2007
#define RS232_SET_VALUE		3
#define RS485_2WIRE_SET_VALUE	2
#define RS485_4WIRE_SET_VALUE	1
#define RS422_SET_VALUE		1
#endif
// above add by Victor Yu. 01-05-2004

#define CONFIG_MOXA_SUPPORT_SPECIAL_BAUD_RATE
#ifdef CONFIG_MOXA_SUPPORT_SPECIAL_BAUD_RATE	// add by Victor Yu. 08-12-2004
#define UC_SET_SPECIAL_BAUD_RATE	(MOXA+68)
#define UC_GET_SPECIAL_BAUD_RATE	(MOXA+69)
#define MOXA_SET_SPECIAL_BAUD_RATE	(MOXA+100)
#define MOXA_GET_SPECIAL_BAUD_RATE	(MOXA+101)
#endif

#if 1   // add by Victor Yu. 09-17-2008
#define MOXA_BREAK_TIMER        (MOXA+70)       // send break signal for a time
#define MOXA_DISABLE_FIFO       (MOXA+71)       // disable UART FIFO
#define MOXA_ENABLE_FIFO        (MOXA+72)       // enable UART FIFO
#endif

static int ttymajor=MXSERMAJOR;
static int calloutmajor=MXSERCUMAJOR;
static int verbose=0;

/* Variables for insmod */
MODULE_AUTHOR("Victor Yu.");
MODULE_DESCRIPTION("MOXA MU860/MU150 UART Device Driver");

#if 1 //  Jared, 11-26-2010, set the rx fifo trigger level
static int rx_trigger = 120; 
module_param(rx_trigger, int, 0);
MODULE_PARM_DESC(rx_trigger, "Set the rx_trigger level. default is 120. 1 for real time receiving.");
#endif

#if 1 // Begin --- add by Wade, 10-22-2010
static int interface = -1; /* RS232 0, RS485_2WIRE 1, RS422 2, RS485_4WIRE 3 */
static int ier = 1;     /* 1: keep ori setting, 0: disable modem status register */
module_param(interface, int, 0);
MODULE_PARM_DESC(interface, "0:RS232 1:RS485_2WIRE 2:RS422 3:RS485_4WIRE");
module_param(ier, int, 0);
MODULE_PARM_DESC(ier, "0:Disable modem status register when opening port");
#endif // End --- add by Wade, 10-22-2010

typedef struct _moxa_pci_info {
	unsigned short busNum;
	unsigned short devNum;
	struct pci_dev  *pdev;  // add by Victor Yu. 06-23-2003
} moxa_pci_info;

struct mxser_hwconf {
	int		board_type;
	int		ports;
	int		irq;
	int		vector;
	int		vector_mask;
	int		uart_type;
	int		ioaddr[MXSER_PORTS_PER_BOARD];
	int		baud_base[MXSER_PORTS_PER_BOARD];
	moxa_pci_info	pciInfo;
	int		IsMoxaMustChipFlag; // add by Victor Yu. 08-30-2002
	int		MaxCanSetBaudRate[MXSER_PORTS_PER_BOARD];	// add by Victor Yu. 09-04-2002
	int		opmode_ioaddr[MXSER_PORTS_PER_BOARD];	// add by Victor Yu. 01-05-2004
};
#ifdef CONFIG_MACH_MOXA_IA261	// add by Victor Yu. 12-14-2007
static unsigned char	keep_opmode=0;	// default all is RS232
static unsigned char	keep_opmode_set_value=0xff;
#endif

struct mxser_struct {
	int			port;
	int			base;		/* port base address */
	int			irq;		/* port using irq no. */
	int			vector; 	/* port irq vector */
	int			vectormask;	/* port vector mask */
	int			rx_trigger;	/* Rx fifo trigger level */
	int			baud_base;	/* max. speed */
	int			flags;		/* defined in tty.h */
	int			type;		/* UART type */
	struct tty_struct *	tty;
	int			read_status_mask;
	int			ignore_status_mask;
	int			xmit_fifo_size;
	int			custom_divisor;
	int			x_char; 	/* xon/xoff character */
	int			close_delay;
	unsigned short		closing_wait;
	int			IER;		/* Interrupt Enable Register */
	int			MCR;		/* Modem control register */
	unsigned long		event;
	int			count;		/* # of fd on device */
	int			blocked_open;	/* # of blocked opens */
	unsigned char		*xmit_buf;
	int			xmit_head;
	int			xmit_tail;
	int			xmit_cnt;
	struct work_struct	tqueue;
	struct ktermios		normal_termios;
	wait_queue_head_t	open_wait;
	wait_queue_head_t	close_wait;
	wait_queue_head_t	delta_msr_wait;
	struct async_icount	icount; /* kernel counters for the 4 input interrupts */
	int			timeout;
	int			IsMoxaMustChipFlag; // add by Victor Yu. 08-30-2002
	int			MaxCanSetBaudRate;	// add by Victor Yu. 09-04-2002
	int			opmode_ioaddr;	// add by Victor Yu. 01-05-2004
#ifdef CONFIG_MOXA_SUPPORT_SPECIAL_BAUD_RATE	// add by Victor Yu. 08-12-2004
	int			speed;
#endif
};

struct mxser_log {
	int	tick;
	int	rxcnt[MXSER_PORTS];
	int	txcnt[MXSER_PORTS];
};

struct mxser_mstatus{
       tcflag_t	cflag;
       int  	cts;
       int  	dsr;
       int  	ri;
       int  	dcd;
};

static  struct mxser_mstatus GMStatus[MXSER_PORTS];

static struct tty_driver	mxvar_sdriver;
static struct mxser_struct	mxvar_table[MXSER_PORTS];
static struct ktermios * 	mxvar_termios[MXSER_PORTS+1];
static struct ktermios * 	mxvar_termios_locked[MXSER_PORTS+1];
static struct mxser_log 	mxvar_log;
static int			mxvar_diagflag;
/*
 * mxvar_tmp_buf is used as a temporary buffer by serial_write. We need
 * to lock it in case the memcpy_fromfs blocks while swapping in a page,
 * and some other program tries to do a serial write at the same time.
 * Since the lock will only come under contention when the system is
 * swapping and available memory is low, it makes sense to share one
 * buffer across all the serial ports, since it significantly saves
 * memory if large numbers of serial ports are open.
 */
static unsigned char *		mxvar_tmp_buf;
//static struct semaphore 	mxvar_tmp_buf_sem;

/*
 * This is used to figure out the divisor speeds and the timeouts
 */
#define B50_INDEX	1	// add by Victor Yu. 08-12-2004
static int mxvar_baud_table[] = {
	0, 50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800, 2400, 4800,
	9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600, 0 };
#define BAUD_TABLE_NO	(sizeof(mxvar_baud_table)/sizeof(int))
	
struct mxser_hwconf mxsercfg[MXSER_BOARDS];

/*
 * static functions:
 */
#if 1	// add by Victor Yu. 02-21-2006
static int      mxser_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data);  // add by Victor Yu. 07-27-2004
#endif
static void 	mxser_getcfg(int board,struct mxser_hwconf *hwconf);
int		mxser_init(void);
#if defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_IA241_32128) || defined(CONFIG_MACH_KIXRP435)	// add by Victor Yu. 05-22-2007
static int	mxser_get_PCI_conf(int ,struct mxser_hwconf *);
#endif
static void	mxser_do_softint(struct work_struct *);
static int	mxser_open(struct tty_struct *, struct file *);
static void	mxser_close(struct tty_struct *, struct file *);
static int	mxser_write(struct tty_struct *, const unsigned char *, int);
static int	mxser_write_room(struct tty_struct *);
static void	mxser_flush_buffer(struct tty_struct *);
static int	mxser_chars_in_buffer(struct tty_struct *);
static void	mxser_flush_chars(struct tty_struct *);
#if 1 // Jared, 09-01-2011
static int	mxser_put_char(struct tty_struct *, unsigned char);
#else
static void	mxser_put_char(struct tty_struct *, unsigned char);
#endif
static int	mxser_ioctl(struct tty_struct *, struct file *, uint, ulong);
static int	mxser_ioctl_special(unsigned int, unsigned long);
static void	mxser_throttle(struct tty_struct *);
static void	mxser_unthrottle(struct tty_struct *);
static void	mxser_set_termios(struct tty_struct *, struct ktermios *);
static void	mxser_stop(struct tty_struct *);
static void	mxser_start(struct tty_struct *);
static void	mxser_hangup(struct tty_struct *);
static irqreturn_t	mxser_interrupt(int, void *);
static inline void mxser_receive_chars(struct mxser_struct *, int *);
static inline void mxser_transmit_chars(struct mxser_struct *);
static inline void mxser_check_modem_status(struct mxser_struct *, int);
static int	mxser_block_til_ready(struct tty_struct *, struct file *, struct mxser_struct *);
static int	mxser_startup(struct mxser_struct *);
static void	mxser_shutdown(struct mxser_struct *);
static int	mxser_change_speed(struct mxser_struct *, struct ktermios *old_termios);
static int	mxser_get_serial_info(struct mxser_struct *, struct serial_struct *);
static int	mxser_set_serial_info(struct mxser_struct *, struct serial_struct *);
static int	mxser_get_lsr_info(struct mxser_struct *, unsigned int *);
static void	mxser_send_break(struct mxser_struct *, int);
static void 	mxser_wait_until_sent(struct tty_struct *tty, int timeout);
static int	mxser_tiocmget(struct tty_struct *tty, struct file *file);
static int	mxser_tiocmset(struct tty_struct *tty, struct file *file, unsigned int set, unsigned int clear);

#ifdef CONFIG_MACH_MOXA_IA261	// add by Victor Yu. 12-14-2007
#undef outb
#define outb(x,y)	{unsigned char __vvc;*(volatile unsigned char *)(y)=(x);__vvc=*(volatile unsigned char *)(MOXA_IA261_UART_DIO_VIRT_BASE+0x60);}
#endif

//
// follwoing is modified by Victor Yu. 08-15-2002
//
// follow just for Moxa Must chip define.
//
// when LCR register (offset 0x03) write following value,
// the Must chip will enter enchance mode. And write value
// on EFR (offset 0x02) bit 6,7 to change bank.
#define MOXA_MUST_ENTER_ENCHANCE	0xBF

// when enhance mode enable, access on general bank register
#if !(defined CONFIG_ARCH_IA241) && !(defined CONFIG_ARCH_IA241_32128) && !(defined CONFIG_MACH_MOXA_IA261) && !(defined CONFIG_MACH_KIXRP435)
#define MOXA_MUST_GDL_REGISTER		28
#else
#define MOXA_MUST_GDL_REGISTER		7
#endif

#define MOXA_MUST_GDL_MASK		0x7F
#define MOXA_MUST_GDL_HAS_BAD_DATA	0x80

#define MOXA_MUST_LSR_RERR		0x80	// error in receive FIFO
// enchance register bank select and enchance mode setting register
// when LCR register equal to 0xBF
#if !defined(CONFIG_ARCH_IA241) && !defined(CONFIG_ARCH_IA241_32128) && !defined(CONFIG_MACH_MOXA_IA261) && !defined(CONFIG_MACH_KIXRP435)
#define MOXA_MUST_EFR_REGISTER		8
#else
#define MOXA_MUST_EFR_REGISTER		2
#endif

// enchance mode enable
#define MOXA_MUST_EFR_EFRB_ENABLE	0x10
// enchance reister bank set 0, 1, 2
#define MOXA_MUST_EFR_BANK0		0x00
#define MOXA_MUST_EFR_BANK1		0x40
#define MOXA_MUST_EFR_BANK2		0x80
#define MOXA_MUST_EFR_BANK3		0xC0
#define MOXA_MUST_EFR_BANK_MASK		0xC0

// set XON1 value register, when LCR=0xBF and change to bank0
#if !defined(CONFIG_ARCH_IA241) && !defined(CONFIG_ARCH_IA241_32128) && !defined(CONFIG_MACH_MOXA_IA261) && !defined(CONFIG_MACH_KIXRP435)
#define MOXA_MUST_XON1_REGISTER		16
#else
#define MOXA_MUST_XON1_REGISTER		4
#endif

// set XON2 value register, when LCR=0xBF and change to bank0
#if !defined(CONFIG_ARCH_IA241) && !defined(CONFIG_ARCH_IA241_32128) && !defined(CONFIG_MACH_MOXA_IA261) && !defined(CONFIG_MACH_KIXRP435)
#define MOXA_MUST_XON2_REGISTER		20
#else
#define MOXA_MUST_XON2_REGISTER		5
#endif

// set XOFF1 value register, when LCR=0xBF and change to bank0
#if !defined(CONFIG_ARCH_IA241) && !defined(CONFIG_ARCH_IA241_32128) && !defined(CONFIG_MACH_MOXA_IA261) && !defined(CONFIG_MACH_KIXRP435)
#define MOXA_MUST_XOFF1_REGISTER	24
#else
#define MOXA_MUST_XOFF1_REGISTER	6
#endif

// set XOFF2 value register, when LCR=0xBF and change to bank0
#if !defined(CONFIG_ARCH_IA241) && !defined(CONFIG_ARCH_IA241_32128) && !defined(CONFIG_MACH_MOXA_IA261) && !defined(CONFIG_MACH_KIXRP435)
#define MOXA_MUST_XOFF2_REGISTER	28
#define MOXA_MUST_RBRTL_REGISTER	16
#define MOXA_MUST_RBRTH_REGISTER	20
#define MOXA_MUST_RBRTI_REGISTER	24
#define MOXA_MUST_THRTL_REGISTER	28
#define MOXA_MUST_ENUM_REGISTER		16
#define MOXA_MUST_HWID_REGISTER		20
#define MOXA_MUST_ECR_REGISTER		24
#define MOXA_MUST_CSR_REGISTER		28
#else
#define MOXA_MUST_XOFF2_REGISTER	7
#define MOXA_MUST_RBRTL_REGISTER	4
#define MOXA_MUST_RBRTH_REGISTER	5
#define MOXA_MUST_RBRTI_REGISTER	6
#define MOXA_MUST_THRTL_REGISTER	7
#define MOXA_MUST_ENUM_REGISTER		4
#define MOXA_MUST_HWID_REGISTER		5
#define MOXA_MUST_ECR_REGISTER		6
#define MOXA_MUST_CSR_REGISTER		7
#endif

// good data mode enable
#define MOXA_MUST_FCR_GDA_MODE_ENABLE	0x20
// only good data put into RxFIFO
#define MOXA_MUST_FCR_GDA_ONLY_ENABLE	0x10

// enable CTS interrupt
#define MOXA_MUST_IER_ECTSI		0x80
// eanble RTS interrupt
#define MOXA_MUST_IER_ERTSI		0x40
// enable Xon/Xoff interrupt
#define MOXA_MUST_IER_XINT		0x20
// enable GDA interrupt
#define MOXA_MUST_IER_EGDAI		0x10

#define MOXA_MUST_RECV_ISR		(UART_IER_RDI | MOXA_MUST_IER_EGDAI)

// GDA interrupt pending
#define MOXA_MUST_IIR_GDA		0x1C
#define MOXA_MUST_IIR_RDA		0x04
#define MOXA_MUST_IIR_RTO		0x0C
#define MOXA_MUST_IIR_LSR		0x06

// recieved Xon/Xoff or specical interrupt pending
#define MOXA_MUST_IIR_XSC		0x10

// RTS/CTS change state interrupt pending
#define MOXA_MUST_IIR_RTSCTS		0x20
#define MOXA_MUST_IIR_MASK		0x3E

#define MOXA_MUST_MCR_XON_FLAG		0x40
#define MOXA_MUST_MCR_XON_ANY		0x80
#define MOXA_MUST_HARDWARE_ID		0x01
#define MOXA_MUST_HARDWARE_ID1		0x02

// software flow control on chip mask value
#define MOXA_MUST_EFR_SF_MASK		0x0F
// send Xon1/Xoff1
#define MOXA_MUST_EFR_SF_TX1		0x08
// send Xon2/Xoff2
#define MOXA_MUST_EFR_SF_TX2		0x04
// send Xon1,Xon2/Xoff1,Xoff2
#define MOXA_MUST_EFR_SF_TX12		0x0C
// don't send Xon/Xoff
#define MOXA_MUST_EFR_SF_TX_NO		0x00
// Tx software flow control mask
#define MOXA_MUST_EFR_SF_TX_MASK	0x0C
// don't receive Xon/Xoff
#define MOXA_MUST_EFR_SF_RX_NO		0x00
// receive Xon1/Xoff1
#define MOXA_MUST_EFR_SF_RX1		0x02
// receive Xon2/Xoff2
#define MOXA_MUST_EFR_SF_RX2		0x01
// receive Xon1,Xon2/Xoff1,Xoff2
#define MOXA_MUST_EFR_SF_RX12		0x03
// Rx software flow control mask
#define MOXA_MUST_EFR_SF_RX_MASK	0x03

#define MOXA_MUST_MIN_XOFFLIMIT		66
#define MOXA_MUST_MIN_XONLIMIT		20

#ifndef UCHAR
typedef unsigned char	UCHAR;
#endif

#define CHECK_MOXA_MUST_XOFFLIMIT(info) { 	\
	if ( (info)->HandFlow.XoffLimit < MOXA_MUST_MIN_XOFFLIMIT ) {	\
		(info)->HandFlow.XoffLimit = MOXA_MUST_MIN_XOFFLIMIT;	\
		(info)->HandFlow.XonLimit = MOXA_MUST_MIN_XONLIMIT;	\
	}	\
}

#define ENABLE_MOXA_MUST_ENCHANCE_MODE(baseio) { \
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((baseio)+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (baseio)+UART_LCR);	\
	__efr = inb((baseio)+MOXA_MUST_EFR_REGISTER);	\
	__efr |= MOXA_MUST_EFR_EFRB_ENABLE;	\
	outb(__efr, (baseio)+MOXA_MUST_EFR_REGISTER);	\
	outb(__oldlcr, (baseio)+UART_LCR);	\
}

#define DISABLE_MOXA_MUST_ENCHANCE_MODE(baseio) {	\
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((baseio)+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (baseio)+UART_LCR);	\
	__efr = inb((baseio)+MOXA_MUST_EFR_REGISTER);	\
	__efr &= ~MOXA_MUST_EFR_EFRB_ENABLE;	\
	outb(__efr, (baseio)+MOXA_MUST_EFR_REGISTER);	\
	outb(__oldlcr, (baseio)+UART_LCR);	\
}

#define SET_MOXA_MUST_XON1_VALUE(baseio, Value) {	\
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((baseio)+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (baseio)+UART_LCR);	\
	__efr = inb((baseio)+MOXA_MUST_EFR_REGISTER);	\
	__efr &= ~MOXA_MUST_EFR_BANK_MASK;	\
	__efr |= MOXA_MUST_EFR_BANK0;	\
	outb(__efr, (baseio)+MOXA_MUST_EFR_REGISTER);	\
	outb((UCHAR)(Value), (baseio)+MOXA_MUST_XON1_REGISTER);	\
	outb(__oldlcr, (baseio)+UART_LCR);	\
}

#define SET_MOXA_MUST_XON2_VALUE(baseio, Value) {	\
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((baseio)+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (baseio)+UART_LCR);	\
	__efr = inb((baseio)+MOXA_MUST_EFR_REGISTER);	\
	__efr &= ~MOXA_MUST_EFR_BANK_MASK;	\
	__efr |= MOXA_MUST_EFR_BANK0;	\
	outb(__efr, (baseio)+MOXA_MUST_EFR_REGISTER);	\
	outb((UCHAR)(Value), (baseio)+MOXA_MUST_XON2_REGISTER);	\
	outb(__oldlcr, (baseio)+UART_LCR);	\
}

#define SET_MOXA_MUST_XOFF1_VALUE(baseio, Value) {	\
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((baseio)+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (baseio)+UART_LCR);	\
	__efr = inb((baseio)+MOXA_MUST_EFR_REGISTER);	\
	__efr &= ~MOXA_MUST_EFR_BANK_MASK;	\
	__efr |= MOXA_MUST_EFR_BANK0;	\
	outb(__efr, (baseio)+MOXA_MUST_EFR_REGISTER);	\
	outb((UCHAR)(Value), (baseio)+MOXA_MUST_XOFF1_REGISTER);	\
	outb(__oldlcr, (baseio)+UART_LCR);	\
}

#define SET_MOXA_MUST_XOFF2_VALUE(baseio, Value) {	\
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((baseio)+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (baseio)+UART_LCR);	\
	__efr = inb((baseio)+MOXA_MUST_EFR_REGISTER);	\
	__efr &= ~MOXA_MUST_EFR_BANK_MASK;	\
	__efr |= MOXA_MUST_EFR_BANK0;	\
	outb(__efr, (baseio)+MOXA_MUST_EFR_REGISTER);	\
	outb((UCHAR)(Value), (baseio)+MOXA_MUST_XOFF2_REGISTER);	\
	outb(__oldlcr, (baseio)+UART_LCR);	\
}

#define SET_MOXA_MUST_RBRTL_VALUE(baseio, Value) {	\
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((baseio)+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (baseio)+UART_LCR);	\
	__efr = inb((baseio)+MOXA_MUST_EFR_REGISTER);	\
	__efr &= ~MOXA_MUST_EFR_BANK_MASK;	\
	__efr |= MOXA_MUST_EFR_BANK1;	\
	outb(__efr, (baseio)+MOXA_MUST_EFR_REGISTER);	\
	outb((UCHAR)(Value), (baseio)+MOXA_MUST_RBRTL_REGISTER);	\
	outb(__oldlcr, (baseio)+UART_LCR);	\
}

#define SET_MOXA_MUST_RBRTH_VALUE(baseio, Value) {	\
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((baseio)+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (baseio)+UART_LCR);	\
	__efr = inb((baseio)+MOXA_MUST_EFR_REGISTER);	\
	__efr &= ~MOXA_MUST_EFR_BANK_MASK;	\
	__efr |= MOXA_MUST_EFR_BANK1;	\
	outb(__efr, (baseio)+MOXA_MUST_EFR_REGISTER);	\
	outb((UCHAR)(Value), (baseio)+MOXA_MUST_RBRTH_REGISTER);	\
	outb(__oldlcr, (baseio)+UART_LCR);	\
}

#define SET_MOXA_MUST_RBRTI_VALUE(baseio, Value) {	\
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((baseio)+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (baseio)+UART_LCR);	\
	__efr = inb((baseio)+MOXA_MUST_EFR_REGISTER);	\
	__efr &= ~MOXA_MUST_EFR_BANK_MASK;	\
	__efr |= MOXA_MUST_EFR_BANK1;	\
	outb(__efr, (baseio)+MOXA_MUST_EFR_REGISTER);	\
	outb((UCHAR)(Value), (baseio)+MOXA_MUST_RBRTI_REGISTER);	\
	outb(__oldlcr, (baseio)+UART_LCR);	\
}

#define SET_MOXA_MUST_THRTL_VALUE(baseio, Value) {	\
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((baseio)+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (baseio)+UART_LCR);	\
	__efr = inb((baseio)+MOXA_MUST_EFR_REGISTER);	\
	__efr &= ~MOXA_MUST_EFR_BANK_MASK;	\
	__efr |= MOXA_MUST_EFR_BANK1;	\
	outb(__efr, (baseio)+MOXA_MUST_EFR_REGISTER);	\
	outb((UCHAR)(Value), (baseio)+MOXA_MUST_THRTL_REGISTER);	\
	outb(__oldlcr, (baseio)+UART_LCR);	\
}

#define MOXA_MUST_RBRL_VALUE	4
#define SET_MOXA_MUST_FIFO_VALUE(info) {	\
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((info)->base+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (info)->base+UART_LCR);	\
	__efr = inb((info)->base+MOXA_MUST_EFR_REGISTER);	\
	__efr &= ~MOXA_MUST_EFR_BANK_MASK;	\
	__efr |= MOXA_MUST_EFR_BANK1;	\
	outb(__efr, (info)->base+MOXA_MUST_EFR_REGISTER);	\
	outb((UCHAR)0, (info)->base+MOXA_MUST_THRTL_REGISTER);	\
	outb((UCHAR)((info)->rx_trigger), (info)->base+MOXA_MUST_RBRTH_REGISTER);	\
	if ( (info)->rx_trigger <= MOXA_MUST_RBRL_VALUE ) {	\
		outb((UCHAR)0, (info)->base+MOXA_MUST_RBRTI_REGISTER);	\
		outb((UCHAR)0, (info)->base+MOXA_MUST_RBRTL_REGISTER);	\
	} else {	\
		outb((UCHAR)((info)->rx_trigger-MOXA_MUST_RBRL_VALUE),	\
			(info)->base+MOXA_MUST_RBRTI_REGISTER);	\
		outb((UCHAR)((info)->rx_trigger-MOXA_MUST_RBRL_VALUE),	\
			(info)->base+MOXA_MUST_RBRTL_REGISTER);	\
	}	\
	outb(__oldlcr, (info)->base+UART_LCR);	\
}		

#define SET_MOXA_MUST_ENUM_VALUE(baseio, Value) {	\
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((baseio)+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (baseio)+UART_LCR);	\
	__efr = inb((baseio)+MOXA_MUST_EFR_REGISTER);	\
	__efr &= ~MOXA_MUST_EFR_BANK_MASK;	\
	__efr |= MOXA_MUST_EFR_BANK2;	\
	outb(__efr, (baseio)+MOXA_MUST_EFR_REGISTER);	\
	outb((UCHAR)(Value), (baseio)+MOXA_MUST_ENUM_REGISTER);	\
	outb(__oldlcr, (baseio)+UART_LCR);	\
}

#define GET_MOXA_MUST_HARDWARE_ID(baseio, pId) {	\
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((baseio)+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (baseio)+UART_LCR);	\
	__efr = inb((baseio)+MOXA_MUST_EFR_REGISTER);	\
	__efr &= ~MOXA_MUST_EFR_BANK_MASK;	\
	__efr |= MOXA_MUST_EFR_BANK2;	\
	outb(__efr, (baseio)+MOXA_MUST_EFR_REGISTER);	\
	*pId = inb((baseio)+MOXA_MUST_HWID_REGISTER);	\
	outb(__oldlcr, (baseio)+UART_LCR);	\
}

#define SET_MOXA_MUST_NO_SOFTWARE_FLOW_CONTROL(baseio) {	\
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((baseio)+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (baseio)+UART_LCR);	\
	__efr = inb((baseio)+MOXA_MUST_EFR_REGISTER);	\
	__efr &= ~MOXA_MUST_EFR_SF_MASK;	\
	outb(__efr, (baseio)+MOXA_MUST_EFR_REGISTER);	\
	outb(__oldlcr, (baseio)+UART_LCR);	\
}

#define SET_MOXA_MUST_JUST_TX_SOFTWARE_FLOW_CONTROL(baseio) {	\
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((baseio)+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (baseio)+UART_LCR);	\
	__efr = inb((baseio)+MOXA_MUST_EFR_REGISTER);	\
	__efr &= ~MOXA_MUST_EFR_SF_MASK;	\
	__efr |= MOXA_MUST_EFR_SF_TX1;	\
	outb(__efr, (baseio)+MOXA_MUST_EFR_REGISTER);	\
	outb(__oldlcr, (baseio)+UART_LCR);	\
}

#define ENABLE_MOXA_MUST_TX_SOFTWARE_FLOW_CONTROL(baseio) {	\
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((baseio)+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (baseio)+UART_LCR);	\
	__efr = inb((baseio)+MOXA_MUST_EFR_REGISTER);	\
	__efr &= ~MOXA_MUST_EFR_SF_TX_MASK;	\
	__efr |= MOXA_MUST_EFR_SF_TX1;	\
	outb(__efr, (baseio)+MOXA_MUST_EFR_REGISTER);	\
	outb(__oldlcr, (baseio)+UART_LCR);	\
}

#define DISABLE_MOXA_MUST_TX_SOFTWARE_FLOW_CONTROL(baseio) {	\
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((baseio)+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (baseio)+UART_LCR);	\
	__efr = inb((baseio)+MOXA_MUST_EFR_REGISTER);	\
	__efr &= ~MOXA_MUST_EFR_SF_TX_MASK;	\
	outb(__efr, (baseio)+MOXA_MUST_EFR_REGISTER);	\
	outb(__oldlcr, (baseio)+UART_LCR);	\
}

#define SET_MOXA_MUST_JUST_RX_SOFTWARE_FLOW_CONTROL(baseio) {	\
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((baseio)+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (baseio)+UART_LCR);	\
	__efr = inb((baseio)+MOXA_MUST_EFR_REGISTER);	\
	__efr &= ~MOXA_MUST_EFR_SF_MASK;	\
	__efr |= MOXA_MUST_EFR_SF_RX1;	\
	outb(__efr, (baseio)+MOXA_MUST_EFR_REGISTER);	\
	outb(__oldlcr, (baseio)+UART_LCR);	\
}

#define ENABLE_MOXA_MUST_RX_SOFTWARE_FLOW_CONTROL(baseio) {	\
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((baseio)+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (baseio)+UART_LCR);	\
	__efr = inb((baseio)+MOXA_MUST_EFR_REGISTER);	\
	__efr &= ~MOXA_MUST_EFR_SF_RX_MASK;	\
	__efr |= MOXA_MUST_EFR_SF_RX1;	\
	outb(__efr, (baseio)+MOXA_MUST_EFR_REGISTER);	\
	outb(__oldlcr, (baseio)+UART_LCR);	\
}

#define DISABLE_MOXA_MUST_RX_SOFTWARE_FLOW_CONTROL(baseio) {	\
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((baseio)+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (baseio)+UART_LCR);	\
	__efr = inb((baseio)+MOXA_MUST_EFR_REGISTER);	\
	__efr &= ~MOXA_MUST_EFR_SF_RX_MASK;	\
	outb(__efr, (baseio)+MOXA_MUST_EFR_REGISTER);	\
	outb(__oldlcr, (baseio)+UART_LCR);	\
}

#define ENABLE_MOXA_MUST_TX_RX_SOFTWARE_FLOW_CONTROL(baseio) {	\
	UCHAR	__oldlcr, __efr;	\
	__oldlcr = inb((baseio)+UART_LCR);	\
	outb(MOXA_MUST_ENTER_ENCHANCE, (baseio)+UART_LCR);	\
	__efr = inb((baseio)+MOXA_MUST_EFR_REGISTER);	\
	__efr &= ~MOXA_MUST_EFR_SF_MASK;	\
	__efr |= (MOXA_MUST_EFR_SF_RX1|MOXA_MUST_EFR_SF_TX1);	\
	outb(__efr, (baseio)+MOXA_MUST_EFR_REGISTER);	\
	outb(__oldlcr, (baseio)+UART_LCR);	\
}

#define ENABLE_MOXA_MUST_XON_ANY_FLOW_CONTROL(baseio) {	\
	UCHAR	__oldmcr;	\
	__oldmcr = inb((baseio)+UART_MCR);	\
	__oldmcr |= MOXA_MUST_MCR_XON_ANY;	\
	outb(__oldmcr, (baseio)+UART_MCR);	\
}

#define DISABLE_MOXA_MUST_XON_ANY_FLOW_CONTROL(baseio) {	\
	UCHAR	__oldmcr;	\
	__oldmcr = inb((baseio)+UART_MCR);	\
	__oldmcr &= ~MOXA_MUST_MCR_XON_ANY;	\
	outb(__oldmcr, (baseio)+UART_MCR);	\
}

#define READ_MOXA_MUST_GDL(baseio)	inb((baseio)+MOXA_MUST_GDL_REGISTER)

#if 1	// add by Victor Yu. 02-21-2006
static int	mxser_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int			len=0, board, port, p, i, j;
	struct mxser_struct	*info;

	for ( board=0, port=0; board<MXSER_BOARDS; board++ ) {
		if ( mxsercfg[board].board_type == -1 )
			continue;
		port += mxsercfg[board].ports;
	}
	len = sprintf(page, "%d\n", port);
	if ( !port )
		goto end_mxser_read_proc;
	for ( board=0, p=0; board<MXSER_BOARDS; board++ ) {
		if ( mxsercfg[board].board_type == -1 )
			continue;
		for ( port=0, i=(board*MXSER_PORTS_PER_BOARD); port<mxsercfg[board].ports; port++, p++, i++ ) {
			info = &mxvar_table[i];
			
			// output PortNo InBytes OutBytes
			len += sprintf(page+len, "%d %d", mxvar_log.rxcnt[i], mxvar_log.txcnt[i]);  

			// output Speed Opened (1 - opened, 2 - not opened)
			if ( info->count ) {	// has opened
				len += sprintf(page+len, " %d 1", info->speed);
			} else {	// not opened
				len += sprintf(page+len, " %d 2", info->speed);
			}

			// output interface (1-other, 2-rs232, 3-rs422, 4-rs423, 5-v35)
			{
			int		opmode;
			int             shiftbit;
#if (defined CONFIG_ARCH_W311)
			shiftbit = ((info->port%4)+1) * 2;
			opmode = inl(info->opmode_ioaddr) >> shiftbit;
#elif defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_IA241_32128) || defined(CONFIG_MACH_KIXRP435)
			shiftbit = (info->port%4) * 2;
			opmode = inb(info->opmode_ioaddr) >> shiftbit;
#elif defined(CONFIG_MACH_MOXA_IA261)	// add by Victor Yu. 12-14-2007
			shiftbit = (info->port%4) * 2;
			opmode = keep_opmode >> shiftbit;
#else
			shiftbit = ((info->port%4)+2) * 2;
			opmode = inl(info->opmode_ioaddr) >> shiftbit;
#endif
			opmode &= OP_MODE_MASK;
			switch ( opmode ) {
			case RS232_MODE :
				len += sprintf(page+len, " 2");
				break;
			case RS422_MODE :
				len += sprintf(page+len, " 3");
				break;
			default :
				len += sprintf(page+len, " 1");
				break;
			}
			}

			// output data bits
			if ( info->count ) { // has opened
				switch ( info->tty->termios->c_cflag & CSIZE ) {
				case CS5 :
					len += sprintf(page+len, " 5");
					break;
				case CS6 :
					len += sprintf(page+len, " 6");
					break;
				case CS7 :
					len += sprintf(page+len, " 7");
					break;
				case CS8 :
				default :
					len += sprintf(page+len, " 8");
					break;
				}
			} else {	// not opened
				len += sprintf(page+len, " 8");
			}

			// output stop bits (1-one, 2-two, 3-one-and-half, 4-dynamic)
			if ( info->count ) { // has opened
				if ( info->tty->termios->c_cflag & CSTOPB ) {
					len += sprintf(page+len, " 2");
				} else {
					len += sprintf(page+len, " 1");
				}
			} else {
				len += sprintf(page+len, " 1");
			}

			// output parity (1-none, 2-odd, 3-even, 4-mark, 5-space)
			if ( info->count ) { // has opened
				if ( info->tty->termios->c_cflag & PARENB ) {
					if ( info->tty->termios->c_cflag & PARODD ) {
						len += sprintf(page+len, " 2");
					} else {
						len += sprintf(page+len, " 3");
					}
				} else {
					len += sprintf(page+len, " 1");
				}
			} else {	// not opened
				len += sprintf(page+len, " 1");
			}

			// output CTS DSR DCD status (1-none, 2-on, 3-off)
			j = inb(info->base+UART_MSR);
			if ( j & UART_MSR_CTS )
				len += sprintf(page+len, " 2");
			else
				len += sprintf(page+len, " 3");
			if ( j & UART_MSR_DSR )
				len += sprintf(page+len, " 2");
			else
				len += sprintf(page+len, " 3");
			if ( j & UART_MSR_DCD )
				len += sprintf(page+len, " 2");
			else
				len += sprintf(page+len, " 3");

			// output RTS DTR status (1-none, 2-on, 3-off)
			j = inb(info->base+UART_MCR);
			if ( j & UART_MCR_RTS )
				len += sprintf(page+len, " 2");
			else
				len += sprintf(page+len, " 3");
			if ( j & UART_MCR_DTR )
				len += sprintf(page+len, " 2");
			else
				len += sprintf(page+len, " 3");

			// end output
			len += sprintf(page+len, "\n");
		}
	}

end_mxser_read_proc:
	if ( len <= (off + count) )
		*eof = 1;
	*start = page + off;
	len -= off;
	if ( len > count )
		len = count;
	if ( len < 0 )
		len = 0;

	return len;
}
#endif

/*
 * The MOXA Smartio/Industio serial driver boot-time initialization code!
 */
static int __init mxser_init_module(void)
{
	int	ret;

	if (verbose)
		printk("Loading module mxser ...\n");
	ret = mxser_init();
	if (verbose)
		printk("Done.\n");
	return (ret);
}

static void __exit mxser_exit_module(void)
{
	int i,err = 0;


	if (verbose)
		printk("Unloading module mxser ...\n");
#if 1	// mask by Victor Yu. 11-22-2005
	if ((err |= tty_unregister_driver(&mxvar_sdriver)))
		printk("Couldn't unregister MOXA Smartio/Industio family serial driver\n");
#else	// add by Victor Yu. 11-22-2005
	put_tty_driver(&mxvar_sdriver);
#endif	// 11-22-2005

        for(i=0; i<MXSER_BOARDS; i++){
#if defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_IA241_32128) || defined(CONFIG_MACH_KIXRP435)	// add by Victor Yu. 05-22-2007
	    struct pci_dev *pdev;
#endif
	    if(mxsercfg[i].board_type == -1)
	        continue;
            else{
	        free_irq(mxsercfg[i].irq, &mxvar_table[i*MXSER_PORTS_PER_BOARD]);
#if defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_IA241_32128) || defined(CONFIG_MACH_KIXRP435)	// add by Victor Yu. 05-22-2007
		pdev = mxsercfg[i].pciInfo.pdev;
		release_region(pci_resource_start(pdev, 2),
	       		pci_resource_len(pdev, 2));
		release_region(pci_resource_start(pdev, 3),
	       		pci_resource_len(pdev, 3));
#endif	// 11-22-2005
            }
        }
        
#if 1	// add by Victor Yu. 02-21-2006
	remove_proc_entry(MXSER_PROC_NODE, NULL);       // add by Victor Yu. 07-27-2004
#endif

	if (verbose)
		printk("Done.\n");
}

module_init(mxser_init_module);
module_exit(mxser_exit_module);

#if 1 // Begin --- add by Wade, 10-22-2010
/* RS232 0, RS485_2WIRE 1, RS422 2, RS485_4WIRE 3 */
static int mxser_set_opmode(struct mxser_struct *info, int opmode) {
	int p, shiftbit;
	unsigned int val, mask;

	p=info->port % 4;
	if ( opmode != RS232_MODE && opmode != RS485_2WIRE_MODE \
			&& opmode != RS422_MODE && opmode != RS485_4WIRE_MODE )
		return -1;

	/* Note: for UC-8400, please reference ioctl SET_OP_MODE */
	p = info->port % 4;
	shiftbit = p * 2;
	mask = (~(OP_MODE_MASK << shiftbit));
	val = inb(info->opmode_ioaddr);
	val &= mask;
	val |= (opmode << shiftbit);
	outb(val, info->opmode_ioaddr);
	
	return 0;
}
#endif // Begin --- add by Wade, 10-22-2010

int mxser_initbrd(int board,struct mxser_hwconf *hwconf)
{
	struct mxser_struct *	info;
	unsigned long	flags;
        int     retval;
	int	i,n;

	//init_MUTEX(&mxvar_tmp_buf_sem); 
	n = board*MXSER_PORTS_PER_BOARD;
	info = &mxvar_table[n];
	for ( i=0; i<hwconf->ports; i++, n++, info++ ) {
		if (verbose) {
			printk("        ttyM%d/cum%d at 0x%04x ", n, n, hwconf->ioaddr[i]);
			if ( hwconf->baud_base[i] == 115200 )
		    		printk(" max. baud rate up to 115200 bps.\n");
			else
		    		printk(" max. baud rate up to 921600 bps.\n");
		}
		info->port = n;
		info->base = hwconf->ioaddr[i];
		info->irq = hwconf->irq;
		info->vector = hwconf->vector;
		info->vectormask = hwconf->vector_mask;
		info->opmode_ioaddr = hwconf->opmode_ioaddr[i];	// add by Victor Yu. 01-05-2004
		
		// following add by Victor Yu. 08-30-2002
		// Moxa Must UART support FIFO is 64bytes for Tx/Rx
		// but receive FIFO just can set up to 62 will be OK.
#ifdef CONFIG_MACH_MOXA_IA261	// add by Victor Yu. 12-13-2007
		info->rx_trigger = 56;
#else
	#if 1//  Jared, 11-26-2010, set the rx fifo trigger level
		info->rx_trigger = rx_trigger;
	#else
		info->rx_trigger = 120;
	#endif
#endif
		info->baud_base = hwconf->baud_base[i];
		info->flags = ASYNC_SHARE_IRQ;
		info->type = hwconf->uart_type;

#if 1 // Begin --- add by Wade, 10-22-2010
		if ( interface != -1 ) {
			retval = mxser_set_opmode(info, interface);
			if( retval < 0 ) {
				printk("mxser_set_opmode: error\n");
				return -1;
			}
		}
#endif // End --- add by Wade, 10-22-2010

		// following add by Victor Yu. 08-30-2002
#ifdef CONFIG_MACH_MOXA_IA261	// add by Victor Yu. 12-13-2007
		info->xmit_fifo_size = 64;
#else
		info->xmit_fifo_size = 128;
#endif
		ENABLE_MOXA_MUST_ENCHANCE_MODE(info->base);
		info->MaxCanSetBaudRate = hwconf->MaxCanSetBaudRate[i];
		// above add by Victor Yu. 08-30-2002

		info->custom_divisor = hwconf->baud_base[i] * 16;
		info->close_delay = 5*HZ/10;
		info->closing_wait = 30*HZ;
		INIT_WORK(&info->tqueue, mxser_do_softint);
		info->normal_termios = mxvar_sdriver.init_termios;
		init_waitqueue_head(&info->open_wait);
		init_waitqueue_head(&info->close_wait);
		init_waitqueue_head(&info->delta_msr_wait);
		info->icount.rx = info->icount.tx = 0;
		info->icount.cts = info->icount.dsr = 
		    info->icount.dsr = info->icount.dcd = 0;
		info->icount.frame = info->icount.overrun = 
		    info->icount.brk = info->icount.parity = 0;
		info->icount.buf_overrun = 0;
#ifdef CONFIG_MOXA_SUPPORT_SPECIAL_BAUD_RATE	// add b Victor Yu. 08-12-2004
		info->speed = 38400;
#endif
	}

#if defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_IA241_32128) || defined(CONFIG_MACH_KIXRP435)	// add by Victor Yu. 05-22-2007
	request_region(hwconf->ioaddr[0],8*hwconf->ports,"mxser(io)");
	if ((hwconf->pciInfo.busNum == 0)&&(hwconf->pciInfo.devNum == 0))
		request_region(hwconf->vector,1,"mxser(vector)");
	else
		request_region(hwconf->vector,16,"mxser(vector)");
#endif

	/*
	 * Allocate the IRQ if necessary
	 */
	local_irq_save(flags);

	n = board*MXSER_PORTS_PER_BOARD;
	info = &mxvar_table[n];

        retval = request_irq(hwconf->irq, mxser_interrupt, IRQF_SHARED, "mxser", info);
	if ( retval ) {
	    local_irq_restore(flags);
	    printk("Board %d: %s", board, mxser_brdname[hwconf->board_type-1]);
	    printk("  Request irq fail,IRQ (%d) may be conflit with another device.\n",info->irq);
	    return(retval);
	}
	local_irq_restore(flags);

        return 0;
}

static void mxser_getcfg(int board,struct mxser_hwconf *hwconf)
{
	mxsercfg[board] = *hwconf;
}

#if defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_IA241_32128) || defined(CONFIG_MACH_KIXRP435)	// add by Victor Yu. 05-22-2007
static int mxser_get_PCI_conf(int board_type,struct mxser_hwconf *hwconf)
{
        int             i;
        unsigned int    ioaddress;
        struct pci_dev  *pdev=hwconf->pciInfo.pdev;

        hwconf->board_type = board_type;
        hwconf->ports = mxser_numports[board_type-1];
        ioaddress = pci_resource_start(pdev, 2);
        request_region(pci_resource_start(pdev, 2),
               pci_resource_len(pdev, 2),
               "mxser(IO)");
	for (i = 0; i < hwconf->ports; i++) {
                hwconf->ioaddr[i] = ioaddress + 8*i;

                // disable the interrupt
                outb(0, hwconf->ioaddr[i]+1);
                // flush FIFO
                outb(0x06, hwconf->ioaddr[i]+2);
                // flush interrupt
                inb(hwconf->ioaddr[i]+2);
                inb(hwconf->ioaddr[i]+6);
                inb(hwconf->ioaddr[i]+5);
                inb(hwconf->ioaddr[i]);
                hwconf->MaxCanSetBaudRate[i] = 921600;
        }

#if 1	// add by Victor Yu. 05-25-2006
	{
	unsigned int	_addr;
	for ( ; i<MXSER_PORTS_PER_BOARD; i++ ) {
		_addr = ioaddress + 8 * i;
		outb(0, _addr+1);
		outb(0x06, _addr+2);
		inb(_addr+2);
		inb(_addr+6);
		inb(_addr+5);
	}
	}
#endif
        ioaddress = pci_resource_start(pdev, 3);
        request_region(pci_resource_start(pdev, 3),
               pci_resource_len(pdev, 3),
               "mxser(vector)");
        hwconf->vector = ioaddress;

        // following add by Victor Yu. 01-05-2004
        for (i = 0; i < hwconf->ports; i++) {
		if ( i < 4 )
                        hwconf->opmode_ioaddr[i] = ioaddress + 4;
                else
                        hwconf->opmode_ioaddr[i] = ioaddress + 0x0c;
        }
        outb(0, ioaddress+4);   // default set to RS232 mode
        outb(0, ioaddress+0x0c); //default set to RS232 mode
        // above add by Victor Yu. 01-05-2004

        hwconf->irq = hwconf->pciInfo.pdev->irq;

	hwconf->uart_type = PORT_16550A;
        hwconf->vector_mask = 0;
        for (i = 0; i < hwconf->ports; i++) {
                hwconf->vector_mask |= (1<<i);
                hwconf->baud_base[i] = 921600;
        }
        return(0);
}
#endif

#if !defined(CONFIG_ARCH_IA241) && !defined(CONFIG_ARCH_IA241_32128) && !defined(CONFIG_MACH_KIXRP435)
static void ia240_hw_set(struct mxser_hwconf *hwconf)
{
	int	i;

	hwconf->board_type = IA240_BOARD;
	hwconf->ports = mxser_numports[IA240_BOARD-1];
#ifdef CONFIG_MACH_MOXA_IA261
	hwconf->irq = IRQ_EP93XX_EXT1;
	hwconf->vector = MOXA_IA261_UART_DIO_VIRT_BASE + 0x70;
#else
	hwconf->irq = IRQ_UART;
	hwconf->vector = CPE_UART_INT_VEC_VA_BASE;
#endif
#if defined(CONFIG_ARCH_W311)
	hwconf->vector_mask = 0x02;
#elif defined(CONFIG_ARCH_W315)
	hwconf->vector_mask = 0x04;
#elif defined(CONFIG_MACH_MOXA_IA261)
	hwconf->vector_mask = 0x0f;
#elif defined(CONFIG_ARCH_IA240) || defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345)
	hwconf->vector_mask = 0x3c;
#else
	hwconf->vector_mask = 0x0c;
#endif
	hwconf->uart_type = PORT_16550A;
	for ( i=0; i<hwconf->ports; i++ ) {
#if (defined CONFIG_ARCH_W311)
		hwconf->ioaddr[i] = CPE_UART2_VA_BASE + i * 32;
#elif defined(CONFIG_MACH_MOXA_IA261)
		hwconf->ioaddr[i] = MOXA_IA261_UART_DIO_VIRT_BASE + i * 16;
#else
		hwconf->ioaddr[i] = CPE_UART3_VA_BASE + i * 32;
#endif
		hwconf->baud_base[i] = 921600;
		hwconf->MaxCanSetBaudRate[i] = 921600;
#ifdef CONFIG_MACH_MOXA_IA261	// add by Victor Yu. 12-14-2007
		hwconf->opmode_ioaddr[i] = MOXA_IA261_UART_DIO_VIRT_BASE + 0x40;
#else
		hwconf->opmode_ioaddr[i] = CPE_UART_MODE_VA_BASE;
#endif
	}
#ifdef CONFIG_MACH_MOXA_IA261	// add by Victor Yu. 12-14-2007
	outb(keep_opmode_set_value, MOXA_IA261_UART_DIO_VIRT_BASE+0x40);
#endif
}
#endif

#if 1 // Jared, 09-01-2011
static const struct tty_operations mxser_ops = {
	.open = mxser_open,
	.close = mxser_close,
	.write = mxser_write,
	.put_char = mxser_put_char,
	.flush_chars = mxser_flush_chars,
	.write_room = mxser_write_room,
	.chars_in_buffer = mxser_chars_in_buffer,
	.flush_buffer = mxser_flush_buffer,
	.ioctl = mxser_ioctl,
	.throttle = mxser_throttle,
	.unthrottle = mxser_unthrottle,
	.set_termios = mxser_set_termios,
	.stop = mxser_stop,
	.start = mxser_start,
	.hangup = mxser_hangup,
	//.break_ctl = mxser_rs_break,
	.wait_until_sent = mxser_wait_until_sent,
	.tiocmget = mxser_tiocmget,
	.tiocmset = mxser_tiocmset,
	//.get_icount = mxser_get_icount,
};
#endif  // Jared, 09-01-2011

int mxser_init(void)
{
	int			i, m;
        int                     ret1, ret2;
#if defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_IA241_32128) || defined(CONFIG_MACH_KIXRP435)	// add by Victor Yu. 05-22-2007
	struct pci_dev		*pdev=NULL;
	int			n, index, retval, b;
#endif
	struct mxser_hwconf	hwconf;

	printk("MOXA MU860/MU150 UART Device Driver version %s\n",MXSER_VERSION);

	/* Initialize the tty_driver structure */
	memset(&mxvar_sdriver, 0, sizeof(struct tty_driver));
	mxvar_sdriver.magic = TTY_DRIVER_MAGIC;
	mxvar_sdriver.name = "ttyM";
	mxvar_sdriver.major = ttymajor;
	mxvar_sdriver.minor_start = 0;
	mxvar_sdriver.num = MXSER_PORTS + 1;
	mxvar_sdriver.type = TTY_DRIVER_TYPE_SERIAL;
	mxvar_sdriver.subtype = SERIAL_TYPE_NORMAL;
	mxvar_sdriver.init_termios = tty_std_termios;
	mxvar_sdriver.init_termios.c_cflag = B9600|CS8|CREAD|HUPCL|CLOCAL;
	mxvar_sdriver.flags = TTY_DRIVER_REAL_RAW;
	#if 0 // Jared, 09-01-2011
	mxvar_sdriver.refcount = 0;
	mxvar_sdriver.open = mxser_open;
	mxvar_sdriver.close = mxser_close;
	mxvar_sdriver.write = mxser_write;
	mxvar_sdriver.put_char = mxser_put_char;
	mxvar_sdriver.flush_chars = mxser_flush_chars;
	mxvar_sdriver.write_room = mxser_write_room;
	mxvar_sdriver.chars_in_buffer = mxser_chars_in_buffer;
	mxvar_sdriver.flush_buffer = mxser_flush_buffer;
	mxvar_sdriver.ioctl = mxser_ioctl;
	mxvar_sdriver.throttle = mxser_throttle;
	mxvar_sdriver.unthrottle = mxser_unthrottle;
	mxvar_sdriver.set_termios = mxser_set_termios;
	mxvar_sdriver.stop = mxser_stop;
	mxvar_sdriver.start = mxser_start;
	mxvar_sdriver.hangup = mxser_hangup;
	mxvar_sdriver.wait_until_sent = mxser_wait_until_sent;
	mxvar_sdriver.tiocmget = mxser_tiocmget;
	mxvar_sdriver.tiocmset = mxser_tiocmset;
	#endif
	mxvar_sdriver.termios = mxvar_termios;
	mxvar_sdriver.termios_locked = mxvar_termios_locked;

	printk("Tty devices major number = %d\n",ttymajor);

	mxvar_diagflag = 0;
	memset(mxvar_table, 0, MXSER_PORTS * sizeof(struct mxser_struct));
	memset(&mxvar_log, 0, sizeof(struct mxser_log));

	m = 0;
#if !defined(CONFIG_ARCH_IA241) && !defined(CONFIG_ARCH_IA241_32128) && !defined(CONFIG_MACH_KIXRP435)
	ia240_hw_set(&hwconf);
	if ( mxser_initbrd(m,&hwconf) < 0 )
		return -1;
	mxser_getcfg(m,&hwconf);
	m++;
#else
	{
                n = sizeof (mxser_pcibrds)/sizeof (mxser_pciinfo);
                index = 0;
                b = 0;
                while (b < n) {
	#if 1 // Jared, 09-01-2011
                       pdev = pci_get_device(mxser_pcibrds[b].vendor_id,
	#else
                       pdev = pci_find_device(mxser_pcibrds[b].vendor_id,
	#endif
                       mxser_pcibrds[b].device_id,
                       pdev);
			if ( pdev == NULL ) {
				b++;
				continue;
			}
                        hwconf.pciInfo.busNum = pdev->bus->number;
                        hwconf.pciInfo.devNum = pdev->devfn;
			hwconf.pciInfo.pdev = pdev;
                        printk("Found MOXA %s board(BusNo=%d,DevNo=%d)\n",mxser_brdname[mxser_pcibrds[b].board_type-1],pdev->bus->number,pdev->devfn >> 3);
                        if ( m >= MXSER_BOARDS) {
                                printk("Too many Smartio/Industio family boards find (maximum %d),board not configured\n",MXSER_BOARDS);
                        }
                        else {
                                if ( pci_enable_device(pdev) ) {
                                        printk("Moxa SmartI/O PCI enable fail !\n");
                                        continue;
                                }
                                retval = mxser_get_PCI_conf(mxser_pcibrds[b].board_type,&hwconf);
                                if (retval < 0) {
                                        if (retval == MXSER_ERR_IRQ)
                                                printk("Invalid interrupt number,board not configured\n");
                                        else if (retval == MXSER_ERR_IRQ_CONFLIT)
                                                printk("Invalid interrupt number,board not configured\n");
                                        else if (retval == MXSER_ERR_VECTOR)
						printk("Invalid interrupt vector,board not configured\n");
                                        else if (retval == MXSER_ERR_IOADDR)
                                                printk("Invalid I/O address,board not configured\n");
                                        continue;

                                }

                                if(mxser_initbrd(m,&hwconf)<0)
                                    continue;
                                mxser_getcfg(m,&hwconf);
                                m++;
                        }
                }
        }
#endif

	for(i=m; i<MXSER_BOARDS; i++){
		mxsercfg[i].board_type = -1;
	}
	

        ret1 = 0;
        ret2 = 0;
        if ( !(ret1=tty_register_driver(&mxvar_sdriver)) ){
#if 1	// add by Victor Yu. 02-21-2006
		create_proc_read_entry(MXSER_PROC_NODE, 0, 0, mxser_read_proc, NULL); // add by Victor Yu. 07-27-2004
#endif
                return 0;
        }else
	    printk("Couldn't install MOXA Smartio/Industio family driver !\n");


        if(ret1 || ret2){
            for(i=0; i<MXSER_BOARDS; i++){
	        if(mxsercfg[i].board_type == -1)
	            continue;
                else{
		    free_irq(mxsercfg[i].irq, &mxvar_table[i*MXSER_PORTS_PER_BOARD]);
                }
            }
            return -1;
        }
        
	return(0);
}

static void mxser_do_softint(struct work_struct *work)
{
	struct mxser_struct *	info=container_of(work, struct mxser_struct, tqueue);
	struct tty_struct *	tty;

	tty = info->tty;

	if (tty) {
	    if ( test_and_clear_bit(MXSER_EVENT_TXLOW, &info->event) ) {
	        if ( (tty->flags & (1 << TTY_DO_WRITE_WAKEUP)) &&
		 tty->ldisc.write_wakeup )
		(tty->ldisc.write_wakeup)(tty);
	        wake_up_interruptible(&tty->write_wait);
	    }
	    if ( test_and_clear_bit(MXSER_EVENT_HANGUP, &info->event) ) {
		tty_hangup(tty);
	    }
	}
}

static int	mxser_tiocmget(struct tty_struct *tty, struct file *file)
{
	struct mxser_struct *	info = (struct mxser_struct *)tty->driver_data;
	unsigned char		control, status;
	int			result;
	unsigned long		flags;

	local_irq_save(flags);
	control = info->MCR;
	status = inb(info->base + UART_MSR);
	local_irq_restore(flags);
	result = ((control & UART_MCR_RTS) ? TIOCM_RTS : 0) |
		 ((control & UART_MCR_DTR) ? TIOCM_DTR : 0) |
		 ((status  & UART_MSR_DCD) ? TIOCM_CAR : 0) |
		 ((status  & UART_MSR_RI)  ? TIOCM_RNG : 0) |
		 ((status  & UART_MSR_DSR) ? TIOCM_DSR : 0) |
		 ((status  & UART_MSR_CTS) ? TIOCM_CTS : 0);
	return(result);
}

static int	mxser_tiocmset(struct tty_struct *tty, struct file *file, unsigned int set, unsigned int clear)
{
	struct mxser_struct *	info = (struct mxser_struct *)tty->driver_data;
	unsigned long		flags;

	local_irq_save(flags);
	if ( set & TIOCM_RTS )
		info->MCR |= UART_MCR_RTS;
	if ( set & TIOCM_DTR )
		info->MCR |= UART_MCR_DTR;
	if ( clear & TIOCM_RTS )
		info->MCR &= ~UART_MCR_RTS;
	if ( clear & TIOCM_DTR )
		info->MCR &= ~UART_MCR_DTR;
	outb(info->MCR, info->base + UART_MCR);
	local_irq_restore(flags);
	return(0);
}

/*
 * This routine is called whenever a serial port is opened.  It
 * enables interrupts for a serial port, linking in its async structure into
 * the IRQ chain.   It also performs the serial-specific
 * initialization for the tty structure.
 */
static int mxser_open(struct tty_struct * tty, struct file * filp)
{
	struct mxser_struct *	info;
	int			retval, line;
	unsigned long		page;
#if 1	// add by Victor Yu. 12-28-2004
	unsigned long		flags;
#endif

	line = PORTNO(tty);
	if ( line == MXSER_PORTS )
	    return(0);
	if ( (line < 0) || (line > MXSER_PORTS) )
	    return(-ENODEV);

	info = mxvar_table + line;
	if ( !info->base )
	    return(-ENODEV);

	tty->driver_data = info;
	local_irq_save(flags);
	info->tty = tty;
	local_irq_restore(flags);

	if ( !mxvar_tmp_buf ) {
	    page = get_zeroed_page(GFP_KERNEL);
	    if ( !page )
		return(-ENOMEM);
	    if ( mxvar_tmp_buf )
		free_page(page);
	    else
		mxvar_tmp_buf = (unsigned char *)page;
	}

	/*
	 * Start up serial port
	 */
	retval = mxser_startup(info);
	if ( retval )
	    return(retval);

	retval = mxser_block_til_ready(tty, filp, info);
	if ( retval )
	    return(retval);

	local_irq_save(flags);
	info->count++;
	local_irq_restore(flags);

	if ( (info->count == 1) && (info->flags & ASYNC_SPLIT_TERMIOS) ) {
	    *tty->termios = info->normal_termios;
	    mxser_change_speed(info, 0);
	}

	tty->low_latency = 1;
	return(0);
}

/*
 * This routine is called when the serial port gets closed.  First, we
 * wait for the last remaining data to be sent.  Then, we unlink its
 * async structure from the interrupt chain if necessary, and we free
 * that IRQ if nothing is left in the chain.
 */
static void mxser_close(struct tty_struct * tty, struct file * filp)
{
	struct mxser_struct * info = (struct mxser_struct *)tty->driver_data;
	unsigned long	flags;
	unsigned long	timeout;

	if ( PORTNO(tty) == MXSER_PORTS )
	    return;
	if ( !info )
	    return;

	if ( tty_hung_up_p(filp) ) {
	    return;
	}

#if 1	// add by Victor Yu. 12-28-2004
	local_irq_save(flags);
#endif
	if ( (tty->count == 1) && (info->count != 1) ) {
	    /*
	     * Uh, oh.	tty->count is 1, which means that the tty
	     * structure will be freed.  Info->count should always
	     * be one in these conditions.  If it's greater than
	     * one, we've got real problems, since it means the
	     * serial port won't be shutdown.
	     */
	    printk("mxser_close: bad serial port count; tty->count is 1, "
		   "info->count is %d\n", info->count);
	    info->count = 1;
	}
	if ( --info->count < 0 ) {
	    printk("mxser_close: bad serial port count for ttys%d: %d\n",
		   info->port, info->count);
	    info->count = 0;
	}
	if ( info->count ) {
	    local_irq_restore(flags);
	    return;
	}
	info->flags |= ASYNC_CLOSING;
	local_irq_restore(flags);	// add by Victor Yu. 09-26-2002

	/*
	 * Save the termios structure, since this port may have
	 * separate termios for callout and dialin.
	 */
	if ( info->flags & ASYNC_NORMAL_ACTIVE )
	    info->normal_termios = *tty->termios;
	/*
	 * Now we wait for the transmit buffer to clear; and we notify
	 * the line discipline to only process XON/XOFF characters.
	 */
	tty->closing = 1;
	if ( info->closing_wait != ASYNC_CLOSING_WAIT_NONE ) {
	    tty_wait_until_sent(tty, info->closing_wait);
	}
	/*
	 * At this point we stop accepting input.  To do this, we
	 * disable the receive line status interrupts, and tell the
	 * interrupt driver to stop checking the data ready bit in the
	 * line status register.
	 */
	info->IER &= ~(UART_IER_RLSI|MOXA_MUST_RECV_ISR);
	if ( info->flags & ASYNC_INITIALIZED ) {
	    outb(info->IER, info->base + UART_IER);
	    /*
	     * Before we drop DTR, make sure the UART transmitter
	     * has completely drained; this is especially
	     * important if there is a transmit FIFO!
	     */
	    timeout = jiffies + HZ;
	    while ( !(inb(info->base + UART_LSR) & UART_LSR_TEMT) ) {
		current->state = TASK_INTERRUPTIBLE;
		schedule_timeout(5);
		if ( time_after(jiffies, timeout) )
		    break;
	    }
	}
	mxser_shutdown(info);

	if ( tty->driver->flush_buffer )
	    tty->driver->flush_buffer(tty);
	if ( tty->ldisc.flush_buffer )
	    tty->ldisc.flush_buffer(tty);
	tty->closing = 0;
	info->event = 0;
	info->tty = 0;
	if ( info->blocked_open ) {
	    if ( info->close_delay ) {
		current->state = TASK_INTERRUPTIBLE;
		schedule_timeout(info->close_delay);
	    }
	    wake_up_interruptible(&info->open_wait);
	}

	info->flags &= ~(ASYNC_NORMAL_ACTIVE |
			 ASYNC_CLOSING);
	wake_up_interruptible(&info->close_wait);
	#if 1 // Jared, 09-01-2011
	/* Right now the tty_port set is done outside of the close_end helper
	 * as we don't yet have everyone using refcounts */
	tty_port_close_end(info->port, tty);
	tty_port_tty_set(info->port, NULL);
	#endif
}

static int mxser_write(struct tty_struct * tty, const unsigned char * buf, int count)
{
	int		c, total = 0;
	struct mxser_struct *info = (struct mxser_struct *)tty->driver_data;
	unsigned long	flags;

	if ( !tty || !info->xmit_buf || !mxvar_tmp_buf )
	    return(0);

	local_irq_save(flags);
	while ( 1 ) {
	    c = MIN(count, MIN(SERIAL_XMIT_SIZE - info->xmit_cnt - 1,
			       SERIAL_XMIT_SIZE - info->xmit_head));
	    if ( c <= 0 )
		break;

	    memcpy(info->xmit_buf + info->xmit_head, buf, c);
	    info->xmit_head = (info->xmit_head + c) & (SERIAL_XMIT_SIZE - 1);
	    info->xmit_cnt += c;
	    buf += c;
	    count -= c;
	    total += c;
	}

	if ( info->xmit_cnt && !tty->stopped && 
	             !(info->IER & UART_IER_THRI) ) {
        	if (!tty->hw_stopped||(info->type == PORT_16550A)) {
	            info->IER |= UART_IER_THRI;
	            outb(info->IER, info->base + UART_IER);
	        }
	}
	local_irq_restore(flags);
	return(total);
}

#if 1 // Jared, 09-01-2011
static int mxser_put_char(struct tty_struct * tty, unsigned char ch)
#else
static void mxser_put_char(struct tty_struct * tty, unsigned char ch)
#endif
{
	struct mxser_struct *info = (struct mxser_struct *)tty->driver_data;
	unsigned long	flags;

	if ( !tty || !info->xmit_buf )
	    return;

	local_irq_save(flags);
	if ( info->xmit_cnt >= SERIAL_XMIT_SIZE - 1 ) {
	    local_irq_restore(flags);
	    return;
	}

	info->xmit_buf[info->xmit_head++] = ch;
	info->xmit_head &= SERIAL_XMIT_SIZE - 1;
	info->xmit_cnt++;
	if ( !tty->stopped && !(info->IER & UART_IER_THRI) ) {
        	if (!tty->hw_stopped||(info->type == PORT_16550A)) {
	    		info->IER |= UART_IER_THRI;
	    		outb(info->IER, info->base + UART_IER);
		}
	}
	local_irq_restore(flags);
}

static void mxser_flush_chars(struct tty_struct * tty)
{
	struct mxser_struct *info = (struct mxser_struct *)tty->driver_data;
	unsigned long	flags;

	if ( info->xmit_cnt <= 0 || tty->stopped || !info->xmit_buf || 
	     (tty->hw_stopped && info->type!=PORT_16550A))
	    return;

	local_irq_save(flags);
	info->IER |= UART_IER_THRI;
	outb(info->IER, info->base + UART_IER);
	local_irq_restore(flags);
}

static int mxser_write_room(struct tty_struct * tty)
{
	struct mxser_struct *info = (struct mxser_struct *)tty->driver_data;
	int	ret;

	ret = SERIAL_XMIT_SIZE - info->xmit_cnt - 1;
	if ( ret < 0 )
	    ret = 0;
	return(ret);
}

static int mxser_chars_in_buffer(struct tty_struct * tty)
{
	struct mxser_struct *info = (struct mxser_struct *)tty->driver_data;

	return(info->xmit_cnt);
}

static void mxser_flush_buffer(struct tty_struct * tty)
{
	struct mxser_struct *info = (struct mxser_struct *)tty->driver_data;
	unsigned long flags;

	local_irq_save(flags);
	info->xmit_cnt = info->xmit_head = info->xmit_tail = 0;
	local_irq_restore(flags);
	wake_up_interruptible(&tty->write_wait);
	if ( (tty->flags & (1 << TTY_DO_WRITE_WAKEUP)) &&
	     tty->ldisc.write_wakeup )
	    (tty->ldisc.write_wakeup)(tty);
}

static int mxser_ioctl(struct tty_struct * tty, struct file * file,
		       unsigned int cmd, unsigned long arg)
{
	unsigned long	flags;
	struct mxser_struct *	info = (struct mxser_struct *)tty->driver_data;
	int			retval;
	struct async_icount	cprev, cnow;	    /* kernel counter temps */
	struct serial_icounter_struct *p_cuser;     /* user space */
	unsigned long 		templ;

	if ( PORTNO(tty) == MXSER_PORTS )
	    return(mxser_ioctl_special(cmd, arg));
	
	// following add by Victor Yu. 01-05-2004
	if ( cmd == MOXA_SET_OP_MODE || cmd == MOXA_GET_OP_MODE ) {
		int		opmode, p;
		int             shiftbit;
		unsigned int	val;

		p = info->port % 4;
		if ( cmd == MOXA_SET_OP_MODE ) {
	    		get_from_user(opmode,(int *)arg);
			if ( opmode != RS232_MODE && opmode != RS485_2WIRE_MODE && opmode != RS422_MODE && opmode != RS485_4WIRE_MODE )
				return -EFAULT;
#if (defined CONFIG_ARCH_W311)
			shiftbit = (p+1) * 2;
			val = inl(info->opmode_ioaddr);
#elif (defined CONFIG_MACH_MOXA_IA261)
			shiftbit = p * 2;
			val = keep_opmode;
#elif defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_IA241_32128) || defined(CONFIG_MACH_KIXRP435)
			shiftbit = p * 2;
			val = inb(info->opmode_ioaddr);
#else
			shiftbit = (p+2) * 2;
			val = inl(info->opmode_ioaddr);
#endif
			val &= (~(OP_MODE_MASK << shiftbit));
			val |= (opmode << shiftbit);
#ifdef CONFIG_MACH_MOXA_IA261	// add by Victor Yu. 12-14-2007
			keep_opmode = val;
			keep_opmode_set_value &= (~(OP_MODE_MASK << shiftbit));
			switch ( opmode ) {
			case RS232_MODE :
				keep_opmode_set_value |= (RS232_SET_VALUE << shiftbit);
				break;
			case RS485_2WIRE_MODE :
				keep_opmode_set_value |= (RS485_2WIRE_SET_VALUE << shiftbit);
				break;
			default :
				keep_opmode_set_value |= (RS485_4WIRE_SET_VALUE << shiftbit);
				break;
			}
#endif
#if !defined(CONFIG_ARCH_IA241) && !defined(CONFIG_ARCH_IA241_32128) && !defined(CONFIG_MACH_KIXRP435)
			outl(val, info->opmode_ioaddr);
#elif defined(CONFIG_MACH_MOXA_IA261)	// add by Victor Yu. 12-14-2007
			outb(keep_opmode_set_value, info->opmode_ioaddr);
#else
			outb(val, info->opmode_ioaddr);
#endif
		} else {
#if (defined CONFIG_ARCH_W311)
			shiftbit = (p+1) * 2;
			opmode = inl(info->opmode_ioaddr) >> shiftbit;
#elif (defined CONFIG_MACH_MOXA_IA261)
			shiftbit = p * 2;
			opmode = keep_opmode >> shiftbit;
#elif defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_IA241_32128) || defined(CONFIG_MACH_MOXA_IA261) || defined(CONFIG_MACH_KIXRP435)
			shiftbit = p * 2;
			opmode = inb(info->opmode_ioaddr) >> shiftbit;
#else
			shiftbit = (p+2) * 2;
			opmode = inl(info->opmode_ioaddr) >> shiftbit;
#endif
			opmode &= OP_MODE_MASK;
	    		copy_to_user((int*)arg, &opmode, sizeof(int));
		}
		return 0;
	}
	// above add by Victor Yu. 01-05-2004

#ifdef CONFIG_MOXA_SUPPORT_SPECIAL_BAUD_RATE	// add by Victor Yu. 08-12-2004
	if ( cmd == MOXA_SET_SPECIAL_BAUD_RATE || cmd == MOXA_GET_SPECIAL_BAUD_RATE ||
	     cmd == UC_SET_SPECIAL_BAUD_RATE || cmd == UC_GET_SPECIAL_BAUD_RATE ) {
		int	speed, i;
		if ( cmd == MOXA_SET_SPECIAL_BAUD_RATE ||
		     cmd == UC_SET_SPECIAL_BAUD_RATE ) {
	    		get_from_user(speed,(int *)arg);
			if ( speed <= 0 || speed > info->MaxCanSetBaudRate )
				return -EFAULT;
			if ( !info->tty || !info->tty->termios || !info->base )
				return 0;
			info->tty->termios->c_cflag &= ~(CBAUD | CBAUDEX);
			for ( i=0; i<BAUD_TABLE_NO && speed != mxvar_baud_table[i]; i++ );
			if ( i == BAUD_TABLE_NO ) {
				info->tty->termios->c_cflag |= B4000000;
			} else {
				switch ( mxvar_baud_table[i] ) {
				case 921600 : info->tty->termios->c_cflag |= B921600; break;
				case 460800 : info->tty->termios->c_cflag |= B460800; break;
				case 230400 : info->tty->termios->c_cflag |= B230400; break;
				case 115200 : info->tty->termios->c_cflag |= B115200; break;
				case 57600 : info->tty->termios->c_cflag |= B57600; break;
				case 38400 : info->tty->termios->c_cflag |= B38400; break;
				case 19200 : info->tty->termios->c_cflag |= B19200; break;
				case 9600 : info->tty->termios->c_cflag |= B9600; break;
				case 4800 : info->tty->termios->c_cflag |= B4800; break;
				case 2400 : info->tty->termios->c_cflag |= B2400; break;
				case 1800 : info->tty->termios->c_cflag |= B1800; break;
				case 1200 : info->tty->termios->c_cflag |= B1200; break;
				case 600 : info->tty->termios->c_cflag |= B600; break;
				case 300 : info->tty->termios->c_cflag |= B300; break;
				case 200 : info->tty->termios->c_cflag |= B200; break;
				case 150 : info->tty->termios->c_cflag |= B150; break;
				case 134 : info->tty->termios->c_cflag |= B134; break;
				case 110 : info->tty->termios->c_cflag |= B110; break;
				case 75 : info->tty->termios->c_cflag |= B75; break;
				case 50 : info->tty->termios->c_cflag |= B50; break;
				}
			}
			info->speed = speed;
			mxser_change_speed(info, 0);
		} else {
	    		copy_to_user((int*)arg, &info->speed, sizeof(int));
		}
		return 0;
	}
#endif

	if ( (cmd != TIOCGSERIAL) && (cmd != TIOCMIWAIT) &&
	     (cmd != TIOCGICOUNT) ) {
	    if ( tty->flags & (1 << TTY_IO_ERROR) )
		return(-EIO);
	}
	switch ( cmd ) {
#if 1   // add by Victor Yu. 05-08-2007, for GL ODM, I let it to be Moxa standard feature
	case MOXA_BREAK_TIMER :
		retval = tty_check_change(tty);
		if ( retval )
			return(retval);
		tty_wait_until_sent(tty, 0);
		local_irq_save(flags);
		outb(inb(info->base + UART_LCR) | UART_LCR_SBC, info->base + UART_LCR);
		udelay(arg);
		outb(inb(info->base + UART_LCR) & ~UART_LCR_SBC, info->base + UART_LCR);
		local_irq_restore(flags);
		break;
#endif
#if 1	// add by Victor Yu. 12-28-2004
	case TIOCSBRK :	// start to send break
	    retval = tty_check_change(tty);
	    if ( retval )
		return(retval);
	    tty_wait_until_sent(tty, 0);
	    local_irq_save(flags);
	    outb(inb(info->base + UART_LCR) | UART_LCR_SBC, info->base + UART_LCR);
	    local_irq_restore(flags);
	    return 0;
	case TIOCCBRK :	// stop to send break
	    local_irq_save(flags);
	    outb(inb(info->base + UART_LCR) & ~UART_LCR_SBC, info->base + UART_LCR);
	    local_irq_restore(flags);
	    return 0;
#endif
	case TCSBRK:	/* SVID version: non-zero arg --> no break */
	    retval = tty_check_change(tty);
	    if ( retval )
		return(retval);
	    tty_wait_until_sent(tty, 0);
	    if ( !arg )
		mxser_send_break(info, HZ/4);		/* 1/4 second */
	    return(0);
	case TCSBRKP:	/* support for POSIX tcsendbreak() */
	    retval = tty_check_change(tty);
	    if ( retval )
		return(retval);
	    tty_wait_until_sent(tty, 0);
	    mxser_send_break(info, arg ? arg*(HZ/10) : HZ/4);
	    return(0);
	case TIOCGSOFTCAR:
	    put_to_user(C_CLOCAL(tty) ? 1 : 0, (unsigned long *)arg);
	    return 0;
	case TIOCSSOFTCAR:
	    get_from_user(templ,(unsigned long *)arg);
	    arg = templ;
	    tty->termios->c_cflag = ((tty->termios->c_cflag & ~CLOCAL) |
				    (arg ? CLOCAL : 0));
	    return(0);
	case TIOCGSERIAL:
	    return(mxser_get_serial_info(info, (struct serial_struct *)arg));
	case TIOCSSERIAL:
	    return(mxser_set_serial_info(info, (struct serial_struct *)arg));
	case TIOCSERGETLSR: /* Get line status register */
	    return(mxser_get_lsr_info(info, (unsigned int *)arg));
	/*
	 * Wait for any of the 4 modem inputs (DCD,RI,DSR,CTS) to change
	 * - mask passed in arg for lines of interest
	 *   (use |'ed TIOCM_RNG/DSR/CD/CTS for masking)
	 * Caller should use TIOCGICOUNT to see which one it was
	 */
	case TIOCMIWAIT:
	    local_irq_save(flags);
	    cprev = info->icount;   /* note the counters on entry */
	    local_irq_restore(flags);
	    while ( 1 ) {
		interruptible_sleep_on(&info->delta_msr_wait);
		/* see if a signal did it */
		if ( signal_pending(current) ) {
		    return(-ERESTARTSYS);
		}
	        local_irq_save(flags);
		cnow = info->icount;	/* atomic copy */
	        local_irq_restore(flags);
#define TIOCM_BRK	0x10000
#define TIOCM_PARITY	0x20000
#define TIOCM_FRAME	0x40000
#define TIOCM_XOFFHOLD	0x80000
		if ( cnow.rng == cprev.rng && cnow.dsr == cprev.dsr &&
		     cnow.dcd == cprev.dcd && cnow.cts == cprev.cts &&
		     cnow.brk == cprev.brk && cnow.parity == cprev.parity &&
		     cnow.frame == cprev.frame && cnow.buf_overrun == cprev.buf_overrun )
		    return(-EIO);	/* no change => error */
		if ( ((arg & TIOCM_RNG) && (cnow.rng != cprev.rng)) ||
		     ((arg & TIOCM_DSR) && (cnow.dsr != cprev.dsr)) ||
		     ((arg & TIOCM_CD)	&& (cnow.dcd != cprev.dcd)) ||
		     ((arg & TIOCM_CTS) && (cnow.cts != cprev.cts)) ||
		     ((arg & TIOCM_BRK) && (cnow.brk != cprev.brk)) ||
		     ((arg & TIOCM_PARITY) && (cnow.parity != cprev.parity)) ||
		     ((arg & TIOCM_FRAME) && (cnow.frame != cprev.frame)) ||
		     ((arg & TIOCM_XOFFHOLD) && (cnow.buf_overrun != cprev.buf_overrun)) ) {
		    return(0);
		}
		cprev = cnow;
	    }
	    /* NOTREACHED */
	/*
	 * Get counter of input serial line interrupts (DCD,RI,DSR,CTS)
	 * Return: write counters to the user passed counter struct
	 * NB: both 1->0 and 0->1 transitions are counted except for
	 *     RI where only 0->1 is counted.
	 */

#if 1	// add by Victor Yu. 01-26-2005
	case MOXA_UNWAIT :
	    wake_up_interruptible(&info->delta_msr_wait);
	    break;
#endif

	case TIOCGICOUNT:
	    local_irq_save(flags);
	    cnow = info->icount;
	    local_irq_restore(flags);
	    p_cuser = (struct serial_icounter_struct *)arg;
	    if (put_user(cnow.frame, &p_cuser->frame))
		return -EFAULT;
	    if (put_user(cnow.brk, &p_cuser->brk))
		return -EFAULT;
	    if (put_user(cnow.overrun, &p_cuser->overrun))
		return -EFAULT;
	    if (put_user(cnow.buf_overrun, &p_cuser->buf_overrun))
		return -EFAULT;
	    if (put_user(cnow.parity, &p_cuser->parity))
		return -EFAULT;
	    if (put_user(cnow.rx, &p_cuser->rx))
		return -EFAULT;
	    if (put_user(cnow.tx, &p_cuser->tx))
		return -EFAULT;

	    put_to_user(cnow.cts, &p_cuser->cts);
	    put_to_user(cnow.dsr, &p_cuser->dsr);
	    put_to_user(cnow.rng, &p_cuser->rng);
	    put_to_user(cnow.dcd, &p_cuser->dcd);

	    return(0);
	case MOXA_HighSpeedOn:
	    put_to_user(info->baud_base != 115200 ? 1 : 0, (int *)arg);
	    return(0);
#if 1   // add by Victor Yu. 09-17-2008
	case MOXA_DISABLE_FIFO :
		save_flags(flags);
		cli();
		info->rx_trigger = 1;
		info->xmit_fifo_size = 1;
		SET_MOXA_MUST_FIFO_VALUE(info);
		restore_flags(flags);
		break;
	case MOXA_ENABLE_FIFO :
		save_flags(flags);
		cli();
		if ( info->IsMoxaMustChipFlag == MOXA_MUST_HARDWARE_ID ) {
			info->rx_trigger = 56;
			info->xmit_fifo_size = 64;
		} else {
			info->rx_trigger = 120;
			info->xmit_fifo_size = 128;
		}
		SET_MOXA_MUST_FIFO_VALUE(info);
		restore_flags(flags);
		break;
#endif
	default:
	    return(-ENOIOCTLCMD);
	}
	return(0);
}

static int mxser_ioctl_special(unsigned int cmd, unsigned long arg)
{
	int	i, result, status;

	switch ( cmd ) {
	case MOXA_GET_CONF:
	    copy_to_user((struct mxser_hwconf *)arg, mxsercfg, 
			    sizeof(struct mxser_hwconf)*4);
	    return 0;
        case MOXA_GET_MAJOR:
	    copy_to_user((int*)arg, &ttymajor, sizeof(int));
            return 0;

        case MOXA_GET_CUMAJOR:
	    copy_to_user((int*)arg, &calloutmajor, sizeof(int));
            return 0;

	case MOXA_CHKPORTENABLE:
	    result = 0;
	    for ( i=0; i<MXSER_PORTS; i++ ) {
		if ( mxvar_table[i].base )
		    result |= (1 << i);
	    }
	    put_to_user(result, (unsigned long *)arg);
	    return(0);
	case MOXA_GETDATACOUNT:
	    copy_to_user((struct mxser_log *)arg, &mxvar_log, sizeof(mxvar_log));
	    return(0);
        case MOXA_GETMSTATUS:
            for(i=0; i<MXSER_PORTS; i++){
                GMStatus[i].ri = 0;
		if ( !mxvar_table[i].base ){
                    GMStatus[i].dcd = 0;
                    GMStatus[i].dsr = 0;
                    GMStatus[i].cts = 0;
		    continue;
                }

	        if ( !mxvar_table[i].tty || !mxvar_table[i].tty->termios )
                    GMStatus[i].cflag=mxvar_table[i].normal_termios.c_cflag;
                else
                    GMStatus[i].cflag = mxvar_table[i].tty->termios->c_cflag;
                
                status = inb(mxvar_table[i].base + UART_MSR);
                if(status  & 0x80/*UART_MSR_DCD*/)
                    GMStatus[i].dcd = 1;
                else
                    GMStatus[i].dcd = 0;

                if(status  & 0x20/*UART_MSR_DSR*/)
                    GMStatus[i].dsr = 1;
                else
                    GMStatus[i].dsr = 0;
                

                if(status  & 0x10/*UART_MSR_CTS*/)
                    GMStatus[i].cts = 1;
                else
                    GMStatus[i].cts = 0;
            }
            copy_to_user((struct mxser_mstatus *)arg, GMStatus,
                                    sizeof(struct mxser_mstatus) * MXSER_PORTS);
            return 0;
	default:
	    return(-ENOIOCTLCMD);
	}
	return(0);
}

/*
 * This routine is called by the upper-layer tty layer to signal that
 * incoming characters should be throttled.
 */
static void mxser_throttle(struct tty_struct * tty)
{
	struct mxser_struct *info = (struct mxser_struct *)tty->driver_data;
	unsigned long	flags;

	if ( I_IXOFF(tty) || (info->tty->termios->c_cflag & CRTSCTS) ) {
	    local_irq_save(flags);
	    info->IER &= ~MOXA_MUST_RECV_ISR;
	    outb(info->IER, info->base+UART_IER);
	    local_irq_restore(flags);
	}
}

static void mxser_unthrottle(struct tty_struct * tty)
{
	struct mxser_struct *info = (struct mxser_struct *)tty->driver_data;
	unsigned long	flags;

	if ( I_IXOFF(tty) ) {
	    if ( info->x_char )
		info->x_char = 0;
	    else {
		local_irq_save(flags);
		info->IER |= MOXA_MUST_RECV_ISR;
		outb(info->IER, info->base+UART_IER);
		local_irq_restore(flags);
	    }
	}

	if ( info->tty->termios->c_cflag & CRTSCTS ) {
	    local_irq_save(flags);
	    info->IER |= MOXA_MUST_RECV_ISR;
	    outb(info->IER, info->base+UART_IER);
	    local_irq_restore(flags);
	}
}

static void mxser_set_termios(struct tty_struct * tty,
			      struct ktermios * old_termios)
{
	struct mxser_struct *info = (struct mxser_struct *)tty->driver_data;

	if ( (tty->termios->c_cflag != old_termios->c_cflag) ||
	     (tty->termios->c_iflag != old_termios->c_iflag) ) {
		mxser_change_speed(info, old_termios);

		if ( (old_termios->c_cflag & CRTSCTS) &&
	     	     !(tty->termios->c_cflag & CRTSCTS) ) {
	    		tty->hw_stopped = 0;
	    		mxser_start(tty);
		}
	}
	
	/* Handle sw stopped */
	if ( (old_termios->c_iflag & IXON) &&
     	     !(tty->termios->c_iflag & IXON) ) {
    		tty->stopped = 0;
    		mxser_start(tty);
	}
}

/*
 * mxser_stop() and mxser_start()
 *
 * This routines are called before setting or resetting tty->stopped.
 * They enable or disable transmitter interrupts, as necessary.
 */
static void mxser_stop(struct tty_struct * tty)
{
	struct mxser_struct *info = (struct mxser_struct *)tty->driver_data;
	unsigned long	flags;

	local_irq_save(flags);
	if ( info->IER & UART_IER_THRI ) {
	    info->IER &= ~UART_IER_THRI;
	    outb(info->IER, info->base + UART_IER);
	}
	local_irq_restore(flags);
}

static void mxser_start(struct tty_struct * tty)
{
	struct mxser_struct *info = (struct mxser_struct *)tty->driver_data;
	unsigned long	flags;

	local_irq_save(flags);
	if ( info->xmit_cnt && info->xmit_buf &&
	     !(info->IER & UART_IER_THRI) ) {
	    info->IER |= UART_IER_THRI;
	    outb(info->IER, info->base + UART_IER);
	}
	local_irq_restore(flags);
}

/*
 * mxser_wait_until_sent() --- wait until the transmitter is empty
 */
static void mxser_wait_until_sent(struct tty_struct *tty, int timeout)
{
	struct mxser_struct * info = (struct mxser_struct *)tty->driver_data;
	unsigned long orig_jiffies, char_time;
	int lsr;
	
	if (info->type == PORT_UNKNOWN)
		return;

	if (info->xmit_fifo_size == 0)
		return; /* Just in case.... */

	/*
	 * Set the check interval to be 1/5 of the estimated time to
	 * send a single character, and make it at least 1.  The check
	 * interval should also be less than the timeout.
	 * 
	 * Note: we have to use pretty tight timings here to satisfy
	 * the NIST-PCTS.
	 */
	char_time = (info->timeout - HZ/50) / info->xmit_fifo_size;
	char_time = char_time / 5;
	if (char_time == 0)
		char_time = 1;
	if (timeout && timeout < char_time)
		char_time = timeout;
	/*
	 * If the transmitter hasn't cleared in twice the approximate
	 * amount of time to send the entire FIFO, it probably won't
	 * ever clear.  This assumes the UART isn't doing flow
	 * control, which is currently the case.  Hence, if it ever
	 * takes longer than info->timeout, this is probably due to a
	 * UART bug of some kind.  So, we clamp the timeout parameter at
	 * 2*info->timeout.
	 */
	if (!timeout || timeout > 2*info->timeout)
		timeout = 2*info->timeout;
#ifdef SERIAL_DEBUG_RS_WAIT_UNTIL_SENT
	printk("In rs_wait_until_sent(%d) check=%lu...", timeout, char_time);
	printk("jiff=%lu...", jiffies);
#endif
	orig_jiffies = jiffies + timeout;
	while (!((lsr = inb(info->base+ UART_LSR)) & UART_LSR_TEMT)) {
#ifdef SERIAL_DEBUG_RS_WAIT_UNTIL_SENT
		printk("lsr = %d (jiff=%lu)...", lsr, jiffies);
#endif
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(char_time);
		if (signal_pending(current))
			break;
		if (timeout && time_after(jiffies, orig_jiffies))
			break;
	}
		set_current_state(TASK_RUNNING);

#ifdef SERIAL_DEBUG_RS_WAIT_UNTIL_SENT
	printk("lsr = %d (jiff=%lu)...done\n", lsr, jiffies);
#endif
}


/*
 * This routine is called by tty_hangup() when a hangup is signaled.
 */
void mxser_hangup(struct tty_struct * tty)
{
	struct mxser_struct * info = (struct mxser_struct *)tty->driver_data;
#if 1	// add by Victor Yu. 12-28-2004
	unsigned long	flags;
#endif

	mxser_flush_buffer(tty);
	mxser_shutdown(info);
	local_irq_save(flags);
	info->event = 0;
	info->count = 0;
	info->flags &= ~(ASYNC_NORMAL_ACTIVE);
	info->tty = 0;
	local_irq_restore(flags);
	wake_up_interruptible(&info->open_wait);
}

/*
 * This is the serial driver's generic interrupt routine
 */
static irqreturn_t mxser_interrupt(int irq, void *dev_id)
{
	int			status, iir, i;
	struct mxser_struct *	info;
	struct mxser_struct *	port=NULL;
	int			max, irqbits, msr;
	int			pass_counter=0, bits;

        for ( i=0; i<MXSER_BOARDS; i++ ) {
		if ( dev_id == &(mxvar_table[i*MXSER_PORTS_PER_BOARD]) ) {
			port = dev_id;
			break;
		}
	}
	if ( port == NULL ) {
		return IRQ_NONE;
	}
        max = mxser_numports[mxsercfg[i].board_type-1];
        
#ifdef CONFIG_MACH_MOXA_IA261	// add by Victor Yu. 12-14-2007
	irqbits = 0;
#endif
	while ( 1 ) {
#ifdef CONFIG_MACH_MOXA_IA261	// add by Victor Yu. 12-13-2007
	    if ( irqbits == max )
		    break;
	    else
		    irqbits = 0;
#else
	    irqbits = inb(port->vector) & port->vectormask;
	    if ( irqbits == port->vectormask )
		break;
#endif
#if defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_IA241_32128) || defined(CONFIG_MACH_KIXRP435)	// add by Victor Yu. 05-22-2007, by PCI interface
	    for ( i=0, bits=1; i<max; i++, irqbits |= bits, bits <<= 1 ) {
#elif (defined CONFIG_ARCH_W311)
	    for ( i=0, bits=2; i<max; i++, irqbits |= bits, bits <<= 1 ) {
#elif defined(CONFIG_MACH_MOXA_IA261)	// add by Victor Yu. 12-14-2007
	    for ( i=0; i<max; i++ ) {
#else
	    for ( i=0, bits=4; i<max; i++, irqbits |= bits, bits <<= 1 ) {
#endif
#ifndef CONFIG_MACH_MOXA_IA261	// add by Victor Yu. 12-14-2007
		if ( irqbits == port->vectormask )
		    break;
		if ( bits & irqbits )
		    continue;
#endif
		info = port + i;

		// following add by Victor Yu. 09-13-2002
		iir = inb(info->base+UART_IIR);
		if ( iir & UART_IIR_NO_INT ) {
#ifdef CONFIG_MACH_MOXA_IA261	// add by Victor Yu. 12-13-2007
			irqbits++;
#endif
			continue;
		}
		iir &= MOXA_MUST_IIR_MASK;
		if ( !info->tty ) {	// because the UART chip has bug. so need to do this
			status = inb(info->base+UART_LSR);
			outb(0x27, info->base+UART_FCR);
			inb(info->base+UART_MSR);
			continue;
		}
		// above add by Victor Yu. 09-13-2002

#if 1	// add by Victor Yu. 01-04-2005
		if ( iir == MOXA_MUST_IIR_XSC ) {
			if ( !(inb(info->base+UART_MCR) & MOXA_MUST_MCR_XON_FLAG) ) {
				info->icount.buf_overrun++;
				wake_up_interruptible(&info->delta_msr_wait);
			}
			continue;
		}
#endif

		if ( iir == MOXA_MUST_IIR_GDA ||
		     iir == MOXA_MUST_IIR_RDA ||
		     iir == MOXA_MUST_IIR_RTO ||
		     iir == MOXA_MUST_IIR_LSR ) {
			status = inb(info->base+UART_LSR);
			mxser_receive_chars(info, &status);
		}
		msr = inb(info->base + UART_MSR);
		if ( msr & UART_MSR_ANY_DELTA ) {
		    mxser_check_modem_status(info, msr);
		}

		if ( iir == 0x02 ) {
			mxser_transmit_chars(info);
		}
	    }
	    if ( pass_counter++ > MXSER_ISR_PASS_LIMIT ) {
		break;	/* Prevent infinite loops */
	    }
	}
	return IRQ_HANDLED;
}

#define N_TTY_BUF_SIZE_MASK	(N_TTY_BUF_SIZE-1)	// add by Victor Yu. 02-04-2005
static inline void mxser_receive_chars(struct mxser_struct *info,
					 int *status)
{
	struct tty_struct *	tty = info->tty;
	unsigned char		ch, gdl;
	int			cnt = 0;

	// following add by Victor Yu. 09-02-2002
	if ( *status & (UART_LSR_SPECIAL|MOXA_MUST_LSR_RERR) )
		goto intr_old;
		
	gdl = inb(info->base+MOXA_MUST_GDL_REGISTER);
#ifdef CONFIG_MACH_MOXA_IA261	// add by Victor Yu. 12-13-2007
	gdl &= MOXA_MUST_GDL_MASK;
#endif
	if ( tty->receive_room == 0 ) {
		if ( I_IXOFF(tty) || (tty->termios->c_cflag & CRTSCTS) ) { // has flow control
			cnt = gdl = MIN(gdl, tty->receive_room);
		} else {
			cnt = gdl;
		}
	} else {
		cnt = gdl = MIN(gdl, tty->receive_room);
	}

	while ( gdl-- ) {
    		ch = inb(info->base + UART_RX);
		tty_insert_flip_char(tty, ch, 0);
	}
	goto end_intr;

intr_old:
	// above add by Victor Yu. 09-02-2002

	if ( tty->receive_room == 0 ) {
		if ( I_IXOFF(tty) || (tty->termios->c_cflag & CRTSCTS) ) // has flow control
			goto end_intr;
	}
	ch = inb(info->base + UART_RX);

	// following add by Victor Yu. 09-02-2002
	if ( (*status&UART_LSR_OE) && !(*status&UART_LSR_DR) ) {
		outb(0x23, info->base+UART_FCR);
	}

#if 1	// add by Victor Yu. 01-06-2005
	{
	int 	__flag=0;
	if ( *status & UART_LSR_BI ) {
		info->icount.brk++;
		__flag = 1;
	}
	if ( *status & UART_LSR_PE ) {
		info->icount.parity++;
		__flag = 1;
	}
	if ( *status & UART_LSR_FE ) {
		info->icount.frame++;
		__flag = 1;
	}
	if ( *status & UART_LSR_OE ) {
		info->icount.overrun++;
		__flag = 1;
	}
	if ( __flag ) 
		wake_up_interruptible(&info->delta_msr_wait);
	}
#endif

	*status &= info->read_status_mask;
	// above add by Victor Yu. 09-02-2002

	if ( !(*status & info->ignore_status_mask) ) {
		char	flag;
		if ( *status & UART_LSR_SPECIAL ) {
		    if ( *status & UART_LSR_BI ) {
			flag = TTY_BREAK;
			if ( info->flags & ASYNC_SAK )
			    do_SAK(tty);
		    } else if ( *status & UART_LSR_PE ) {
			flag = TTY_PARITY;
		    } else if ( *status & UART_LSR_FE ) {
			flag = TTY_FRAME;
		    } else if ( *status & UART_LSR_OE ) {
			flag = TTY_OVERRUN;
		    } else {
			flag = 0;
		    }
		} else {
		    flag = 0;
		}
		tty_insert_flip_char(tty, ch, flag);
		cnt++;
	}

end_intr:	// add by Victor Yu. 09-02-2002

	mxvar_log.rxcnt[info->port] += cnt;
	info->icount.rx += cnt;

	tty_flip_buffer_push(tty);
}

static inline void mxser_transmit_chars(struct mxser_struct *info)
{
	int	count, cnt;

	if ( info->x_char ) {
	    outb(info->x_char, info->base + UART_TX);
	    info->x_char = 0;
	    mxvar_log.txcnt[info->port]++;
	    info->icount.tx++;
	    return;
	}

	if ( info->xmit_buf == 0 )
		return;

	if ((info->xmit_cnt <= 0) || info->tty->stopped ||
	    (info->tty->hw_stopped && (info->type != PORT_16550A))) {
		info->IER &= ~UART_IER_THRI;
		outb(info->IER, info->base + UART_IER);
		return;
	}

	{
	int		tail=info->xmit_tail;
	unsigned char	*ptr=info->xmit_buf;
#define XMIT_SIZE_MASK	(SERIAL_XMIT_SIZE - 1)
	cnt = count = MIN(info->xmit_fifo_size, info->xmit_cnt);
	while ( count-- > 0 ) {
		outb(ptr[tail++], info->base);
		tail &= XMIT_SIZE_MASK;
	}
	info->xmit_tail = tail;
	info->xmit_cnt -= cnt;
	mxvar_log.txcnt[info->port] += cnt;
	}

        info->icount.tx += cnt;

	if ( info->xmit_cnt < WAKEUP_CHARS ) {
		set_bit(MXSER_EVENT_TXLOW,&info->event);
		schedule_work(&info->tqueue);
	}
	if (info->xmit_cnt <= 0) {
		info->IER &= ~UART_IER_THRI;
		outb(info->IER, info->base + UART_IER);
	}
}

static inline void mxser_check_modem_status(struct mxser_struct *info,
					      int status)
{
	/* update input line counters */
	if ( status & UART_MSR_TERI )
	    info->icount.rng++;
	if ( status & UART_MSR_DDSR )
	    info->icount.dsr++;
	if ( status & UART_MSR_DDCD )
	    info->icount.dcd++;
	if ( status & UART_MSR_DCTS )
	    info->icount.cts++;
	wake_up_interruptible(&info->delta_msr_wait);

	if ( (info->flags & ASYNC_CHECK_CD) && (status & UART_MSR_DDCD) ) {
	    if ( status & UART_MSR_DCD )
		wake_up_interruptible(&info->open_wait);
	    else
	        set_bit(MXSER_EVENT_HANGUP,&info->event);
		schedule_work(&info->tqueue);
	}
	if ( info->flags & ASYNC_CTS_FLOW ) {
	    if ( info->tty->hw_stopped ) {
		if (status & UART_MSR_CTS ){  
		    info->tty->hw_stopped = 0;

		    if (info->type != PORT_16550A) {
			info->IER |= UART_IER_THRI;
			outb(info->IER, info->base + UART_IER);
		    }
	       	    set_bit(MXSER_EVENT_TXLOW,&info->event);
		    schedule_work(&info->tqueue);
                }   
	    } else {
		if ( !(status & UART_MSR_CTS) ){
		    info->tty->hw_stopped = 1;
		    if (info->type != PORT_16550A) {
			info->IER &= ~UART_IER_THRI;
			outb(info->IER, info->base + UART_IER);
		    }
		}
	    }
	}
}

static int mxser_block_til_ready(struct tty_struct *tty, struct file * filp,
				 struct mxser_struct *info)
{
	DECLARE_WAITQUEUE(wait, current); 
	unsigned long 		flags;
	int			retval;
	int			do_clocal = 0;

	/*
	 * If the device is in the middle of being closed, then block
	 * until it's done, and then try again.
	 */
	if ( tty_hung_up_p(filp) || (info->flags & ASYNC_CLOSING) ) {
	    if ( info->flags & ASYNC_CLOSING )
		interruptible_sleep_on(&info->close_wait);
#ifdef SERIAL_DO_RESTART
	    if ( info->flags & ASYNC_HUP_NOTIFY )
		return(-EAGAIN);
	    else
		return(-ERESTARTSYS);
#else
	    return(-EAGAIN);
#endif
	}

	/*
	 * If this is a callout device, then just make sure the normal
	 * device isn't being used.
	 */

	/*
	 * If non-blocking mode is set, or the port is not enabled,
	 * then make the check up front and then exit.
	 */
	if ( (filp->f_flags & O_NONBLOCK) ||
	     (tty->flags & (1 << TTY_IO_ERROR)) ) {
	    info->flags |= ASYNC_NORMAL_ACTIVE;
	    return(0);
	}

	if ( tty->termios->c_cflag & CLOCAL )
		do_clocal = 1;

	/*
	 * Block waiting for the carrier detect and the line to become
	 * free (i.e., not in use by the callout).  While we are in
	 * this loop, info->count is dropped by one, so that
	 * mxser_close() knows when to free things.  We restore it upon
	 * exit, either normal or abnormal.
	 */
	retval = 0;
	add_wait_queue(&info->open_wait, &wait);
	if ( !tty_hung_up_p(filp) ) {
		local_irq_save(flags);
		info->count--;
		local_irq_restore(flags);
	}
	info->blocked_open++;
	while ( 1 ) {
	    local_irq_save(flags);
	    outb(inb(info->base + UART_MCR) | UART_MCR_DTR | UART_MCR_RTS, info->base + UART_MCR);
	    local_irq_restore(flags);
	    current->state = TASK_INTERRUPTIBLE;
	    if ( tty_hung_up_p(filp) || !(info->flags & ASYNC_INITIALIZED) ) {
#ifdef SERIAL_DO_RESTART
		if ( info->flags & ASYNC_HUP_NOTIFY )
		    retval = -EAGAIN;
		else
		    retval = -ERESTARTSYS;
#else
		retval = -EAGAIN;
#endif
		break;
	    }
	    if ( !(info->flags & ASYNC_CLOSING) &&
		 (do_clocal || (inb(info->base + UART_MSR) & UART_MSR_DCD)) )
		break;
	    if ( signal_pending(current) ) {
		retval = -ERESTARTSYS;
		break;
	    }
	    schedule();
	}
	current->state = TASK_RUNNING;
	remove_wait_queue(&info->open_wait, &wait);
	if ( !tty_hung_up_p(filp) ) {
	    local_irq_save(flags);
	    info->count++;
	    local_irq_restore(flags);
	}
	info->blocked_open--;
	if ( retval )
	    return(retval);
	info->flags |= ASYNC_NORMAL_ACTIVE;
	return(0);
}

static int mxser_startup(struct mxser_struct * info)
{
	unsigned long	flags;
	unsigned long	page;

	page = get_zeroed_page(GFP_KERNEL);
	if ( !page )
	    return(-ENOMEM);

	local_irq_save(flags);

	if ( info->flags & ASYNC_INITIALIZED ) {
	    free_page(page);
	    local_irq_restore(flags);
	    return(0);
	}

	if ( !info->base || !info->type ) {
	    if ( info->tty ) {
		set_bit(TTY_IO_ERROR, &info->tty->flags);
	    }
	    free_page(page);
	    local_irq_restore(flags);
	    return(0);
	}
	if ( info->xmit_buf )
	    free_page(page);
	else
	    info->xmit_buf = (unsigned char *)page;

	/*
	 * Clear the FIFO buffers and disable them
	 * (they will be reenabled in mxser_change_speed())
	 */
	if ( info->xmit_fifo_size == 16 ) {
	    outb((UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT),
		 info->base + UART_FCR);
	// following add by Victor Yu. 08-30-2002
	} else if ( info->xmit_fifo_size == 64 || info->xmit_fifo_size == 128 ) {
	    outb((UART_FCR_CLEAR_RCVR|UART_FCR_CLEAR_XMIT|MOXA_MUST_FCR_GDA_MODE_ENABLE), info->base+UART_FCR);
	}
	// above add by Victor Yu. 08-30-2002

	/*
	 * At this point there's no way the LSR could still be 0xFF;
	 * if it is, then bail out, because there's likely no UART
	 * here.
	 */
	if ( inb(info->base + UART_LSR) == 0xff ) {
	    local_irq_restore(flags);
            if (capable(CAP_SYS_ADMIN)) {
		if ( info->tty ) {
		    set_bit(TTY_IO_ERROR, &info->tty->flags);
		}
		return(0);
	    } else
		return(-ENODEV);
	}
         
	/*
	 * Clear the interrupt registers.
	 */
	(void)inb(info->base + UART_LSR);
	(void)inb(info->base + UART_RX);
	(void)inb(info->base + UART_IIR);
	(void)inb(info->base + UART_MSR);

	/*
	 * Now, initialize the UART
	 */
	outb(UART_LCR_WLEN8, info->base + UART_LCR);	/* reset DLAB */
	info->MCR = UART_MCR_DTR | UART_MCR_RTS;
	outb(info->MCR, info->base + UART_MCR);

	/*
	 * Finally, enable interrupts
	 */
#if 1 // Begin --- add by Wade, 10-22-2010, not enable UART_IER_MSI when startup
	if ( ier == 0 )
		info->IER = UART_IER_RLSI | UART_IER_RDI | MOXA_MUST_IER_EGDAI;
	else
		info->IER = UART_IER_MSI | UART_IER_RLSI | UART_IER_RDI | MOXA_MUST_IER_EGDAI;
#else

	info->IER = UART_IER_MSI | UART_IER_RLSI | UART_IER_RDI | MOXA_MUST_IER_EGDAI;
#endif // End --- add by Wade, 10-22-2010
	outb(info->IER, info->base + UART_IER); /* enable interrupts */

	/*
	 * And clear the interrupt registers again for luck.
	 */
	(void)inb(info->base + UART_LSR);
	(void)inb(info->base + UART_RX);
	(void)inb(info->base + UART_IIR);
	(void)inb(info->base + UART_MSR);

	if ( info->tty )
	    test_and_clear_bit(TTY_IO_ERROR, &info->tty->flags);
	info->xmit_cnt = info->xmit_head = info->xmit_tail = 0;

	/*
	 * and set the speed of the serial port
	 */
#if 1	// add by Victor Yu. 12-27-2004
	local_irq_restore(flags);
#endif
	mxser_change_speed(info, 0);

#if 1	// add by Victor Yu. 12-27-2004
	local_irq_save(flags);
#endif
	info->flags |= ASYNC_INITIALIZED;
	local_irq_restore(flags);
	return(0);
}

/*
 * This routine will shutdown a serial port; interrupts maybe disabled, and
 * DTR is dropped if the hangup on close termio flag is on.
 */
static void mxser_shutdown(struct mxser_struct * info)
{
	unsigned long	flags;

	if ( !(info->flags & ASYNC_INITIALIZED) )
	    return;

	local_irq_save(flags);

	/*
	 * clear delta_msr_wait queue to avoid mem leaks: we may free the irq
	 * here so the queue might never be waken up
	 */
	wake_up_interruptible(&info->delta_msr_wait);

	/*
	 * Free the IRQ, if necessary
	 */
	if ( info->xmit_buf ) {
	    free_page((unsigned long)info->xmit_buf);
	    info->xmit_buf = 0;
	}

	info->IER = 0;
	outb(0x00, info->base + UART_IER);

	if ( !info->tty || (info->tty->termios->c_cflag & HUPCL) )
	    info->MCR &= ~(UART_MCR_DTR | UART_MCR_RTS);
	outb(info->MCR, info->base + UART_MCR);

	/* clear Rx/Tx FIFO's */
	outb((UART_FCR_CLEAR_RCVR|UART_FCR_CLEAR_XMIT|MOXA_MUST_FCR_GDA_MODE_ENABLE), info->base + UART_FCR);

	/* read data port to reset things */
	(void)inb(info->base + UART_RX);

	if ( info->tty )
	    set_bit(TTY_IO_ERROR, &info->tty->flags);

	info->flags &= ~ASYNC_INITIALIZED;
	SET_MOXA_MUST_NO_SOFTWARE_FLOW_CONTROL(info->base);
	local_irq_restore(flags);
}

/*
 * This routine is called to set the UART divisor registers to match
 * the specified baud rate for a serial port.
 */
static int mxser_change_speed(struct mxser_struct *info,
                              struct ktermios *old_termios)
{
	int		quot = 0;
	unsigned	cflag, cval, fcr;
	int		bindex;
        int             ret = 0;
	unsigned long	flags;

	if ( !info->tty || !info->tty->termios )
	    return ret;
	cflag = info->tty->termios->c_cflag;
	if ( !(info->base) )
	    return ret;
           
#ifndef B921600
#define B921600 (B460800 +1)
#endif 
	switch( cflag & (CBAUD | CBAUDEX) ){
#ifdef CONFIG_MOXA_SUPPORT_SPECIAL_BAUD_RATE	// add by Victor Yu. 08-13-2004
	case B4000000 : bindex = BAUD_TABLE_NO; break;
#endif
        case B921600 : bindex = 20; break;
        case B460800 : bindex = 19; break;
        case B230400 : bindex = 18; break;
        case B115200 : bindex = 17; break;
        case B57600 : bindex = 16; break;
        case B38400 : bindex = 15; break;
        case B19200 : bindex = 14; break;
        case B9600 : bindex = 13; break;
        case B4800 : bindex = 12; break;
        case B2400 : bindex = 11; break;
        case B1800 : bindex = 10; break;
        case B1200 : bindex = 9; break;
        case B600 : bindex = 8; break;
        case B300 : bindex = 7; break;
        case B200 : bindex = 6; break;
        case B150 : bindex = 5; break;
        case B134 : bindex = 4; break;
        case B110 : bindex = 3; break;
        case B75 : bindex = 2; break;
        case B50 : bindex = 1; break;
        default:   bindex = 0; break;
	}
	
	if ( bindex == 15 ) {
	    if ( (info->flags & ASYNC_SPD_MASK) == ASYNC_SPD_HI )
		bindex = 16; 					/* 57600 bps */
	    if ( (info->flags & ASYNC_SPD_MASK) == ASYNC_SPD_VHI )
		bindex = 17; 					/* 115200 bps */
                
#ifdef ASYNC_SPD_SHI
	    if ((info->flags & ASYNC_SPD_MASK) == ASYNC_SPD_SHI)
                bindex = 18;
#endif

#ifdef ASYNC_SPD_WARP
            if ((info->flags & ASYNC_SPD_MASK) == ASYNC_SPD_WARP)
                bindex = 19;
#endif
	}
#ifdef CONFIG_MOXA_SUPPORT_SPECIAL_BAUD_RATE	// add by Victor Yu. 08-13-2004
	if ( bindex == BAUD_TABLE_NO ) {
		quot = info->baud_base / info->speed;
		if ( info->speed <= 0 || info->speed > info->MaxCanSetBaudRate )
			quot = 0;
	}
	else
#endif
	if ( mxvar_baud_table[bindex] == 134 ) {
	    quot = (2 * info->baud_base / 269);
#ifdef CONFIG_MOXA_SUPPORT_SPECIAL_BAUD_RATE	// add by Victor Yu. 08-12-2004
	    info->speed = 134;
#endif
	} else if ( mxvar_baud_table[bindex] ) {
	    quot = info->baud_base / mxvar_baud_table[bindex];
	    // following add by Victor Yu. 09-04-2002
	    if ( mxvar_baud_table[bindex] > info->MaxCanSetBaudRate )
		quot = 0;
	    // add by Victor Yu. 09-04-2002
#ifdef CONFIG_MOXA_SUPPORT_SPECIAL_BAUD_RATE	// add by Victor Yu. 08-12-2004
	    info->speed = mxvar_baud_table[bindex];
#endif
       	    if (!quot && old_termios) {
                /* re-calculate */
        	info->tty->termios->c_cflag &= ~CBAUD;
        	info->tty->termios->c_cflag |= (old_termios->c_cflag & CBAUD);
		switch( info->tty->termios->c_cflag & (CBAUD | CBAUDEX) ){
#ifdef CONFIG_MOXA_SUPPORT_SPECIAL_BAUD_RATE	// add by Victor Yu. 08-13-2004
		case B4000000 : bindex = BAUD_TABLE_NO; break;
#endif
	        case B921600 : bindex = 20; break;
        	case B460800 : bindex = 19; break;
	        case B230400 : bindex = 18; break;
        	case B115200 : bindex = 17; break;
	        case B57600 : bindex = 16; break;
        	case B38400 : bindex = 15; break;
	        case B19200 : bindex = 14; break;
        	case B9600 : bindex = 13; break;
	        case B4800 : bindex = 12; break;
        	case B2400 : bindex = 11; break;
	        case B1800 : bindex = 10; break;
        	case B1200 : bindex = 9; break;
	        case B600 : bindex = 8; break;
        	case B300 : bindex = 7; break;
	        case B200 : bindex = 6; break;
        	case B150 : bindex = 5; break;
	        case B134 : bindex = 4; break;
        	case B110 : bindex = 3; break;
	        case B75 : bindex = 2; break;
        	case B50 : bindex = 1; break;
	        default:   bindex = 0; break;
		}
        	if ( bindex == 15 ) {
        	    if ( (info->flags & ASYNC_SPD_MASK) == ASYNC_SPD_HI )
        	        bindex = 16; 					/* 57600 bps */
        	    if ( (info->flags & ASYNC_SPD_MASK) == ASYNC_SPD_VHI )
        	        bindex = 17; 					/* 115200 bps */
#ifdef ASYNC_SPD_SHI
	            if ((info->flags & ASYNC_SPD_MASK) == ASYNC_SPD_SHI)
                        bindex = 18;
#endif
#ifdef ASYNC_SPD_WARP
                    if ((info->flags & ASYNC_SPD_MASK) == ASYNC_SPD_WARP)
                        bindex = 19;
#endif
        	}
#ifdef CONFIG_MOXA_SUPPORT_SPECIAL_BAUD_RATE	// add by Victor Yu. 08-13-2004
		if ( bindex == BAUD_TABLE_NO ) {
			quot = info->baud_base / info->speed;
		    	if ( info->speed <= 0 || info->speed > info->MaxCanSetBaudRate )
				quot = 0;
		}
		else
#endif
        	if ( mxvar_baud_table[bindex] == 134 ) {
        	    quot = (2 * info->baud_base / 269);
#ifdef CONFIG_MOXA_SUPPORT_SPECIAL_BAUD_RATE	// add by Victor Yu. 08-12-2004
		    info->speed = 134;
#endif
                } else if ( mxvar_baud_table[bindex] ) {
        	    quot = info->baud_base / mxvar_baud_table[bindex];
		    // following add by Victor Yu. 09-04-2002
		    if ( mxvar_baud_table[bindex] > info->MaxCanSetBaudRate )
			quot = 0;
		    // above add by Victor Yu. 09-04-2002
#ifdef CONFIG_MOXA_SUPPORT_SPECIAL_BAUD_RATE	// add by Victor Yu. 08-12-2004
		    info->speed = mxvar_baud_table[bindex];
#endif
                    if(quot==0)
                        quot = 1;
        	} else {
        	    quot = 0;
        	}
            }else if(quot==0)
                quot = 1;
	} else {
	    quot = 0;
	}

	info->timeout = ((info->xmit_fifo_size*HZ*10*quot) / info->baud_base);
	info->timeout += HZ/50;		/* Add .02 seconds of slop */

	if ( quot ) {
	    local_irq_save(flags);
	    info->MCR |= UART_MCR_DTR;
	    outb(info->MCR, info->base + UART_MCR);
	    local_irq_restore(flags);
	} else {
	    local_irq_save(flags);
	    info->MCR &= ~UART_MCR_DTR;
	    outb(info->MCR, info->base + UART_MCR);
	    local_irq_restore(flags);
	    return ret;
	}
	/* byte size and parity */
	switch ( cflag & CSIZE ) {
	case CS5: cval = 0x00; break;
	case CS6: cval = 0x01; break;
	case CS7: cval = 0x02; break;
	case CS8: cval = 0x03; break;
	default:  cval = 0x00; break;	/* too keep GCC shut... */
	}
	if ( cflag & CSTOPB )
	    cval |= 0x04;
	if ( cflag & PARENB )
	    cval |= UART_LCR_PARITY;
#ifndef CMSPAR
#define	CMSPAR 010000000000
#endif
	if ( !(cflag & PARODD) ){
	    cval |= UART_LCR_EPAR;
	}
	if ( cflag & CMSPAR )
	    cval |= UART_LCR_SPAR;

	if ( (info->type == PORT_8250) || (info->type == PORT_16450) ) {
	    fcr = 0;
	} else {
	    fcr = UART_FCR_ENABLE_FIFO;
	    // following add by Victor Yu. 08-30-2002
	    fcr |= MOXA_MUST_FCR_GDA_MODE_ENABLE;
#if 1	// add by Victor Yu. 05-04-2005
	    local_irq_save(flags);
#endif
	    SET_MOXA_MUST_FIFO_VALUE(info);
#if 1	// add by Victor Yu. 05-04-2005
	    local_irq_restore(flags);
#endif
	}

	/* CTS flow control flag and modem status interrupts */
#if 1	// add by Victor Yu. 12-30-2004
	local_irq_save(flags);
#if 1	// add by Victor Yu. 05-04-2005
	outb(cval | UART_LCR_DLAB, info->base + UART_LCR);  /* set DLAB */
	outb(quot & 0xff, info->base + UART_DLL);	    /* LS of divisor */
	outb(quot >> 8, info->base + UART_DLM); 	    /* MS of divisor */
	outb(cval, info->base + UART_LCR);		    /* reset DLAB */
#ifdef CONFIG_MOXA_SUPPORT_SPECIAL_BAUD_RATE	// add by Victor Yu. 08-12-2004
	if ( info->speed ) {
		quot = info->baud_base % info->speed;
		quot *= 8;
		if ( (quot % info->speed) > (info->speed / 2) ) {
			quot /= info->speed;
			quot++;
		} else {
			quot /= info->speed;
		}
		SET_MOXA_MUST_ENUM_VALUE(info->base, quot);
	} else {
		SET_MOXA_MUST_ENUM_VALUE(info->base, 0);
	}
#endif
	outb(fcr, info->base + UART_FCR);		    /* set fcr */
#endif	// 05-04-2005
#endif	// 12-30-2004
	info->MCR &= ~UART_MCR_AFE;
	if ( cflag & CRTSCTS ) {
	    info->flags |= ASYNC_CTS_FLOW;
	    info->IER |= UART_IER_MSI;
	    if ( info->type == PORT_16550A ) {
		info->MCR |= UART_MCR_AFE;
	    } else {
			unsigned char status;
			status = inb(info->base + UART_MSR);
			if (info->tty->hw_stopped) {
				if (status & UART_MSR_CTS) {
					info->tty->hw_stopped = 0;
					if (info->type != PORT_16550A) {
						info->IER |= UART_IER_THRI;
						outb(info->IER, info->base + UART_IER);
					}
					set_bit(MXSER_EVENT_TXLOW, &info->event);
					schedule_work(&info->tqueue);
				}
			} else {
				if (!(status & UART_MSR_CTS)) {
					info->tty->hw_stopped = 1;
					if (info->type != PORT_16550A) {
						info->IER &= ~UART_IER_THRI;
						outb(info->IER, info->base + UART_IER);
					}
				}
			}
	        }
	} else {
	    info->flags &= ~ASYNC_CTS_FLOW;
	}
	outb(info->MCR, info->base + UART_MCR);
	if ( cflag & CLOCAL ){
	    info->flags &= ~ASYNC_CHECK_CD;
	}else {
	    info->flags |= ASYNC_CHECK_CD;
	    info->IER |= UART_IER_MSI;
	}
	outb(info->IER, info->base + UART_IER);
#if 1	// add by Victor Yu. 12-27-2004
	local_irq_restore(flags);
#endif

	/*
	 * Set up parity check flag
	 */
	info->read_status_mask = UART_LSR_OE | UART_LSR_THRE | UART_LSR_DR;
	if ( I_INPCK(info->tty) )
	    info->read_status_mask |= UART_LSR_FE | UART_LSR_PE;
	if ( I_BRKINT(info->tty) || I_PARMRK(info->tty) )
	    info->read_status_mask |= UART_LSR_BI;

	info->ignore_status_mask = 0;
	if ( I_IGNBRK(info->tty) ) {
	    info->ignore_status_mask |= UART_LSR_BI;
	    info->read_status_mask |= UART_LSR_BI;
	    /*
	     * If we're ignore parity and break indicators, ignore
	     * overruns too.  (For real raw support).
	     */
	    if ( I_IGNPAR(info->tty) ) {
		info->ignore_status_mask |= UART_LSR_OE|UART_LSR_PE|UART_LSR_FE;
		info->read_status_mask |= UART_LSR_OE|UART_LSR_PE|UART_LSR_FE;
	    }
	}
	
	// following add by Victor Yu. 09-02-2002
	local_irq_save(flags);
	SET_MOXA_MUST_XON1_VALUE(info->base, START_CHAR(info->tty));
	SET_MOXA_MUST_XOFF1_VALUE(info->base, STOP_CHAR(info->tty));
	if ( I_IXON(info->tty) ) {
#if 1	// add by Victor Yu. 01-04-2005
		info->IER |= MOXA_MUST_IER_XINT;
#endif
		ENABLE_MOXA_MUST_RX_SOFTWARE_FLOW_CONTROL(info->base);
	} else {
#if 1	// add by Victor Yu. 01-04-2005
		info->IER &= ~MOXA_MUST_IER_XINT;
#endif
		DISABLE_MOXA_MUST_RX_SOFTWARE_FLOW_CONTROL(info->base);
	}
#if 1	// add by Victor Yu. 01-04-2005
	outb(info->IER, info->base+UART_IER);
#endif
	if ( I_IXOFF(info->tty) ) {
		ENABLE_MOXA_MUST_TX_SOFTWARE_FLOW_CONTROL(info->base);
	} else {
		DISABLE_MOXA_MUST_TX_SOFTWARE_FLOW_CONTROL(info->base);
	}
	if ( I_IXANY(info->tty) ) {
		info->MCR |= MOXA_MUST_MCR_XON_ANY;
		ENABLE_MOXA_MUST_XON_ANY_FLOW_CONTROL(info->base);
	} else {
		info->MCR &= ~MOXA_MUST_MCR_XON_ANY;
		DISABLE_MOXA_MUST_XON_ANY_FLOW_CONTROL(info->base);
	}
	local_irq_restore(flags);
	// above add by Victor Yu. 09-02-2002

        return ret;
}

/*
 * ------------------------------------------------------------
 * friends of mxser_ioctl()
 * ------------------------------------------------------------
 */
static int mxser_get_serial_info(struct mxser_struct * info,
				 struct serial_struct * retinfo)
{
	struct serial_struct	tmp;

	if ( !retinfo )
	    return(-EFAULT);
	memset(&tmp, 0, sizeof(tmp));
	tmp.type = info->type;
	tmp.line = info->port;
	tmp.port = info->base;
	tmp.irq = info->irq;
	tmp.flags = info->flags;
	tmp.baud_base = info->baud_base;
	tmp.close_delay = info->close_delay;
	tmp.closing_wait = info->closing_wait;
	tmp.custom_divisor = info->custom_divisor;
	tmp.hub6 = 0;
	copy_to_user(retinfo, &tmp, sizeof(*retinfo));
	return(0);
}

static int mxser_set_serial_info(struct mxser_struct * info,
				 struct serial_struct * new_info)
{
	struct serial_struct	new_serial;
	unsigned int		flags;
	int			retval = 0;

	if ( !new_info || !info->base )
	    return(-EFAULT);
	copy_from_user(&new_serial, new_info, sizeof(new_serial));

	if ( (new_serial.irq != info->irq) ||
	     (new_serial.port != info->base) ||
	     (new_serial.custom_divisor != info->custom_divisor) ||
	     (new_serial.baud_base != info->baud_base) )
	    return(-EPERM);

	flags = info->flags & ASYNC_SPD_MASK;

       if ( !capable(CAP_SYS_ADMIN)) {
	    if ( (new_serial.baud_base != info->baud_base) ||
		 (new_serial.close_delay != info->close_delay) ||
		 ((new_serial.flags & ~ASYNC_USR_MASK) !=
		 (info->flags & ~ASYNC_USR_MASK)) )
		return(-EPERM);
	    info->flags = ((info->flags & ~ASYNC_USR_MASK) |
			  (new_serial.flags & ASYNC_USR_MASK));
	} else {
	    /*
	     * OK, past this point, all the error checking has been done.
	     * At this point, we start making changes.....
	     */
	    info->flags = ((info->flags & ~ASYNC_FLAGS) |
			  (new_serial.flags & ASYNC_FLAGS));
	    info->close_delay = new_serial.close_delay * HZ/100;
	    info->closing_wait = new_serial.closing_wait * HZ/100;
	    info->tty->low_latency = (info->flags & ASYNC_LOW_LATENCY) ? 1 : 0;
	}

	info->type = new_serial.type;

#ifdef CONFIG_MACH_MOXA_IA261
	info->xmit_fifo_size = 64;
#else
	info->xmit_fifo_size = 128;
#endif
	if ( info->flags & ASYNC_INITIALIZED ) {
	    if ( flags != (info->flags & ASYNC_SPD_MASK) ){
		mxser_change_speed(info,0);
	    }
	} else{
	    retval = mxser_startup(info);
	}
	return(retval);
}

/*
 * mxser_get_lsr_info - get line status register info
 *
 * Purpose: Let user call ioctl() to get info when the UART physically
 *	    is emptied.  On bus types like RS485, the transmitter must
 *	    release the bus after transmitting. This must be done when
 *	    the transmit shift register is empty, not be done when the
 *	    transmit holding register is empty.  This functionality
 *	    allows an RS485 driver to be written in user space.
 */
static int mxser_get_lsr_info(struct mxser_struct * info, unsigned int *value)
{
	unsigned char	status;
	unsigned int	result;
	unsigned long	flags;

	local_irq_save(flags);
	status = inb(info->base + UART_LSR);
	local_irq_restore(flags);
	result = ((status & UART_LSR_TEMT) ? TIOCSER_TEMT : 0);
	put_to_user(result, value);
	return(0);
}

/*
 * This routine sends a break character out the serial port.
 */
static void mxser_send_break(struct mxser_struct * info, int duration)
{
	unsigned long	flags;
#if 1	// add by Victor Yu. 12-27-2004
	unsigned long	timeout;
#endif

	if ( !info->base )
	    return;
	local_irq_save(flags);
	outb(inb(info->base + UART_LCR) | UART_LCR_SBC, info->base + UART_LCR);
	local_irq_restore(flags);
	timeout = jiffies + (unsigned long)duration;
	while ( 1 ) {
		current->state = TASK_INTERRUPTIBLE;
		schedule_timeout(5);
		if ( time_after(jiffies, timeout) )
			break;
	}
	local_irq_save(flags);
	outb(inb(info->base + UART_LCR) & ~UART_LCR_SBC, info->base + UART_LCR);
	local_irq_restore(flags);
}
