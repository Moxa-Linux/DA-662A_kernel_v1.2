
/* 
 * This file is to define Moxa UC7000 series embedded device private ioctrl command. 
 * It is include LCM, KEYPAD, UART operating mode (RS232, RS422, RS485-2wire, RS485-4wires).
 * LCM device node is /dev/lcm.
 * KEYPAD device node is /dev/keypad.
 * UART device node is ttyM0 - ttyM7.
 * Copyright Moxa Technologies L.T.D.
 * History:
 * Date		Author			Comment
 * 01-13-2004	Victor Yu.		Create it.
 * 06-15-2004	Victor Yu.		Add to support UC7220 PIO library define.
 * 08-12-2004	Victor Yu.		Add to support to set special baud rate. By ioctl
 * 					MOXA_SET_SPECIAL_BAUD_RATE &
 * 					MOXA_GET_SPECIAL_BAUD_RATE.
 * 					And the termios will be B4000000. The actual baud rate
 * 					you must use above ioctl to get.
 * 09-16-2004	Victor Yu.		Add to support sWatchDog library define.
 * 01-13-2005	Victor Yu.		Add to support DIN & DOUT.
 * 14-06-2005	Jared Wu.		Add to support duration.
 */

#ifndef _MOXADEVICE_H
#define _MOXADEVICE_H

// following about LCM
// ioctl command define
// ioctl(fd, IOCTL_LCM_GOTO_XY, &pos)
// ioctl(fd, IOCTL_LCM_CLS, NULL)
// ioctl(fd, IOCTL_LCM_CLEAN_LINE, NULL)
// ioctl(fd, IOCTL_LCM_GET_XY, &pos)
// ioctl(fd, IOCTL_LCM_BACK_LIGHT_ON, NULL)
// ioctl(fd, IOCTL_LCM_BACK_LIGHT_OFF, NULL)
// ioctl(fd, IOCTL_LCM_AUTO_SCROLL_ON, NULL);
// ioctl(fd, IOCTL_LCM_AUTO_SCROLL_OFF, NULL);
#define IOCTL_LCM_BASE			0x100
#define IOCTL_LCM_GOTO_XY               (1+IOCTL_LCM_BASE)	// jump to position x, y
#define IOCTL_LCM_CLS                   (2+IOCTL_LCM_BASE)	// clear all screen on LCM
#define IOCTL_LCM_CLEAN_LINE            (3+IOCTL_LCM_BASE)	// clear this line on position y
#define IOCTL_LCM_GET_XY                (4+IOCTL_LCM_BASE)	// get now position x, y
#define IOCTL_LCM_BACK_LIGHT_ON         (5+IOCTL_LCM_BASE)	// let back light on
#define IOCTL_LCM_BACK_LIGHT_OFF        (6+IOCTL_LCM_BASE)	// let back light off
#define IOCTL_LCM_AUTO_SCROLL_ON	(13+IOCTL_LCM_BASE)	// when the LCM full will auto scroll up one line 
#define IOCTL_LCM_AUTO_SCROLL_OFF       (14+IOCTL_LCM_BASE)	// don't auto scroll 
#define IOCTL_LCM_CURSOR_ON         (9+IOCTL_LCM_BASE)
#define IOCTL_LCM_CURSOR_OFF        (10+IOCTL_LCM_BASE)
#define IOCTL_LCM_BLINK_ON          (15+IOCTL_LCM_BASE)
#define IOCTL_LCM_BLINK_OFF         (16+IOCTL_LCM_BASE)
typedef struct lcm_xy {	// LCM position struct define
	int     x;      // 0 - 15
	int     y;      // 0 - 7
} lcm_xy_t;

// following about KEYPAD
// ioctl command define
// ioctl(fd, IOCTL_KEYPAD_HAS_PRESS, &no)
// ioctl(fd, IOCTL_KEYPAD_GET_KEY, &value)
#define IOCTL_KEYPAD_BASE	0x100
#define IOCTL_KEYPAD_HAS_PRESS  (1+IOCTL_KEYPAD_BASE)	// to check any key has pressed, 0 - means no, > 0 has pressed key number
#define IOCTL_KEYPAD_GET_KEY    (2+IOCTL_KEYPAD_BASE)	// to get has pressed key value

// keypad value define
#define KEY0                    0
#define KEY1                    1
#define KEY2                    2
#define KEY3                    3
#define KEY4                    4

// following about UART operatin mode
// ioctl command define
// ioctl(fd, MOXA_SET_OP_MODE, &mode)
// ioctl(fd, MOXA_GET_OP_MODE, &mode)
#define MOXA_SET_OP_MODE      (0x400+66)	// to set operating mode
#define MOXA_GET_OP_MODE      (0x400+67)	// to get now operating mode
// following add by Victor Yu. 08-12-2004
// ioctl(fd, MOXA_SET_SPECIAL_BAUD_RATE, &speed)
// ioctl(fd, MOXA_GET_SPECIAL_BAUD_RATE, &speed)
#define MOXA_SET_SPECIAL_BAUD_RATE      (0x400+68)	// to set special baud rate
#define MOXA_GET_SPECIAL_BAUD_RATE      (0x400+69)	// to get now special baud rate

