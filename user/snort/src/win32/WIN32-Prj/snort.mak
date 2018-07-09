# Microsoft Developer Studio Generated NMAKE File, Based on snort.dsp
!IF "$(CFG)" == ""
CFG=snort - Win32 StdDB FlexResp Release
!MESSAGE No configuration specified. Defaulting to snort - Win32 StdDB FlexResp Release.
!ENDIF 

!IF "$(CFG)" != "snort - Win32 StdDB Debug" && "$(CFG)" != "snort - Win32 StdDB Release" && "$(CFG)" != "snort - Win32 StdDB FlexResp Debug" && "$(CFG)" != "snort - Win32 StdDB FlexResp Release" && "$(CFG)" != "snort - Win32 StdDB FlexResp Service Debug" && "$(CFG)" != "snort - Win32 StdDB FlexResp Service Release" && "$(CFG)" != "snort - Win32 StdDB MSSQL Debug" && "$(CFG)" != "snort - Win32 StdDB MSSQL Release" && "$(CFG)" != "snort - Win32 StdDB MSSQL Service Debug" && "$(CFG)" != "snort - Win32 StdDB MSSQL Service Release" && "$(CFG)" != "snort - Win32 StdDB MSSQL FlexResp Debug" && "$(CFG)" != "snort - Win32 StdDB MSSQL FlexResp Release" && "$(CFG)" != "snort - Win32 StdDB MSSQL FlexResp Service Debug" && "$(CFG)" != "snort - Win32 StdDB MSSQL FlexResp Service Release" && "$(CFG)" != "snort - Win32 StdDB Service Debug" && "$(CFG)" != "snort - Win32 StdDB Service Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"

OUTDIR=.\snort___Win32_StdDB_Debug
INTDIR=.\snort___Win32_StdDB_Debug
# Begin Custom Macros
OutDir=.\snort___Win32_StdDB_Debug
# End Custom Macros

ALL : "$(OUTDIR)\snort.exe" "$(OUTDIR)\snort.bsc"


CLEAN :
	-@erase "$(INTDIR)\acsmx.obj"
	-@erase "$(INTDIR)\acsmx.sbr"
	-@erase "$(INTDIR)\byte_extract.obj"
	-@erase "$(INTDIR)\byte_extract.sbr"
	-@erase "$(INTDIR)\codes.obj"
	-@erase "$(INTDIR)\codes.sbr"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\debug.sbr"
	-@erase "$(INTDIR)\decode.obj"
	-@erase "$(INTDIR)\decode.sbr"
	-@erase "$(INTDIR)\detect.obj"
	-@erase "$(INTDIR)\detect.sbr"
	-@erase "$(INTDIR)\fpcreate.obj"
	-@erase "$(INTDIR)\fpcreate.sbr"
	-@erase "$(INTDIR)\fpdetect.obj"
	-@erase "$(INTDIR)\fpdetect.sbr"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\getopt.sbr"
	-@erase "$(INTDIR)\http-resp.obj"
	-@erase "$(INTDIR)\http-resp.sbr"
	-@erase "$(INTDIR)\IpAddrSet.obj"
	-@erase "$(INTDIR)\IpAddrSet.sbr"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\log.sbr"
	-@erase "$(INTDIR)\mempool.obj"
	-@erase "$(INTDIR)\mempool.sbr"
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\misc.sbr"
	-@erase "$(INTDIR)\mpse.obj"
	-@erase "$(INTDIR)\mpse.sbr"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\mstring.sbr"
	-@erase "$(INTDIR)\mwm.obj"
	-@erase "$(INTDIR)\mwm.sbr"
	-@erase "$(INTDIR)\parser.obj"
	-@erase "$(INTDIR)\parser.sbr"
	-@erase "$(INTDIR)\pcrm.obj"
	-@erase "$(INTDIR)\pcrm.sbr"
	-@erase "$(INTDIR)\perf-base.obj"
	-@erase "$(INTDIR)\perf-base.sbr"
	-@erase "$(INTDIR)\perf-event.obj"
	-@erase "$(INTDIR)\perf-event.sbr"
	-@erase "$(INTDIR)\perf-flow.obj"
	-@erase "$(INTDIR)\perf-flow.sbr"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\perf.sbr"
	-@erase "$(INTDIR)\plugbase.obj"
	-@erase "$(INTDIR)\plugbase.sbr"
	-@erase "$(INTDIR)\sf_sdlist.obj"
	-@erase "$(INTDIR)\sf_sdlist.sbr"
	-@erase "$(INTDIR)\sfksearch.obj"
	-@erase "$(INTDIR)\sfksearch.sbr"
	-@erase "$(INTDIR)\sfprocpidstats.obj"
	-@erase "$(INTDIR)\sfprocpidstats.sbr"
	-@erase "$(INTDIR)\signature.obj"
	-@erase "$(INTDIR)\signature.sbr"
	-@erase "$(INTDIR)\snort.obj"
	-@erase "$(INTDIR)\snort.sbr"
	-@erase "$(INTDIR)\snprintf.obj"
	-@erase "$(INTDIR)\snprintf.sbr"
	-@erase "$(INTDIR)\sp_byte_check.obj"
	-@erase "$(INTDIR)\sp_byte_check.sbr"
	-@erase "$(INTDIR)\sp_byte_jump.obj"
	-@erase "$(INTDIR)\sp_byte_jump.sbr"
	-@erase "$(INTDIR)\sp_clientserver.obj"
	-@erase "$(INTDIR)\sp_clientserver.sbr"
	-@erase "$(INTDIR)\sp_dsize_check.obj"
	-@erase "$(INTDIR)\sp_dsize_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_code_check.obj"
	-@erase "$(INTDIR)\sp_icmp_code_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_id_check.obj"
	-@erase "$(INTDIR)\sp_icmp_id_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_seq_check.obj"
	-@erase "$(INTDIR)\sp_icmp_seq_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_type_check.obj"
	-@erase "$(INTDIR)\sp_icmp_type_check.sbr"
	-@erase "$(INTDIR)\sp_ip_fragbits.obj"
	-@erase "$(INTDIR)\sp_ip_fragbits.sbr"
	-@erase "$(INTDIR)\sp_ip_id_check.obj"
	-@erase "$(INTDIR)\sp_ip_id_check.sbr"
	-@erase "$(INTDIR)\sp_ip_proto.obj"
	-@erase "$(INTDIR)\sp_ip_proto.sbr"
	-@erase "$(INTDIR)\sp_ip_same_check.obj"
	-@erase "$(INTDIR)\sp_ip_same_check.sbr"
	-@erase "$(INTDIR)\sp_ip_tos_check.obj"
	-@erase "$(INTDIR)\sp_ip_tos_check.sbr"
	-@erase "$(INTDIR)\sp_ipoption_check.obj"
	-@erase "$(INTDIR)\sp_ipoption_check.sbr"
	-@erase "$(INTDIR)\sp_pattern_match.obj"
	-@erase "$(INTDIR)\sp_pattern_match.sbr"
	-@erase "$(INTDIR)\sp_react.obj"
	-@erase "$(INTDIR)\sp_react.sbr"
	-@erase "$(INTDIR)\sp_respond.obj"
	-@erase "$(INTDIR)\sp_respond.sbr"
	-@erase "$(INTDIR)\sp_rpc_check.obj"
	-@erase "$(INTDIR)\sp_rpc_check.sbr"
	-@erase "$(INTDIR)\sp_session.obj"
	-@erase "$(INTDIR)\sp_session.sbr"
	-@erase "$(INTDIR)\sp_tcp_ack_check.obj"
	-@erase "$(INTDIR)\sp_tcp_ack_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_flag_check.obj"
	-@erase "$(INTDIR)\sp_tcp_flag_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_seq_check.obj"
	-@erase "$(INTDIR)\sp_tcp_seq_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_win_check.obj"
	-@erase "$(INTDIR)\sp_tcp_win_check.sbr"
	-@erase "$(INTDIR)\sp_ttl_check.obj"
	-@erase "$(INTDIR)\sp_ttl_check.sbr"
	-@erase "$(INTDIR)\spo_alert_fast.obj"
	-@erase "$(INTDIR)\spo_alert_fast.sbr"
	-@erase "$(INTDIR)\spo_alert_full.obj"
	-@erase "$(INTDIR)\spo_alert_full.sbr"
	-@erase "$(INTDIR)\spo_alert_sf_socket.obj"
	-@erase "$(INTDIR)\spo_alert_sf_socket.sbr"
	-@erase "$(INTDIR)\spo_alert_smb.obj"
	-@erase "$(INTDIR)\spo_alert_smb.sbr"
	-@erase "$(INTDIR)\spo_alert_syslog.obj"
	-@erase "$(INTDIR)\spo_alert_syslog.sbr"
	-@erase "$(INTDIR)\spo_alert_unixsock.obj"
	-@erase "$(INTDIR)\spo_alert_unixsock.sbr"
	-@erase "$(INTDIR)\spo_csv.obj"
	-@erase "$(INTDIR)\spo_csv.sbr"
	-@erase "$(INTDIR)\spo_database.obj"
	-@erase "$(INTDIR)\spo_database.sbr"
	-@erase "$(INTDIR)\spo_log_ascii.obj"
	-@erase "$(INTDIR)\spo_log_ascii.sbr"
	-@erase "$(INTDIR)\spo_log_null.obj"
	-@erase "$(INTDIR)\spo_log_null.sbr"
	-@erase "$(INTDIR)\spo_log_tcpdump.obj"
	-@erase "$(INTDIR)\spo_log_tcpdump.sbr"
	-@erase "$(INTDIR)\spo_unified.obj"
	-@erase "$(INTDIR)\spo_unified.sbr"
	-@erase "$(INTDIR)\spp_arpspoof.obj"
	-@erase "$(INTDIR)\spp_arpspoof.sbr"
	-@erase "$(INTDIR)\spp_bo.obj"
	-@erase "$(INTDIR)\spp_bo.sbr"
	-@erase "$(INTDIR)\spp_conversation.obj"
	-@erase "$(INTDIR)\spp_conversation.sbr"
	-@erase "$(INTDIR)\spp_frag2.obj"
	-@erase "$(INTDIR)\spp_frag2.sbr"
	-@erase "$(INTDIR)\spp_http_decode.obj"
	-@erase "$(INTDIR)\spp_http_decode.sbr"
	-@erase "$(INTDIR)\spp_httpflow.obj"
	-@erase "$(INTDIR)\spp_httpflow.sbr"
	-@erase "$(INTDIR)\spp_perfmonitor.obj"
	-@erase "$(INTDIR)\spp_perfmonitor.sbr"
	-@erase "$(INTDIR)\spp_portscan.obj"
	-@erase "$(INTDIR)\spp_portscan.sbr"
	-@erase "$(INTDIR)\spp_portscan2.obj"
	-@erase "$(INTDIR)\spp_portscan2.sbr"
	-@erase "$(INTDIR)\spp_rpc_decode.obj"
	-@erase "$(INTDIR)\spp_rpc_decode.sbr"
	-@erase "$(INTDIR)\spp_stream4.obj"
	-@erase "$(INTDIR)\spp_stream4.sbr"
	-@erase "$(INTDIR)\spp_telnet_negotiation.obj"
	-@erase "$(INTDIR)\spp_telnet_negotiation.sbr"
	-@erase "$(INTDIR)\strlcatu.obj"
	-@erase "$(INTDIR)\strlcatu.sbr"
	-@erase "$(INTDIR)\strlcpyu.obj"
	-@erase "$(INTDIR)\strlcpyu.sbr"
	-@erase "$(INTDIR)\syslog.obj"
	-@erase "$(INTDIR)\syslog.sbr"
	-@erase "$(INTDIR)\tag.obj"
	-@erase "$(INTDIR)\tag.sbr"
	-@erase "$(INTDIR)\threshold.obj"
	-@erase "$(INTDIR)\threshold.sbr"
	-@erase "$(INTDIR)\ubi_BinTree.obj"
	-@erase "$(INTDIR)\ubi_BinTree.sbr"
	-@erase "$(INTDIR)\ubi_SplayTree.obj"
	-@erase "$(INTDIR)\ubi_SplayTree.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\win32_service.obj"
	-@erase "$(INTDIR)\win32_service.sbr"
	-@erase "$(OUTDIR)\snort.bsc"
	-@erase "$(OUTDIR)\snort.exe"
	-@erase "$(OUTDIR)\snort.ilk"
	-@erase "$(OUTDIR)\snort.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\snort.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\snort.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\acsmx.sbr" \
	"$(INTDIR)\byte_extract.sbr" \
	"$(INTDIR)\codes.sbr" \
	"$(INTDIR)\debug.sbr" \
	"$(INTDIR)\decode.sbr" \
	"$(INTDIR)\detect.sbr" \
	"$(INTDIR)\fpcreate.sbr" \
	"$(INTDIR)\fpdetect.sbr" \
	"$(INTDIR)\log.sbr" \
	"$(INTDIR)\mempool.sbr" \
	"$(INTDIR)\mpse.sbr" \
	"$(INTDIR)\mstring.sbr" \
	"$(INTDIR)\mwm.sbr" \
	"$(INTDIR)\parser.sbr" \
	"$(INTDIR)\pcrm.sbr" \
	"$(INTDIR)\plugbase.sbr" \
	"$(INTDIR)\sf_sdlist.sbr" \
	"$(INTDIR)\sfksearch.sbr" \
	"$(INTDIR)\signature.sbr" \
	"$(INTDIR)\snort.sbr" \
	"$(INTDIR)\snprintf.sbr" \
	"$(INTDIR)\strlcatu.sbr" \
	"$(INTDIR)\strlcpyu.sbr" \
	"$(INTDIR)\tag.sbr" \
	"$(INTDIR)\threshold.sbr" \
	"$(INTDIR)\ubi_BinTree.sbr" \
	"$(INTDIR)\ubi_SplayTree.sbr" \
	"$(INTDIR)\util.sbr" \
	"$(INTDIR)\getopt.sbr" \
	"$(INTDIR)\misc.sbr" \
	"$(INTDIR)\syslog.sbr" \
	"$(INTDIR)\win32_service.sbr" \
	"$(INTDIR)\sp_byte_check.sbr" \
	"$(INTDIR)\sp_byte_jump.sbr" \
	"$(INTDIR)\sp_clientserver.sbr" \
	"$(INTDIR)\sp_dsize_check.sbr" \
	"$(INTDIR)\sp_icmp_code_check.sbr" \
	"$(INTDIR)\sp_icmp_id_check.sbr" \
	"$(INTDIR)\sp_icmp_seq_check.sbr" \
	"$(INTDIR)\sp_icmp_type_check.sbr" \
	"$(INTDIR)\sp_ip_fragbits.sbr" \
	"$(INTDIR)\sp_ip_id_check.sbr" \
	"$(INTDIR)\sp_ip_proto.sbr" \
	"$(INTDIR)\sp_ip_same_check.sbr" \
	"$(INTDIR)\sp_ip_tos_check.sbr" \
	"$(INTDIR)\sp_ipoption_check.sbr" \
	"$(INTDIR)\sp_pattern_match.sbr" \
	"$(INTDIR)\sp_react.sbr" \
	"$(INTDIR)\sp_respond.sbr" \
	"$(INTDIR)\sp_rpc_check.sbr" \
	"$(INTDIR)\sp_session.sbr" \
	"$(INTDIR)\sp_tcp_ack_check.sbr" \
	"$(INTDIR)\sp_tcp_flag_check.sbr" \
	"$(INTDIR)\sp_tcp_seq_check.sbr" \
	"$(INTDIR)\sp_tcp_win_check.sbr" \
	"$(INTDIR)\sp_ttl_check.sbr" \
	"$(INTDIR)\spo_alert_fast.sbr" \
	"$(INTDIR)\spo_alert_full.sbr" \
	"$(INTDIR)\spo_alert_sf_socket.sbr" \
	"$(INTDIR)\spo_alert_smb.sbr" \
	"$(INTDIR)\spo_alert_syslog.sbr" \
	"$(INTDIR)\spo_alert_unixsock.sbr" \
	"$(INTDIR)\spo_csv.sbr" \
	"$(INTDIR)\spo_database.sbr" \
	"$(INTDIR)\spo_log_ascii.sbr" \
	"$(INTDIR)\spo_log_null.sbr" \
	"$(INTDIR)\spo_log_tcpdump.sbr" \
	"$(INTDIR)\spo_unified.sbr" \
	"$(INTDIR)\http-resp.sbr" \
	"$(INTDIR)\perf-base.sbr" \
	"$(INTDIR)\perf-event.sbr" \
	"$(INTDIR)\perf-flow.sbr" \
	"$(INTDIR)\perf.sbr" \
	"$(INTDIR)\sfprocpidstats.sbr" \
	"$(INTDIR)\spp_arpspoof.sbr" \
	"$(INTDIR)\spp_bo.sbr" \
	"$(INTDIR)\spp_conversation.sbr" \
	"$(INTDIR)\spp_frag2.sbr" \
	"$(INTDIR)\spp_http_decode.sbr" \
	"$(INTDIR)\spp_httpflow.sbr" \
	"$(INTDIR)\spp_perfmonitor.sbr" \
	"$(INTDIR)\spp_portscan.sbr" \
	"$(INTDIR)\spp_portscan2.sbr" \
	"$(INTDIR)\spp_rpc_decode.sbr" \
	"$(INTDIR)\spp_stream4.sbr" \
	"$(INTDIR)\spp_telnet_negotiation.sbr" \
	"$(INTDIR)\IpAddrSet.sbr"

"$(OUTDIR)\snort.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\snort.pdb" /debug /machine:I386 /out:"$(OUTDIR)\snort.exe" /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" 
LINK32_OBJS= \
	"$(INTDIR)\acsmx.obj" \
	"$(INTDIR)\byte_extract.obj" \
	"$(INTDIR)\codes.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\decode.obj" \
	"$(INTDIR)\detect.obj" \
	"$(INTDIR)\fpcreate.obj" \
	"$(INTDIR)\fpdetect.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\mempool.obj" \
	"$(INTDIR)\mpse.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\mwm.obj" \
	"$(INTDIR)\parser.obj" \
	"$(INTDIR)\pcrm.obj" \
	"$(INTDIR)\plugbase.obj" \
	"$(INTDIR)\sf_sdlist.obj" \
	"$(INTDIR)\sfksearch.obj" \
	"$(INTDIR)\signature.obj" \
	"$(INTDIR)\snort.obj" \
	"$(INTDIR)\snprintf.obj" \
	"$(INTDIR)\strlcatu.obj" \
	"$(INTDIR)\strlcpyu.obj" \
	"$(INTDIR)\tag.obj" \
	"$(INTDIR)\threshold.obj" \
	"$(INTDIR)\ubi_BinTree.obj" \
	"$(INTDIR)\ubi_SplayTree.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\syslog.obj" \
	"$(INTDIR)\win32_service.obj" \
	"$(INTDIR)\sp_byte_check.obj" \
	"$(INTDIR)\sp_byte_jump.obj" \
	"$(INTDIR)\sp_clientserver.obj" \
	"$(INTDIR)\sp_dsize_check.obj" \
	"$(INTDIR)\sp_icmp_code_check.obj" \
	"$(INTDIR)\sp_icmp_id_check.obj" \
	"$(INTDIR)\sp_icmp_seq_check.obj" \
	"$(INTDIR)\sp_icmp_type_check.obj" \
	"$(INTDIR)\sp_ip_fragbits.obj" \
	"$(INTDIR)\sp_ip_id_check.obj" \
	"$(INTDIR)\sp_ip_proto.obj" \
	"$(INTDIR)\sp_ip_same_check.obj" \
	"$(INTDIR)\sp_ip_tos_check.obj" \
	"$(INTDIR)\sp_ipoption_check.obj" \
	"$(INTDIR)\sp_pattern_match.obj" \
	"$(INTDIR)\sp_react.obj" \
	"$(INTDIR)\sp_respond.obj" \
	"$(INTDIR)\sp_rpc_check.obj" \
	"$(INTDIR)\sp_session.obj" \
	"$(INTDIR)\sp_tcp_ack_check.obj" \
	"$(INTDIR)\sp_tcp_flag_check.obj" \
	"$(INTDIR)\sp_tcp_seq_check.obj" \
	"$(INTDIR)\sp_tcp_win_check.obj" \
	"$(INTDIR)\sp_ttl_check.obj" \
	"$(INTDIR)\spo_alert_fast.obj" \
	"$(INTDIR)\spo_alert_full.obj" \
	"$(INTDIR)\spo_alert_sf_socket.obj" \
	"$(INTDIR)\spo_alert_smb.obj" \
	"$(INTDIR)\spo_alert_syslog.obj" \
	"$(INTDIR)\spo_alert_unixsock.obj" \
	"$(INTDIR)\spo_csv.obj" \
	"$(INTDIR)\spo_database.obj" \
	"$(INTDIR)\spo_log_ascii.obj" \
	"$(INTDIR)\spo_log_null.obj" \
	"$(INTDIR)\spo_log_tcpdump.obj" \
	"$(INTDIR)\spo_unified.obj" \
	"$(INTDIR)\http-resp.obj" \
	"$(INTDIR)\perf-base.obj" \
	"$(INTDIR)\perf-event.obj" \
	"$(INTDIR)\perf-flow.obj" \
	"$(INTDIR)\perf.obj" \
	"$(INTDIR)\sfprocpidstats.obj" \
	"$(INTDIR)\spp_arpspoof.obj" \
	"$(INTDIR)\spp_bo.obj" \
	"$(INTDIR)\spp_conversation.obj" \
	"$(INTDIR)\spp_frag2.obj" \
	"$(INTDIR)\spp_http_decode.obj" \
	"$(INTDIR)\spp_httpflow.obj" \
	"$(INTDIR)\spp_perfmonitor.obj" \
	"$(INTDIR)\spp_portscan.obj" \
	"$(INTDIR)\spp_portscan2.obj" \
	"$(INTDIR)\spp_rpc_decode.obj" \
	"$(INTDIR)\spp_stream4.obj" \
	"$(INTDIR)\spp_telnet_negotiation.obj" \
	"$(INTDIR)\IpAddrSet.obj"

"$(OUTDIR)\snort.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"

OUTDIR=.\snort___Win32_StdDB_Release
INTDIR=.\snort___Win32_StdDB_Release
# Begin Custom Macros
OutDir=.\snort___Win32_StdDB_Release
# End Custom Macros

ALL : "$(OUTDIR)\snort.exe"


CLEAN :
	-@erase "$(INTDIR)\acsmx.obj"
	-@erase "$(INTDIR)\byte_extract.obj"
	-@erase "$(INTDIR)\codes.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\decode.obj"
	-@erase "$(INTDIR)\detect.obj"
	-@erase "$(INTDIR)\fpcreate.obj"
	-@erase "$(INTDIR)\fpdetect.obj"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\http-resp.obj"
	-@erase "$(INTDIR)\IpAddrSet.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\mempool.obj"
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\mpse.obj"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\mwm.obj"
	-@erase "$(INTDIR)\parser.obj"
	-@erase "$(INTDIR)\pcrm.obj"
	-@erase "$(INTDIR)\perf-base.obj"
	-@erase "$(INTDIR)\perf-event.obj"
	-@erase "$(INTDIR)\perf-flow.obj"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\plugbase.obj"
	-@erase "$(INTDIR)\sf_sdlist.obj"
	-@erase "$(INTDIR)\sfksearch.obj"
	-@erase "$(INTDIR)\sfprocpidstats.obj"
	-@erase "$(INTDIR)\signature.obj"
	-@erase "$(INTDIR)\snort.obj"
	-@erase "$(INTDIR)\snprintf.obj"
	-@erase "$(INTDIR)\sp_byte_check.obj"
	-@erase "$(INTDIR)\sp_byte_jump.obj"
	-@erase "$(INTDIR)\sp_clientserver.obj"
	-@erase "$(INTDIR)\sp_dsize_check.obj"
	-@erase "$(INTDIR)\sp_icmp_code_check.obj"
	-@erase "$(INTDIR)\sp_icmp_id_check.obj"
	-@erase "$(INTDIR)\sp_icmp_seq_check.obj"
	-@erase "$(INTDIR)\sp_icmp_type_check.obj"
	-@erase "$(INTDIR)\sp_ip_fragbits.obj"
	-@erase "$(INTDIR)\sp_ip_id_check.obj"
	-@erase "$(INTDIR)\sp_ip_proto.obj"
	-@erase "$(INTDIR)\sp_ip_same_check.obj"
	-@erase "$(INTDIR)\sp_ip_tos_check.obj"
	-@erase "$(INTDIR)\sp_ipoption_check.obj"
	-@erase "$(INTDIR)\sp_pattern_match.obj"
	-@erase "$(INTDIR)\sp_react.obj"
	-@erase "$(INTDIR)\sp_respond.obj"
	-@erase "$(INTDIR)\sp_rpc_check.obj"
	-@erase "$(INTDIR)\sp_session.obj"
	-@erase "$(INTDIR)\sp_tcp_ack_check.obj"
	-@erase "$(INTDIR)\sp_tcp_flag_check.obj"
	-@erase "$(INTDIR)\sp_tcp_seq_check.obj"
	-@erase "$(INTDIR)\sp_tcp_win_check.obj"
	-@erase "$(INTDIR)\sp_ttl_check.obj"
	-@erase "$(INTDIR)\spo_alert_fast.obj"
	-@erase "$(INTDIR)\spo_alert_full.obj"
	-@erase "$(INTDIR)\spo_alert_sf_socket.obj"
	-@erase "$(INTDIR)\spo_alert_smb.obj"
	-@erase "$(INTDIR)\spo_alert_syslog.obj"
	-@erase "$(INTDIR)\spo_alert_unixsock.obj"
	-@erase "$(INTDIR)\spo_csv.obj"
	-@erase "$(INTDIR)\spo_database.obj"
	-@erase "$(INTDIR)\spo_log_ascii.obj"
	-@erase "$(INTDIR)\spo_log_null.obj"
	-@erase "$(INTDIR)\spo_log_tcpdump.obj"
	-@erase "$(INTDIR)\spo_unified.obj"
	-@erase "$(INTDIR)\spp_arpspoof.obj"
	-@erase "$(INTDIR)\spp_bo.obj"
	-@erase "$(INTDIR)\spp_conversation.obj"
	-@erase "$(INTDIR)\spp_frag2.obj"
	-@erase "$(INTDIR)\spp_http_decode.obj"
	-@erase "$(INTDIR)\spp_httpflow.obj"
	-@erase "$(INTDIR)\spp_perfmonitor.obj"
	-@erase "$(INTDIR)\spp_portscan.obj"
	-@erase "$(INTDIR)\spp_portscan2.obj"
	-@erase "$(INTDIR)\spp_rpc_decode.obj"
	-@erase "$(INTDIR)\spp_stream4.obj"
	-@erase "$(INTDIR)\spp_telnet_negotiation.obj"
	-@erase "$(INTDIR)\strlcatu.obj"
	-@erase "$(INTDIR)\strlcpyu.obj"
	-@erase "$(INTDIR)\syslog.obj"
	-@erase "$(INTDIR)\tag.obj"
	-@erase "$(INTDIR)\threshold.obj"
	-@erase "$(INTDIR)\ubi_BinTree.obj"
	-@erase "$(INTDIR)\ubi_SplayTree.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\win32_service.obj"
	-@erase "$(OUTDIR)\snort.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /Fp"$(INTDIR)\snort.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\snort.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\snort.pdb" /machine:I386 /out:"$(OUTDIR)\snort.exe" /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" 
LINK32_OBJS= \
	"$(INTDIR)\acsmx.obj" \
	"$(INTDIR)\byte_extract.obj" \
	"$(INTDIR)\codes.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\decode.obj" \
	"$(INTDIR)\detect.obj" \
	"$(INTDIR)\fpcreate.obj" \
	"$(INTDIR)\fpdetect.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\mempool.obj" \
	"$(INTDIR)\mpse.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\mwm.obj" \
	"$(INTDIR)\parser.obj" \
	"$(INTDIR)\pcrm.obj" \
	"$(INTDIR)\plugbase.obj" \
	"$(INTDIR)\sf_sdlist.obj" \
	"$(INTDIR)\sfksearch.obj" \
	"$(INTDIR)\signature.obj" \
	"$(INTDIR)\snort.obj" \
	"$(INTDIR)\snprintf.obj" \
	"$(INTDIR)\strlcatu.obj" \
	"$(INTDIR)\strlcpyu.obj" \
	"$(INTDIR)\tag.obj" \
	"$(INTDIR)\threshold.obj" \
	"$(INTDIR)\ubi_BinTree.obj" \
	"$(INTDIR)\ubi_SplayTree.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\syslog.obj" \
	"$(INTDIR)\win32_service.obj" \
	"$(INTDIR)\sp_byte_check.obj" \
	"$(INTDIR)\sp_byte_jump.obj" \
	"$(INTDIR)\sp_clientserver.obj" \
	"$(INTDIR)\sp_dsize_check.obj" \
	"$(INTDIR)\sp_icmp_code_check.obj" \
	"$(INTDIR)\sp_icmp_id_check.obj" \
	"$(INTDIR)\sp_icmp_seq_check.obj" \
	"$(INTDIR)\sp_icmp_type_check.obj" \
	"$(INTDIR)\sp_ip_fragbits.obj" \
	"$(INTDIR)\sp_ip_id_check.obj" \
	"$(INTDIR)\sp_ip_proto.obj" \
	"$(INTDIR)\sp_ip_same_check.obj" \
	"$(INTDIR)\sp_ip_tos_check.obj" \
	"$(INTDIR)\sp_ipoption_check.obj" \
	"$(INTDIR)\sp_pattern_match.obj" \
	"$(INTDIR)\sp_react.obj" \
	"$(INTDIR)\sp_respond.obj" \
	"$(INTDIR)\sp_rpc_check.obj" \
	"$(INTDIR)\sp_session.obj" \
	"$(INTDIR)\sp_tcp_ack_check.obj" \
	"$(INTDIR)\sp_tcp_flag_check.obj" \
	"$(INTDIR)\sp_tcp_seq_check.obj" \
	"$(INTDIR)\sp_tcp_win_check.obj" \
	"$(INTDIR)\sp_ttl_check.obj" \
	"$(INTDIR)\spo_alert_fast.obj" \
	"$(INTDIR)\spo_alert_full.obj" \
	"$(INTDIR)\spo_alert_sf_socket.obj" \
	"$(INTDIR)\spo_alert_smb.obj" \
	"$(INTDIR)\spo_alert_syslog.obj" \
	"$(INTDIR)\spo_alert_unixsock.obj" \
	"$(INTDIR)\spo_csv.obj" \
	"$(INTDIR)\spo_database.obj" \
	"$(INTDIR)\spo_log_ascii.obj" \
	"$(INTDIR)\spo_log_null.obj" \
	"$(INTDIR)\spo_log_tcpdump.obj" \
	"$(INTDIR)\spo_unified.obj" \
	"$(INTDIR)\http-resp.obj" \
	"$(INTDIR)\perf-base.obj" \
	"$(INTDIR)\perf-event.obj" \
	"$(INTDIR)\perf-flow.obj" \
	"$(INTDIR)\perf.obj" \
	"$(INTDIR)\sfprocpidstats.obj" \
	"$(INTDIR)\spp_arpspoof.obj" \
	"$(INTDIR)\spp_bo.obj" \
	"$(INTDIR)\spp_conversation.obj" \
	"$(INTDIR)\spp_frag2.obj" \
	"$(INTDIR)\spp_http_decode.obj" \
	"$(INTDIR)\spp_httpflow.obj" \
	"$(INTDIR)\spp_perfmonitor.obj" \
	"$(INTDIR)\spp_portscan.obj" \
	"$(INTDIR)\spp_portscan2.obj" \
	"$(INTDIR)\spp_rpc_decode.obj" \
	"$(INTDIR)\spp_stream4.obj" \
	"$(INTDIR)\spp_telnet_negotiation.obj" \
	"$(INTDIR)\IpAddrSet.obj"

"$(OUTDIR)\snort.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"

OUTDIR=.\snort___Win32_StdDB_FlexResp_Debug
INTDIR=.\snort___Win32_StdDB_FlexResp_Debug
# Begin Custom Macros
OutDir=.\snort___Win32_StdDB_FlexResp_Debug
# End Custom Macros

ALL : "$(OUTDIR)\snort.exe" "$(OUTDIR)\snort.bsc"


