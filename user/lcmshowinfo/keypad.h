#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
#include "moxadevice.h"

// Defined for DA-661/662/663-16-LX
#define KEY_MENU        0
#define KEY_SEL         1
#define KEY_DOWN        2
#define KEY_UP          3

int	KeypadInit();
int	KeypadHasPress();
int	KeypadGetKey();

/* Get the key from keypad
   *key:
     KEY_MENU           0
     KEY_SEL            1
     KEY_DOWN                   2
     KEY_UP             3
   return:
     0 : success
     -1: fail
 */
int     wait_keypad_key(int *key);
#endif

