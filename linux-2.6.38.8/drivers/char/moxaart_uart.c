/*
 *          moxaart_uart.c  -- MOXA ART SoC multiport serial driver.
 *
 *      Copyright (C) 1999-2006  Moxa Technologies (support@moxa.com).
 *	Copyright (C) 2006-2008  Jiri Slaby <jirislaby@gmail.com>
 *      Copyright (C) 2011-2011  Moxa Technologies (support@moxa.com).
 *
 *      This code is loosely based on the 1.8 moxa driver which is based on
 *	Linux serial driver, written by Linus Torvalds, Theodore T'so and
 *	others.
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *	Fed through a cleanup, indent and remove of non 2.6 code by Alan Cox
 *	<alan@lxorguk.ukuu.org.uk>. The original 1.8 code is available on
 *	www.moxa.com.
 *	- Fixed x86_64 cleanness
 */

#include <linux/module.h>
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
#include <linux/delay.h>
#include <linux/pci.h>
#include <linux/bitops.h>
#include <linux/slab.h>
#include <linux/irq.h>

#include <asm/system.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <mach/hardware.h>
#include <mach/moxaart/moxaart.h>
#include <mach/moxaart_int.h>
#include <mach/gpio.h>

#include "moxaart_uart.h"

#define	MXSER_VERSION	"1.0"
#define	MXSERMAJOR	 30

#define MXSER_BOARDS		4	/* Max. boards */
#define MXSER_PORTS_PER_BOARD	8	/* Max. ports per board */
#define MXSER_PORTS		(MXSER_BOARDS * MXSER_PORTS_PER_BOARD)
#define MXSER_ISR_PASS_LIMIT	100

/*CheckIsMoxaMust return value*/
#define MOXA_OTHER_UART		0x00
#define MOXA_MUST_MU150_HWID	0x01
#define MOXA_MUST_MU860_HWID	0x02

/*
 * This is used to figure out the divisor speeds and the timeouts
 */
static struct _baud_rate_index {
	int	speed;
	int	index;
} mxvar_baud_table[] = {
	{50, B50},
	{75, B75},
	{110, B110},
	{134, B134},
	{150, B150},
	{200, B200},
	{300, B300},
	{600, B600},
	{1200, B1200},
	{1800, B1800},
	{2400, B2400},
	{4800, B4800},
	{9600, B9600},
	{19200, B19200},
	{38400, B38400},
	{57600, B57600},
	{115200, B115200},
	{230400, B230400},
	{460800, B460800},
	{921600, B921600},
};
#define BAUD_TABLE_NO   (sizeof(mxvar_baud_table)/sizeof(struct _baud_rate_index))

#define WAKEUP_CHARS		256
#define UART_LSR_SPECIAL	(UART_LSR_BI|UART_LSR_FE|UART_LSR_PE|UART_LSR_OE)

#define MXSER_HIGHBAUD	1
#define MXSER_HAS2	2

struct mxser_cardinfo {
	char *name;
	unsigned int nports;
	unsigned int flags;
};

static const struct mxser_cardinfo mxser_cards[] = {
/* 0*/	{ "MoxaART SoC UART",	4, MXSER_HIGHBAUD},
};

/* Variables for insmod */
static int rx_trigger=120;
static int xmit_fifo_size=128;
static int ttymajor = MXSERMAJOR;

MODULE_AUTHOR("Victor Yu");
MODULE_DESCRIPTION("MOXA ART SoC Multiport Device Driver");
module_param(ttymajor, int, 0);
module_param(xmit_fifo_size, int, 128);
module_param(rx_trigger, int, 120);
MODULE_LICENSE("GPL");

struct mxser_log {
	int tick;
	unsigned long rxcnt[MXSER_PORTS];
	unsigned long txcnt[MXSER_PORTS];
};

struct mxser_mon {
	unsigned long rxcnt;
	unsigned long txcnt;
	unsigned long up_rxcnt;
	unsigned long up_txcnt;
	int modem_status;
	unsigned char hold_reason;
};

struct mxser_mon_ext {
	unsigned long rx_cnt[32];
	unsigned long tx_cnt[32];
	unsigned long up_rxcnt[32];
	unsigned long up_txcnt[32];
	int modem_status[32];

	long baudrate[32];
	int databits[32];
	int stopbits[32];
	int parity[32];
	int flowctrl[32];
	int fifo[32];
	int iftype[32];
};

struct mxser_board;

struct mxser_port {
	struct tty_port port;
	struct mxser_board *board;

	unsigned long ioaddr;
	unsigned long opmode_ioaddr;
	int max_baud;

	int rx_trigger;		/* Rx fifo trigger level */
	int baud_base;		/* max. speed */
	int type;		/* UART type */
	int speed;		/* special baud rate to keep speed */

	int x_char;		/* xon/xoff character */
	int IER;		/* Interrupt Enable Register */
	int MCR;		/* Modem control register */

	unsigned char stop_rx;
	unsigned char ldisc_stop_rx;

	int custom_divisor;
	unsigned char err_shadow;

	struct async_icount icount; /* kernel counters for 4 input interrupts */
	int timeout;

	int read_status_mask;
	int ignore_status_mask;
	int xmit_fifo_size;
	int xmit_head;
	int xmit_tail;
	int xmit_cnt;

	struct ktermios normal_termios;

	struct mxser_mon mon_data;

	spinlock_t slock;
};

struct mxser_board {
	unsigned int idx;
	int irq;
	const struct mxser_cardinfo *info;
	unsigned long vector;
	unsigned long vector_mask;

	int chip_flag;
	int uart_type;

	struct mxser_port ports[MXSER_PORTS_PER_BOARD];
};

struct mxser_mstatus {
	tcflag_t cflag;
	int cts;
	int dsr;
	int ri;
	int dcd;
};

static struct mxser_board mxser_boards[MXSER_BOARDS];
static struct tty_driver *mxvar_sdriver;
static struct mxser_log mxvar_log;
static int mxser_set_baud_method[MXSER_PORTS + 1];

static void mxser_enable_must_enchance_mode(unsigned long baseio)
{
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr |= MOXA_MUST_EFR_EFRB_ENABLE;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);
	outb(oldlcr, baseio + UART_LCR);
}

static void mxser_set_must_xon1_value(unsigned long baseio, u8 value)
{
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr &= ~MOXA_MUST_EFR_BANK_MASK;
	efr |= MOXA_MUST_EFR_BANK0;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);
	outb(value, baseio + MOXA_MUST_XON1_REGISTER);
	outb(oldlcr, baseio + UART_LCR);
}

static void mxser_set_must_xoff1_value(unsigned long baseio, u8 value)
{
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr &= ~MOXA_MUST_EFR_BANK_MASK;
	efr |= MOXA_MUST_EFR_BANK0;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);
	outb(value, baseio + MOXA_MUST_XOFF1_REGISTER);
	outb(oldlcr, baseio + UART_LCR);
}

static void mxser_set_must_fifo_value(unsigned long baseio)
{
	u8 oldlcr;
	u8 efr;

	// enter enchance mode
	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr &= ~MOXA_MUST_EFR_BANK_MASK;
	efr |= MOXA_MUST_EFR_BANK1;
	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);

#define MOXA_MUST_HIGH_WATER	120
#define MOXA_MUST_LOW_WATER	8
	outb((u8)0, baseio + MOXA_MUST_THRTL_REGISTER);
	outb((u8)MOXA_MUST_HIGH_WATER, baseio + MOXA_MUST_RBRTH_REGISTER);
	outb((u8)rx_trigger, baseio + MOXA_MUST_RBRTI_REGISTER);
	outb((u8)MOXA_MUST_LOW_WATER, baseio + MOXA_MUST_RBRTL_REGISTER);

	// release enchance mode
	outb(oldlcr, baseio + UART_LCR);
}

static void mxser_set_must_enum_value(unsigned long baseio, u8 value)
{
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr &= ~MOXA_MUST_EFR_BANK_MASK;
	efr |= MOXA_MUST_EFR_BANK2;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);
	outb(value, baseio + MOXA_MUST_ENUM_REGISTER);
	outb(oldlcr, baseio + UART_LCR);
}

static void SET_MOXA_MUST_NO_SOFTWARE_FLOW_CONTROL(unsigned long baseio)
{
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr &= ~MOXA_MUST_EFR_SF_MASK;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);
	outb(oldlcr, baseio + UART_LCR);
}

static void ENABLE_MOXA_MUST_XON_ANY_FLOW_CONTROL(unsigned long baseio)
{
	u8	oldmcr;

	oldmcr = inb((baseio)+UART_MCR);
	oldmcr |= MOXA_MUST_MCR_XON_ANY;
	outb(oldmcr, (baseio)+UART_MCR);
}