// operating mode value define
#define RS232_MODE              0
#define RS485_2WIRE_MODE        1
#define RS422_MODE              2
#define RS485_4WIRE_MODE        3

// following add by Victor Yu. 06-15-2004 to support UC-7220 PIO function
// following just support for UC-7220 model.
#define MAX_PIO			4
#define PIO_NO_ERROR		-1 // the PIO number error
#define PIO_MODE_ERROR		-2 // the PIO mode error
#define PIO_DATA_ERROR		-3 // the PIO data error
#define PIO_NODE_ERROR		-4 // open PIO device node error
#define PIO_ERROR		-5 // some error, get error number from errno
#define PIO_INPUT		1 // the PIO mode is input
#define PIO_OUTPUT		0 // the PIO mode is output
#define PIO_HIGH		1 // the PIO data is high
#define PIO_LOW			0 // the PIO data is low
#define PIO_OK			0 // function is OK
#ifdef __cplusplus
extern	"C" {
#endif
int get_pio_mode(unsigned int pio); // 1 for input, 0 for output, PIO from 0
int get_pio_data(unsigned int pio); // 1 for high, 0 for low, PIO from 0
int set_pio_mode(unsigned int pio, int mode); // 1 for intput, 0 for output, PIO from 0
int set_pio_data(unsigned int pio, int data); // 1 for high, 0 for low, PIO from 0

// following add by Victor Yu. 09-16-2004 to suppport sWatchDog function
int swtd_open(void);	// to get a file handle to control sWatchDog,<0 will be error
int swtd_enable(int fd, unsigned long time);	// to enable sWatchDog by application 
							// to do ACK, fd is returned mdog_open
int swtd_disable(int fd);	// to disable sWatchDog
int swtd_get(int fd, int *mode, unsigned long *time);	// to get now setting
								// mode - 1 for enable
								//	  0 for disable
								// time - now ACK time
int swtd_ack(int fd);	// to ACk sWatchDog
int swtd_close(int fd);	// to close sWatchDog
#ifdef __cplusplus
}
#endif
//
// following add by Victor Yu. 01-13-2005 to support DIN & DOUT model API
//
#define DIO_ERROR_PORT		-1
#define DIO_ERROR_MODE		-2
#define DIO_ERROR_CONTROL	-3
#define DIO_ERROR_DURATION	-4		// Add by Jared , 06-14-2005, The value of duration is not 0 or  not in the range, 40000 <= duration <= 3600 seconds (1 hour) .
#define DIO_ERROR_DURATION_20MS	-5	// Add by Jared , 07-6-2005, The value of duration must be a multiple of 20 ms .
#define DIO_OK			0

#define DIN_EVENT_STATE_CHANGE	2 // high to low or low to high
#define DIN_EVENT_HIGH_TO_LOW	1 // high to low
#define DIN_EVENT_LOW_TO_HIGH	0 // high to low
#define DIN_EVENT_CLEAR		-1 // clear callback event

#define MAX_DIN_PORT		8
#define MAX_DOUT_PORT		8
#define DIO_HIGH		1 // the DIO data is high 
#define DIO_LOW			0 // the DIO data is low

#ifdef __cplusplus
extern	"C" {
#endif
// to set DOUT to high or low
// int doport - want to set DOUT port number, must 0 - 7
// int state - 1 for high, 0 for low
int set_dout_state(int doport, int state);
// to get DOUT state at now
// int doport - want to get DOUT port number, must 0 - 7
// int *state - 1 for high, 0 for low
int get_dout_state(int doport, int *state);
// to get DIN state at now
// int diport - want to get DIN port number, must 0 - 7
// int *state - 1 for high, 0 for low
int get_din_state(int diport, int *state);
// to set DIN event, you can set callback function for high to low or low to high
// int diport - want to set event DIN port number, must 0 - 7
// void (*func)(int diport) - callback function, will send the DIN port to it, NULL for clear this event
// int mode - want to set event mode, 1 for high to low, 0 for low to high, -1 for clear this event
// unsigned long duration - when the duration of the DIN port keep over a period in this state, invoke the callback function.
//											The time unit of duration is milliseconds.	Give 0 implies no duration check. 
//											The value of duration must be a multiple of 20 milliseconds. The range of duration is 0 or 40 <= duration <= 3600000 milliseconds.
//											An inaccuracy of the duration is 24 ms. For example, if the din duration is 200 ms, this event will be generate when the din pin keep in one state between 176 ms ~ 200 ms.
// Add by Jared , 06-14-2005, the duration parameter
int	set_din_event(int diport, void (*func)(int diport), int mode, unsigned long duration);
// to get DIN event mode at now
// int diport - want to get DIN port number, must 0 - 7
// int *mode - the event mode at now, 1 for high to low, 0 for low to high, -1 for clear
// unsigned long *duration -  when the duration of the DIN port keep over a period in this state, invoke the callback function.
//									 The value of duration should be 0 or 40 <= duration <= 3600000 milliseconds.
// Add by Jared , 06-14-2005, the duration parameter
int	get_din_event(int diport, int *mode, unsigned long *duration);
#ifdef __cplusplus
}
#endif

#endif	// _MOXADEVICE_H
