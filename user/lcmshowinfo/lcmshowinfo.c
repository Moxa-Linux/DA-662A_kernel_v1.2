/*******************************************************************************
File Name : lcmshowinfo.c

Description : 
	This program demo how to code lcm and keypad program on UC.
	The lcm screen can display 16*8 words (one word is 8*8 pix).

Usage : 
	1.Compile this file and execute on DA-66x series.
	2.The Progarm will show a menu with number, used the keypad to choose 
	the menu on lcm.

History : 
	Version		Author		Date		Comment
	1.0		Jared		01-23-2006	Wrote it.
*******************************************************************************/
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <fcntl.h>
#include  <errno.h>
#include  <termios.h>
#include  "moxadevice.h"
#include  "keypad.h"
#include  "lcm.h"
#include  "id.h"

#ifdef DEBUG
#define dbg_printf(x...)        printf(x)
#else
#define dbg_printf(x...)
#endif

#define MAX_LCM_LINE_NUM     2
#define MAX_LCM_LINE_LEN    16

#define MAX_LEN             20
#define BUF_SIZE           100

#define UP_PATTERN         0xa
#define DOWN_PATTERN       0xb


#define MOXA                    0x400
#define RS232_MODE              0
#define RS485_2WIRE_MODE        1
#define RS422_MODE              2
#define RS485_4WIRE_MODE        3
#define NOT_SET_MODE		4

struct speed_map {
	speed_t speed;                  /* Internal form. */
	unsigned long value;            /* Numeric value. */
};

static const struct speed_map speeds[] = {
	{B0, 0},
	{B50, 50},
	{B75, 75},
	{B110, 110},
	{B134, 134},
	{B150, 150},
	{B200, 200},
	{B300, 300},
	{B600, 600},
	{B1200, 1200},
	{B1800, 1800},
	{B2400, 2400},
	{B4800, 4800},
	{B9600, 9600},
	{B19200, 19200},
	{B38400, 38400},
	{B57600, 57600},
	{B115200, 115200},
	{B230400, 230400},
	{B460800, 460800},
	{B921600, 921600},
};

typedef struct _screen_t {
	char line[ MAX_LCM_LINE_NUM ][ MAX_LEN ];
} Screen_t;

typedef struct _Select_Table_Struct {
	char 	Description[MAX_LCM_LINE_LEN];
	int	  SelectValue;
	int	  (*Func)(void *pParam);
	void	*Param;
} SelectTableStruct, *pSelectTableStruct;

typedef struct _Menu_Struct {
	char	*Title;
	int	TableNo;
	int CurrentSelect;
	pSelectTableStruct	SelectTable;
} MenuStruct_t, *pMenuStruct_t;

union ver_union {
	unsigned long	ul; // version unsigned long format
	struct s_part {
		unsigned char	major;  // major version number
		unsigned char	minor;  // minor version number
		unsigned char	oem;    // oem version number
		unsigned char	testing;// oem upgrade version number
	} part;
};

// The serial port Number is ( MU860# * 8 )
// We use this control the number of serial port information displayed on LCM
static int nMU860Num;  

inline void MenuScrollDown(void *pMenu);
inline void MenuScrollUp(void *pMenu);
inline void LCMDisplayCursorOnFirstLineEnd(void);
inline int execShellCMD(char *instruction, char *result);
inline int getSerialPortInfo(char *device_name, unsigned long *speed, char *parenb, int *bit_num, float *stops, char *interface_type);
inline unsigned long baud_to_value(speed_t speed);
void Initialize_ModelInfo(void);
void LCMDisplayScreen( Screen_t *pScreen );
int LCMDisplayModelName(void *pParam);
int LCMDisplayMenu(void *pMenu);
int EnterMenu(void *pMenu);
int LCMDisplaySystemInfo(void *pMenu);
int LCMDisplayNetworkInfo(void *interface);
int LCMDisplaySerialPortInfo(void *pMenu);
int LCMDisplayConsolePortInfo(void *pMenu);

// Network menu index
#define NETWORK_SEL_NUM      3
#define NETWORK_MENU_ETH0   20
#define NETWORK_MENU_ETH1   21
#define NETWORK_MENU_ETH2   22
#define NETWORK_MENU_ETH3   23
#define NETWORK_MENU_ETH4   24
#define NETWORK_MENU_RETURN 25