static void DISABLE_MOXA_MUST_XON_ANY_FLOW_CONTROL(unsigned long baseio)
{
	u8	oldmcr;

	oldmcr = inb((baseio)+UART_MCR);
	oldmcr &= ~MOXA_MUST_MCR_XON_ANY;
	outb(oldmcr, (baseio)+UART_MCR);
}

static void mxser_enable_must_tx_software_flow_control(unsigned long baseio)
{
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr &= ~MOXA_MUST_EFR_SF_TX_MASK;
	efr |= MOXA_MUST_EFR_SF_TX1;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);
	outb(oldlcr, baseio + UART_LCR);
}

static void mxser_disable_must_tx_software_flow_control(unsigned long baseio)
{
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr &= ~MOXA_MUST_EFR_SF_TX_MASK;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);
	outb(oldlcr, baseio + UART_LCR);
}

static void mxser_enable_must_rx_software_flow_control(unsigned long baseio)
{
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr &= ~MOXA_MUST_EFR_SF_RX_MASK;
	efr |= MOXA_MUST_EFR_SF_RX1;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);
	outb(oldlcr, baseio + UART_LCR);
}

static void mxser_disable_must_rx_software_flow_control(unsigned long baseio)
{
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr &= ~MOXA_MUST_EFR_SF_RX_MASK;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);
	outb(oldlcr, baseio + UART_LCR);
}

static void process_txrx_fifo(struct mxser_port *info)
{
	info->rx_trigger = rx_trigger;
	info->xmit_fifo_size = xmit_fifo_size;
}

static unsigned char mxser_get_msr(int baseaddr, int mode, int port)
{
	static unsigned char mxser_msr[MXSER_PORTS + 1];
	unsigned char status = 0;

	status = inb(baseaddr + UART_MSR);

	mxser_msr[port] &= 0x0F;
	mxser_msr[port] |= status;
	status = mxser_msr[port];
	if (mode)
		mxser_msr[port] = 0;

	return status;
}

static int mxser_carrier_raised(struct tty_port *port)
{
	struct mxser_port *mp = container_of(port, struct mxser_port, port);
	return (inb(mp->ioaddr + UART_MSR) & UART_MSR_DCD)?1:0;
}

static void mxser_dtr_rts(struct tty_port *port, int on)
{
	struct mxser_port *mp = container_of(port, struct mxser_port, port);
	unsigned long flags;

	spin_lock_irqsave(&mp->slock, flags);
	if (on)
		outb(inb(mp->ioaddr + UART_MCR) |
			UART_MCR_DTR | UART_MCR_RTS, mp->ioaddr + UART_MCR);
	else
		outb(inb(mp->ioaddr + UART_MCR)&~(UART_MCR_DTR | UART_MCR_RTS),
			mp->ioaddr + UART_MCR);
	spin_unlock_irqrestore(&mp->slock, flags);
}

static int mxser_set_baud(struct tty_struct *tty, long newspd)
{
	struct mxser_port *info = tty->driver_data;
	int quot = 0, baud;
	unsigned char cval;

	if (!info->ioaddr)
		return -1;

	if (newspd > info->max_baud)
		return -1;

	if (newspd == 134) {
		quot = 2 * info->baud_base / 269;
		tty_encode_baud_rate(tty, 134, 134);
	} else if (newspd) {
		quot = info->baud_base / newspd;
		if (quot == 0)
			quot = 1;
		baud = info->baud_base/quot;
		tty_encode_baud_rate(tty, baud, baud);
	} else {
		quot = 0;
	}

	info->timeout = ((info->xmit_fifo_size * HZ * 10 * quot) / info->baud_base);
	info->timeout += HZ / 50;	/* Add .02 seconds of slop */

	if (quot) {
		info->MCR |= UART_MCR_DTR;
		outb(info->MCR, info->ioaddr + UART_MCR);
	} else {
		info->MCR &= ~UART_MCR_DTR;
		outb(info->MCR, info->ioaddr + UART_MCR);
		return 0;
	}

	cval = inb(info->ioaddr + UART_LCR);

	outb(cval | UART_LCR_DLAB, info->ioaddr + UART_LCR);	/* set DLAB */

	outb(quot & 0xff, info->ioaddr + UART_DLL);	/* LS of divisor */
	outb(quot >> 8, info->ioaddr + UART_DLM);	/* MS of divisor */
	outb(cval, info->ioaddr + UART_LCR);	/* reset DLAB */

#ifdef BOTHER
	if (C_BAUD(tty) == BOTHER) {
		quot = info->baud_base % newspd;
		quot *= 8;
		if (quot % newspd > newspd / 2) {
			quot /= newspd;
			quot++;
		} else
			quot /= newspd;

		mxser_set_must_enum_value(info->ioaddr, quot);
	} else
#endif
		mxser_set_must_enum_value(info->ioaddr, 0);

	return 0;
}

/*
 * This routine is called to set the UART divisor registers to match
 * the specified baud rate for a serial port.
 */
static int mxser_change_speed(struct tty_struct *tty,
					struct ktermios *old_termios)
{
	struct mxser_port *info = tty->driver_data;
	unsigned cflag, cval, fcr;
	int ret = 0;

	cflag = tty->termios->c_cflag;
	if (!info->ioaddr)
		return ret;

	if (mxser_set_baud_method[tty->index] == 0)
		mxser_set_baud(tty, tty_get_baud_rate(tty));

	/* byte size and parity */
	switch (cflag & CSIZE) {
	case CS5:
		cval = 0x00;
		break;
	case CS6:
		cval = 0x01;
		break;
	case CS7:
		cval = 0x02;
		break;
	case CS8:
		cval = 0x03;
		break;
	default:
		cval = 0x00;
		break;		/* too keep GCC shut... */
	}
	if (cflag & CSTOPB)
		cval |= 0x04;
	if (cflag & PARENB)
		cval |= UART_LCR_PARITY;
	if (!(cflag & PARODD))
		cval |= UART_LCR_EPAR;
	if (cflag & CMSPAR)
		cval |= UART_LCR_SPAR;
	outb(cval, info->ioaddr + UART_LCR);

	fcr = UART_FCR_ENABLE_FIFO;
	fcr |= MOXA_MUST_FCR_GDA_MODE_ENABLE;
	outb(fcr, info->ioaddr + UART_FCR);	/* set fcr */
	mxser_set_must_fifo_value(info->ioaddr);

	/* CTS flow control flag and modem status interrupts */
	info->IER &= ~UART_IER_MSI;
	info->MCR &= ~UART_MCR_AFE;
	if (cflag & CRTSCTS) {
		info->port.flags |= ASYNC_CTS_FLOW;
		info->IER |= UART_IER_MSI;
		info->MCR |= UART_MCR_AFE;
	} else {
		info->port.flags &= ~ASYNC_CTS_FLOW;
	}
	outb(info->MCR, info->ioaddr + UART_MCR);

	if (cflag & CLOCAL) {
		info->port.flags &= ~ASYNC_CHECK_CD;
	} else {
		info->port.flags |= ASYNC_CHECK_CD;
		info->IER |= UART_IER_MSI;
	}
	outb(info->IER, info->ioaddr + UART_IER);

	/*
	 * Set up parity check flag
	 */
	info->read_status_mask = UART_LSR_OE;
	if (I_INPCK(tty))
		info->read_status_mask |= UART_LSR_FE | UART_LSR_PE;
	if (I_BRKINT(tty) || I_PARMRK(tty))
		info->read_status_mask |= UART_LSR_BI;

	info->ignore_status_mask = 0;

	if (I_IGNBRK(tty)) {
		info->ignore_status_mask |= UART_LSR_BI;
		info->read_status_mask |= UART_LSR_BI;
		/*
		 * If we're ignore parity and break indicators, ignore
		 * overruns too.  (For real raw support).
		 */
		if ( I_IGNPAR(tty) ) {
			info->ignore_status_mask |= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE);
			info->read_status_mask |= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE);
		}
	}

	mxser_set_must_xon1_value(info->ioaddr, START_CHAR(tty));
	mxser_set_must_xoff1_value(info->ioaddr, STOP_CHAR(tty));

	if (I_IXON(tty)) {
		info->IER |= MOXA_MUST_IER_XINT;
		mxser_enable_must_rx_software_flow_control(info->ioaddr);
	} else {
		info->IER &= ~MOXA_MUST_IER_XINT;
		mxser_disable_must_rx_software_flow_control(info->ioaddr);
	}
	outb(info->IER, info->ioaddr + UART_IER);

	if (I_IXOFF(tty))
		mxser_enable_must_tx_software_flow_control(info->ioaddr);
	else
		mxser_disable_must_tx_software_flow_control(info->ioaddr);

	if ( I_IXANY(tty) ) {
                info->MCR |= MOXA_MUST_MCR_XON_ANY;
                ENABLE_MOXA_MUST_XON_ANY_FLOW_CONTROL(info->ioaddr);
        } else {
                info->MCR &= ~MOXA_MUST_MCR_XON_ANY;
                DISABLE_MOXA_MUST_XON_ANY_FLOW_CONTROL(info->ioaddr);
        }
	outb(info->MCR, info->ioaddr + UART_MCR);

	return ret;
}

