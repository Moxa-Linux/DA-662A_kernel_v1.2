CFLAGS=-I../../include
LDFLAGS=-L../../src
LIBS=-ldcu -lsqlite3

all: net.o
	${CC} ${CFLAGS} ${LDFLAGS} -o net_test net.o ${LIBS}

net.o: net.c
	${CC} ${CFLAGS} ${LDFLAGS} -c $^
clean:
	@rm -rf net_test net.o
