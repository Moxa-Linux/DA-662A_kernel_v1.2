
# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   /usr/local/arm-linux-4.4.2/bin/xscale-linux-gcc)
SET(CMAKE_CXX_COMPILER /usr/local/arm-linux-4.4.2/bin/xscale-linux-g++)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH  /usr/local/arm-linux-4.4.2)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# set libraries search path
SET(CMAKE_LIBRARY_PATH /usr/local/arm-linux-4.4.2/lib/be)

# use ncurses instead of curses
#SET(CURSES_USE_NCURSES TRUE)