static void mxser_check_modem_status(struct tty_struct *tty,
				struct mxser_port *port, int status)
{
	/* update input line counters */
	if (status & UART_MSR_TERI)
		port->icount.rng++;
	if (status & UART_MSR_DDSR)
		port->icount.dsr++;
	if (status & UART_MSR_DDCD)
		port->icount.dcd++;
	if (status & UART_MSR_DCTS)
		port->icount.cts++;
	port->mon_data.modem_status = status;
	wake_up_interruptible(&port->port.delta_msr_wait);

	if ((port->port.flags & ASYNC_CHECK_CD) && (status & UART_MSR_DDCD)) {
		if (status & UART_MSR_DCD)
			wake_up_interruptible(&port->port.open_wait);
	}

	if ( port->port.flags & ASYNC_CTS_FLOW ) {
		if (tty->hw_stopped) {
			if (status & UART_MSR_CTS) {
				tty->hw_stopped = 0;
				tty_wakeup(tty);
				tty->ops->start(tty);  //Re-enable TX interrupt. addded by Vincent Wu 2011-12-01
			}
		} else {
			if (!(status & UART_MSR_CTS))
				tty->hw_stopped = 1;
		}
	}
}

static int mxser_activate(struct tty_port *port, struct tty_struct *tty)
{
	struct mxser_port *info = container_of(port, struct mxser_port, port);
	unsigned long page;
	unsigned long flags;

	page = __get_free_page(GFP_KERNEL);
	if (!page)
		return -ENOMEM;

	spin_lock_irqsave(&info->slock, flags);

	if (!info->ioaddr || !info->type) {
		set_bit(TTY_IO_ERROR, &tty->flags);
		free_page(page);
		spin_unlock_irqrestore(&info->slock, flags);
		return 0;
	}
	info->port.xmit_buf = (unsigned char *) page;

	/*
	 * Clear the FIFO buffers and disable them
	 * (they will be reenabled in mxser_change_speed())
	 */
	if (info->board->chip_flag)
		outb((UART_FCR_CLEAR_RCVR |
			UART_FCR_CLEAR_XMIT |
			MOXA_MUST_FCR_GDA_MODE_ENABLE), info->ioaddr + UART_FCR);
	else
		outb((UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT),
			info->ioaddr + UART_FCR);

	/*
	 * At this point there's no way the LSR could still be 0xFF;
	 * if it is, then bail out, because there's likely no UART
	 * here.
	 */
	if (inb(info->ioaddr + UART_LSR) == 0xff) {
		spin_unlock_irqrestore(&info->slock, flags);
		if (capable(CAP_SYS_ADMIN)) {
			set_bit(TTY_IO_ERROR, &tty->flags);
			return 0;
		} else
			return -ENODEV;
	}

	/*
	 * Clear the interrupt registers.
	 */
	(void) inb(info->ioaddr + UART_LSR);
	(void) inb(info->ioaddr + UART_RX);
	(void) inb(info->ioaddr + UART_IIR);
	(void) inb(info->ioaddr + UART_MSR);

	/*
	 * Now, initialize the UART
	 */
	outb(UART_LCR_WLEN8, info->ioaddr + UART_LCR);	/* reset DLAB */
	info->MCR = UART_MCR_DTR | UART_MCR_RTS;
	outb(info->MCR, info->ioaddr + UART_MCR);

	/*
	 * Finally, enable interrupts
	 */
	info->IER = UART_IER_MSI | UART_IER_RLSI | UART_IER_RDI | MOXA_MUST_IER_EGDAI;
	outb(info->IER, info->ioaddr + UART_IER);	/* enable interrupts */

	/*
	 * And clear the interrupt registers again for luck.
	 */
	(void) inb(info->ioaddr + UART_LSR);
	(void) inb(info->ioaddr + UART_RX);
	(void) inb(info->ioaddr + UART_IIR);
	(void) inb(info->ioaddr + UART_MSR);

	clear_bit(TTY_IO_ERROR, &tty->flags);
	info->xmit_cnt = info->xmit_head = info->xmit_tail = 0;

	/*
	 * and set the speed of the serial port
	 */
	mxser_change_speed(tty, NULL);
	spin_unlock_irqrestore(&info->slock, flags);

	return 0;
}

/*
 * This routine will shutdown a serial port
 */
static void mxser_shutdown_port(struct tty_port *port)
{
	struct mxser_port *info = container_of(port, struct mxser_port, port);
	unsigned long flags;

	spin_lock_irqsave(&info->slock, flags);

	/*
	 * clear delta_msr_wait queue to avoid mem leaks: we may free the irq
	 * here so the queue might never be waken up
	 */
	wake_up_interruptible(&info->port.delta_msr_wait);

	/*
	 * Free the xmit buffer, if necessary
	 */
	if (info->port.xmit_buf) {
		free_page((unsigned long) info->port.xmit_buf);
		info->port.xmit_buf = NULL;
	}

	info->IER = 0;
	outb(0x00, info->ioaddr + UART_IER);

	/* clear Rx/Tx FIFO's */
	outb(UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT |
			MOXA_MUST_FCR_GDA_MODE_ENABLE,
			info->ioaddr + UART_FCR);

	/* read data port to reset things */
	(void) inb(info->ioaddr + UART_RX);

	SET_MOXA_MUST_NO_SOFTWARE_FLOW_CONTROL(info->ioaddr);

	spin_unlock_irqrestore(&info->slock, flags);
}

/*
 * This routine is called whenever a serial port is opened.  It
 * enables interrupts for a serial port, linking in its async structure into
 * the IRQ chain.   It also performs the serial-specific
 * initialization for the tty structure.
 */
static int mxser_open(struct tty_struct *tty, struct file *filp)
{
	struct mxser_port *info;
	int line;

	line = tty->index;
	if (line == MXSER_PORTS)
		return 0;
	if (line < 0 || line > MXSER_PORTS)
		return -ENODEV;
	info = &mxser_boards[line / MXSER_PORTS_PER_BOARD].ports[line % MXSER_PORTS_PER_BOARD];
	if (!info->ioaddr)
		return -ENODEV;

	tty->driver_data = info;
	tty->low_latency = 1;
	return tty_port_open(&info->port, tty, filp);
}

static void mxser_flush_buffer(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;
	char fcr;
	unsigned long flags;


	spin_lock_irqsave(&info->slock, flags);
	info->xmit_cnt = info->xmit_head = info->xmit_tail = 0;

	fcr = inb(info->ioaddr + UART_FCR);
	outb((fcr | UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT),
		info->ioaddr + UART_FCR);
	outb(fcr, info->ioaddr + UART_FCR);

	spin_unlock_irqrestore(&info->slock, flags);

	tty_wakeup(tty);
}


static void mxser_close_port(struct tty_port *port)
{
	struct mxser_port *info = container_of(port, struct mxser_port, port);
	unsigned long timeout;
	/*
	 * At this point we stop accepting input.  To do this, we
	 * disable the receive line status interrupts, and tell the
	 * interrupt driver to stop checking the data ready bit in the
	 * line status register.
	 */
	info->IER &= ~(UART_IER_RLSI | MOXA_MUST_RECV_ISR);
	outb(info->IER, info->ioaddr + UART_IER);

	/*
	 * Before we drop DTR, make sure the UART transmitter
	 * has completely drained; this is especially
	 * important if there is a transmit FIFO!
	 */
	timeout = jiffies + HZ;
	while (!(inb(info->ioaddr + UART_LSR) & UART_LSR_TEMT)) {
		schedule_timeout_interruptible(5);
		if (time_after(jiffies, timeout))
			break;
	}
}

/*
 * This routine is called when the serial port gets closed.  First, we
 * wait for the last remaining data to be sent.  Then, we unlink its
 * async structure from the interrupt chain if necessary, and we free
 * that IRQ if nothing is left in the chain.
 */
static void mxser_close(struct tty_struct *tty, struct file *filp)
{
	struct mxser_port *info = tty->driver_data;
	struct tty_port *port = &info->port;

	if (tty->index == MXSER_PORTS || info == NULL)
		return;
	if (tty_port_close_start(port, tty, filp) == 0)
		return;
	mutex_lock(&port->mutex);
	mxser_close_port(port);
	mxser_flush_buffer(tty);
	mxser_shutdown_port(port);
	clear_bit(ASYNCB_INITIALIZED, &port->flags);
	mutex_unlock(&port->mutex);
	/* Right now the tty_port set is done outside of the close_end helper
	   as we don't yet have everyone using refcounts */	
	tty_port_close_end(port, tty);
	tty_port_tty_set(port, NULL);
}

