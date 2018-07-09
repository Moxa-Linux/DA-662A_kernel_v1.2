# Microsoft Developer Studio Project File - Name="snort" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=snort - Win32 StdDB FlexResp Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "snort.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "snort.mak" CFG="snort - Win32 StdDB FlexResp Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "snort - Win32 StdDB Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "snort - Win32 StdDB Release" (based on "Win32 (x86) Console Application")
!MESSAGE "snort - Win32 StdDB FlexResp Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "snort - Win32 StdDB FlexResp Release" (based on "Win32 (x86) Console Application")
!MESSAGE "snort - Win32 StdDB FlexResp Service Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "snort - Win32 StdDB FlexResp Service Release" (based on "Win32 (x86) Console Application")
!MESSAGE "snort - Win32 StdDB MSSQL Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "snort - Win32 StdDB MSSQL Release" (based on "Win32 (x86) Console Application")
!MESSAGE "snort - Win32 StdDB MSSQL Service Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "snort - Win32 StdDB MSSQL Service Release" (based on "Win32 (x86) Console Application")
!MESSAGE "snort - Win32 StdDB MSSQL FlexResp Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "snort - Win32 StdDB MSSQL FlexResp Release" (based on "Win32 (x86) Console Application")
!MESSAGE "snort - Win32 StdDB MSSQL FlexResp Service Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "snort - Win32 StdDB MSSQL FlexResp Service Release" (based on "Win32 (x86) Console Application")
!MESSAGE "snort - Win32 StdDB Service Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "snort - Win32 StdDB Service Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "snort___Win32_StdDB_Debug"
# PROP BASE Intermediate_Dir "snort___Win32_StdDB_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "snort___Win32_StdDB_Debug"
# PROP Intermediate_Dir "snort___Win32_StdDB_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql"
# ADD LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql"

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "snort___Win32_StdDB_Release"
# PROP BASE Intermediate_Dir "snort___Win32_StdDB_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "snort___Win32_StdDB_Release"
# PROP Intermediate_Dir "snort___Win32_StdDB_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql"
# ADD LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql"

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "snort___Win32_StdDB_FlexResp_Debug"
# PROP BASE Intermediate_Dir "snort___Win32_StdDB_FlexResp_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "snort___Win32_StdDB_FlexResp_Debug"
# PROP Intermediate_Dir "snort___Win32_StdDB_FlexResp_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet"
# ADD LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet"

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "snort___Win32_StdDB_FlexResp_Release"
# PROP BASE Intermediate_Dir "snort___Win32_StdDB_FlexResp_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "snort___Win32_StdDB_FlexResp_Release"
# PROP Intermediate_Dir "snort___Win32_StdDB_FlexResp_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet"
# ADD LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /incremental:no /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "snort___Win32_StdDB_FlexResp_Service_Debug"
# PROP BASE Intermediate_Dir "snort___Win32_StdDB_FlexResp_Service_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "snort___Win32_StdDB_FlexResp_Service_Debug"
# PROP Intermediate_Dir "snort___Win32_StdDB_FlexResp_Service_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /D "ENABLE_SERVICE" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /D "ENABLE_WIN32_SERVICE" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet"
# ADD LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet"

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "snort___Win32_StdDB_FlexResp_Service_Release"
# PROP BASE Intermediate_Dir "snort___Win32_StdDB_FlexResp_Service_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "snort___Win32_StdDB_FlexResp_Service_Release"
# PROP Intermediate_Dir "snort___Win32_StdDB_FlexResp_Service_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /D "ENABLE_SERVICE" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /D "ENABLE_WIN32_SERVICE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet"
# ADD LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet"

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "snort___Win32_StdDB_MSSQL_Debug"
# PROP BASE Intermediate_Dir "snort___Win32_StdDB_MSSQL_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "snort___Win32_StdDB_MSSQL_Debug"
# PROP Intermediate_Dir "snort___Win32_StdDB_MSSQL_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /FR /YX"snort.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /FR /YX"snort.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql"
# ADD LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql"

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "snort___Win32_StdDB_MSSQL_Release"
# PROP BASE Intermediate_Dir "snort___Win32_StdDB_MSSQL_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "snort___Win32_StdDB_MSSQL_Release"
# PROP Intermediate_Dir "snort___Win32_StdDB_MSSQL_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /FR /YX"snort.pch" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /YX"snort.pch" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql"
# ADD LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql"

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "snort___Win32_StdDB_MSSQL_Service_Debug"
# PROP BASE Intermediate_Dir "snort___Win32_StdDB_MSSQL_Service_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "snort___Win32_StdDB_MSSQL_Service_Debug"
# PROP Intermediate_Dir "snort___Win32_StdDB_MSSQL_Service_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_SERVICE" /FR /YX"snort.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_WIN32_SERVICE" /FR /YX"snort.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql"
# ADD LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql"

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "snort___Win32_StdDB_MSSQL_Service_Release"
# PROP BASE Intermediate_Dir "snort___Win32_StdDB_MSSQL_Service_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "snort___Win32_StdDB_MSSQL_Service_Release"
# PROP Intermediate_Dir "snort___Win32_StdDB_MSSQL_Service_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_SERVICE" /FR /YX"snort.pch" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_WIN32_SERVICE" /YX"snort.pch" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql"
# ADD LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql"

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "snort___Win32_StdDB_MSSQL_FlexResp_Debug"
# PROP BASE Intermediate_Dir "snort___Win32_StdDB_MSSQL_FlexResp_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "snort___Win32_StdDB_MSSQL_FlexResp_Debug"
# PROP Intermediate_Dir "snort___Win32_StdDB_MSSQL_FlexResp_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /FR /YX"snort.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /FR /YX"snort.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet"
# ADD LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet"

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "snort___Win32_StdDB_MSSQL_FlexResp_Release"
# PROP BASE Intermediate_Dir "snort___Win32_StdDB_MSSQL_FlexResp_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "snort___Win32_StdDB_MSSQL_FlexResp_Release"
# PROP Intermediate_Dir "snort___Win32_StdDB_MSSQL_FlexResp_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /FR /YX"snort.pch" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /YX"snort.pch" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet"
# ADD LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet"

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "snort___Win32_StdDB_MSSQL_FlexResp_Service_Debug"
# PROP BASE Intermediate_Dir "snort___Win32_StdDB_MSSQL_FlexResp_Service_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "snort___Win32_StdDB_MSSQL_FlexResp_Service_Debug"
# PROP Intermediate_Dir "snort___Win32_StdDB_MSSQL_FlexResp_Service_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /D "ENABLE_SERVICE" /FR /YX"snort.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /D "ENABLE_WIN32_SERVICE" /Fr /YX"snort.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet"
# ADD LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet"

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "snort___Win32_StdDB_MSSQL_FlexResp_Service_Release"
# PROP BASE Intermediate_Dir "snort___Win32_StdDB_MSSQL_FlexResp_Service_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "snort___Win32_StdDB_MSSQL_FlexResp_Service_Release"
# PROP Intermediate_Dir "snort___Win32_StdDB_MSSQL_FlexResp_Service_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /D "ENABLE_SERVICE" /FR /YX"snort.pch" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /D "ENABLE_WIN32_SERVICE" /YX"snort.pch" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet"
# ADD LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet"

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "snort___Win32_StdDB_Service_Debug"
# PROP BASE Intermediate_Dir "snort___Win32_StdDB_Service_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "snort___Win32_StdDB_Service_Debug"
# PROP Intermediate_Dir "snort___Win32_StdDB_Service_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_SERVICE" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_WIN32_SERVICE" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql"
# ADD LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql"

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "snort___Win32_StdDB_Service_Release"
# PROP BASE Intermediate_Dir "snort___Win32_StdDB_Service_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "snort___Win32_StdDB_Service_Release"
# PROP Intermediate_Dir "snort___Win32_StdDB_Service_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_SERVICE" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_WIN32_SERVICE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql"
# ADD LINK32 user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql"

