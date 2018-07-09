/******************************************************************************

 (c) 2011 Cinterion Wireless Modules GmbH <technical.feedback@cinterion.com>

 All Rights Reserved.

 Cinterion provides this source code under the GPL v2 License.
 The GPL v2 license is available at

 http://opensource.org/licenses/gpl-license.php

******************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//
// Linmuxcfg.c
//
// This file contains the implementation of the linmux configuration command
// line utility
//
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/ioctl.h>

#include "linmuxcfg.h"

//////////////////////////////////////////////////////////////////////////////

#define DEVICE_NAME "/dev/"MUX_CFG_NAME

//////////////////////////////////////////////////////////////////////////////

static struct option Opts[] = {
  { "baud_rate",        optional_argument,  0, 'b'},
  { "init_cmd",         optional_argument,  0, 'c'},
  { "device_name",      optional_argument,  0, 'd'},
  { "deinit_cmd",       optional_argument,  0, 'e'},
  { "hdlc_frame_size",  optional_argument,  0, 'f'},
  { "help",             no_argument,        0, 'h'},
  { "instance",         required_argument,  0, 'i'},
  { "trace_mask",       optional_argument,  0, 'm'},
  { "num_of_channels",  optional_argument,  0, 'n'},
  { "switchoff_on",     optional_argument,  0, 'o'},
  { "base_port",        optional_argument,  0, 'p'},
  { "reload",           no_argument,        0, 'r'},
  { "init_on",          optional_argument,  0, 't'},
  { "max_mux_ver",      optional_argument,  0, 'v'},
  { "hdlc_win_size",    optional_argument,  0, 'w'},
  { "cmd_index",        required_argument,  0, 'x'},
  { 0, 0, 0, 0}
};
static char strOpts[] = "hri:x:p::d::b::m::n::c::v::e::w::f::o::t::";

//////////////////////////////////////////////////////////////////////////////

#define HELP_TEXT                                                                    \
"Usage: linmuxcfg [OPTIONS]...\n"                                                    \
"Send configuration settings to the linmux serial multiplex driver\n"                \
"\n"                                                                                 \
"All arguments for long options have the same meaning as for short arguments.\n"     \
"Obmitting non-mandatory arguments leads to a query and output of the current\n"     \
"value. The format of the output is \"argument[instance]=<value>\" or in case of\n"  \
"the init commands \"init_cmd[instance][cmd_index]=<value>\" or in case of the\n"    \
"trace mask \"trace_mask=0x<hex-value>\".\n"                                         \
"All arguments are executed immediately in the order they appear in the arguement\n" \
"list. That means that settings like the instance number are valid for all\n"        \
"following arguments until the setting is chaged again. In case of an error\n"       \
"execution of the following options is continued.\n"                                 \
"\n"                                                                                 \
"  -b, --baud_rate[=value]   Set or get the baud rate value of the current driver\n" \
"                            instance. A new baud rate will be used when the\n"      \
"                            multiplex mode is started the next time by closing\n"   \
"                            and reopening all virtual ports. The default baud\n"    \
"                            rate is %d.\n"                                          \
"  -c, --init_cmd[=cmd]      Set or get an initialization command of the current\n"  \
"                            driver instance. These commands are user definable\n"   \
"                            AT commands which are sent to the module before the\n"  \
"                            multiplex mode is started additional to the commands\n" \
"                            which are mandatory for the mux mode initialization.\n" \
"                            The required carriage return is appended\n"             \
"                            automatically. Currently the usage of up to %d init\n"  \
"                            commands with a maximum length of %d characters is\n"   \
"                            possible. The index of the command to be set or get\n"  \
"                            can be configured with \"--cmd_index\" in front of\n"   \
"                            this option. New init commands will be used when the\n" \
"                            multiplex mode is started the next time by closing\n"   \
"                            and reopening all vitual ports.\n"                      \
"  -d, --device_name[=name]  Set or get the device name of the virtual ports. If\n"  \
"                            there is only one driver instance used the default\n"   \
"                            value is \"%s\", if there are more instances used\n"    \
"                            the default values are \"%sA\", \"%sB\" etc..\n"        \
"                            To make this setting valid a reload of the\n"           \
"                            corresponding driver instance is required (see\n"       \
"                            \"--reload\").\n"                                       \
"  -e, --deinit_cmd[=cmd]    Set or get an deinitialization commands of the\n"       \
"                            current driver instance. These commands are user\n"     \
"                            definable AT commands which are sent to the module\n"   \
"                            after the multiplex mode has been shut down. The\n"     \
"                            required carriage return is appended automatically.\n"  \
"                            Currently the usage of up to %d deinit commands with\n" \
"                            a maximum length of %d characters is possible. The\n"   \
"                            index of the command to be set or get can be\n"         \
"                            configured with \"--cmd_index\" in front of this\n"     \
"                            option. New deinit commands will be used when the\n"    \
"                            multiplex mode is closed the next time by closing\n"    \
"                            all virtual ports.\n"                                   \
"  -f, --hdlc_frame_size[=value]\n"                                                  \
"                            Set or get the hdlc frame size of the current\n"        \
"                            instance. Multiplex protocol version 4 and above are\n" \
"                            using a hdlc framing mechanism for error correction.\n" \
"                            With this option the size of hdlc frames can be\n"      \
"                            adjusted between 1 and %d. The default value is\n"      \
"                            %d. The setting is used when the multiplex mode\n"      \
"                            is started the next time.\n"                            \
"                            Please note that this parameter defines only the\n"     \
"                            max. frame size to be used. The real size is\n"         \
"                            negotiated with the connected module and can be\n"      \
"                            smaller, depending on the modules capabilities.\n"      \
"  -h, --help                Display this help and exit.\n"                          \
"  -i, --instance=value      Set the driver instance for the following options.\n"   \
"                            This index is zero based. If this options isn't set\n"  \
"                            all arguments apply to instance 0. The number of\n"     \
"                            available driver instances is set with the module\n"    \
"                            parameter \"instances\" when the module is\n"           \
"                            installed.\n"                                           \
"  -m, --trace_mask[=value]  Set or get the trace mask defining the level of\n"      \
"                            trace output of the driver during operation. This\n"    \
"                            value is globally valid for all driver instances and\n" \
"                            virtual multiplex channels.\n"                          \
"  -n, --num_of_channels[=value]\n"                                                  \
"                            Set or get the number of available multiplex\n"         \
"                            channels for the current driver instance. By default\n" \
"                            there are %d virtual ports available for each driver\n" \
"                            instance. To make this setting valid a reload of the\n" \
"                            corresponding driver instance is required (see\n"       \
"                            \"--reload\"). The number of available multiplex\n"     \
"                            channels depends on the connected hardware.\n"          \
"  -o, --switchoff_on[=flag] Set or get the switch off setting of the current\n"     \
"                            driver instance. The \"linmux\" driver provides the\n"  \
"                            possibility to switch the connected module off after\n" \
"                            all multiplex ports has been closed. The default\n"     \
"                            value is 1 (module will be switched off). The\n"        \
"                            setting is used when the multiplex mode is ended the\n" \
"                            next time.\n"                                           \
"  -p, --base_port[=port]    Set the device name of the base port the hardware is\n" \
"                            connected to. By default the devices \"%s0\",\n"        \
"                            \"%s1\" etc. are used for the driver\n"                 \
"                            instances.\n"                                           \
"  -r, --reload              Reload the current driver instance. A driver reload\n"  \
"                            is necessary to make the settings\n"                    \
"                            \"--num_of_channels\" and \"--device_name\" valid.\n"   \
"  -t, --init_on[=flag]      Set or get the init setting of the current driver\n"    \
"                            instance. If that setting is enabled the \"linmux\"\n"  \
"                            driver tries to switch the module on and do a baud\n"   \
"                            rate synchronization before the multiplex mode is\n"    \
"                            started. If the module is switched on otherwise and\n"  \
"                            it can be ensured that the module is always\n"          \
"                            configured to the baud rate used by the driver that\n"  \
"                            feature can be disabled. The default value is 1\n"      \
"                            (initialization will be executed). The setting is\n"    \
"                            used when the multiplex mode is started the next\n"     \
"                            time.\n"                                                \
"  -v, --max_mux_ver[=value] Set or get the max. used version of the multiplex\n"    \
"                            protocol. The multiplex protocol uses an internal\n"    \
"                            version control. Higher versions of the multiplex\n"    \
"                            protocol are supporting additional features. During\n"  \
"                            startup of the multiplex mode the max. possible\n"      \
"                            version supported by the driver and the module is\n"    \
"                            negotiated. With this option the max. version the\n"    \
"                            driver instance is supporting can be reduced.\n"        \
"                            The setting is used when the multiplex mode is\n"       \
"                            started the next time.\n"                               \
"                            Please note that later Cinterion Wireless Modules do\n" \
"                            not support multiplex protocol versions below 4.\n"     \
"                            Usually module and driver are negotiating the\n"        \
"                            optimal protocol version to be used automatically.\n"   \
"                            So this configuration value should not be used\n"       \
"                            during normal operation and it is intended only for\n"  \
"                            test and development.\n"                                \
"  -w, --hdlc_win_size[=value]\n"                                                    \
"                            Set or get the hdlc window size of the current\n"       \
"                            instance. Multiplex protocol version 4 and above are\n" \
"                            using a hdlc framing mechanism for error correction.\n" \
"                            With this option the size of the hdlc window can be\n"  \
"                            adjusted between 1 and 7. The default value is %d.\n"   \
"                            The setting is used when the multiplex mode is\n"       \
"                            started the next time.\n"                               \
"                            Please note that this parameter defines only the\n"     \
"                            max. window size to be used. The real size is\n"        \
"                            negotiated with the connected module and can be\n"      \
"                            smaller, depending on the modules capabilities.\n"      \
"  -x, --cmd_index=index     Set the zero based command index for the\n"             \
"                            initialization commands (see \"--init_cmd\")\n"         \
"\n"                                                                                 \
"Return values:\n"                                                                   \
"  0 if everything was ok, the corresponding error value if something went wrong.\n" \
"  Please note that the tool tries to handle all given parameters and does not\n"    \
"  abort if handling of a single parameter fails. In case of failure the error\n"    \
"  value of the last error occured is returned.\n"                                   \
"\n"                                                                                 \
"Examples:\n"                                                                        \
"  Set the number of multiplex channels to 4 for the 1st and to 5 for the 2nd\n"     \
"  driver instance. Reload the driver after each setting:\n"                         \
"    linmuxcfg -n=4 -r -i=1 -n=5 -r\n"                                               \
"  Query the 1st and second init command of driver instance 0:\n"                    \
"    linmuxcfg -c - x=1 -c\n",                                                       \
DEFAULT_BAUD_RATE, MAX_INITCMD_NUM, MAX_INTCMD_LEN - 1, DEFAULT_TTYPORT_NAME,        \
DEFAULT_TTYPORT_NAME, DEFAULT_TTYPORT_NAME, MAX_INITCMD_NUM, MAX_INTCMD_LEN - 1,     \
DEFAULT_HDLC_FRAME_SIZE, DEFAULT_HDLC_FRAME_SIZE,                                    \
DEFAULT_NUM_OF_PORTS, DEFAULT_BASEPORT_NAME, DEFAULT_BASEPORT_NAME,                  \
DEFAULT_HDLC_WINDOW_SIZE

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
// Print the help text.
//
// Parameters:
// None.
//
// Return:
// None.
//
//////////////////////////////////////////////////////////////////////////////
void PrintHelp(void) {
  printf(HELP_TEXT);
}

//////////////////////////////////////////////////////////////////////////////
//
// Get the numeric value in the given string. The value can be represented
// in hex format 0x<value> or in simple decimal format <value>.
//
// Parameters:
// String to be parsed.
//
// Return:
// The value found in the given string.
//
//////////////////////////////////////////////////////////////////////////////
int GetVal(const char *str) {
int iRet = 0;

  if (sscanf(str, "0x%x", &iRet) != 1) {
    if (sscanf(str, "0X%x", &iRet) != 1) {
      sscanf(str, "%d", &iRet);
    }
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Set an integer configuration value.
//
// Parameters:
// iFile    : The driver's file handle.
// iInstance: The driver's instance number.
// iVal     : The value to be set.
// uiIoctl  : The configuration ioctl.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int SetInt(int iFile, int iInstance, int iVal, unsigned int uiIoctl) {
  MuxCfgData Data;
  Data.iInstance = iInstance;
  Data.iValue = iVal;
  return ioctl(iFile, uiIoctl, &Data);
}

//////////////////////////////////////////////////////////////////////////////
//
// Get an integer configuration value.
//
// Parameters:
// iFile    : The driver's file handle.
// iInstance: The driver's instance number.
// piVal    : Pointer to the integer receiving the value (output).
// uiIoctl  : The configuration ioctl.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int GetInt(int iFile, int iInstance, int *piVal, unsigned int uiIoctl) {
  int iRet;
  MuxCfgData Data;
  Data.iInstance = iInstance;
  iRet = ioctl(iFile, uiIoctl, &Data);
  if (!iRet) {
    *piVal = Data.iValue;
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Set a string configuration value.
//
// Parameters:
// iFile    : The driver's file handle.
// iInstance: The driver's instance number.
// strBuf   : The value to be set.
// uiIoctl  : The configuration ioctl.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int SetString(int iFile, int iInstance, int iIndex, char* strBuf, unsigned int uiIoctl) {
  MuxCfgData Data;
  Data.iInstance = iInstance;
  Data.iIndex = iIndex;
  if (strlen(strBuf) > MAX_CFG_STR_LEN) {
    return -EINVAL;
  }
  strcpy(Data.strBuf, strBuf);
  return ioctl(iFile, uiIoctl, &Data);
}

//////////////////////////////////////////////////////////////////////////////
//
// Get a string configuration value.
//
// Parameters:
// iFile    : The driver's file handle.
// iInstance: The driver's instance number.
// piVal    : Pointer to the buffer receiving the string (output).
// uiIoctl  : The configuration ioctl.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int GetString(int iFile, int iInstance, int iIndex, char* strBuf, unsigned int uiIoctl) {
  int iRet;
  MuxCfgData Data;
  Data.iInstance = iInstance;
  Data.iIndex = iIndex;
  iRet = ioctl(iFile, uiIoctl, &Data);
  if (!iRet) {
    strcpy(strBuf, Data.strBuf);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Reloead a driver instance.
//
// Parameters:
// iFile    : The driver's file handle.
// iInstance: The driver's instance number.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int ReloadDriver(int iFile, int iInstance) {
  int iRet = SetInt(iFile, iInstance, 0, IOCTL_MUXCFG_RELOAD);
  if (iRet) {
    printf("Error %d: Failed to reload driver instance %d\n", iRet, iInstance);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Set the baud rate used on the parent port.
//
// Parameters:
// iFile    : The driver's file handle.
// iInstance: The driver's instance number.
// iBaudRate: The baud rate to be set.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int SetBaudRate(int iFile, int iInstance, int iBaudRate) {
  int iRet = SetInt(iFile, iInstance, iBaudRate, IOCTL_MUXCFG_SET_BAUDRATE);
  if (iRet) {
    printf("Error %d: Failed to set baud rate %d on driver instance %d\n", iRet, iBaudRate, iInstance);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Get and print the baud rate used on the parent port.
//
// Parameters:
// iFile    : The driver's file handle.
// iInstance: The driver's instance number.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int GetBaudRate(int iFile, int iInstance) {
  int iValue;
  int iRet = GetInt(iFile, iInstance, &iValue, IOCTL_MUXCFG_GET_BAUDRATE);
  if (iRet) {
    printf("Error %d: Failed to query baud rate on driver instance %d\n", iRet, iInstance);
  } else {
    printf("baud_rate[%d]=%d\n", iInstance, iValue);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Set the trace mask.
//
// Parameters:
// iFile     : The driver's file handle.
// iTraceMask: The trace mask to be set.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int SetTraceMask(int iFile, int iTraceMask) {
  int iRet = SetInt(iFile, 0, iTraceMask, IOCTL_MUXCFG_SET_TRACEMASK);
  if (iRet) {
    printf("Error %d: Failed to set the trace mask 0x%08x\n", iRet, iTraceMask);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Get and print the current trace mask.
//
// Parameters:
// iFile: The driver's file handle.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int GetTraceMask(int iFile) {
  int iValue;
  int iRet = GetInt(iFile, 0, &iValue, IOCTL_MUXCFG_GET_TRACEMASK);
  if (iRet) {
    printf("Error %d: Failed to query trace mask\n", iRet);
  } else {
    printf("trace_mask=0x%08x\n", iValue);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Set the device name of the parent port.
//
// Parameters:
// iFile    : The driver's file handle.
// iInstance: The driver's instance number.
// strPort  : The device name of the parent port to be set.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int SetBasePort(int iFile, int iInstance, char* strPort) {
  int iRet = SetString(iFile, iInstance, 0, strPort, IOCTL_MUXCFG_SET_BASEPORT);
  if (iRet) {
    printf("Error %d: Failed to set base port %s on driver instance %d\n", iRet, strPort, iInstance);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Get and print the device name of the parent port.
//
// Parameters:
// iFile    : The driver's file handle.
// iInstance: The driver's instance number.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int GetBasePort(int iFile, int iInstance) {
  char strPort[MAX_CFG_STR_LEN] = "";
  int iRet = GetString(iFile, iInstance, 0, strPort, IOCTL_MUXCFG_GET_BASEPORT);
  if (iRet) {
    printf("Error %d: Failed to query base port on driver instance %d\n", iRet, iInstance);
  } else {
    printf("base_port[%d]=%s\n", iInstance, strPort);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Set the number of multiplex channels to be used.
//
// Parameters:
// iFile    : The driver's file handle.
// iInstance: The driver's instance number.
// iChannels: The number of multiplex channels to be set.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int SetNumberOfChannels(int iFile, int iInstance, int iChannels) {
  int iRet = SetInt(iFile, iInstance, iChannels, IOCTL_MUXCFG_SET_CHNNUM);
  if (iRet) {
    printf("Error %d: Failed to set number of mux channels %d on driver instance %d\n", iRet, iChannels, iInstance);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Get and print the number of multiplex channels used.
//
// Parameters:
// iFile    : The driver's file handle.
// iInstance: The driver's instance number.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int GetNumberOfChannels(int iFile, int iInstance) {
  int iValue;
  int iRet = GetInt(iFile, iInstance, &iValue, IOCTL_MUXCFG_GET_CHNNUM);
  if (iRet) {
    printf("Error %d: Failed to query number of mux channels on driver instance %d\n", iRet, iInstance);
  } else {
    printf("num_of_channels[%d]=%d\n", iInstance, iValue);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Set the device name for the driver instance.
//
// Parameters:
// iFile    : The driver's file handle.
// iInstance: The driver's instance number.
// strDevice: The device name to be set.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int SetDeviceName(int iFile, int iInstance, char* strDevice) {
  int iRet = SetString(iFile, iInstance, 0, strDevice, IOCTL_MUXCFG_SET_DEVNAME);
  if (iRet) {
    printf("Error %d: Failed to set device name %s of driver instance %d\n", iRet, strDevice, iInstance);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Get and print the device name for the driver instance.
//
// Parameters:
// iFile    : The driver's file handle.
// iInstance: The driver's instance number.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int GetDeviceName(int iFile, int iInstance) {
  char strDevice[MAX_CFG_STR_LEN] = "";
  int iRet = GetString(iFile, iInstance, 0, strDevice, IOCTL_MUXCFG_GET_DEVNAME);
  if (iRet) {
    printf("Error %d: Failed to query device name of driver instance %d\n", iRet, iInstance);
  } else {
    printf("device_name[%d]=%s\n", iInstance, strDevice);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Set an additional AT configuration command.
//
// Parameters:
// iFile    : The driver's file handle.
// iInstance: The driver's instance number.
// iIndex   : The index of the configuration command.
// strCmd   : The configuration command to be set.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int SetInitCommand(int iFile, int iInstance, int iIndex, char* strCmd) {
  int iRet = SetString(iFile, iInstance, iIndex, strCmd, IOCTL_MUXCFG_SET_INITCMD);
  if (iRet) {
    printf("Error %d: Failed to set init cmd %s with index %d on driver instance %d\n", iRet, strCmd, iIndex, iInstance);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Get and print an additional AT configuration command.
//
// Parameters:
// iFile    : The driver's file handle.
// iInstance: The driver's instance number.
// iIndex   : The index of the configuration command.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int GetInitCommand(int iFile, int iInstance, int iIndex) {
  char strCmd[MAX_CFG_STR_LEN] = "";
  int iRet = GetString(iFile, iInstance, iIndex, strCmd, IOCTL_MUXCFG_GET_INITCMD);
  if (iRet) {
    printf("Error %d: Failed to query init cmd with index %d on driver instance %d\n", iRet, iIndex, iInstance);
  } else {
    printf("init_cmd[%d][%d]=%s\n", iInstance, iIndex, strCmd);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Set an additional AT deinit command.
//
// Parameters:
// iFile    : The driver's file handle.
// iInstance: The driver's instance number.
// iIndex   : The index of the deinit command.
// strCmd   : The deinit command to be set.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int SetDeinitCommand(int iFile, int iInstance, int iIndex, char* strCmd) {
  int iRet = SetString(iFile, iInstance, iIndex, strCmd, IOCTL_MUXCFG_SET_DEINITCMD);
  if (iRet) {
    printf("Error %d: Failed to set deinit cmd %s with index %d on driver instance %d\n", iRet, strCmd, iIndex, iInstance);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Get and print an additional AT deinit command.
//
// Parameters:
// iFile    : The driver's file handle.
// iInstance: The driver's instance number.
// iIndex   : The index of the deinit command.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int GetDeinitCommand(int iFile, int iInstance, int iIndex) {
  char strCmd[MAX_CFG_STR_LEN] = "";
  int iRet = GetString(iFile, iInstance, iIndex, strCmd, IOCTL_MUXCFG_GET_DEINITCMD);
  if (iRet) {
    printf("Error %d: Failed to query deinit cmd with index %d on driver instance %d\n", iRet, iIndex, iInstance);
  } else {
    printf("deinit_cmd[%d][%d]=%s\n", iInstance, iIndex, strCmd);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Set the maximal version of the mux protocol to be used.
//
// Parameters:
// iFile     : The driver's file handle.
// iInstance : The driver's instance number.
// iMaxMuxVer: The max. mux. protocol version to be used.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int SetMaxMuxVersion(int iFile, int iInstance, int iMaxMuxVer) {
  int iRet = SetInt(iFile, iInstance, iMaxMuxVer, IOCTL_MUXCFG_SET_MAXMUXVER);
  if (iRet) {
    printf("Error %d: Failed to set max. mux version %d on driver instance %d\n", iRet, iMaxMuxVer, iInstance);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Get and print the maximal version of the mux protocol to be used.
//
// Parameters:
// iFile     : The driver's file handle.
// iInstance : The driver's instance number.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int GetMaxMuxVersion(int iFile, int iInstance) {
  int iValue;
  int iRet = GetInt(iFile, iInstance, &iValue, IOCTL_MUXCFG_GET_MAXMUXVER);
  if (iRet) {
    printf("Error %d: Failed to query max. mux version on driver instance %d\n", iRet, iInstance);
  } else {
    printf("max_mux_ver[%d]=%d\n", iInstance, iValue);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Set the maximal hdlc window size to be used.
//
// Parameters:
// iFile     : The driver's file handle.
// iInstance : The driver's instance number.
// iWinSize  : The hdlc window size to be used.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int SetHdlcWindowSize(int iFile, int iInstance, int iWinSize) {
  int iRet = SetInt(iFile, iInstance, iWinSize, IOCTL_MUXCFG_SET_HDLCWINSIZE);
  if (iRet) {
    printf("Error %d: Failed to set hdlc window size %d on driver instance %d\n", iRet, iWinSize, iInstance);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Get and print the maximal hdlc window size to be used.
//
// Parameters:
// iFile     : The driver's file handle.
// iInstance : The driver's instance number.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int GetHdlcWindowSize(int iFile, int iInstance) {
  int iValue;
  int iRet = GetInt(iFile, iInstance, &iValue, IOCTL_MUXCFG_GET_HDLCWINSIZE);
  if (iRet) {
    printf("Error %d: Failed to query hdlc window size on driver instance %d\n", iRet, iInstance);
  } else {
    printf("hdlc_win_size[%d]=%d\n", iInstance, iValue);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Set the maximal hdlc frame size to be used.
//
// Parameters:
// iFile     : The driver's file handle.
// iInstance : The driver's instance number.
// iWinSize  : The hdlc frame size to be used.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int SetHdlcFrameSize(int iFile, int iInstance, int iFrameSize) {
  int iRet = SetInt(iFile, iInstance, iFrameSize, IOCTL_MUXCFG_SET_HDLCFRAMESIZE);
  if (iRet) {
    printf("Error %d: Failed to set hdlc frame size %d on driver instance %d\n", iRet, iFrameSize, iInstance);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Get and print the maximal hdlc frame size to be used.
//
// Parameters:
// iFile     : The driver's file handle.
// iInstance : The driver's instance number.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int GetHdlcFrameSize(int iFile, int iInstance) {
  int iValue;
  int iRet = GetInt(iFile, iInstance, &iValue, IOCTL_MUXCFG_GET_HDLCFRAMESIZE);
  if (iRet) {
    printf("Error %d: Failed to query hdlc frame size on driver instance %d\n", iRet, iInstance);
  } else {
    printf("hdlc_frame_size[%d]=%d\n", iInstance, iValue);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Enable or disable module power on and baud rate synchronization before
// multiplex mode is started.
//
// Parameters:
// iFile     : The driver's file handle.
// iInstance : The driver's instance number.
// iFlag     : Flag indicating if module initialization is enabled (!= 0) or
//             disabled (== 0).
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int SetInitOn(int iFile, int iInstance, int iFlag) {
  int iRet = SetInt(iFile, iInstance, iFlag ? true : false, IOCTL_MUXCFG_SET_INIT);
  if (iRet) {
    printf("Error %d: Failed to switch init %s on driver instance %d\n", iRet, iFlag ? "on" : "off", iInstance);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Get and print the module module initialization setting.
//
// Parameters:
// iFile     : The driver's file handle.
// iInstance : The driver's instance number.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int GetInitOn(int iFile, int iInstance) {
  int iValue;
  int iRet = GetInt(iFile, iInstance, &iValue, IOCTL_MUXCFG_GET_INIT);
  if (iRet) {
    printf("Error %d: Failed to query init value on driver instance %d\n", iRet, iInstance);
  } else {
    printf("init_on[%d]=%d\n", iInstance, iValue);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Enable or disable module power of after multiplex mode is closed down.
//
// Parameters:
// iFile     : The driver's file handle.
// iInstance : The driver's instance number.
// iFlag     : Flag indicating if module power off is enabled (!= 0) or
//             disabled (== 0).
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int SetSwitchoffOn(int iFile, int iInstance, int iFlag) {
  int iRet = SetInt(iFile, iInstance, iFlag ? true : false, IOCTL_MUXCFG_SET_SWITCHOFF);
  if (iRet) {
    printf("Error %d: Failed to switch switchoff %s on driver instance %d\n", iRet, iFlag ? "on" : "off", iInstance);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Get and print the module power of setting.
//
// Parameters:
// iFile     : The driver's file handle.
// iInstance : The driver's instance number.
//
// Return:
// The return value of the driver ioctl call.
//
//////////////////////////////////////////////////////////////////////////////
int GetSwitchoffOn(int iFile, int iInstance) {
  int iValue;
  int iRet = GetInt(iFile, iInstance, &iValue, IOCTL_MUXCFG_GET_SWITCHOFF);
  if (iRet) {
    printf("Error %d: Failed to query switchoff value on driver instance %d\n", iRet, iInstance);
  } else {
    printf("switchoff_on[%d]=%d\n", iInstance, iValue);
  }
  return iRet;
}

//////////////////////////////////////////////////////////////////////////////
//
// Check the given command line arguments for errors or help requests before
// they are actually executed. In case of an error or help request the
// program is terminated.
//
// Parameters:
// argc: The number of command line arguments.
// argv: Array with the command line arguments.
//
// Return:
// None.
//
//////////////////////////////////////////////////////////////////////////////
void PreCheckCommandLine(int argc, char **argv) {
  int iOpts = 0;
  int c;

  // No arguemnts given
  if (argc <= 1) {
    PrintHelp();
    exit(0);
  }

  // Check for help or invalid arguments
  iOpts = 0;
  while ((c = getopt_long(argc, argv, strOpts, Opts, &iOpts)) != -1) {
    switch (c) {
      case 'h':
        PrintHelp();
        exit(0);
      case '?':
        exit(-EINVAL);
    }
  }

  // Invalid arguments at end of list
  if (optind != argc) {
    printf ("Invalid argument(s) given, use \"linmuxcfg -h\" to display help\n");
    exit(-EINVAL);
  }

  // reset getopt() counter
  optind = 0;
}

//////////////////////////////////////////////////////////////////////////////
//
// The entry point function of the linmux configuration utility.
//
// Parameters:
// argc: The number of command line arguments.
// argv: Array with the command line arguments.
//
// Return:
// 0 in case of success, a negative error value in case of an error.
//
//////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
  int    iInstance = 0;
  int    iIndex = 0;
  int    iOpts = 0;
  int    file_desc;
  int    c;
  int    iRet;
  int    iRetLocal;
  char  *pArg;

  PreCheckCommandLine(argc, argv);

  file_desc = open(DEVICE_NAME, 0);
  if (file_desc < 0) {
    printf("Can't open device file: %s, check if linmux module is properly installed\n", DEVICE_NAME);
    exit(file_desc);
  }

  while ((c = getopt_long(argc, argv, strOpts, Opts, &iOpts)) != -1) {
    pArg = optarg;
    if (pArg && (pArg[0] == '=')) {
      pArg++;
    }
    switch (c) {
      case 'r':
        iRetLocal = ReloadDriver(file_desc, iInstance);
        break;
      case 'i':
        iInstance = GetVal(pArg);
        break;
      case 'x':
        iIndex = GetVal(pArg);
        break;
      case 'p':
        if (pArg) {
          iRetLocal = SetBasePort(file_desc, iInstance, pArg);
        } else {
          iRetLocal = GetBasePort(file_desc, iInstance);
        }
        break;
      case 'd':
        if (pArg) {
          iRetLocal = SetDeviceName(file_desc, iInstance, pArg);
        } else {
          iRetLocal = GetDeviceName(file_desc, iInstance);
        }
        break;
      case 'b':
        if (pArg) {
          iRetLocal = SetBaudRate(file_desc, iInstance, GetVal(pArg));
        } else {
          iRetLocal = GetBaudRate(file_desc, iInstance);
        }
        break;
      case 'm':
        if (pArg) {
          iRetLocal = SetTraceMask(file_desc, GetVal(pArg));
        } else {
          iRetLocal = GetTraceMask(file_desc);
        }
        break;
      case 'n':
        if (pArg) {
          iRetLocal = SetNumberOfChannels(file_desc, iInstance, GetVal(pArg));
        } else {
          iRetLocal = GetNumberOfChannels(file_desc, iInstance);
        }
        break;
      case 'c':
        if (pArg) {
          iRetLocal = SetInitCommand(file_desc, iInstance, iIndex, pArg);
        } else {
          iRetLocal = GetInitCommand(file_desc, iInstance, iIndex);
        }
        break;
      case 'e':
        if (pArg) {
          iRetLocal = SetDeinitCommand(file_desc, iInstance, iIndex, pArg);
        } else {
          iRetLocal = GetDeinitCommand(file_desc, iInstance, iIndex);
        }
        break;
      case 't':
        if (pArg) {
          iRetLocal = SetInitOn(file_desc, iInstance, GetVal(pArg));
        } else {
          iRetLocal = GetInitOn(file_desc, iInstance);
        }
        break;
      case 'o':
        if (pArg) {
          iRetLocal = SetSwitchoffOn(file_desc, iInstance, GetVal(pArg));
        } else {
          iRetLocal = GetSwitchoffOn(file_desc, iInstance);
        }
        break;
      case 'v':
        if (pArg) {
          iRetLocal = SetMaxMuxVersion(file_desc, iInstance, GetVal(pArg));
        } else {
          iRetLocal = GetMaxMuxVersion(file_desc, iInstance);
        }
        break;
      case 'w':
        if (pArg) {
          iRetLocal = SetHdlcWindowSize(file_desc, iInstance, GetVal(pArg));
        } else {
          iRetLocal = GetHdlcWindowSize(file_desc, iInstance);
        }
        break;
      case 'f':
        if (pArg) {
          iRetLocal = SetHdlcFrameSize(file_desc, iInstance, GetVal(pArg));
        } else {
          iRetLocal = GetHdlcFrameSize(file_desc, iInstance);
        }
        break;
    }
    if (iRetLocal) {
      iRet = iRetLocal;
    }
  }

  close(file_desc);

  return iRet;
}