static int mxser_write(struct tty_struct *tty, const unsigned char *buf, int count)
{
	int c, total = 0;
	struct mxser_port *info = tty->driver_data;
	unsigned long flags;
	
	if (!info->port.xmit_buf)
		return 0;

	spin_lock_irqsave(&info->slock, flags);
	while (1) {
		c = min_t(int, count, min(SERIAL_XMIT_SIZE - info->xmit_cnt - 1,
					  SERIAL_XMIT_SIZE - info->xmit_head));
		if (c <= 0)
			break;

		memcpy(info->port.xmit_buf + info->xmit_head, buf, c);
		info->xmit_head = (info->xmit_head + c) &
				  (SERIAL_XMIT_SIZE - 1);
		info->xmit_cnt += c;

		buf += c;
		count -= c;
		total += c;
	}

	if (info->xmit_cnt && !tty->stopped) {
		if ( !tty->hw_stopped &&
		     !(info->IER & UART_IER_THRI) ) {
			info->IER |= UART_IER_THRI;
			outb(info->IER, info->ioaddr + UART_IER);
		}
	}
	spin_unlock_irqrestore(&info->slock, flags);

	return total;
}

static int mxser_put_char(struct tty_struct *tty, unsigned char ch)
{
	struct mxser_port *info = tty->driver_data;
	unsigned long flags;

	if (!info->port.xmit_buf)
		return 0;

	if (info->xmit_cnt >= SERIAL_XMIT_SIZE - 1)
		return 0;

	spin_lock_irqsave(&info->slock, flags);
	info->port.xmit_buf[info->xmit_head++] = ch;
	info->xmit_head &= SERIAL_XMIT_SIZE - 1;
	info->xmit_cnt++;
	spin_unlock_irqrestore(&info->slock, flags);
	if (!tty->stopped) {
		if (!tty->hw_stopped ) {
			spin_lock_irqsave(&info->slock, flags);
			outb(info->IER & ~UART_IER_THRI, info->ioaddr + UART_IER);
			info->IER |= UART_IER_THRI;
			outb(info->IER, info->ioaddr + UART_IER);
			spin_unlock_irqrestore(&info->slock, flags);
		}
	}
	return 1;
}


static void mxser_flush_chars(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;
	unsigned long flags;

	if (info->xmit_cnt <= 0 || tty->stopped || !info->port.xmit_buf || tty->hw_stopped )
		return;

	spin_lock_irqsave(&info->slock, flags);

	outb(info->IER & ~UART_IER_THRI, info->ioaddr + UART_IER);
	info->IER |= UART_IER_THRI;
	outb(info->IER, info->ioaddr + UART_IER);

	spin_unlock_irqrestore(&info->slock, flags);
}

static int mxser_write_room(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;
	int ret;

	ret = SERIAL_XMIT_SIZE - info->xmit_cnt - 1;
	return ret < 0 ? 0 : ret;
}

static int mxser_chars_in_buffer(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;
	return info->xmit_cnt;
}

/*
 * ------------------------------------------------------------
 * friends of mxser_ioctl()
 * ------------------------------------------------------------
 */
static int mxser_get_serial_info(struct tty_struct *tty,
		struct serial_struct __user *retinfo)
{
	struct mxser_port *info = tty->driver_data;
	struct serial_struct tmp = {
		.type = info->type,
		.line = tty->index,
		.port = info->ioaddr,
		.irq = info->board->irq,
		.flags = info->port.flags,
		.baud_base = info->baud_base,
		.close_delay = info->port.close_delay,
		.closing_wait = info->port.closing_wait,
		.custom_divisor = info->custom_divisor,
		.hub6 = 0
	};
	if (copy_to_user(retinfo, &tmp, sizeof(*retinfo)))
		return -EFAULT;
	return 0;
}

static int mxser_set_serial_info(struct tty_struct *tty,
		struct serial_struct __user *new_info)
{
	struct mxser_port *info = tty->driver_data;
	struct tty_port *port = &info->port;
	struct serial_struct new_serial;
	speed_t baud;
	unsigned long sl_flags;
	unsigned int flags;
	int retval = 0;

	if (!new_info || !info->ioaddr)
		return -ENODEV;
	if (copy_from_user(&new_serial, new_info, sizeof(new_serial)))
		return -EFAULT;

	if (new_serial.irq != info->board->irq ||
			new_serial.port != info->ioaddr)
		return -EINVAL;

	flags = port->flags & ASYNC_SPD_MASK;

	if (!capable(CAP_SYS_ADMIN)) {
		if ((new_serial.baud_base != info->baud_base) ||
				(new_serial.close_delay != info->port.close_delay) ||
				((new_serial.flags & ~ASYNC_USR_MASK) != (info->port.flags & ~ASYNC_USR_MASK)))
			return -EPERM;
		info->port.flags = ((info->port.flags & ~ASYNC_USR_MASK) |
				(new_serial.flags & ASYNC_USR_MASK));
	} else {
		/*
		 * OK, past this point, all the error checking has been done.
		 * At this point, we start making changes.....
		 */
		port->flags = ((port->flags & ~ASYNC_FLAGS) |
				(new_serial.flags & ASYNC_FLAGS));
		port->close_delay = new_serial.close_delay * HZ / 100;
		port->closing_wait = new_serial.closing_wait * HZ / 100;
		tty->low_latency = (port->flags & ASYNC_LOW_LATENCY) ? 1 : 0;
		if ((port->flags & ASYNC_SPD_MASK) == ASYNC_SPD_CUST &&
				(new_serial.baud_base != info->baud_base ||
				new_serial.custom_divisor !=
				info->custom_divisor)) {
			if (new_serial.custom_divisor == 0)
				return -EINVAL;
			baud = new_serial.baud_base / new_serial.custom_divisor;
			tty_encode_baud_rate(tty, baud, baud);
		}
	}

	info->type = new_serial.type;

	process_txrx_fifo(info);

