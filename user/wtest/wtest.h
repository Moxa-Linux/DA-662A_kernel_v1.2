#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>

#include <linux/sockios.h>

#define	ZDAPIOCTL	(SIOCDEVPRIVATE)

struct zdap_ioctl {
	unsigned short cmd;                /* Command to run */
	unsigned long addr;                /* Length of the data buffer */
	unsigned long value;               /* Pointer to the data buffer */
	unsigned char data[0x100];
};

#define ZD_IOCTL_REG_READ			0x01
#define ZD_IOCTL_REG_WRITE			0x02
#define ZD_IOCTL_MEM_DUMP			0x03
#define ZD_IOCTL_RATE       			0x04
#define ZD_IOCTL_SNIFFER    			0x05
#define ZD_IOCTL_CAM_DUMP   			0x06
#define ZD_IOCTL_DUMP_PHY   			0x07
#define ZD_IOCTL_CARD_SETTING 			0x08
#define ZD_IOCTL_HASH_DUMP			0x09
#define ZD_IOCTL_RFD_DUMP			0x0A
#define ZD_IOCTL_MEM_READ			0x0B
#define ZD_IOCTL_MEM_WRITE			0x0C

//for STA
#define ZD_IOCTL_TX_RATE			0x0D
#define ZD_IOCTL_EEPROM				0x0E

#define ZD_IOCTL_BCN				0x10
#define ZD_IOCTL_REG_READ16			0x11
#define ZD_IOCTL_REG_WRITE16			0x12

//for CAM Test
#define ZD_IOCTL_CAM_READ			0x13
#define ZD_IOCTL_CAM_WRITE			0x14
#define ZD_IOCTL_CAM_RESET			0x15
#define ZD_IOCTL_READ_PHY			0x16
#define ZD_IOCTL_WRITE_PHY			0x17
#define ZD_IOCTL_CONT_TX            0x18
#define ZD_IOCTL_SET_MIC_CNT_ENABLE 0x19
#define ZD_IOCTL_GET_MIC_CNT_ENABLE 0x1A
#define	ZD_IOCTL_SET_CHANNEL        0x1B
#define ZD_IOCTL_DEBUG_FLAG         0x21
#define ZD_IOCTL_UW_PWR             0x28

const int dot11A_Channel[]={36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140,184,188,192,196,8,12,16,34,38,42,46,149,153,157,161,165};


//prince add

#define		RTS_RATE_1M				0x00
#define		RTS_RATE_11M			0x03
#define		RTS_RATE_24M			0x09
#define		RTS_MOD_TYPE_OFDM		0x100
#define		RTS_PMB_TYPE_SHORT		0x200
#define		CTS_RATE_11M			0x30000
#define		CTS_RATE_24M			0x90000
#define		CTS_MOD_TYPE_OFDM		0x1000000
#define		CTS_PMB_TYPE_SHORT		0x2000000
#define		NON_BARKER_PMB_SET		(RTS_MOD_TYPE_OFDM | RTS_PMB_TYPE_SHORT | CTS_MOD_TYPE_OFDM | CTS_PMB_TYPE_SHORT)
#define		NON_PROTECT_SET			(RTS_MOD_TYPE_OFDM | CTS_MOD_TYPE_OFDM)