!ENDIF 

# Begin Target

# Name "snort - Win32 StdDB Debug"
# Name "snort - Win32 StdDB Release"
# Name "snort - Win32 StdDB FlexResp Debug"
# Name "snort - Win32 StdDB FlexResp Release"
# Name "snort - Win32 StdDB FlexResp Service Debug"
# Name "snort - Win32 StdDB FlexResp Service Release"
# Name "snort - Win32 StdDB MSSQL Debug"
# Name "snort - Win32 StdDB MSSQL Release"
# Name "snort - Win32 StdDB MSSQL Service Debug"
# Name "snort - Win32 StdDB MSSQL Service Release"
# Name "snort - Win32 StdDB MSSQL FlexResp Debug"
# Name "snort - Win32 StdDB MSSQL FlexResp Release"
# Name "snort - Win32 StdDB MSSQL FlexResp Service Debug"
# Name "snort - Win32 StdDB MSSQL FlexResp Service Release"
# Name "snort - Win32 StdDB Service Debug"
# Name "snort - Win32 StdDB Service Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\acsmx.c
# End Source File
# Begin Source File

SOURCE=..\..\byte_extract.c
# End Source File
# Begin Source File

SOURCE=..\..\codes.c
# End Source File
# Begin Source File

SOURCE=..\..\debug.c
# End Source File
# Begin Source File

