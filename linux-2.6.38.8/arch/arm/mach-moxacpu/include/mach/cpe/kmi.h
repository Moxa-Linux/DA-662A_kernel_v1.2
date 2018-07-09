
#ifndef _KMI_H
#define _KMI_H

#define MOUSE_ENABLE 0xF4

#define KEYPAD_INT        	0x04
#define KEYBOARD_TXINT    	0x02
#define KEYBOARD_RXINT    	0x01


/* register */

#define KMI_Control            	0x0
#define KMI_SampleRate        	0x4
#define KMI_RequestToSend     	0x8
#define KMI_Status             	0xC
#define KMI_IntStatus        	0x10
#define KMI_Receive         	0x14
#define KMI_Transmit         	0x18
#define KMI_Keypad_X        	0x30
#define KMI_Keypad_Y         	0x34
#define KMI_AutoscanPeriod     	0x38

/* control register */

#define Autoscan_two_key		0x800
#define Clear_PADINT			0x400
#define Enable_Autoscan			0x200
#define EnableKeypad			0x100
#define Clear_RX_INT			0x80
#define Clear_TX_INT			0x40
#define Line_Control			0x0
#define No_Line_Control			0x20
#define Enable_RX_INT			0x10
#define Enable_TX_INT			0x8
#define Enable_KMI				0x4
#define Force_data_line_low		0x2
#define Force_clock_line_low	0x1

/* Status register */

#define RX_Busy					0x2
#define TX_Busy					0x8
#define RX_Full					0x4
#define TX_Empty				0x10


/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
#ifdef not_complete_yet
extern void fLib_Keyboard_Init(int level_edge);    
extern void fLib_Keyboard_Get(unsigned* input);
extern void fLib_Keyboard_Put(unsigned output);

extern void fLib_Keypad_Init(int level_edge);
extern void fLib_Keypad_AutoScan(unsigned enable);
extern void fLib_Keypad_SetScanPeriod(unsigned data);

extern void fLib_Keypad_GetX(unsigned* ch);
extern void fLib_Keypad_GetY(unsigned* ch);
extern void fLib_Keypad_PutX(unsigned ch);

extern void fLib_Keypad_TwoKeyPush(unsigned enable);
extern void fLib_Keyboard_ClearIntStatus(void);

extern BOOL fLib_KMI_IsReceiveBusy(void);
extern BOOL fLib_KMI_IsSendBusy(void);
extern BOOL fLib_KMI_IsReceiveBufferFull(void);
extern BOOL fLib_KMI_IsSendBufferEmpty(void);

extern UINT32 fLib_KMI_GetIntStatus(void);

extern void Keyboard_ForceDataLow(unsigned enable);
extern void Keyboard_ForceClockLow(unsigned enable);

extern void fLib_Mouse_Init(int level_edge);
extern UINT32 fLib_Mouse_Get(void);
extern void fLib_Mouse_Put(unsigned output);
extern void fLib_Mouse_ClearInt(void);
#endif /* end_of_not */

#endif	// _KMI_H