CLEAN :
	-@erase "$(INTDIR)\acsmx.obj"
	-@erase "$(INTDIR)\acsmx.sbr"
	-@erase "$(INTDIR)\byte_extract.obj"
	-@erase "$(INTDIR)\byte_extract.sbr"
	-@erase "$(INTDIR)\codes.obj"
	-@erase "$(INTDIR)\codes.sbr"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\debug.sbr"
	-@erase "$(INTDIR)\decode.obj"
	-@erase "$(INTDIR)\decode.sbr"
	-@erase "$(INTDIR)\detect.obj"
	-@erase "$(INTDIR)\detect.sbr"
	-@erase "$(INTDIR)\fpcreate.obj"
	-@erase "$(INTDIR)\fpcreate.sbr"
	-@erase "$(INTDIR)\fpdetect.obj"
	-@erase "$(INTDIR)\fpdetect.sbr"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\getopt.sbr"
	-@erase "$(INTDIR)\http-resp.obj"
	-@erase "$(INTDIR)\http-resp.sbr"
	-@erase "$(INTDIR)\IpAddrSet.obj"
	-@erase "$(INTDIR)\IpAddrSet.sbr"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\log.sbr"
	-@erase "$(INTDIR)\mempool.obj"
	-@erase "$(INTDIR)\mempool.sbr"
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\misc.sbr"
	-@erase "$(INTDIR)\mpse.obj"
	-@erase "$(INTDIR)\mpse.sbr"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\mstring.sbr"
	-@erase "$(INTDIR)\mwm.obj"
	-@erase "$(INTDIR)\mwm.sbr"
	-@erase "$(INTDIR)\parser.obj"
	-@erase "$(INTDIR)\parser.sbr"
	-@erase "$(INTDIR)\pcrm.obj"
	-@erase "$(INTDIR)\pcrm.sbr"
	-@erase "$(INTDIR)\perf-base.obj"
	-@erase "$(INTDIR)\perf-base.sbr"
	-@erase "$(INTDIR)\perf-event.obj"
	-@erase "$(INTDIR)\perf-event.sbr"
	-@erase "$(INTDIR)\perf-flow.obj"
	-@erase "$(INTDIR)\perf-flow.sbr"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\perf.sbr"
	-@erase "$(INTDIR)\plugbase.obj"
	-@erase "$(INTDIR)\plugbase.sbr"
	-@erase "$(INTDIR)\sf_sdlist.obj"
	-@erase "$(INTDIR)\sf_sdlist.sbr"
	-@erase "$(INTDIR)\sfksearch.obj"
	-@erase "$(INTDIR)\sfksearch.sbr"
	-@erase "$(INTDIR)\sfprocpidstats.obj"
	-@erase "$(INTDIR)\sfprocpidstats.sbr"
	-@erase "$(INTDIR)\signature.obj"
	-@erase "$(INTDIR)\signature.sbr"
	-@erase "$(INTDIR)\snort.obj"
	-@erase "$(INTDIR)\snort.sbr"
	-@erase "$(INTDIR)\snprintf.obj"
	-@erase "$(INTDIR)\snprintf.sbr"
	-@erase "$(INTDIR)\sp_byte_check.obj"
	-@erase "$(INTDIR)\sp_byte_check.sbr"
	-@erase "$(INTDIR)\sp_byte_jump.obj"
	-@erase "$(INTDIR)\sp_byte_jump.sbr"
	-@erase "$(INTDIR)\sp_clientserver.obj"
	-@erase "$(INTDIR)\sp_clientserver.sbr"
	-@erase "$(INTDIR)\sp_dsize_check.obj"
	-@erase "$(INTDIR)\sp_dsize_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_code_check.obj"
	-@erase "$(INTDIR)\sp_icmp_code_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_id_check.obj"
	-@erase "$(INTDIR)\sp_icmp_id_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_seq_check.obj"
	-@erase "$(INTDIR)\sp_icmp_seq_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_type_check.obj"
	-@erase "$(INTDIR)\sp_icmp_type_check.sbr"
	-@erase "$(INTDIR)\sp_ip_fragbits.obj"
	-@erase "$(INTDIR)\sp_ip_fragbits.sbr"
	-@erase "$(INTDIR)\sp_ip_id_check.obj"
	-@erase "$(INTDIR)\sp_ip_id_check.sbr"
	-@erase "$(INTDIR)\sp_ip_proto.obj"
	-@erase "$(INTDIR)\sp_ip_proto.sbr"
	-@erase "$(INTDIR)\sp_ip_same_check.obj"
	-@erase "$(INTDIR)\sp_ip_same_check.sbr"
	-@erase "$(INTDIR)\sp_ip_tos_check.obj"
	-@erase "$(INTDIR)\sp_ip_tos_check.sbr"
	-@erase "$(INTDIR)\sp_ipoption_check.obj"
	-@erase "$(INTDIR)\sp_ipoption_check.sbr"
	-@erase "$(INTDIR)\sp_pattern_match.obj"
	-@erase "$(INTDIR)\sp_pattern_match.sbr"
	-@erase "$(INTDIR)\sp_react.obj"
	-@erase "$(INTDIR)\sp_react.sbr"
	-@erase "$(INTDIR)\sp_respond.obj"
	-@erase "$(INTDIR)\sp_respond.sbr"
	-@erase "$(INTDIR)\sp_rpc_check.obj"
	-@erase "$(INTDIR)\sp_rpc_check.sbr"
	-@erase "$(INTDIR)\sp_session.obj"
	-@erase "$(INTDIR)\sp_session.sbr"
	-@erase "$(INTDIR)\sp_tcp_ack_check.obj"
	-@erase "$(INTDIR)\sp_tcp_ack_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_flag_check.obj"
	-@erase "$(INTDIR)\sp_tcp_flag_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_seq_check.obj"
	-@erase "$(INTDIR)\sp_tcp_seq_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_win_check.obj"
	-@erase "$(INTDIR)\sp_tcp_win_check.sbr"
	-@erase "$(INTDIR)\sp_ttl_check.obj"
	-@erase "$(INTDIR)\sp_ttl_check.sbr"
	-@erase "$(INTDIR)\spo_alert_fast.obj"
	-@erase "$(INTDIR)\spo_alert_fast.sbr"
	-@erase "$(INTDIR)\spo_alert_full.obj"
	-@erase "$(INTDIR)\spo_alert_full.sbr"
	-@erase "$(INTDIR)\spo_alert_sf_socket.obj"
	-@erase "$(INTDIR)\spo_alert_sf_socket.sbr"
	-@erase "$(INTDIR)\spo_alert_smb.obj"
	-@erase "$(INTDIR)\spo_alert_smb.sbr"
	-@erase "$(INTDIR)\spo_alert_syslog.obj"
	-@erase "$(INTDIR)\spo_alert_syslog.sbr"
	-@erase "$(INTDIR)\spo_alert_unixsock.obj"
	-@erase "$(INTDIR)\spo_alert_unixsock.sbr"
	-@erase "$(INTDIR)\spo_csv.obj"
	-@erase "$(INTDIR)\spo_csv.sbr"
	-@erase "$(INTDIR)\spo_database.obj"
	-@erase "$(INTDIR)\spo_database.sbr"
	-@erase "$(INTDIR)\spo_log_ascii.obj"
	-@erase "$(INTDIR)\spo_log_ascii.sbr"
	-@erase "$(INTDIR)\spo_log_null.obj"
	-@erase "$(INTDIR)\spo_log_null.sbr"
	-@erase "$(INTDIR)\spo_log_tcpdump.obj"
	-@erase "$(INTDIR)\spo_log_tcpdump.sbr"
	-@erase "$(INTDIR)\spo_unified.obj"
	-@erase "$(INTDIR)\spo_unified.sbr"
	-@erase "$(INTDIR)\spp_arpspoof.obj"
	-@erase "$(INTDIR)\spp_arpspoof.sbr"
	-@erase "$(INTDIR)\spp_bo.obj"
	-@erase "$(INTDIR)\spp_bo.sbr"
	-@erase "$(INTDIR)\spp_conversation.obj"
	-@erase "$(INTDIR)\spp_conversation.sbr"
	-@erase "$(INTDIR)\spp_frag2.obj"
	-@erase "$(INTDIR)\spp_frag2.sbr"
	-@erase "$(INTDIR)\spp_http_decode.obj"
	-@erase "$(INTDIR)\spp_http_decode.sbr"
	-@erase "$(INTDIR)\spp_httpflow.obj"
	-@erase "$(INTDIR)\spp_httpflow.sbr"
	-@erase "$(INTDIR)\spp_perfmonitor.obj"
	-@erase "$(INTDIR)\spp_perfmonitor.sbr"
	-@erase "$(INTDIR)\spp_portscan.obj"
	-@erase "$(INTDIR)\spp_portscan.sbr"
	-@erase "$(INTDIR)\spp_portscan2.obj"
	-@erase "$(INTDIR)\spp_portscan2.sbr"
	-@erase "$(INTDIR)\spp_rpc_decode.obj"
	-@erase "$(INTDIR)\spp_rpc_decode.sbr"
	-@erase "$(INTDIR)\spp_stream4.obj"
	-@erase "$(INTDIR)\spp_stream4.sbr"
	-@erase "$(INTDIR)\spp_telnet_negotiation.obj"
	-@erase "$(INTDIR)\spp_telnet_negotiation.sbr"
	-@erase "$(INTDIR)\strlcatu.obj"
	-@erase "$(INTDIR)\strlcatu.sbr"
	-@erase "$(INTDIR)\strlcpyu.obj"
	-@erase "$(INTDIR)\strlcpyu.sbr"
	-@erase "$(INTDIR)\syslog.obj"
	-@erase "$(INTDIR)\syslog.sbr"
	-@erase "$(INTDIR)\tag.obj"
	-@erase "$(INTDIR)\tag.sbr"
	-@erase "$(INTDIR)\threshold.obj"
	-@erase "$(INTDIR)\threshold.sbr"
	-@erase "$(INTDIR)\ubi_BinTree.obj"
	-@erase "$(INTDIR)\ubi_BinTree.sbr"
	-@erase "$(INTDIR)\ubi_SplayTree.obj"
	-@erase "$(INTDIR)\ubi_SplayTree.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\win32_service.obj"
	-@erase "$(INTDIR)\win32_service.sbr"
	-@erase "$(OUTDIR)\snort.bsc"
	-@erase "$(OUTDIR)\snort.exe"
	-@erase "$(OUTDIR)\snort.ilk"
	-@erase "$(OUTDIR)\snort.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\snort.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\snort.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\acsmx.sbr" \
	"$(INTDIR)\byte_extract.sbr" \
	"$(INTDIR)\codes.sbr" \
	"$(INTDIR)\debug.sbr" \
	"$(INTDIR)\decode.sbr" \
	"$(INTDIR)\detect.sbr" \
	"$(INTDIR)\fpcreate.sbr" \
	"$(INTDIR)\fpdetect.sbr" \
	"$(INTDIR)\log.sbr" \
	"$(INTDIR)\mempool.sbr" \
	"$(INTDIR)\mpse.sbr" \
	"$(INTDIR)\mstring.sbr" \
	"$(INTDIR)\mwm.sbr" \
	"$(INTDIR)\parser.sbr" \
	"$(INTDIR)\pcrm.sbr" \
	"$(INTDIR)\plugbase.sbr" \
	"$(INTDIR)\sf_sdlist.sbr" \
	"$(INTDIR)\sfksearch.sbr" \
	"$(INTDIR)\signature.sbr" \
	"$(INTDIR)\snort.sbr" \
	"$(INTDIR)\snprintf.sbr" \
	"$(INTDIR)\strlcatu.sbr" \
	"$(INTDIR)\strlcpyu.sbr" \
	"$(INTDIR)\tag.sbr" \
	"$(INTDIR)\threshold.sbr" \
	"$(INTDIR)\ubi_BinTree.sbr" \
	"$(INTDIR)\ubi_SplayTree.sbr" \
	"$(INTDIR)\util.sbr" \
	"$(INTDIR)\getopt.sbr" \
	"$(INTDIR)\misc.sbr" \
	"$(INTDIR)\syslog.sbr" \
	"$(INTDIR)\win32_service.sbr" \
	"$(INTDIR)\sp_byte_check.sbr" \
	"$(INTDIR)\sp_byte_jump.sbr" \
	"$(INTDIR)\sp_clientserver.sbr" \
	"$(INTDIR)\sp_dsize_check.sbr" \
	"$(INTDIR)\sp_icmp_code_check.sbr" \
	"$(INTDIR)\sp_icmp_id_check.sbr" \
	"$(INTDIR)\sp_icmp_seq_check.sbr" \
	"$(INTDIR)\sp_icmp_type_check.sbr" \
	"$(INTDIR)\sp_ip_fragbits.sbr" \
	"$(INTDIR)\sp_ip_id_check.sbr" \
	"$(INTDIR)\sp_ip_proto.sbr" \
	"$(INTDIR)\sp_ip_same_check.sbr" \
	"$(INTDIR)\sp_ip_tos_check.sbr" \
	"$(INTDIR)\sp_ipoption_check.sbr" \
	"$(INTDIR)\sp_pattern_match.sbr" \
	"$(INTDIR)\sp_react.sbr" \
	"$(INTDIR)\sp_respond.sbr" \
	"$(INTDIR)\sp_rpc_check.sbr" \
	"$(INTDIR)\sp_session.sbr" \
	"$(INTDIR)\sp_tcp_ack_check.sbr" \
	"$(INTDIR)\sp_tcp_flag_check.sbr" \
	"$(INTDIR)\sp_tcp_seq_check.sbr" \
	"$(INTDIR)\sp_tcp_win_check.sbr" \
	"$(INTDIR)\sp_ttl_check.sbr" \
	"$(INTDIR)\spo_alert_fast.sbr" \
	"$(INTDIR)\spo_alert_full.sbr" \
	"$(INTDIR)\spo_alert_sf_socket.sbr" \
	"$(INTDIR)\spo_alert_smb.sbr" \
	"$(INTDIR)\spo_alert_syslog.sbr" \
	"$(INTDIR)\spo_alert_unixsock.sbr" \
	"$(INTDIR)\spo_csv.sbr" \
	"$(INTDIR)\spo_database.sbr" \
	"$(INTDIR)\spo_log_ascii.sbr" \
	"$(INTDIR)\spo_log_null.sbr" \
	"$(INTDIR)\spo_log_tcpdump.sbr" \
	"$(INTDIR)\spo_unified.sbr" \
	"$(INTDIR)\http-resp.sbr" \
	"$(INTDIR)\perf-base.sbr" \
	"$(INTDIR)\perf-event.sbr" \
	"$(INTDIR)\perf-flow.sbr" \
	"$(INTDIR)\perf.sbr" \
	"$(INTDIR)\sfprocpidstats.sbr" \
	"$(INTDIR)\spp_arpspoof.sbr" \
	"$(INTDIR)\spp_bo.sbr" \
	"$(INTDIR)\spp_conversation.sbr" \
	"$(INTDIR)\spp_frag2.sbr" \
	"$(INTDIR)\spp_http_decode.sbr" \
	"$(INTDIR)\spp_httpflow.sbr" \
	"$(INTDIR)\spp_perfmonitor.sbr" \
	"$(INTDIR)\spp_portscan.sbr" \
	"$(INTDIR)\spp_portscan2.sbr" \
	"$(INTDIR)\spp_rpc_decode.sbr" \
	"$(INTDIR)\spp_stream4.sbr" \
	"$(INTDIR)\spp_telnet_negotiation.sbr" \
	"$(INTDIR)\IpAddrSet.sbr"

"$(OUTDIR)\snort.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\snort.pdb" /debug /machine:I386 /out:"$(OUTDIR)\snort.exe" /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet" 
LINK32_OBJS= \
	"$(INTDIR)\acsmx.obj" \
	"$(INTDIR)\byte_extract.obj" \
	"$(INTDIR)\codes.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\decode.obj" \
	"$(INTDIR)\detect.obj" \
	"$(INTDIR)\fpcreate.obj" \
	"$(INTDIR)\fpdetect.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\mempool.obj" \
	"$(INTDIR)\mpse.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\mwm.obj" \
	"$(INTDIR)\parser.obj" \
	"$(INTDIR)\pcrm.obj" \
	"$(INTDIR)\plugbase.obj" \
	"$(INTDIR)\sf_sdlist.obj" \
	"$(INTDIR)\sfksearch.obj" \
	"$(INTDIR)\signature.obj" \
	"$(INTDIR)\snort.obj" \
	"$(INTDIR)\snprintf.obj" \
	"$(INTDIR)\strlcatu.obj" \
	"$(INTDIR)\strlcpyu.obj" \
	"$(INTDIR)\tag.obj" \
	"$(INTDIR)\threshold.obj" \
	"$(INTDIR)\ubi_BinTree.obj" \
	"$(INTDIR)\ubi_SplayTree.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\syslog.obj" \
	"$(INTDIR)\win32_service.obj" \
	"$(INTDIR)\sp_byte_check.obj" \
	"$(INTDIR)\sp_byte_jump.obj" \
	"$(INTDIR)\sp_clientserver.obj" \
	"$(INTDIR)\sp_dsize_check.obj" \
	"$(INTDIR)\sp_icmp_code_check.obj" \
	"$(INTDIR)\sp_icmp_id_check.obj" \
	"$(INTDIR)\sp_icmp_seq_check.obj" \
	"$(INTDIR)\sp_icmp_type_check.obj" \
	"$(INTDIR)\sp_ip_fragbits.obj" \
	"$(INTDIR)\sp_ip_id_check.obj" \
	"$(INTDIR)\sp_ip_proto.obj" \
	"$(INTDIR)\sp_ip_same_check.obj" \
	"$(INTDIR)\sp_ip_tos_check.obj" \
	"$(INTDIR)\sp_ipoption_check.obj" \
	"$(INTDIR)\sp_pattern_match.obj" \
	"$(INTDIR)\sp_react.obj" \
	"$(INTDIR)\sp_respond.obj" \
	"$(INTDIR)\sp_rpc_check.obj" \
	"$(INTDIR)\sp_session.obj" \
	"$(INTDIR)\sp_tcp_ack_check.obj" \
	"$(INTDIR)\sp_tcp_flag_check.obj" \
	"$(INTDIR)\sp_tcp_seq_check.obj" \
	"$(INTDIR)\sp_tcp_win_check.obj" \
	"$(INTDIR)\sp_ttl_check.obj" \
	"$(INTDIR)\spo_alert_fast.obj" \
	"$(INTDIR)\spo_alert_full.obj" \
	"$(INTDIR)\spo_alert_sf_socket.obj" \
	"$(INTDIR)\spo_alert_smb.obj" \
	"$(INTDIR)\spo_alert_syslog.obj" \
	"$(INTDIR)\spo_alert_unixsock.obj" \
	"$(INTDIR)\spo_csv.obj" \
	"$(INTDIR)\spo_database.obj" \
	"$(INTDIR)\spo_log_ascii.obj" \
	"$(INTDIR)\spo_log_null.obj" \
	"$(INTDIR)\spo_log_tcpdump.obj" \
	"$(INTDIR)\spo_unified.obj" \
	"$(INTDIR)\http-resp.obj" \
	"$(INTDIR)\perf-base.obj" \
	"$(INTDIR)\perf-event.obj" \
	"$(INTDIR)\perf-flow.obj" \
	"$(INTDIR)\perf.obj" \
	"$(INTDIR)\sfprocpidstats.obj" \
	"$(INTDIR)\spp_arpspoof.obj" \
	"$(INTDIR)\spp_bo.obj" \
	"$(INTDIR)\spp_conversation.obj" \
	"$(INTDIR)\spp_frag2.obj" \
	"$(INTDIR)\spp_http_decode.obj" \
	"$(INTDIR)\spp_httpflow.obj" \
	"$(INTDIR)\spp_perfmonitor.obj" \
	"$(INTDIR)\spp_portscan.obj" \
	"$(INTDIR)\spp_portscan2.obj" \
	"$(INTDIR)\spp_rpc_decode.obj" \
	"$(INTDIR)\spp_stream4.obj" \
	"$(INTDIR)\spp_telnet_negotiation.obj" \
	"$(INTDIR)\IpAddrSet.obj"

"$(OUTDIR)\snort.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"

OUTDIR=.\snort___Win32_StdDB_FlexResp_Release
INTDIR=.\snort___Win32_StdDB_FlexResp_Release
# Begin Custom Macros
OutDir=.\snort___Win32_StdDB_FlexResp_Release
# End Custom Macros

ALL : "$(OUTDIR)\snort.exe"


CLEAN :
	-@erase "$(INTDIR)\acsmx.obj"
	-@erase "$(INTDIR)\byte_extract.obj"
	-@erase "$(INTDIR)\codes.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\decode.obj"
	-@erase "$(INTDIR)\detect.obj"
	-@erase "$(INTDIR)\fpcreate.obj"
	-@erase "$(INTDIR)\fpdetect.obj"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\http-resp.obj"
	-@erase "$(INTDIR)\IpAddrSet.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\mempool.obj"
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\mpse.obj"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\mwm.obj"
	-@erase "$(INTDIR)\parser.obj"
	-@erase "$(INTDIR)\pcrm.obj"
	-@erase "$(INTDIR)\perf-base.obj"
	-@erase "$(INTDIR)\perf-event.obj"
	-@erase "$(INTDIR)\perf-flow.obj"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\plugbase.obj"
	-@erase "$(INTDIR)\sf_sdlist.obj"
	-@erase "$(INTDIR)\sfksearch.obj"
	-@erase "$(INTDIR)\sfprocpidstats.obj"
	-@erase "$(INTDIR)\signature.obj"
	-@erase "$(INTDIR)\snort.obj"
	-@erase "$(INTDIR)\snprintf.obj"
	-@erase "$(INTDIR)\sp_byte_check.obj"
	-@erase "$(INTDIR)\sp_byte_jump.obj"
	-@erase "$(INTDIR)\sp_clientserver.obj"
	-@erase "$(INTDIR)\sp_dsize_check.obj"
	-@erase "$(INTDIR)\sp_icmp_code_check.obj"
	-@erase "$(INTDIR)\sp_icmp_id_check.obj"
	-@erase "$(INTDIR)\sp_icmp_seq_check.obj"
	-@erase "$(INTDIR)\sp_icmp_type_check.obj"
	-@erase "$(INTDIR)\sp_ip_fragbits.obj"
	-@erase "$(INTDIR)\sp_ip_id_check.obj"
	-@erase "$(INTDIR)\sp_ip_proto.obj"
	-@erase "$(INTDIR)\sp_ip_same_check.obj"
	-@erase "$(INTDIR)\sp_ip_tos_check.obj"
	-@erase "$(INTDIR)\sp_ipoption_check.obj"
	-@erase "$(INTDIR)\sp_pattern_match.obj"
	-@erase "$(INTDIR)\sp_react.obj"
	-@erase "$(INTDIR)\sp_respond.obj"
	-@erase "$(INTDIR)\sp_rpc_check.obj"
	-@erase "$(INTDIR)\sp_session.obj"
	-@erase "$(INTDIR)\sp_tcp_ack_check.obj"
	-@erase "$(INTDIR)\sp_tcp_flag_check.obj"
	-@erase "$(INTDIR)\sp_tcp_seq_check.obj"
	-@erase "$(INTDIR)\sp_tcp_win_check.obj"
	-@erase "$(INTDIR)\sp_ttl_check.obj"
	-@erase "$(INTDIR)\spo_alert_fast.obj"
	-@erase "$(INTDIR)\spo_alert_full.obj"
	-@erase "$(INTDIR)\spo_alert_sf_socket.obj"
	-@erase "$(INTDIR)\spo_alert_smb.obj"
	-@erase "$(INTDIR)\spo_alert_syslog.obj"
	-@erase "$(INTDIR)\spo_alert_unixsock.obj"
	-@erase "$(INTDIR)\spo_csv.obj"
	-@erase "$(INTDIR)\spo_database.obj"
	-@erase "$(INTDIR)\spo_log_ascii.obj"
	-@erase "$(INTDIR)\spo_log_null.obj"
	-@erase "$(INTDIR)\spo_log_tcpdump.obj"
	-@erase "$(INTDIR)\spo_unified.obj"
	-@erase "$(INTDIR)\spp_arpspoof.obj"
	-@erase "$(INTDIR)\spp_bo.obj"
	-@erase "$(INTDIR)\spp_conversation.obj"
	-@erase "$(INTDIR)\spp_frag2.obj"
	-@erase "$(INTDIR)\spp_http_decode.obj"
	-@erase "$(INTDIR)\spp_httpflow.obj"
	-@erase "$(INTDIR)\spp_perfmonitor.obj"
	-@erase "$(INTDIR)\spp_portscan.obj"
	-@erase "$(INTDIR)\spp_portscan2.obj"
	-@erase "$(INTDIR)\spp_rpc_decode.obj"
	-@erase "$(INTDIR)\spp_stream4.obj"
	-@erase "$(INTDIR)\spp_telnet_negotiation.obj"
	-@erase "$(INTDIR)\strlcatu.obj"
	-@erase "$(INTDIR)\strlcpyu.obj"
	-@erase "$(INTDIR)\syslog.obj"
	-@erase "$(INTDIR)\tag.obj"
	-@erase "$(INTDIR)\threshold.obj"
	-@erase "$(INTDIR)\ubi_BinTree.obj"
	-@erase "$(INTDIR)\ubi_SplayTree.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\win32_service.obj"
	-@erase "$(OUTDIR)\snort.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /Fp"$(INTDIR)\snort.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\snort.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\snort.pdb" /machine:I386 /nodefaultlib:"libcmt.lib" /out:"$(OUTDIR)\snort.exe" /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet" 
LINK32_OBJS= \
	"$(INTDIR)\acsmx.obj" \
	"$(INTDIR)\byte_extract.obj" \
	"$(INTDIR)\codes.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\decode.obj" \
	"$(INTDIR)\detect.obj" \
	"$(INTDIR)\fpcreate.obj" \
	"$(INTDIR)\fpdetect.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\mempool.obj" \
	"$(INTDIR)\mpse.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\mwm.obj" \
	"$(INTDIR)\parser.obj" \
	"$(INTDIR)\pcrm.obj" \
	"$(INTDIR)\plugbase.obj" \
	"$(INTDIR)\sf_sdlist.obj" \
	"$(INTDIR)\sfksearch.obj" \
	"$(INTDIR)\signature.obj" \
	"$(INTDIR)\snort.obj" \
	"$(INTDIR)\snprintf.obj" \
	"$(INTDIR)\strlcatu.obj" \
	"$(INTDIR)\strlcpyu.obj" \
	"$(INTDIR)\tag.obj" \
	"$(INTDIR)\threshold.obj" \
	"$(INTDIR)\ubi_BinTree.obj" \
	"$(INTDIR)\ubi_SplayTree.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\syslog.obj" \
	"$(INTDIR)\win32_service.obj" \
	"$(INTDIR)\sp_byte_check.obj" \
	"$(INTDIR)\sp_byte_jump.obj" \
	"$(INTDIR)\sp_clientserver.obj" \
	"$(INTDIR)\sp_dsize_check.obj" \
	"$(INTDIR)\sp_icmp_code_check.obj" \
	"$(INTDIR)\sp_icmp_id_check.obj" \
	"$(INTDIR)\sp_icmp_seq_check.obj" \
	"$(INTDIR)\sp_icmp_type_check.obj" \
	"$(INTDIR)\sp_ip_fragbits.obj" \
	"$(INTDIR)\sp_ip_id_check.obj" \
	"$(INTDIR)\sp_ip_proto.obj" \
	"$(INTDIR)\sp_ip_same_check.obj" \
	"$(INTDIR)\sp_ip_tos_check.obj" \
	"$(INTDIR)\sp_ipoption_check.obj" \
	"$(INTDIR)\sp_pattern_match.obj" \
	"$(INTDIR)\sp_react.obj" \
	"$(INTDIR)\sp_respond.obj" \
	"$(INTDIR)\sp_rpc_check.obj" \
	"$(INTDIR)\sp_session.obj" \
	"$(INTDIR)\sp_tcp_ack_check.obj" \
	"$(INTDIR)\sp_tcp_flag_check.obj" \
	"$(INTDIR)\sp_tcp_seq_check.obj" \
	"$(INTDIR)\sp_tcp_win_check.obj" \
	"$(INTDIR)\sp_ttl_check.obj" \
	"$(INTDIR)\spo_alert_fast.obj" \
	"$(INTDIR)\spo_alert_full.obj" \
	"$(INTDIR)\spo_alert_sf_socket.obj" \
	"$(INTDIR)\spo_alert_smb.obj" \
	"$(INTDIR)\spo_alert_syslog.obj" \
	"$(INTDIR)\spo_alert_unixsock.obj" \
	"$(INTDIR)\spo_csv.obj" \
	"$(INTDIR)\spo_database.obj" \
	"$(INTDIR)\spo_log_ascii.obj" \
	"$(INTDIR)\spo_log_null.obj" \
	"$(INTDIR)\spo_log_tcpdump.obj" \
	"$(INTDIR)\spo_unified.obj" \
	"$(INTDIR)\http-resp.obj" \
	"$(INTDIR)\perf-base.obj" \
	"$(INTDIR)\perf-event.obj" \
	"$(INTDIR)\perf-flow.obj" \
	"$(INTDIR)\perf.obj" \
	"$(INTDIR)\sfprocpidstats.obj" \
	"$(INTDIR)\spp_arpspoof.obj" \
	"$(INTDIR)\spp_bo.obj" \
	"$(INTDIR)\spp_conversation.obj" \
	"$(INTDIR)\spp_frag2.obj" \
	"$(INTDIR)\spp_http_decode.obj" \
	"$(INTDIR)\spp_httpflow.obj" \
	"$(INTDIR)\spp_perfmonitor.obj" \
	"$(INTDIR)\spp_portscan.obj" \
	"$(INTDIR)\spp_portscan2.obj" \
	"$(INTDIR)\spp_rpc_decode.obj" \
	"$(INTDIR)\spp_stream4.obj" \
	"$(INTDIR)\spp_telnet_negotiation.obj" \
	"$(INTDIR)\IpAddrSet.obj"

"$(OUTDIR)\snort.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"

OUTDIR=.\snort___Win32_StdDB_FlexResp_Service_Debug
INTDIR=.\snort___Win32_StdDB_FlexResp_Service_Debug
# Begin Custom Macros
OutDir=.\snort___Win32_StdDB_FlexResp_Service_Debug
# End Custom Macros

ALL : "$(OUTDIR)\snort.exe" "$(OUTDIR)\snort.bsc"


CLEAN :
	-@erase "$(INTDIR)\acsmx.obj"
	-@erase "$(INTDIR)\acsmx.sbr"
	-@erase "$(INTDIR)\byte_extract.obj"
	-@erase "$(INTDIR)\byte_extract.sbr"
	-@erase "$(INTDIR)\codes.obj"
	-@erase "$(INTDIR)\codes.sbr"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\debug.sbr"
	-@erase "$(INTDIR)\decode.obj"
	-@erase "$(INTDIR)\decode.sbr"
	-@erase "$(INTDIR)\detect.obj"
	-@erase "$(INTDIR)\detect.sbr"
	-@erase "$(INTDIR)\fpcreate.obj"
	-@erase "$(INTDIR)\fpcreate.sbr"
	-@erase "$(INTDIR)\fpdetect.obj"
	-@erase "$(INTDIR)\fpdetect.sbr"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\getopt.sbr"
	-@erase "$(INTDIR)\http-resp.obj"
	-@erase "$(INTDIR)\http-resp.sbr"
	-@erase "$(INTDIR)\IpAddrSet.obj"
	-@erase "$(INTDIR)\IpAddrSet.sbr"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\log.sbr"
	-@erase "$(INTDIR)\mempool.obj"
	-@erase "$(INTDIR)\mempool.sbr"
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\misc.sbr"
	-@erase "$(INTDIR)\mpse.obj"
	-@erase "$(INTDIR)\mpse.sbr"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\mstring.sbr"
	-@erase "$(INTDIR)\mwm.obj"
	-@erase "$(INTDIR)\mwm.sbr"
	-@erase "$(INTDIR)\parser.obj"
	-@erase "$(INTDIR)\parser.sbr"
	-@erase "$(INTDIR)\pcrm.obj"
	-@erase "$(INTDIR)\pcrm.sbr"
	-@erase "$(INTDIR)\perf-base.obj"
	-@erase "$(INTDIR)\perf-base.sbr"
	-@erase "$(INTDIR)\perf-event.obj"
	-@erase "$(INTDIR)\perf-event.sbr"
	-@erase "$(INTDIR)\perf-flow.obj"
	-@erase "$(INTDIR)\perf-flow.sbr"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\perf.sbr"
	-@erase "$(INTDIR)\plugbase.obj"
	-@erase "$(INTDIR)\plugbase.sbr"
	-@erase "$(INTDIR)\sf_sdlist.obj"
	-@erase "$(INTDIR)\sf_sdlist.sbr"
	-@erase "$(INTDIR)\sfksearch.obj"
	-@erase "$(INTDIR)\sfksearch.sbr"
	-@erase "$(INTDIR)\sfprocpidstats.obj"
	-@erase "$(INTDIR)\sfprocpidstats.sbr"
	-@erase "$(INTDIR)\signature.obj"
	-@erase "$(INTDIR)\signature.sbr"
	-@erase "$(INTDIR)\snort.obj"
	-@erase "$(INTDIR)\snort.sbr"
	-@erase "$(INTDIR)\snprintf.obj"
	-@erase "$(INTDIR)\snprintf.sbr"
	-@erase "$(INTDIR)\sp_byte_check.obj"
	-@erase "$(INTDIR)\sp_byte_check.sbr"
	-@erase "$(INTDIR)\sp_byte_jump.obj"
	-@erase "$(INTDIR)\sp_byte_jump.sbr"
	-@erase "$(INTDIR)\sp_clientserver.obj"
	-@erase "$(INTDIR)\sp_clientserver.sbr"
	-@erase "$(INTDIR)\sp_dsize_check.obj"
	-@erase "$(INTDIR)\sp_dsize_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_code_check.obj"
	-@erase "$(INTDIR)\sp_icmp_code_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_id_check.obj"
	-@erase "$(INTDIR)\sp_icmp_id_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_seq_check.obj"
	-@erase "$(INTDIR)\sp_icmp_seq_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_type_check.obj"
	-@erase "$(INTDIR)\sp_icmp_type_check.sbr"
	-@erase "$(INTDIR)\sp_ip_fragbits.obj"
	-@erase "$(INTDIR)\sp_ip_fragbits.sbr"
	-@erase "$(INTDIR)\sp_ip_id_check.obj"
	-@erase "$(INTDIR)\sp_ip_id_check.sbr"
	-@erase "$(INTDIR)\sp_ip_proto.obj"
	-@erase "$(INTDIR)\sp_ip_proto.sbr"
	-@erase "$(INTDIR)\sp_ip_same_check.obj"
	-@erase "$(INTDIR)\sp_ip_same_check.sbr"
	-@erase "$(INTDIR)\sp_ip_tos_check.obj"
	-@erase "$(INTDIR)\sp_ip_tos_check.sbr"
	-@erase "$(INTDIR)\sp_ipoption_check.obj"
	-@erase "$(INTDIR)\sp_ipoption_check.sbr"
	-@erase "$(INTDIR)\sp_pattern_match.obj"
	-@erase "$(INTDIR)\sp_pattern_match.sbr"
	-@erase "$(INTDIR)\sp_react.obj"
	-@erase "$(INTDIR)\sp_react.sbr"
	-@erase "$(INTDIR)\sp_respond.obj"
	-@erase "$(INTDIR)\sp_respond.sbr"
	-@erase "$(INTDIR)\sp_rpc_check.obj"
	-@erase "$(INTDIR)\sp_rpc_check.sbr"
	-@erase "$(INTDIR)\sp_session.obj"
	-@erase "$(INTDIR)\sp_session.sbr"
	-@erase "$(INTDIR)\sp_tcp_ack_check.obj"
	-@erase "$(INTDIR)\sp_tcp_ack_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_flag_check.obj"
	-@erase "$(INTDIR)\sp_tcp_flag_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_seq_check.obj"
	-@erase "$(INTDIR)\sp_tcp_seq_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_win_check.obj"
	-@erase "$(INTDIR)\sp_tcp_win_check.sbr"
	-@erase "$(INTDIR)\sp_ttl_check.obj"
	-@erase "$(INTDIR)\sp_ttl_check.sbr"
	-@erase "$(INTDIR)\spo_alert_fast.obj"
	-@erase "$(INTDIR)\spo_alert_fast.sbr"
	-@erase "$(INTDIR)\spo_alert_full.obj"
	-@erase "$(INTDIR)\spo_alert_full.sbr"
	-@erase "$(INTDIR)\spo_alert_sf_socket.obj"
	-@erase "$(INTDIR)\spo_alert_sf_socket.sbr"
	-@erase "$(INTDIR)\spo_alert_smb.obj"
	-@erase "$(INTDIR)\spo_alert_smb.sbr"
	-@erase "$(INTDIR)\spo_alert_syslog.obj"
	-@erase "$(INTDIR)\spo_alert_syslog.sbr"
	-@erase "$(INTDIR)\spo_alert_unixsock.obj"
	-@erase "$(INTDIR)\spo_alert_unixsock.sbr"
	-@erase "$(INTDIR)\spo_csv.obj"
	-@erase "$(INTDIR)\spo_csv.sbr"
	-@erase "$(INTDIR)\spo_database.obj"
	-@erase "$(INTDIR)\spo_database.sbr"
	-@erase "$(INTDIR)\spo_log_ascii.obj"
	-@erase "$(INTDIR)\spo_log_ascii.sbr"
	-@erase "$(INTDIR)\spo_log_null.obj"
	-@erase "$(INTDIR)\spo_log_null.sbr"
	-@erase "$(INTDIR)\spo_log_tcpdump.obj"
	-@erase "$(INTDIR)\spo_log_tcpdump.sbr"
	-@erase "$(INTDIR)\spo_unified.obj"
	-@erase "$(INTDIR)\spo_unified.sbr"
	-@erase "$(INTDIR)\spp_arpspoof.obj"
	-@erase "$(INTDIR)\spp_arpspoof.sbr"
	-@erase "$(INTDIR)\spp_bo.obj"
	-@erase "$(INTDIR)\spp_bo.sbr"
	-@erase "$(INTDIR)\spp_conversation.obj"
	-@erase "$(INTDIR)\spp_conversation.sbr"
	-@erase "$(INTDIR)\spp_frag2.obj"
	-@erase "$(INTDIR)\spp_frag2.sbr"
	-@erase "$(INTDIR)\spp_http_decode.obj"
	-@erase "$(INTDIR)\spp_http_decode.sbr"
	-@erase "$(INTDIR)\spp_httpflow.obj"
	-@erase "$(INTDIR)\spp_httpflow.sbr"
	-@erase "$(INTDIR)\spp_perfmonitor.obj"
	-@erase "$(INTDIR)\spp_perfmonitor.sbr"
	-@erase "$(INTDIR)\spp_portscan.obj"
	-@erase "$(INTDIR)\spp_portscan.sbr"
	-@erase "$(INTDIR)\spp_portscan2.obj"
	-@erase "$(INTDIR)\spp_portscan2.sbr"
	-@erase "$(INTDIR)\spp_rpc_decode.obj"
	-@erase "$(INTDIR)\spp_rpc_decode.sbr"
	-@erase "$(INTDIR)\spp_stream4.obj"
	-@erase "$(INTDIR)\spp_stream4.sbr"
	-@erase "$(INTDIR)\spp_telnet_negotiation.obj"
	-@erase "$(INTDIR)\spp_telnet_negotiation.sbr"
	-@erase "$(INTDIR)\strlcatu.obj"
	-@erase "$(INTDIR)\strlcatu.sbr"
	-@erase "$(INTDIR)\strlcpyu.obj"
	-@erase "$(INTDIR)\strlcpyu.sbr"
	-@erase "$(INTDIR)\syslog.obj"
	-@erase "$(INTDIR)\syslog.sbr"
	-@erase "$(INTDIR)\tag.obj"
	-@erase "$(INTDIR)\tag.sbr"
	-@erase "$(INTDIR)\threshold.obj"
	-@erase "$(INTDIR)\threshold.sbr"
	-@erase "$(INTDIR)\ubi_BinTree.obj"
	-@erase "$(INTDIR)\ubi_BinTree.sbr"
	-@erase "$(INTDIR)\ubi_SplayTree.obj"
	-@erase "$(INTDIR)\ubi_SplayTree.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\win32_service.obj"
	-@erase "$(INTDIR)\win32_service.sbr"
	-@erase "$(OUTDIR)\snort.bsc"
	-@erase "$(OUTDIR)\snort.exe"
	-@erase "$(OUTDIR)\snort.ilk"
	-@erase "$(OUTDIR)\snort.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /D "ENABLE_WIN32_SERVICE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\snort.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\snort.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\acsmx.sbr" \
	"$(INTDIR)\byte_extract.sbr" \
	"$(INTDIR)\codes.sbr" \
	"$(INTDIR)\debug.sbr" \
	"$(INTDIR)\decode.sbr" \
	"$(INTDIR)\detect.sbr" \
	"$(INTDIR)\fpcreate.sbr" \
	"$(INTDIR)\fpdetect.sbr" \
	"$(INTDIR)\log.sbr" \
	"$(INTDIR)\mempool.sbr" \
	"$(INTDIR)\mpse.sbr" \
	"$(INTDIR)\mstring.sbr" \
	"$(INTDIR)\mwm.sbr" \
	"$(INTDIR)\parser.sbr" \
	"$(INTDIR)\pcrm.sbr" \
	"$(INTDIR)\plugbase.sbr" \
	"$(INTDIR)\sf_sdlist.sbr" \
	"$(INTDIR)\sfksearch.sbr" \
	"$(INTDIR)\signature.sbr" \
	"$(INTDIR)\snort.sbr" \
	"$(INTDIR)\snprintf.sbr" \
	"$(INTDIR)\strlcatu.sbr" \
	"$(INTDIR)\strlcpyu.sbr" \
	"$(INTDIR)\tag.sbr" \
	"$(INTDIR)\threshold.sbr" \
	"$(INTDIR)\ubi_BinTree.sbr" \
	"$(INTDIR)\ubi_SplayTree.sbr" \
	"$(INTDIR)\util.sbr" \
	"$(INTDIR)\getopt.sbr" \
	"$(INTDIR)\misc.sbr" \
	"$(INTDIR)\syslog.sbr" \
	"$(INTDIR)\win32_service.sbr" \
	"$(INTDIR)\sp_byte_check.sbr" \
	"$(INTDIR)\sp_byte_jump.sbr" \
	"$(INTDIR)\sp_clientserver.sbr" \
	"$(INTDIR)\sp_dsize_check.sbr" \
	"$(INTDIR)\sp_icmp_code_check.sbr" \
	"$(INTDIR)\sp_icmp_id_check.sbr" \
	"$(INTDIR)\sp_icmp_seq_check.sbr" \
	"$(INTDIR)\sp_icmp_type_check.sbr" \
	"$(INTDIR)\sp_ip_fragbits.sbr" \
	"$(INTDIR)\sp_ip_id_check.sbr" \
	"$(INTDIR)\sp_ip_proto.sbr" \
	"$(INTDIR)\sp_ip_same_check.sbr" \
	"$(INTDIR)\sp_ip_tos_check.sbr" \
	"$(INTDIR)\sp_ipoption_check.sbr" \
	"$(INTDIR)\sp_pattern_match.sbr" \
	"$(INTDIR)\sp_react.sbr" \
	"$(INTDIR)\sp_respond.sbr" \
	"$(INTDIR)\sp_rpc_check.sbr" \
	"$(INTDIR)\sp_session.sbr" \
	"$(INTDIR)\sp_tcp_ack_check.sbr" \
	"$(INTDIR)\sp_tcp_flag_check.sbr" \
	"$(INTDIR)\sp_tcp_seq_check.sbr" \
	"$(INTDIR)\sp_tcp_win_check.sbr" \
	"$(INTDIR)\sp_ttl_check.sbr" \
	"$(INTDIR)\spo_alert_fast.sbr" \
	"$(INTDIR)\spo_alert_full.sbr" \
	"$(INTDIR)\spo_alert_sf_socket.sbr" \
	"$(INTDIR)\spo_alert_smb.sbr" \
	"$(INTDIR)\spo_alert_syslog.sbr" \
	"$(INTDIR)\spo_alert_unixsock.sbr" \
	"$(INTDIR)\spo_csv.sbr" \
	"$(INTDIR)\spo_database.sbr" \
	"$(INTDIR)\spo_log_ascii.sbr" \
	"$(INTDIR)\spo_log_null.sbr" \
	"$(INTDIR)\spo_log_tcpdump.sbr" \
	"$(INTDIR)\spo_unified.sbr" \
	"$(INTDIR)\http-resp.sbr" \
	"$(INTDIR)\perf-base.sbr" \
	"$(INTDIR)\perf-event.sbr" \
	"$(INTDIR)\perf-flow.sbr" \
	"$(INTDIR)\perf.sbr" \
	"$(INTDIR)\sfprocpidstats.sbr" \
	"$(INTDIR)\spp_arpspoof.sbr" \
	"$(INTDIR)\spp_bo.sbr" \
	"$(INTDIR)\spp_conversation.sbr" \
	"$(INTDIR)\spp_frag2.sbr" \
	"$(INTDIR)\spp_http_decode.sbr" \
	"$(INTDIR)\spp_httpflow.sbr" \
	"$(INTDIR)\spp_perfmonitor.sbr" \
	"$(INTDIR)\spp_portscan.sbr" \
	"$(INTDIR)\spp_portscan2.sbr" \
	"$(INTDIR)\spp_rpc_decode.sbr" \
	"$(INTDIR)\spp_stream4.sbr" \
	"$(INTDIR)\spp_telnet_negotiation.sbr" \
	"$(INTDIR)\IpAddrSet.sbr"

