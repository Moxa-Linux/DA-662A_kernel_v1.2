#ifndef _CPE_SERIAL_H
#define _CPE_SERIAL_H

#include <linux/config.h>
#include <mach/cpe/cpe.h>
#include <mach/hardware.h>

#define BASE_BAUD (CONFIG_UART_CLK/16)

/* Standard COM flags */
#define STD_SERIAL_PORT_DEFNS
#define EXTRA_SERIAL_PORT_DEFNS

#endif	// _CPE_SERIAL_H