#define		ZD_CR0			0x0000
#define		ZD_CR1			0x0004
#define		ZD_CR2			0x0008
#define		ZD_CR3			0x000C
#define		ZD_CR5			0x0010
#define		ZD_CR6			0x0014
#define		ZD_CR7			0x0018
#define		ZD_CR8			0x001C
#define		ZD_CR4			0x0020
#define		ZD_CR9			0x0024
#define		ZD_CR10			0x0028
#define		ZD_CR11			0x002C
#define		ZD_CR12			0x0030
#define		ZD_CR13			0x0034
#define		ZD_CR14			0x0038
#define		ZD_CR15			0x003C
#define		ZD_CR16			0x0040
#define		ZD_CR17			0x0044
#define		ZD_CR18			0x0048
#define		ZD_CR19			0x004C
#define		ZD_CR20			0x0050
#define		ZD_CR21			0x0054
#define		ZD_CR22			0x0058
#define		ZD_CR23			0x005C
#define		ZD_CR24			0x0060
#define		ZD_CR25			0x0064
#define		ZD_CR26			0x0068
#define		ZD_CR27			0x006C
#define		ZD_CR28			0x0070
#define		ZD_CR29			0x0074
#define		ZD_CR30			0x0078
#define		ZD_CR31			0x007C
#define		ZD_CR32			0x0080
#define		ZD_CR33			0x0084
#define		ZD_CR34			0x0088
#define		ZD_CR35			0x008C
#define		ZD_CR36			0x0090
#define		ZD_CR37			0x0094
#define		ZD_CR38			0x0098
#define		ZD_CR39			0x009C
#define		ZD_CR40			0x00A0
#define		ZD_CR41			0x00A4
#define		ZD_CR42			0x00A8
#define		ZD_CR43			0x00AC
#define		ZD_CR44			0x00B0
#define		ZD_CR45			0x00B4
#define		ZD_CR46			0x00B8
#define		ZD_CR47			0x00BC
#define		ZD_CR48			0x00C0
#define		ZD_CR49			0x00C4
#define		ZD_CR50			0x00C8
#define		ZD_CR51			0x00CC
#define		ZD_CR52			0x00D0
#define		ZD_CR53			0x00D4
#define		ZD_CR54			0x00D8
#define		ZD_CR55			0x00DC
#define		ZD_CR56			0x00E0
#define		ZD_CR57			0x00E4
#define		ZD_CR58			0x00E8
#define		ZD_CR59			0x00EC
#define		ZD_CR60			0x00F0
#define		ZD_CR61			0x00F4
#define		ZD_CR62			0x00F8
#define		ZD_CR63			0x00FC
#define		ZD_CR64			0x0100
#define		ZD_CR65			0x0104
#define		ZD_CR66			0x0108
#define		ZD_CR67			0x010C
#define		ZD_CR68			0x0110
#define		ZD_CR69			0x0114
#define		ZD_CR70			0x0118
#define		ZD_CR71			0x011C
#define		ZD_CR72			0x0120
#define		ZD_CR73			0x0124
#define		ZD_CR74			0x0128
#define		ZD_CR75			0x012C
#define		ZD_CR76			0x0130
#define		ZD_CR77			0x0134
#define		ZD_CR78			0x0138
#define		ZD_CR79			0x013C
#define		ZD_CR80			0x0140
#define		ZD_CR81			0x0144
#define		ZD_CR82			0x0148
#define		ZD_CR83			0x014C
#define		ZD_CR84			0x0150
#define		ZD_CR85			0x0154
#define		ZD_CR86			0x0158
#define		ZD_CR87			0x015C
#define		ZD_CR88			0x0160
#define		ZD_CR89			0x0164
#define		ZD_CR90			0x0168
#define		ZD_CR91			0x016C
#define		ZD_CR92			0x0170
#define		ZD_CR93			0x0174
#define		ZD_CR94			0x0178
#define		ZD_CR95			0x017C
#define		ZD_CR96			0x0180
#define		ZD_CR97			0x0184
#define		ZD_CR98			0x0188
#define		ZD_CR99			0x018C
#define		ZD_CR100		0x0190
#define		ZD_CR101		0x0194
#define		ZD_CR102		0x0198
#define		ZD_CR103		0x019C
#define		ZD_CR104		0x01A0
#define		ZD_CR105		0x01A4
#define		ZD_CR106		0x01A8
#define		ZD_CR107		0x01AC
#define		ZD_CR108		0x01B0
#define		ZD_CR109		0x01B4
#define		ZD_CR110		0x01B8
#define		ZD_CR111		0x01BC
#define		ZD_CR112		0x01C0
#define		ZD_CR113		0x01C4
#define		ZD_CR114		0x01C8
#define		ZD_CR115		0x01CC
#define		ZD_CR116		0x01D0
#define		ZD_CR117		0x01D4
#define		ZD_CR118		0x01D8
#define		ZD_CR119		0x01DC
#define		ZD_CR120		0x01E0
#define		ZD_CR121		0x01E4
#define		ZD_CR122		0x01E8
#define		ZD_CR123		0x01EC
#define		ZD_CR124		0x01F0
#define		ZD_CR125		0x01F4
#define		ZD_CR126		0x01F8
#define		ZD_CR127		0x01FC
#define		ZD_CR128		0x0200
#define		ZD_CR129		0x0204
#define		ZD_CR130		0x0208
#define		ZD_CR131		0x020C
#define		ZD_CR132		0x0210
#define		ZD_CR133		0x0214
#define		ZD_CR134		0x0218
#define		ZD_CR135		0x021C
#define		ZD_CR136		0x0220
#define		ZD_CR137		0x0224
#define		ZD_CR138		0x0228
#define		ZD_CR139		0x022C
#define		ZD_CR140		0x0230
#define		ZD_CR141		0x0234
#define		ZD_CR142		0x0238
#define		ZD_CR143		0x023C
#define		ZD_CR144		0x0240
#define		ZD_CR145		0x0244
#define		ZD_CR146		0x0248
#define		ZD_CR147		0x024C
#define		ZD_CR148		0x0250
#define		ZD_CR149		0x0254
#define		ZD_CR150		0x0258
#define		ZD_CR151		0x025C
#define		ZD_CR152		0x0260
#define		ZD_CR153		0x0264
#define		ZD_CR154		0x0268
#define		ZD_CR155		0x026C
#define		ZD_CR156		0x0270
#define		ZD_CR157		0x0274
#define		ZD_CR158		0x0278
#define		ZD_CR159		0x027C
#define		ZD_CR160		0x0280
#define		ZD_CR161		0x0284
#define		ZD_CR162		0x0288
#define		ZD_CR163		0x028C
#define		ZD_CR164		0x0290
#define		ZD_CR165		0x0294
#define		ZD_CR166		0x0298
#define		ZD_CR167		0x029C
#define		ZD_CR168		0x02A0
#define		ZD_CR169		0x02A4
#define		ZD_CR170		0x02A8
#define		ZD_CR171		0x02AC
#define		ZD_CR172		0x02B0
#define		ZD_CR173		0x02B4
#define		ZD_CR174		0x02B8
#define		ZD_CR175		0x02BC
#define		ZD_CR176		0x02C0
#define		ZD_CR177		0x02C4
#define		ZD_CR178		0x02C8
#define		ZD_CR179		0x02CC
#define		ZD_CR180		0x02D0
#define		ZD_CR181		0x02D4
#define		ZD_CR182		0x02D8
#define		ZD_CR183		0x02DC
#define		ZD_CR184		0x02E0
#define		ZD_CR185		0x02E4
#define		ZD_CR186		0x02E8
#define		ZD_CR187		0x02EC
#define		ZD_CR188		0x02F0
#define		ZD_CR189		0x02F4
#define		ZD_CR190		0x02F8
#define		ZD_CR191		0x02FC
#define		ZD_CR192		0x0300
#define		ZD_CR193		0x0304
#define		ZD_CR194		0x0308
#define		ZD_CR195		0x030C
#define		ZD_CR196		0x0310
#define		ZD_CR197		0x0314
#define		ZD_CR198		0x0318
#define		ZD_CR199		0x031C
#define		ZD_CR200		0x0320
#define		ZD_CR201		0x0324
#define		ZD_CR202		0x0328
#define		ZD_CR203		0x032C
#define		ZD_CR204		0x0330
#define		ZD_CR205		0x0334
#define		ZD_CR206		0x0338
#define		ZD_CR207		0x033C
#define		ZD_CR208		0x0340
#define		ZD_CR209		0x0344
#define		ZD_CR210		0x0348
#define		ZD_CR211		0x034C
#define		ZD_CR212		0x0350
#define		ZD_CR213		0x0354
#define		ZD_CR214		0x0358
#define		ZD_CR215		0x035C
#define		ZD_CR216		0x0360
#define		ZD_CR217		0x0364
#define		ZD_CR218		0x0368
#define		ZD_CR219		0x036C
#define		ZD_CR220		0x0370
#define		ZD_CR221		0x0374
#define		ZD_CR222		0x0378
#define		ZD_CR223		0x037C
#define		ZD_CR224		0x0380
#define		ZD_CR225		0x0384
#define		ZD_CR226		0x0388
#define		ZD_CR227		0x038C
#define		ZD_CR228		0x0390
#define		ZD_CR229		0x0394
#define		ZD_CR230		0x0398
#define		ZD_CR231		0x039C
#define		ZD_CR232		0x03A0
#define		ZD_CR233		0x03A4
#define		ZD_CR234		0x03A8
#define		ZD_CR235		0x03AC
#define		ZD_CR236		0x03B0

