TARGET = swap32
CFLAGS = -m32

all:	$(TARGET) $(TARGET)-dbg

$(TARGET):$(TARGET).c
	gcc $(CFLAGS) -o $(TARGET) $(TARGET).c

$(TARGET)-dbg:$(TARGET).c
	gcc $(CFLAGS) -DDEBUG -o $(TARGET)-dbg $(TARGET).c

clean:
	rm -f $(TARGET) *.o $(TARGET)-dbg

