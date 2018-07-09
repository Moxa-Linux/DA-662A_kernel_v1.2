#define	WDT_IN_USE		0
#define	WDT_OK_TO_CLOSE		1

/*  
 *  APB_CLOCK is for bus and watchdog, APB=(arm peripheral bus), 
 *  APB_CLK= the HZ of bus for watchdog 
 */
#define WDT_GET_TICK_FROM_SEC(sec)	(APB_CLK * sec)

/* 
 * Register
 */
typedef struct _wdt_regs {
	unsigned long WdCounter; 
	unsigned long WdLoad;		/* Contain the value would be reload to WdCounter */
	unsigned long WdRestart;	/* Load WdLoad to WdCounter */
#define RestartReload 0x5ab9 
	unsigned long WdCR;
#define CREnable	0x01
#define CRRst		0x02		/* System Reset  */
#define CRIntr	0x04
	unsigned long WdStatus;		/* Set to 0 when the counter reaches zero */
	unsigned long WdClear;		/* Clear WdStatus */
	unsigned long WdIntrLen;
} wdt_regs_t; 

/* 
 * Flash
 */
#define SR_WRITE_STATE_MACHINE     (1<<7)      /* 1:Ready 0: Busy */ 
#define SR_ERASE_SUSPEND           (1<<6)
#define SR_ERASE_CLEAR_LCOK_BITSSTATUS (1<<5)
#define SR_PROBRAM_SET_LOCK_BIT    (1<<4)
#define SR_PROGRAM_VOLTAGE         (1<<3)
#define SR_PROGRAM_SUSPEND         (1<<2)
#define SR_DEVICE_PROTECT          (1<<1)

#define CMD_PROG_SUSPEND    0xb0    /* allow programming interrupt to read data in other flash memory */
#define CMD_PROG_RESUME     0xd0
#define CMD_READ_STATUS_REG 0x70
#define CMD_READ_ARRAY      0xff

/* 
 * Debug 
 */
//#define DEBUG
#ifdef DEBUG
#define drv_dbg(fmt, args...) printk(KERN_INFO "%s: "fmt, __FUNCTION__, ##args)
#else 
#define drv_dbg(fmt, args...)
#endif
#define drv_err(fmt, args...) printk(KERN_ERR "moxaart_wdt: "fmt, ##args)
#define drv_info(fmt, args...) printk(KERN_INFO "moxaart_wdt: "fmt, ##args)

#ifdef DEBUG
static void dump_regs(wdt_regs_t *wr) {
	drv_dbg("%s\n", __FUNCTION__);
	drv_dbg("counter=%x\n", (unsigned int)wr->WdCounter);
	drv_dbg("load=%x\n", (unsigned int)wr->WdLoad);
	drv_dbg("restart=%x\n", (unsigned int)wr->WdRestart);
	drv_dbg("cr=%x\n", (unsigned int)wr->WdCR);
	drv_dbg("status=%x\n", (unsigned int)wr->WdStatus);
	drv_dbg("clear=%x\n", (unsigned int)wr->WdClear);
	drv_dbg("intrlen=%x\n", (unsigned int)wr->WdIntrLen);
}
#endif

enum { MODE_INTERRUPT, MODE_RESET }; 