#define		ZD_CR240		0x03C0
#define		ZD_CR241		0x03C4
#define		ZD_CR242		0x03C8
#define		ZD_CR243		0x03CC
#define		ZD_CR244		0x03D0
#define		ZD_CR245		0x03D4

#define		ZD_CR251		0x03EC
#define		ZD_CR252		0x03F0
#define		ZD_CR253		0x03F4
#define		ZD_CR254		0x03F8
#define		ZD_CR255		0x03FC

#define		ZD_RF_IF_CLK			0x0400
#define		ZD_RF_IF_DATA			0x0404
#define		ZD_PE1_PE2				0x0408
#define		ZD_PE2_DLY				0x040C
#define		ZD_LE1					0x0410
#define		ZD_LE2					0x0414
#define		ZD_GPI_EN				0x0418
#define		ZD_RADIO_PD				0x042C
#define		ZD_RF2948_PD			0x042C


	
#define		ZD_EnablePSManualAGC	0x043C	// 1: enable
#define		ZD_CONFIGPhilips		0x0440
#define		ZD_SA2400_SER_AP		0x0444
#define		ZD_I2C_WRITE			0x0444	// Same as SA2400_SER_AP (for compatible with ZD1201)
#define		ZD_SA2400_SER_RP		0x0448


#define		ZD_RADIO_PE				0x0458
#define		ZD_RstBusMaster			0x045C