"$(OUTDIR)\snort.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\snort.pdb" /debug /machine:I386 /out:"$(OUTDIR)\snort.exe" /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet" 
LINK32_OBJS= \
	"$(INTDIR)\acsmx.obj" \
	"$(INTDIR)\byte_extract.obj" \
	"$(INTDIR)\codes.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\decode.obj" \
	"$(INTDIR)\detect.obj" \
	"$(INTDIR)\fpcreate.obj" \
	"$(INTDIR)\fpdetect.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\mempool.obj" \
	"$(INTDIR)\mpse.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\mwm.obj" \
	"$(INTDIR)\parser.obj" \
	"$(INTDIR)\pcrm.obj" \
	"$(INTDIR)\plugbase.obj" \
	"$(INTDIR)\sf_sdlist.obj" \
	"$(INTDIR)\sfksearch.obj" \
	"$(INTDIR)\signature.obj" \
	"$(INTDIR)\snort.obj" \
	"$(INTDIR)\snprintf.obj" \
	"$(INTDIR)\strlcatu.obj" \
	"$(INTDIR)\strlcpyu.obj" \
	"$(INTDIR)\tag.obj" \
	"$(INTDIR)\threshold.obj" \
	"$(INTDIR)\ubi_BinTree.obj" \
	"$(INTDIR)\ubi_SplayTree.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\syslog.obj" \
	"$(INTDIR)\win32_service.obj" \
	"$(INTDIR)\sp_byte_check.obj" \
	"$(INTDIR)\sp_byte_jump.obj" \
	"$(INTDIR)\sp_clientserver.obj" \
	"$(INTDIR)\sp_dsize_check.obj" \
	"$(INTDIR)\sp_icmp_code_check.obj" \
	"$(INTDIR)\sp_icmp_id_check.obj" \
	"$(INTDIR)\sp_icmp_seq_check.obj" \
	"$(INTDIR)\sp_icmp_type_check.obj" \
	"$(INTDIR)\sp_ip_fragbits.obj" \
	"$(INTDIR)\sp_ip_id_check.obj" \
	"$(INTDIR)\sp_ip_proto.obj" \
	"$(INTDIR)\sp_ip_same_check.obj" \
	"$(INTDIR)\sp_ip_tos_check.obj" \
	"$(INTDIR)\sp_ipoption_check.obj" \
	"$(INTDIR)\sp_pattern_match.obj" \
	"$(INTDIR)\sp_react.obj" \
	"$(INTDIR)\sp_respond.obj" \
	"$(INTDIR)\sp_rpc_check.obj" \
	"$(INTDIR)\sp_session.obj" \
	"$(INTDIR)\sp_tcp_ack_check.obj" \
	"$(INTDIR)\sp_tcp_flag_check.obj" \
	"$(INTDIR)\sp_tcp_seq_check.obj" \
	"$(INTDIR)\sp_tcp_win_check.obj" \
	"$(INTDIR)\sp_ttl_check.obj" \
	"$(INTDIR)\spo_alert_fast.obj" \
	"$(INTDIR)\spo_alert_full.obj" \
	"$(INTDIR)\spo_alert_sf_socket.obj" \
	"$(INTDIR)\spo_alert_smb.obj" \
	"$(INTDIR)\spo_alert_syslog.obj" \
	"$(INTDIR)\spo_alert_unixsock.obj" \
	"$(INTDIR)\spo_csv.obj" \
	"$(INTDIR)\spo_database.obj" \
	"$(INTDIR)\spo_log_ascii.obj" \
	"$(INTDIR)\spo_log_null.obj" \
	"$(INTDIR)\spo_log_tcpdump.obj" \
	"$(INTDIR)\spo_unified.obj" \
	"$(INTDIR)\http-resp.obj" \
	"$(INTDIR)\perf-base.obj" \
	"$(INTDIR)\perf-event.obj" \
	"$(INTDIR)\perf-flow.obj" \
	"$(INTDIR)\perf.obj" \
	"$(INTDIR)\sfprocpidstats.obj" \
	"$(INTDIR)\spp_arpspoof.obj" \
	"$(INTDIR)\spp_bo.obj" \
	"$(INTDIR)\spp_conversation.obj" \
	"$(INTDIR)\spp_frag2.obj" \
	"$(INTDIR)\spp_http_decode.obj" \
	"$(INTDIR)\spp_httpflow.obj" \
	"$(INTDIR)\spp_perfmonitor.obj" \
	"$(INTDIR)\spp_portscan.obj" \
	"$(INTDIR)\spp_portscan2.obj" \
	"$(INTDIR)\spp_rpc_decode.obj" \
	"$(INTDIR)\spp_stream4.obj" \
	"$(INTDIR)\spp_telnet_negotiation.obj" \
	"$(INTDIR)\IpAddrSet.obj"

"$(OUTDIR)\snort.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"

OUTDIR=.\snort___Win32_StdDB_FlexResp_Service_Release
INTDIR=.\snort___Win32_StdDB_FlexResp_Service_Release
# Begin Custom Macros
OutDir=.\snort___Win32_StdDB_FlexResp_Service_Release
# End Custom Macros

ALL : "$(OUTDIR)\snort.exe"


CLEAN :
	-@erase "$(INTDIR)\acsmx.obj"
	-@erase "$(INTDIR)\byte_extract.obj"
	-@erase "$(INTDIR)\codes.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\decode.obj"
	-@erase "$(INTDIR)\detect.obj"
	-@erase "$(INTDIR)\fpcreate.obj"
	-@erase "$(INTDIR)\fpdetect.obj"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\http-resp.obj"
	-@erase "$(INTDIR)\IpAddrSet.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\mempool.obj"
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\mpse.obj"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\mwm.obj"
	-@erase "$(INTDIR)\parser.obj"
	-@erase "$(INTDIR)\pcrm.obj"
	-@erase "$(INTDIR)\perf-base.obj"
	-@erase "$(INTDIR)\perf-event.obj"
	-@erase "$(INTDIR)\perf-flow.obj"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\plugbase.obj"
	-@erase "$(INTDIR)\sf_sdlist.obj"
	-@erase "$(INTDIR)\sfksearch.obj"
	-@erase "$(INTDIR)\sfprocpidstats.obj"
	-@erase "$(INTDIR)\signature.obj"
	-@erase "$(INTDIR)\snort.obj"
	-@erase "$(INTDIR)\snprintf.obj"
	-@erase "$(INTDIR)\sp_byte_check.obj"
	-@erase "$(INTDIR)\sp_byte_jump.obj"
	-@erase "$(INTDIR)\sp_clientserver.obj"
	-@erase "$(INTDIR)\sp_dsize_check.obj"
	-@erase "$(INTDIR)\sp_icmp_code_check.obj"
	-@erase "$(INTDIR)\sp_icmp_id_check.obj"
	-@erase "$(INTDIR)\sp_icmp_seq_check.obj"
	-@erase "$(INTDIR)\sp_icmp_type_check.obj"
	-@erase "$(INTDIR)\sp_ip_fragbits.obj"
	-@erase "$(INTDIR)\sp_ip_id_check.obj"
	-@erase "$(INTDIR)\sp_ip_proto.obj"
	-@erase "$(INTDIR)\sp_ip_same_check.obj"
	-@erase "$(INTDIR)\sp_ip_tos_check.obj"
	-@erase "$(INTDIR)\sp_ipoption_check.obj"
	-@erase "$(INTDIR)\sp_pattern_match.obj"
	-@erase "$(INTDIR)\sp_react.obj"
	-@erase "$(INTDIR)\sp_respond.obj"
	-@erase "$(INTDIR)\sp_rpc_check.obj"
	-@erase "$(INTDIR)\sp_session.obj"
	-@erase "$(INTDIR)\sp_tcp_ack_check.obj"
	-@erase "$(INTDIR)\sp_tcp_flag_check.obj"
	-@erase "$(INTDIR)\sp_tcp_seq_check.obj"
	-@erase "$(INTDIR)\sp_tcp_win_check.obj"
	-@erase "$(INTDIR)\sp_ttl_check.obj"
	-@erase "$(INTDIR)\spo_alert_fast.obj"
	-@erase "$(INTDIR)\spo_alert_full.obj"
	-@erase "$(INTDIR)\spo_alert_sf_socket.obj"
	-@erase "$(INTDIR)\spo_alert_smb.obj"
	-@erase "$(INTDIR)\spo_alert_syslog.obj"
	-@erase "$(INTDIR)\spo_alert_unixsock.obj"
	-@erase "$(INTDIR)\spo_csv.obj"
	-@erase "$(INTDIR)\spo_database.obj"
	-@erase "$(INTDIR)\spo_log_ascii.obj"
	-@erase "$(INTDIR)\spo_log_null.obj"
	-@erase "$(INTDIR)\spo_log_tcpdump.obj"
	-@erase "$(INTDIR)\spo_unified.obj"
	-@erase "$(INTDIR)\spp_arpspoof.obj"
	-@erase "$(INTDIR)\spp_bo.obj"
	-@erase "$(INTDIR)\spp_conversation.obj"
	-@erase "$(INTDIR)\spp_frag2.obj"
	-@erase "$(INTDIR)\spp_http_decode.obj"
	-@erase "$(INTDIR)\spp_httpflow.obj"
	-@erase "$(INTDIR)\spp_perfmonitor.obj"
	-@erase "$(INTDIR)\spp_portscan.obj"
	-@erase "$(INTDIR)\spp_portscan2.obj"
	-@erase "$(INTDIR)\spp_rpc_decode.obj"
	-@erase "$(INTDIR)\spp_stream4.obj"
	-@erase "$(INTDIR)\spp_telnet_negotiation.obj"
	-@erase "$(INTDIR)\strlcatu.obj"
	-@erase "$(INTDIR)\strlcpyu.obj"
	-@erase "$(INTDIR)\syslog.obj"
	-@erase "$(INTDIR)\tag.obj"
	-@erase "$(INTDIR)\threshold.obj"
	-@erase "$(INTDIR)\ubi_BinTree.obj"
	-@erase "$(INTDIR)\ubi_SplayTree.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\win32_service.obj"
	-@erase "$(OUTDIR)\snort.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /D "ENABLE_WIN32_SERVICE" /Fp"$(INTDIR)\snort.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\snort.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\snort.pdb" /machine:I386 /out:"$(OUTDIR)\snort.exe" /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet" 
LINK32_OBJS= \
	"$(INTDIR)\acsmx.obj" \
	"$(INTDIR)\byte_extract.obj" \
	"$(INTDIR)\codes.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\decode.obj" \
	"$(INTDIR)\detect.obj" \
	"$(INTDIR)\fpcreate.obj" \
	"$(INTDIR)\fpdetect.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\mempool.obj" \
	"$(INTDIR)\mpse.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\mwm.obj" \
	"$(INTDIR)\parser.obj" \
	"$(INTDIR)\pcrm.obj" \
	"$(INTDIR)\plugbase.obj" \
	"$(INTDIR)\sf_sdlist.obj" \
	"$(INTDIR)\sfksearch.obj" \
	"$(INTDIR)\signature.obj" \
	"$(INTDIR)\snort.obj" \
	"$(INTDIR)\snprintf.obj" \
	"$(INTDIR)\strlcatu.obj" \
	"$(INTDIR)\strlcpyu.obj" \
	"$(INTDIR)\tag.obj" \
	"$(INTDIR)\threshold.obj" \
	"$(INTDIR)\ubi_BinTree.obj" \
	"$(INTDIR)\ubi_SplayTree.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\syslog.obj" \
	"$(INTDIR)\win32_service.obj" \
	"$(INTDIR)\sp_byte_check.obj" \
	"$(INTDIR)\sp_byte_jump.obj" \
	"$(INTDIR)\sp_clientserver.obj" \
	"$(INTDIR)\sp_dsize_check.obj" \
	"$(INTDIR)\sp_icmp_code_check.obj" \
	"$(INTDIR)\sp_icmp_id_check.obj" \
	"$(INTDIR)\sp_icmp_seq_check.obj" \
	"$(INTDIR)\sp_icmp_type_check.obj" \
	"$(INTDIR)\sp_ip_fragbits.obj" \
	"$(INTDIR)\sp_ip_id_check.obj" \
	"$(INTDIR)\sp_ip_proto.obj" \
	"$(INTDIR)\sp_ip_same_check.obj" \
	"$(INTDIR)\sp_ip_tos_check.obj" \
	"$(INTDIR)\sp_ipoption_check.obj" \
	"$(INTDIR)\sp_pattern_match.obj" \
	"$(INTDIR)\sp_react.obj" \
	"$(INTDIR)\sp_respond.obj" \
	"$(INTDIR)\sp_rpc_check.obj" \
	"$(INTDIR)\sp_session.obj" \
	"$(INTDIR)\sp_tcp_ack_check.obj" \
	"$(INTDIR)\sp_tcp_flag_check.obj" \
	"$(INTDIR)\sp_tcp_seq_check.obj" \
	"$(INTDIR)\sp_tcp_win_check.obj" \
	"$(INTDIR)\sp_ttl_check.obj" \
	"$(INTDIR)\spo_alert_fast.obj" \
	"$(INTDIR)\spo_alert_full.obj" \
	"$(INTDIR)\spo_alert_sf_socket.obj" \
	"$(INTDIR)\spo_alert_smb.obj" \
	"$(INTDIR)\spo_alert_syslog.obj" \
	"$(INTDIR)\spo_alert_unixsock.obj" \
	"$(INTDIR)\spo_csv.obj" \
	"$(INTDIR)\spo_database.obj" \
	"$(INTDIR)\spo_log_ascii.obj" \
	"$(INTDIR)\spo_log_null.obj" \
	"$(INTDIR)\spo_log_tcpdump.obj" \
	"$(INTDIR)\spo_unified.obj" \
	"$(INTDIR)\http-resp.obj" \
	"$(INTDIR)\perf-base.obj" \
	"$(INTDIR)\perf-event.obj" \
	"$(INTDIR)\perf-flow.obj" \
	"$(INTDIR)\perf.obj" \
	"$(INTDIR)\sfprocpidstats.obj" \
	"$(INTDIR)\spp_arpspoof.obj" \
	"$(INTDIR)\spp_bo.obj" \
	"$(INTDIR)\spp_conversation.obj" \
	"$(INTDIR)\spp_frag2.obj" \
	"$(INTDIR)\spp_http_decode.obj" \
	"$(INTDIR)\spp_httpflow.obj" \
	"$(INTDIR)\spp_perfmonitor.obj" \
	"$(INTDIR)\spp_portscan.obj" \
	"$(INTDIR)\spp_portscan2.obj" \
	"$(INTDIR)\spp_rpc_decode.obj" \
	"$(INTDIR)\spp_stream4.obj" \
	"$(INTDIR)\spp_telnet_negotiation.obj" \
	"$(INTDIR)\IpAddrSet.obj"

"$(OUTDIR)\snort.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"

OUTDIR=.\snort___Win32_StdDB_MSSQL_Debug
INTDIR=.\snort___Win32_StdDB_MSSQL_Debug
# Begin Custom Macros
OutDir=.\snort___Win32_StdDB_MSSQL_Debug
# End Custom Macros

ALL : "$(OUTDIR)\snort.exe" "$(OUTDIR)\snort.bsc"


CLEAN :
	-@erase "$(INTDIR)\acsmx.obj"
	-@erase "$(INTDIR)\acsmx.sbr"
	-@erase "$(INTDIR)\byte_extract.obj"
	-@erase "$(INTDIR)\byte_extract.sbr"
	-@erase "$(INTDIR)\codes.obj"
	-@erase "$(INTDIR)\codes.sbr"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\debug.sbr"
	-@erase "$(INTDIR)\decode.obj"
	-@erase "$(INTDIR)\decode.sbr"
	-@erase "$(INTDIR)\detect.obj"
	-@erase "$(INTDIR)\detect.sbr"
	-@erase "$(INTDIR)\fpcreate.obj"
	-@erase "$(INTDIR)\fpcreate.sbr"
	-@erase "$(INTDIR)\fpdetect.obj"
	-@erase "$(INTDIR)\fpdetect.sbr"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\getopt.sbr"
	-@erase "$(INTDIR)\http-resp.obj"
	-@erase "$(INTDIR)\http-resp.sbr"
	-@erase "$(INTDIR)\IpAddrSet.obj"
	-@erase "$(INTDIR)\IpAddrSet.sbr"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\log.sbr"
	-@erase "$(INTDIR)\mempool.obj"
	-@erase "$(INTDIR)\mempool.sbr"
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\misc.sbr"
	-@erase "$(INTDIR)\mpse.obj"
	-@erase "$(INTDIR)\mpse.sbr"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\mstring.sbr"
	-@erase "$(INTDIR)\mwm.obj"
	-@erase "$(INTDIR)\mwm.sbr"
	-@erase "$(INTDIR)\parser.obj"
	-@erase "$(INTDIR)\parser.sbr"
	-@erase "$(INTDIR)\pcrm.obj"
	-@erase "$(INTDIR)\pcrm.sbr"
	-@erase "$(INTDIR)\perf-base.obj"
	-@erase "$(INTDIR)\perf-base.sbr"
	-@erase "$(INTDIR)\perf-event.obj"
	-@erase "$(INTDIR)\perf-event.sbr"
	-@erase "$(INTDIR)\perf-flow.obj"
	-@erase "$(INTDIR)\perf-flow.sbr"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\perf.sbr"
	-@erase "$(INTDIR)\plugbase.obj"
	-@erase "$(INTDIR)\plugbase.sbr"
	-@erase "$(INTDIR)\sf_sdlist.obj"
	-@erase "$(INTDIR)\sf_sdlist.sbr"
	-@erase "$(INTDIR)\sfksearch.obj"
	-@erase "$(INTDIR)\sfksearch.sbr"
	-@erase "$(INTDIR)\sfprocpidstats.obj"
	-@erase "$(INTDIR)\sfprocpidstats.sbr"
	-@erase "$(INTDIR)\signature.obj"
	-@erase "$(INTDIR)\signature.sbr"
	-@erase "$(INTDIR)\snort.obj"
	-@erase "$(INTDIR)\snort.sbr"
	-@erase "$(INTDIR)\snprintf.obj"
	-@erase "$(INTDIR)\snprintf.sbr"
	-@erase "$(INTDIR)\sp_byte_check.obj"
	-@erase "$(INTDIR)\sp_byte_check.sbr"
	-@erase "$(INTDIR)\sp_byte_jump.obj"
	-@erase "$(INTDIR)\sp_byte_jump.sbr"
	-@erase "$(INTDIR)\sp_clientserver.obj"
	-@erase "$(INTDIR)\sp_clientserver.sbr"
	-@erase "$(INTDIR)\sp_dsize_check.obj"
	-@erase "$(INTDIR)\sp_dsize_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_code_check.obj"
	-@erase "$(INTDIR)\sp_icmp_code_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_id_check.obj"
	-@erase "$(INTDIR)\sp_icmp_id_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_seq_check.obj"
	-@erase "$(INTDIR)\sp_icmp_seq_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_type_check.obj"
	-@erase "$(INTDIR)\sp_icmp_type_check.sbr"
	-@erase "$(INTDIR)\sp_ip_fragbits.obj"
	-@erase "$(INTDIR)\sp_ip_fragbits.sbr"
	-@erase "$(INTDIR)\sp_ip_id_check.obj"
	-@erase "$(INTDIR)\sp_ip_id_check.sbr"
	-@erase "$(INTDIR)\sp_ip_proto.obj"
	-@erase "$(INTDIR)\sp_ip_proto.sbr"
	-@erase "$(INTDIR)\sp_ip_same_check.obj"
	-@erase "$(INTDIR)\sp_ip_same_check.sbr"
	-@erase "$(INTDIR)\sp_ip_tos_check.obj"
	-@erase "$(INTDIR)\sp_ip_tos_check.sbr"
	-@erase "$(INTDIR)\sp_ipoption_check.obj"
	-@erase "$(INTDIR)\sp_ipoption_check.sbr"
	-@erase "$(INTDIR)\sp_pattern_match.obj"
	-@erase "$(INTDIR)\sp_pattern_match.sbr"
	-@erase "$(INTDIR)\sp_react.obj"
	-@erase "$(INTDIR)\sp_react.sbr"
	-@erase "$(INTDIR)\sp_respond.obj"
	-@erase "$(INTDIR)\sp_respond.sbr"
	-@erase "$(INTDIR)\sp_rpc_check.obj"
	-@erase "$(INTDIR)\sp_rpc_check.sbr"
	-@erase "$(INTDIR)\sp_session.obj"
	-@erase "$(INTDIR)\sp_session.sbr"
	-@erase "$(INTDIR)\sp_tcp_ack_check.obj"
	-@erase "$(INTDIR)\sp_tcp_ack_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_flag_check.obj"
	-@erase "$(INTDIR)\sp_tcp_flag_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_seq_check.obj"
	-@erase "$(INTDIR)\sp_tcp_seq_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_win_check.obj"
	-@erase "$(INTDIR)\sp_tcp_win_check.sbr"
	-@erase "$(INTDIR)\sp_ttl_check.obj"
	-@erase "$(INTDIR)\sp_ttl_check.sbr"
	-@erase "$(INTDIR)\spo_alert_fast.obj"
	-@erase "$(INTDIR)\spo_alert_fast.sbr"
	-@erase "$(INTDIR)\spo_alert_full.obj"
	-@erase "$(INTDIR)\spo_alert_full.sbr"
	-@erase "$(INTDIR)\spo_alert_sf_socket.obj"
	-@erase "$(INTDIR)\spo_alert_sf_socket.sbr"
	-@erase "$(INTDIR)\spo_alert_smb.obj"
	-@erase "$(INTDIR)\spo_alert_smb.sbr"
	-@erase "$(INTDIR)\spo_alert_syslog.obj"
	-@erase "$(INTDIR)\spo_alert_syslog.sbr"
	-@erase "$(INTDIR)\spo_alert_unixsock.obj"
	-@erase "$(INTDIR)\spo_alert_unixsock.sbr"
	-@erase "$(INTDIR)\spo_csv.obj"
	-@erase "$(INTDIR)\spo_csv.sbr"
	-@erase "$(INTDIR)\spo_database.obj"
	-@erase "$(INTDIR)\spo_database.sbr"
	-@erase "$(INTDIR)\spo_log_ascii.obj"
	-@erase "$(INTDIR)\spo_log_ascii.sbr"
	-@erase "$(INTDIR)\spo_log_null.obj"
	-@erase "$(INTDIR)\spo_log_null.sbr"
	-@erase "$(INTDIR)\spo_log_tcpdump.obj"
	-@erase "$(INTDIR)\spo_log_tcpdump.sbr"
	-@erase "$(INTDIR)\spo_unified.obj"
	-@erase "$(INTDIR)\spo_unified.sbr"
	-@erase "$(INTDIR)\spp_arpspoof.obj"
	-@erase "$(INTDIR)\spp_arpspoof.sbr"
	-@erase "$(INTDIR)\spp_bo.obj"
	-@erase "$(INTDIR)\spp_bo.sbr"
	-@erase "$(INTDIR)\spp_conversation.obj"
	-@erase "$(INTDIR)\spp_conversation.sbr"
	-@erase "$(INTDIR)\spp_frag2.obj"
	-@erase "$(INTDIR)\spp_frag2.sbr"
	-@erase "$(INTDIR)\spp_http_decode.obj"
	-@erase "$(INTDIR)\spp_http_decode.sbr"
	-@erase "$(INTDIR)\spp_httpflow.obj"
	-@erase "$(INTDIR)\spp_httpflow.sbr"
	-@erase "$(INTDIR)\spp_perfmonitor.obj"
	-@erase "$(INTDIR)\spp_perfmonitor.sbr"
	-@erase "$(INTDIR)\spp_portscan.obj"
	-@erase "$(INTDIR)\spp_portscan.sbr"
	-@erase "$(INTDIR)\spp_portscan2.obj"
	-@erase "$(INTDIR)\spp_portscan2.sbr"
	-@erase "$(INTDIR)\spp_rpc_decode.obj"
	-@erase "$(INTDIR)\spp_rpc_decode.sbr"
	-@erase "$(INTDIR)\spp_stream4.obj"
	-@erase "$(INTDIR)\spp_stream4.sbr"
	-@erase "$(INTDIR)\spp_telnet_negotiation.obj"
	-@erase "$(INTDIR)\spp_telnet_negotiation.sbr"
	-@erase "$(INTDIR)\strlcatu.obj"
	-@erase "$(INTDIR)\strlcatu.sbr"
	-@erase "$(INTDIR)\strlcpyu.obj"
	-@erase "$(INTDIR)\strlcpyu.sbr"
	-@erase "$(INTDIR)\syslog.obj"
	-@erase "$(INTDIR)\syslog.sbr"
	-@erase "$(INTDIR)\tag.obj"
	-@erase "$(INTDIR)\tag.sbr"
	-@erase "$(INTDIR)\threshold.obj"
	-@erase "$(INTDIR)\threshold.sbr"
	-@erase "$(INTDIR)\ubi_BinTree.obj"
	-@erase "$(INTDIR)\ubi_BinTree.sbr"
	-@erase "$(INTDIR)\ubi_SplayTree.obj"
	-@erase "$(INTDIR)\ubi_SplayTree.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\win32_service.obj"
	-@erase "$(INTDIR)\win32_service.sbr"
	-@erase "$(OUTDIR)\snort.bsc"
	-@erase "$(OUTDIR)\snort.exe"
	-@erase "$(OUTDIR)\snort.ilk"
	-@erase "$(OUTDIR)\snort.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\snort.pch" /YX"snort.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\snort.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\acsmx.sbr" \
	"$(INTDIR)\byte_extract.sbr" \
	"$(INTDIR)\codes.sbr" \
	"$(INTDIR)\debug.sbr" \
	"$(INTDIR)\decode.sbr" \
	"$(INTDIR)\detect.sbr" \
	"$(INTDIR)\fpcreate.sbr" \
	"$(INTDIR)\fpdetect.sbr" \
	"$(INTDIR)\log.sbr" \
	"$(INTDIR)\mempool.sbr" \
	"$(INTDIR)\mpse.sbr" \
	"$(INTDIR)\mstring.sbr" \
	"$(INTDIR)\mwm.sbr" \
	"$(INTDIR)\parser.sbr" \
	"$(INTDIR)\pcrm.sbr" \
	"$(INTDIR)\plugbase.sbr" \
	"$(INTDIR)\sf_sdlist.sbr" \
	"$(INTDIR)\sfksearch.sbr" \
	"$(INTDIR)\signature.sbr" \
	"$(INTDIR)\snort.sbr" \
	"$(INTDIR)\snprintf.sbr" \
	"$(INTDIR)\strlcatu.sbr" \
	"$(INTDIR)\strlcpyu.sbr" \
	"$(INTDIR)\tag.sbr" \
	"$(INTDIR)\threshold.sbr" \
	"$(INTDIR)\ubi_BinTree.sbr" \
	"$(INTDIR)\ubi_SplayTree.sbr" \
	"$(INTDIR)\util.sbr" \
	"$(INTDIR)\getopt.sbr" \
	"$(INTDIR)\misc.sbr" \
	"$(INTDIR)\syslog.sbr" \
	"$(INTDIR)\win32_service.sbr" \
	"$(INTDIR)\sp_byte_check.sbr" \
	"$(INTDIR)\sp_byte_jump.sbr" \
	"$(INTDIR)\sp_clientserver.sbr" \
	"$(INTDIR)\sp_dsize_check.sbr" \
	"$(INTDIR)\sp_icmp_code_check.sbr" \
	"$(INTDIR)\sp_icmp_id_check.sbr" \
	"$(INTDIR)\sp_icmp_seq_check.sbr" \
	"$(INTDIR)\sp_icmp_type_check.sbr" \
	"$(INTDIR)\sp_ip_fragbits.sbr" \
	"$(INTDIR)\sp_ip_id_check.sbr" \
	"$(INTDIR)\sp_ip_proto.sbr" \
	"$(INTDIR)\sp_ip_same_check.sbr" \
	"$(INTDIR)\sp_ip_tos_check.sbr" \
	"$(INTDIR)\sp_ipoption_check.sbr" \
	"$(INTDIR)\sp_pattern_match.sbr" \
	"$(INTDIR)\sp_react.sbr" \
	"$(INTDIR)\sp_respond.sbr" \
	"$(INTDIR)\sp_rpc_check.sbr" \
	"$(INTDIR)\sp_session.sbr" \
	"$(INTDIR)\sp_tcp_ack_check.sbr" \
	"$(INTDIR)\sp_tcp_flag_check.sbr" \
	"$(INTDIR)\sp_tcp_seq_check.sbr" \
	"$(INTDIR)\sp_tcp_win_check.sbr" \
	"$(INTDIR)\sp_ttl_check.sbr" \
	"$(INTDIR)\spo_alert_fast.sbr" \
	"$(INTDIR)\spo_alert_full.sbr" \
	"$(INTDIR)\spo_alert_sf_socket.sbr" \
	"$(INTDIR)\spo_alert_smb.sbr" \
	"$(INTDIR)\spo_alert_syslog.sbr" \
	"$(INTDIR)\spo_alert_unixsock.sbr" \
	"$(INTDIR)\spo_csv.sbr" \
	"$(INTDIR)\spo_database.sbr" \
	"$(INTDIR)\spo_log_ascii.sbr" \
	"$(INTDIR)\spo_log_null.sbr" \
	"$(INTDIR)\spo_log_tcpdump.sbr" \
	"$(INTDIR)\spo_unified.sbr" \
	"$(INTDIR)\http-resp.sbr" \
	"$(INTDIR)\perf-base.sbr" \
	"$(INTDIR)\perf-event.sbr" \
	"$(INTDIR)\perf-flow.sbr" \
	"$(INTDIR)\perf.sbr" \
	"$(INTDIR)\sfprocpidstats.sbr" \
	"$(INTDIR)\spp_arpspoof.sbr" \
	"$(INTDIR)\spp_bo.sbr" \
	"$(INTDIR)\spp_conversation.sbr" \
	"$(INTDIR)\spp_frag2.sbr" \
	"$(INTDIR)\spp_http_decode.sbr" \
	"$(INTDIR)\spp_httpflow.sbr" \
	"$(INTDIR)\spp_perfmonitor.sbr" \
	"$(INTDIR)\spp_portscan.sbr" \
	"$(INTDIR)\spp_portscan2.sbr" \
	"$(INTDIR)\spp_rpc_decode.sbr" \
	"$(INTDIR)\spp_stream4.sbr" \
	"$(INTDIR)\spp_telnet_negotiation.sbr" \
	"$(INTDIR)\IpAddrSet.sbr"

"$(OUTDIR)\snort.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\snort.pdb" /debug /machine:I386 /out:"$(OUTDIR)\snort.exe" /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" 
LINK32_OBJS= \
	"$(INTDIR)\acsmx.obj" \
	"$(INTDIR)\byte_extract.obj" \
	"$(INTDIR)\codes.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\decode.obj" \
	"$(INTDIR)\detect.obj" \
	"$(INTDIR)\fpcreate.obj" \
	"$(INTDIR)\fpdetect.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\mempool.obj" \
	"$(INTDIR)\mpse.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\mwm.obj" \
	"$(INTDIR)\parser.obj" \
	"$(INTDIR)\pcrm.obj" \
	"$(INTDIR)\plugbase.obj" \
	"$(INTDIR)\sf_sdlist.obj" \
	"$(INTDIR)\sfksearch.obj" \
	"$(INTDIR)\signature.obj" \
	"$(INTDIR)\snort.obj" \
	"$(INTDIR)\snprintf.obj" \
	"$(INTDIR)\strlcatu.obj" \
	"$(INTDIR)\strlcpyu.obj" \
	"$(INTDIR)\tag.obj" \
	"$(INTDIR)\threshold.obj" \
	"$(INTDIR)\ubi_BinTree.obj" \
	"$(INTDIR)\ubi_SplayTree.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\syslog.obj" \
	"$(INTDIR)\win32_service.obj" \
	"$(INTDIR)\sp_byte_check.obj" \
	"$(INTDIR)\sp_byte_jump.obj" \
	"$(INTDIR)\sp_clientserver.obj" \
	"$(INTDIR)\sp_dsize_check.obj" \
	"$(INTDIR)\sp_icmp_code_check.obj" \
	"$(INTDIR)\sp_icmp_id_check.obj" \
	"$(INTDIR)\sp_icmp_seq_check.obj" \
	"$(INTDIR)\sp_icmp_type_check.obj" \
	"$(INTDIR)\sp_ip_fragbits.obj" \
	"$(INTDIR)\sp_ip_id_check.obj" \
	"$(INTDIR)\sp_ip_proto.obj" \
	"$(INTDIR)\sp_ip_same_check.obj" \
	"$(INTDIR)\sp_ip_tos_check.obj" \
	"$(INTDIR)\sp_ipoption_check.obj" \
	"$(INTDIR)\sp_pattern_match.obj" \
	"$(INTDIR)\sp_react.obj" \
	"$(INTDIR)\sp_respond.obj" \
	"$(INTDIR)\sp_rpc_check.obj" \
	"$(INTDIR)\sp_session.obj" \
	"$(INTDIR)\sp_tcp_ack_check.obj" \
	"$(INTDIR)\sp_tcp_flag_check.obj" \
	"$(INTDIR)\sp_tcp_seq_check.obj" \
	"$(INTDIR)\sp_tcp_win_check.obj" \
	"$(INTDIR)\sp_ttl_check.obj" \
	"$(INTDIR)\spo_alert_fast.obj" \
	"$(INTDIR)\spo_alert_full.obj" \
	"$(INTDIR)\spo_alert_sf_socket.obj" \
	"$(INTDIR)\spo_alert_smb.obj" \
	"$(INTDIR)\spo_alert_syslog.obj" \
	"$(INTDIR)\spo_alert_unixsock.obj" \
	"$(INTDIR)\spo_csv.obj" \
	"$(INTDIR)\spo_database.obj" \
	"$(INTDIR)\spo_log_ascii.obj" \
	"$(INTDIR)\spo_log_null.obj" \
	"$(INTDIR)\spo_log_tcpdump.obj" \
	"$(INTDIR)\spo_unified.obj" \
	"$(INTDIR)\http-resp.obj" \
	"$(INTDIR)\perf-base.obj" \
	"$(INTDIR)\perf-event.obj" \
	"$(INTDIR)\perf-flow.obj" \
	"$(INTDIR)\perf.obj" \
	"$(INTDIR)\sfprocpidstats.obj" \
	"$(INTDIR)\spp_arpspoof.obj" \
	"$(INTDIR)\spp_bo.obj" \
	"$(INTDIR)\spp_conversation.obj" \
	"$(INTDIR)\spp_frag2.obj" \
	"$(INTDIR)\spp_http_decode.obj" \
	"$(INTDIR)\spp_httpflow.obj" \
	"$(INTDIR)\spp_perfmonitor.obj" \
	"$(INTDIR)\spp_portscan.obj" \
	"$(INTDIR)\spp_portscan2.obj" \
	"$(INTDIR)\spp_rpc_decode.obj" \
	"$(INTDIR)\spp_stream4.obj" \
	"$(INTDIR)\spp_telnet_negotiation.obj" \
	"$(INTDIR)\IpAddrSet.obj"

"$(OUTDIR)\snort.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"

OUTDIR=.\snort___Win32_StdDB_MSSQL_Release
INTDIR=.\snort___Win32_StdDB_MSSQL_Release
# Begin Custom Macros
OutDir=.\snort___Win32_StdDB_MSSQL_Release
# End Custom Macros

ALL : "$(OUTDIR)\snort.exe"


CLEAN :
	-@erase "$(INTDIR)\acsmx.obj"
	-@erase "$(INTDIR)\byte_extract.obj"
	-@erase "$(INTDIR)\codes.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\decode.obj"
	-@erase "$(INTDIR)\detect.obj"
	-@erase "$(INTDIR)\fpcreate.obj"
	-@erase "$(INTDIR)\fpdetect.obj"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\http-resp.obj"
	-@erase "$(INTDIR)\IpAddrSet.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\mempool.obj"
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\mpse.obj"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\mwm.obj"
	-@erase "$(INTDIR)\parser.obj"
	-@erase "$(INTDIR)\pcrm.obj"
	-@erase "$(INTDIR)\perf-base.obj"
	-@erase "$(INTDIR)\perf-event.obj"
	-@erase "$(INTDIR)\perf-flow.obj"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\plugbase.obj"
	-@erase "$(INTDIR)\sf_sdlist.obj"
	-@erase "$(INTDIR)\sfksearch.obj"
	-@erase "$(INTDIR)\sfprocpidstats.obj"
	-@erase "$(INTDIR)\signature.obj"
	-@erase "$(INTDIR)\snort.obj"
	-@erase "$(INTDIR)\snprintf.obj"
	-@erase "$(INTDIR)\sp_byte_check.obj"
	-@erase "$(INTDIR)\sp_byte_jump.obj"
	-@erase "$(INTDIR)\sp_clientserver.obj"
	-@erase "$(INTDIR)\sp_dsize_check.obj"
	-@erase "$(INTDIR)\sp_icmp_code_check.obj"
	-@erase "$(INTDIR)\sp_icmp_id_check.obj"
	-@erase "$(INTDIR)\sp_icmp_seq_check.obj"
	-@erase "$(INTDIR)\sp_icmp_type_check.obj"
	-@erase "$(INTDIR)\sp_ip_fragbits.obj"
	-@erase "$(INTDIR)\sp_ip_id_check.obj"
	-@erase "$(INTDIR)\sp_ip_proto.obj"
	-@erase "$(INTDIR)\sp_ip_same_check.obj"
	-@erase "$(INTDIR)\sp_ip_tos_check.obj"
	-@erase "$(INTDIR)\sp_ipoption_check.obj"
	-@erase "$(INTDIR)\sp_pattern_match.obj"
	-@erase "$(INTDIR)\sp_react.obj"
	-@erase "$(INTDIR)\sp_respond.obj"
	-@erase "$(INTDIR)\sp_rpc_check.obj"
	-@erase "$(INTDIR)\sp_session.obj"
	-@erase "$(INTDIR)\sp_tcp_ack_check.obj"
	-@erase "$(INTDIR)\sp_tcp_flag_check.obj"
	-@erase "$(INTDIR)\sp_tcp_seq_check.obj"
	-@erase "$(INTDIR)\sp_tcp_win_check.obj"
	-@erase "$(INTDIR)\sp_ttl_check.obj"
	-@erase "$(INTDIR)\spo_alert_fast.obj"
	-@erase "$(INTDIR)\spo_alert_full.obj"
	-@erase "$(INTDIR)\spo_alert_sf_socket.obj"
	-@erase "$(INTDIR)\spo_alert_smb.obj"
	-@erase "$(INTDIR)\spo_alert_syslog.obj"
	-@erase "$(INTDIR)\spo_alert_unixsock.obj"
	-@erase "$(INTDIR)\spo_csv.obj"
	-@erase "$(INTDIR)\spo_database.obj"
	-@erase "$(INTDIR)\spo_log_ascii.obj"
	-@erase "$(INTDIR)\spo_log_null.obj"
	-@erase "$(INTDIR)\spo_log_tcpdump.obj"
	-@erase "$(INTDIR)\spo_unified.obj"
	-@erase "$(INTDIR)\spp_arpspoof.obj"
	-@erase "$(INTDIR)\spp_bo.obj"
	-@erase "$(INTDIR)\spp_conversation.obj"
	-@erase "$(INTDIR)\spp_frag2.obj"
	-@erase "$(INTDIR)\spp_http_decode.obj"
	-@erase "$(INTDIR)\spp_httpflow.obj"
	-@erase "$(INTDIR)\spp_perfmonitor.obj"
	-@erase "$(INTDIR)\spp_portscan.obj"
	-@erase "$(INTDIR)\spp_portscan2.obj"
	-@erase "$(INTDIR)\spp_rpc_decode.obj"
	-@erase "$(INTDIR)\spp_stream4.obj"
	-@erase "$(INTDIR)\spp_telnet_negotiation.obj"
	-@erase "$(INTDIR)\strlcatu.obj"
	-@erase "$(INTDIR)\strlcpyu.obj"
	-@erase "$(INTDIR)\syslog.obj"
	-@erase "$(INTDIR)\tag.obj"
	-@erase "$(INTDIR)\threshold.obj"
	-@erase "$(INTDIR)\ubi_BinTree.obj"
	-@erase "$(INTDIR)\ubi_SplayTree.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\win32_service.obj"
	-@erase "$(OUTDIR)\snort.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /Fp"$(INTDIR)\snort.pch" /YX"snort.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\snort.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\snort.pdb" /machine:I386 /out:"$(OUTDIR)\snort.exe" /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" 
