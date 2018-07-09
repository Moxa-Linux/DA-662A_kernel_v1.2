#
#	Makefile -- Build instructions for user level apps
#

.EXPORT_ALL_VARIABLES:
.PHONY: config all romfs clean prune

#
# Include architecture specific build rules.
#

ifndef ROOTDIR
ROOTDIR=..
endif

#UCLINUX_BUILD_USER=1   /* Aeluin Chen. 03-14-2013 */
-include $(LINUX_CONFIG)
-include $(CONFIG_CONFIG)
-include $(ARCH_CONFIG)
-include $(MODULES_CONFIG)

VEND=$(ROOTDIR)/vendors

#
# must run the vendor build first
#
dir_v = $(VEND)/$(CONFIG_VENDOR)/$(CONFIG_PRODUCT)/.
#dir_p = $(ROOTDIR)/prop
dir_p = 

dir_y =
dir_n =
dir_  =

dir_$(CONFIG_JFFS_FS)                       += mtd-utils
dir_$(CONFIG_JFFS2_FS)                      += mtd-utils

dir_$(CONFIG_USER_AGETTY_AGETTY)            += agetty
dir_$(CONFIG_USER_APPWEB_APPWEB)            += appWeb
dir_$(CONFIG_USER_AT_AT)                    += at
dir_$(CONFIG_USER_AT_ATD)                   += at
dir_$(CONFIG_USER_AT_ATRUN)                 += at
dir_$(CONFIG_USER_BASH_BASH)                += bash
dir_$(CONFIG_USER_BC)                       += bc-1.06
dir_$(CONFIG_USER_BOA_SRC_BOA)              += boa
dir_$(CONFIG_USER_BOOTTOOLS_FLASHLOADER)    += boottools
dir_$(CONFIG_USER_BOOTTOOLS_HIMEMLOADER)    += boottools
dir_$(CONFIG_USER_BPALOGIN_BPALOGIN)        += bpalogin
dir_$(CONFIG_USER_BR2684CTL_BR2684CTL)      += br2684ctl
dir_$(CONFIG_USER_BRCFG_BRCFG)              += brcfg
dir_$(CONFIG_USER_BRCTL_BRCTL)              += bridge-utils
dir_$(CONFIG_USER_BUSYBOX_BUSYBOX_1_10_4)   += busybox-1.10.4
dir_$(CONFIG_USER_BUSYBOX_BUSYBOX_1_14_1)   += busybox-1.14.1
dir_$(CONFIG_USER_BUSYBOX_BUSYBOX_1_18_5)   += busybox-1.18.5
dir_$(CONFIG_USER_BUSYBOX_BUSYBOX_1_21_1)   += busybox-1.21.1
dir_$(CONFIG_USER_CAL_CAL)                  += cal
dir_$(CONFIG_USER_CAL_DATE)                 += cal
dir_$(CONFIG_USER_CAMSERV)                  += camserv
dir_$(CONFIG_USER_CAN_EXAMPLES)             += can4linux
dir_$(CONFIG_USER_CAN_HORCH)                += horch
dir_$(CONFIG_USER_CGI_GENERIC)              += cgi_generic
dir_$(CONFIG_USER_CGIHTML)                  += cgihtml/examples
#dir_$(CONFIG_USER_CHAT_CHAT)                += pppd/chat
dir_$(CONFIG_USER_CKSUM_CKSUM)              += cksum
dir_$(CONFIG_USER_CLOCK_CLOCK)              += clock
dir_$(CONFIG_USER_CPU_CPU)                  += cpu
dir_$(CONFIG_USER_CRDA)                     += crda
dir_$(CONFIG_USER_CRAMFS_MKCRAMFS)          += cramfs
dir_$(CONFIG_USER_CRAMFS_CRAMFSCK)          += cramfs
dir_$(CONFIG_USER_CRON_CRON)                += cron
dir_$(CONFIG_USER_SCREEN)                   += screen-4.0.3
dir_$(CONFIG_USER_CRYPT)		    += libgcrypt-1.5.0
dir_$(CONFIG_USER_CRYPTSETUP)		    += cryptsetup-1.5.0
#dir_$(CONFIG_USER_CURL)	                    += curl-7.19.4
dir_$(CONFIG_USER_CURL)	                    += curl-7.52
dir_$(CONFIG_USER_CXXTEST_CXXTEST)          += cxxtest
dir_$(CONFIG_USER_DE2TSCAL_DE2TSCAL)        += de2ts-cal
dir_$(CONFIG_USER_DEMO_BUTTON)              += demo
dir_$(CONFIG_USER_DEMO_MORSE)               += demo
dir_$(CONFIG_USER_DOSUTILS)                 += dosfstools
dir_$(CONFIG_USER_DHRYSTONE_DHRYSTONE)      += dhrystone
dir_$(CONFIG_USER_DHCP_ISC_SERVER_DHCPD)    += dhcp-isc
dir_$(CONFIG_USER_DHCP_ISC_CLIENT_DHCLIENT) += dhcp-isc
dir_$(CONFIG_USER_DHCP_ISC_RELAY_DHCRELAY)  += dhcp-isc
dir_$(CONFIG_USER_DHCP_ISC_3_1_1_SERVER_DHCPD)    += dhcp-isc_3.1.1
dir_$(CONFIG_USER_DHCP_ISC_3_1_1_CLIENT_DHCLIENT) += dhcp-isc_3.1.1
dir_$(CONFIG_USER_DHCP_ISC_3_1_1_RELAY_DHCRELAY)  += dhcp-isc_3.1.1
dir_$(CONFIG_USER_DHCPCD_DHCPCD)            += dhcpcd
dir_$(CONFIG_USER_DHCPCD_NEW_DHCPCD)        += dhcpcd-new
dir_$(CONFIG_USER_DHCPD_DHCPD)              += dhcpd
dir_$(CONFIG_USER_DIALD_DIALD)              += diald
dir_$(CONFIG_USER_DISCARD_DISCARD)          += discard
dir_$(CONFIG_USER_DISCARD_ECHO)             += discard
dir_$(CONFIG_USER_DNSMASQ_DNSMASQ)          += dnsmasq
dir_$(CONFIG_USER_E2FSPROGS_E2FSCK_E2FSCK)  += e2fsprogs
dir_$(CONFIG_USER_E2FSPROGS_E2FSCK_1_41_3_E2FSCK)  += e2fsprogs-1.41.3
dir_$(CONFIG_USER_E2FSPROGS_MISC_BADBLOCKS) += e2fsprogs
dir_$(CONFIG_USER_E2FSPROGS_MISC_1_41_3_BADBLOCKS) += e2fsprogs-1.41.3
dir_$(CONFIG_USER_E2FSPROGS_MISC_CHATTR)    += e2fsprogs
dir_$(CONFIG_USER_E2FSPROGS_MISC_1_41_3_CHATTR)    += e2fsprogs-1.41.3
dir_$(CONFIG_USER_E2FSPROGS_MISC_DUMPE2FS)  += e2fsprogs
dir_$(CONFIG_USER_E2FSPROGS_MISC_1_41_3_DUMPE2FS)  += e2fsprogs-1.41.3
dir_$(CONFIG_USER_E2FSPROGS_MISC_E2LABEL)   += e2fsprogs
dir_$(CONFIG_USER_E2FSPROGS_MISC_1_41_3_E2LABEL)   += e2fsprogs-1.41.3
dir_$(CONFIG_USER_E2FSPROGS_MISC_FSCK)      += e2fsprogs
dir_$(CONFIG_USER_E2FSPROGS_MISC_1_41_3_FSCK)      += e2fsprogs-1.41.3
dir_$(CONFIG_USER_E2FSPROGS_MISC_LSATTR)    += e2fsprogs
dir_$(CONFIG_USER_E2FSPROGS_MISC_1_41_3_LSATTR)    += e2fsprogs-1.41.3
dir_$(CONFIG_USER_E2FSPROGS_MISC_MKE2FS)    += e2fsprogs
dir_$(CONFIG_USER_E2FSPROGS_MISC_1_41_3_MKE2FS)    += e2fsprogs-1.41.3
dir_$(CONFIG_USER_E2FSPROGS_MISC_MKLOST_FOUND) += e2fsprogs
dir_$(CONFIG_USER_E2FSPROGS_MISC_1_41_3_MKLOST_FOUND) += e2fsprogs-1.41.3
dir_$(CONFIG_USER_E2FSPROGS_MISC_TUNE2FS)   += e2fsprogs
dir_$(CONFIG_USER_E2FSPROGS_MISC_1_41_3_TUNE2FS)   += e2fsprogs-1.41.3
dir_$(CONFIG_USER_E2FSPROGS_MISC_UUIDGEN)   += e2fsprogs
dir_$(CONFIG_USER_E2FSPROGS_MISC_1_41_3_UUIDGEN)   += e2fsprogs-1.41.3
dir_$(CONFIG_USER_ELVISTINY_VI)             += elvis-tiny
dir_$(CONFIG_USER_ETHATTACH_ETHATTACH)      += ethattach
dir_$(CONFIG_USER_ETHTOOL_ETHTOOL)          += ethtool
dir_$(CONFIG_USER_EXPAT_EXAMPLES)           += expat-examples
dir_$(CONFIG_USER_EZIPUPDATE_EZIPUPDATE)    += ez-ipupdate
dir_$(CONFIG_USER_FAKEIDENTD_IDENTD)        += fakeidentd
dir_$(CONFIG_USER_FDISK_FDISK)              += fdisk
dir_$(CONFIG_USER_FDISK_SFDISK)             += fdisk
dir_$(CONFIG_USER_FILEUTILS_CAT)            += fileutils
dir_$(CONFIG_USER_FILEUTILS_CHGRP)          += fileutils
dir_$(CONFIG_USER_FILEUTILS_CHMOD)          += fileutils
dir_$(CONFIG_USER_FILEUTILS_CHOWN)          += fileutils
dir_$(CONFIG_USER_FILEUTILS_CMP)            += fileutils
dir_$(CONFIG_USER_FILEUTILS_CP)             += fileutils
dir_$(CONFIG_USER_FILEUTILS_DD)             += fileutils
dir_$(CONFIG_USER_FILEUTILS_GREP)           += fileutils
dir_$(CONFIG_USER_FILEUTILS_L)              += fileutils
dir_$(CONFIG_USER_FILEUTILS_LN)             += fileutils
dir_$(CONFIG_USER_FILEUTILS_LS)             += fileutils
dir_$(CONFIG_USER_FILEUTILS_MKDIR)          += fileutils
dir_$(CONFIG_USER_FILEUTILS_MKFIFO)         += fileutils
dir_$(CONFIG_USER_FILEUTILS_MKNOD)          += fileutils
dir_$(CONFIG_USER_FILEUTILS_MORE)           += fileutils
dir_$(CONFIG_USER_FILEUTILS_MV)             += fileutils
dir_$(CONFIG_USER_FILEUTILS_RM)             += fileutils
dir_$(CONFIG_USER_FILEUTILS_RMDIR)          += fileutils
dir_$(CONFIG_USER_FILEUTILS_SYNC)           += fileutils
dir_$(CONFIG_USER_FILEUTILS_TOUCH)          += fileutils
dir_$(CONFIG_USER_FNORD_HTTPD)              += fnord
dir_$(CONFIG_USER_FLASHW_FLASHW)            += flashw
dir_$(CONFIG_USER_FLATFSD_FLATFSD)          += flatfsd
dir_$(CONFIG_USER_FREESWAN)                 += freeswan
dir_$(CONFIG_USER_FROB_LED_FROB_LED)        += frob-led
dir_$(CONFIG_USER_FSWCERT_FSWCERT)          += fswcert
dir_$(CONFIG_USER_FTP_FTP_FTP)              += ftp
dir_$(CONFIG_USER_FTPD_FTPD)                += ftpd
dir_$(CONFIG_USER_FTPD_FTPD_0_3_3)          += ftpd_0.3.3
dir_$(CONFIG_USER_FTPD_VSFTPD)              += vsftpd-2.3.4
dir_$(CONFIG_USER_FTPD_PUREFTPD)            += pure-ftpd
dir_$(CONFIG_USER_GETTEXT)                  += gettext
dir_$(CONFIG_USER_GPSD)                     += gpsd
dir_$(CONFIG_USER_GDBSERVER53_GDBSERVER)    += gdbserver.5.3
dir_$(CONFIG_USER_GDBSERVER_GDBREPLAY)      += gdbserver
dir_$(CONFIG_USER_GDBSERVER_GDBSERVER)      += gdbserver
dir_$(CONFIG_USER_GETTYD_GETTYD)            += gettyd
dir_$(CONFIG_USER_GPG)                      += libgpg-error-1.10 
dir_$(CONFIG_USER_GREP_GREP)                += grep
dir_$(CONFIG_USER_HD_HD)                    += hd
dir_$(CONFIG_USER_HOSTAP_HOSTAP)            += hostap
dir_$(CONFIG_USER_HOSTAP_HOSTAP)            += hostap/utils
dir_$(CONFIG_USER_HOSTAP_HOSTAPD)           += hostap/hostapd
dir_$(CONFIG_USER_HOSTAP_HOSTAPD_0_6_10)    += hostapd-0.6.10
dir_$(CONFIG_USER_HOSTAP_HOSTAPD_0_7_3)     += hostapd-0.7.3
dir_$(CONFIG_USER_HOSTAP_HOSTAPD_1_0)       += hostapd-1.0
dir_$(CONFIG_USER_HOSTAP_HOSTAPD_2_0)       += hostapd-2.0
dir_$(CONFIG_USER_HTTPD_HTTPD)              += httpd
dir_$(CONFIG_USER_HTTPTUNNEL_CLIENT)        += httptunnel
dir_$(CONFIG_USER_HTTPTUNNEL_SERVER)        += httptunnel
dir_$(CONFIG_USER_HWCLOCK_HWCLOCK)          += hwclock
dir_$(CONFIG_USER_IFATTACH_IFATTACH)        += ifattach
dir_$(CONFIG_USER_IKED)                     += iked
dir_$(CONFIG_USER_IFENSLAVE)                += ifenslave
dir_$(CONFIG_USER_INETD_INETD)              += inetd
dir_$(CONFIG_USER_INETD_INETD_IPV6)         += inetd_ipv6
dir_$(CONFIG_USER_INIT.ORG_INIT)            += init.org
dir_$(CONFIG_USER_INIT_EXPAND)              += init
dir_$(CONFIG_USER_INIT_INIT)                += init
dir_$(CONFIG_USER_IPERF)                    += iperf
dir_$(CONFIG_USER_IPCHAINS_IPCHAINS)        += ipchains
dir_$(CONFIG_USER_IPFWADM_IPFWADM)          += ipfwadm
dir_$(CONFIG_USER_IPKG)                     += ipkg
dir_$(CONFIG_USER_IPMASQADM_IPMASQADM)      += ipmasqadm
dir_$(CONFIG_USER_IPPORTFW_IPPORTFW)        += ipportfw
#dir_$(CONFIG_USER_IPSEC_TOOLS)              += ipsec-tools-0.7
dir_$(CONFIG_USER_IPSEC_TOOLS)              += ipsec-tools-0.8.0
dir_$(CONFIG_USER_IPREDIR_IPREDIR)          += ipredir
dir_$(CONFIG_USER_IPROUTE2)                 += iproute2
dir_$(CONFIG_USER_IPTABLES_IPTABLES)        += iptables
dir_$(CONFIG_USER_IPTABLES_IPTABLES_1_4_8)        += iptables-1.4.8
dir_$(CONFIG_USER_IPUTILS_IPUTILS)          += iputils
dir_$(CONFIG_USER_IW)                       += iw
dir_$(CONFIG_USER_KENDIN_CONFIG)            += kendin-config
dir_$(CONFIG_USER_KLAXON_KLAXON)            += klaxon
dir_$(CONFIG_USER_L2TPD_L2TPD)              += l2tpd
dir_$(CONFIG_USER_LANG_A60)                 += a60
dir_$(CONFIG_USER_LCD_LCD)                  += lcd
dir_$(CONFIG_USER_LEDCON_LEDCON)            += ledcon
dir_$(CONFIG_USER_LEVEE_VI)                 += levee
dir_$(CONFIG_USER_NANO)                     += nano
dir_$(CONFIG_USER_LINUXIGD_LINUXIGD)        += linux-igd
dir_$(CONFIG_USER_LILO_LILO)                += lilo
dir_$(CONFIG_USER_LIRC)                     += lirc
dir_$(CONFIG_USER_LISSA_LISSA)              += lissa
dir_$(CONFIG_USER_LOATTACH_LOATTACH)        += loattach
dir_$(CONFIG_USER_LOGIN_LOGIN)              += login
dir_$(CONFIG_USER_LOGIN_PASSWD)             += login
dir_$(CONFIG_USER_LRPSTAT_LRPSTAT)          += lrpstat
dir_$(CONFIG_USER_LRZSZ_LRZSZ)              += lrzsz
dir_$(CONFIG_USER_LVM)			    += LVM2.2.02.96
dir_$(CONFIG_USER_MAIL_MAIL_IP)             += mail
dir_$(CONFIG_USER_MAILX)                    += mailx
dir_$(CONFIG_USER_MATH_TEST)                += mathtest
dir_$(CONFIG_USER_MAWK_AWK)                 += mawk
dir_$(CONFIG_USER_UTIL_LINIX)               += util-linux
dir_$(CONFIG_USER_MTDUTILS)                 += mtd-utils
dir_$(CONFIG_USER_MICROWIN)                 += microwin
dir_$(CONFIG_USER_MINI_HTTPD_MINI_HTTPD)    += mini_httpd
dir_$(CONFIG_USER_MINICOM)                  += minicom
dir_$(CONFIG_USER_MOUNT_MOUNT)              += mount
dir_$(CONFIG_USER_MOUNT_UMOUNT)             += mount
dir_$(CONFIG_USER_MDADM)		    += mdadm-3.2.5
dir_$(CONFIG_USER_MP3PLAY_MP3PLAY)          += mp3play
dir_$(CONFIG_USER_MSNTP_MSNTP)              += msntp
dir_$(CONFIG_USER_MUSICBOX_MUSICBOX)        += musicbox
dir_$(CONFIG_USER_MYSQL)                    += mysql
dir_$(CONFIG_USER_MYSQL_5_6_11)             += mysql-5.6.11
dir_$(CONFIG_USER_NESSUS_NASL)              += nessus
dir_$(CONFIG_USER_NETCAT_NETCAT)	    += netcat
dir_$(CONFIG_USER_NETFLASH_NETFLASH)        += netflash
dir_$(CONFIG_USER_NET_TOOLS_ARP)            += net-tools
dir_$(CONFIG_USER_NET_TOOLS_HOSTNAME)       += net-tools
dir_$(CONFIG_USER_NET_TOOLS_IFCONFIG)       += net-tools
dir_$(CONFIG_USER_NET_TOOLS_NAMEIF)         += net-tools
dir_$(CONFIG_USER_NET_TOOLS_NETSTAT)        += net-tools
dir_$(CONFIG_USER_NET_TOOLS_PLIPCONFIG)     += net-tools
dir_$(CONFIG_USER_NET_TOOLS_RARP)           += net-tools
dir_$(CONFIG_USER_NET_TOOLS_ROUTE)          += net-tools
dir_$(CONFIG_USER_NET_TOOLS_SLATTACH)       += net-tools
dir_$(CONFIG_USER_NET_TOOLS_IPMADDR)        += net-tools
dir_$(CONFIG_USER_NET_TOOLS_IPTUNNEL)       += net-tools
dir_$(CONFIG_USER_NET_TOOLS_MII_TOOL)       += net-tools
dir_$(CONFIG_USER_NETSTAT_NAT_NETSTAT_NAT)  += netstat-nat
dir_$(CONFIG_USER_NTPD_NTPD)                += ntp
dir_$(CONFIG_USER_NTPD_NTPDATE)             += ntp
dir_$(CONFIG_USER_NTPD_NTPQ)                += ntp
dir_$(CONFIG_USER_NTPD_NTPD_4_2_6)          += ntp-4.2.6p5
dir_$(CONFIG_USER_NULL_NULL)                += null
dir_$(CONFIG_USER_NWSH_SH)                  += nwsh
dir_$(CONFIG_USER_OGGPLAY_EXAMPLE)          += oggplay
dir_$(CONFIG_USER_OGGPLAY_OGG123)           += oggplay
dir_$(CONFIG_USER_PALMBOT_PALMBOT)          += palmbot
dir_$(CONFIG_USER_PCIUTILS_LSPCI)           += pciutils
dir_$(CONFIG_USER_PCIUTILS_SETPCI)          += pciutils
dir_$(CONFIG_USER_PCMCIA_CS)                += pcmcia-cs-3.2.8
dir_$(CONFIG_USER_PERL_PERL)                += perl
dir_$(CONFIG_USER_PIMD)                     += pimd
dir_$(CONFIG_USER_PING_PING)                += ping
dir_$(CONFIG_USER_PLAY_PLAY)                += play
dir_$(CONFIG_USER_PLAY_TONE)                += play
dir_$(CONFIG_USER_PLAYRT_PLAYRT)            += playrt
dir_$(CONFIG_USER_PLUG_PLUG)                += plug
dir_$(CONFIG_USER_PORTMAP_PORTMAP)          += portmap
#dir_$(CONFIG_USER_PPPD_PPPD_PPPD)           += pppd
dir_$(CONFIG_USER_PPPD_PPPD_PPPD)           += pppd-2.4.5
dir_$(CONFIG_USER_PPTP_CLIENT_PPTP)         += pptp-client
dir_$(CONFIG_USER_PPTP_CLIENT_PPTP_CALLMGR) += pptp-client
dir_$(CONFIG_USER_PPTP_CLIENT_PPTP_CALLMGR) += pptp-client
dir_$(CONFIG_USER_PPTPD_PPTPCTRL)           += pptpd
dir_$(CONFIG_USER_PPTPD_PPTPD)              += pptpd
dir_$(CONFIG_USER_PROCPS_FREE)              += procps
dir_$(CONFIG_USER_PROCPS_KILL)              += procps
dir_$(CONFIG_USER_PROCPS_PGREP)             += procps
dir_$(CONFIG_USER_PROCPS_PKILL)             += procps
dir_$(CONFIG_USER_PROCPS_PS)                += procps
dir_$(CONFIG_USER_PROCPS_SKILL)             += procps
dir_$(CONFIG_USER_PROCPS_SNICE)             += procps
dir_$(CONFIG_USER_PROCPS_SYSCTL)            += procps
dir_$(CONFIG_USER_PROCPS_TLOAD)             += procps
dir_$(CONFIG_USER_PROCPS_TOP)               += procps
dir_$(CONFIG_USER_PROCPS_UPTIME)            += procps
dir_$(CONFIG_USER_PROCPS_VMSTAT)            += procps
dir_$(CONFIG_USER_PROCPS_W)                 += procps
dir_$(CONFIG_USER_PROCPS_WATCH)             += procps
dir_$(CONFIG_USER_PYTHON_PYTHON)            += python-2.6.6
dir_$(CONFIG_USER_PYTHON_2_7_5)             += python-2.7.5
dir_$(CONFIG_USER_RDATE_RDATE)              += rdate
dir_$(CONFIG_USER_RAMIMAGE_RAMFS64)         += ramimage
dir_$(CONFIG_USER_RAMIMAGE_RAMFS128)        += ramimage
dir_$(CONFIG_USER_RAMIMAGE_RAMFS256)        += ramimage
dir_$(CONFIG_USER_RAMIMAGE_RAMFS512)        += ramimage
dir_$(CONFIG_USER_RAMIMAGE_RAMFS1024)       += ramimage
dir_$(CONFIG_USER_RAMIMAGE_RAMFS2048)       += ramimage
dir_$(CONFIG_USER_RAMIMAGE_RAMFS8192)       += ramimage
dir_$(CONFIG_USER_READPROFILE_READPROFILE)  += readprofile
dir_$(CONFIG_USER_RECOVER_RECOVER)          += recover
dir_$(CONFIG_USER_REISERFSPROGS)            += reiserfsprogs
dir_$(CONFIG_USER_ROOTLOADER_ROOTLOADER)    += rootloader
dir_$(CONFIG_USER_ROUTE_ARP)                += route
dir_$(CONFIG_USER_ROUTE_IFCONFIG)           += route
dir_$(CONFIG_USER_ROUTE_MIITOOL)            += route
dir_$(CONFIG_USER_ROUTE_ROUTE)              += route
dir_$(CONFIG_USER_ROUTE_NETSTAT)            += route
dir_$(CONFIG_USER_ROUTED_ROUTED)            += routed
dir_$(CONFIG_USER_RP_PPPOE_PPPOE)           += rp-pppoe
dir_$(CONFIG_USER_RP_PPPOE_PPPOE_SERVER)    += rp-pppoe
dir_$(CONFIG_USER_RSYSLOGD_RSYSLOGD)        += rsyslogd
dir_$(CONFIG_USER_SYSLOGNG_SYSLOGNG_3_4_1)  += syslog-ng-3.4.1
dir_$(CONFIG_USER_LOGROTATE_LOGROTATE_3_8_6)+= logrotate-3.8.6
dir_$(CONFIG_USER_RTC_M41T11)               += rtc-m41t11
dir_$(CONFIG_USER_RTC_DS1302)               += rtc-ds1302
dir_$(CONFIG_USER_SAMBA)                    += samba-3.5.6
dir_$(CONFIG_USER_SASH_REBOOT)              += sash
dir_$(CONFIG_USER_SASH_SH)                  += sash
dir_$(CONFIG_USER_SASH_SHUTDOWN)            += sash
dir_$(CONFIG_USER_SASH_SHUTDOWN)            += sash
dir_$(CONFIG_USER_SER_SER)                  += ser
dir_$(CONFIG_USER_SER2NET)                  += ser2net
dir_$(CONFIG_USER_SETHDLC_SETHDLC)          += sethdlc
dir_$(CONFIG_USER_SETMAC_SETMAC)            += setmac
dir_$(CONFIG_USER_SETSERIAL_SETSERIAL)      += setserial
dir_$(CONFIG_USER_SH_SH)                    += sh
dir_$(CONFIG_USER_SHUTILS_BASENAME)         += shutils
dir_$(CONFIG_USER_SHUTILS_DATE)             += shutils
dir_$(CONFIG_USER_SHUTILS_DIRNAME)          += shutils
dir_$(CONFIG_USER_SHUTILS_ECHO)             += shutils
dir_$(CONFIG_USER_SHUTILS_FALSE)            += shutils
dir_$(CONFIG_USER_SHUTILS_LOGNAME)          += shutils
dir_$(CONFIG_USER_SHUTILS_PRINTENV)         += shutils
dir_$(CONFIG_USER_SHUTILS_PWD)              += shutils
dir_$(CONFIG_USER_SHUTILS_TRUE)             += shutils
dir_$(CONFIG_USER_SHUTILS_UNAME)            += shutils
dir_$(CONFIG_USER_SHUTILS_WHICH)            += shutils
dir_$(CONFIG_USER_SHUTILS_WHOAMI)           += shutils
dir_$(CONFIG_USER_SHUTILS_YES)              += shutils
dir_$(CONFIG_USER_SLATTACH_SLATTACH)        += slattach
dir_$(CONFIG_USER_SMART)		    += smartmontools-5.42
dir_$(CONFIG_USER_SMBMOUNT_SMBMOUNT)        += smbmount
dir_$(CONFIG_USER_SMBMOUNT_SMBUMOUNT)       += smbmount
dir_$(CONFIG_USER_SMTP_SMTPCLIENT)          += smtpclient
dir_$(CONFIG_USER_SMCROUTE)                 += smcroute
dir_$(CONFIG_USER_SNMPD_SNMPD)              += snmpd
dir_$(CONFIG_USER_SNORT_SNORT)              += snort
dir_$(CONFIG_USER_SOCAT)                    += socat
dir_$(CONFIG_USER_SPEEDTOUCH_MODEM_RUN)     += speedtouch/src
dir_$(CONFIG_USER_STUNNEL_STUNNEL)          += stunnel
dir_$(CONFIG_USER_SSLWRAP_SSLWRAP)          += sslwrap
dir_$(CONFIG_USER_SQUID_SQUID)              += squid
dir_$(CONFIG_USER_SSH_SSHD)                 += ssh
dir_$(CONFIG_USER_SSH_SSH)                  += ssh
dir_$(CONFIG_USER_SSH_SSHKEYGEN)            += ssh
dir_$(CONFIG_USER_STP_STP)                  += stp
dir_$(CONFIG_USER_SQLITE3)                  += sqlite3
dir_$(CONFIG_USER_STRACE_STRACE)            += strace
dir_$(CONFIG_USER_STTY_STTY)                += stty
dir_$(CONFIG_USER_SUDO)                     += sudo
dir_$(CONFIG_USER_SYSUTILS_DF)              += sysutils
dir_$(CONFIG_USER_SYSUTILS_FREE)            += sysutils
dir_$(CONFIG_USER_SYSUTILS_HOSTNAME)        += sysutils
dir_$(CONFIG_USER_SYSUTILS_KILL)            += sysutils
dir_$(CONFIG_USER_SYSUTILS_PS)              += sysutils
dir_$(CONFIG_USER_SYSUTILS_REBOOT)          += sysutils
dir_$(CONFIG_USER_SYSUTILS_SHUTDOWN)        += sysutils
dir_$(CONFIG_USER_TCPBLAST_TCPBLAST)        += tcpblast
dir_$(CONFIG_USER_TCPWRAP_TCPD)             += tcpwrappers
dir_$(CONFIG_USER_TCPDUMP_TCPDUMP)          += tcpdump
dir_$(CONFIG_USER_TCSH_TCSH)                += tcsh
dir_$(CONFIG_USER_TELNET_TELNET)            += telnet
dir_$(CONFIG_USER_TELNETD_TELNETD)          += telnetd
dir_$(CONFIG_USER_TELNETD_TELNETD_0_17)     += telnetd_0.17
dir_$(CONFIG_USER_TFTP_TFTP)                += tftp
dir_$(CONFIG_USER_TFTPD_TFTPD)              += tftpd
dir_$(CONFIG_USER_THREADDEMOS_THREADDEMOS)  += threaddemos
dir_$(CONFIG_USER_THTTPD_THTTPD)            += thttpd
dir_$(CONFIG_USER_TINYLOGIN_CRYPT_CRYPT)    += tinylogin
dir_$(CONFIG_USER_TINYLOGIN_TINYLOGIN)      += tinylogin
dir_$(CONFIG_USER_TINYTCL_TCLSH)            += tinytcl
dir_$(CONFIG_USER_TIP_TIP)                  += tip
dir_$(CONFIG_USER_TIMEPEG_TPT)              += tpt
dir_$(CONFIG_USER_TRACEROUTE_TRACEROUTE)    += traceroute
dir_$(CONFIG_USER_TCPTRACEROUTE)            += tcptraceroute
dir_$(CONFIG_USER_TRIPWIRE_SIGGEN)          += tripwire
dir_$(CONFIG_USER_TRIPWIRE_TRIPWIRE)        += tripwire
dir_$(CONFIG_USER_UCDSNMP_SNMPD)            += net-snmp
dir_$(CONFIG_USER_USBCBI_USBCBI)            += cbi_client
dir_$(CONFIG_USER_USBISO_USBISO)            += iso_client
dir_$(CONFIG_USER_VERSION_VERSION)          += version
dir_$(CONFIG_USER_VCONFIG_VCONFIG)          += vconfig
dir_$(CONFIG_USER_VIXIECRON_CRON)           += vixie-cron
dir_$(CONFIG_USER_VIXIECRON_CRONTAB)        += vixie-cron
dir_$(CONFIG_USER_VPLAY_VPLAY)              += vplay
dir_$(CONFIG_USER_VPLAY_VREC)               += vplay
dir_$(CONFIG_USER_VPLAY_MIXER)              += vplay
dir_$(CONFIG_USER_ALSA_UTILS)               += alsa-utils
dir_$(CONFIG_USER_VPNLED_VPNLED)            += vpnled
dir_$(CONFIG_USER_VPNC)                     += vpnc
dir_$(CONFIG_USER_W3CAM_VIDCAT)             += w3cam
dir_$(CONFIG_USER_W3CAM_W3CAMD)             += w3cam
dir_$(CONFIG_USER_WATCHDOG_R1200)           += watchdog_r1200
dir_$(CONFIG_USER_WGET)                     += wget
dir_$(CONFIG_USER_WIRELESS_TOOLS)           += wireless_tools
dir_$(CONFIG_USER_WLAN_NG)                  += wlan-ng
dir_$(CONFIG_USER_ZEBRA_BGPD_BGPD)          += zebra
dir_$(CONFIG_USER_ZEBRA_OSPFD_OSPFD)        += zebra
dir_$(CONFIG_USER_ZEBRA_RIPD_RIPD)          += zebra
dir_$(CONFIG_USER_ZEBRA_ZEBRA_ZEBRA)        += zebra
dir_$(CONFIG_USER_OPENVPN)                  += openvpn-2.1.1
dir_$(CONFIG_USER_APACHE)		    += apache
dir_$(CONFIG_USER_NGINX)		    += nginx
dir_$(CONFIG_USER_PHP)			    += php
dir_$(CONFIG_USER_PHP_5_1_6)		    += php-5.1.6
dir_$(CONFIG_USER_PHP_5_3_10)		    += php-5.3.10
dir_$(CONFIG_USER_LIBPHP_5_3_14)	    += libphp-5.3.14
dir_$(CONFIG_USER_QUOTA)		    += quota-tools
dir_$(CONFIG_USER_UPGRADEHFM)               += upgradehfm
dir_$(CONFIG_USER_BACKUPUF)		    += backupuf
dir_$(CONFIG_USER_BACKUPHFM)		    += backuphfm
#dir_$(CONFIG_USER_CHBKFILE)		    += chbkfile
dir_$(CONFIG_USER_REPORTIP)		    += reportip
dir_$(CONFIG_USER_MX_SMG1100)               += moxalib_smg1100
dir_$(CONFIG_USER_LIBSMS)                   += libsms-0.1
dir_$(CONFIG_USER_DCUT3TEST)		    += dcu-t3-test
dir_$(CONFIG_USER_MOXABURN)		    += moxaburn
dir_$(CONFIG_USER_MOXABURN_UC8400)	    += moxaburn_uc8400
dir_$(CONFIG_USER_READY_LED)		    += readyled
dir_$(CONFIG_USER_FAULT_LED)		    += faultled
dir_$(CONFIG_USER_REALTTY_1_18)		    += npreal2_1.18
dir_$(CONFIG_USER_MOXA_PCIE_RESET)	    += mx_pcie_reset
dir_$(CONFIG_USER_MOXA_IGT)	            += mx_igt
dir_$(CONFIG_USER_UC_CTRL)	            += uc_ctrl
dir_$(CONFIG_USER_XMLLINT)	            += xmllint-2.9.0