static SelectTableStruct NetworkMenuTable[] = {
	{"eth0",   NETWORK_MENU_ETH0,   LCMDisplayNetworkInfo, NULL },
	{"eth1",   NETWORK_MENU_ETH1,   LCMDisplayNetworkInfo, NULL },
	{"eth2",   NETWORK_MENU_ETH2,   LCMDisplayNetworkInfo, NULL },
	{"eth3",   NETWORK_MENU_ETH3,   LCMDisplayNetworkInfo, NULL },
	{"eth4",   NETWORK_MENU_ETH4,   LCMDisplayNetworkInfo, NULL },
	{"Return", NETWORK_MENU_RETURN, NULL,                  NULL },
};

static MenuStruct_t NetworkMenu = {
	.Title         = "Network Menu",
	.TableNo       = sizeof(NetworkMenuTable)/sizeof(SelectTableStruct),
	.CurrentSelect = 0,
	.SelectTable   = NetworkMenuTable	
};

static char Model_Name[BUF_SIZE];

// Serial port menu index
#define SERIAL_MENU_SERIAL0   30
#define SERIAL_MENU_SERIAL1   31
#define SERIAL_MENU_SERIAL2   32
#define SERIAL_MENU_SERIAL3   33
#define SERIAL_MENU_SERIAL4   34
#define SERIAL_MENU_SERIAL5   35
#define SERIAL_MENU_SERIAL6   36
#define SERIAL_MENU_SERIAL7   37
#define SERIAL_MENU_SERIAL8   38
#define SERIAL_MENU_SERIAL9   39
#define SERIAL_MENU_SERIAL10  40
#define SERIAL_MENU_SERIAL11  41
#define SERIAL_MENU_SERIAL12  42
#define SERIAL_MENU_SERIAL13  43
#define SERIAL_MENU_SERIAL14  44
#define SERIAL_MENU_SERIAL15  45
#define SERIAL_MENU_RETURN    46

static SelectTableStruct SerialMenuTable[] = {
	{"Serial Port 1",   SERIAL_MENU_SERIAL0,   LCMDisplaySerialPortInfo, NULL  },
	{"Serial Port 2",   SERIAL_MENU_SERIAL1,   LCMDisplaySerialPortInfo, NULL  },
	{"Serial Port 3",   SERIAL_MENU_SERIAL2,   LCMDisplaySerialPortInfo, NULL  },
	{"Serial Port 4",   SERIAL_MENU_SERIAL3,   LCMDisplaySerialPortInfo, NULL  },
	{"Serial Port 5",   SERIAL_MENU_SERIAL4,   LCMDisplaySerialPortInfo, NULL  },
	{"Serial Port 6",   SERIAL_MENU_SERIAL5,   LCMDisplaySerialPortInfo, NULL  },
	{"Serial Port 7",   SERIAL_MENU_SERIAL6,   LCMDisplaySerialPortInfo, NULL  },
	{"Serial Port 8",   SERIAL_MENU_SERIAL7,   LCMDisplaySerialPortInfo, NULL  },
	{"Serial Port 9",   SERIAL_MENU_SERIAL8,   LCMDisplaySerialPortInfo, NULL  },
	{"Serial Port 10",  SERIAL_MENU_SERIAL9,   LCMDisplaySerialPortInfo, NULL  },
	{"Serial Port 11",  SERIAL_MENU_SERIAL10,  LCMDisplaySerialPortInfo, NULL  },
	{"Serial Port 12",  SERIAL_MENU_SERIAL11,  LCMDisplaySerialPortInfo, NULL  },
	{"Serial Port 13",  SERIAL_MENU_SERIAL12,  LCMDisplaySerialPortInfo, NULL  },
	{"Serial Port 14",  SERIAL_MENU_SERIAL13,  LCMDisplaySerialPortInfo, NULL  },
	{"Serial Port 15",  SERIAL_MENU_SERIAL14,  LCMDisplaySerialPortInfo, NULL  },
	{"Serial Port 16",  SERIAL_MENU_SERIAL15,  LCMDisplaySerialPortInfo, NULL  },
	{"Return",          SERIAL_MENU_RETURN,    NULL,                     NULL  },
};

