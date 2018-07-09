EXEC = gprsled
OBJS = gprsled.o

CC=arm-linux-gcc
#CFLAGS += -DDEBUG

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

romfs:
	$(STRIP) -s $(EXEC)
	$(ROMFSINST) /bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.elf *.o *.gdb

