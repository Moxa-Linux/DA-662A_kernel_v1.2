include compile.mk
INSTALLDIR=/usr/lib
# OS
OS_DIR=os/linux

# Object files that sub directory should offer
OS_OBJS=$(OS_DIR)/log/log.o \
	$(OS_DIR)/ini/ini.o \
	$(OS_DIR)/db/db.o \
	$(OS_DIR)/mdp/mdp.o \
	$(OS_DIR)/net/net.o

# Add your header here
INC_DIR = ../include
LDFLAGS += -lsqlite3
CFLAGS += -I$(INC_DIR)
LIBNAME=libmoxasys
REAL_NAME=${LIBNAME}.so.1.0.0
LINKER_NAME=${LIBNAME}.so
SONAME=${LINKER_NAME}.1
STATIC_LIB=${LIBNAME}.a
OBJS = log.o ini.o mdp.o db.o net.o

all: os_objs $(EXEC)

$(EXEC): $(OBJS) $(OS_OBJS)

os_objs: ${OBJS}
	make -C $(OS_DIR)
	${CC} -shared -Wl,-soname,${SONAME} -o ${REAL_NAME} ${OS_OBJS} ${OBJS}
	ar rcs ${LIBNAME}.a ${OS_OBJS} ${OBJS}

clean:
	rm -f $(EXEC) $(OBJS)
	rm -rf ${REAL_NAME}
	rm -rf ${STATIC_LIB}
	make -C $(OS_DIR) clean

install:
	-[ ! -d $(INSTALLDIR) ] && mkdir -p $(INSTALLDIR)
	cp -p $(REAL_NAME) $(INSTALLDIR)/$(REAL_NAME)

