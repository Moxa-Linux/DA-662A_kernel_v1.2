include ../src/compile.mk
CFLAGS+=-I../include
LDFLAGS+=-L../src
LIBS=-lmoxasys -lsqlite3

all: log_example.o ini_example.o net_client_example.o net_server_example.o db_example.o mdp_example.o
	${CC} ${CFLAGS} ${LDFLAGS} -o log_example log_example.o ${LIBS}
	${CC} ${CFLAGS} ${LDFLAGS} -o ini_example ini_example.o ${LIBS}
	${CC} ${CFLAGS} ${LDFLAGS} -o db_example db_example.o ${LIBS}
	${CC} ${CFLAGS} ${LDFLAGS} -o mdp_example mdp_example.o ${LIBS}
	${CC} ${CFLAGS} ${LDFLAGS} -o net_client_example net_client_example.o ${LIBS}
	${CC} ${CFLAGS} ${LDFLAGS} -o net_server_example net_server_example.o ${LIBS}

log_example.o:
	${CC} ${CFLAGS} ${LDFLAGS} -c log_example.c ${LIBS}

db_example.o:
	${CC} ${CFLAGS} ${LDFLAGS} -c db_example.c ${LIBS}

ini_example.o:
	${CC} ${CFLAGS} ${LDFLAGS} -c ini_example.c ${LIBS}

mdp_example.o:
	${CC} ${CFLAGS} ${LDFLAGS} -c mdp_example.c ${LIBS}

net_client_example.o:
	${CC} ${CFLAGS} ${LDFLAGS} -c net_client_example.c ${LIBS}

net_server_example.o:
	${CC} ${CFLAGS} ${LDFLAGS} -c net_server_example.c ${LIBS}

clean:
	@rm -rf log_example ini_example net_client_example net_server_example db_example mdp_example
	@rm -rf log_example.o ini_example.o net_client_example.o net_server_example.o db_example.o mdp_example.o