#define		ZD_RFCFG				0x0464

#define		ZD_HSTSCHG				0x046C

#define		ZD_PHY_ON				0x0474
#define		ZD_RX_DELAY				0x0478
#define		ZD_RX_PE_DELAY			0x047C


#define		ZD_GPIO_1				0x0490
#define		ZD_GPIO_2				0x0494


#define		ZD_EncryBufMux			0x04A8


#define		ZD_PS_Ctrl				0x0500
#define		ZD_ADDA_PwrDwn_Ctrl		0x0504
#define		ZD_ADDA_MBIAS_WarmTime	0x0508
#define		ZD_MAC_PS_STATE			0x050C
#define		ZD_InterruptCtrl		0x0510
#define		ZD_TSF_LowPart			0x0514
#define		ZD_TSF_HighPart			0x0518
#define		ZD_ATIMWndPeriod		0x051C
#define		ZD_BCNInterval			0x0520
#define		ZD_Pre_TBTT				0x0524	//In unit of TU(1024us)

//for UART support
#define		ZD_UART_RBR_THR_DLL		0x0540
#define		ZD_UART_DLM_IER			0x0544
#define		ZD_UART_IIR_FCR			0x0548
#define		ZD_UART_LCR				0x054c
#define		ZD_UART_MCR				0x0550
#define		ZD_UART_LSR				0x0554
#define		ZD_UART_MSR				0x0558
#define		ZD_UART_ECR				0x055c
#define		ZD_UART_STATUS			0x0560

#define		ZD_PCI_TxAddr_p1		0x0600
#define		ZD_PCI_TxAddr_p2		0x0604
#define		ZD_PCI_RxAddr_p1		0x0608
#define		ZD_PCI_RxAddr_p2		0x060C
#define		ZD_MACAddr_P1			0x0610
#define		ZD_MACAddr_P2			0x0614
#define		ZD_BSSID_P1				0x0618
#define		ZD_BSSID_P2				0x061C
#define		ZD_BCNPLCPCfg			0x0620
#define		ZD_GroupHash_P1			0x0624
#define		ZD_GroupHash_P2			0x0628
#define		ZD_RX_TIMEOUT			0x062C

#define		ZD_BasicRateTbl			0x0630
#define		ZD_MandatoryRateTbl		0x0634
#define		ZD_RTS_CTS_Rate			0x0638

//				Wep protect
//				_
//	if set to 0x114
//				 __
//				0x14 * slot time(if is 2us) = 40us = wep init time.
//
// So if we change slot time, change 0x14 to let wep init time close to 40us.
#define		ZD_Wep_Protect			0x063C
#define		ZD_RX_THRESHOLD			0x0640

#define		RFCFG1				0x0644
#define		ZD_AfterPNP			0x0648
#define		ZD_AckTime80211		0x0658	

#define		ZD_Rx_OFFSET			0x065c	
#define		ZD_BCNLENGTH			0x0664
#define		ZD_PHYDelay				0x066C
#define		ZD_BCNFIFO				0x0670
#define		ZD_SnifferOn			0x0674
#define		ZD_EncryType			0x0678
#define		ZD_CtlReg1				0x0680	//Bit0:		IBSS mode

