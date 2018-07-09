/*******************************************************************************
File Name : keypad.c

Description :
	The program demo how to code Keypad pragram on UC.

Usage :
	1.Compile this file and execute on UC7000 series.
	2.When you press the F1~5 key of UC,the screen will show
	  the key that you pressed.
History :
	Version		Author		Date		Comment
	1.0		Hank		01-15-2004	Wrote it
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "keypad.h"

static int	fd_keypad= -1;

int	KeypadInit()
{
	fd_keypad = open("/dev/keypad",O_RDWR);
	return fd_keypad;
}

// flag > 1 ==> means how many keys pressed, flag = 0 ==> no key pressed
int	KeypadHasPress()
{
	int flag;
	
	if( fd_keypad < 0)
		if( KeypadInit() < 0)
			return -1;

	ioctl( fd_keypad, IOCTL_KEYPAD_HAS_PRESS, &flag);

	return flag;
}

// key = 0~4 ==> F1~F5 function key, key = -1 ==> no key pressed
int	KeypadGetKey()
{
	int key;
	if( fd_keypad < 0)
		if( KeypadInit() < 0)
			return -1;

	ioctl( fd_keypad, IOCTL_KEYPAD_GET_KEY, &key);

	return key;
}

int     wait_keypad_key(int *key)
{
        int     fd, i, key_count=0, key_old;
        struct timeval tv_start={0, 0}, tv={0, 0};

        fd = open("/dev/keypad", O_RDWR);
        if ( fd < 0 ) {
                printf("Open keypad device fail !\n");
                return -1;
        }

        // first flush all input key
        while ( 1 ) {
                i = 0;
                ioctl(fd, IOCTL_KEYPAD_HAS_PRESS, &i);
                if ( i <= 0 )
                        break;
                ioctl(fd, IOCTL_KEYPAD_GET_KEY, &i);
        }

        // Waiting the *key pressed
        while ( 1 ) {
                i = 0;
                ioctl(fd, IOCTL_KEYPAD_HAS_PRESS, &i);
                if ( i <= 0 ) {

                        gettimeofday(&tv, NULL);
                        if( key_count ) // timeout, return the key
                          if ( (( 1000000* (unsigned long)(tv.tv_sec - tv_start.tv_sec) ) + (tv.tv_usec - tv_start.tv_usec)) > 500000 )  {
                            // timeout
                            *key = key_old;
                            break;
                          }
                        usleep(20 * 1000);
                        continue;
                }
                if( key_count == 0 ) {
                        gettimeofday(&tv_start, NULL);
                  ioctl(fd, IOCTL_KEYPAD_GET_KEY, &key_old);
                }
                else {
                  ioctl(fd, IOCTL_KEYPAD_GET_KEY, &i);
                }

                key_count++;

                if( key_count >=2 ) {
                         *key = key_old;
                   break;
                }
        }
        close(fd);
        return 0;
}

