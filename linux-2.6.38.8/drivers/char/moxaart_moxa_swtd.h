#ifndef __MOXAART_MOXA_SWDT__
#define __MOXAART_MOXA_SWDT__

#define DEFAULT_WATCHDOG_TIME	(30UL*1000UL)	// 30 seconds
#define WATCHDOG_MIN_TIME	50UL		// 50 msec

#if defined ARCH_W325_TATUNG
#define WATCHDOG_MAX_TIME	(300UL*1000UL)	// 300 seconds
#else
#define WATCHDOG_MAX_TIME	(60UL*1000UL)	// 60 seconds
#endif


#define WATCHDOG_TOL_TIME	(50UL)	// 50 msec, for watchdog timer polling
#define WATCHDOG_DEFER_TIME	(15000UL)	// 15 sec, for hw watchdog timer rebooting

// APB_CLOCK is for bus and watchdog, APB=(arm peripheral bus), APB_CLK= the HZ of bus for watchdog
#define WATCHDOG_COUNTER(x)	((APB_CLK/1000UL)*(x))

// Note: HZ is for kernel timer
// for moxaart HZ=100, 
// for other series, it may be 1000
#define WATCHDOG_ACK_JIFFIES(x)	(((x-WATCHDOG_TOL_TIME)*HZ/1000UL))

#define MOXA_WATCHDOG_MINOR	106
#define IOCTL_WATCHDOG_ENABLE		1	// enable watch dog and set time (unint msec)
#define IOCTL_WATCHDOG_DISABLE		2	// disable watch dog, kernle do it
#define IOCTL_WATCHDOG_GET_SETTING	3	// get now setting about mode and time
#define IOCTL_WATCHDOG_ACK		4	// to ack watch dog

struct swtd_set_struct {
	int		mode;
	unsigned long	time;
};

#endif	//___MOXAART_MOXA_SWDT__
