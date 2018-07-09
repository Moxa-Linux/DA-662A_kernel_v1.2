#CC = arm-linux-gcc
CFLAGS = -g -O2 -I../include -I../ -L../src
LDFLAGS = -lsms
#CPP = xscale-linux-gcc -E
#CPPFLAGS = 
#CXX = arm-linux-g++
#CXXCPP = arm-linux-g++ -E
#RANLIB = arm-linux-ranlib
VERSION = 0.1
MODULES=main.o
EXEC=egprsagent

all: ${MODULES}
	${CC} ${CFLAGS} -o ${EXEC} ${MODULES} ${LDFLAGS} 
	cp $(EXEC) $(INSTALL_ROOT)/bin/$(EXEC)

main.o: main.c
	${CC} ${CFLAGS} -c $^

clean:
	rm -rf *.o
	rm -rf ${EXEC}

romfs:
	$(ROMFSINST) $(INSTALL_ROOT)/bin/$(EXEC) /bin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)

