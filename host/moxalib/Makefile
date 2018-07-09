# Defined for UC-7420/7410/7408/7402-LX/DA-660-8/16-LX
#PLATFORM = UC7400_AND_DA660

# Defined for UC-7420/7410/7408/7402-LX Plus and /DA-661/662/663-16-LX
#PLATFORM = UC7400PLUS_AND_DA66X

# Defined for UC-8410/8416/8418-LX
#PLATFORM = UC8400

# Defined for RNAS-1200-LX
#PLATFORM = RNAS1200

CONFIGURE_TARGET := $(strip $(patsubst %-,%,$(CROSS)))


ifeq ($(PLATFORM),UC7400_AND_DA660)
  PREFIX=mxscaleb-
  LIB_INSTALL_DIR=/usr/local/mxscaleb/armv5b-linux/lib/
  HEADER_INSTALL_DIR=/usr/local/mxscaleb/armv5b-linux/include
  SOURCE=dio.c swtd.c keypad.c lcm.c
  OBJ=dio.o swtd.o keypad.o lcm.o
else
  ifeq ($(PLATFORM),UC7400PLUS_AND_DA66X)
    PREFIX=xscale_be-
    LIB_INSTALL_DIR=/usr/local/xscale_be/target/usr/lib/
    HEADER_INSTALL_DIR=/usr/local/xscale_be/target/usr/include
    SOURCE=dio.c swtd.c keypad.c lcm.c
    OBJ=dio.o swtd.o keypad.o lcm.o
  else
    ifeq ($(PLATFORM),UC8400)
      PREFIX=$(CROSS)
      LIB_INSTALL_DIR=$(INSTALL_ROOT)/$(CONFIGURE_TARGET)/lib
      HEADER_INSTALL_DIR=$(INSTALL_ROOT)/$(CONFIGURE_TARGET)/include
      SOURCE=dio.c swtd.c
      OBJ=dio.o swtd.o
    else
      ifeq ($(PLATFORM),IA261)
        PREFIX=$(CROSS)
        LIB_INSTALL_DIR=$(INSTALL_ROOT)/$(CONFIGURE_TARGET)/lib
        HEADER_INSTALL_DIR=$(INSTALL_ROOT)/$(CONFIGURE_TARGET)/include
        SOURCE=dio.c swtd.c
        OBJ=dio.o swtd.o
      else
        ifeq ($(PLATFORM),RNAS1200)
          PREFIX=$(CROSS)
          LIB_INSTALL_DIR=$(INSTALL_ROOT)/$(CONFIGURE_TARGET)/lib
          HEADER_INSTALL_DIR=$(INSTALL_ROOT)/$(CONFIGURE_TARGET)/include
          SOURCE=dio.c
          OBJ=dio.o
        else
          PREFIX?=
          LIB_INSTALL_DIR?=
          HEADER_INSTALL_DIR?=
          SOURCE=dio.c swtd.c gpio.c keypad.c lcm.c
#          OBJ=dio.o swtd.o gpio.o keypad.o lcm.o
          OBJ=dio.o swtd.o
        endif
      endif
    endif
  endif
endif  

CC=$(PREFIX)gcc
STRIP=$(PREFIX)strip
AR=$(PREFIX)ar

LNAME=moxalib

all: release

release: $(OBJ)
	$(AR) rcs lib$(LNAME).a $(OBJ)

%.o:%.c
	$(CC) -c $<

install:	lib$(LNAME).a
	cp -a lib$(LNAME).a $(LIB_INSTALL_DIR)
	cp -a moxadevice.h $(HEADER_INSTALL_DIR)

clean:
	/bin/rm -f *.o *.a