static MenuStruct_t SerialMenu = {
	.Title         ="Serial Menu",
	.TableNo       = sizeof(SerialMenuTable)/sizeof(SelectTableStruct),
	.CurrentSelect = 0,
	.SelectTable   = SerialMenuTable,	
};

// Main menu index
#define MAIN_MENU_SYSINFO   0
#define MAIN_MENU_NETWORK   1
#define MAIN_MENU_SERIAL    2
#define MAIN_MENU_CONSOLE   3
#define MAIN_MENU_RETURN    4

static SelectTableStruct MainSelectTable[] = {
	{"System Info" , MAIN_MENU_SYSINFO,  LCMDisplaySystemInfo,      NULL         },
	{"Network"     , MAIN_MENU_NETWORK,  EnterMenu,                 &NetworkMenu },
	{"Serial port" , MAIN_MENU_SERIAL,   EnterMenu,                 &SerialMenu  },
	{"Console port", MAIN_MENU_CONSOLE,  LCMDisplayConsolePortInfo, "/dev/ttyS0" },
	{"Return"      , MAIN_MENU_RETURN,   NULL,                      NULL         },
};

static MenuStruct_t MainMenu = {
	.Title         = "Main Menu",
	.TableNo       = sizeof(MainSelectTable)/sizeof(SelectTableStruct),
	.CurrentSelect = 0,
	.SelectTable   = MainSelectTable
};

inline void MenuScrollDown(void *pMenu)
{
	pMenuStruct_t ptmpMenu = (pMenuStruct_t)pMenu;

	if( ptmpMenu->CurrentSelect != ptmpMenu->TableNo - 1 )
		ptmpMenu->CurrentSelect++ ;
}

inline void MenuScrollUp(void *pMenu)
{
	pMenuStruct_t ptmpMenu = (pMenuStruct_t)pMenu;

	if(  ptmpMenu->CurrentSelect != 0 )
		ptmpMenu->CurrentSelect-- ;
}

inline void LCMDisplayCursorOnFirstLineEnd(void)
{
	LCMGotoXY(MAX_LCM_LINE_LEN-1, 0);
}

static const int NUM_SPEEDS = (sizeof(speeds) / sizeof(struct speed_map));

inline unsigned long baud_to_value(speed_t speed)
{
	int	i;

	for (i = 0; i < NUM_SPEEDS; ++i)
		if (speed == speeds[i].speed)
			return speeds[i].value;
	return 0;
}

inline int getSerialPortInfo(char *device_name, unsigned long *speed, char *parenb, int *bit_num, float *stops, char *interface_type)
{
	int		fd, interface;
	struct termios	mode;
	
	fd = open(device_name, O_RDONLY | O_NONBLOCK);
	if (fd < 0) {
		dbg_printf("%s open fail!", device_name);
		return -1;
	}
	
	memset(&mode, 0, sizeof(struct termios));
	if (tcgetattr(fd, &mode)) {
		dbg_printf("%s tcgetattr fail!", device_name);
	}
	ioctl(fd, MOXA_GET_OP_MODE, &interface);
	switch ( interface ) {
	case RS232_MODE :
		strcpy(interface_type, "RS232");
		dbg_printf("Now setting is RS232 interface.\n");
		break;
	case RS485_2WIRE_MODE :
		strcpy(interface_type, "RS485-2WIRE");
		dbg_printf("Now setting is RS485-2WIRES interface.\n");
		break;
	case RS422_MODE :
		strcpy(interface_type, "RS422");
		dbg_printf("Now setting is RS422 interface.\n");
		break;
	case RS485_4WIRE_MODE :
		strcpy(interface_type, "RS485-4WIRE");
		dbg_printf("Now setting is RS485-4WIRES interface.\n");
		break;
	case NOT_SET_MODE :
		dbg_printf("Now does not set to any type interface.\n");
		break;
	default :
		dbg_printf("Unknow interface is set.\n");
	}
	
	close(fd);
	
	if( (mode.c_cflag & (CBAUD|CBAUDEX)) != B4000000) {
		*speed = baud_to_value(cfgetospeed(&mode));
		dbg_printf("!= B4000000: *CBAUD:%x, CBAUDEX:%x, speed:%d!\n", CBAUD, CBAUDEX, *speed);
	} else {
		ioctl(fd, MOXA_GET_SPECIAL_BAUD_RATE, speed);
		dbg_printf("== B4000000: CBAUD:%x, CBAUDEX:%x, *speed:%d!\n", CBAUD, CBAUDEX, *speed);
	}
	
	if( (mode.c_cflag & PARENB) && (mode.c_cflag & PARODD ) )
		*parenb = 'o';
	else if ( mode.c_cflag & PARENB )
		*parenb = 'e';
	else
		*parenb = 'n';
	
	dbg_printf("mode.c_cflag: %x, CS5:%x, CS6:%x CS7:%x CS8:%x!\n", mode.c_cflag, CS5, CS6, CS7, CS8);
	if ( mode.c_cflag & CS8 )
		*bit_num = 8;
	else if ( mode.c_cflag & CS7 )
		*bit_num = 7;
	else if ( mode.c_cflag & CS6 )
		*bit_num = 6;
	else
		*bit_num = 5;	

	dbg_printf("mode.c_cflag: %x, CSTOPB:%x mode.c_cflag & CSTOPB:%x\n", mode.c_cflag, CSTOPB, mode.c_cflag & CSTOPB);
	if ( mode.c_cflag & CSTOPB ) {
		if( *bit_num != 5 )
			*stops = 2;
		else
			*stops = 1.5;
	} else
		*stops = 1;
	
	return 0;
}

