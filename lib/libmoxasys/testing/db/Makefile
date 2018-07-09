include ../../src/compile.mk
SRC_DIR=../../src
DB_DIR=$(SRC_DIR)/os/linux/db
INI_DIR=$(SRC_DIR)/os/linux/ini
CFLAGS+=-I../../include -I$(DB_DIR)
LDFLAGS+=-lsqlite3

EXEC=db_operation

all: db_obj $(EXEC)

clean: db_clean

db_obj: 
	make -C $(DB_DIR)

$(EXEC): $(DB_DIR)/db.o $(INI_DIR)/ini.o  $(SRC_DIR)/db.o $(SRC_DIR)/ini.o $(EXEC).o

db_clean:
	rm -rf *.o
	rm -rf db_operation