	if (test_bit(ASYNCB_INITIALIZED, &port->flags)) {
		if (flags != (port->flags & ASYNC_SPD_MASK)) {
			spin_lock_irqsave(&info->slock, sl_flags);
			mxser_change_speed(tty, NULL);
			spin_unlock_irqrestore(&info->slock, sl_flags);
		}
	} else {
		retval = mxser_activate(port, tty);
		if (retval == 0)
			set_bit(ASYNCB_INITIALIZED, &port->flags);
	}
	return retval;
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
static int mxser_get_lsr_info(struct mxser_port *info,
		unsigned int __user *value)
{
	unsigned char status;
	unsigned int result;
	unsigned long flags;

	spin_lock_irqsave(&info->slock, flags);
	status = inb(info->ioaddr + UART_LSR);
	spin_unlock_irqrestore(&info->slock, flags);
	result = ((status & UART_LSR_TEMT) ? TIOCSER_TEMT : 0);
	return put_user(result, value);
}

static int mxser_tiocmget(struct tty_struct *tty, struct file *file)
{
	struct mxser_port *info = tty->driver_data;
	unsigned char control, status;
	unsigned long flags;


	if (tty->index == MXSER_PORTS)
		return -ENOIOCTLCMD;
	if (test_bit(TTY_IO_ERROR, &tty->flags))
		return -EIO;

	control = info->MCR;

	spin_lock_irqsave(&info->slock, flags);
	status = inb(info->ioaddr + UART_MSR);
	if (status & UART_MSR_ANY_DELTA)
		mxser_check_modem_status(tty, info, status);
	spin_unlock_irqrestore(&info->slock, flags);
	return ((control & UART_MCR_RTS) ? TIOCM_RTS : 0) |
		    ((control & UART_MCR_DTR) ? TIOCM_DTR : 0) |
		    ((status & UART_MSR_DCD) ? TIOCM_CAR : 0) |
		    ((status & UART_MSR_RI) ? TIOCM_RNG : 0) |
		    ((status & UART_MSR_DSR) ? TIOCM_DSR : 0) |
		    ((status & UART_MSR_CTS) ? TIOCM_CTS : 0);
}

static int mxser_tiocmset(struct tty_struct *tty, struct file *file,
		unsigned int set, unsigned int clear)
{
	struct mxser_port *info = tty->driver_data;
	unsigned long flags;


	if (tty->index == MXSER_PORTS)
		return -ENOIOCTLCMD;
	if (test_bit(TTY_IO_ERROR, &tty->flags))
		return -EIO;

	spin_lock_irqsave(&info->slock, flags);

	if (set & TIOCM_RTS)
		info->MCR |= UART_MCR_RTS;
	if (set & TIOCM_DTR)
		info->MCR |= UART_MCR_DTR;

	if (clear & TIOCM_RTS)
		info->MCR &= ~UART_MCR_RTS;
	if (clear & TIOCM_DTR)
		info->MCR &= ~UART_MCR_DTR;

	outb(info->MCR, info->ioaddr + UART_MCR);
	spin_unlock_irqrestore(&info->slock, flags);
	return 0;
}

static int mxser_ioctl_special(unsigned int cmd, void __user *argp)
{
	struct mxser_port *ip;
	struct tty_port *port;
	struct tty_struct *tty;
	int result, status;
	unsigned int i, j;
	int ret = 0;

	switch (cmd) {
	case MOXA_GET_MAJOR:
		if (printk_ratelimit())
			printk(KERN_WARNING "mxser: '%s' uses deprecated ioctl "
					"%x (GET_MAJOR), fix your userspace\n",
					current->comm, cmd);
		return put_user(ttymajor, (int __user *)argp);

	case MOXA_CHKPORTENABLE:
		result = 0;
		for (i = 0; i < MXSER_BOARDS; i++)
			for (j = 0; j < MXSER_PORTS_PER_BOARD; j++)
				if (mxser_boards[i].ports[j].ioaddr)
					result |= (1 << i);
		return put_user(result, (unsigned long __user *)argp);
	case MOXA_GETDATACOUNT:
		/* The receive side is locked by port->slock but it isn't
		   clear that an exact snapshot is worth copying here */
		if (copy_to_user(argp, &mxvar_log, sizeof(mxvar_log)))
			ret = -EFAULT;
		return ret;
	case MOXA_GETMSTATUS: {
		struct mxser_mstatus ms, __user *msu = argp;
		for (i = 0; i < MXSER_BOARDS; i++)
			for (j = 0; j < MXSER_PORTS_PER_BOARD; j++) {
				unsigned long	flags;
				ip = &mxser_boards[i].ports[j];
				port = &ip->port;
				memset(&ms, 0, sizeof(ms));

				mutex_lock(&port->mutex);
				if (!ip->ioaddr)
					goto copy;
				
				tty = tty_port_tty_get(port);

				if (!tty || !tty->termios)
					ms.cflag = ip->normal_termios.c_cflag;
				else
					ms.cflag = tty->termios->c_cflag;
				tty_kref_put(tty);
				spin_lock_irqsave(&ip->slock, flags);
				status = inb(ip->ioaddr + UART_MSR);
				spin_unlock_irqrestore(&ip->slock, flags);
				if (status & UART_MSR_DCD)
					ms.dcd = 1;
				if (status & UART_MSR_DSR)
					ms.dsr = 1;
				if (status & UART_MSR_CTS)
					ms.cts = 1;
			copy:
				mutex_unlock(&port->mutex);
				if (copy_to_user(msu, &ms, sizeof(ms)))
					return -EFAULT;
				msu++;
			}
		return 0;
	}
	case MOXA_ASPP_MON_EXT: {
		struct mxser_mon_ext *me; /* it's 2k, stack unfriendly */
		unsigned int cflag, iflag, p;
		u8 opmode;
		unsigned long	flags;

		me = kzalloc(sizeof(*me), GFP_KERNEL);
		if (!me)
			return -ENOMEM;

		for (i = 0, p = 0; i < MXSER_BOARDS; i++) {
			for (j = 0; j < MXSER_PORTS_PER_BOARD; j++, p++) {
				if (p >= ARRAY_SIZE(me->rx_cnt)) {
					i = MXSER_BOARDS;
					break;
				}
				ip = &mxser_boards[i].ports[j];
				port = &ip->port;

				mutex_lock(&port->mutex);
				if (!ip->ioaddr) {
					mutex_unlock(&port->mutex);
					continue;
				}

				spin_lock_irqsave(&ip->slock, flags);
				status = mxser_get_msr(ip->ioaddr, 0, p);

				if (status & UART_MSR_TERI)
					ip->icount.rng++;
				if (status & UART_MSR_DDSR)
					ip->icount.dsr++;
				if (status & UART_MSR_DDCD)
					ip->icount.dcd++;
				if (status & UART_MSR_DCTS)
					ip->icount.cts++;

				ip->mon_data.modem_status = status;
				me->rx_cnt[p] = ip->mon_data.rxcnt;
				me->tx_cnt[p] = ip->mon_data.txcnt;
				me->up_rxcnt[p] = ip->mon_data.up_rxcnt;
				me->up_txcnt[p] = ip->mon_data.up_txcnt;
				me->modem_status[p] =
					ip->mon_data.modem_status;
				spin_unlock_irqrestore(&ip->slock, flags);

				tty = tty_port_tty_get(&ip->port);

				if (!tty || !tty->termios) {
					cflag = ip->normal_termios.c_cflag;
					iflag = ip->normal_termios.c_iflag;
					me->baudrate[p] = tty_termios_baud_rate(&ip->normal_termios);
				} else {
					cflag = tty->termios->c_cflag;
					iflag = tty->termios->c_iflag;
					me->baudrate[p] = tty_get_baud_rate(tty);
				}
				tty_kref_put(tty);

				me->databits[p] = cflag & CSIZE;
				me->stopbits[p] = cflag & CSTOPB;
				me->parity[p] = cflag & (PARENB | PARODD |
						CMSPAR);

				if (cflag & CRTSCTS)
					me->flowctrl[p] |= 0x03;

				if (iflag & (IXON | IXOFF))
					me->flowctrl[p] |= 0x0C;

				if (ip->type == PORT_16550A)
					me->fifo[p] = 1;

				opmode = inb(ip->opmode_ioaddr)>>((p % 4) * 2);
				opmode &= OP_MODE_MASK;
				me->iftype[p] = opmode;
				mutex_unlock(&port->mutex);
			}
		}
		if (copy_to_user(argp, me, sizeof(*me)))
			ret = -EFAULT;
		kfree(me);
		return ret;
	}
	default:
		return -ENOIOCTLCMD;
	}
	return 0;
}

static int mxser_cflags_changed(struct mxser_port *info, unsigned long arg,
		struct async_icount *cprev)
{
	struct async_icount cnow;
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&info->slock, flags);
	cnow = info->icount;	/* atomic copy */
	spin_unlock_irqrestore(&info->slock, flags);

	ret =	((arg & TIOCM_RNG) && (cnow.rng != cprev->rng)) ||
		((arg & TIOCM_DSR) && (cnow.dsr != cprev->dsr)) ||
		((arg & TIOCM_CD)  && (cnow.dcd != cprev->dcd)) ||
		((arg & TIOCM_CTS) && (cnow.cts != cprev->cts));

	*cprev = cnow;

	return ret;
}