LINK32_OBJS= \
	"$(INTDIR)\acsmx.obj" \
	"$(INTDIR)\byte_extract.obj" \
	"$(INTDIR)\codes.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\decode.obj" \
	"$(INTDIR)\detect.obj" \
	"$(INTDIR)\fpcreate.obj" \
	"$(INTDIR)\fpdetect.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\mempool.obj" \
	"$(INTDIR)\mpse.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\mwm.obj" \
	"$(INTDIR)\parser.obj" \
	"$(INTDIR)\pcrm.obj" \
	"$(INTDIR)\plugbase.obj" \
	"$(INTDIR)\sf_sdlist.obj" \
	"$(INTDIR)\sfksearch.obj" \
	"$(INTDIR)\signature.obj" \
	"$(INTDIR)\snort.obj" \
	"$(INTDIR)\snprintf.obj" \
	"$(INTDIR)\strlcatu.obj" \
	"$(INTDIR)\strlcpyu.obj" \
	"$(INTDIR)\tag.obj" \
	"$(INTDIR)\threshold.obj" \
	"$(INTDIR)\ubi_BinTree.obj" \
	"$(INTDIR)\ubi_SplayTree.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\syslog.obj" \
	"$(INTDIR)\win32_service.obj" \
	"$(INTDIR)\sp_byte_check.obj" \
	"$(INTDIR)\sp_byte_jump.obj" \
	"$(INTDIR)\sp_clientserver.obj" \
	"$(INTDIR)\sp_dsize_check.obj" \
	"$(INTDIR)\sp_icmp_code_check.obj" \
	"$(INTDIR)\sp_icmp_id_check.obj" \
	"$(INTDIR)\sp_icmp_seq_check.obj" \
	"$(INTDIR)\sp_icmp_type_check.obj" \
	"$(INTDIR)\sp_ip_fragbits.obj" \
	"$(INTDIR)\sp_ip_id_check.obj" \
	"$(INTDIR)\sp_ip_proto.obj" \
	"$(INTDIR)\sp_ip_same_check.obj" \
	"$(INTDIR)\sp_ip_tos_check.obj" \
	"$(INTDIR)\sp_ipoption_check.obj" \
	"$(INTDIR)\sp_pattern_match.obj" \
	"$(INTDIR)\sp_react.obj" \
	"$(INTDIR)\sp_respond.obj" \
	"$(INTDIR)\sp_rpc_check.obj" \
	"$(INTDIR)\sp_session.obj" \
	"$(INTDIR)\sp_tcp_ack_check.obj" \
	"$(INTDIR)\sp_tcp_flag_check.obj" \
	"$(INTDIR)\sp_tcp_seq_check.obj" \
	"$(INTDIR)\sp_tcp_win_check.obj" \
	"$(INTDIR)\sp_ttl_check.obj" \
	"$(INTDIR)\spo_alert_fast.obj" \
	"$(INTDIR)\spo_alert_full.obj" \
	"$(INTDIR)\spo_alert_sf_socket.obj" \
	"$(INTDIR)\spo_alert_smb.obj" \
	"$(INTDIR)\spo_alert_syslog.obj" \
	"$(INTDIR)\spo_alert_unixsock.obj" \
	"$(INTDIR)\spo_csv.obj" \
	"$(INTDIR)\spo_database.obj" \
	"$(INTDIR)\spo_log_ascii.obj" \
	"$(INTDIR)\spo_log_null.obj" \
	"$(INTDIR)\spo_log_tcpdump.obj" \
	"$(INTDIR)\spo_unified.obj" \
	"$(INTDIR)\http-resp.obj" \
	"$(INTDIR)\perf-base.obj" \
	"$(INTDIR)\perf-event.obj" \
	"$(INTDIR)\perf-flow.obj" \
	"$(INTDIR)\perf.obj" \
	"$(INTDIR)\sfprocpidstats.obj" \
	"$(INTDIR)\spp_arpspoof.obj" \
	"$(INTDIR)\spp_bo.obj" \
	"$(INTDIR)\spp_conversation.obj" \
	"$(INTDIR)\spp_frag2.obj" \
	"$(INTDIR)\spp_http_decode.obj" \
	"$(INTDIR)\spp_httpflow.obj" \
	"$(INTDIR)\spp_perfmonitor.obj" \
	"$(INTDIR)\spp_portscan.obj" \
	"$(INTDIR)\spp_portscan2.obj" \
	"$(INTDIR)\spp_rpc_decode.obj" \
	"$(INTDIR)\spp_stream4.obj" \
	"$(INTDIR)\spp_telnet_negotiation.obj" \
	"$(INTDIR)\IpAddrSet.obj"

"$(OUTDIR)\snort.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"

OUTDIR=.\snort___Win32_StdDB_MSSQL_Service_Debug
INTDIR=.\snort___Win32_StdDB_MSSQL_Service_Debug
# Begin Custom Macros
OutDir=.\snort___Win32_StdDB_MSSQL_Service_Debug
# End Custom Macros

ALL : "$(OUTDIR)\snort.exe" "$(OUTDIR)\snort.bsc"


CLEAN :
	-@erase "$(INTDIR)\acsmx.obj"
	-@erase "$(INTDIR)\acsmx.sbr"
	-@erase "$(INTDIR)\byte_extract.obj"
	-@erase "$(INTDIR)\byte_extract.sbr"
	-@erase "$(INTDIR)\codes.obj"
	-@erase "$(INTDIR)\codes.sbr"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\debug.sbr"
	-@erase "$(INTDIR)\decode.obj"
	-@erase "$(INTDIR)\decode.sbr"
	-@erase "$(INTDIR)\detect.obj"
	-@erase "$(INTDIR)\detect.sbr"
	-@erase "$(INTDIR)\fpcreate.obj"
	-@erase "$(INTDIR)\fpcreate.sbr"
	-@erase "$(INTDIR)\fpdetect.obj"
	-@erase "$(INTDIR)\fpdetect.sbr"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\getopt.sbr"
	-@erase "$(INTDIR)\http-resp.obj"
	-@erase "$(INTDIR)\http-resp.sbr"
	-@erase "$(INTDIR)\IpAddrSet.obj"
	-@erase "$(INTDIR)\IpAddrSet.sbr"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\log.sbr"
	-@erase "$(INTDIR)\mempool.obj"
	-@erase "$(INTDIR)\mempool.sbr"
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\misc.sbr"
	-@erase "$(INTDIR)\mpse.obj"
	-@erase "$(INTDIR)\mpse.sbr"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\mstring.sbr"
	-@erase "$(INTDIR)\mwm.obj"
	-@erase "$(INTDIR)\mwm.sbr"
	-@erase "$(INTDIR)\parser.obj"
	-@erase "$(INTDIR)\parser.sbr"
	-@erase "$(INTDIR)\pcrm.obj"
	-@erase "$(INTDIR)\pcrm.sbr"
	-@erase "$(INTDIR)\perf-base.obj"
	-@erase "$(INTDIR)\perf-base.sbr"
	-@erase "$(INTDIR)\perf-event.obj"
	-@erase "$(INTDIR)\perf-event.sbr"
	-@erase "$(INTDIR)\perf-flow.obj"
	-@erase "$(INTDIR)\perf-flow.sbr"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\perf.sbr"
	-@erase "$(INTDIR)\plugbase.obj"
	-@erase "$(INTDIR)\plugbase.sbr"
	-@erase "$(INTDIR)\sf_sdlist.obj"
	-@erase "$(INTDIR)\sf_sdlist.sbr"
	-@erase "$(INTDIR)\sfksearch.obj"
	-@erase "$(INTDIR)\sfksearch.sbr"
	-@erase "$(INTDIR)\sfprocpidstats.obj"
	-@erase "$(INTDIR)\sfprocpidstats.sbr"
	-@erase "$(INTDIR)\signature.obj"
	-@erase "$(INTDIR)\signature.sbr"
	-@erase "$(INTDIR)\snort.obj"
	-@erase "$(INTDIR)\snort.sbr"
	-@erase "$(INTDIR)\snprintf.obj"
	-@erase "$(INTDIR)\snprintf.sbr"
	-@erase "$(INTDIR)\sp_byte_check.obj"
	-@erase "$(INTDIR)\sp_byte_check.sbr"
	-@erase "$(INTDIR)\sp_byte_jump.obj"
	-@erase "$(INTDIR)\sp_byte_jump.sbr"
	-@erase "$(INTDIR)\sp_clientserver.obj"
	-@erase "$(INTDIR)\sp_clientserver.sbr"
	-@erase "$(INTDIR)\sp_dsize_check.obj"
	-@erase "$(INTDIR)\sp_dsize_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_code_check.obj"
	-@erase "$(INTDIR)\sp_icmp_code_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_id_check.obj"
	-@erase "$(INTDIR)\sp_icmp_id_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_seq_check.obj"
	-@erase "$(INTDIR)\sp_icmp_seq_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_type_check.obj"
	-@erase "$(INTDIR)\sp_icmp_type_check.sbr"
	-@erase "$(INTDIR)\sp_ip_fragbits.obj"
	-@erase "$(INTDIR)\sp_ip_fragbits.sbr"
	-@erase "$(INTDIR)\sp_ip_id_check.obj"
	-@erase "$(INTDIR)\sp_ip_id_check.sbr"
	-@erase "$(INTDIR)\sp_ip_proto.obj"
	-@erase "$(INTDIR)\sp_ip_proto.sbr"
	-@erase "$(INTDIR)\sp_ip_same_check.obj"
	-@erase "$(INTDIR)\sp_ip_same_check.sbr"
	-@erase "$(INTDIR)\sp_ip_tos_check.obj"
	-@erase "$(INTDIR)\sp_ip_tos_check.sbr"
	-@erase "$(INTDIR)\sp_ipoption_check.obj"
	-@erase "$(INTDIR)\sp_ipoption_check.sbr"
	-@erase "$(INTDIR)\sp_pattern_match.obj"
	-@erase "$(INTDIR)\sp_pattern_match.sbr"
	-@erase "$(INTDIR)\sp_react.obj"
	-@erase "$(INTDIR)\sp_react.sbr"
	-@erase "$(INTDIR)\sp_respond.obj"
	-@erase "$(INTDIR)\sp_respond.sbr"
	-@erase "$(INTDIR)\sp_rpc_check.obj"
	-@erase "$(INTDIR)\sp_rpc_check.sbr"
	-@erase "$(INTDIR)\sp_session.obj"
	-@erase "$(INTDIR)\sp_session.sbr"
	-@erase "$(INTDIR)\sp_tcp_ack_check.obj"
	-@erase "$(INTDIR)\sp_tcp_ack_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_flag_check.obj"
	-@erase "$(INTDIR)\sp_tcp_flag_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_seq_check.obj"
	-@erase "$(INTDIR)\sp_tcp_seq_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_win_check.obj"
	-@erase "$(INTDIR)\sp_tcp_win_check.sbr"
	-@erase "$(INTDIR)\sp_ttl_check.obj"
	-@erase "$(INTDIR)\sp_ttl_check.sbr"
	-@erase "$(INTDIR)\spo_alert_fast.obj"
	-@erase "$(INTDIR)\spo_alert_fast.sbr"
	-@erase "$(INTDIR)\spo_alert_full.obj"
	-@erase "$(INTDIR)\spo_alert_full.sbr"
	-@erase "$(INTDIR)\spo_alert_sf_socket.obj"
	-@erase "$(INTDIR)\spo_alert_sf_socket.sbr"
	-@erase "$(INTDIR)\spo_alert_smb.obj"
	-@erase "$(INTDIR)\spo_alert_smb.sbr"
	-@erase "$(INTDIR)\spo_alert_syslog.obj"
	-@erase "$(INTDIR)\spo_alert_syslog.sbr"
	-@erase "$(INTDIR)\spo_alert_unixsock.obj"
	-@erase "$(INTDIR)\spo_alert_unixsock.sbr"
	-@erase "$(INTDIR)\spo_csv.obj"
	-@erase "$(INTDIR)\spo_csv.sbr"
	-@erase "$(INTDIR)\spo_database.obj"
	-@erase "$(INTDIR)\spo_database.sbr"
	-@erase "$(INTDIR)\spo_log_ascii.obj"
	-@erase "$(INTDIR)\spo_log_ascii.sbr"
	-@erase "$(INTDIR)\spo_log_null.obj"
	-@erase "$(INTDIR)\spo_log_null.sbr"
	-@erase "$(INTDIR)\spo_log_tcpdump.obj"
	-@erase "$(INTDIR)\spo_log_tcpdump.sbr"
	-@erase "$(INTDIR)\spo_unified.obj"
	-@erase "$(INTDIR)\spo_unified.sbr"
	-@erase "$(INTDIR)\spp_arpspoof.obj"
	-@erase "$(INTDIR)\spp_arpspoof.sbr"
	-@erase "$(INTDIR)\spp_bo.obj"
	-@erase "$(INTDIR)\spp_bo.sbr"
	-@erase "$(INTDIR)\spp_conversation.obj"
	-@erase "$(INTDIR)\spp_conversation.sbr"
	-@erase "$(INTDIR)\spp_frag2.obj"
	-@erase "$(INTDIR)\spp_frag2.sbr"
	-@erase "$(INTDIR)\spp_http_decode.obj"
	-@erase "$(INTDIR)\spp_http_decode.sbr"
	-@erase "$(INTDIR)\spp_httpflow.obj"
	-@erase "$(INTDIR)\spp_httpflow.sbr"
	-@erase "$(INTDIR)\spp_perfmonitor.obj"
	-@erase "$(INTDIR)\spp_perfmonitor.sbr"
	-@erase "$(INTDIR)\spp_portscan.obj"
	-@erase "$(INTDIR)\spp_portscan.sbr"
	-@erase "$(INTDIR)\spp_portscan2.obj"
	-@erase "$(INTDIR)\spp_portscan2.sbr"
	-@erase "$(INTDIR)\spp_rpc_decode.obj"
	-@erase "$(INTDIR)\spp_rpc_decode.sbr"
	-@erase "$(INTDIR)\spp_stream4.obj"
	-@erase "$(INTDIR)\spp_stream4.sbr"
	-@erase "$(INTDIR)\spp_telnet_negotiation.obj"
	-@erase "$(INTDIR)\spp_telnet_negotiation.sbr"
	-@erase "$(INTDIR)\strlcatu.obj"
	-@erase "$(INTDIR)\strlcatu.sbr"
	-@erase "$(INTDIR)\strlcpyu.obj"
	-@erase "$(INTDIR)\strlcpyu.sbr"
	-@erase "$(INTDIR)\syslog.obj"
	-@erase "$(INTDIR)\syslog.sbr"
	-@erase "$(INTDIR)\tag.obj"
	-@erase "$(INTDIR)\tag.sbr"
	-@erase "$(INTDIR)\threshold.obj"
	-@erase "$(INTDIR)\threshold.sbr"
	-@erase "$(INTDIR)\ubi_BinTree.obj"
	-@erase "$(INTDIR)\ubi_BinTree.sbr"
	-@erase "$(INTDIR)\ubi_SplayTree.obj"
	-@erase "$(INTDIR)\ubi_SplayTree.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\win32_service.obj"
	-@erase "$(INTDIR)\win32_service.sbr"
	-@erase "$(OUTDIR)\snort.bsc"
	-@erase "$(OUTDIR)\snort.exe"
	-@erase "$(OUTDIR)\snort.ilk"
	-@erase "$(OUTDIR)\snort.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_WIN32_SERVICE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\snort.pch" /YX"snort.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\snort.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\acsmx.sbr" \
	"$(INTDIR)\byte_extract.sbr" \
	"$(INTDIR)\codes.sbr" \
	"$(INTDIR)\debug.sbr" \
	"$(INTDIR)\decode.sbr" \
	"$(INTDIR)\detect.sbr" \
	"$(INTDIR)\fpcreate.sbr" \
	"$(INTDIR)\fpdetect.sbr" \
	"$(INTDIR)\log.sbr" \
	"$(INTDIR)\mempool.sbr" \
	"$(INTDIR)\mpse.sbr" \
	"$(INTDIR)\mstring.sbr" \
	"$(INTDIR)\mwm.sbr" \
	"$(INTDIR)\parser.sbr" \
	"$(INTDIR)\pcrm.sbr" \
	"$(INTDIR)\plugbase.sbr" \
	"$(INTDIR)\sf_sdlist.sbr" \
	"$(INTDIR)\sfksearch.sbr" \
	"$(INTDIR)\signature.sbr" \
	"$(INTDIR)\snort.sbr" \
	"$(INTDIR)\snprintf.sbr" \
	"$(INTDIR)\strlcatu.sbr" \
	"$(INTDIR)\strlcpyu.sbr" \
	"$(INTDIR)\tag.sbr" \
	"$(INTDIR)\threshold.sbr" \
	"$(INTDIR)\ubi_BinTree.sbr" \
	"$(INTDIR)\ubi_SplayTree.sbr" \
	"$(INTDIR)\util.sbr" \
	"$(INTDIR)\getopt.sbr" \
	"$(INTDIR)\misc.sbr" \
	"$(INTDIR)\syslog.sbr" \
	"$(INTDIR)\win32_service.sbr" \
	"$(INTDIR)\sp_byte_check.sbr" \
	"$(INTDIR)\sp_byte_jump.sbr" \
	"$(INTDIR)\sp_clientserver.sbr" \
	"$(INTDIR)\sp_dsize_check.sbr" \
	"$(INTDIR)\sp_icmp_code_check.sbr" \
	"$(INTDIR)\sp_icmp_id_check.sbr" \
	"$(INTDIR)\sp_icmp_seq_check.sbr" \
	"$(INTDIR)\sp_icmp_type_check.sbr" \
	"$(INTDIR)\sp_ip_fragbits.sbr" \
	"$(INTDIR)\sp_ip_id_check.sbr" \
	"$(INTDIR)\sp_ip_proto.sbr" \
	"$(INTDIR)\sp_ip_same_check.sbr" \
	"$(INTDIR)\sp_ip_tos_check.sbr" \
	"$(INTDIR)\sp_ipoption_check.sbr" \
	"$(INTDIR)\sp_pattern_match.sbr" \
	"$(INTDIR)\sp_react.sbr" \
	"$(INTDIR)\sp_respond.sbr" \
	"$(INTDIR)\sp_rpc_check.sbr" \
	"$(INTDIR)\sp_session.sbr" \
	"$(INTDIR)\sp_tcp_ack_check.sbr" \
	"$(INTDIR)\sp_tcp_flag_check.sbr" \
	"$(INTDIR)\sp_tcp_seq_check.sbr" \
	"$(INTDIR)\sp_tcp_win_check.sbr" \
	"$(INTDIR)\sp_ttl_check.sbr" \
	"$(INTDIR)\spo_alert_fast.sbr" \
	"$(INTDIR)\spo_alert_full.sbr" \
	"$(INTDIR)\spo_alert_sf_socket.sbr" \
	"$(INTDIR)\spo_alert_smb.sbr" \
	"$(INTDIR)\spo_alert_syslog.sbr" \
	"$(INTDIR)\spo_alert_unixsock.sbr" \
	"$(INTDIR)\spo_csv.sbr" \
	"$(INTDIR)\spo_database.sbr" \
	"$(INTDIR)\spo_log_ascii.sbr" \
	"$(INTDIR)\spo_log_null.sbr" \
	"$(INTDIR)\spo_log_tcpdump.sbr" \
	"$(INTDIR)\spo_unified.sbr" \
	"$(INTDIR)\http-resp.sbr" \
	"$(INTDIR)\perf-base.sbr" \
	"$(INTDIR)\perf-event.sbr" \
	"$(INTDIR)\perf-flow.sbr" \
	"$(INTDIR)\perf.sbr" \
	"$(INTDIR)\sfprocpidstats.sbr" \
	"$(INTDIR)\spp_arpspoof.sbr" \
	"$(INTDIR)\spp_bo.sbr" \
	"$(INTDIR)\spp_conversation.sbr" \
	"$(INTDIR)\spp_frag2.sbr" \
	"$(INTDIR)\spp_http_decode.sbr" \
	"$(INTDIR)\spp_httpflow.sbr" \
	"$(INTDIR)\spp_perfmonitor.sbr" \
	"$(INTDIR)\spp_portscan.sbr" \
	"$(INTDIR)\spp_portscan2.sbr" \
	"$(INTDIR)\spp_rpc_decode.sbr" \
	"$(INTDIR)\spp_stream4.sbr" \
	"$(INTDIR)\spp_telnet_negotiation.sbr" \
	"$(INTDIR)\IpAddrSet.sbr"

"$(OUTDIR)\snort.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\snort.pdb" /debug /machine:I386 /out:"$(OUTDIR)\snort.exe" /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" 
LINK32_OBJS= \
	"$(INTDIR)\acsmx.obj" \
	"$(INTDIR)\byte_extract.obj" \
	"$(INTDIR)\codes.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\decode.obj" \
	"$(INTDIR)\detect.obj" \
	"$(INTDIR)\fpcreate.obj" \
	"$(INTDIR)\fpdetect.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\mempool.obj" \
	"$(INTDIR)\mpse.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\mwm.obj" \
	"$(INTDIR)\parser.obj" \
	"$(INTDIR)\pcrm.obj" \
	"$(INTDIR)\plugbase.obj" \
	"$(INTDIR)\sf_sdlist.obj" \
	"$(INTDIR)\sfksearch.obj" \
	"$(INTDIR)\signature.obj" \
	"$(INTDIR)\snort.obj" \
	"$(INTDIR)\snprintf.obj" \
	"$(INTDIR)\strlcatu.obj" \
	"$(INTDIR)\strlcpyu.obj" \
	"$(INTDIR)\tag.obj" \
	"$(INTDIR)\threshold.obj" \
	"$(INTDIR)\ubi_BinTree.obj" \
	"$(INTDIR)\ubi_SplayTree.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\syslog.obj" \
	"$(INTDIR)\win32_service.obj" \
	"$(INTDIR)\sp_byte_check.obj" \
	"$(INTDIR)\sp_byte_jump.obj" \
	"$(INTDIR)\sp_clientserver.obj" \
	"$(INTDIR)\sp_dsize_check.obj" \
	"$(INTDIR)\sp_icmp_code_check.obj" \
	"$(INTDIR)\sp_icmp_id_check.obj" \
	"$(INTDIR)\sp_icmp_seq_check.obj" \
	"$(INTDIR)\sp_icmp_type_check.obj" \
	"$(INTDIR)\sp_ip_fragbits.obj" \
	"$(INTDIR)\sp_ip_id_check.obj" \
	"$(INTDIR)\sp_ip_proto.obj" \
	"$(INTDIR)\sp_ip_same_check.obj" \
	"$(INTDIR)\sp_ip_tos_check.obj" \
	"$(INTDIR)\sp_ipoption_check.obj" \
	"$(INTDIR)\sp_pattern_match.obj" \
	"$(INTDIR)\sp_react.obj" \
	"$(INTDIR)\sp_respond.obj" \
	"$(INTDIR)\sp_rpc_check.obj" \
	"$(INTDIR)\sp_session.obj" \
	"$(INTDIR)\sp_tcp_ack_check.obj" \
	"$(INTDIR)\sp_tcp_flag_check.obj" \
	"$(INTDIR)\sp_tcp_seq_check.obj" \
	"$(INTDIR)\sp_tcp_win_check.obj" \
	"$(INTDIR)\sp_ttl_check.obj" \
	"$(INTDIR)\spo_alert_fast.obj" \
	"$(INTDIR)\spo_alert_full.obj" \
	"$(INTDIR)\spo_alert_sf_socket.obj" \
	"$(INTDIR)\spo_alert_smb.obj" \
	"$(INTDIR)\spo_alert_syslog.obj" \
	"$(INTDIR)\spo_alert_unixsock.obj" \
	"$(INTDIR)\spo_csv.obj" \
	"$(INTDIR)\spo_database.obj" \
	"$(INTDIR)\spo_log_ascii.obj" \
	"$(INTDIR)\spo_log_null.obj" \
	"$(INTDIR)\spo_log_tcpdump.obj" \
	"$(INTDIR)\spo_unified.obj" \
	"$(INTDIR)\http-resp.obj" \
	"$(INTDIR)\perf-base.obj" \
	"$(INTDIR)\perf-event.obj" \
	"$(INTDIR)\perf-flow.obj" \
	"$(INTDIR)\perf.obj" \
	"$(INTDIR)\sfprocpidstats.obj" \
	"$(INTDIR)\spp_arpspoof.obj" \
	"$(INTDIR)\spp_bo.obj" \
	"$(INTDIR)\spp_conversation.obj" \
	"$(INTDIR)\spp_frag2.obj" \
	"$(INTDIR)\spp_http_decode.obj" \
	"$(INTDIR)\spp_httpflow.obj" \
	"$(INTDIR)\spp_perfmonitor.obj" \
	"$(INTDIR)\spp_portscan.obj" \
	"$(INTDIR)\spp_portscan2.obj" \
	"$(INTDIR)\spp_rpc_decode.obj" \
	"$(INTDIR)\spp_stream4.obj" \
	"$(INTDIR)\spp_telnet_negotiation.obj" \
	"$(INTDIR)\IpAddrSet.obj"

"$(OUTDIR)\snort.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"

OUTDIR=.\snort___Win32_StdDB_MSSQL_Service_Release
INTDIR=.\snort___Win32_StdDB_MSSQL_Service_Release
# Begin Custom Macros
OutDir=.\snort___Win32_StdDB_MSSQL_Service_Release
# End Custom Macros

ALL : "$(OUTDIR)\snort.exe"


CLEAN :
	-@erase "$(INTDIR)\acsmx.obj"
	-@erase "$(INTDIR)\byte_extract.obj"
	-@erase "$(INTDIR)\codes.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\decode.obj"
	-@erase "$(INTDIR)\detect.obj"
	-@erase "$(INTDIR)\fpcreate.obj"
	-@erase "$(INTDIR)\fpdetect.obj"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\http-resp.obj"
	-@erase "$(INTDIR)\IpAddrSet.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\mempool.obj"
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\mpse.obj"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\mwm.obj"
	-@erase "$(INTDIR)\parser.obj"
	-@erase "$(INTDIR)\pcrm.obj"
	-@erase "$(INTDIR)\perf-base.obj"
	-@erase "$(INTDIR)\perf-event.obj"
	-@erase "$(INTDIR)\perf-flow.obj"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\plugbase.obj"
	-@erase "$(INTDIR)\sf_sdlist.obj"
	-@erase "$(INTDIR)\sfksearch.obj"
	-@erase "$(INTDIR)\sfprocpidstats.obj"
	-@erase "$(INTDIR)\signature.obj"
	-@erase "$(INTDIR)\snort.obj"
	-@erase "$(INTDIR)\snprintf.obj"
	-@erase "$(INTDIR)\sp_byte_check.obj"
	-@erase "$(INTDIR)\sp_byte_jump.obj"
	-@erase "$(INTDIR)\sp_clientserver.obj"
	-@erase "$(INTDIR)\sp_dsize_check.obj"
	-@erase "$(INTDIR)\sp_icmp_code_check.obj"
	-@erase "$(INTDIR)\sp_icmp_id_check.obj"
	-@erase "$(INTDIR)\sp_icmp_seq_check.obj"
	-@erase "$(INTDIR)\sp_icmp_type_check.obj"
	-@erase "$(INTDIR)\sp_ip_fragbits.obj"
	-@erase "$(INTDIR)\sp_ip_id_check.obj"
	-@erase "$(INTDIR)\sp_ip_proto.obj"
	-@erase "$(INTDIR)\sp_ip_same_check.obj"
	-@erase "$(INTDIR)\sp_ip_tos_check.obj"
	-@erase "$(INTDIR)\sp_ipoption_check.obj"
	-@erase "$(INTDIR)\sp_pattern_match.obj"
	-@erase "$(INTDIR)\sp_react.obj"
	-@erase "$(INTDIR)\sp_respond.obj"
	-@erase "$(INTDIR)\sp_rpc_check.obj"
	-@erase "$(INTDIR)\sp_session.obj"
	-@erase "$(INTDIR)\sp_tcp_ack_check.obj"
	-@erase "$(INTDIR)\sp_tcp_flag_check.obj"
	-@erase "$(INTDIR)\sp_tcp_seq_check.obj"
	-@erase "$(INTDIR)\sp_tcp_win_check.obj"
	-@erase "$(INTDIR)\sp_ttl_check.obj"
	-@erase "$(INTDIR)\spo_alert_fast.obj"
	-@erase "$(INTDIR)\spo_alert_full.obj"
	-@erase "$(INTDIR)\spo_alert_sf_socket.obj"
	-@erase "$(INTDIR)\spo_alert_smb.obj"
	-@erase "$(INTDIR)\spo_alert_syslog.obj"
	-@erase "$(INTDIR)\spo_alert_unixsock.obj"
	-@erase "$(INTDIR)\spo_csv.obj"
	-@erase "$(INTDIR)\spo_database.obj"
	-@erase "$(INTDIR)\spo_log_ascii.obj"
	-@erase "$(INTDIR)\spo_log_null.obj"
	-@erase "$(INTDIR)\spo_log_tcpdump.obj"
	-@erase "$(INTDIR)\spo_unified.obj"
	-@erase "$(INTDIR)\spp_arpspoof.obj"
	-@erase "$(INTDIR)\spp_bo.obj"
	-@erase "$(INTDIR)\spp_conversation.obj"
	-@erase "$(INTDIR)\spp_frag2.obj"
	-@erase "$(INTDIR)\spp_http_decode.obj"
	-@erase "$(INTDIR)\spp_httpflow.obj"
	-@erase "$(INTDIR)\spp_perfmonitor.obj"
	-@erase "$(INTDIR)\spp_portscan.obj"
	-@erase "$(INTDIR)\spp_portscan2.obj"
	-@erase "$(INTDIR)\spp_rpc_decode.obj"
	-@erase "$(INTDIR)\spp_stream4.obj"
	-@erase "$(INTDIR)\spp_telnet_negotiation.obj"
	-@erase "$(INTDIR)\strlcatu.obj"
	-@erase "$(INTDIR)\strlcpyu.obj"
	-@erase "$(INTDIR)\syslog.obj"
	-@erase "$(INTDIR)\tag.obj"
	-@erase "$(INTDIR)\threshold.obj"
	-@erase "$(INTDIR)\ubi_BinTree.obj"
	-@erase "$(INTDIR)\ubi_SplayTree.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\win32_service.obj"
	-@erase "$(OUTDIR)\snort.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_WIN32_SERVICE" /Fp"$(INTDIR)\snort.pch" /YX"snort.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\snort.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\snort.pdb" /machine:I386 /out:"$(OUTDIR)\snort.exe" /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" 
LINK32_OBJS= \
	"$(INTDIR)\acsmx.obj" \
	"$(INTDIR)\byte_extract.obj" \
	"$(INTDIR)\codes.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\decode.obj" \
	"$(INTDIR)\detect.obj" \
	"$(INTDIR)\fpcreate.obj" \
	"$(INTDIR)\fpdetect.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\mempool.obj" \
	"$(INTDIR)\mpse.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\mwm.obj" \
	"$(INTDIR)\parser.obj" \
	"$(INTDIR)\pcrm.obj" \
	"$(INTDIR)\plugbase.obj" \
	"$(INTDIR)\sf_sdlist.obj" \
	"$(INTDIR)\sfksearch.obj" \
	"$(INTDIR)\signature.obj" \
	"$(INTDIR)\snort.obj" \
	"$(INTDIR)\snprintf.obj" \
	"$(INTDIR)\strlcatu.obj" \
	"$(INTDIR)\strlcpyu.obj" \
	"$(INTDIR)\tag.obj" \
	"$(INTDIR)\threshold.obj" \
	"$(INTDIR)\ubi_BinTree.obj" \
	"$(INTDIR)\ubi_SplayTree.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\syslog.obj" \
	"$(INTDIR)\win32_service.obj" \
	"$(INTDIR)\sp_byte_check.obj" \
	"$(INTDIR)\sp_byte_jump.obj" \
	"$(INTDIR)\sp_clientserver.obj" \
	"$(INTDIR)\sp_dsize_check.obj" \
	"$(INTDIR)\sp_icmp_code_check.obj" \
	"$(INTDIR)\sp_icmp_id_check.obj" \
	"$(INTDIR)\sp_icmp_seq_check.obj" \
	"$(INTDIR)\sp_icmp_type_check.obj" \
	"$(INTDIR)\sp_ip_fragbits.obj" \
	"$(INTDIR)\sp_ip_id_check.obj" \
	"$(INTDIR)\sp_ip_proto.obj" \
	"$(INTDIR)\sp_ip_same_check.obj" \
	"$(INTDIR)\sp_ip_tos_check.obj" \
	"$(INTDIR)\sp_ipoption_check.obj" \
	"$(INTDIR)\sp_pattern_match.obj" \
	"$(INTDIR)\sp_react.obj" \
	"$(INTDIR)\sp_respond.obj" \
	"$(INTDIR)\sp_rpc_check.obj" \
	"$(INTDIR)\sp_session.obj" \
	"$(INTDIR)\sp_tcp_ack_check.obj" \
	"$(INTDIR)\sp_tcp_flag_check.obj" \
	"$(INTDIR)\sp_tcp_seq_check.obj" \
	"$(INTDIR)\sp_tcp_win_check.obj" \
	"$(INTDIR)\sp_ttl_check.obj" \
	"$(INTDIR)\spo_alert_fast.obj" \
	"$(INTDIR)\spo_alert_full.obj" \
	"$(INTDIR)\spo_alert_sf_socket.obj" \
	"$(INTDIR)\spo_alert_smb.obj" \
	"$(INTDIR)\spo_alert_syslog.obj" \
	"$(INTDIR)\spo_alert_unixsock.obj" \
	"$(INTDIR)\spo_csv.obj" \
	"$(INTDIR)\spo_database.obj" \
	"$(INTDIR)\spo_log_ascii.obj" \
	"$(INTDIR)\spo_log_null.obj" \
	"$(INTDIR)\spo_log_tcpdump.obj" \
	"$(INTDIR)\spo_unified.obj" \
	"$(INTDIR)\http-resp.obj" \
	"$(INTDIR)\perf-base.obj" \
	"$(INTDIR)\perf-event.obj" \
	"$(INTDIR)\perf-flow.obj" \
	"$(INTDIR)\perf.obj" \
	"$(INTDIR)\sfprocpidstats.obj" \
	"$(INTDIR)\spp_arpspoof.obj" \
	"$(INTDIR)\spp_bo.obj" \
	"$(INTDIR)\spp_conversation.obj" \
	"$(INTDIR)\spp_frag2.obj" \
	"$(INTDIR)\spp_http_decode.obj" \
	"$(INTDIR)\spp_httpflow.obj" \
	"$(INTDIR)\spp_perfmonitor.obj" \
	"$(INTDIR)\spp_portscan.obj" \
	"$(INTDIR)\spp_portscan2.obj" \
	"$(INTDIR)\spp_rpc_decode.obj" \
	"$(INTDIR)\spp_stream4.obj" \
	"$(INTDIR)\spp_telnet_negotiation.obj" \
	"$(INTDIR)\IpAddrSet.obj"

"$(OUTDIR)\snort.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"

OUTDIR=.\snort___Win32_StdDB_MSSQL_FlexResp_Debug
INTDIR=.\snort___Win32_StdDB_MSSQL_FlexResp_Debug
# Begin Custom Macros
OutDir=.\snort___Win32_StdDB_MSSQL_FlexResp_Debug
# End Custom Macros

ALL : "$(OUTDIR)\snort.exe" "$(OUTDIR)\snort.bsc"