inline int execShellCMD(char *instruction, char *result)
{
	FILE	*read_fp;
	int	chars_read;
	
	read_fp = popen(instruction, "r");
	if(read_fp==NULL)
		return -1;
  
	chars_read = fread( result, sizeof(char), BUF_SIZE, read_fp);
	if(chars_read==0) {
		dbg_printf(" pipe read error\n");
		return -1;
	}
	result[chars_read-1]= '\0';
	pclose(read_fp);
	return chars_read;
}

int LCMDisplaySystemInfo(void *pMenu)
{	
  int total_mem, used_mem, free_mem, key, tmp;
  float total_disk, used_disk, free_disk;
  char result[BUF_SIZE];
  Screen_t sysinfo;
  
  // Get system memory information
  execShellCMD("free |grep Total", &result[0]);
  sscanf(result, "Total: %d %d %d", &total_mem, &used_mem, &free_mem);
  sprintf(sysinfo.line[0], "Mem:%d/%dM", used_mem>>10, total_mem>>10);
  
  // Get system memory information
  execShellCMD("df -h|grep  mtdblock3", &result[0]);
  dbg_printf("%s\n", result);
  sscanf(result, "/dev/mtdblock3 %fM %fM %fM", &total_disk, &used_disk, &free_disk);
  dbg_printf( "Disk:%.1f/%.1fM", used_disk, total_disk);
  sprintf(sysinfo.line[1], "Disk:%.1f/%.1fM", used_disk, total_disk);
  
  LCMDisplayScreen(&sysinfo);
	
  while(1) {
    wait_keypad_key(&key);
    switch(key) {
      case KEY_MENU: // return to Main Menu
        return 1;
      case KEY_SEL: 
        return 0;
    }
  }
  return 0;
}

