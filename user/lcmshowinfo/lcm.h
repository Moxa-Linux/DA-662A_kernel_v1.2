#ifndef LCM_H
#define LCM_H

#include <fcntl.h>
#include "moxadevice.h"

/* Descriptions: Get the cursor position
 * Parameters:
 *    x: the cursor's x position
 *    y: the cursor's y position
 *  return:
 *    none
 */
int LCMGetXY( int *x, int *y);
/* Descriptions: Clean the line of the cursor position
 * Parameters:
 *    none
 *  return:
 *    none
 */
int LCMCleanLine();
/* Descriptions: Clean display
 * Parameters:
 *    none
 *  return:
 *    none
 */
int LCMClear();
/* Descriptions: Set the cursor position
 * Parameters:
 *    x: the cursor's x position
 *    y: the cursor's y position
 *  return:
 *    none
 */
int LCMGotoXY( int x, int y);
/* Descriptions: print a string on LCM
 * Parameters:
 *    ch: the displayed string
 *  return:
 *    none
 */
int LCMPrint( char* str);
/* Descriptions: Turn on the cursor display
 * Parameters:
 *    none
 *  return:
 *    0: successful
 *    -1: fail
 */
int LCMBackLightOn(); // Only avaliable for UC-7410/7420-LX and UC-7410/7420-LX Plus
/* Descriptions: Turn off the blink display
 * Parameters:
 *    none
 *  return:
 *    0: successful
 *    -1: fail
 */
int LCMBackLightOff();  // Only avaliable for UC-7410/7420-LX and UC-7410/7420-LX Plus
/* Descriptions: Turn off the blink display
 * Parameters:
 *    none
 *  return:
 *    0: successful
 *    -1: fail
 */
int LCM_blink_off(void); // Only avaliable for DA-660/661/662/663-LX
/* Descriptions: Turn on the blink display
 * Parameters:
 *    none
 *  return:
 *    0: successful
 *    -1: fail
 */
int LCM_blink_on(void); // Only avaliable for DA-660/661/662/663-LX
/* Descriptions: Turn off the cursor display
 * Parameters:
 *    none
 *  return:
 *    0: successful
 *    -1: fail
 */
int LCM_cursor_off(void); // Only avaliable for DA-660/661/662/663-LX
/* Descriptions: Turn on the cursor display
 * Parameters:
 *    none
 *  return:
 *    0: successful
 *    -1: fail
 */
int LCM_cursor_on(void); // Only avaliable for DA-660/661/662/663-LX


#endif