# add by Victor Yu. 07-22-2005 following
dir_$(CONFIG_USER_SYSVINIT)		    += sysvinit-2.86
dir_$(CONFIG_USER_NFS_UTILS)		    += nfs-utils
#dir_$(CONFIG_USER_WPA_TOOLS)                += wpa_supplicant-0.4.7
dir_$(CONFIG_USER_WPA_TOOLS)                += wpa_supplicant-0.7.3
#dir_$(CONFIG_USER_WPA_TOOLS)                += wpa-supplicant-2.2
dir_$(CONFIG_USER_WIMAX_TOOLS)              += wimax-tools
dir_$(CONFIG_USER_WIMAX_SERVICE)            += wimax-service
dir_$(CONFIG_USER_MXMPTEST)                 += mxmptest
dir_$(CONFIG_USER_MXMPTEST_UC8400)          += mxmptest_uc8400
#dir_$(CONFIG_USER_OPENSWAN)		    += openswan
dir_$(CONFIG_USER_OPENSWAN)		    += openswan-2.4.15
dir_$(CONFIG_USER_OPENLLDP)		    += openlldp-0.4alpha
dir_$(CONFIG_USER_OPENSC)                   += OpenSC
dir_$(CONFIG_USER_WTEST)                    += wtest
dir_$(CONFIG_LIB_QT_4_5_1)		    += qt-lib-4.5.1
dir_$(CONFIG_LIB_QT_4_7_1)		    += qt-lib-4.7.1
dir_$(CONFIG_USER_LCMMESSAGE)			+= lcmmessage
dir_$(CONFIG_USER_LCMSHOWINFO)			+= lcmshowinfo
dir_$(CONFIG_USER_SHOWIPONLCM)			+= showiponlcm