int LCMDisplayNetworkInfo(void *pParam)
{
  int	key, index, index_upper_bound;
  char result[BUF_SIZE], instruction[BUF_SIZE], tmpBuf[BUF_SIZE];
  Screen_t network_info;
  pMenuStruct_t ptmpMenu = (pMenuStruct_t)&NetworkMenu;
  index = ptmpMenu->CurrentSelect * 3;
  index_upper_bound = ((ptmpMenu->TableNo -1) *3)-1;
  
  dbg_printf("index_upper_bound:%d\n", index_upper_bound);
  
  do {
	  
    switch( index %3 ) {
      case 0:
        sprintf( instruction, "ifconfig %s|grep \"inet addr\"|cut -d ' ' -f12|cut -d ':' -f2", ptmpMenu->SelectTable[ index/3 ].Description);
        execShellCMD( instruction, result );
        sprintf( tmpBuf, "%s:IP", ptmpMenu->SelectTable[ index/3 ].Description );
	break;
      case 1:
        sprintf( instruction, "ifconfig %s|grep \"inet addr\"|cut -d ' ' -f14|cut -d ':' -f2", ptmpMenu->SelectTable[ index/3 ].Description);
        execShellCMD( instruction, result );
        sprintf( tmpBuf, "%s:Broadcast", ptmpMenu->SelectTable[ index/3 ].Description );
        break;
      case 2:
        sprintf( instruction, "ifconfig %s|grep \"inet addr\"|cut -d ' ' -f16|cut -d ':' -f2", ptmpMenu->SelectTable[ index/3 ].Description);
        execShellCMD( instruction, result );
        sprintf( tmpBuf, "%s:Netmask", ptmpMenu->SelectTable[ index/3 ].Description );
	break;
    }

    // Set the display buffer
    memset(&network_info, 0, sizeof(Screen_t));
    if ( index == 0 ) {
      sprintf( network_info.line[0], "%-15s", tmpBuf );
      sprintf( network_info.line[1], "%-15s%c", result, DOWN_PATTERN);
    }
    else if ( index < index_upper_bound ) {
      sprintf( network_info.line[0], "%-15s%c", tmpBuf, UP_PATTERN);
      sprintf( network_info.line[1], "%-15s%c", result, DOWN_PATTERN);
    }
    else if ( index == index_upper_bound ) {
      sprintf( network_info.line[0], "%-15s%c", tmpBuf, UP_PATTERN);
      sprintf( network_info.line[1], "%-15s", result );
    }
    LCMDisplayScreen(&network_info);
	  
    wait_keypad_key(&key);
  
    switch(key) {
      case KEY_MENU: // return to Main Menu
        return 1;
      break;
      case KEY_SEL:  // return to the parent's Menu
        return 0;
      case KEY_UP:   // display next Ethernet port information
	if(  index != 0 )
          index-- ;
        break;
      case KEY_DOWN: // display previous Ethernet port information
	if( index != index_upper_bound )
	  index++ ;
        break;
    }
  } while(1);
	
  return 0;
}

int LCMDisplaySerialPortInfo(void *pParam)
{
  struct termios mode;
  int len, csize, key, port_num;
  unsigned long ulSpeed;
  char parenb, result[BUF_SIZE], device_file[BUF_SIZE], interface_type[MAX_LCM_LINE_LEN];
  float cstop;
  Screen_t serial_info;
  pMenuStruct_t ptmpMenu = (pMenuStruct_t)&SerialMenu;
  
  while(1) {
  	
    // Get the port
    sprintf( device_file, "/dev/ttyM%d", ptmpMenu->CurrentSelect);	
    getSerialPortInfo( device_file, &ulSpeed, &parenb, &csize, &cstop, interface_type);
    dbg_printf("speed:%d, parity en:%c, csize:%d, stop bits:%.0f\n", ulSpeed, parenb, csize, cstop);
  	
    // Set the display buffer
    memset(&serial_info, 0, sizeof(Screen_t));
    if ( ptmpMenu->CurrentSelect == 0 ) {
      //sprintf( serial_info.line[0], "%-15s:", ptmpMenu->SelectTable[ptmpMenu->CurrentSelect].Description );
      sprintf( serial_info.line[0], "P%-2d:%-11s", ptmpMenu->CurrentSelect+1, interface_type );
      sprintf( result, "%d,%c,%d,%.0f", ulSpeed, parenb, csize, cstop);
      sprintf( serial_info.line[1], "%-15s%c", result, DOWN_PATTERN);
    }
    else if ( ptmpMenu->CurrentSelect < ptmpMenu->TableNo -2 ) {
      //sprintf( serial_info.line[0], "%-15s%c:", ptmpMenu->SelectTable[ ptmpMenu->CurrentSelect ].Description, UP_PATTERN);
      sprintf( serial_info.line[0], "P%-2d:%-11s%c", ptmpMenu->CurrentSelect+1, interface_type, UP_PATTERN);
      sprintf( result, "%d,%c,%d,%.0f", ulSpeed, parenb, csize, cstop);
      sprintf( serial_info.line[1], "%-15s%c", result, DOWN_PATTERN);
    }
    else if ( ptmpMenu->CurrentSelect == ptmpMenu->TableNo -2 ) {
      //sprintf(serial_info.line[0], "%-15s%c:",ptmpMenu->SelectTable[ ptmpMenu->CurrentSelect ].Description, UP_PATTERN);
      sprintf(serial_info.line[0], "P%-2d:%-11s%c:", ptmpMenu->CurrentSelect+1, interface_type, UP_PATTERN);
      sprintf( result, "%d,%c,%d,%.0f", ulSpeed, parenb, csize, cstop);
      sprintf( serial_info.line[1], "%-15s", result);
    }
  	
    LCMDisplayScreen(&serial_info);

    wait_keypad_key(&key);
    
    switch(key) {
      case KEY_MENU: // return to Main Menu
        return 1;
      case KEY_SEL:  // return to the parent's Menu
        return 0;
      case KEY_UP:   // display next serial port information
	if(  ptmpMenu->CurrentSelect != 0 )
          ptmpMenu->CurrentSelect-- ;
        break;
      case KEY_DOWN: // display previous serial port information
	if( ptmpMenu->CurrentSelect != ptmpMenu->TableNo - 2 )
	  ptmpMenu->CurrentSelect++ ;
        break;
    }
  }
  return 0;
}

