-include $(ARCH_CONFIG)
CC=arm-mv5sft-linux-gnueabi-gcc
CFLAGS+=-I. 
#CFLAGS+=-I.  -DDEBUG
OBJS=main.o
EXEC=uc_ctrl

all: clean $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $@ $(OBJS)

romfs: clean $(EXEC)
	$(STRIP) -s $(EXEC)
	$(ROMFSINST) /bin/$(EXEC)
	chmod 755 $(ROMFSDIR)/bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.elf *.o *.gdb
