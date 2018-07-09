#ifndef INCLUDE_DEBUG_MOXA
#define INCLUDE_DEBUG_MOXA

#define ENABLE_DEBUG_MOXA 0

#if ENABLE_DEBUG_MOXA && defined DEBUG_MOXA
#define debugm(fmt, args...) printk("%s: "fmt, __FUNCTION__, ##args)
#define debug_in(fmt, args...) printk("%s: enter\n", __FUNCTION__, ##args)
#define debug_out(fmt, args...) printk("%s: leave\n", __FUNCTION__, ##args)
#define debugh(fmt, args...) printk("  >%s: "fmt, __FUNCTION__, ##args)
#else
#define debugm(fmt, args...) 
#define debug_in(fmt, args...) 
#define debug_out(fmt, args...) 
#define debugh(fmt, args...) 
#endif

#endif


