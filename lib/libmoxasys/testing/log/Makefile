include ../../src/compile.mk

SOURCE_DIR = ../../src
INCLUDE_DIR = ../../include
CFLAGS += -I$(INCLUDE_DIR)
LDFLAGS+=-lsqlite3

LIBS = $(SOURCE_DIR)/log.o $(SOURCE_DIR)/os/linux/log/log.o
LIBS += $(SOURCE_DIR)/db.o $(SOURCE_DIR)/os/linux/db/db.o
LIBS += $(SOURCE_DIR)/ini.o $(SOURCE_DIR)/os/linux/ini/ini.o
EXEC = log_test


all: log_obj $(EXEC)
	-rm logTest*

log_obj:
	make -C $(SOURCE_DIR)

$(EXEC): $(LIBS) $(EXEC).o

clean:
	@-rm -r $(EXEC) $(EXEC).o logTest*
	make -C $(SOURCE_DIR) clean
