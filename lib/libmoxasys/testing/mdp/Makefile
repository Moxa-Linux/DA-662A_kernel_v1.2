CFLAGS=-I../../include
LDFLAGS=-L../../src
LIBS=-ldcu -lsqlite3

all: mdp.o
	${CC} ${CFLAGS} ${LDFLAGS} -o mdp_test mdp.o ${LIBS}

mdp.o: mdp.c
	${CC} ${CFLAGS} ${LDFLAGS} -c $^
clean:
	@rm -rf mdp_test mdp.o