static int mxser_ioctl(struct tty_struct *tty, struct file *file,
		unsigned int cmd, unsigned long arg)
{
	struct mxser_port *info = tty->driver_data;
	struct tty_port *port = &info->port;
	struct async_icount cnow;
	unsigned long flags;
	void __user *argp = (void __user *)arg;
	int retval;

	if (tty->index == MXSER_PORTS)
		return mxser_ioctl_special(cmd, argp);

	if (cmd == MOXA_SET_OP_MODE || cmd == MOXA_GET_OP_MODE) {
		int p;
		unsigned long opmode;
		int shiftbit;
		unsigned long	val;

		p = tty->index % 4;
		if (cmd == MOXA_SET_OP_MODE) {
			if (get_user(opmode, (int __user *) argp))
				return -EFAULT;
			if (opmode != RS232_MODE &&
					opmode != RS485_2WIRE_MODE &&
					opmode != RS422_MODE &&
					opmode != RS485_4WIRE_MODE)
				return -EFAULT;
			shiftbit = (p + 2) * 2;
			spin_lock_irqsave(&info->slock, flags);
			val = inl(info->opmode_ioaddr);
			val &= (~(3 << shiftbit));
			val |= (opmode << shiftbit);
			outl(val, info->opmode_ioaddr);
			spin_unlock_irqrestore(&info->slock, flags);
		} else {
			shiftbit = (p + 2) * 2;
			spin_lock_irqsave(&info->slock, flags);
			opmode = inl(info->opmode_ioaddr) >> shiftbit;
			spin_unlock_irqrestore(&info->slock, flags);
			opmode &= OP_MODE_MASK;
			if (put_user(opmode, (int __user *)argp))
				return -EFAULT;
		}
		return 0;
	}

	if ( cmd == MOXA_SET_SPECIAL_BAUD_RATE || cmd == MOXA_GET_SPECIAL_BAUD_RATE ) {
		int     speed, i;
		if ( cmd == MOXA_SET_SPECIAL_BAUD_RATE ) {
			if (get_user(speed, (int __user *) argp))
				return -EFAULT;
			if ( speed <=0 || speed > info->max_baud )
				return -EFAULT;
			tty->termios->c_cflag &= ~(CBAUD | CBAUDEX);
			for ( i=0; i<BAUD_TABLE_NO && speed != mxvar_baud_table[i].speed; i++ );
			if ( i == BAUD_TABLE_NO ) {
                                tty->termios->c_cflag |= B4000000;
                        } else {
                                tty->termios->c_cflag |= mxvar_baud_table[i].index;
			}
			info->speed = speed;
			spin_lock_irqsave(&info->slock, flags);
			mxser_change_speed(tty, NULL);
			spin_unlock_irqrestore(&info->slock, flags);
		} else {
			if (put_user(info->speed, (int __user *)argp))
				return -EFAULT;
		}
		return 0;
	}

	if (cmd != TIOCGSERIAL && cmd != TIOCMIWAIT &&
			test_bit(TTY_IO_ERROR, &tty->flags))
		return -EIO;

	switch (cmd) {
	case TIOCGSERIAL:
		mutex_lock(&port->mutex);
		retval = mxser_get_serial_info(tty, argp);
		mutex_unlock(&port->mutex);
		return retval;
	case TIOCSSERIAL:
		mutex_lock(&port->mutex);
		retval = mxser_set_serial_info(tty, argp);
		mutex_unlock(&port->mutex);
		return retval;
	case TIOCSERGETLSR:	/* Get line status register */
		return  mxser_get_lsr_info(info, argp);
		/*
		 * Wait for any of the 4 modem inputs (DCD,RI,DSR,CTS) to change
		 * - mask passed in arg for lines of interest
		 *   (use |'ed TIOCM_RNG/DSR/CD/CTS for masking)
		 * Caller should use TIOCGICOUNT to see which one it was
		 */
	case TIOCMIWAIT:
		spin_lock_irqsave(&info->slock, flags);
		cnow = info->icount;	/* note the counters on entry */
		spin_unlock_irqrestore(&info->slock, flags);

		return wait_event_interruptible(info->port.delta_msr_wait,
				mxser_cflags_changed(info, arg, &cnow));
	case MOXA_HighSpeedOn:
		return put_user(info->baud_base != 115200 ? 1 : 0, (int __user *)argp);
	case MOXA_SDS_RSTICOUNTER:
		spin_lock_irqsave(&info->slock, flags);
		info->mon_data.rxcnt = 0;
		info->mon_data.txcnt = 0;
		spin_unlock_irqrestore(&info->slock, flags);
		return 0;

	case MOXA_ASPP_OQUEUE:{
		int len, lsr;

		len = mxser_chars_in_buffer(tty);
		spin_lock_irqsave(&info->slock, flags);
		lsr = inb(info->ioaddr + UART_LSR) & UART_LSR_THRE;
		spin_unlock_irqrestore(&info->slock, flags);
		len += (lsr ? 0 : 1);

		return put_user(len, (int __user *)argp);
	}
	case MOXA_ASPP_MON: {
		int mcr, status;

		spin_lock_irqsave(&info->slock, flags);
		status = mxser_get_msr(info->ioaddr, 1, tty->index);
		mxser_check_modem_status(tty, info, status);

		mcr = inb(info->ioaddr + UART_MCR);
		spin_unlock_irqrestore(&info->slock, flags);

		if (mcr & MOXA_MUST_MCR_XON_FLAG)
			info->mon_data.hold_reason &= ~NPPI_NOTIFY_XOFFHOLD;
		else
			info->mon_data.hold_reason |= NPPI_NOTIFY_XOFFHOLD;

		if (mcr & MOXA_MUST_MCR_TX_XON)
			info->mon_data.hold_reason &= ~NPPI_NOTIFY_XOFFXENT;
		else
			info->mon_data.hold_reason |= NPPI_NOTIFY_XOFFXENT;

		if (tty->hw_stopped)
			info->mon_data.hold_reason |= NPPI_NOTIFY_CTSHOLD;
		else
			info->mon_data.hold_reason &= ~NPPI_NOTIFY_CTSHOLD;

		if (copy_to_user(argp, &info->mon_data,
				sizeof(struct mxser_mon)))
			return -EFAULT;

		return 0;
	}
	case MOXA_ASPP_LSTATUS: {
		if (put_user(info->err_shadow, (unsigned char __user *)argp))
			return -EFAULT;

		info->err_shadow = 0;
		return 0;
	}
	case MOXA_SET_BAUD_METHOD: {
		int method;

		if (get_user(method, (int __user *)argp))
			return -EFAULT;
		mxser_set_baud_method[tty->index] = method;
		return put_user(method, (int __user *)argp);
	}
	default:
		return -ENOIOCTLCMD;
	}
	return 0;
}

	/*
	 * Get counter of input serial line interrupts (DCD,RI,DSR,CTS)
	 * Return: write counters to the user passed counter struct
	 * NB: both 1->0 and 0->1 transitions are counted except for
	 *     RI where only 0->1 is counted.
	 */

static int mxser_get_icount(struct tty_struct *tty,
		struct serial_icounter_struct *icount)

{
	struct mxser_port *info = tty->driver_data;
	struct async_icount cnow;
	unsigned long flags;

	spin_lock_irqsave(&info->slock, flags);
	cnow = info->icount;
	spin_unlock_irqrestore(&info->slock, flags);

	icount->frame = cnow.frame;
	icount->brk = cnow.brk;
	icount->overrun = cnow.overrun;
	icount->buf_overrun = cnow.buf_overrun;
	icount->parity = cnow.parity;
	icount->rx = cnow.rx;
	icount->tx = cnow.tx;
	icount->cts = cnow.cts;
	icount->dsr = cnow.dsr;
	icount->rng = cnow.rng;
	icount->dcd = cnow.dcd;
	return 0;
}

static void mxser_stoprx(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;

	info->ldisc_stop_rx = 1;
	if ( I_IXOFF(tty) || (tty->termios->c_cflag & CRTSCTS) ) {
		info->IER &= ~MOXA_MUST_RECV_ISR;
		outb(info->IER, info->ioaddr + UART_IER);
	}
}

/*
 * This routine is called by the upper-layer tty layer to signal that
 * incoming characters should be throttled.
 */
static void mxser_throttle(struct tty_struct *tty)
{
//	struct mxser_port	*info = tty->driver_data;

//	if ( !info->ldisc_stop_rx )
		mxser_stoprx(tty);
}

static void mxser_unthrottle(struct tty_struct *tty)
{
	struct mxser_port	*info = tty->driver_data;

	/* startrx */
	info->ldisc_stop_rx = 0;
	if (I_IXOFF(tty)) {
		if (info->x_char)
			info->x_char = 0;
		else {
			info->IER |= MOXA_MUST_RECV_ISR;
			outb(info->IER, info->ioaddr + UART_IER);
		}
	}

	if (tty->termios->c_cflag & CRTSCTS) {
		info->IER |= MOXA_MUST_RECV_ISR;
		outb(info->IER, info->ioaddr + UART_IER);
	}
}

/*
 * mxser_stop() and mxser_start()
 *
 * This routines are called before setting or resetting tty->stopped.
 * They enable or disable transmitter interrupts, as necessary.
 */
static void mxser_stop(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;
	unsigned long flags;

	spin_lock_irqsave(&info->slock, flags);
	if (info->IER & UART_IER_THRI) {
		info->IER &= ~UART_IER_THRI;
		outb(info->IER, info->ioaddr + UART_IER);
	}
	spin_unlock_irqrestore(&info->slock, flags);
}

static void mxser_start(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;
	unsigned long flags;

	spin_lock_irqsave(&info->slock, flags);
	if (info->xmit_cnt && info->port.xmit_buf) {
		outb(info->IER & ~UART_IER_THRI, info->ioaddr + UART_IER);
		info->IER |= UART_IER_THRI;
		outb(info->IER, info->ioaddr + UART_IER);
	}
	spin_unlock_irqrestore(&info->slock, flags);
}

static void mxser_set_termios(struct tty_struct *tty, struct ktermios *old_termios)
{
	struct mxser_port *info = tty->driver_data;
	unsigned long flags;

	spin_lock_irqsave(&info->slock, flags);
	mxser_change_speed(tty, old_termios);
	spin_unlock_irqrestore(&info->slock, flags);

	if ((old_termios->c_cflag & CRTSCTS) &&
			!(tty->termios->c_cflag & CRTSCTS)) {
		tty->hw_stopped = 0;
		mxser_start(tty);
	}

	/* Handle sw stopped */
	if ((old_termios->c_iflag & IXON) &&
			!(tty->termios->c_iflag & IXON)) {
		tty->stopped = 0;
		spin_lock_irqsave(&info->slock, flags);
		mxser_disable_must_rx_software_flow_control(info->ioaddr);
		spin_unlock_irqrestore(&info->slock, flags);
		mxser_start(tty);
	}
}

