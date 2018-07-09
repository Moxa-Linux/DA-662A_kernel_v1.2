#ifndef _MOXAART_SERIAL_H
#define _MOXAART_SERIAL_H

#include <linux/config.h>
#include <mach/moxaart/moxaart.h>
#include <mach/hardware.h>

#define BASE_BAUD (CONFIG_UART_CLK/16)

/* Standard COM flags */
#define STD_SERIAL_PORT_DEFNS
#define EXTRA_SERIAL_PORT_DEFNS

#endif	// _MOXAART_SERIAL_H