CLEAN :
	-@erase "$(INTDIR)\acsmx.obj"
	-@erase "$(INTDIR)\acsmx.sbr"
	-@erase "$(INTDIR)\byte_extract.obj"
	-@erase "$(INTDIR)\byte_extract.sbr"
	-@erase "$(INTDIR)\codes.obj"
	-@erase "$(INTDIR)\codes.sbr"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\debug.sbr"
	-@erase "$(INTDIR)\decode.obj"
	-@erase "$(INTDIR)\decode.sbr"
	-@erase "$(INTDIR)\detect.obj"
	-@erase "$(INTDIR)\detect.sbr"
	-@erase "$(INTDIR)\fpcreate.obj"
	-@erase "$(INTDIR)\fpcreate.sbr"
	-@erase "$(INTDIR)\fpdetect.obj"
	-@erase "$(INTDIR)\fpdetect.sbr"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\getopt.sbr"
	-@erase "$(INTDIR)\http-resp.obj"
	-@erase "$(INTDIR)\http-resp.sbr"
	-@erase "$(INTDIR)\IpAddrSet.obj"
	-@erase "$(INTDIR)\IpAddrSet.sbr"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\log.sbr"
	-@erase "$(INTDIR)\mempool.obj"
	-@erase "$(INTDIR)\mempool.sbr"
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\misc.sbr"
	-@erase "$(INTDIR)\mpse.obj"
	-@erase "$(INTDIR)\mpse.sbr"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\mstring.sbr"
	-@erase "$(INTDIR)\mwm.obj"
	-@erase "$(INTDIR)\mwm.sbr"
	-@erase "$(INTDIR)\parser.obj"
	-@erase "$(INTDIR)\parser.sbr"
	-@erase "$(INTDIR)\pcrm.obj"
	-@erase "$(INTDIR)\pcrm.sbr"
	-@erase "$(INTDIR)\perf-base.obj"
	-@erase "$(INTDIR)\perf-base.sbr"
	-@erase "$(INTDIR)\perf-event.obj"
	-@erase "$(INTDIR)\perf-event.sbr"
	-@erase "$(INTDIR)\perf-flow.obj"
	-@erase "$(INTDIR)\perf-flow.sbr"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\perf.sbr"
	-@erase "$(INTDIR)\plugbase.obj"
	-@erase "$(INTDIR)\plugbase.sbr"
	-@erase "$(INTDIR)\sf_sdlist.obj"
	-@erase "$(INTDIR)\sf_sdlist.sbr"
	-@erase "$(INTDIR)\sfksearch.obj"
	-@erase "$(INTDIR)\sfksearch.sbr"
	-@erase "$(INTDIR)\sfprocpidstats.obj"
	-@erase "$(INTDIR)\sfprocpidstats.sbr"
	-@erase "$(INTDIR)\signature.obj"
	-@erase "$(INTDIR)\signature.sbr"
	-@erase "$(INTDIR)\snort.obj"
	-@erase "$(INTDIR)\snort.sbr"
	-@erase "$(INTDIR)\snprintf.obj"
	-@erase "$(INTDIR)\snprintf.sbr"
	-@erase "$(INTDIR)\sp_byte_check.obj"
	-@erase "$(INTDIR)\sp_byte_check.sbr"
	-@erase "$(INTDIR)\sp_byte_jump.obj"
	-@erase "$(INTDIR)\sp_byte_jump.sbr"
	-@erase "$(INTDIR)\sp_clientserver.obj"
	-@erase "$(INTDIR)\sp_clientserver.sbr"
	-@erase "$(INTDIR)\sp_dsize_check.obj"
	-@erase "$(INTDIR)\sp_dsize_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_code_check.obj"
	-@erase "$(INTDIR)\sp_icmp_code_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_id_check.obj"
	-@erase "$(INTDIR)\sp_icmp_id_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_seq_check.obj"
	-@erase "$(INTDIR)\sp_icmp_seq_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_type_check.obj"
	-@erase "$(INTDIR)\sp_icmp_type_check.sbr"
	-@erase "$(INTDIR)\sp_ip_fragbits.obj"
	-@erase "$(INTDIR)\sp_ip_fragbits.sbr"
	-@erase "$(INTDIR)\sp_ip_id_check.obj"
	-@erase "$(INTDIR)\sp_ip_id_check.sbr"
	-@erase "$(INTDIR)\sp_ip_proto.obj"
	-@erase "$(INTDIR)\sp_ip_proto.sbr"
	-@erase "$(INTDIR)\sp_ip_same_check.obj"
	-@erase "$(INTDIR)\sp_ip_same_check.sbr"
	-@erase "$(INTDIR)\sp_ip_tos_check.obj"
	-@erase "$(INTDIR)\sp_ip_tos_check.sbr"
	-@erase "$(INTDIR)\sp_ipoption_check.obj"
	-@erase "$(INTDIR)\sp_ipoption_check.sbr"
	-@erase "$(INTDIR)\sp_pattern_match.obj"
	-@erase "$(INTDIR)\sp_pattern_match.sbr"
	-@erase "$(INTDIR)\sp_react.obj"
	-@erase "$(INTDIR)\sp_react.sbr"
	-@erase "$(INTDIR)\sp_respond.obj"
	-@erase "$(INTDIR)\sp_respond.sbr"
	-@erase "$(INTDIR)\sp_rpc_check.obj"
	-@erase "$(INTDIR)\sp_rpc_check.sbr"
	-@erase "$(INTDIR)\sp_session.obj"
	-@erase "$(INTDIR)\sp_session.sbr"
	-@erase "$(INTDIR)\sp_tcp_ack_check.obj"
	-@erase "$(INTDIR)\sp_tcp_ack_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_flag_check.obj"
	-@erase "$(INTDIR)\sp_tcp_flag_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_seq_check.obj"
	-@erase "$(INTDIR)\sp_tcp_seq_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_win_check.obj"
	-@erase "$(INTDIR)\sp_tcp_win_check.sbr"
	-@erase "$(INTDIR)\sp_ttl_check.obj"
	-@erase "$(INTDIR)\sp_ttl_check.sbr"
	-@erase "$(INTDIR)\spo_alert_fast.obj"
	-@erase "$(INTDIR)\spo_alert_fast.sbr"
	-@erase "$(INTDIR)\spo_alert_full.obj"
	-@erase "$(INTDIR)\spo_alert_full.sbr"
	-@erase "$(INTDIR)\spo_alert_sf_socket.obj"
	-@erase "$(INTDIR)\spo_alert_sf_socket.sbr"
	-@erase "$(INTDIR)\spo_alert_smb.obj"
	-@erase "$(INTDIR)\spo_alert_smb.sbr"
	-@erase "$(INTDIR)\spo_alert_syslog.obj"
	-@erase "$(INTDIR)\spo_alert_syslog.sbr"
	-@erase "$(INTDIR)\spo_alert_unixsock.obj"
	-@erase "$(INTDIR)\spo_alert_unixsock.sbr"
	-@erase "$(INTDIR)\spo_csv.obj"
	-@erase "$(INTDIR)\spo_csv.sbr"
	-@erase "$(INTDIR)\spo_database.obj"
	-@erase "$(INTDIR)\spo_database.sbr"
	-@erase "$(INTDIR)\spo_log_ascii.obj"
	-@erase "$(INTDIR)\spo_log_ascii.sbr"
	-@erase "$(INTDIR)\spo_log_null.obj"
	-@erase "$(INTDIR)\spo_log_null.sbr"
	-@erase "$(INTDIR)\spo_log_tcpdump.obj"
	-@erase "$(INTDIR)\spo_log_tcpdump.sbr"
	-@erase "$(INTDIR)\spo_unified.obj"
	-@erase "$(INTDIR)\spo_unified.sbr"
	-@erase "$(INTDIR)\spp_arpspoof.obj"
	-@erase "$(INTDIR)\spp_arpspoof.sbr"
	-@erase "$(INTDIR)\spp_bo.obj"
	-@erase "$(INTDIR)\spp_bo.sbr"
	-@erase "$(INTDIR)\spp_conversation.obj"
	-@erase "$(INTDIR)\spp_conversation.sbr"
	-@erase "$(INTDIR)\spp_frag2.obj"
	-@erase "$(INTDIR)\spp_frag2.sbr"
	-@erase "$(INTDIR)\spp_http_decode.obj"
	-@erase "$(INTDIR)\spp_http_decode.sbr"
	-@erase "$(INTDIR)\spp_httpflow.obj"
	-@erase "$(INTDIR)\spp_httpflow.sbr"
	-@erase "$(INTDIR)\spp_perfmonitor.obj"
	-@erase "$(INTDIR)\spp_perfmonitor.sbr"
	-@erase "$(INTDIR)\spp_portscan.obj"
	-@erase "$(INTDIR)\spp_portscan.sbr"
	-@erase "$(INTDIR)\spp_portscan2.obj"
	-@erase "$(INTDIR)\spp_portscan2.sbr"
	-@erase "$(INTDIR)\spp_rpc_decode.obj"
	-@erase "$(INTDIR)\spp_rpc_decode.sbr"
	-@erase "$(INTDIR)\spp_stream4.obj"
	-@erase "$(INTDIR)\spp_stream4.sbr"
	-@erase "$(INTDIR)\spp_telnet_negotiation.obj"
	-@erase "$(INTDIR)\spp_telnet_negotiation.sbr"
	-@erase "$(INTDIR)\strlcatu.obj"
	-@erase "$(INTDIR)\strlcatu.sbr"
	-@erase "$(INTDIR)\strlcpyu.obj"
	-@erase "$(INTDIR)\strlcpyu.sbr"
	-@erase "$(INTDIR)\syslog.obj"
	-@erase "$(INTDIR)\syslog.sbr"
	-@erase "$(INTDIR)\tag.obj"
	-@erase "$(INTDIR)\tag.sbr"
	-@erase "$(INTDIR)\threshold.obj"
	-@erase "$(INTDIR)\threshold.sbr"
	-@erase "$(INTDIR)\ubi_BinTree.obj"
	-@erase "$(INTDIR)\ubi_BinTree.sbr"
	-@erase "$(INTDIR)\ubi_SplayTree.obj"
	-@erase "$(INTDIR)\ubi_SplayTree.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\win32_service.obj"
	-@erase "$(INTDIR)\win32_service.sbr"
	-@erase "$(OUTDIR)\snort.bsc"
	-@erase "$(OUTDIR)\snort.exe"
	-@erase "$(OUTDIR)\snort.ilk"
	-@erase "$(OUTDIR)\snort.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\snort.pch" /YX"snort.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\snort.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\acsmx.sbr" \
	"$(INTDIR)\byte_extract.sbr" \
	"$(INTDIR)\codes.sbr" \
	"$(INTDIR)\debug.sbr" \
	"$(INTDIR)\decode.sbr" \
	"$(INTDIR)\detect.sbr" \
	"$(INTDIR)\fpcreate.sbr" \
	"$(INTDIR)\fpdetect.sbr" \
	"$(INTDIR)\log.sbr" \
	"$(INTDIR)\mempool.sbr" \
	"$(INTDIR)\mpse.sbr" \
	"$(INTDIR)\mstring.sbr" \
	"$(INTDIR)\mwm.sbr" \
	"$(INTDIR)\parser.sbr" \
	"$(INTDIR)\pcrm.sbr" \
	"$(INTDIR)\plugbase.sbr" \
	"$(INTDIR)\sf_sdlist.sbr" \
	"$(INTDIR)\sfksearch.sbr" \
	"$(INTDIR)\signature.sbr" \
	"$(INTDIR)\snort.sbr" \
	"$(INTDIR)\snprintf.sbr" \
	"$(INTDIR)\strlcatu.sbr" \
	"$(INTDIR)\strlcpyu.sbr" \
	"$(INTDIR)\tag.sbr" \
	"$(INTDIR)\threshold.sbr" \
	"$(INTDIR)\ubi_BinTree.sbr" \
	"$(INTDIR)\ubi_SplayTree.sbr" \
	"$(INTDIR)\util.sbr" \
	"$(INTDIR)\getopt.sbr" \
	"$(INTDIR)\misc.sbr" \
	"$(INTDIR)\syslog.sbr" \
	"$(INTDIR)\win32_service.sbr" \
	"$(INTDIR)\sp_byte_check.sbr" \
	"$(INTDIR)\sp_byte_jump.sbr" \
	"$(INTDIR)\sp_clientserver.sbr" \
	"$(INTDIR)\sp_dsize_check.sbr" \
	"$(INTDIR)\sp_icmp_code_check.sbr" \
	"$(INTDIR)\sp_icmp_id_check.sbr" \
	"$(INTDIR)\sp_icmp_seq_check.sbr" \
	"$(INTDIR)\sp_icmp_type_check.sbr" \
	"$(INTDIR)\sp_ip_fragbits.sbr" \
	"$(INTDIR)\sp_ip_id_check.sbr" \
	"$(INTDIR)\sp_ip_proto.sbr" \
	"$(INTDIR)\sp_ip_same_check.sbr" \
	"$(INTDIR)\sp_ip_tos_check.sbr" \
	"$(INTDIR)\sp_ipoption_check.sbr" \
	"$(INTDIR)\sp_pattern_match.sbr" \
	"$(INTDIR)\sp_react.sbr" \
	"$(INTDIR)\sp_respond.sbr" \
	"$(INTDIR)\sp_rpc_check.sbr" \
	"$(INTDIR)\sp_session.sbr" \
	"$(INTDIR)\sp_tcp_ack_check.sbr" \
	"$(INTDIR)\sp_tcp_flag_check.sbr" \
	"$(INTDIR)\sp_tcp_seq_check.sbr" \
	"$(INTDIR)\sp_tcp_win_check.sbr" \
	"$(INTDIR)\sp_ttl_check.sbr" \
	"$(INTDIR)\spo_alert_fast.sbr" \
	"$(INTDIR)\spo_alert_full.sbr" \
	"$(INTDIR)\spo_alert_sf_socket.sbr" \
	"$(INTDIR)\spo_alert_smb.sbr" \
	"$(INTDIR)\spo_alert_syslog.sbr" \
	"$(INTDIR)\spo_alert_unixsock.sbr" \
	"$(INTDIR)\spo_csv.sbr" \
	"$(INTDIR)\spo_database.sbr" \
	"$(INTDIR)\spo_log_ascii.sbr" \
	"$(INTDIR)\spo_log_null.sbr" \
	"$(INTDIR)\spo_log_tcpdump.sbr" \
	"$(INTDIR)\spo_unified.sbr" \
	"$(INTDIR)\http-resp.sbr" \
	"$(INTDIR)\perf-base.sbr" \
	"$(INTDIR)\perf-event.sbr" \
	"$(INTDIR)\perf-flow.sbr" \
	"$(INTDIR)\perf.sbr" \
	"$(INTDIR)\sfprocpidstats.sbr" \
	"$(INTDIR)\spp_arpspoof.sbr" \
	"$(INTDIR)\spp_bo.sbr" \
	"$(INTDIR)\spp_conversation.sbr" \
	"$(INTDIR)\spp_frag2.sbr" \
	"$(INTDIR)\spp_http_decode.sbr" \
	"$(INTDIR)\spp_httpflow.sbr" \
	"$(INTDIR)\spp_perfmonitor.sbr" \
	"$(INTDIR)\spp_portscan.sbr" \
	"$(INTDIR)\spp_portscan2.sbr" \
	"$(INTDIR)\spp_rpc_decode.sbr" \
	"$(INTDIR)\spp_stream4.sbr" \
	"$(INTDIR)\spp_telnet_negotiation.sbr" \
	"$(INTDIR)\IpAddrSet.sbr"

"$(OUTDIR)\snort.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\snort.pdb" /debug /machine:I386 /out:"$(OUTDIR)\snort.exe" /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet" 
LINK32_OBJS= \
	"$(INTDIR)\acsmx.obj" \
	"$(INTDIR)\byte_extract.obj" \
	"$(INTDIR)\codes.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\decode.obj" \
	"$(INTDIR)\detect.obj" \
	"$(INTDIR)\fpcreate.obj" \
	"$(INTDIR)\fpdetect.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\mempool.obj" \
	"$(INTDIR)\mpse.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\mwm.obj" \
	"$(INTDIR)\parser.obj" \
	"$(INTDIR)\pcrm.obj" \
	"$(INTDIR)\plugbase.obj" \
	"$(INTDIR)\sf_sdlist.obj" \
	"$(INTDIR)\sfksearch.obj" \
	"$(INTDIR)\signature.obj" \
	"$(INTDIR)\snort.obj" \
	"$(INTDIR)\snprintf.obj" \
	"$(INTDIR)\strlcatu.obj" \
	"$(INTDIR)\strlcpyu.obj" \
	"$(INTDIR)\tag.obj" \
	"$(INTDIR)\threshold.obj" \
	"$(INTDIR)\ubi_BinTree.obj" \
	"$(INTDIR)\ubi_SplayTree.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\syslog.obj" \
	"$(INTDIR)\win32_service.obj" \
	"$(INTDIR)\sp_byte_check.obj" \
	"$(INTDIR)\sp_byte_jump.obj" \
	"$(INTDIR)\sp_clientserver.obj" \
	"$(INTDIR)\sp_dsize_check.obj" \
	"$(INTDIR)\sp_icmp_code_check.obj" \
	"$(INTDIR)\sp_icmp_id_check.obj" \
	"$(INTDIR)\sp_icmp_seq_check.obj" \
	"$(INTDIR)\sp_icmp_type_check.obj" \
	"$(INTDIR)\sp_ip_fragbits.obj" \
	"$(INTDIR)\sp_ip_id_check.obj" \
	"$(INTDIR)\sp_ip_proto.obj" \
	"$(INTDIR)\sp_ip_same_check.obj" \
	"$(INTDIR)\sp_ip_tos_check.obj" \
	"$(INTDIR)\sp_ipoption_check.obj" \
	"$(INTDIR)\sp_pattern_match.obj" \
	"$(INTDIR)\sp_react.obj" \
	"$(INTDIR)\sp_respond.obj" \
	"$(INTDIR)\sp_rpc_check.obj" \
	"$(INTDIR)\sp_session.obj" \
	"$(INTDIR)\sp_tcp_ack_check.obj" \
	"$(INTDIR)\sp_tcp_flag_check.obj" \
	"$(INTDIR)\sp_tcp_seq_check.obj" \
	"$(INTDIR)\sp_tcp_win_check.obj" \
	"$(INTDIR)\sp_ttl_check.obj" \
	"$(INTDIR)\spo_alert_fast.obj" \
	"$(INTDIR)\spo_alert_full.obj" \
	"$(INTDIR)\spo_alert_sf_socket.obj" \
	"$(INTDIR)\spo_alert_smb.obj" \
	"$(INTDIR)\spo_alert_syslog.obj" \
	"$(INTDIR)\spo_alert_unixsock.obj" \
	"$(INTDIR)\spo_csv.obj" \
	"$(INTDIR)\spo_database.obj" \
	"$(INTDIR)\spo_log_ascii.obj" \
	"$(INTDIR)\spo_log_null.obj" \
	"$(INTDIR)\spo_log_tcpdump.obj" \
	"$(INTDIR)\spo_unified.obj" \
	"$(INTDIR)\http-resp.obj" \
	"$(INTDIR)\perf-base.obj" \
	"$(INTDIR)\perf-event.obj" \
	"$(INTDIR)\perf-flow.obj" \
	"$(INTDIR)\perf.obj" \
	"$(INTDIR)\sfprocpidstats.obj" \
	"$(INTDIR)\spp_arpspoof.obj" \
	"$(INTDIR)\spp_bo.obj" \
	"$(INTDIR)\spp_conversation.obj" \
	"$(INTDIR)\spp_frag2.obj" \
	"$(INTDIR)\spp_http_decode.obj" \
	"$(INTDIR)\spp_httpflow.obj" \
	"$(INTDIR)\spp_perfmonitor.obj" \
	"$(INTDIR)\spp_portscan.obj" \
	"$(INTDIR)\spp_portscan2.obj" \
	"$(INTDIR)\spp_rpc_decode.obj" \
	"$(INTDIR)\spp_stream4.obj" \
	"$(INTDIR)\spp_telnet_negotiation.obj" \
	"$(INTDIR)\IpAddrSet.obj"

"$(OUTDIR)\snort.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"

OUTDIR=.\snort___Win32_StdDB_MSSQL_FlexResp_Release
INTDIR=.\snort___Win32_StdDB_MSSQL_FlexResp_Release
# Begin Custom Macros
OutDir=.\snort___Win32_StdDB_MSSQL_FlexResp_Release
# End Custom Macros

ALL : "$(OUTDIR)\snort.exe"


CLEAN :
	-@erase "$(INTDIR)\acsmx.obj"
	-@erase "$(INTDIR)\byte_extract.obj"
	-@erase "$(INTDIR)\codes.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\decode.obj"
	-@erase "$(INTDIR)\detect.obj"
	-@erase "$(INTDIR)\fpcreate.obj"
	-@erase "$(INTDIR)\fpdetect.obj"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\http-resp.obj"
	-@erase "$(INTDIR)\IpAddrSet.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\mempool.obj"
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\mpse.obj"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\mwm.obj"
	-@erase "$(INTDIR)\parser.obj"
	-@erase "$(INTDIR)\pcrm.obj"
	-@erase "$(INTDIR)\perf-base.obj"
	-@erase "$(INTDIR)\perf-event.obj"
	-@erase "$(INTDIR)\perf-flow.obj"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\plugbase.obj"
	-@erase "$(INTDIR)\sf_sdlist.obj"
	-@erase "$(INTDIR)\sfksearch.obj"
	-@erase "$(INTDIR)\sfprocpidstats.obj"
	-@erase "$(INTDIR)\signature.obj"
	-@erase "$(INTDIR)\snort.obj"
	-@erase "$(INTDIR)\snprintf.obj"
	-@erase "$(INTDIR)\sp_byte_check.obj"
	-@erase "$(INTDIR)\sp_byte_jump.obj"
	-@erase "$(INTDIR)\sp_clientserver.obj"
	-@erase "$(INTDIR)\sp_dsize_check.obj"
	-@erase "$(INTDIR)\sp_icmp_code_check.obj"
	-@erase "$(INTDIR)\sp_icmp_id_check.obj"
	-@erase "$(INTDIR)\sp_icmp_seq_check.obj"
	-@erase "$(INTDIR)\sp_icmp_type_check.obj"
	-@erase "$(INTDIR)\sp_ip_fragbits.obj"
	-@erase "$(INTDIR)\sp_ip_id_check.obj"
	-@erase "$(INTDIR)\sp_ip_proto.obj"
	-@erase "$(INTDIR)\sp_ip_same_check.obj"
	-@erase "$(INTDIR)\sp_ip_tos_check.obj"
	-@erase "$(INTDIR)\sp_ipoption_check.obj"
	-@erase "$(INTDIR)\sp_pattern_match.obj"
	-@erase "$(INTDIR)\sp_react.obj"
	-@erase "$(INTDIR)\sp_respond.obj"
	-@erase "$(INTDIR)\sp_rpc_check.obj"
	-@erase "$(INTDIR)\sp_session.obj"
	-@erase "$(INTDIR)\sp_tcp_ack_check.obj"
	-@erase "$(INTDIR)\sp_tcp_flag_check.obj"
	-@erase "$(INTDIR)\sp_tcp_seq_check.obj"
	-@erase "$(INTDIR)\sp_tcp_win_check.obj"
	-@erase "$(INTDIR)\sp_ttl_check.obj"
	-@erase "$(INTDIR)\spo_alert_fast.obj"
	-@erase "$(INTDIR)\spo_alert_full.obj"
	-@erase "$(INTDIR)\spo_alert_sf_socket.obj"
	-@erase "$(INTDIR)\spo_alert_smb.obj"
	-@erase "$(INTDIR)\spo_alert_syslog.obj"
	-@erase "$(INTDIR)\spo_alert_unixsock.obj"
	-@erase "$(INTDIR)\spo_csv.obj"
	-@erase "$(INTDIR)\spo_database.obj"
	-@erase "$(INTDIR)\spo_log_ascii.obj"
	-@erase "$(INTDIR)\spo_log_null.obj"
	-@erase "$(INTDIR)\spo_log_tcpdump.obj"
	-@erase "$(INTDIR)\spo_unified.obj"
	-@erase "$(INTDIR)\spp_arpspoof.obj"
	-@erase "$(INTDIR)\spp_bo.obj"
	-@erase "$(INTDIR)\spp_conversation.obj"
	-@erase "$(INTDIR)\spp_frag2.obj"
	-@erase "$(INTDIR)\spp_http_decode.obj"
	-@erase "$(INTDIR)\spp_httpflow.obj"
	-@erase "$(INTDIR)\spp_perfmonitor.obj"
	-@erase "$(INTDIR)\spp_portscan.obj"
	-@erase "$(INTDIR)\spp_portscan2.obj"
	-@erase "$(INTDIR)\spp_rpc_decode.obj"
	-@erase "$(INTDIR)\spp_stream4.obj"
	-@erase "$(INTDIR)\spp_telnet_negotiation.obj"
	-@erase "$(INTDIR)\strlcatu.obj"
	-@erase "$(INTDIR)\strlcpyu.obj"
	-@erase "$(INTDIR)\syslog.obj"
	-@erase "$(INTDIR)\tag.obj"
	-@erase "$(INTDIR)\threshold.obj"
	-@erase "$(INTDIR)\ubi_BinTree.obj"
	-@erase "$(INTDIR)\ubi_SplayTree.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\win32_service.obj"
	-@erase "$(OUTDIR)\snort.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /Fp"$(INTDIR)\snort.pch" /YX"snort.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\snort.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\snort.pdb" /machine:I386 /out:"$(OUTDIR)\snort.exe" /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet" 
LINK32_OBJS= \
	"$(INTDIR)\acsmx.obj" \
	"$(INTDIR)\byte_extract.obj" \
	"$(INTDIR)\codes.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\decode.obj" \
	"$(INTDIR)\detect.obj" \
	"$(INTDIR)\fpcreate.obj" \
	"$(INTDIR)\fpdetect.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\mempool.obj" \
	"$(INTDIR)\mpse.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\mwm.obj" \
	"$(INTDIR)\parser.obj" \
	"$(INTDIR)\pcrm.obj" \
	"$(INTDIR)\plugbase.obj" \
	"$(INTDIR)\sf_sdlist.obj" \
	"$(INTDIR)\sfksearch.obj" \
	"$(INTDIR)\signature.obj" \
	"$(INTDIR)\snort.obj" \
	"$(INTDIR)\snprintf.obj" \
	"$(INTDIR)\strlcatu.obj" \
	"$(INTDIR)\strlcpyu.obj" \
	"$(INTDIR)\tag.obj" \
	"$(INTDIR)\threshold.obj" \
	"$(INTDIR)\ubi_BinTree.obj" \
	"$(INTDIR)\ubi_SplayTree.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\syslog.obj" \
	"$(INTDIR)\win32_service.obj" \
	"$(INTDIR)\sp_byte_check.obj" \
	"$(INTDIR)\sp_byte_jump.obj" \
	"$(INTDIR)\sp_clientserver.obj" \
	"$(INTDIR)\sp_dsize_check.obj" \
	"$(INTDIR)\sp_icmp_code_check.obj" \
	"$(INTDIR)\sp_icmp_id_check.obj" \
	"$(INTDIR)\sp_icmp_seq_check.obj" \
	"$(INTDIR)\sp_icmp_type_check.obj" \
	"$(INTDIR)\sp_ip_fragbits.obj" \
	"$(INTDIR)\sp_ip_id_check.obj" \
	"$(INTDIR)\sp_ip_proto.obj" \
	"$(INTDIR)\sp_ip_same_check.obj" \
	"$(INTDIR)\sp_ip_tos_check.obj" \
	"$(INTDIR)\sp_ipoption_check.obj" \
	"$(INTDIR)\sp_pattern_match.obj" \
	"$(INTDIR)\sp_react.obj" \
	"$(INTDIR)\sp_respond.obj" \
	"$(INTDIR)\sp_rpc_check.obj" \
	"$(INTDIR)\sp_session.obj" \
	"$(INTDIR)\sp_tcp_ack_check.obj" \
	"$(INTDIR)\sp_tcp_flag_check.obj" \
	"$(INTDIR)\sp_tcp_seq_check.obj" \
	"$(INTDIR)\sp_tcp_win_check.obj" \
	"$(INTDIR)\sp_ttl_check.obj" \
	"$(INTDIR)\spo_alert_fast.obj" \
	"$(INTDIR)\spo_alert_full.obj" \
	"$(INTDIR)\spo_alert_sf_socket.obj" \
	"$(INTDIR)\spo_alert_smb.obj" \
	"$(INTDIR)\spo_alert_syslog.obj" \
	"$(INTDIR)\spo_alert_unixsock.obj" \
	"$(INTDIR)\spo_csv.obj" \
	"$(INTDIR)\spo_database.obj" \
	"$(INTDIR)\spo_log_ascii.obj" \
	"$(INTDIR)\spo_log_null.obj" \
	"$(INTDIR)\spo_log_tcpdump.obj" \
	"$(INTDIR)\spo_unified.obj" \
	"$(INTDIR)\http-resp.obj" \
	"$(INTDIR)\perf-base.obj" \
	"$(INTDIR)\perf-event.obj" \
	"$(INTDIR)\perf-flow.obj" \
	"$(INTDIR)\perf.obj" \
	"$(INTDIR)\sfprocpidstats.obj" \
	"$(INTDIR)\spp_arpspoof.obj" \
	"$(INTDIR)\spp_bo.obj" \
	"$(INTDIR)\spp_conversation.obj" \
	"$(INTDIR)\spp_frag2.obj" \
	"$(INTDIR)\spp_http_decode.obj" \
	"$(INTDIR)\spp_httpflow.obj" \
	"$(INTDIR)\spp_perfmonitor.obj" \
	"$(INTDIR)\spp_portscan.obj" \
	"$(INTDIR)\spp_portscan2.obj" \
	"$(INTDIR)\spp_rpc_decode.obj" \
	"$(INTDIR)\spp_stream4.obj" \
	"$(INTDIR)\spp_telnet_negotiation.obj" \
	"$(INTDIR)\IpAddrSet.obj"

"$(OUTDIR)\snort.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"

OUTDIR=.\snort___Win32_StdDB_MSSQL_FlexResp_Service_Debug
INTDIR=.\snort___Win32_StdDB_MSSQL_FlexResp_Service_Debug
# Begin Custom Macros
OutDir=.\snort___Win32_StdDB_MSSQL_FlexResp_Service_Debug
# End Custom Macros

ALL : "$(OUTDIR)\snort.exe" "$(OUTDIR)\snort.bsc"


CLEAN :
	-@erase "$(INTDIR)\acsmx.obj"
	-@erase "$(INTDIR)\acsmx.sbr"
	-@erase "$(INTDIR)\byte_extract.obj"
	-@erase "$(INTDIR)\byte_extract.sbr"
	-@erase "$(INTDIR)\codes.obj"
	-@erase "$(INTDIR)\codes.sbr"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\debug.sbr"
	-@erase "$(INTDIR)\decode.obj"
	-@erase "$(INTDIR)\decode.sbr"
	-@erase "$(INTDIR)\detect.obj"
	-@erase "$(INTDIR)\detect.sbr"
	-@erase "$(INTDIR)\fpcreate.obj"
	-@erase "$(INTDIR)\fpcreate.sbr"
	-@erase "$(INTDIR)\fpdetect.obj"
	-@erase "$(INTDIR)\fpdetect.sbr"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\getopt.sbr"
	-@erase "$(INTDIR)\http-resp.obj"
	-@erase "$(INTDIR)\http-resp.sbr"
	-@erase "$(INTDIR)\IpAddrSet.obj"
	-@erase "$(INTDIR)\IpAddrSet.sbr"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\log.sbr"
	-@erase "$(INTDIR)\mempool.obj"
	-@erase "$(INTDIR)\mempool.sbr"
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\misc.sbr"
	-@erase "$(INTDIR)\mpse.obj"
	-@erase "$(INTDIR)\mpse.sbr"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\mstring.sbr"
	-@erase "$(INTDIR)\mwm.obj"
	-@erase "$(INTDIR)\mwm.sbr"
	-@erase "$(INTDIR)\parser.obj"
	-@erase "$(INTDIR)\parser.sbr"
	-@erase "$(INTDIR)\pcrm.obj"
	-@erase "$(INTDIR)\pcrm.sbr"
	-@erase "$(INTDIR)\perf-base.obj"
	-@erase "$(INTDIR)\perf-base.sbr"
	-@erase "$(INTDIR)\perf-event.obj"
	-@erase "$(INTDIR)\perf-event.sbr"
	-@erase "$(INTDIR)\perf-flow.obj"
	-@erase "$(INTDIR)\perf-flow.sbr"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\perf.sbr"
	-@erase "$(INTDIR)\plugbase.obj"
	-@erase "$(INTDIR)\plugbase.sbr"
	-@erase "$(INTDIR)\sf_sdlist.obj"
	-@erase "$(INTDIR)\sf_sdlist.sbr"
	-@erase "$(INTDIR)\sfksearch.obj"
	-@erase "$(INTDIR)\sfksearch.sbr"
	-@erase "$(INTDIR)\sfprocpidstats.obj"
	-@erase "$(INTDIR)\sfprocpidstats.sbr"
	-@erase "$(INTDIR)\signature.obj"
	-@erase "$(INTDIR)\signature.sbr"
	-@erase "$(INTDIR)\snort.obj"
	-@erase "$(INTDIR)\snort.sbr"
	-@erase "$(INTDIR)\snprintf.obj"
	-@erase "$(INTDIR)\snprintf.sbr"
	-@erase "$(INTDIR)\sp_byte_check.obj"
	-@erase "$(INTDIR)\sp_byte_check.sbr"
	-@erase "$(INTDIR)\sp_byte_jump.obj"
	-@erase "$(INTDIR)\sp_byte_jump.sbr"
	-@erase "$(INTDIR)\sp_clientserver.obj"
	-@erase "$(INTDIR)\sp_clientserver.sbr"
	-@erase "$(INTDIR)\sp_dsize_check.obj"
	-@erase "$(INTDIR)\sp_dsize_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_code_check.obj"
	-@erase "$(INTDIR)\sp_icmp_code_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_id_check.obj"
	-@erase "$(INTDIR)\sp_icmp_id_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_seq_check.obj"
	-@erase "$(INTDIR)\sp_icmp_seq_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_type_check.obj"
	-@erase "$(INTDIR)\sp_icmp_type_check.sbr"
	-@erase "$(INTDIR)\sp_ip_fragbits.obj"
	-@erase "$(INTDIR)\sp_ip_fragbits.sbr"
	-@erase "$(INTDIR)\sp_ip_id_check.obj"
	-@erase "$(INTDIR)\sp_ip_id_check.sbr"
	-@erase "$(INTDIR)\sp_ip_proto.obj"
	-@erase "$(INTDIR)\sp_ip_proto.sbr"
	-@erase "$(INTDIR)\sp_ip_same_check.obj"
	-@erase "$(INTDIR)\sp_ip_same_check.sbr"
	-@erase "$(INTDIR)\sp_ip_tos_check.obj"
	-@erase "$(INTDIR)\sp_ip_tos_check.sbr"
	-@erase "$(INTDIR)\sp_ipoption_check.obj"
	-@erase "$(INTDIR)\sp_ipoption_check.sbr"
	-@erase "$(INTDIR)\sp_pattern_match.obj"
	-@erase "$(INTDIR)\sp_pattern_match.sbr"
	-@erase "$(INTDIR)\sp_react.obj"
	-@erase "$(INTDIR)\sp_react.sbr"
	-@erase "$(INTDIR)\sp_respond.obj"
	-@erase "$(INTDIR)\sp_respond.sbr"
	-@erase "$(INTDIR)\sp_rpc_check.obj"
	-@erase "$(INTDIR)\sp_rpc_check.sbr"
	-@erase "$(INTDIR)\sp_session.obj"
	-@erase "$(INTDIR)\sp_session.sbr"
	-@erase "$(INTDIR)\sp_tcp_ack_check.obj"
	-@erase "$(INTDIR)\sp_tcp_ack_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_flag_check.obj"
	-@erase "$(INTDIR)\sp_tcp_flag_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_seq_check.obj"
	-@erase "$(INTDIR)\sp_tcp_seq_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_win_check.obj"
	-@erase "$(INTDIR)\sp_tcp_win_check.sbr"
	-@erase "$(INTDIR)\sp_ttl_check.obj"
	-@erase "$(INTDIR)\sp_ttl_check.sbr"
	-@erase "$(INTDIR)\spo_alert_fast.obj"
	-@erase "$(INTDIR)\spo_alert_fast.sbr"
	-@erase "$(INTDIR)\spo_alert_full.obj"
	-@erase "$(INTDIR)\spo_alert_full.sbr"
	-@erase "$(INTDIR)\spo_alert_sf_socket.obj"
	-@erase "$(INTDIR)\spo_alert_sf_socket.sbr"
	-@erase "$(INTDIR)\spo_alert_smb.obj"
	-@erase "$(INTDIR)\spo_alert_smb.sbr"
	-@erase "$(INTDIR)\spo_alert_syslog.obj"
	-@erase "$(INTDIR)\spo_alert_syslog.sbr"
	-@erase "$(INTDIR)\spo_alert_unixsock.obj"
	-@erase "$(INTDIR)\spo_alert_unixsock.sbr"
	-@erase "$(INTDIR)\spo_csv.obj"
	-@erase "$(INTDIR)\spo_csv.sbr"
	-@erase "$(INTDIR)\spo_database.obj"
	-@erase "$(INTDIR)\spo_database.sbr"
	-@erase "$(INTDIR)\spo_log_ascii.obj"
	-@erase "$(INTDIR)\spo_log_ascii.sbr"
	-@erase "$(INTDIR)\spo_log_null.obj"
	-@erase "$(INTDIR)\spo_log_null.sbr"
	-@erase "$(INTDIR)\spo_log_tcpdump.obj"
	-@erase "$(INTDIR)\spo_log_tcpdump.sbr"
	-@erase "$(INTDIR)\spo_unified.obj"
	-@erase "$(INTDIR)\spo_unified.sbr"
	-@erase "$(INTDIR)\spp_arpspoof.obj"
	-@erase "$(INTDIR)\spp_arpspoof.sbr"
	-@erase "$(INTDIR)\spp_bo.obj"
	-@erase "$(INTDIR)\spp_bo.sbr"
	-@erase "$(INTDIR)\spp_conversation.obj"
	-@erase "$(INTDIR)\spp_conversation.sbr"
	-@erase "$(INTDIR)\spp_frag2.obj"
	-@erase "$(INTDIR)\spp_frag2.sbr"
	-@erase "$(INTDIR)\spp_http_decode.obj"
	-@erase "$(INTDIR)\spp_http_decode.sbr"
	-@erase "$(INTDIR)\spp_httpflow.obj"
	-@erase "$(INTDIR)\spp_httpflow.sbr"
	-@erase "$(INTDIR)\spp_perfmonitor.obj"
	-@erase "$(INTDIR)\spp_perfmonitor.sbr"
	-@erase "$(INTDIR)\spp_portscan.obj"
	-@erase "$(INTDIR)\spp_portscan.sbr"
	-@erase "$(INTDIR)\spp_portscan2.obj"
	-@erase "$(INTDIR)\spp_portscan2.sbr"
	-@erase "$(INTDIR)\spp_rpc_decode.obj"
	-@erase "$(INTDIR)\spp_rpc_decode.sbr"
	-@erase "$(INTDIR)\spp_stream4.obj"
	-@erase "$(INTDIR)\spp_stream4.sbr"
	-@erase "$(INTDIR)\spp_telnet_negotiation.obj"
	-@erase "$(INTDIR)\spp_telnet_negotiation.sbr"
	-@erase "$(INTDIR)\strlcatu.obj"
	-@erase "$(INTDIR)\strlcatu.sbr"
	-@erase "$(INTDIR)\strlcpyu.obj"
	-@erase "$(INTDIR)\strlcpyu.sbr"
	-@erase "$(INTDIR)\syslog.obj"
	-@erase "$(INTDIR)\syslog.sbr"
	-@erase "$(INTDIR)\tag.obj"
	-@erase "$(INTDIR)\tag.sbr"
	-@erase "$(INTDIR)\threshold.obj"
	-@erase "$(INTDIR)\threshold.sbr"
	-@erase "$(INTDIR)\ubi_BinTree.obj"
	-@erase "$(INTDIR)\ubi_BinTree.sbr"
	-@erase "$(INTDIR)\ubi_SplayTree.obj"
	-@erase "$(INTDIR)\ubi_SplayTree.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\win32_service.obj"
	-@erase "$(INTDIR)\win32_service.sbr"
	-@erase "$(OUTDIR)\snort.bsc"
	-@erase "$(OUTDIR)\snort.exe"
	-@erase "$(OUTDIR)\snort.ilk"
	-@erase "$(OUTDIR)\snort.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /D "ENABLE_WIN32_SERVICE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\snort.pch" /YX"snort.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\snort.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\acsmx.sbr" \
	"$(INTDIR)\byte_extract.sbr" \
	"$(INTDIR)\codes.sbr" \
	"$(INTDIR)\debug.sbr" \
	"$(INTDIR)\decode.sbr" \
	"$(INTDIR)\detect.sbr" \
	"$(INTDIR)\fpcreate.sbr" \
	"$(INTDIR)\fpdetect.sbr" \
	"$(INTDIR)\log.sbr" \
	"$(INTDIR)\mempool.sbr" \
	"$(INTDIR)\mpse.sbr" \
	"$(INTDIR)\mstring.sbr" \
	"$(INTDIR)\mwm.sbr" \
	"$(INTDIR)\parser.sbr" \
	"$(INTDIR)\pcrm.sbr" \
	"$(INTDIR)\plugbase.sbr" \
	"$(INTDIR)\sf_sdlist.sbr" \
	"$(INTDIR)\sfksearch.sbr" \
	"$(INTDIR)\signature.sbr" \
	"$(INTDIR)\snort.sbr" \
	"$(INTDIR)\snprintf.sbr" \
	"$(INTDIR)\strlcatu.sbr" \
	"$(INTDIR)\strlcpyu.sbr" \
	"$(INTDIR)\tag.sbr" \
	"$(INTDIR)\threshold.sbr" \
	"$(INTDIR)\ubi_BinTree.sbr" \
	"$(INTDIR)\ubi_SplayTree.sbr" \
	"$(INTDIR)\util.sbr" \
	"$(INTDIR)\getopt.sbr" \
	"$(INTDIR)\misc.sbr" \
	"$(INTDIR)\syslog.sbr" \
	"$(INTDIR)\win32_service.sbr" \
	"$(INTDIR)\sp_byte_check.sbr" \
	"$(INTDIR)\sp_byte_jump.sbr" \
	"$(INTDIR)\sp_clientserver.sbr" \
	"$(INTDIR)\sp_dsize_check.sbr" \
	"$(INTDIR)\sp_icmp_code_check.sbr" \
	"$(INTDIR)\sp_icmp_id_check.sbr" \
	"$(INTDIR)\sp_icmp_seq_check.sbr" \
	"$(INTDIR)\sp_icmp_type_check.sbr" \
	"$(INTDIR)\sp_ip_fragbits.sbr" \
	"$(INTDIR)\sp_ip_id_check.sbr" \
	"$(INTDIR)\sp_ip_proto.sbr" \
	"$(INTDIR)\sp_ip_same_check.sbr" \
	"$(INTDIR)\sp_ip_tos_check.sbr" \
	"$(INTDIR)\sp_ipoption_check.sbr" \
	"$(INTDIR)\sp_pattern_match.sbr" \
	"$(INTDIR)\sp_react.sbr" \
	"$(INTDIR)\sp_respond.sbr" \
	"$(INTDIR)\sp_rpc_check.sbr" \
	"$(INTDIR)\sp_session.sbr" \
	"$(INTDIR)\sp_tcp_ack_check.sbr" \
	"$(INTDIR)\sp_tcp_flag_check.sbr" \
	"$(INTDIR)\sp_tcp_seq_check.sbr" \
	"$(INTDIR)\sp_tcp_win_check.sbr" \
	"$(INTDIR)\sp_ttl_check.sbr" \
	"$(INTDIR)\spo_alert_fast.sbr" \
	"$(INTDIR)\spo_alert_full.sbr" \
	"$(INTDIR)\spo_alert_sf_socket.sbr" \
	"$(INTDIR)\spo_alert_smb.sbr" \
	"$(INTDIR)\spo_alert_syslog.sbr" \
	"$(INTDIR)\spo_alert_unixsock.sbr" \
	"$(INTDIR)\spo_csv.sbr" \
	"$(INTDIR)\spo_database.sbr" \
	"$(INTDIR)\spo_log_ascii.sbr" \
	"$(INTDIR)\spo_log_null.sbr" \
	"$(INTDIR)\spo_log_tcpdump.sbr" \
	"$(INTDIR)\spo_unified.sbr" \
	"$(INTDIR)\http-resp.sbr" \
	"$(INTDIR)\perf-base.sbr" \
	"$(INTDIR)\perf-event.sbr" \
	"$(INTDIR)\perf-flow.sbr" \
	"$(INTDIR)\perf.sbr" \
	"$(INTDIR)\sfprocpidstats.sbr" \
	"$(INTDIR)\spp_arpspoof.sbr" \
	"$(INTDIR)\spp_bo.sbr" \
	"$(INTDIR)\spp_conversation.sbr" \
	"$(INTDIR)\spp_frag2.sbr" \
	"$(INTDIR)\spp_http_decode.sbr" \
	"$(INTDIR)\spp_httpflow.sbr" \
	"$(INTDIR)\spp_perfmonitor.sbr" \
	"$(INTDIR)\spp_portscan.sbr" \
	"$(INTDIR)\spp_portscan2.sbr" \
	"$(INTDIR)\spp_rpc_decode.sbr" \
	"$(INTDIR)\spp_stream4.sbr" \
	"$(INTDIR)\spp_telnet_negotiation.sbr" \
	"$(INTDIR)\IpAddrSet.sbr"

