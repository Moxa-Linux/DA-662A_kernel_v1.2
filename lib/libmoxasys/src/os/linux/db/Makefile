include ../../../compile.mk
INC_DIR=../../../../include
CFLAGS+=-I$(INC_DIR) -I.

all: db.o

db.o: db.c 
	$(CC) $(CFLAGS) -c db.c -o db.o
clean:
	rm -f db.o

