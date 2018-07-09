ifdef CONFIG_MOXAMACRO_REMAP
zreladdr-y	:= 0x00008000
params_phys-y	:= 0x00000100
else

ifdef CONFIG_MOXAMACRO_DDR2_SDRAM
zreladdr-y      := 0x80008000
params_phys-y   := 0x80000100
else
zreladdr-y      := 0x60008000
params_phys-y   := 0x60000100
endif

endif