int LCMDisplayConsolePortInfo(void *device_file)
{
  struct termios mode;
  int len, csize, key;
  unsigned long ulSpeed;
  char parenb, result[BUF_SIZE], interface_type[MAX_LCM_LINE_LEN];
  float cstop;
  Screen_t consoleinfo;
	
  // Get the getty information
  len = execShellCMD("grep getty /etc/inittab | grep -v '#'", &result[0]);
  if( len == 0 ) {     // /sbin/getty is not running now: console disable
  	sprintf(consoleinfo.line[0], "Console:disable");
  }
  else if( len > 0 ) { // /sbin/getty is running now: console enable
    sprintf(consoleinfo.line[0], "Console:enable");  
  }
  else {
  	dbg_printf("Cannot get console information");
  }
  dbg_printf("%s", consoleinfo.line[0]);
  
  getSerialPortInfo((char*)device_file, &ulSpeed, &parenb, &csize, &cstop, interface_type);
	
  dbg_printf("speed:%d, parity en:%c, csize:%d, stop bits:%.0f", ulSpeed, parenb, csize, cstop);
  sprintf(consoleinfo.line[1], "%d,%c,%d,%.0f", ulSpeed, parenb, csize, cstop);
	
  LCMDisplayScreen(&consoleinfo);
	
  while(1) {
    wait_keypad_key(&key);
    switch(key) {
      case KEY_MENU: // return to Main Menu
        return 1;
      case KEY_SEL: 
        return 0;
    }		
  }
	
  return 0;
}