# add by Victor Yu. 02-19-2008
dir_$(CONFIG_USER_ZHCON)		    += zhcon
# add by Victor Yu. 07-22-2005 above
dir_y	+= setkey
dir_y	+= setinterface
dir_y   += kversion

ifeq ($(CONFIG_PRODUCT),w345)
dir_y	+= gprsled
endif

ifeq ($(CONFIG_PRODUCT),w345with16128)
dir_y	+= gprsled
endif

ifeq ($(CONFIG_PRODUCT),w325)
dir_y	+= gprsled
endif

ifeq ($(CONFIG_PRODUCT),w315)
dir_y	+= gprsled
endif

ifeq ($(CONFIG_PRODUCT),ia240_mari)
CFLAGS += -DARCH_IA240_MARI
endif

ifeq ($(CONFIG_PRODUCT),ia241_nsps)
CFLAGS += -DARCH_IA241_NSPS
endif

ifeq ($(CONFIG_PRODUCT),uc_7112_lx_plus_liton)
CFLAGS += -DARCH_UC_7112_LX_PLUS_LITON
endif

ifeq ($(CONFIG_PRODUCT),w345_imp1)
CFLAGS += -DARCH_W345_IMP1
endif

all: config
	for i in $(sort $(dir_y)) $(dir_v) $(dir_p); do \
		[ ! -d $$i ] || make -C $$i || exit $$? ; \
	done
