
include ../../../compile.mk

SRCDIR = ../../../
INCDIR = -I./ -I../../../../include

SRCS = \
	   strext.c \
	   ini_parser.c \
	   ini_find.c

OBJS = $(SRCS:.c=.o)
TARGET = ini.o


DEFS = $(INCDIR)
ifdef DEBUG
	DEFS += -DDEBUG
endif
CFLAGS += -Wall -O2 $(DEFS)


all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) -r $(OBJS) -o $(TARGET)

clean: 
	@$(RM) $(TARGET) $(OBJS)