"$(OUTDIR)\snort.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\snort.pdb" /debug /machine:I386 /out:"$(OUTDIR)\snort.exe" /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet" 
LINK32_OBJS= \
	"$(INTDIR)\acsmx.obj" \
	"$(INTDIR)\byte_extract.obj" \
	"$(INTDIR)\codes.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\decode.obj" \
	"$(INTDIR)\detect.obj" \
	"$(INTDIR)\fpcreate.obj" \
	"$(INTDIR)\fpdetect.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\mempool.obj" \
	"$(INTDIR)\mpse.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\mwm.obj" \
	"$(INTDIR)\parser.obj" \
	"$(INTDIR)\pcrm.obj" \
	"$(INTDIR)\plugbase.obj" \
	"$(INTDIR)\sf_sdlist.obj" \
	"$(INTDIR)\sfksearch.obj" \
	"$(INTDIR)\signature.obj" \
	"$(INTDIR)\snort.obj" \
	"$(INTDIR)\snprintf.obj" \
	"$(INTDIR)\strlcatu.obj" \
	"$(INTDIR)\strlcpyu.obj" \
	"$(INTDIR)\tag.obj" \
	"$(INTDIR)\threshold.obj" \
	"$(INTDIR)\ubi_BinTree.obj" \
	"$(INTDIR)\ubi_SplayTree.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\syslog.obj" \
	"$(INTDIR)\win32_service.obj" \
	"$(INTDIR)\sp_byte_check.obj" \
	"$(INTDIR)\sp_byte_jump.obj" \
	"$(INTDIR)\sp_clientserver.obj" \
	"$(INTDIR)\sp_dsize_check.obj" \
	"$(INTDIR)\sp_icmp_code_check.obj" \
	"$(INTDIR)\sp_icmp_id_check.obj" \
	"$(INTDIR)\sp_icmp_seq_check.obj" \
	"$(INTDIR)\sp_icmp_type_check.obj" \
	"$(INTDIR)\sp_ip_fragbits.obj" \
	"$(INTDIR)\sp_ip_id_check.obj" \
	"$(INTDIR)\sp_ip_proto.obj" \
	"$(INTDIR)\sp_ip_same_check.obj" \
	"$(INTDIR)\sp_ip_tos_check.obj" \
	"$(INTDIR)\sp_ipoption_check.obj" \
	"$(INTDIR)\sp_pattern_match.obj" \
	"$(INTDIR)\sp_react.obj" \
	"$(INTDIR)\sp_respond.obj" \
	"$(INTDIR)\sp_rpc_check.obj" \
	"$(INTDIR)\sp_session.obj" \
	"$(INTDIR)\sp_tcp_ack_check.obj" \
	"$(INTDIR)\sp_tcp_flag_check.obj" \
	"$(INTDIR)\sp_tcp_seq_check.obj" \
	"$(INTDIR)\sp_tcp_win_check.obj" \
	"$(INTDIR)\sp_ttl_check.obj" \
	"$(INTDIR)\spo_alert_fast.obj" \
	"$(INTDIR)\spo_alert_full.obj" \
	"$(INTDIR)\spo_alert_sf_socket.obj" \
	"$(INTDIR)\spo_alert_smb.obj" \
	"$(INTDIR)\spo_alert_syslog.obj" \
	"$(INTDIR)\spo_alert_unixsock.obj" \
	"$(INTDIR)\spo_csv.obj" \
	"$(INTDIR)\spo_database.obj" \
	"$(INTDIR)\spo_log_ascii.obj" \
	"$(INTDIR)\spo_log_null.obj" \
	"$(INTDIR)\spo_log_tcpdump.obj" \
	"$(INTDIR)\spo_unified.obj" \
	"$(INTDIR)\http-resp.obj" \
	"$(INTDIR)\perf-base.obj" \
	"$(INTDIR)\perf-event.obj" \
	"$(INTDIR)\perf-flow.obj" \
	"$(INTDIR)\perf.obj" \
	"$(INTDIR)\sfprocpidstats.obj" \
	"$(INTDIR)\spp_arpspoof.obj" \
	"$(INTDIR)\spp_bo.obj" \
	"$(INTDIR)\spp_conversation.obj" \
	"$(INTDIR)\spp_frag2.obj" \
	"$(INTDIR)\spp_http_decode.obj" \
	"$(INTDIR)\spp_httpflow.obj" \
	"$(INTDIR)\spp_perfmonitor.obj" \
	"$(INTDIR)\spp_portscan.obj" \
	"$(INTDIR)\spp_portscan2.obj" \
	"$(INTDIR)\spp_rpc_decode.obj" \
	"$(INTDIR)\spp_stream4.obj" \
	"$(INTDIR)\spp_telnet_negotiation.obj" \
	"$(INTDIR)\IpAddrSet.obj"

"$(OUTDIR)\snort.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"

OUTDIR=.\snort___Win32_StdDB_MSSQL_FlexResp_Service_Release
INTDIR=.\snort___Win32_StdDB_MSSQL_FlexResp_Service_Release
# Begin Custom Macros
OutDir=.\snort___Win32_StdDB_MSSQL_FlexResp_Service_Release
# End Custom Macros

ALL : "$(OUTDIR)\snort.exe"


CLEAN :
	-@erase "$(INTDIR)\acsmx.obj"
	-@erase "$(INTDIR)\byte_extract.obj"
	-@erase "$(INTDIR)\codes.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\decode.obj"
	-@erase "$(INTDIR)\detect.obj"
	-@erase "$(INTDIR)\fpcreate.obj"
	-@erase "$(INTDIR)\fpdetect.obj"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\http-resp.obj"
	-@erase "$(INTDIR)\IpAddrSet.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\mempool.obj"
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\mpse.obj"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\mwm.obj"
	-@erase "$(INTDIR)\parser.obj"
	-@erase "$(INTDIR)\pcrm.obj"
	-@erase "$(INTDIR)\perf-base.obj"
	-@erase "$(INTDIR)\perf-event.obj"
	-@erase "$(INTDIR)\perf-flow.obj"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\plugbase.obj"
	-@erase "$(INTDIR)\sf_sdlist.obj"
	-@erase "$(INTDIR)\sfksearch.obj"
	-@erase "$(INTDIR)\sfprocpidstats.obj"
	-@erase "$(INTDIR)\signature.obj"
	-@erase "$(INTDIR)\snort.obj"
	-@erase "$(INTDIR)\snprintf.obj"
	-@erase "$(INTDIR)\sp_byte_check.obj"
	-@erase "$(INTDIR)\sp_byte_jump.obj"
	-@erase "$(INTDIR)\sp_clientserver.obj"
	-@erase "$(INTDIR)\sp_dsize_check.obj"
	-@erase "$(INTDIR)\sp_icmp_code_check.obj"
	-@erase "$(INTDIR)\sp_icmp_id_check.obj"
	-@erase "$(INTDIR)\sp_icmp_seq_check.obj"
	-@erase "$(INTDIR)\sp_icmp_type_check.obj"
	-@erase "$(INTDIR)\sp_ip_fragbits.obj"
	-@erase "$(INTDIR)\sp_ip_id_check.obj"
	-@erase "$(INTDIR)\sp_ip_proto.obj"
	-@erase "$(INTDIR)\sp_ip_same_check.obj"
	-@erase "$(INTDIR)\sp_ip_tos_check.obj"
	-@erase "$(INTDIR)\sp_ipoption_check.obj"
	-@erase "$(INTDIR)\sp_pattern_match.obj"
	-@erase "$(INTDIR)\sp_react.obj"
	-@erase "$(INTDIR)\sp_respond.obj"
	-@erase "$(INTDIR)\sp_rpc_check.obj"
	-@erase "$(INTDIR)\sp_session.obj"
	-@erase "$(INTDIR)\sp_tcp_ack_check.obj"
	-@erase "$(INTDIR)\sp_tcp_flag_check.obj"
	-@erase "$(INTDIR)\sp_tcp_seq_check.obj"
	-@erase "$(INTDIR)\sp_tcp_win_check.obj"
	-@erase "$(INTDIR)\sp_ttl_check.obj"
	-@erase "$(INTDIR)\spo_alert_fast.obj"
	-@erase "$(INTDIR)\spo_alert_full.obj"
	-@erase "$(INTDIR)\spo_alert_sf_socket.obj"
	-@erase "$(INTDIR)\spo_alert_smb.obj"
	-@erase "$(INTDIR)\spo_alert_syslog.obj"
	-@erase "$(INTDIR)\spo_alert_unixsock.obj"
	-@erase "$(INTDIR)\spo_csv.obj"
	-@erase "$(INTDIR)\spo_database.obj"
	-@erase "$(INTDIR)\spo_log_ascii.obj"
	-@erase "$(INTDIR)\spo_log_null.obj"
	-@erase "$(INTDIR)\spo_log_tcpdump.obj"
	-@erase "$(INTDIR)\spo_unified.obj"
	-@erase "$(INTDIR)\spp_arpspoof.obj"
	-@erase "$(INTDIR)\spp_bo.obj"
	-@erase "$(INTDIR)\spp_conversation.obj"
	-@erase "$(INTDIR)\spp_frag2.obj"
	-@erase "$(INTDIR)\spp_http_decode.obj"
	-@erase "$(INTDIR)\spp_httpflow.obj"
	-@erase "$(INTDIR)\spp_perfmonitor.obj"
	-@erase "$(INTDIR)\spp_portscan.obj"
	-@erase "$(INTDIR)\spp_portscan2.obj"
	-@erase "$(INTDIR)\spp_rpc_decode.obj"
	-@erase "$(INTDIR)\spp_stream4.obj"
	-@erase "$(INTDIR)\spp_telnet_negotiation.obj"
	-@erase "$(INTDIR)\strlcatu.obj"
	-@erase "$(INTDIR)\strlcpyu.obj"
	-@erase "$(INTDIR)\syslog.obj"
	-@erase "$(INTDIR)\tag.obj"
	-@erase "$(INTDIR)\threshold.obj"
	-@erase "$(INTDIR)\ubi_BinTree.obj"
	-@erase "$(INTDIR)\ubi_SplayTree.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\win32_service.obj"
	-@erase "$(OUTDIR)\snort.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\Win32-Includes\libnet" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MSSQL" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_RESPONSE" /D "ENABLE_WIN32_SERVICE" /Fp"$(INTDIR)\snort.pch" /YX"snort.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\snort.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=user32.lib wsock32.lib libpcap.lib advapi32.lib Ntwdblib.lib mysqlclient.lib libnetnt.lib odbc32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\snort.pdb" /machine:I386 /out:"$(OUTDIR)\snort.exe" /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" /libpath:"..\Win32-Libraries\libnet" 
LINK32_OBJS= \
	"$(INTDIR)\acsmx.obj" \
	"$(INTDIR)\byte_extract.obj" \
	"$(INTDIR)\codes.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\decode.obj" \
	"$(INTDIR)\detect.obj" \
	"$(INTDIR)\fpcreate.obj" \
	"$(INTDIR)\fpdetect.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\mempool.obj" \
	"$(INTDIR)\mpse.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\mwm.obj" \
	"$(INTDIR)\parser.obj" \
	"$(INTDIR)\pcrm.obj" \
	"$(INTDIR)\plugbase.obj" \
	"$(INTDIR)\sf_sdlist.obj" \
	"$(INTDIR)\sfksearch.obj" \
	"$(INTDIR)\signature.obj" \
	"$(INTDIR)\snort.obj" \
	"$(INTDIR)\snprintf.obj" \
	"$(INTDIR)\strlcatu.obj" \
	"$(INTDIR)\strlcpyu.obj" \
	"$(INTDIR)\tag.obj" \
	"$(INTDIR)\threshold.obj" \
	"$(INTDIR)\ubi_BinTree.obj" \
	"$(INTDIR)\ubi_SplayTree.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\syslog.obj" \
	"$(INTDIR)\win32_service.obj" \
	"$(INTDIR)\sp_byte_check.obj" \
	"$(INTDIR)\sp_byte_jump.obj" \
	"$(INTDIR)\sp_clientserver.obj" \
	"$(INTDIR)\sp_dsize_check.obj" \
	"$(INTDIR)\sp_icmp_code_check.obj" \
	"$(INTDIR)\sp_icmp_id_check.obj" \
	"$(INTDIR)\sp_icmp_seq_check.obj" \
	"$(INTDIR)\sp_icmp_type_check.obj" \
	"$(INTDIR)\sp_ip_fragbits.obj" \
	"$(INTDIR)\sp_ip_id_check.obj" \
	"$(INTDIR)\sp_ip_proto.obj" \
	"$(INTDIR)\sp_ip_same_check.obj" \
	"$(INTDIR)\sp_ip_tos_check.obj" \
	"$(INTDIR)\sp_ipoption_check.obj" \
	"$(INTDIR)\sp_pattern_match.obj" \
	"$(INTDIR)\sp_react.obj" \
	"$(INTDIR)\sp_respond.obj" \
	"$(INTDIR)\sp_rpc_check.obj" \
	"$(INTDIR)\sp_session.obj" \
	"$(INTDIR)\sp_tcp_ack_check.obj" \
	"$(INTDIR)\sp_tcp_flag_check.obj" \
	"$(INTDIR)\sp_tcp_seq_check.obj" \
	"$(INTDIR)\sp_tcp_win_check.obj" \
	"$(INTDIR)\sp_ttl_check.obj" \
	"$(INTDIR)\spo_alert_fast.obj" \
	"$(INTDIR)\spo_alert_full.obj" \
	"$(INTDIR)\spo_alert_sf_socket.obj" \
	"$(INTDIR)\spo_alert_smb.obj" \
	"$(INTDIR)\spo_alert_syslog.obj" \
	"$(INTDIR)\spo_alert_unixsock.obj" \
	"$(INTDIR)\spo_csv.obj" \
	"$(INTDIR)\spo_database.obj" \
	"$(INTDIR)\spo_log_ascii.obj" \
	"$(INTDIR)\spo_log_null.obj" \
	"$(INTDIR)\spo_log_tcpdump.obj" \
	"$(INTDIR)\spo_unified.obj" \
	"$(INTDIR)\http-resp.obj" \
	"$(INTDIR)\perf-base.obj" \
	"$(INTDIR)\perf-event.obj" \
	"$(INTDIR)\perf-flow.obj" \
	"$(INTDIR)\perf.obj" \
	"$(INTDIR)\sfprocpidstats.obj" \
	"$(INTDIR)\spp_arpspoof.obj" \
	"$(INTDIR)\spp_bo.obj" \
	"$(INTDIR)\spp_conversation.obj" \
	"$(INTDIR)\spp_frag2.obj" \
	"$(INTDIR)\spp_http_decode.obj" \
	"$(INTDIR)\spp_httpflow.obj" \
	"$(INTDIR)\spp_perfmonitor.obj" \
	"$(INTDIR)\spp_portscan.obj" \
	"$(INTDIR)\spp_portscan2.obj" \
	"$(INTDIR)\spp_rpc_decode.obj" \
	"$(INTDIR)\spp_stream4.obj" \
	"$(INTDIR)\spp_telnet_negotiation.obj" \
	"$(INTDIR)\IpAddrSet.obj"

"$(OUTDIR)\snort.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"

OUTDIR=.\snort___Win32_StdDB_Service_Debug
INTDIR=.\snort___Win32_StdDB_Service_Debug
# Begin Custom Macros
OutDir=.\snort___Win32_StdDB_Service_Debug
# End Custom Macros

ALL : "$(OUTDIR)\snort.exe" "$(OUTDIR)\snort.bsc"


CLEAN :
	-@erase "$(INTDIR)\acsmx.obj"
	-@erase "$(INTDIR)\acsmx.sbr"
	-@erase "$(INTDIR)\byte_extract.obj"
	-@erase "$(INTDIR)\byte_extract.sbr"
	-@erase "$(INTDIR)\codes.obj"
	-@erase "$(INTDIR)\codes.sbr"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\debug.sbr"
	-@erase "$(INTDIR)\decode.obj"
	-@erase "$(INTDIR)\decode.sbr"
	-@erase "$(INTDIR)\detect.obj"
	-@erase "$(INTDIR)\detect.sbr"
	-@erase "$(INTDIR)\fpcreate.obj"
	-@erase "$(INTDIR)\fpcreate.sbr"
	-@erase "$(INTDIR)\fpdetect.obj"
	-@erase "$(INTDIR)\fpdetect.sbr"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\getopt.sbr"
	-@erase "$(INTDIR)\http-resp.obj"
	-@erase "$(INTDIR)\http-resp.sbr"
	-@erase "$(INTDIR)\IpAddrSet.obj"
	-@erase "$(INTDIR)\IpAddrSet.sbr"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\log.sbr"
	-@erase "$(INTDIR)\mempool.obj"
	-@erase "$(INTDIR)\mempool.sbr"
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\misc.sbr"
	-@erase "$(INTDIR)\mpse.obj"
	-@erase "$(INTDIR)\mpse.sbr"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\mstring.sbr"
	-@erase "$(INTDIR)\mwm.obj"
	-@erase "$(INTDIR)\mwm.sbr"
	-@erase "$(INTDIR)\parser.obj"
	-@erase "$(INTDIR)\parser.sbr"
	-@erase "$(INTDIR)\pcrm.obj"
	-@erase "$(INTDIR)\pcrm.sbr"
	-@erase "$(INTDIR)\perf-base.obj"
	-@erase "$(INTDIR)\perf-base.sbr"
	-@erase "$(INTDIR)\perf-event.obj"
	-@erase "$(INTDIR)\perf-event.sbr"
	-@erase "$(INTDIR)\perf-flow.obj"
	-@erase "$(INTDIR)\perf-flow.sbr"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\perf.sbr"
	-@erase "$(INTDIR)\plugbase.obj"
	-@erase "$(INTDIR)\plugbase.sbr"
	-@erase "$(INTDIR)\sf_sdlist.obj"
	-@erase "$(INTDIR)\sf_sdlist.sbr"
	-@erase "$(INTDIR)\sfksearch.obj"
	-@erase "$(INTDIR)\sfksearch.sbr"
	-@erase "$(INTDIR)\sfprocpidstats.obj"
	-@erase "$(INTDIR)\sfprocpidstats.sbr"
	-@erase "$(INTDIR)\signature.obj"
	-@erase "$(INTDIR)\signature.sbr"
	-@erase "$(INTDIR)\snort.obj"
	-@erase "$(INTDIR)\snort.sbr"
	-@erase "$(INTDIR)\snprintf.obj"
	-@erase "$(INTDIR)\snprintf.sbr"
	-@erase "$(INTDIR)\sp_byte_check.obj"
	-@erase "$(INTDIR)\sp_byte_check.sbr"
	-@erase "$(INTDIR)\sp_byte_jump.obj"
	-@erase "$(INTDIR)\sp_byte_jump.sbr"
	-@erase "$(INTDIR)\sp_clientserver.obj"
	-@erase "$(INTDIR)\sp_clientserver.sbr"
	-@erase "$(INTDIR)\sp_dsize_check.obj"
	-@erase "$(INTDIR)\sp_dsize_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_code_check.obj"
	-@erase "$(INTDIR)\sp_icmp_code_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_id_check.obj"
	-@erase "$(INTDIR)\sp_icmp_id_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_seq_check.obj"
	-@erase "$(INTDIR)\sp_icmp_seq_check.sbr"
	-@erase "$(INTDIR)\sp_icmp_type_check.obj"
	-@erase "$(INTDIR)\sp_icmp_type_check.sbr"
	-@erase "$(INTDIR)\sp_ip_fragbits.obj"
	-@erase "$(INTDIR)\sp_ip_fragbits.sbr"
	-@erase "$(INTDIR)\sp_ip_id_check.obj"
	-@erase "$(INTDIR)\sp_ip_id_check.sbr"
	-@erase "$(INTDIR)\sp_ip_proto.obj"
	-@erase "$(INTDIR)\sp_ip_proto.sbr"
	-@erase "$(INTDIR)\sp_ip_same_check.obj"
	-@erase "$(INTDIR)\sp_ip_same_check.sbr"
	-@erase "$(INTDIR)\sp_ip_tos_check.obj"
	-@erase "$(INTDIR)\sp_ip_tos_check.sbr"
	-@erase "$(INTDIR)\sp_ipoption_check.obj"
	-@erase "$(INTDIR)\sp_ipoption_check.sbr"
	-@erase "$(INTDIR)\sp_pattern_match.obj"
	-@erase "$(INTDIR)\sp_pattern_match.sbr"
	-@erase "$(INTDIR)\sp_react.obj"
	-@erase "$(INTDIR)\sp_react.sbr"
	-@erase "$(INTDIR)\sp_respond.obj"
	-@erase "$(INTDIR)\sp_respond.sbr"
	-@erase "$(INTDIR)\sp_rpc_check.obj"
	-@erase "$(INTDIR)\sp_rpc_check.sbr"
	-@erase "$(INTDIR)\sp_session.obj"
	-@erase "$(INTDIR)\sp_session.sbr"
	-@erase "$(INTDIR)\sp_tcp_ack_check.obj"
	-@erase "$(INTDIR)\sp_tcp_ack_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_flag_check.obj"
	-@erase "$(INTDIR)\sp_tcp_flag_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_seq_check.obj"
	-@erase "$(INTDIR)\sp_tcp_seq_check.sbr"
	-@erase "$(INTDIR)\sp_tcp_win_check.obj"
	-@erase "$(INTDIR)\sp_tcp_win_check.sbr"
	-@erase "$(INTDIR)\sp_ttl_check.obj"
	-@erase "$(INTDIR)\sp_ttl_check.sbr"
	-@erase "$(INTDIR)\spo_alert_fast.obj"
	-@erase "$(INTDIR)\spo_alert_fast.sbr"
	-@erase "$(INTDIR)\spo_alert_full.obj"
	-@erase "$(INTDIR)\spo_alert_full.sbr"
	-@erase "$(INTDIR)\spo_alert_sf_socket.obj"
	-@erase "$(INTDIR)\spo_alert_sf_socket.sbr"
	-@erase "$(INTDIR)\spo_alert_smb.obj"
	-@erase "$(INTDIR)\spo_alert_smb.sbr"
	-@erase "$(INTDIR)\spo_alert_syslog.obj"
	-@erase "$(INTDIR)\spo_alert_syslog.sbr"
	-@erase "$(INTDIR)\spo_alert_unixsock.obj"
	-@erase "$(INTDIR)\spo_alert_unixsock.sbr"
	-@erase "$(INTDIR)\spo_csv.obj"
	-@erase "$(INTDIR)\spo_csv.sbr"
	-@erase "$(INTDIR)\spo_database.obj"
	-@erase "$(INTDIR)\spo_database.sbr"
	-@erase "$(INTDIR)\spo_log_ascii.obj"
	-@erase "$(INTDIR)\spo_log_ascii.sbr"
	-@erase "$(INTDIR)\spo_log_null.obj"
	-@erase "$(INTDIR)\spo_log_null.sbr"
	-@erase "$(INTDIR)\spo_log_tcpdump.obj"
	-@erase "$(INTDIR)\spo_log_tcpdump.sbr"
	-@erase "$(INTDIR)\spo_unified.obj"
	-@erase "$(INTDIR)\spo_unified.sbr"
	-@erase "$(INTDIR)\spp_arpspoof.obj"
	-@erase "$(INTDIR)\spp_arpspoof.sbr"
	-@erase "$(INTDIR)\spp_bo.obj"
	-@erase "$(INTDIR)\spp_bo.sbr"
	-@erase "$(INTDIR)\spp_conversation.obj"
	-@erase "$(INTDIR)\spp_conversation.sbr"
	-@erase "$(INTDIR)\spp_frag2.obj"
	-@erase "$(INTDIR)\spp_frag2.sbr"
	-@erase "$(INTDIR)\spp_http_decode.obj"
	-@erase "$(INTDIR)\spp_http_decode.sbr"
	-@erase "$(INTDIR)\spp_httpflow.obj"
	-@erase "$(INTDIR)\spp_httpflow.sbr"
	-@erase "$(INTDIR)\spp_perfmonitor.obj"
	-@erase "$(INTDIR)\spp_perfmonitor.sbr"
	-@erase "$(INTDIR)\spp_portscan.obj"
	-@erase "$(INTDIR)\spp_portscan.sbr"
	-@erase "$(INTDIR)\spp_portscan2.obj"
	-@erase "$(INTDIR)\spp_portscan2.sbr"
	-@erase "$(INTDIR)\spp_rpc_decode.obj"
	-@erase "$(INTDIR)\spp_rpc_decode.sbr"
	-@erase "$(INTDIR)\spp_stream4.obj"
	-@erase "$(INTDIR)\spp_stream4.sbr"
	-@erase "$(INTDIR)\spp_telnet_negotiation.obj"
	-@erase "$(INTDIR)\spp_telnet_negotiation.sbr"
	-@erase "$(INTDIR)\strlcatu.obj"
	-@erase "$(INTDIR)\strlcatu.sbr"
	-@erase "$(INTDIR)\strlcpyu.obj"
	-@erase "$(INTDIR)\strlcpyu.sbr"
	-@erase "$(INTDIR)\syslog.obj"
	-@erase "$(INTDIR)\syslog.sbr"
	-@erase "$(INTDIR)\tag.obj"
	-@erase "$(INTDIR)\tag.sbr"
	-@erase "$(INTDIR)\threshold.obj"
	-@erase "$(INTDIR)\threshold.sbr"
	-@erase "$(INTDIR)\ubi_BinTree.obj"
	-@erase "$(INTDIR)\ubi_BinTree.sbr"
	-@erase "$(INTDIR)\ubi_SplayTree.obj"
	-@erase "$(INTDIR)\ubi_SplayTree.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\win32_service.obj"
	-@erase "$(INTDIR)\win32_service.sbr"
	-@erase "$(OUTDIR)\snort.bsc"
	-@erase "$(OUTDIR)\snort.exe"
	-@erase "$(OUTDIR)\snort.ilk"
	-@erase "$(OUTDIR)\snort.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_WIN32_SERVICE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\snort.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\snort.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\acsmx.sbr" \
	"$(INTDIR)\byte_extract.sbr" \
	"$(INTDIR)\codes.sbr" \
	"$(INTDIR)\debug.sbr" \
	"$(INTDIR)\decode.sbr" \
	"$(INTDIR)\detect.sbr" \
	"$(INTDIR)\fpcreate.sbr" \
	"$(INTDIR)\fpdetect.sbr" \
	"$(INTDIR)\log.sbr" \
	"$(INTDIR)\mempool.sbr" \
	"$(INTDIR)\mpse.sbr" \
	"$(INTDIR)\mstring.sbr" \
	"$(INTDIR)\mwm.sbr" \
	"$(INTDIR)\parser.sbr" \
	"$(INTDIR)\pcrm.sbr" \
	"$(INTDIR)\plugbase.sbr" \
	"$(INTDIR)\sf_sdlist.sbr" \
	"$(INTDIR)\sfksearch.sbr" \
	"$(INTDIR)\signature.sbr" \
	"$(INTDIR)\snort.sbr" \
	"$(INTDIR)\snprintf.sbr" \
	"$(INTDIR)\strlcatu.sbr" \
	"$(INTDIR)\strlcpyu.sbr" \
	"$(INTDIR)\tag.sbr" \
	"$(INTDIR)\threshold.sbr" \
	"$(INTDIR)\ubi_BinTree.sbr" \
	"$(INTDIR)\ubi_SplayTree.sbr" \
	"$(INTDIR)\util.sbr" \
	"$(INTDIR)\getopt.sbr" \
	"$(INTDIR)\misc.sbr" \
	"$(INTDIR)\syslog.sbr" \
	"$(INTDIR)\win32_service.sbr" \
	"$(INTDIR)\sp_byte_check.sbr" \
	"$(INTDIR)\sp_byte_jump.sbr" \
	"$(INTDIR)\sp_clientserver.sbr" \
	"$(INTDIR)\sp_dsize_check.sbr" \
	"$(INTDIR)\sp_icmp_code_check.sbr" \
	"$(INTDIR)\sp_icmp_id_check.sbr" \
	"$(INTDIR)\sp_icmp_seq_check.sbr" \
	"$(INTDIR)\sp_icmp_type_check.sbr" \
	"$(INTDIR)\sp_ip_fragbits.sbr" \
	"$(INTDIR)\sp_ip_id_check.sbr" \
	"$(INTDIR)\sp_ip_proto.sbr" \
	"$(INTDIR)\sp_ip_same_check.sbr" \
	"$(INTDIR)\sp_ip_tos_check.sbr" \
	"$(INTDIR)\sp_ipoption_check.sbr" \
	"$(INTDIR)\sp_pattern_match.sbr" \
	"$(INTDIR)\sp_react.sbr" \
	"$(INTDIR)\sp_respond.sbr" \
	"$(INTDIR)\sp_rpc_check.sbr" \
	"$(INTDIR)\sp_session.sbr" \
	"$(INTDIR)\sp_tcp_ack_check.sbr" \
	"$(INTDIR)\sp_tcp_flag_check.sbr" \
	"$(INTDIR)\sp_tcp_seq_check.sbr" \
	"$(INTDIR)\sp_tcp_win_check.sbr" \
	"$(INTDIR)\sp_ttl_check.sbr" \
	"$(INTDIR)\spo_alert_fast.sbr" \
	"$(INTDIR)\spo_alert_full.sbr" \
	"$(INTDIR)\spo_alert_sf_socket.sbr" \
	"$(INTDIR)\spo_alert_smb.sbr" \
	"$(INTDIR)\spo_alert_syslog.sbr" \
	"$(INTDIR)\spo_alert_unixsock.sbr" \
	"$(INTDIR)\spo_csv.sbr" \
	"$(INTDIR)\spo_database.sbr" \
	"$(INTDIR)\spo_log_ascii.sbr" \
	"$(INTDIR)\spo_log_null.sbr" \
	"$(INTDIR)\spo_log_tcpdump.sbr" \
	"$(INTDIR)\spo_unified.sbr" \
	"$(INTDIR)\http-resp.sbr" \
	"$(INTDIR)\perf-base.sbr" \
	"$(INTDIR)\perf-event.sbr" \
	"$(INTDIR)\perf-flow.sbr" \
	"$(INTDIR)\perf.sbr" \
	"$(INTDIR)\sfprocpidstats.sbr" \
	"$(INTDIR)\spp_arpspoof.sbr" \
	"$(INTDIR)\spp_bo.sbr" \
	"$(INTDIR)\spp_conversation.sbr" \
	"$(INTDIR)\spp_frag2.sbr" \
	"$(INTDIR)\spp_http_decode.sbr" \
	"$(INTDIR)\spp_httpflow.sbr" \
	"$(INTDIR)\spp_perfmonitor.sbr" \
	"$(INTDIR)\spp_portscan.sbr" \
	"$(INTDIR)\spp_portscan2.sbr" \
	"$(INTDIR)\spp_rpc_decode.sbr" \
	"$(INTDIR)\spp_stream4.sbr" \
	"$(INTDIR)\spp_telnet_negotiation.sbr" \
	"$(INTDIR)\IpAddrSet.sbr"

