
# include the dependent information
SRC_DIR = ../../src
INC_DIR = -I../../include
INI_DIR = $(SRC_DIR)/os/linux/ini
OS_OBJS = $(INI_DIR)/ini.o

include $(SRC_DIR)/compile.mk

CFLAGS += $(INC_DIR)

EXEC = ini_test
OBJS = $(SRC_DIR)/ini.o


all: os_objs $(EXEC)

$(EXEC): $(OS_OBJS) $(OBJS)

os_objs:
	make -C $(INI_DIR)

clean: 
	@$(RM) $(EXEC) $(OBJS)
	make -C $(INI_DIR) clean