SOURCE=..\..\decode.c
# End Source File
# Begin Source File

SOURCE=..\..\detect.c
# End Source File
# Begin Source File

SOURCE=..\..\fpcreate.c
# End Source File
# Begin Source File

SOURCE=..\..\fpdetect.c
# End Source File
# Begin Source File

SOURCE=..\..\log.c
# End Source File
# Begin Source File

SOURCE=..\..\mempool.c
# End Source File
# Begin Source File

SOURCE=..\..\mpse.c
# End Source File
# Begin Source File

SOURCE=..\..\mstring.c
# End Source File
# Begin Source File

SOURCE=..\..\mwm.c
# End Source File
# Begin Source File

SOURCE=..\..\parser.c
# End Source File
# Begin Source File

SOURCE=..\..\pcrm.c
# End Source File
# Begin Source File

SOURCE=..\..\plugbase.c
# End Source File
# Begin Source File

SOURCE=..\..\sf_sdlist.c
# End Source File
# Begin Source File

SOURCE=..\..\sfksearch.c
# End Source File
# Begin Source File

SOURCE=..\..\signature.c
# End Source File
# Begin Source File

SOURCE=..\..\snort.c
# End Source File
# Begin Source File

SOURCE=..\..\snprintf.c
# End Source File
# Begin Source File

SOURCE=..\..\strlcatu.c
# End Source File
# Begin Source File

SOURCE=..\..\strlcpyu.c
# End Source File
# Begin Source File

SOURCE=..\..\tag.c
# End Source File
# Begin Source File

SOURCE=..\..\threshold.c
# End Source File
# Begin Source File

SOURCE=..\..\ubi_BinTree.c
# End Source File
# Begin Source File

SOURCE=..\..\ubi_SplayTree.c
# End Source File
# Begin Source File

SOURCE=..\..\util.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\acsmx.h
# End Source File
# Begin Source File

SOURCE=..\..\bitop.h
# End Source File
# Begin Source File

SOURCE=..\..\bounds.h
# End Source File
# Begin Source File

SOURCE=..\..\byte_extract.h
# End Source File
# Begin Source File

SOURCE=..\..\cdefs.h
# End Source File
# Begin Source File

SOURCE=..\..\checksum.h
# End Source File
# Begin Source File

SOURCE=..\..\codes.h
# End Source File
# Begin Source File

SOURCE=..\..\debug.h
# End Source File
# Begin Source File

SOURCE=..\..\decode.h
# End Source File
# Begin Source File

SOURCE=..\..\detect.h
# End Source File
# Begin Source File

SOURCE=..\..\event.h
# End Source File
# Begin Source File