"$(OUTDIR)\snort.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\snort.pdb" /debug /machine:I386 /out:"$(OUTDIR)\snort.exe" /pdbtype:sept /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" 
LINK32_OBJS= \
	"$(INTDIR)\acsmx.obj" \
	"$(INTDIR)\byte_extract.obj" \
	"$(INTDIR)\codes.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\decode.obj" \
	"$(INTDIR)\detect.obj" \
	"$(INTDIR)\fpcreate.obj" \
	"$(INTDIR)\fpdetect.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\mempool.obj" \
	"$(INTDIR)\mpse.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\mwm.obj" \
	"$(INTDIR)\parser.obj" \
	"$(INTDIR)\pcrm.obj" \
	"$(INTDIR)\plugbase.obj" \
	"$(INTDIR)\sf_sdlist.obj" \
	"$(INTDIR)\sfksearch.obj" \
	"$(INTDIR)\signature.obj" \
	"$(INTDIR)\snort.obj" \
	"$(INTDIR)\snprintf.obj" \
	"$(INTDIR)\strlcatu.obj" \
	"$(INTDIR)\strlcpyu.obj" \
	"$(INTDIR)\tag.obj" \
	"$(INTDIR)\threshold.obj" \
	"$(INTDIR)\ubi_BinTree.obj" \
	"$(INTDIR)\ubi_SplayTree.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\syslog.obj" \
	"$(INTDIR)\win32_service.obj" \
	"$(INTDIR)\sp_byte_check.obj" \
	"$(INTDIR)\sp_byte_jump.obj" \
	"$(INTDIR)\sp_clientserver.obj" \
	"$(INTDIR)\sp_dsize_check.obj" \
	"$(INTDIR)\sp_icmp_code_check.obj" \
	"$(INTDIR)\sp_icmp_id_check.obj" \
	"$(INTDIR)\sp_icmp_seq_check.obj" \
	"$(INTDIR)\sp_icmp_type_check.obj" \
	"$(INTDIR)\sp_ip_fragbits.obj" \
	"$(INTDIR)\sp_ip_id_check.obj" \
	"$(INTDIR)\sp_ip_proto.obj" \
	"$(INTDIR)\sp_ip_same_check.obj" \
	"$(INTDIR)\sp_ip_tos_check.obj" \
	"$(INTDIR)\sp_ipoption_check.obj" \
	"$(INTDIR)\sp_pattern_match.obj" \
	"$(INTDIR)\sp_react.obj" \
	"$(INTDIR)\sp_respond.obj" \
	"$(INTDIR)\sp_rpc_check.obj" \
	"$(INTDIR)\sp_session.obj" \
	"$(INTDIR)\sp_tcp_ack_check.obj" \
	"$(INTDIR)\sp_tcp_flag_check.obj" \
	"$(INTDIR)\sp_tcp_seq_check.obj" \
	"$(INTDIR)\sp_tcp_win_check.obj" \
	"$(INTDIR)\sp_ttl_check.obj" \
	"$(INTDIR)\spo_alert_fast.obj" \
	"$(INTDIR)\spo_alert_full.obj" \
	"$(INTDIR)\spo_alert_sf_socket.obj" \
	"$(INTDIR)\spo_alert_smb.obj" \
	"$(INTDIR)\spo_alert_syslog.obj" \
	"$(INTDIR)\spo_alert_unixsock.obj" \
	"$(INTDIR)\spo_csv.obj" \
	"$(INTDIR)\spo_database.obj" \
	"$(INTDIR)\spo_log_ascii.obj" \
	"$(INTDIR)\spo_log_null.obj" \
	"$(INTDIR)\spo_log_tcpdump.obj" \
	"$(INTDIR)\spo_unified.obj" \
	"$(INTDIR)\http-resp.obj" \
	"$(INTDIR)\perf-base.obj" \
	"$(INTDIR)\perf-event.obj" \
	"$(INTDIR)\perf-flow.obj" \
	"$(INTDIR)\perf.obj" \
	"$(INTDIR)\sfprocpidstats.obj" \
	"$(INTDIR)\spp_arpspoof.obj" \
	"$(INTDIR)\spp_bo.obj" \
	"$(INTDIR)\spp_conversation.obj" \
	"$(INTDIR)\spp_frag2.obj" \
	"$(INTDIR)\spp_http_decode.obj" \
	"$(INTDIR)\spp_httpflow.obj" \
	"$(INTDIR)\spp_perfmonitor.obj" \
	"$(INTDIR)\spp_portscan.obj" \
	"$(INTDIR)\spp_portscan2.obj" \
	"$(INTDIR)\spp_rpc_decode.obj" \
	"$(INTDIR)\spp_stream4.obj" \
	"$(INTDIR)\spp_telnet_negotiation.obj" \
	"$(INTDIR)\IpAddrSet.obj"

"$(OUTDIR)\snort.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"

OUTDIR=.\snort___Win32_StdDB_Service_Release
INTDIR=.\snort___Win32_StdDB_Service_Release
# Begin Custom Macros
OutDir=.\snort___Win32_StdDB_Service_Release
# End Custom Macros

ALL : "$(OUTDIR)\snort.exe"


CLEAN :
	-@erase "$(INTDIR)\acsmx.obj"
	-@erase "$(INTDIR)\byte_extract.obj"
	-@erase "$(INTDIR)\codes.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\decode.obj"
	-@erase "$(INTDIR)\detect.obj"
	-@erase "$(INTDIR)\fpcreate.obj"
	-@erase "$(INTDIR)\fpdetect.obj"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\http-resp.obj"
	-@erase "$(INTDIR)\IpAddrSet.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\mempool.obj"
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\mpse.obj"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\mwm.obj"
	-@erase "$(INTDIR)\parser.obj"
	-@erase "$(INTDIR)\pcrm.obj"
	-@erase "$(INTDIR)\perf-base.obj"
	-@erase "$(INTDIR)\perf-event.obj"
	-@erase "$(INTDIR)\perf-flow.obj"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\plugbase.obj"
	-@erase "$(INTDIR)\sf_sdlist.obj"
	-@erase "$(INTDIR)\sfksearch.obj"
	-@erase "$(INTDIR)\sfprocpidstats.obj"
	-@erase "$(INTDIR)\signature.obj"
	-@erase "$(INTDIR)\snort.obj"
	-@erase "$(INTDIR)\snprintf.obj"
	-@erase "$(INTDIR)\sp_byte_check.obj"
	-@erase "$(INTDIR)\sp_byte_jump.obj"
	-@erase "$(INTDIR)\sp_clientserver.obj"
	-@erase "$(INTDIR)\sp_dsize_check.obj"
	-@erase "$(INTDIR)\sp_icmp_code_check.obj"
	-@erase "$(INTDIR)\sp_icmp_id_check.obj"
	-@erase "$(INTDIR)\sp_icmp_seq_check.obj"
	-@erase "$(INTDIR)\sp_icmp_type_check.obj"
	-@erase "$(INTDIR)\sp_ip_fragbits.obj"
	-@erase "$(INTDIR)\sp_ip_id_check.obj"
	-@erase "$(INTDIR)\sp_ip_proto.obj"
	-@erase "$(INTDIR)\sp_ip_same_check.obj"
	-@erase "$(INTDIR)\sp_ip_tos_check.obj"
	-@erase "$(INTDIR)\sp_ipoption_check.obj"
	-@erase "$(INTDIR)\sp_pattern_match.obj"
	-@erase "$(INTDIR)\sp_react.obj"
	-@erase "$(INTDIR)\sp_respond.obj"
	-@erase "$(INTDIR)\sp_rpc_check.obj"
	-@erase "$(INTDIR)\sp_session.obj"
	-@erase "$(INTDIR)\sp_tcp_ack_check.obj"
	-@erase "$(INTDIR)\sp_tcp_flag_check.obj"
	-@erase "$(INTDIR)\sp_tcp_seq_check.obj"
	-@erase "$(INTDIR)\sp_tcp_win_check.obj"
	-@erase "$(INTDIR)\sp_ttl_check.obj"
	-@erase "$(INTDIR)\spo_alert_fast.obj"
	-@erase "$(INTDIR)\spo_alert_full.obj"
	-@erase "$(INTDIR)\spo_alert_sf_socket.obj"
	-@erase "$(INTDIR)\spo_alert_smb.obj"
	-@erase "$(INTDIR)\spo_alert_syslog.obj"
	-@erase "$(INTDIR)\spo_alert_unixsock.obj"
	-@erase "$(INTDIR)\spo_csv.obj"
	-@erase "$(INTDIR)\spo_database.obj"
	-@erase "$(INTDIR)\spo_log_ascii.obj"
	-@erase "$(INTDIR)\spo_log_null.obj"
	-@erase "$(INTDIR)\spo_log_tcpdump.obj"
	-@erase "$(INTDIR)\spo_unified.obj"
	-@erase "$(INTDIR)\spp_arpspoof.obj"
	-@erase "$(INTDIR)\spp_bo.obj"
	-@erase "$(INTDIR)\spp_conversation.obj"
	-@erase "$(INTDIR)\spp_frag2.obj"
	-@erase "$(INTDIR)\spp_http_decode.obj"
	-@erase "$(INTDIR)\spp_httpflow.obj"
	-@erase "$(INTDIR)\spp_perfmonitor.obj"
	-@erase "$(INTDIR)\spp_portscan.obj"
	-@erase "$(INTDIR)\spp_portscan2.obj"
	-@erase "$(INTDIR)\spp_rpc_decode.obj"
	-@erase "$(INTDIR)\spp_stream4.obj"
	-@erase "$(INTDIR)\spp_telnet_negotiation.obj"
	-@erase "$(INTDIR)\strlcatu.obj"
	-@erase "$(INTDIR)\strlcpyu.obj"
	-@erase "$(INTDIR)\syslog.obj"
	-@erase "$(INTDIR)\tag.obj"
	-@erase "$(INTDIR)\threshold.obj"
	-@erase "$(INTDIR)\ubi_BinTree.obj"
	-@erase "$(INTDIR)\ubi_SplayTree.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\win32_service.obj"
	-@erase "$(OUTDIR)\snort.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\Win32-Includes" /I "..\Win32-Includes\mysql" /I "..\..\detection-plugins" /I "..\..\output-plugins" /I "..\..\preprocessors" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D __BEGIN_DECLS="" /D __END_DECLS="" /D "HAVE_CONFIG_H" /D "ENABLE_MYSQL" /D "ENABLE_ODBC" /D "ENABLE_WIN32_SERVICE" /Fp"$(INTDIR)\snort.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\snort.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=user32.lib wsock32.lib libpcap.lib advapi32.lib mysqlclient.lib odbc32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\snort.pdb" /machine:I386 /out:"$(OUTDIR)\snort.exe" /libpath:"..\Win32-Libraries" /libpath:"..\Win32-Libraries\mysql" 
LINK32_OBJS= \
	"$(INTDIR)\acsmx.obj" \
	"$(INTDIR)\byte_extract.obj" \
	"$(INTDIR)\codes.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\decode.obj" \
	"$(INTDIR)\detect.obj" \
	"$(INTDIR)\fpcreate.obj" \
	"$(INTDIR)\fpdetect.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\mempool.obj" \
	"$(INTDIR)\mpse.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\mwm.obj" \
	"$(INTDIR)\parser.obj" \
	"$(INTDIR)\pcrm.obj" \
	"$(INTDIR)\plugbase.obj" \
	"$(INTDIR)\sf_sdlist.obj" \
	"$(INTDIR)\sfksearch.obj" \
	"$(INTDIR)\signature.obj" \
	"$(INTDIR)\snort.obj" \
	"$(INTDIR)\snprintf.obj" \
	"$(INTDIR)\strlcatu.obj" \
	"$(INTDIR)\strlcpyu.obj" \
	"$(INTDIR)\tag.obj" \
	"$(INTDIR)\threshold.obj" \
	"$(INTDIR)\ubi_BinTree.obj" \
	"$(INTDIR)\ubi_SplayTree.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\syslog.obj" \
	"$(INTDIR)\win32_service.obj" \
	"$(INTDIR)\sp_byte_check.obj" \
	"$(INTDIR)\sp_byte_jump.obj" \
	"$(INTDIR)\sp_clientserver.obj" \
	"$(INTDIR)\sp_dsize_check.obj" \
	"$(INTDIR)\sp_icmp_code_check.obj" \
	"$(INTDIR)\sp_icmp_id_check.obj" \
	"$(INTDIR)\sp_icmp_seq_check.obj" \
	"$(INTDIR)\sp_icmp_type_check.obj" \
	"$(INTDIR)\sp_ip_fragbits.obj" \
	"$(INTDIR)\sp_ip_id_check.obj" \
	"$(INTDIR)\sp_ip_proto.obj" \
	"$(INTDIR)\sp_ip_same_check.obj" \
	"$(INTDIR)\sp_ip_tos_check.obj" \
	"$(INTDIR)\sp_ipoption_check.obj" \
	"$(INTDIR)\sp_pattern_match.obj" \
	"$(INTDIR)\sp_react.obj" \
	"$(INTDIR)\sp_respond.obj" \
	"$(INTDIR)\sp_rpc_check.obj" \
	"$(INTDIR)\sp_session.obj" \
	"$(INTDIR)\sp_tcp_ack_check.obj" \
	"$(INTDIR)\sp_tcp_flag_check.obj" \
	"$(INTDIR)\sp_tcp_seq_check.obj" \
	"$(INTDIR)\sp_tcp_win_check.obj" \
	"$(INTDIR)\sp_ttl_check.obj" \
	"$(INTDIR)\spo_alert_fast.obj" \
	"$(INTDIR)\spo_alert_full.obj" \
	"$(INTDIR)\spo_alert_sf_socket.obj" \
	"$(INTDIR)\spo_alert_smb.obj" \
	"$(INTDIR)\spo_alert_syslog.obj" \
	"$(INTDIR)\spo_alert_unixsock.obj" \
	"$(INTDIR)\spo_csv.obj" \
	"$(INTDIR)\spo_database.obj" \
	"$(INTDIR)\spo_log_ascii.obj" \
	"$(INTDIR)\spo_log_null.obj" \
	"$(INTDIR)\spo_log_tcpdump.obj" \
	"$(INTDIR)\spo_unified.obj" \
	"$(INTDIR)\http-resp.obj" \
	"$(INTDIR)\perf-base.obj" \
	"$(INTDIR)\perf-event.obj" \
	"$(INTDIR)\perf-flow.obj" \
	"$(INTDIR)\perf.obj" \
	"$(INTDIR)\sfprocpidstats.obj" \
	"$(INTDIR)\spp_arpspoof.obj" \
	"$(INTDIR)\spp_bo.obj" \
	"$(INTDIR)\spp_conversation.obj" \
	"$(INTDIR)\spp_frag2.obj" \
	"$(INTDIR)\spp_http_decode.obj" \
	"$(INTDIR)\spp_httpflow.obj" \
	"$(INTDIR)\spp_perfmonitor.obj" \
	"$(INTDIR)\spp_portscan.obj" \
	"$(INTDIR)\spp_portscan2.obj" \
	"$(INTDIR)\spp_rpc_decode.obj" \
	"$(INTDIR)\spp_stream4.obj" \
	"$(INTDIR)\spp_telnet_negotiation.obj" \
	"$(INTDIR)\IpAddrSet.obj"

