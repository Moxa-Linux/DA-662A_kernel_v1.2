#ifndef __IXP43X_MOXA_SWDT__
#define __IXP43X_MOXA_SWDT__


#if defined (CONFIG_CPU_IXP43X)
	#define MOXA_WATCHDOG_MINOR	106
#elif defined (ARCH_IXDP422) || defined (CONFIG_ARCH_DA66X_UC7400)
	#define MOXA_WATCHDOG_MINOR	105
#endif

#define DEFAULT_WATCHDOG_TIME	(30UL*1000UL)	// 30 seconds
#define WATCHDOG_MIN_TIME	50UL		// 50 msec
#define WATCHDOG_MAX_TIME	(60UL*1000UL)	// 60 seconds
#define WATCHDOG_TOL_TIME	(50UL)	// 50 msec, for watchdog timer polling
#define WATCHDOG_DEFER_TIME	(15000UL)	// 5 sec, for hw watchdog timer rebooting
/* Jared, 11-29-2010, if x> 64000, the counter will be overflow, set it to the upper bounding 0xFFFFFFFF */
#define WATCHDOG_COUNTER(x)	(x<64000) ? (((x)*66666UL)+(((x)*666UL)/1000UL)) : (0xFFFFFFFF)
#define WATCHDOG_ACK_JIFFIES(x)	(((x)*HZ/1000UL)-WATCHDOG_TOL_TIME)
#define IXP4XX_WDOG_CNT_ENA	BIT(2)
#define IXP4XX_WDOG_INT_ENA	BIT(1)
#define IXP4XX_WDOG_RST_ENA	BIT(0)

#define IOCTL_WATCHDOG_ENABLE		1	// enable watch dog and set time (unint msec)
#define IOCTL_WATCHDOG_DISABLE		2	// disable watch dog, kernle do it
#define IOCTL_WATCHDOG_GET_SETTING	3	// get now setting about mode and time
#define IOCTL_WATCHDOG_ACK		4	// to ack watch dog

struct swtd_set_struct {
	int		mode;
	unsigned long	time;
};

#endif	//__IXP43X_MOXA_SWDT__