#	if [ $(CONFIG_USER_PERL_PERL) == "y" ]; then \
#		unset CFLAGS CC CPPFLAGS CXXFLAGS && cd perl/Cross && make perl ; \
#	fi

romfs:
	for i in $(dir_v) $(sort $(dir_y)) $(dir_p) ; do \
		[ ! -d $$i ] || make -C $$i romfs || exit $$? ; \
	done
#	if [ $(CONFIG_USER_PERL_PERL) == "y" ]; then \
#		cd perl && make romfs;\
#	fi

package:
	for i in $(dir_y) ; do \
		[ ! -d $$i ] || make -C $$i package || exit $$? ; \
	done

clean:
	-for i in $(dir_v) $(sort $(dir_y) $(dir_n) $(dir_)) $(dir_p) ; do \
		[ ! -d $$i ] || make -C $$i clean; \
	done

prune:
	-for i in $(sort $(dir_n) $(dir_)) ; do \
		found=0; \
		for j in $(sort $(dir_y)) ; do \
			if [ $$i == $$j ]; then \
				found=1; \
			fi; \
		done; \
		if [ $$found == 0 ]; then \
			rm -r $$i; \
		fi; \
	done


%_only:
	make -C $(@:_only=)

%_romfs %_debianfs %_package %_menuconfig %_clean:
	[ ! -d "$(word 1,$(subst _, ,$@))" ] || \
		$(MAKEARCH) -C $(word 1,$(subst _, ,$@)) \
		$(word 2,$(subst _, ,$@)); \

