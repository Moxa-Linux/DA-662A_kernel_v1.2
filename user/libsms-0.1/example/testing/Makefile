CC = arm-linux-gcc
CFLAGS = -g -O2 -I../../include -L../../src
LDFLAGS = -lsms
CPP = xscale-linux-gcc -E
CPPFLAGS = 
CXX = arm-linux-g++
CXXCPP = arm-linux-g++ -E
RANLIB = arm-linux-ranlib
VERSION = 0.1
MODULES=main.o
EXE=smstest

all: ${MODULES}
	${CC} ${CFLAGS} -o ${EXE} ${MODULES} ${LDFLAGS}

main.o: main.c
	${CC} ${CFLAGS} -c $^

clean:
	rm -rf ${MODULES}
	rm -rf ${EXE}