/*
 * mxser_wait_until_sent() --- wait until the transmitter is empty
 */
static void mxser_wait_until_sent(struct tty_struct *tty, int timeout)
{
	struct mxser_port *info = tty->driver_data;
	unsigned long orig_jiffies, char_time;
	unsigned long flags;
	int lsr;

	if (info->type == PORT_UNKNOWN)
		return;

	if (info->xmit_fifo_size == 0)
		return;		/* Just in case.... */

	orig_jiffies = jiffies;
	/*
	 * Set the check interval to be 1/5 of the estimated time to
	 * send a single character, and make it at least 1.  The check
	 * interval should also be less than the timeout.
	 *
	 * Note: we have to use pretty tight timings here to satisfy
	 * the NIST-PCTS.
	 */
	char_time = (info->timeout - HZ / 50) / info->xmit_fifo_size;
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
	if (!timeout || timeout > 2 * info->timeout)
		timeout = 2 * info->timeout;
#ifdef SERIAL_DEBUG_RS_WAIT_UNTIL_SENT
	printk(KERN_DEBUG "In rs_wait_until_sent(%d) check=%lu...",
		timeout, char_time);
	printk("jiff=%lu...", jiffies);
#endif
	spin_lock_irqsave(&info->slock, flags);
	while (!((lsr = inb(info->ioaddr + UART_LSR)) & UART_LSR_TEMT)) {
#ifdef SERIAL_DEBUG_RS_WAIT_UNTIL_SENT
		printk("lsr = %d (jiff=%lu)...", lsr, jiffies);
#endif
		spin_unlock_irqrestore(&info->slock, flags);
		schedule_timeout_interruptible(char_time);
		spin_lock_irqsave(&info->slock, flags);
		if (signal_pending(current))
			break;
		if (timeout && time_after(jiffies, orig_jiffies + timeout))
			break;
	}
	spin_unlock_irqrestore(&info->slock, flags);
	set_current_state(TASK_RUNNING);

#ifdef SERIAL_DEBUG_RS_WAIT_UNTIL_SENT
	printk("lsr = %d (jiff=%lu)...done\n", lsr, jiffies);
#endif
}

/*
 * This routine is called by tty_hangup() when a hangup is signaled.
 */
static void mxser_hangup(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;

	mxser_flush_buffer(tty);
	tty_port_hangup(&info->port);
}

/*
 * mxser_rs_break() --- routine which turns the break handling on or off
 */
static int mxser_rs_break(struct tty_struct *tty, int break_state)
{
	struct mxser_port *info = tty->driver_data;
	unsigned long flags;

	spin_lock_irqsave(&info->slock, flags);
	if (break_state == -1)
		outb(inb(info->ioaddr + UART_LCR) | UART_LCR_SBC,
			info->ioaddr + UART_LCR);
	else
		outb(inb(info->ioaddr + UART_LCR) & ~UART_LCR_SBC,
			info->ioaddr + UART_LCR);
	spin_unlock_irqrestore(&info->slock, flags);
	return 0;
}

static void mxser_receive_chars(struct tty_struct *tty,
				struct mxser_port *port, unsigned char *status)
{
	unsigned char	ch, gdl;
	int		cnt=0, recv_room;

	recv_room = tty->receive_room;
	if ( *status & (UART_LSR_SPECIAL|MOXA_MUST_LSR_RERR) )
		goto intr_old;

	gdl = inb(port->ioaddr + MOXA_MUST_GDL_REGISTER);
	if (gdl >= recv_room) {
//		if ( !port->ldisc_stop_rx )
			mxser_stoprx(tty);
//		gdl = recv_room;
	}
	while ( gdl-- ) {
		ch = inb(port->ioaddr + UART_RX);
		tty_insert_flip_char(tty, ch, 0);
		cnt++;
	}
	goto end_intr;

intr_old:

	ch = inb(port->ioaddr + UART_RX);
	if ( (*status & UART_LSR_OE) && !(*status & UART_LSR_DR) )
		outb(0x23, port->ioaddr + UART_FCR);
	*status &= port->read_status_mask;
	if (*status & port->ignore_status_mask) {
		goto end_intr;
	} else {
		char flag = 0;
		if (*status & UART_LSR_SPECIAL) {
			if (*status & UART_LSR_BI) {
				port->err_shadow |= NPPI_NOTIFY_BREAK;
				flag = TTY_BREAK;
				port->icount.brk++;

				if (port->port.flags & ASYNC_SAK)
					do_SAK(tty);
			} else if (*status & UART_LSR_PE) {
				port->err_shadow |= NPPI_NOTIFY_PARITY;
				flag = TTY_PARITY;
				port->icount.parity++;
			} else if (*status & UART_LSR_FE) {
				port->err_shadow |= NPPI_NOTIFY_FRAMING;
				flag = TTY_FRAME;
				port->icount.frame++;
			} else if (*status & UART_LSR_OE) {
				port->err_shadow |= NPPI_NOTIFY_HW_OVERRUN;
				flag = TTY_OVERRUN;
				port->icount.overrun++;
			} else
				flag = TTY_BREAK;
			wake_up_interruptible(&port->port.delta_msr_wait);
		}
		tty_insert_flip_char(tty, ch, flag);
		cnt++;
		if (cnt >= recv_room) {
//			if (!port->ldisc_stop_rx)
				mxser_stoprx(tty);
		}
	}

end_intr:
	mxvar_log.rxcnt[tty->index] += cnt;
	port->mon_data.rxcnt += cnt;
	port->mon_data.up_rxcnt += cnt;

	/*
	 * We are called from an interrupt context with &port->slock
	 * being held. Drop it temporarily in order to prevent
	 * recursive locking.
	 */
	tty_flip_buffer_push(tty);
}

static void mxser_transmit_chars(struct tty_struct *tty, struct mxser_port *port)
{
	int		count, cnt;
	unsigned char	*ptr=port->port.xmit_buf;
	unsigned long	ioaddr=port->ioaddr;

	if (port->x_char) {
		outb(port->x_char, ioaddr + UART_TX);
		port->x_char = 0;
		mxvar_log.txcnt[tty->index]++;
		port->mon_data.txcnt++;
		port->mon_data.up_txcnt++;
		port->icount.tx++;
		return;
	}

	if ( ptr == NULL )
		return;

	if ( port->xmit_cnt <= 0 || tty->stopped || tty->hw_stopped ) {
		port->IER &= ~UART_IER_THRI;
		outb(port->IER, ioaddr + UART_IER);
		return;
	}

	cnt = port->xmit_cnt;
	count = port->xmit_fifo_size;
	if ( count >= cnt )
		count = cnt;
	cnt = count;
	{
	int		tail=port->xmit_tail;
	int		tailmask=SERIAL_XMIT_SIZE - 1;
	do {
		outb(ptr[tail], ioaddr + UART_TX);
		tail++;
		tail &= tailmask;
	} while (--count > 0);
	port->xmit_tail = tail;
	}

	port->xmit_cnt -= cnt;
	mxvar_log.txcnt[tty->index] += cnt;

	port->mon_data.txcnt += cnt;
	port->mon_data.up_txcnt += cnt;
	port->icount.tx += cnt;

	if ( port->xmit_cnt < WAKEUP_CHARS )
		tty_wakeup(tty);

	if ( port->xmit_cnt <= 0 ) {
		port->IER &= ~UART_IER_THRI;
		outb(port->IER, ioaddr + UART_IER);
	}
}

/*
 * This is the serial driver's generic interrupt routine
 */