SOURCE=..\..\fatal.h
# End Source File
# Begin Source File

SOURCE=..\..\fpcreate.h
# End Source File
# Begin Source File

SOURCE=..\..\fpdetect.h
# End Source File
# Begin Source File

SOURCE=..\..\generators.h
# End Source File
# Begin Source File

SOURCE=..\..\log.h
# End Source File
# Begin Source File

SOURCE=..\..\mempool.h
# End Source File
# Begin Source File

SOURCE=..\..\mpse.h
# End Source File
# Begin Source File

SOURCE=..\..\mstring.h
# End Source File
# Begin Source File

SOURCE=..\..\mwm.h
# End Source File
# Begin Source File

SOURCE=..\..\parser.h
# End Source File
# Begin Source File

SOURCE=..\..\pcrm.h
# End Source File
# Begin Source File

SOURCE=..\..\plugbase.h
# End Source File
# Begin Source File

SOURCE=..\..\plugin_enum.h
# End Source File
# Begin Source File

SOURCE=..\..\prototypes.h
# End Source File
# Begin Source File

SOURCE=..\..\rules.h
# End Source File
# Begin Source File

SOURCE=..\..\sf_sdlist.h
# End Source File
# Begin Source File

SOURCE=..\..\sfksearch.h
# End Source File
# Begin Source File

SOURCE=..\..\signature.h
# End Source File
# Begin Source File

SOURCE=..\..\smalloc.h
# End Source File
# Begin Source File

SOURCE=..\..\snort.h
# End Source File
# Begin Source File

SOURCE=..\..\snprintf.h
# End Source File
# Begin Source File

SOURCE=..\..\spo_plugbase.h
# End Source File
# Begin Source File

SOURCE=..\..\strlcatu.h
# End Source File
# Begin Source File

SOURCE=..\..\strlcpyu.h
# End Source File
# Begin Source File

SOURCE=..\..\sys_include.h
# End Source File
# Begin Source File

SOURCE=..\..\tag.h
# End Source File
# Begin Source File

SOURCE=..\..\threshold.h
# End Source File
# Begin Source File

SOURCE=..\..\timersub.h
# End Source File
# Begin Source File

SOURCE=..\..\ubi_BinTree.h
# End Source File
# Begin Source File

SOURCE=..\..\ubi_SplayTree.h
# End Source File
# Begin Source File

SOURCE=..\..\util.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE="..\WIN32-Code\MSG00001.bin"
# End Source File
# Begin Source File

SOURCE="..\WIN32-Code\name.rc"
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Win32 Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\WIN32-Code\getopt.c"
# End Source File
# Begin Source File

SOURCE="..\WIN32-Code\misc.c"
# End Source File
# Begin Source File

SOURCE="..\WIN32-Code\syslog.c"
# End Source File
# Begin Source File

SOURCE="..\WIN32-Code\win32_service.c"
# End Source File
# End Group
# Begin Group "Win32 Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\WIN32-Includes\rpc\auth.h"
# End Source File
# Begin Source File

SOURCE="..\WIN32-Includes\NET\Bpf.h"
# End Source File
# Begin Source File

SOURCE="..\WIN32-Includes\rpc\clnt.h"
# End Source File
# Begin Source File

SOURCE="..\WIN32-Includes\config.h"
# End Source File
# Begin Source File

SOURCE="..\WIN32-Includes\getopt.h"
# End Source File
# Begin Source File

SOURCE="..\WIN32-Includes\gnuc.h"
# End Source File
# Begin Source File

SOURCE="..\WIN32-Code\name.h"
# End Source File
# Begin Source File

SOURCE="..\WIN32-Includes\pcap-namedb.h"
# End Source File
# Begin Source File

SOURCE="..\WIN32-Includes\pcap.h"
# End Source File
# Begin Source File

SOURCE="..\WIN32-Includes\rpc\rpc_msg.h"
# End Source File
# Begin Source File

SOURCE="..\WIN32-Includes\syslog.h"
# End Source File
# Begin Source File