//
//  Adjust some variables dependent on the model
//
void Initialize_ModelInfo(void)
{
	char		result[BUF_SIZE];
	int		len;
	unsigned int	swid;
	
	// Get software ID from redboot partition and check which product this is.
	swid = get_swid();
	switch(swid) {
	case DA662A_8_SOFTWARE_ID :
		nMU860Num = 1;
	  	SerialMenu.TableNo = nMU860Num*8 + 1 ;
	  	strcpy( SerialMenu.SelectTable[SerialMenu.TableNo-1].Description, "Return" );
	  	SerialMenu.SelectTable[SerialMenu.TableNo-1].SelectValue = SERIAL_MENU_RETURN;
	  	SerialMenu.SelectTable[SerialMenu.TableNo-1].Func = NULL;
	case DA662A_16_SOFTWARE_ID :
		if ( swid == DA662A_16_SOFTWARE_ID )
			nMU860Num = 2;
  		NetworkMenu.TableNo = 4 + 1 ;
	  	strcpy( NetworkMenu.SelectTable[NetworkMenu.TableNo-1].Description, "Return" );
	  	NetworkMenu.SelectTable[NetworkMenu.TableNo-1].SelectValue = NETWORK_MENU_RETURN;
	  	NetworkMenu.SelectTable[NetworkMenu.TableNo-1].Func = NULL;
  		sprintf(Model_Name, "DA-662A-%d-LX" , nMU860Num*8);
		break;
  	case DA660_SOFTWARE_ID: // Model Name is DA-660-LX-8 or DA-660-LX-16, , 2 LANs
		// grep the number of MU860 vendor/device ID
  		len = execShellCMD("cat /proc/pci | grep 1393:0001 |wc -l", &result[0]);
  		sscanf(result, "      %d", &nMU860Num);
  
		// For flexibility used on both hardwares, DA-660-8-LX / DA-660-16-LX
		// For DA-660-8-LX, adjust the # of SerialMenu size. 
		// The Table size is serial port# ( MU860# * 8 ) + 1 (for "Return") 
		// and modify the last SelectTable[] to "Return"
  		if( nMU860Num == 1 ) {
	  		SerialMenu.TableNo = nMU860Num*8 + 1 ;
	  		strcpy( SerialMenu.SelectTable[SerialMenu.TableNo-1].Description, "Return" );
	  		SerialMenu.SelectTable[SerialMenu.TableNo-1].SelectValue = SERIAL_MENU_RETURN;
	  		SerialMenu.SelectTable[SerialMenu.TableNo-1].Func = NULL;
	  	}
	  	
	  	NetworkMenu.TableNo = 2 + 1 ;
	  	strcpy( NetworkMenu.SelectTable[NetworkMenu.TableNo-1].Description, "Return" );
	  	NetworkMenu.SelectTable[NetworkMenu.TableNo-1].SelectValue = NETWORK_MENU_RETURN;
	  	NetworkMenu.SelectTable[NetworkMenu.TableNo-1].Func = NULL;
	  	
  		sprintf(Model_Name, "DA-660-LX-%d" , nMU860Num*8);
  		break;
  	case DA661_SOFTWARE_ID: // Model Name is DA-661-LX, 2 LANs
  		NetworkMenu.TableNo = 2 + 1 ;
	  	strcpy( NetworkMenu.SelectTable[NetworkMenu.TableNo-1].Description, "Return" );
	  	NetworkMenu.SelectTable[NetworkMenu.TableNo-1].SelectValue = NETWORK_MENU_RETURN;
	  	NetworkMenu.SelectTable[NetworkMenu.TableNo-1].Func = NULL;
  		sprintf(Model_Name, DA661_16_MODE_NAME);
  		break;
  	case DA662_SOFTWARE_ID: // Model Name is DA-662-LX, 4 LANs
  		NetworkMenu.TableNo = 4 + 1 ;
	  	strcpy( NetworkMenu.SelectTable[NetworkMenu.TableNo-1].Description, "Return" );
	  	NetworkMenu.SelectTable[NetworkMenu.TableNo-1].SelectValue = NETWORK_MENU_RETURN;
	  	NetworkMenu.SelectTable[NetworkMenu.TableNo-1].Func = NULL;
  		sprintf(Model_Name, DA662_16_MODE_NAME);
  		break;
  	case DA663_SOFTWARE_ID: // Model Name is DA-663-LX, 2 LANs
  		NetworkMenu.TableNo = 2 + 1 ;
	  	strcpy( NetworkMenu.SelectTable[NetworkMenu.TableNo-1].Description, "Return" );
	  	NetworkMenu.SelectTable[NetworkMenu.TableNo-1].SelectValue = NETWORK_MENU_RETURN;
	  	NetworkMenu.SelectTable[NetworkMenu.TableNo-1].Func = NULL;
	  	sprintf(Model_Name, DA663_16_MODE_NAME);
  		break;
  }
}

/*
 * Display new information on LCM
 */
void LCMDisplayScreen(Screen_t *pScreen)
{
	LCMClear();
	LCMPrint(pScreen->line[0]);
	LCMGotoXY(0, 1);
	LCMPrint(pScreen->line[1]);
	LCMDisplayCursorOnFirstLineEnd();
	dbg_printf("%s\n", pScreen->line[0]);
	dbg_printf("%s\n", pScreen->line[1]);
}
/*
 * Display model name and version information on LCM
 */
int LCMDisplayModelName(void *pParam)
{
	union ver_union	ver;
	Screen_t	model_version_info;
  
	sprintf(model_version_info.line[0], Model_Name);
	ver.ul = 0;
	ver.part.major = MAJORVER;
	ver.part.minor = MINORVER;
#ifdef OEMVER
	ver.part.oem = OEMVER;
#endif
	if ( ver.part.oem ) {
		sprintf(model_version_info.line[1], "Ver %d.%d.%d", ver.part.major, ver.part.minor, ver.part.oem);
	} else {
		sprintf(model_version_info.line[1], "Ver %d.%d", ver.part.major, ver.part.minor);
	}
	LCMDisplayScreen(&model_version_info);
	return 0;
}