static irqreturn_t mxser_interrupt(int irq, void *dev_id)
{
	unsigned char		iir, status;
	struct mxser_board	*brd = NULL;
	struct mxser_port	*port;
	int 			i, max, irqbits, bits, msr;
	unsigned int		pass_counter = 0;
	int			handled = IRQ_NONE;

	for ( i=0; i<MXSER_BOARDS; i++ )
		if ( dev_id == &mxser_boards[i] ) {
			brd = dev_id;
			break;
		}

	if ( i == MXSER_BOARDS || brd == NULL )
		goto irq_stop;

	max = brd->info->nports;
	//while ( pass_counter++ < MXSER_ISR_PASS_LIMIT ) {
	do {
		irqbits = inb(brd->vector) & brd->vector_mask;
		if ( irqbits == brd->vector_mask )
			break;

		handled = IRQ_HANDLED;
		for ( i=0, bits=4; i < max; i++, irqbits |= bits, bits <<= 1 ) {
			struct tty_struct *tty;

			if (irqbits == brd->vector_mask)
				break;
			if (bits & irqbits)
				continue;

			port = &brd->ports[i];
			iir = inb(port->ioaddr + UART_IIR);
			if (iir & UART_IIR_NO_INT)
				continue;

			iir &= MOXA_MUST_IIR_MASK;
			tty = tty_port_tty_get(&port->port);

			// the data is put on buffer, but appliation is closing,
			// the driver have to continue to send data
			// fixed by Victor Yu. 11-09-2011
			if (!tty || !(port->port.flags & ASYNC_INITIALIZED)) {
				status = inb(port->ioaddr + UART_LSR);
				outb(0x27, port->ioaddr + UART_FCR);
				inb(port->ioaddr + UART_MSR);
				goto go_next_port;
			}

			if ( iir == MOXA_MUST_IIR_XSC ) {
				if ( !(inb(port->ioaddr+UART_MCR) & MOXA_MUST_MCR_XON_FLAG) ) {
					port->icount.buf_overrun++;
					wake_up_interruptible(&port->port.delta_msr_wait);
				}
				goto go_next_port;
			}

			if (iir == MOXA_MUST_IIR_GDA ||
			    iir == MOXA_MUST_IIR_RDA ||
			    iir == MOXA_MUST_IIR_RTO ||
			    iir == MOXA_MUST_IIR_LSR) {
				status = inb(port->ioaddr + UART_LSR);
				mxser_receive_chars(tty, port, &status);
				goto go_next_port;
			}

			if ( iir == UART_IIR_MSI ) {
				msr = inb(port->ioaddr + UART_MSR);
				mxser_check_modem_status(tty, port, msr);
				goto go_next_port;
			}

			if ( iir == UART_IIR_THRI ) {
				mxser_transmit_chars(tty, port);
			}

go_next_port:
			tty_kref_put(tty);
			continue;	// just for compiler requests
		}
	//}
	} while (0);

irq_stop:
	return handled;
}

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
	.break_ctl = mxser_rs_break,
	.wait_until_sent = mxser_wait_until_sent,
	.tiocmget = mxser_tiocmget,
	.tiocmset = mxser_tiocmset,
	.get_icount = mxser_get_icount,
};

struct tty_port_operations mxser_port_ops = {
	.carrier_raised = mxser_carrier_raised,
	.dtr_rts = mxser_dtr_rts,
	.activate = mxser_activate,
	.shutdown = mxser_shutdown_port,
};

/*
 * The MOXA ART SoC UART serial driver boot-time initialization code!
 */
static void mxser_release_embedded_res(struct mxser_board *brd)
{
	free_irq(brd->irq, brd);
#if 1
	release_mem_region(MOXAART_UART3_BASE, 32 * 4);
	release_mem_region(MOXAART_UART_INT_VEC_BASE, 16);
	release_mem_region(MOXAART_UART_MODE_BASE, 4);
#endif
}

extern void	moxaart_int_set_irq(unsigned int irq,int mode,int level);
static int __devinit mxser_initbrd(struct mxser_board *brd)
{
	struct mxser_port *info;
	unsigned int i;
	int retval;

	printk(KERN_INFO "mxser: max. baud rate = %d bps\n",
			brd->ports[0].max_baud);

	for (i = 0; i < brd->info->nports; i++) {
		info = &brd->ports[i];
		tty_port_init(&info->port);
		info->port.ops = &mxser_port_ops;
		info->board = brd;
		info->stop_rx = 0;
		info->ldisc_stop_rx = 0;

		/* Enhance mode enabled here */
		mxser_enable_must_enchance_mode(info->ioaddr);

		info->port.flags = ASYNC_SHARE_IRQ;
		info->type = brd->uart_type;

		process_txrx_fifo(info);

		info->custom_divisor = info->baud_base * 16;
		info->port.close_delay = 5 * HZ / 10;
		info->port.closing_wait = 30 * HZ;
		info->speed = 9600; /* default speed */
		info->normal_termios = mxvar_sdriver->init_termios;
		memset(&info->mon_data, 0, sizeof(struct mxser_mon));
		info->err_shadow = 0;
		spin_lock_init(&info->slock);

		/* before set INT ISR, disable all int */
		info->IER = 0;
		outb(0, info->ioaddr + UART_IER);
	}

	retval = request_irq(brd->irq, mxser_interrupt, IRQF_SHARED | IRQ_TYPE_LEVEL_HIGH, "MoxaARTSoCUART", brd);
	if (retval)
		printk(KERN_ERR "Board %s: Request irq failed, IRQ (%d) may "
			"conflict with another device.\n",
			brd->info->name, brd->irq);

	return retval;
}

static void __init mxser_get_embedded_conf(struct mxser_board *brd)
{
	int	i;

	brd->chip_flag = MOXA_MUST_MU860_HWID;
	brd->uart_type = PORT_16550A;
	brd->info = &mxser_cards[0];
	brd->irq = IRQ_UART;
	for ( i=0; i<brd->info->nports; i++ ) {
		brd->ports[i].ioaddr = MOXAART_UART3_VA_BASE + i * 32;
		brd->ports[i].baud_base = 921600;
		brd->ports[i].max_baud = 921600;
		brd->ports[i].opmode_ioaddr = MOXAART_UART_MODE_VA_BASE;
	}
	brd->vector = MOXAART_UART_INT_VEC_VA_BASE;
	brd->vector_mask = 0x3c;
	
	// default set the interface to RS232 mode
	i = inl(MOXAART_UART_MODE_VA_BASE);
	i &= ~((3<<4)|(3<<6)|(3<<8)|(3<<10));
	outl(i, MOXAART_UART_MODE_VA_BASE);

#if 1	
	/* FIX ME */
	request_mem_region(MOXAART_UART3_BASE, 32 * 4, "SoC UART");
	request_mem_region(MOXAART_UART_INT_VEC_BASE, 16, "SoC UART vector");
	request_mem_region(MOXAART_UART_MODE_BASE, 4, "SoC UART mode");
#endif
}

static int __init mxser_module_init(void)
{
	struct mxser_board	*brd;
	int			i, retval;

	mxvar_sdriver = alloc_tty_driver(MXSER_PORTS + 1);
	if (!mxvar_sdriver)
		return -ENOMEM;

	printk(KERN_INFO "MOXA ART CPU SoC UART driver version %s\n",
		MXSER_VERSION);

	/* Initialize the tty_driver structure */
	mxvar_sdriver->owner = THIS_MODULE;
	mxvar_sdriver->magic = TTY_DRIVER_MAGIC;
	mxvar_sdriver->name = "ttyM";
	mxvar_sdriver->major = ttymajor;
	mxvar_sdriver->minor_start = 0;
	mxvar_sdriver->num = MXSER_PORTS + 1;
	mxvar_sdriver->type = TTY_DRIVER_TYPE_SERIAL;
	mxvar_sdriver->subtype = SERIAL_TYPE_NORMAL;
	mxvar_sdriver->init_termios = tty_std_termios;
	mxvar_sdriver->init_termios.c_cflag = B9600|CS8|CREAD|CLOCAL;
	mxvar_sdriver->flags = TTY_DRIVER_REAL_RAW|TTY_DRIVER_DYNAMIC_DEV;
	tty_set_operations(mxvar_sdriver, &mxser_ops);

	retval = tty_register_driver(mxvar_sdriver);
	if (retval) {
		printk(KERN_ERR "Couldn't install MOXA ART CPU SoC UART "
				"tty driver !\n");
		goto err_put;
	}

	brd = &mxser_boards[0];
	mxser_get_embedded_conf(brd);
	retval = mxser_initbrd(brd);
	if ( retval )
		goto err_unr;
	brd->idx = 0;
	for (i = 0; i < brd->info->nports; i++)
		tty_register_device(mxvar_sdriver, brd->idx + i, NULL);

	return 0;

err_unr:
	tty_unregister_driver(mxvar_sdriver);
err_put:
	put_tty_driver(mxvar_sdriver);
	return retval;
}

static void __exit mxser_module_exit(void)
{
	unsigned int i, j;

	for (i = 0; i < MXSER_BOARDS; i++)
		if (mxser_boards[i].info != NULL)
			for (j = 0; j < mxser_boards[i].info->nports; j++)
				tty_unregister_device(mxvar_sdriver,
						mxser_boards[i].idx + j);
	tty_unregister_driver(mxvar_sdriver);
	put_tty_driver(mxvar_sdriver);

	mxser_release_embedded_res(&mxser_boards[0]);
}

module_init(mxser_module_init);
module_exit(mxser_module_exit);
