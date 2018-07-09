#CC=arm-linux-gcc
#CFLAG=-I$(ROOTDIR)/include
EXEC=wtest

all:	wtest

wtest:
	$(CC) -o $(EXEC) $(CFLAG) wtest.c
romfs:
	$(STRIP) -s $(EXEC)
	$(ROMFSINST) /bin/$(EXEC)
clean:
	rm -rf *.o $(EXEC)	
	