/*
 * Display the menu on LCM
 */
int LCMDisplayMenu(void *pMenu)
{
  Screen_t display_menu;
  pMenuStruct_t ptmpMenu = (pMenuStruct_t)pMenu;
  
  memset(&display_menu, 0, sizeof(Screen_t));
  
  dbg_printf("TableNo:%d, CurrentSelect:%d\n", ptmpMenu->TableNo, ptmpMenu->CurrentSelect);

  // DOWN_PATTERN(0xb)   UP_PATTERN(0xa)
  if ( ptmpMenu->CurrentSelect == 0 ) {
    sprintf(display_menu.line[0], "%-15s", ptmpMenu->SelectTable[ ptmpMenu->CurrentSelect ].Description);
    sprintf(display_menu.line[1], "%-15s%c", ptmpMenu->SelectTable[ ptmpMenu->CurrentSelect+1 ].Description, DOWN_PATTERN);
  }
  else if ( ptmpMenu->CurrentSelect < ptmpMenu->TableNo -1 ) {
    sprintf(display_menu.line[0], "%-15s%c", ptmpMenu->SelectTable[ ptmpMenu->CurrentSelect ].Description, UP_PATTERN);
    sprintf(display_menu.line[1], "%-15s%c", ptmpMenu->SelectTable[ ptmpMenu->CurrentSelect+1 ].Description, DOWN_PATTERN);
  }
  else if ( ptmpMenu->CurrentSelect == ptmpMenu->TableNo -1 ) {
    sprintf(display_menu.line[0], "%-15s%c",ptmpMenu->SelectTable[ ptmpMenu->CurrentSelect ].Description, UP_PATTERN);
  }

  LCMDisplayScreen(&display_menu);;
  LCMDisplayCursorOnFirstLineEnd();
  
  return 0;
}

/*
 * Enter a menu
 */
int EnterMenu(void *pMenu)
{
  int	key;
  int retval;
  pMenuStruct_t ptmpMenu = (pMenuStruct_t)pMenu;
	
  while( 1 ) {
  	
  	LCM_blink_on();
    LCMDisplayMenu(ptmpMenu);
    wait_keypad_key(&key);
    dbg_printf("EnterMenu:%s key:%d\n", ptmpMenu->Title, key);
    switch(key) {
      case KEY_MENU:
        return 1;
      case KEY_DOWN:
	MenuScrollDown(ptmpMenu);
        break;			
      case KEY_UP:
	MenuScrollUp(ptmpMenu);
        break;
      case KEY_SEL:
        if( strcmp(ptmpMenu->SelectTable[ptmpMenu->CurrentSelect].Description, "Return" ) == 0 ) {
          if( strcmp( ptmpMenu->Title, "Main Menu" ) == 0 ) {
	    // Return to show the model name and version informatino
            dbg_printf("Main Menu|Return");
            return 0;  
          }
	  else { // Return to Main Menu
	    dbg_printf("not Main Menu|Return");
            ptmpMenu = &MainMenu;
	  }
	}
        else if( ptmpMenu->SelectTable[ptmpMenu->CurrentSelect].Func != NULL ) {
          retval=(*ptmpMenu->SelectTable[ptmpMenu->CurrentSelect].Func)( ptmpMenu->SelectTable[ptmpMenu->CurrentSelect].Param ) ;

          if( retval == 1 && strcmp( ptmpMenu->Title, "Main Menu" ) != 0 ) //return to Main Menu
            return 1;
   	}
          break;
        default:
	  // Do nothing
	  dbg_printf("Do nothing!\n");
	  break;
    }
  }
  return 0;
}

int main(int argc, char *argv[])
{
	int		key;
	lcm_xy_t	xy;
	
	Initialize_ModelInfo();
	LCM_cursor_off();
	LCMDisplayModelName(NULL);

	while( 1 ) {
		LCM_blink_off();
		wait_keypad_key(&key);
		switch(key) {
		case KEY_MENU:
			EnterMenu((void*)&MainMenu);
			LCMDisplayModelName(NULL);
			break;
		case KEY_DOWN:
		case KEY_UP:
		case KEY_SEL:			
		default:
			// Do nothing
			dbg_printf("Do nothing!\n");
			break;
		}
	}
	return 0;
}