"$(OUTDIR)\snort.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("snort.dep")
!INCLUDE "snort.dep"
!ELSE 
!MESSAGE Warning: cannot find "snort.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "snort - Win32 StdDB Debug" || "$(CFG)" == "snort - Win32 StdDB Release" || "$(CFG)" == "snort - Win32 StdDB FlexResp Debug" || "$(CFG)" == "snort - Win32 StdDB FlexResp Release" || "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug" || "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release" || "$(CFG)" == "snort - Win32 StdDB MSSQL Debug" || "$(CFG)" == "snort - Win32 StdDB MSSQL Release" || "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug" || "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release" || "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug" || "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release" || "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug" || "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release" || "$(CFG)" == "snort - Win32 StdDB Service Debug" || "$(CFG)" == "snort - Win32 StdDB Service Release"
SOURCE=..\..\acsmx.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\acsmx.obj"	"$(INTDIR)\acsmx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\acsmx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\acsmx.obj"	"$(INTDIR)\acsmx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\acsmx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\acsmx.obj"	"$(INTDIR)\acsmx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\acsmx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\acsmx.obj"	"$(INTDIR)\acsmx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\acsmx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\acsmx.obj"	"$(INTDIR)\acsmx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\acsmx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\acsmx.obj"	"$(INTDIR)\acsmx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\acsmx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\acsmx.obj"	"$(INTDIR)\acsmx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\acsmx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\acsmx.obj"	"$(INTDIR)\acsmx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\acsmx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\byte_extract.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\byte_extract.obj"	"$(INTDIR)\byte_extract.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\byte_extract.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\byte_extract.obj"	"$(INTDIR)\byte_extract.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\byte_extract.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\byte_extract.obj"	"$(INTDIR)\byte_extract.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\byte_extract.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\byte_extract.obj"	"$(INTDIR)\byte_extract.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\byte_extract.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\byte_extract.obj"	"$(INTDIR)\byte_extract.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\byte_extract.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\byte_extract.obj"	"$(INTDIR)\byte_extract.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\byte_extract.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\byte_extract.obj"	"$(INTDIR)\byte_extract.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\byte_extract.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\byte_extract.obj"	"$(INTDIR)\byte_extract.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\byte_extract.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\codes.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\codes.obj"	"$(INTDIR)\codes.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\codes.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\codes.obj"	"$(INTDIR)\codes.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\codes.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\codes.obj"	"$(INTDIR)\codes.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\codes.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\codes.obj"	"$(INTDIR)\codes.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\codes.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\codes.obj"	"$(INTDIR)\codes.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\codes.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\codes.obj"	"$(INTDIR)\codes.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\codes.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\codes.obj"	"$(INTDIR)\codes.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\codes.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\codes.obj"	"$(INTDIR)\codes.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\codes.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\debug.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\debug.obj"	"$(INTDIR)\debug.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\debug.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\debug.obj"	"$(INTDIR)\debug.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\debug.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\debug.obj"	"$(INTDIR)\debug.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\debug.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\debug.obj"	"$(INTDIR)\debug.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\debug.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\debug.obj"	"$(INTDIR)\debug.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\debug.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\debug.obj"	"$(INTDIR)\debug.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\debug.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\debug.obj"	"$(INTDIR)\debug.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\debug.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\debug.obj"	"$(INTDIR)\debug.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\debug.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\decode.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\decode.obj"	"$(INTDIR)\decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\decode.obj"	"$(INTDIR)\decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\decode.obj"	"$(INTDIR)\decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\decode.obj"	"$(INTDIR)\decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\decode.obj"	"$(INTDIR)\decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\decode.obj"	"$(INTDIR)\decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\decode.obj"	"$(INTDIR)\decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\decode.obj"	"$(INTDIR)\decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\detect.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\detect.obj"	"$(INTDIR)\detect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\detect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\detect.obj"	"$(INTDIR)\detect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\detect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\detect.obj"	"$(INTDIR)\detect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\detect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\detect.obj"	"$(INTDIR)\detect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\detect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\detect.obj"	"$(INTDIR)\detect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\detect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\detect.obj"	"$(INTDIR)\detect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\detect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\detect.obj"	"$(INTDIR)\detect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\detect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\detect.obj"	"$(INTDIR)\detect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\detect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\fpcreate.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\fpcreate.obj"	"$(INTDIR)\fpcreate.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\fpcreate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\fpcreate.obj"	"$(INTDIR)\fpcreate.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\fpcreate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\fpcreate.obj"	"$(INTDIR)\fpcreate.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\fpcreate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\fpcreate.obj"	"$(INTDIR)\fpcreate.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\fpcreate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\fpcreate.obj"	"$(INTDIR)\fpcreate.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\fpcreate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\fpcreate.obj"	"$(INTDIR)\fpcreate.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\fpcreate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\fpcreate.obj"	"$(INTDIR)\fpcreate.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\fpcreate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\fpcreate.obj"	"$(INTDIR)\fpcreate.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\fpcreate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\fpdetect.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\fpdetect.obj"	"$(INTDIR)\fpdetect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\fpdetect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\fpdetect.obj"	"$(INTDIR)\fpdetect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\fpdetect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\fpdetect.obj"	"$(INTDIR)\fpdetect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\fpdetect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\fpdetect.obj"	"$(INTDIR)\fpdetect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\fpdetect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\fpdetect.obj"	"$(INTDIR)\fpdetect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\fpdetect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\fpdetect.obj"	"$(INTDIR)\fpdetect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\fpdetect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\fpdetect.obj"	"$(INTDIR)\fpdetect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\fpdetect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\fpdetect.obj"	"$(INTDIR)\fpdetect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\fpdetect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\log.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\log.obj"	"$(INTDIR)\log.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\log.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\log.obj"	"$(INTDIR)\log.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\log.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\log.obj"	"$(INTDIR)\log.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\log.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\log.obj"	"$(INTDIR)\log.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\log.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\log.obj"	"$(INTDIR)\log.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\log.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\log.obj"	"$(INTDIR)\log.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\log.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\log.obj"	"$(INTDIR)\log.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\log.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\log.obj"	"$(INTDIR)\log.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\log.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\mempool.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\mempool.obj"	"$(INTDIR)\mempool.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\mempool.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\mempool.obj"	"$(INTDIR)\mempool.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\mempool.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\mempool.obj"	"$(INTDIR)\mempool.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\mempool.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\mempool.obj"	"$(INTDIR)\mempool.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\mempool.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\mempool.obj"	"$(INTDIR)\mempool.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\mempool.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\mempool.obj"	"$(INTDIR)\mempool.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\mempool.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\mempool.obj"	"$(INTDIR)\mempool.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\mempool.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\mempool.obj"	"$(INTDIR)\mempool.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\mempool.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\mpse.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\mpse.obj"	"$(INTDIR)\mpse.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\mpse.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\mpse.obj"	"$(INTDIR)\mpse.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\mpse.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\mpse.obj"	"$(INTDIR)\mpse.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\mpse.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\mpse.obj"	"$(INTDIR)\mpse.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\mpse.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\mpse.obj"	"$(INTDIR)\mpse.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\mpse.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\mpse.obj"	"$(INTDIR)\mpse.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\mpse.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\mpse.obj"	"$(INTDIR)\mpse.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\mpse.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\mpse.obj"	"$(INTDIR)\mpse.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\mpse.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\mstring.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\mstring.obj"	"$(INTDIR)\mstring.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\mstring.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\mstring.obj"	"$(INTDIR)\mstring.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\mstring.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\mstring.obj"	"$(INTDIR)\mstring.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\mstring.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\mstring.obj"	"$(INTDIR)\mstring.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\mstring.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\mstring.obj"	"$(INTDIR)\mstring.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\mstring.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\mstring.obj"	"$(INTDIR)\mstring.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\mstring.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\mstring.obj"	"$(INTDIR)\mstring.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\mstring.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\mstring.obj"	"$(INTDIR)\mstring.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\mstring.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\mwm.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\mwm.obj"	"$(INTDIR)\mwm.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\mwm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\mwm.obj"	"$(INTDIR)\mwm.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\mwm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\mwm.obj"	"$(INTDIR)\mwm.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\mwm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\mwm.obj"	"$(INTDIR)\mwm.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\mwm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\mwm.obj"	"$(INTDIR)\mwm.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\mwm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\mwm.obj"	"$(INTDIR)\mwm.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\mwm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\mwm.obj"	"$(INTDIR)\mwm.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\mwm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\mwm.obj"	"$(INTDIR)\mwm.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\mwm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\parser.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\parser.obj"	"$(INTDIR)\parser.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\parser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\parser.obj"	"$(INTDIR)\parser.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\parser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\parser.obj"	"$(INTDIR)\parser.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\parser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\parser.obj"	"$(INTDIR)\parser.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\parser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\parser.obj"	"$(INTDIR)\parser.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\parser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\parser.obj"	"$(INTDIR)\parser.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\parser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\parser.obj"	"$(INTDIR)\parser.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\parser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\parser.obj"	"$(INTDIR)\parser.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\parser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\pcrm.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\pcrm.obj"	"$(INTDIR)\pcrm.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\pcrm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\pcrm.obj"	"$(INTDIR)\pcrm.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\pcrm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\pcrm.obj"	"$(INTDIR)\pcrm.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\pcrm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\pcrm.obj"	"$(INTDIR)\pcrm.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\pcrm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\pcrm.obj"	"$(INTDIR)\pcrm.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\pcrm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\pcrm.obj"	"$(INTDIR)\pcrm.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\pcrm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\pcrm.obj"	"$(INTDIR)\pcrm.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\pcrm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\pcrm.obj"	"$(INTDIR)\pcrm.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\pcrm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\plugbase.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\plugbase.obj"	"$(INTDIR)\plugbase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\plugbase.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\plugbase.obj"	"$(INTDIR)\plugbase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\plugbase.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\plugbase.obj"	"$(INTDIR)\plugbase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\plugbase.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\plugbase.obj"	"$(INTDIR)\plugbase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\plugbase.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\plugbase.obj"	"$(INTDIR)\plugbase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\plugbase.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\plugbase.obj"	"$(INTDIR)\plugbase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\plugbase.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\plugbase.obj"	"$(INTDIR)\plugbase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\plugbase.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\plugbase.obj"	"$(INTDIR)\plugbase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\plugbase.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\sf_sdlist.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sf_sdlist.obj"	"$(INTDIR)\sf_sdlist.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sf_sdlist.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sf_sdlist.obj"	"$(INTDIR)\sf_sdlist.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sf_sdlist.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sf_sdlist.obj"	"$(INTDIR)\sf_sdlist.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sf_sdlist.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sf_sdlist.obj"	"$(INTDIR)\sf_sdlist.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sf_sdlist.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sf_sdlist.obj"	"$(INTDIR)\sf_sdlist.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sf_sdlist.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sf_sdlist.obj"	"$(INTDIR)\sf_sdlist.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sf_sdlist.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sf_sdlist.obj"	"$(INTDIR)\sf_sdlist.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sf_sdlist.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sf_sdlist.obj"	"$(INTDIR)\sf_sdlist.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sf_sdlist.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\sfksearch.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sfksearch.obj"	"$(INTDIR)\sfksearch.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sfksearch.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sfksearch.obj"	"$(INTDIR)\sfksearch.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sfksearch.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sfksearch.obj"	"$(INTDIR)\sfksearch.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sfksearch.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sfksearch.obj"	"$(INTDIR)\sfksearch.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sfksearch.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sfksearch.obj"	"$(INTDIR)\sfksearch.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sfksearch.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sfksearch.obj"	"$(INTDIR)\sfksearch.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sfksearch.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sfksearch.obj"	"$(INTDIR)\sfksearch.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sfksearch.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sfksearch.obj"	"$(INTDIR)\sfksearch.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sfksearch.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\signature.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\signature.obj"	"$(INTDIR)\signature.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\signature.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\signature.obj"	"$(INTDIR)\signature.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\signature.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\signature.obj"	"$(INTDIR)\signature.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\signature.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\signature.obj"	"$(INTDIR)\signature.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\signature.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\signature.obj"	"$(INTDIR)\signature.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\signature.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\signature.obj"	"$(INTDIR)\signature.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\signature.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\signature.obj"	"$(INTDIR)\signature.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\signature.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\signature.obj"	"$(INTDIR)\signature.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\signature.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\snort.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\snort.obj"	"$(INTDIR)\snort.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\snort.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\snort.obj"	"$(INTDIR)\snort.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\snort.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\snort.obj"	"$(INTDIR)\snort.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\snort.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\snort.obj"	"$(INTDIR)\snort.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\snort.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\snort.obj"	"$(INTDIR)\snort.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\snort.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\snort.obj"	"$(INTDIR)\snort.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\snort.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\snort.obj"	"$(INTDIR)\snort.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\snort.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\snort.obj"	"$(INTDIR)\snort.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\snort.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\snprintf.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\snprintf.obj"	"$(INTDIR)\snprintf.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\snprintf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\snprintf.obj"	"$(INTDIR)\snprintf.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\snprintf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\snprintf.obj"	"$(INTDIR)\snprintf.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\snprintf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\snprintf.obj"	"$(INTDIR)\snprintf.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\snprintf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\snprintf.obj"	"$(INTDIR)\snprintf.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\snprintf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\snprintf.obj"	"$(INTDIR)\snprintf.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\snprintf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\snprintf.obj"	"$(INTDIR)\snprintf.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\snprintf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\snprintf.obj"	"$(INTDIR)\snprintf.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\snprintf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\strlcatu.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\strlcatu.obj"	"$(INTDIR)\strlcatu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\strlcatu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\strlcatu.obj"	"$(INTDIR)\strlcatu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\strlcatu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\strlcatu.obj"	"$(INTDIR)\strlcatu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\strlcatu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\strlcatu.obj"	"$(INTDIR)\strlcatu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\strlcatu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\strlcatu.obj"	"$(INTDIR)\strlcatu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\strlcatu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\strlcatu.obj"	"$(INTDIR)\strlcatu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\strlcatu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\strlcatu.obj"	"$(INTDIR)\strlcatu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\strlcatu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\strlcatu.obj"	"$(INTDIR)\strlcatu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\strlcatu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\strlcpyu.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\strlcpyu.obj"	"$(INTDIR)\strlcpyu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\strlcpyu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\strlcpyu.obj"	"$(INTDIR)\strlcpyu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\strlcpyu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\strlcpyu.obj"	"$(INTDIR)\strlcpyu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\strlcpyu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\strlcpyu.obj"	"$(INTDIR)\strlcpyu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\strlcpyu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\strlcpyu.obj"	"$(INTDIR)\strlcpyu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\strlcpyu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\strlcpyu.obj"	"$(INTDIR)\strlcpyu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\strlcpyu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\strlcpyu.obj"	"$(INTDIR)\strlcpyu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\strlcpyu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\strlcpyu.obj"	"$(INTDIR)\strlcpyu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\strlcpyu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\tag.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\tag.obj"	"$(INTDIR)\tag.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\tag.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\tag.obj"	"$(INTDIR)\tag.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\tag.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\tag.obj"	"$(INTDIR)\tag.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\tag.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\tag.obj"	"$(INTDIR)\tag.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\tag.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\tag.obj"	"$(INTDIR)\tag.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\tag.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\tag.obj"	"$(INTDIR)\tag.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\tag.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\tag.obj"	"$(INTDIR)\tag.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\tag.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\tag.obj"	"$(INTDIR)\tag.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\tag.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\threshold.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\threshold.obj"	"$(INTDIR)\threshold.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\threshold.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\threshold.obj"	"$(INTDIR)\threshold.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\threshold.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\threshold.obj"	"$(INTDIR)\threshold.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\threshold.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\threshold.obj"	"$(INTDIR)\threshold.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\threshold.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\threshold.obj"	"$(INTDIR)\threshold.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\threshold.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\threshold.obj"	"$(INTDIR)\threshold.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\threshold.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\threshold.obj"	"$(INTDIR)\threshold.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\threshold.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\threshold.obj"	"$(INTDIR)\threshold.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\threshold.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\ubi_BinTree.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\ubi_BinTree.obj"	"$(INTDIR)\ubi_BinTree.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\ubi_BinTree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\ubi_BinTree.obj"	"$(INTDIR)\ubi_BinTree.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\ubi_BinTree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\ubi_BinTree.obj"	"$(INTDIR)\ubi_BinTree.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\ubi_BinTree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\ubi_BinTree.obj"	"$(INTDIR)\ubi_BinTree.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\ubi_BinTree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\ubi_BinTree.obj"	"$(INTDIR)\ubi_BinTree.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\ubi_BinTree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\ubi_BinTree.obj"	"$(INTDIR)\ubi_BinTree.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\ubi_BinTree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\ubi_BinTree.obj"	"$(INTDIR)\ubi_BinTree.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\ubi_BinTree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\ubi_BinTree.obj"	"$(INTDIR)\ubi_BinTree.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\ubi_BinTree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\ubi_SplayTree.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\ubi_SplayTree.obj"	"$(INTDIR)\ubi_SplayTree.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\ubi_SplayTree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\ubi_SplayTree.obj"	"$(INTDIR)\ubi_SplayTree.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\ubi_SplayTree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\ubi_SplayTree.obj"	"$(INTDIR)\ubi_SplayTree.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\ubi_SplayTree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\ubi_SplayTree.obj"	"$(INTDIR)\ubi_SplayTree.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\ubi_SplayTree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\ubi_SplayTree.obj"	"$(INTDIR)\ubi_SplayTree.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\ubi_SplayTree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\ubi_SplayTree.obj"	"$(INTDIR)\ubi_SplayTree.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\ubi_SplayTree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\ubi_SplayTree.obj"	"$(INTDIR)\ubi_SplayTree.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\ubi_SplayTree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\ubi_SplayTree.obj"	"$(INTDIR)\ubi_SplayTree.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\ubi_SplayTree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\util.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\util.obj"	"$(INTDIR)\util.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\util.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\util.obj"	"$(INTDIR)\util.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\util.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\util.obj"	"$(INTDIR)\util.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\util.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\util.obj"	"$(INTDIR)\util.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\util.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\util.obj"	"$(INTDIR)\util.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\util.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\util.obj"	"$(INTDIR)\util.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\util.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\util.obj"	"$(INTDIR)\util.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\util.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\util.obj"	"$(INTDIR)\util.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\util.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\WIN32-Code\name.rc"
SOURCE="..\WIN32-Code\getopt.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\getopt.obj"	"$(INTDIR)\getopt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\getopt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\getopt.obj"	"$(INTDIR)\getopt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\getopt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\getopt.obj"	"$(INTDIR)\getopt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\getopt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\getopt.obj"	"$(INTDIR)\getopt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\getopt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\getopt.obj"	"$(INTDIR)\getopt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\getopt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\getopt.obj"	"$(INTDIR)\getopt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\getopt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\getopt.obj"	"$(INTDIR)\getopt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\getopt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\getopt.obj"	"$(INTDIR)\getopt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\getopt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\WIN32-Code\misc.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\misc.obj"	"$(INTDIR)\misc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\misc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\misc.obj"	"$(INTDIR)\misc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\misc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\misc.obj"	"$(INTDIR)\misc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\misc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\misc.obj"	"$(INTDIR)\misc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\misc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\misc.obj"	"$(INTDIR)\misc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\misc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\misc.obj"	"$(INTDIR)\misc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\misc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\misc.obj"	"$(INTDIR)\misc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\misc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\misc.obj"	"$(INTDIR)\misc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\misc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\WIN32-Code\syslog.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\syslog.obj"	"$(INTDIR)\syslog.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\syslog.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\syslog.obj"	"$(INTDIR)\syslog.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\syslog.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\syslog.obj"	"$(INTDIR)\syslog.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\syslog.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\syslog.obj"	"$(INTDIR)\syslog.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\syslog.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\syslog.obj"	"$(INTDIR)\syslog.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\syslog.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\syslog.obj"	"$(INTDIR)\syslog.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\syslog.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\syslog.obj"	"$(INTDIR)\syslog.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\syslog.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\syslog.obj"	"$(INTDIR)\syslog.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\syslog.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\WIN32-Code\win32_service.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\win32_service.obj"	"$(INTDIR)\win32_service.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\win32_service.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\win32_service.obj"	"$(INTDIR)\win32_service.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\win32_service.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\win32_service.obj"	"$(INTDIR)\win32_service.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\win32_service.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\win32_service.obj"	"$(INTDIR)\win32_service.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\win32_service.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\win32_service.obj"	"$(INTDIR)\win32_service.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\win32_service.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\win32_service.obj"	"$(INTDIR)\win32_service.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\win32_service.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\win32_service.obj"	"$(INTDIR)\win32_service.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\win32_service.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\win32_service.obj"	"$(INTDIR)\win32_service.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\win32_service.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_byte_check.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_byte_check.obj"	"$(INTDIR)\sp_byte_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_byte_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_byte_check.obj"	"$(INTDIR)\sp_byte_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_byte_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_byte_check.obj"	"$(INTDIR)\sp_byte_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_byte_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_byte_check.obj"	"$(INTDIR)\sp_byte_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_byte_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_byte_check.obj"	"$(INTDIR)\sp_byte_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_byte_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_byte_check.obj"	"$(INTDIR)\sp_byte_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_byte_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_byte_check.obj"	"$(INTDIR)\sp_byte_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_byte_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_byte_check.obj"	"$(INTDIR)\sp_byte_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_byte_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_byte_jump.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_byte_jump.obj"	"$(INTDIR)\sp_byte_jump.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_byte_jump.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_byte_jump.obj"	"$(INTDIR)\sp_byte_jump.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_byte_jump.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_byte_jump.obj"	"$(INTDIR)\sp_byte_jump.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_byte_jump.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_byte_jump.obj"	"$(INTDIR)\sp_byte_jump.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_byte_jump.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_byte_jump.obj"	"$(INTDIR)\sp_byte_jump.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_byte_jump.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_byte_jump.obj"	"$(INTDIR)\sp_byte_jump.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_byte_jump.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_byte_jump.obj"	"$(INTDIR)\sp_byte_jump.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_byte_jump.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_byte_jump.obj"	"$(INTDIR)\sp_byte_jump.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_byte_jump.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_clientserver.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_clientserver.obj"	"$(INTDIR)\sp_clientserver.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_clientserver.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_clientserver.obj"	"$(INTDIR)\sp_clientserver.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_clientserver.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_clientserver.obj"	"$(INTDIR)\sp_clientserver.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_clientserver.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_clientserver.obj"	"$(INTDIR)\sp_clientserver.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_clientserver.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_clientserver.obj"	"$(INTDIR)\sp_clientserver.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_clientserver.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_clientserver.obj"	"$(INTDIR)\sp_clientserver.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_clientserver.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_clientserver.obj"	"$(INTDIR)\sp_clientserver.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_clientserver.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_clientserver.obj"	"$(INTDIR)\sp_clientserver.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_clientserver.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_dsize_check.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_dsize_check.obj"	"$(INTDIR)\sp_dsize_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_dsize_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_dsize_check.obj"	"$(INTDIR)\sp_dsize_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_dsize_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_dsize_check.obj"	"$(INTDIR)\sp_dsize_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_dsize_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_dsize_check.obj"	"$(INTDIR)\sp_dsize_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_dsize_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_dsize_check.obj"	"$(INTDIR)\sp_dsize_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_dsize_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_dsize_check.obj"	"$(INTDIR)\sp_dsize_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_dsize_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_dsize_check.obj"	"$(INTDIR)\sp_dsize_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_dsize_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_dsize_check.obj"	"$(INTDIR)\sp_dsize_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_dsize_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_icmp_code_check.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_icmp_code_check.obj"	"$(INTDIR)\sp_icmp_code_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_icmp_code_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_icmp_code_check.obj"	"$(INTDIR)\sp_icmp_code_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_icmp_code_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_icmp_code_check.obj"	"$(INTDIR)\sp_icmp_code_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_icmp_code_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_icmp_code_check.obj"	"$(INTDIR)\sp_icmp_code_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_icmp_code_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_icmp_code_check.obj"	"$(INTDIR)\sp_icmp_code_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_icmp_code_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_icmp_code_check.obj"	"$(INTDIR)\sp_icmp_code_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_icmp_code_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_icmp_code_check.obj"	"$(INTDIR)\sp_icmp_code_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_icmp_code_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_icmp_code_check.obj"	"$(INTDIR)\sp_icmp_code_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_icmp_code_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_icmp_id_check.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_icmp_id_check.obj"	"$(INTDIR)\sp_icmp_id_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_icmp_id_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_icmp_id_check.obj"	"$(INTDIR)\sp_icmp_id_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_icmp_id_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_icmp_id_check.obj"	"$(INTDIR)\sp_icmp_id_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_icmp_id_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_icmp_id_check.obj"	"$(INTDIR)\sp_icmp_id_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_icmp_id_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_icmp_id_check.obj"	"$(INTDIR)\sp_icmp_id_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_icmp_id_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_icmp_id_check.obj"	"$(INTDIR)\sp_icmp_id_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_icmp_id_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_icmp_id_check.obj"	"$(INTDIR)\sp_icmp_id_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_icmp_id_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_icmp_id_check.obj"	"$(INTDIR)\sp_icmp_id_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_icmp_id_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_icmp_seq_check.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_icmp_seq_check.obj"	"$(INTDIR)\sp_icmp_seq_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_icmp_seq_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_icmp_seq_check.obj"	"$(INTDIR)\sp_icmp_seq_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_icmp_seq_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_icmp_seq_check.obj"	"$(INTDIR)\sp_icmp_seq_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_icmp_seq_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_icmp_seq_check.obj"	"$(INTDIR)\sp_icmp_seq_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_icmp_seq_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_icmp_seq_check.obj"	"$(INTDIR)\sp_icmp_seq_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_icmp_seq_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_icmp_seq_check.obj"	"$(INTDIR)\sp_icmp_seq_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_icmp_seq_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_icmp_seq_check.obj"	"$(INTDIR)\sp_icmp_seq_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_icmp_seq_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_icmp_seq_check.obj"	"$(INTDIR)\sp_icmp_seq_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_icmp_seq_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_icmp_type_check.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_icmp_type_check.obj"	"$(INTDIR)\sp_icmp_type_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_icmp_type_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_icmp_type_check.obj"	"$(INTDIR)\sp_icmp_type_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_icmp_type_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_icmp_type_check.obj"	"$(INTDIR)\sp_icmp_type_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_icmp_type_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_icmp_type_check.obj"	"$(INTDIR)\sp_icmp_type_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_icmp_type_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_icmp_type_check.obj"	"$(INTDIR)\sp_icmp_type_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_icmp_type_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_icmp_type_check.obj"	"$(INTDIR)\sp_icmp_type_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_icmp_type_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_icmp_type_check.obj"	"$(INTDIR)\sp_icmp_type_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_icmp_type_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_icmp_type_check.obj"	"$(INTDIR)\sp_icmp_type_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_icmp_type_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_ip_fragbits.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_ip_fragbits.obj"	"$(INTDIR)\sp_ip_fragbits.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_ip_fragbits.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_ip_fragbits.obj"	"$(INTDIR)\sp_ip_fragbits.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_ip_fragbits.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_ip_fragbits.obj"	"$(INTDIR)\sp_ip_fragbits.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_ip_fragbits.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_ip_fragbits.obj"	"$(INTDIR)\sp_ip_fragbits.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_ip_fragbits.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_ip_fragbits.obj"	"$(INTDIR)\sp_ip_fragbits.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_ip_fragbits.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_ip_fragbits.obj"	"$(INTDIR)\sp_ip_fragbits.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_ip_fragbits.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_ip_fragbits.obj"	"$(INTDIR)\sp_ip_fragbits.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_ip_fragbits.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_ip_fragbits.obj"	"$(INTDIR)\sp_ip_fragbits.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_ip_fragbits.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_ip_id_check.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_ip_id_check.obj"	"$(INTDIR)\sp_ip_id_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_ip_id_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_ip_id_check.obj"	"$(INTDIR)\sp_ip_id_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_ip_id_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_ip_id_check.obj"	"$(INTDIR)\sp_ip_id_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_ip_id_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_ip_id_check.obj"	"$(INTDIR)\sp_ip_id_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_ip_id_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_ip_id_check.obj"	"$(INTDIR)\sp_ip_id_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_ip_id_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_ip_id_check.obj"	"$(INTDIR)\sp_ip_id_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_ip_id_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_ip_id_check.obj"	"$(INTDIR)\sp_ip_id_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_ip_id_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_ip_id_check.obj"	"$(INTDIR)\sp_ip_id_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_ip_id_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_ip_proto.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_ip_proto.obj"	"$(INTDIR)\sp_ip_proto.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_ip_proto.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_ip_proto.obj"	"$(INTDIR)\sp_ip_proto.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_ip_proto.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_ip_proto.obj"	"$(INTDIR)\sp_ip_proto.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_ip_proto.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_ip_proto.obj"	"$(INTDIR)\sp_ip_proto.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_ip_proto.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_ip_proto.obj"	"$(INTDIR)\sp_ip_proto.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_ip_proto.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_ip_proto.obj"	"$(INTDIR)\sp_ip_proto.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_ip_proto.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_ip_proto.obj"	"$(INTDIR)\sp_ip_proto.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_ip_proto.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_ip_proto.obj"	"$(INTDIR)\sp_ip_proto.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_ip_proto.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_ip_same_check.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_ip_same_check.obj"	"$(INTDIR)\sp_ip_same_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_ip_same_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_ip_same_check.obj"	"$(INTDIR)\sp_ip_same_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_ip_same_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_ip_same_check.obj"	"$(INTDIR)\sp_ip_same_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_ip_same_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_ip_same_check.obj"	"$(INTDIR)\sp_ip_same_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_ip_same_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_ip_same_check.obj"	"$(INTDIR)\sp_ip_same_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_ip_same_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_ip_same_check.obj"	"$(INTDIR)\sp_ip_same_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_ip_same_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_ip_same_check.obj"	"$(INTDIR)\sp_ip_same_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_ip_same_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_ip_same_check.obj"	"$(INTDIR)\sp_ip_same_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_ip_same_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_ip_tos_check.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_ip_tos_check.obj"	"$(INTDIR)\sp_ip_tos_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_ip_tos_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_ip_tos_check.obj"	"$(INTDIR)\sp_ip_tos_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_ip_tos_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_ip_tos_check.obj"	"$(INTDIR)\sp_ip_tos_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_ip_tos_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_ip_tos_check.obj"	"$(INTDIR)\sp_ip_tos_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_ip_tos_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_ip_tos_check.obj"	"$(INTDIR)\sp_ip_tos_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_ip_tos_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_ip_tos_check.obj"	"$(INTDIR)\sp_ip_tos_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_ip_tos_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_ip_tos_check.obj"	"$(INTDIR)\sp_ip_tos_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_ip_tos_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_ip_tos_check.obj"	"$(INTDIR)\sp_ip_tos_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_ip_tos_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_ipoption_check.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_ipoption_check.obj"	"$(INTDIR)\sp_ipoption_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_ipoption_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_ipoption_check.obj"	"$(INTDIR)\sp_ipoption_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_ipoption_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_ipoption_check.obj"	"$(INTDIR)\sp_ipoption_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_ipoption_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_ipoption_check.obj"	"$(INTDIR)\sp_ipoption_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_ipoption_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_ipoption_check.obj"	"$(INTDIR)\sp_ipoption_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_ipoption_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_ipoption_check.obj"	"$(INTDIR)\sp_ipoption_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_ipoption_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_ipoption_check.obj"	"$(INTDIR)\sp_ipoption_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_ipoption_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_ipoption_check.obj"	"$(INTDIR)\sp_ipoption_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_ipoption_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_pattern_match.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_pattern_match.obj"	"$(INTDIR)\sp_pattern_match.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_pattern_match.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_pattern_match.obj"	"$(INTDIR)\sp_pattern_match.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_pattern_match.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_pattern_match.obj"	"$(INTDIR)\sp_pattern_match.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_pattern_match.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_pattern_match.obj"	"$(INTDIR)\sp_pattern_match.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_pattern_match.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_pattern_match.obj"	"$(INTDIR)\sp_pattern_match.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_pattern_match.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_pattern_match.obj"	"$(INTDIR)\sp_pattern_match.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_pattern_match.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_pattern_match.obj"	"$(INTDIR)\sp_pattern_match.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_pattern_match.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_pattern_match.obj"	"$(INTDIR)\sp_pattern_match.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_pattern_match.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_react.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_react.obj"	"$(INTDIR)\sp_react.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_react.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_react.obj"	"$(INTDIR)\sp_react.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_react.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_react.obj"	"$(INTDIR)\sp_react.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_react.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_react.obj"	"$(INTDIR)\sp_react.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_react.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_react.obj"	"$(INTDIR)\sp_react.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_react.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_react.obj"	"$(INTDIR)\sp_react.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_react.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_react.obj"	"$(INTDIR)\sp_react.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_react.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_react.obj"	"$(INTDIR)\sp_react.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_react.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_respond.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_respond.obj"	"$(INTDIR)\sp_respond.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_respond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_respond.obj"	"$(INTDIR)\sp_respond.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_respond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_respond.obj"	"$(INTDIR)\sp_respond.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_respond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_respond.obj"	"$(INTDIR)\sp_respond.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_respond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_respond.obj"	"$(INTDIR)\sp_respond.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_respond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_respond.obj"	"$(INTDIR)\sp_respond.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_respond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_respond.obj"	"$(INTDIR)\sp_respond.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_respond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_respond.obj"	"$(INTDIR)\sp_respond.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_respond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_rpc_check.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_rpc_check.obj"	"$(INTDIR)\sp_rpc_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_rpc_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_rpc_check.obj"	"$(INTDIR)\sp_rpc_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_rpc_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_rpc_check.obj"	"$(INTDIR)\sp_rpc_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_rpc_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_rpc_check.obj"	"$(INTDIR)\sp_rpc_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_rpc_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_rpc_check.obj"	"$(INTDIR)\sp_rpc_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_rpc_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_rpc_check.obj"	"$(INTDIR)\sp_rpc_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_rpc_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_rpc_check.obj"	"$(INTDIR)\sp_rpc_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_rpc_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_rpc_check.obj"	"$(INTDIR)\sp_rpc_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_rpc_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_session.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_session.obj"	"$(INTDIR)\sp_session.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_session.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_session.obj"	"$(INTDIR)\sp_session.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_session.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_session.obj"	"$(INTDIR)\sp_session.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_session.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_session.obj"	"$(INTDIR)\sp_session.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_session.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_session.obj"	"$(INTDIR)\sp_session.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_session.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_session.obj"	"$(INTDIR)\sp_session.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_session.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_session.obj"	"$(INTDIR)\sp_session.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_session.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_session.obj"	"$(INTDIR)\sp_session.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_session.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_tcp_ack_check.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_tcp_ack_check.obj"	"$(INTDIR)\sp_tcp_ack_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_tcp_ack_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_tcp_ack_check.obj"	"$(INTDIR)\sp_tcp_ack_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_tcp_ack_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_tcp_ack_check.obj"	"$(INTDIR)\sp_tcp_ack_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_tcp_ack_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_tcp_ack_check.obj"	"$(INTDIR)\sp_tcp_ack_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_tcp_ack_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_tcp_ack_check.obj"	"$(INTDIR)\sp_tcp_ack_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_tcp_ack_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_tcp_ack_check.obj"	"$(INTDIR)\sp_tcp_ack_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_tcp_ack_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_tcp_ack_check.obj"	"$(INTDIR)\sp_tcp_ack_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_tcp_ack_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_tcp_ack_check.obj"	"$(INTDIR)\sp_tcp_ack_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_tcp_ack_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_tcp_flag_check.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_tcp_flag_check.obj"	"$(INTDIR)\sp_tcp_flag_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_tcp_flag_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_tcp_flag_check.obj"	"$(INTDIR)\sp_tcp_flag_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_tcp_flag_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_tcp_flag_check.obj"	"$(INTDIR)\sp_tcp_flag_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_tcp_flag_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_tcp_flag_check.obj"	"$(INTDIR)\sp_tcp_flag_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_tcp_flag_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_tcp_flag_check.obj"	"$(INTDIR)\sp_tcp_flag_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_tcp_flag_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_tcp_flag_check.obj"	"$(INTDIR)\sp_tcp_flag_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_tcp_flag_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_tcp_flag_check.obj"	"$(INTDIR)\sp_tcp_flag_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_tcp_flag_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_tcp_flag_check.obj"	"$(INTDIR)\sp_tcp_flag_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_tcp_flag_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_tcp_seq_check.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_tcp_seq_check.obj"	"$(INTDIR)\sp_tcp_seq_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_tcp_seq_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_tcp_seq_check.obj"	"$(INTDIR)\sp_tcp_seq_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_tcp_seq_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_tcp_seq_check.obj"	"$(INTDIR)\sp_tcp_seq_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_tcp_seq_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_tcp_seq_check.obj"	"$(INTDIR)\sp_tcp_seq_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_tcp_seq_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_tcp_seq_check.obj"	"$(INTDIR)\sp_tcp_seq_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_tcp_seq_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_tcp_seq_check.obj"	"$(INTDIR)\sp_tcp_seq_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_tcp_seq_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_tcp_seq_check.obj"	"$(INTDIR)\sp_tcp_seq_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_tcp_seq_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_tcp_seq_check.obj"	"$(INTDIR)\sp_tcp_seq_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_tcp_seq_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_tcp_win_check.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_tcp_win_check.obj"	"$(INTDIR)\sp_tcp_win_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_tcp_win_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_tcp_win_check.obj"	"$(INTDIR)\sp_tcp_win_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_tcp_win_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_tcp_win_check.obj"	"$(INTDIR)\sp_tcp_win_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_tcp_win_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_tcp_win_check.obj"	"$(INTDIR)\sp_tcp_win_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_tcp_win_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_tcp_win_check.obj"	"$(INTDIR)\sp_tcp_win_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_tcp_win_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_tcp_win_check.obj"	"$(INTDIR)\sp_tcp_win_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_tcp_win_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_tcp_win_check.obj"	"$(INTDIR)\sp_tcp_win_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_tcp_win_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_tcp_win_check.obj"	"$(INTDIR)\sp_tcp_win_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_tcp_win_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\detection-plugins\sp_ttl_check.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sp_ttl_check.obj"	"$(INTDIR)\sp_ttl_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sp_ttl_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sp_ttl_check.obj"	"$(INTDIR)\sp_ttl_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sp_ttl_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sp_ttl_check.obj"	"$(INTDIR)\sp_ttl_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sp_ttl_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sp_ttl_check.obj"	"$(INTDIR)\sp_ttl_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sp_ttl_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sp_ttl_check.obj"	"$(INTDIR)\sp_ttl_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sp_ttl_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sp_ttl_check.obj"	"$(INTDIR)\sp_ttl_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sp_ttl_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sp_ttl_check.obj"	"$(INTDIR)\sp_ttl_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sp_ttl_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sp_ttl_check.obj"	"$(INTDIR)\sp_ttl_check.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sp_ttl_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\output-plugins\spo_alert_fast.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spo_alert_fast.obj"	"$(INTDIR)\spo_alert_fast.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spo_alert_fast.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spo_alert_fast.obj"	"$(INTDIR)\spo_alert_fast.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spo_alert_fast.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spo_alert_fast.obj"	"$(INTDIR)\spo_alert_fast.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spo_alert_fast.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spo_alert_fast.obj"	"$(INTDIR)\spo_alert_fast.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spo_alert_fast.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spo_alert_fast.obj"	"$(INTDIR)\spo_alert_fast.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spo_alert_fast.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spo_alert_fast.obj"	"$(INTDIR)\spo_alert_fast.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spo_alert_fast.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spo_alert_fast.obj"	"$(INTDIR)\spo_alert_fast.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spo_alert_fast.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spo_alert_fast.obj"	"$(INTDIR)\spo_alert_fast.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spo_alert_fast.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\output-plugins\spo_alert_full.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spo_alert_full.obj"	"$(INTDIR)\spo_alert_full.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spo_alert_full.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spo_alert_full.obj"	"$(INTDIR)\spo_alert_full.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spo_alert_full.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spo_alert_full.obj"	"$(INTDIR)\spo_alert_full.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spo_alert_full.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spo_alert_full.obj"	"$(INTDIR)\spo_alert_full.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spo_alert_full.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spo_alert_full.obj"	"$(INTDIR)\spo_alert_full.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spo_alert_full.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spo_alert_full.obj"	"$(INTDIR)\spo_alert_full.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spo_alert_full.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spo_alert_full.obj"	"$(INTDIR)\spo_alert_full.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spo_alert_full.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spo_alert_full.obj"	"$(INTDIR)\spo_alert_full.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spo_alert_full.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\output-plugins\spo_alert_sf_socket.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spo_alert_sf_socket.obj"	"$(INTDIR)\spo_alert_sf_socket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spo_alert_sf_socket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spo_alert_sf_socket.obj"	"$(INTDIR)\spo_alert_sf_socket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spo_alert_sf_socket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spo_alert_sf_socket.obj"	"$(INTDIR)\spo_alert_sf_socket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spo_alert_sf_socket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spo_alert_sf_socket.obj"	"$(INTDIR)\spo_alert_sf_socket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spo_alert_sf_socket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spo_alert_sf_socket.obj"	"$(INTDIR)\spo_alert_sf_socket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spo_alert_sf_socket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spo_alert_sf_socket.obj"	"$(INTDIR)\spo_alert_sf_socket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spo_alert_sf_socket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spo_alert_sf_socket.obj"	"$(INTDIR)\spo_alert_sf_socket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spo_alert_sf_socket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spo_alert_sf_socket.obj"	"$(INTDIR)\spo_alert_sf_socket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spo_alert_sf_socket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\output-plugins\spo_alert_smb.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spo_alert_smb.obj"	"$(INTDIR)\spo_alert_smb.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spo_alert_smb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spo_alert_smb.obj"	"$(INTDIR)\spo_alert_smb.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spo_alert_smb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spo_alert_smb.obj"	"$(INTDIR)\spo_alert_smb.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spo_alert_smb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spo_alert_smb.obj"	"$(INTDIR)\spo_alert_smb.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spo_alert_smb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spo_alert_smb.obj"	"$(INTDIR)\spo_alert_smb.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spo_alert_smb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spo_alert_smb.obj"	"$(INTDIR)\spo_alert_smb.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spo_alert_smb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spo_alert_smb.obj"	"$(INTDIR)\spo_alert_smb.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spo_alert_smb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spo_alert_smb.obj"	"$(INTDIR)\spo_alert_smb.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spo_alert_smb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\output-plugins\spo_alert_syslog.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spo_alert_syslog.obj"	"$(INTDIR)\spo_alert_syslog.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spo_alert_syslog.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spo_alert_syslog.obj"	"$(INTDIR)\spo_alert_syslog.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spo_alert_syslog.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spo_alert_syslog.obj"	"$(INTDIR)\spo_alert_syslog.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spo_alert_syslog.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spo_alert_syslog.obj"	"$(INTDIR)\spo_alert_syslog.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spo_alert_syslog.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spo_alert_syslog.obj"	"$(INTDIR)\spo_alert_syslog.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spo_alert_syslog.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spo_alert_syslog.obj"	"$(INTDIR)\spo_alert_syslog.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spo_alert_syslog.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spo_alert_syslog.obj"	"$(INTDIR)\spo_alert_syslog.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spo_alert_syslog.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spo_alert_syslog.obj"	"$(INTDIR)\spo_alert_syslog.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spo_alert_syslog.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\output-plugins\spo_alert_unixsock.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spo_alert_unixsock.obj"	"$(INTDIR)\spo_alert_unixsock.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spo_alert_unixsock.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spo_alert_unixsock.obj"	"$(INTDIR)\spo_alert_unixsock.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spo_alert_unixsock.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spo_alert_unixsock.obj"	"$(INTDIR)\spo_alert_unixsock.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spo_alert_unixsock.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spo_alert_unixsock.obj"	"$(INTDIR)\spo_alert_unixsock.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spo_alert_unixsock.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spo_alert_unixsock.obj"	"$(INTDIR)\spo_alert_unixsock.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spo_alert_unixsock.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spo_alert_unixsock.obj"	"$(INTDIR)\spo_alert_unixsock.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spo_alert_unixsock.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spo_alert_unixsock.obj"	"$(INTDIR)\spo_alert_unixsock.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spo_alert_unixsock.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spo_alert_unixsock.obj"	"$(INTDIR)\spo_alert_unixsock.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spo_alert_unixsock.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\output-plugins\spo_csv.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spo_csv.obj"	"$(INTDIR)\spo_csv.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spo_csv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spo_csv.obj"	"$(INTDIR)\spo_csv.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spo_csv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spo_csv.obj"	"$(INTDIR)\spo_csv.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spo_csv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spo_csv.obj"	"$(INTDIR)\spo_csv.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spo_csv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spo_csv.obj"	"$(INTDIR)\spo_csv.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spo_csv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spo_csv.obj"	"$(INTDIR)\spo_csv.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spo_csv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spo_csv.obj"	"$(INTDIR)\spo_csv.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spo_csv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spo_csv.obj"	"$(INTDIR)\spo_csv.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spo_csv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\output-plugins\spo_database.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spo_database.obj"	"$(INTDIR)\spo_database.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spo_database.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spo_database.obj"	"$(INTDIR)\spo_database.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spo_database.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spo_database.obj"	"$(INTDIR)\spo_database.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spo_database.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spo_database.obj"	"$(INTDIR)\spo_database.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spo_database.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spo_database.obj"	"$(INTDIR)\spo_database.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spo_database.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spo_database.obj"	"$(INTDIR)\spo_database.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spo_database.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spo_database.obj"	"$(INTDIR)\spo_database.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spo_database.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spo_database.obj"	"$(INTDIR)\spo_database.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spo_database.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\output-plugins\spo_log_ascii.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spo_log_ascii.obj"	"$(INTDIR)\spo_log_ascii.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spo_log_ascii.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spo_log_ascii.obj"	"$(INTDIR)\spo_log_ascii.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spo_log_ascii.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spo_log_ascii.obj"	"$(INTDIR)\spo_log_ascii.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spo_log_ascii.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spo_log_ascii.obj"	"$(INTDIR)\spo_log_ascii.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spo_log_ascii.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spo_log_ascii.obj"	"$(INTDIR)\spo_log_ascii.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spo_log_ascii.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spo_log_ascii.obj"	"$(INTDIR)\spo_log_ascii.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spo_log_ascii.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spo_log_ascii.obj"	"$(INTDIR)\spo_log_ascii.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spo_log_ascii.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spo_log_ascii.obj"	"$(INTDIR)\spo_log_ascii.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spo_log_ascii.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\output-plugins\spo_log_null.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spo_log_null.obj"	"$(INTDIR)\spo_log_null.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spo_log_null.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spo_log_null.obj"	"$(INTDIR)\spo_log_null.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spo_log_null.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spo_log_null.obj"	"$(INTDIR)\spo_log_null.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spo_log_null.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spo_log_null.obj"	"$(INTDIR)\spo_log_null.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spo_log_null.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spo_log_null.obj"	"$(INTDIR)\spo_log_null.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spo_log_null.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spo_log_null.obj"	"$(INTDIR)\spo_log_null.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spo_log_null.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spo_log_null.obj"	"$(INTDIR)\spo_log_null.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spo_log_null.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spo_log_null.obj"	"$(INTDIR)\spo_log_null.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spo_log_null.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\output-plugins\spo_log_tcpdump.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spo_log_tcpdump.obj"	"$(INTDIR)\spo_log_tcpdump.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spo_log_tcpdump.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spo_log_tcpdump.obj"	"$(INTDIR)\spo_log_tcpdump.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spo_log_tcpdump.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spo_log_tcpdump.obj"	"$(INTDIR)\spo_log_tcpdump.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spo_log_tcpdump.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spo_log_tcpdump.obj"	"$(INTDIR)\spo_log_tcpdump.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spo_log_tcpdump.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spo_log_tcpdump.obj"	"$(INTDIR)\spo_log_tcpdump.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spo_log_tcpdump.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spo_log_tcpdump.obj"	"$(INTDIR)\spo_log_tcpdump.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spo_log_tcpdump.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spo_log_tcpdump.obj"	"$(INTDIR)\spo_log_tcpdump.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spo_log_tcpdump.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spo_log_tcpdump.obj"	"$(INTDIR)\spo_log_tcpdump.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spo_log_tcpdump.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\output-plugins\spo_unified.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spo_unified.obj"	"$(INTDIR)\spo_unified.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spo_unified.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spo_unified.obj"	"$(INTDIR)\spo_unified.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spo_unified.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spo_unified.obj"	"$(INTDIR)\spo_unified.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spo_unified.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spo_unified.obj"	"$(INTDIR)\spo_unified.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spo_unified.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spo_unified.obj"	"$(INTDIR)\spo_unified.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spo_unified.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spo_unified.obj"	"$(INTDIR)\spo_unified.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spo_unified.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spo_unified.obj"	"$(INTDIR)\spo_unified.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spo_unified.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spo_unified.obj"	"$(INTDIR)\spo_unified.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spo_unified.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\preprocessors\http-resp.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\http-resp.obj"	"$(INTDIR)\http-resp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\http-resp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\http-resp.obj"	"$(INTDIR)\http-resp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\http-resp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\http-resp.obj"	"$(INTDIR)\http-resp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\http-resp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\http-resp.obj"	"$(INTDIR)\http-resp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\http-resp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\http-resp.obj"	"$(INTDIR)\http-resp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\http-resp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\http-resp.obj"	"$(INTDIR)\http-resp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\http-resp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\http-resp.obj"	"$(INTDIR)\http-resp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\http-resp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\http-resp.obj"	"$(INTDIR)\http-resp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\http-resp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\preprocessors\perf-base.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\perf-base.obj"	"$(INTDIR)\perf-base.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\perf-base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\perf-base.obj"	"$(INTDIR)\perf-base.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\perf-base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\perf-base.obj"	"$(INTDIR)\perf-base.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\perf-base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\perf-base.obj"	"$(INTDIR)\perf-base.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\perf-base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\perf-base.obj"	"$(INTDIR)\perf-base.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\perf-base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\perf-base.obj"	"$(INTDIR)\perf-base.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\perf-base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\perf-base.obj"	"$(INTDIR)\perf-base.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\perf-base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\perf-base.obj"	"$(INTDIR)\perf-base.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\perf-base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\preprocessors\perf-event.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\perf-event.obj"	"$(INTDIR)\perf-event.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\perf-event.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\perf-event.obj"	"$(INTDIR)\perf-event.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\perf-event.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\perf-event.obj"	"$(INTDIR)\perf-event.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\perf-event.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\perf-event.obj"	"$(INTDIR)\perf-event.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\perf-event.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\perf-event.obj"	"$(INTDIR)\perf-event.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\perf-event.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\perf-event.obj"	"$(INTDIR)\perf-event.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\perf-event.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\perf-event.obj"	"$(INTDIR)\perf-event.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\perf-event.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\perf-event.obj"	"$(INTDIR)\perf-event.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\perf-event.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\..\preprocessors\perf-flow.c"

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\perf-flow.obj"	"$(INTDIR)\perf-flow.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\perf-flow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\perf-flow.obj"	"$(INTDIR)\perf-flow.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\perf-flow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\perf-flow.obj"	"$(INTDIR)\perf-flow.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\perf-flow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\perf-flow.obj"	"$(INTDIR)\perf-flow.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\perf-flow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\perf-flow.obj"	"$(INTDIR)\perf-flow.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\perf-flow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\perf-flow.obj"	"$(INTDIR)\perf-flow.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\perf-flow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\perf-flow.obj"	"$(INTDIR)\perf-flow.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\perf-flow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\perf-flow.obj"	"$(INTDIR)\perf-flow.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\perf-flow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\preprocessors\perf.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\perf.obj"	"$(INTDIR)\perf.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\perf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\perf.obj"	"$(INTDIR)\perf.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\perf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\perf.obj"	"$(INTDIR)\perf.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\perf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\perf.obj"	"$(INTDIR)\perf.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\perf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\perf.obj"	"$(INTDIR)\perf.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\perf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\perf.obj"	"$(INTDIR)\perf.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\perf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\perf.obj"	"$(INTDIR)\perf.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\perf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\perf.obj"	"$(INTDIR)\perf.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\perf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\preprocessors\sfprocpidstats.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\sfprocpidstats.obj"	"$(INTDIR)\sfprocpidstats.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\sfprocpidstats.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\sfprocpidstats.obj"	"$(INTDIR)\sfprocpidstats.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\sfprocpidstats.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\sfprocpidstats.obj"	"$(INTDIR)\sfprocpidstats.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\sfprocpidstats.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\sfprocpidstats.obj"	"$(INTDIR)\sfprocpidstats.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\sfprocpidstats.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\sfprocpidstats.obj"	"$(INTDIR)\sfprocpidstats.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\sfprocpidstats.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\sfprocpidstats.obj"	"$(INTDIR)\sfprocpidstats.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\sfprocpidstats.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\sfprocpidstats.obj"	"$(INTDIR)\sfprocpidstats.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\sfprocpidstats.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\sfprocpidstats.obj"	"$(INTDIR)\sfprocpidstats.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\sfprocpidstats.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\preprocessors\spp_arpspoof.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spp_arpspoof.obj"	"$(INTDIR)\spp_arpspoof.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spp_arpspoof.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spp_arpspoof.obj"	"$(INTDIR)\spp_arpspoof.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spp_arpspoof.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spp_arpspoof.obj"	"$(INTDIR)\spp_arpspoof.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spp_arpspoof.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spp_arpspoof.obj"	"$(INTDIR)\spp_arpspoof.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spp_arpspoof.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spp_arpspoof.obj"	"$(INTDIR)\spp_arpspoof.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spp_arpspoof.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spp_arpspoof.obj"	"$(INTDIR)\spp_arpspoof.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spp_arpspoof.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spp_arpspoof.obj"	"$(INTDIR)\spp_arpspoof.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spp_arpspoof.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spp_arpspoof.obj"	"$(INTDIR)\spp_arpspoof.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spp_arpspoof.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\preprocessors\spp_bo.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spp_bo.obj"	"$(INTDIR)\spp_bo.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spp_bo.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spp_bo.obj"	"$(INTDIR)\spp_bo.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spp_bo.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spp_bo.obj"	"$(INTDIR)\spp_bo.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spp_bo.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spp_bo.obj"	"$(INTDIR)\spp_bo.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spp_bo.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spp_bo.obj"	"$(INTDIR)\spp_bo.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spp_bo.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spp_bo.obj"	"$(INTDIR)\spp_bo.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spp_bo.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spp_bo.obj"	"$(INTDIR)\spp_bo.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spp_bo.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spp_bo.obj"	"$(INTDIR)\spp_bo.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spp_bo.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\preprocessors\spp_conversation.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spp_conversation.obj"	"$(INTDIR)\spp_conversation.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spp_conversation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spp_conversation.obj"	"$(INTDIR)\spp_conversation.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spp_conversation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spp_conversation.obj"	"$(INTDIR)\spp_conversation.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spp_conversation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spp_conversation.obj"	"$(INTDIR)\spp_conversation.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spp_conversation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spp_conversation.obj"	"$(INTDIR)\spp_conversation.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spp_conversation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spp_conversation.obj"	"$(INTDIR)\spp_conversation.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spp_conversation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spp_conversation.obj"	"$(INTDIR)\spp_conversation.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spp_conversation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spp_conversation.obj"	"$(INTDIR)\spp_conversation.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spp_conversation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\preprocessors\spp_frag2.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spp_frag2.obj"	"$(INTDIR)\spp_frag2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spp_frag2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spp_frag2.obj"	"$(INTDIR)\spp_frag2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spp_frag2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spp_frag2.obj"	"$(INTDIR)\spp_frag2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spp_frag2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spp_frag2.obj"	"$(INTDIR)\spp_frag2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spp_frag2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spp_frag2.obj"	"$(INTDIR)\spp_frag2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spp_frag2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spp_frag2.obj"	"$(INTDIR)\spp_frag2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spp_frag2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spp_frag2.obj"	"$(INTDIR)\spp_frag2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spp_frag2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spp_frag2.obj"	"$(INTDIR)\spp_frag2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spp_frag2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\preprocessors\spp_http_decode.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spp_http_decode.obj"	"$(INTDIR)\spp_http_decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spp_http_decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spp_http_decode.obj"	"$(INTDIR)\spp_http_decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spp_http_decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spp_http_decode.obj"	"$(INTDIR)\spp_http_decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spp_http_decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spp_http_decode.obj"	"$(INTDIR)\spp_http_decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spp_http_decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spp_http_decode.obj"	"$(INTDIR)\spp_http_decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spp_http_decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spp_http_decode.obj"	"$(INTDIR)\spp_http_decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spp_http_decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spp_http_decode.obj"	"$(INTDIR)\spp_http_decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spp_http_decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spp_http_decode.obj"	"$(INTDIR)\spp_http_decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spp_http_decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\preprocessors\spp_httpflow.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spp_httpflow.obj"	"$(INTDIR)\spp_httpflow.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spp_httpflow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spp_httpflow.obj"	"$(INTDIR)\spp_httpflow.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spp_httpflow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spp_httpflow.obj"	"$(INTDIR)\spp_httpflow.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spp_httpflow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spp_httpflow.obj"	"$(INTDIR)\spp_httpflow.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spp_httpflow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spp_httpflow.obj"	"$(INTDIR)\spp_httpflow.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spp_httpflow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spp_httpflow.obj"	"$(INTDIR)\spp_httpflow.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spp_httpflow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spp_httpflow.obj"	"$(INTDIR)\spp_httpflow.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spp_httpflow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spp_httpflow.obj"	"$(INTDIR)\spp_httpflow.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spp_httpflow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\preprocessors\spp_perfmonitor.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spp_perfmonitor.obj"	"$(INTDIR)\spp_perfmonitor.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spp_perfmonitor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spp_perfmonitor.obj"	"$(INTDIR)\spp_perfmonitor.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spp_perfmonitor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spp_perfmonitor.obj"	"$(INTDIR)\spp_perfmonitor.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spp_perfmonitor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spp_perfmonitor.obj"	"$(INTDIR)\spp_perfmonitor.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spp_perfmonitor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spp_perfmonitor.obj"	"$(INTDIR)\spp_perfmonitor.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spp_perfmonitor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spp_perfmonitor.obj"	"$(INTDIR)\spp_perfmonitor.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spp_perfmonitor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spp_perfmonitor.obj"	"$(INTDIR)\spp_perfmonitor.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spp_perfmonitor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spp_perfmonitor.obj"	"$(INTDIR)\spp_perfmonitor.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spp_perfmonitor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\preprocessors\spp_portscan.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spp_portscan.obj"	"$(INTDIR)\spp_portscan.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spp_portscan.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spp_portscan.obj"	"$(INTDIR)\spp_portscan.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spp_portscan.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spp_portscan.obj"	"$(INTDIR)\spp_portscan.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spp_portscan.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spp_portscan.obj"	"$(INTDIR)\spp_portscan.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spp_portscan.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spp_portscan.obj"	"$(INTDIR)\spp_portscan.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spp_portscan.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spp_portscan.obj"	"$(INTDIR)\spp_portscan.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spp_portscan.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spp_portscan.obj"	"$(INTDIR)\spp_portscan.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spp_portscan.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spp_portscan.obj"	"$(INTDIR)\spp_portscan.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spp_portscan.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\preprocessors\spp_portscan2.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spp_portscan2.obj"	"$(INTDIR)\spp_portscan2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spp_portscan2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spp_portscan2.obj"	"$(INTDIR)\spp_portscan2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spp_portscan2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spp_portscan2.obj"	"$(INTDIR)\spp_portscan2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spp_portscan2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spp_portscan2.obj"	"$(INTDIR)\spp_portscan2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spp_portscan2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spp_portscan2.obj"	"$(INTDIR)\spp_portscan2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spp_portscan2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spp_portscan2.obj"	"$(INTDIR)\spp_portscan2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spp_portscan2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spp_portscan2.obj"	"$(INTDIR)\spp_portscan2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spp_portscan2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spp_portscan2.obj"	"$(INTDIR)\spp_portscan2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spp_portscan2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\preprocessors\spp_rpc_decode.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spp_rpc_decode.obj"	"$(INTDIR)\spp_rpc_decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spp_rpc_decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spp_rpc_decode.obj"	"$(INTDIR)\spp_rpc_decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spp_rpc_decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spp_rpc_decode.obj"	"$(INTDIR)\spp_rpc_decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spp_rpc_decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spp_rpc_decode.obj"	"$(INTDIR)\spp_rpc_decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spp_rpc_decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spp_rpc_decode.obj"	"$(INTDIR)\spp_rpc_decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spp_rpc_decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spp_rpc_decode.obj"	"$(INTDIR)\spp_rpc_decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spp_rpc_decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spp_rpc_decode.obj"	"$(INTDIR)\spp_rpc_decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spp_rpc_decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spp_rpc_decode.obj"	"$(INTDIR)\spp_rpc_decode.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spp_rpc_decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\preprocessors\spp_stream4.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spp_stream4.obj"	"$(INTDIR)\spp_stream4.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spp_stream4.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spp_stream4.obj"	"$(INTDIR)\spp_stream4.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spp_stream4.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spp_stream4.obj"	"$(INTDIR)\spp_stream4.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spp_stream4.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spp_stream4.obj"	"$(INTDIR)\spp_stream4.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spp_stream4.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spp_stream4.obj"	"$(INTDIR)\spp_stream4.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spp_stream4.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spp_stream4.obj"	"$(INTDIR)\spp_stream4.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spp_stream4.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spp_stream4.obj"	"$(INTDIR)\spp_stream4.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spp_stream4.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spp_stream4.obj"	"$(INTDIR)\spp_stream4.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spp_stream4.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\preprocessors\spp_telnet_negotiation.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\spp_telnet_negotiation.obj"	"$(INTDIR)\spp_telnet_negotiation.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\spp_telnet_negotiation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\spp_telnet_negotiation.obj"	"$(INTDIR)\spp_telnet_negotiation.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\spp_telnet_negotiation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\spp_telnet_negotiation.obj"	"$(INTDIR)\spp_telnet_negotiation.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\spp_telnet_negotiation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\spp_telnet_negotiation.obj"	"$(INTDIR)\spp_telnet_negotiation.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\spp_telnet_negotiation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\spp_telnet_negotiation.obj"	"$(INTDIR)\spp_telnet_negotiation.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\spp_telnet_negotiation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\spp_telnet_negotiation.obj"	"$(INTDIR)\spp_telnet_negotiation.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\spp_telnet_negotiation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\spp_telnet_negotiation.obj"	"$(INTDIR)\spp_telnet_negotiation.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\spp_telnet_negotiation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\spp_telnet_negotiation.obj"	"$(INTDIR)\spp_telnet_negotiation.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\spp_telnet_negotiation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\parser\IpAddrSet.c

!IF  "$(CFG)" == "snort - Win32 StdDB Debug"


"$(INTDIR)\IpAddrSet.obj"	"$(INTDIR)\IpAddrSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Release"


"$(INTDIR)\IpAddrSet.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Debug"


"$(INTDIR)\IpAddrSet.obj"	"$(INTDIR)\IpAddrSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Release"


"$(INTDIR)\IpAddrSet.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Debug"


"$(INTDIR)\IpAddrSet.obj"	"$(INTDIR)\IpAddrSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB FlexResp Service Release"


"$(INTDIR)\IpAddrSet.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Debug"


"$(INTDIR)\IpAddrSet.obj"	"$(INTDIR)\IpAddrSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Release"


"$(INTDIR)\IpAddrSet.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Debug"


"$(INTDIR)\IpAddrSet.obj"	"$(INTDIR)\IpAddrSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL Service Release"


"$(INTDIR)\IpAddrSet.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Debug"


"$(INTDIR)\IpAddrSet.obj"	"$(INTDIR)\IpAddrSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Release"


"$(INTDIR)\IpAddrSet.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Debug"


"$(INTDIR)\IpAddrSet.obj"	"$(INTDIR)\IpAddrSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB MSSQL FlexResp Service Release"


"$(INTDIR)\IpAddrSet.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Debug"


"$(INTDIR)\IpAddrSet.obj"	"$(INTDIR)\IpAddrSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "snort - Win32 StdDB Service Release"


"$(INTDIR)\IpAddrSet.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

