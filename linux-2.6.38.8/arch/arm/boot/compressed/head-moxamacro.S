#include <asm/mach-types.h>
	.section	".start", "ax"
	mov r7, #MACH_TYPE_MOXAMACRO & 0xff
	orr r7, r7, #MACH_TYPE_MOXAMACRO & 0xff00