SOURCE="..\WIN32-Includes\UNISTD.H"
# End Source File
# Begin Source File

SOURCE="..\WIN32-Includes\rpc\xdr.h"
# End Source File
# End Group
# Begin Group "Detection Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\detection-plugins\sp_byte_check.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_byte_jump.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_clientserver.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_dsize_check.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_icmp_code_check.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_icmp_id_check.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_icmp_seq_check.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_icmp_type_check.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_ip_fragbits.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_ip_id_check.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_ip_proto.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_ip_same_check.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_ip_tos_check.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_ipoption_check.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_pattern_match.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_react.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_respond.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_rpc_check.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_session.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_tcp_ack_check.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_tcp_flag_check.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_tcp_seq_check.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_tcp_win_check.c"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_ttl_check.c"
# End Source File
# End Group
# Begin Group "Detection Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\detection-plugins\sp_byte_check.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_byte_jump.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_clientserver.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_dsize_check.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_icmp_code_check.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_icmp_id_check.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_icmp_seq_check.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_icmp_type_check.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_ip_fragbits.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_ip_id_check.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_ip_proto.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_ip_same_check.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_ip_tos_check.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_ipoption_check.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_pattern_match.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_react.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_respond.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_rpc_check.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_session.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_tcp_ack_check.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_tcp_flag_check.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_tcp_seq_check.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_tcp_win_check.h"
# End Source File
# Begin Source File

SOURCE="..\..\detection-plugins\sp_ttl_check.h"
# End Source File
# End Group
# Begin Group "Output Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\output-plugins\spo_alert_fast.c"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_alert_full.c"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_alert_sf_socket.c"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_alert_smb.c"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_alert_syslog.c"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_alert_unixsock.c"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_csv.c"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_database.c"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_log_ascii.c"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_log_null.c"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_log_tcpdump.c"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_unified.c"
# End Source File
# End Group
# Begin Group "Output Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\output-plugins\spo_alert_fast.h"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_alert_full.h"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_alert_sf_socket.h"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_alert_smb.h"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_alert_syslog.h"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_alert_unixsock.h"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_csv.h"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_database.h"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_log_ascii.h"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_log_null.h"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_log_tcpdump.h"
# End Source File
# Begin Source File

SOURCE="..\..\output-plugins\spo_unified.h"
# End Source File
# End Group
# Begin Group "Preprocessor Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\preprocessors\http-resp.c"
# End Source File
# Begin Source File

SOURCE="..\..\preprocessors\perf-base.c"
# End Source File
# Begin Source File

SOURCE="..\..\preprocessors\perf-event.c"
# End Source File
# Begin Source File

SOURCE="..\..\preprocessors\perf-flow.c"
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\perf.c
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\sfprocpidstats.c
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_arpspoof.c
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_bo.c
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_conversation.c
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_frag2.c
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_http_decode.c
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_httpflow.c
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_perfmonitor.c
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_portscan.c
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_portscan2.c
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_rpc_decode.c
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_stream4.c
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_telnet_negotiation.c
# End Source File
# End Group
# Begin Group "Preprocessor Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\preprocessors\http-resp.h"
# End Source File
# Begin Source File

SOURCE="..\..\preprocessors\perf-base.h"
# End Source File
# Begin Source File

SOURCE="..\..\preprocessors\perf-event.h"
# End Source File
# Begin Source File

SOURCE="..\..\preprocessors\perf-flow.h"
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\perf.h
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\sfprocpidstats.h
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_arpspoof.h
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_bo.h
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_conversation.h
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_frag2.h
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_http_decode.h
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_httpflow.h
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_perfmonitor.h
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_portscan.h
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_portscan2.h
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_rpc_decode.h
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_stream4.h
# End Source File
# Begin Source File

SOURCE=..\..\preprocessors\spp_telnet_negotiation.h
# End Source File
# End Group
# Begin Group "Parser Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\parser\IpAddrSet.c
# End Source File
# End Group
# Begin Group "Parser Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\parser\IpAddrSet.h
# End Source File
# End Group
# End Target
# End Project
