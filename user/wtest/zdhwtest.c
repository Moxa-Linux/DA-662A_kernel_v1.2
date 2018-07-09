#ifdef WLAN_HW_TEST

#include	<userlib/stdlib.h>
#include	<userlib/ddk_func.h>
#include	<userlib/ddk_lib.h>
#include	<userlib/mos_sys.h>
#include	<userlib/mos_task.h>
#include	<userlib/mos_time.h>
#include	<userlib/mos_usb.h>
#include	<userlib/string.h>
#include	<userlib/mos_dio.h>
#include	<sharelib/sharelib.h>
#include	"zdfunc.h"
#include	"80211.h"
#include	"wlink.h"
#include	"zdhw.h"

extern int Scf_getWlanTestChannel(void);
extern int Scf_getWlanTxRate(void);
extern int Scf_getWlanTxPower(void);
extern int Scf_getWlanContinuousTx(void);
extern int Scf_getWlanContinuousRx(void);

extern char *Cwlan_hw_test_channel[];


U16	zd_test_writeAddr[256];
U16	zd_test_writeData[256];
U16	zd_test_writeIndex = 0;

#define D_TEST_FILL_WRITE_REGISTER(addr0, value0)		\
{							\
	zd_test_writeAddr[zd_test_writeIndex] = addr0;		\
	zd_test_writeData[zd_test_writeIndex++] = value0;	\
}
#define D_TEST_SET_IF_SYNTHESIZER(wp, InputValue)		\
{							\
	D_TEST_FILL_WRITE_REGISTER(DZD_CR244, (U8)((InputValue & 0xFF0000) >> 16)); \
	D_TEST_FILL_WRITE_REGISTER(DZD_CR243, (U8)((InputValue & 0xFF00) >> 8));    \
	D_TEST_FILL_WRITE_REGISTER(DZD_CR242, (U8)((InputValue & 0xFF)));	       \
}

static	U32	TEST_AL7230BTB[] = {
		0x09EC04, 0x8CCCC8,   //;Null
		0x09EC00, 0x8CCCC8,   //;Ch 1
		0x09EC00, 0x8CCCD8,   //;Ch 2
		0x09EC00, 0x8CCCC0,   //;Ch 3
		0x09EC00, 0x8CCCD0,   //;Ch 4
		0x05EC00, 0x8CCCC8,   //;Ch 5
		0x05EC00, 0x8CCCD8,   //;Ch 6
		0x05EC00, 0x8CCCC0,   //;Ch 7
		0x05EC00, 0x8CCCD0,   //;Ch 8
		0x0dEC00, 0x8CCCC8,   //;Ch 9
		0x0dEC00, 0x8CCCD8,   //;Ch 10
		0x0dEC00, 0x8CCCC0,   //;Ch 11
		0x0dEC00, 0x8CCCD0,   //;Ch 12
		0x03EC00, 0x8CCCC8,   //;Ch 13
		0x03EC00, 0x866660    //;Ch 14
};
static	U32	TEST_AL7230BTB_a[] = {
		0x06AFF4, 0x855550, 0x47F8A2, 0x21EBFE, //;Null
		0x02AFF0, 0x800000, 0x47F8A2, 0x21EBF6, //;CH 8 ,  5040MHz
		0x02AFF0, 0x855550, 0x47F8A2, 0x21EBFE, //;CH 12,  5060MHz
		0x0AAFF0, 0x8AAAA8, 0x47F8A2, 0x21EBFE, //;CH 16,  5080MHz
		0x06AFF0, 0x8AAAA0, 0x47F8A2, 0x21EBFE, //;CH 34,  5170MHz
		0x06AFF0, 0x855550, 0x47F8A2, 0x21EBFE, //;Ch 36,  5180MHz
		0x0EAFF0, 0x800008, 0x47F8A2, 0x21EBFE, //;Ch 38,  5190MHz
		0x0EAFF0, 0x8AAAA8, 0x47F8A2, 0x21EBFE, //;Ch 40,  5200MHz
		0x0EAFF0, 0x855558, 0x47F8A2, 0x21EBFE, //;Ch 42,  5210MHz
		0x0EAFF0, 0x800000, 0x47F8A2, 0x21EBF6, //;Ch 44,  5220MHz
		0x0EAFF0, 0x8AAAA0, 0x47F8A2, 0x21EBFE, //;Ch 46,  5230MHz
		0x0EAFF0, 0x855550, 0x47F8A2, 0x21EBFE, //;Ch 48,  5240MHz
		0x01AFF0, 0x8AAAA8, 0x47F8A2, 0x21EBFE, //;Ch 52,  5260MHz
		0x01AFF0, 0x800000, 0x47F8A2, 0x21EBF6, //;Ch 56,  5280MHz
		0x01AFF0, 0x855550, 0x47F8A2, 0x21EBFE, //;Ch 60,  5300MHz
		0x09AFF0, 0x8AAAA8, 0x47F8A2, 0x21EBFE, //;Ch 64,  5320MHz
		0x09AFF0, 0x8AAAA8, 0x47F8A2, 0x21EBFE, //;Ch 68,  5320MHz
		0x09AFF0, 0x8AAAA8, 0x47F8A2, 0x21EBFE, //;Ch 72,  5320MHz
		0x09AFF0, 0x8AAAA8, 0x47F8A2, 0x21EBFE, //;Ch 76,  5320MHz
		0x09AFF0, 0x8AAAA8, 0x47F8A2, 0x21EBFE, //;Ch 80,  5320MHz
		0x09AFF0, 0x8AAAA8, 0x47F8A2, 0x21EBFE, //;Ch 84,  5320MHz
		0x09AFF0, 0x8AAAA8, 0x47F8A2, 0x21EBFE, //;Ch 88,  5320MHz
		0x09AFF0, 0x8AAAA8, 0x47F8A2, 0x21EBFE, //;Ch 92,  5320MHz
		0x09AFF0, 0x8AAAA8, 0x47F8A2, 0x21EBFE, //;Ch 96,  5320MHz
		0x03AFF0, 0x8AAAA8, 0x47F8A2, 0x21EBFE, //;Ch 100, 5500MHz
		0x03AFF0, 0x800000, 0x47F8A2, 0x21EBF6, //;Ch 104, 5520MHz
		0x03AFF0, 0x855550, 0x47F8A2, 0x21EBFE, //;Ch 108, 5540MHz
		0x0BAFF0, 0x8AAAA8, 0x47F8A2, 0x21EBFE, //;Ch 112, 5560MHz
		0x0BAFF0, 0x800000, 0x47F8A2, 0x21EBF6, //;Ch 116, 5580MHz
		0x0BAFF0, 0x855550, 0x47F8A2, 0x21EBFE, //;Ch 120, 5600MHz
		0x07AFF0, 0x8AAAA8, 0x47F8A2, 0x21EBFE, //;Ch 124, 5620MHz
		0x07AFF0, 0x800000, 0x47F8A2, 0x21EBF6, //;Ch 128, 5640MHz
		0x07AFF0, 0x855550, 0x47F8A2, 0x21EBFE, //;Ch 132, 5660MHz
		0x0FAFF0, 0x8AAAA8, 0x47F8A2, 0x21EBFE, //;Ch 136, 5680MHz
		0x0FAFF0, 0x800000, 0x47F8A2, 0x21EBF6, //;Ch 140, 5700MHz
		0x0FAFF0, 0x800000, 0x47F8A2, 0x21EBF6, //;Ch 144, 5700MHz
		0x006FF0, 0x800018, 0x47F8A2, 0x21EBFE, //;Ch 149, 5745MHz
		0x006FF0, 0x855540, 0x47F8A2, 0x21EBFE, //;Ch 153, 5765MHz
		0x006FF0, 0x8AAAB0, 0x47F8A2, 0x21EBFE, //;Ch 157, 5785MHz
		0x086FF0, 0x800018, 0x47F8A2, 0x21EBFE, //;Ch 161, 5805MHz
		0x086FF0, 0x855540, 0x47F8A2, 0x21EBFE, //;Ch 165, 5825MHz
		0x086FF0, 0x8D5540, 0x47F8A2, 0x21EBFE, //;Ch 168, 5825MHz
		0x086FF0, 0x8D5540, 0x47F8A2, 0x21EBFE, //;Ch 172, 5825MHz
		0x086FF0, 0x8D5540, 0x47F8A2, 0x21EBFE, //;Ch 176, 5825MHz
		0x086FF0, 0x8D5540, 0x47F8A2, 0x21EBFE, //;Ch 180, 5825MHz
		0x04AFF0, 0x800000, 0x47F8A2, 0x21EBF6, //;Ch 184, 4920MHz
		0x04AFF0, 0x855550, 0x47F8A2, 0x21EBFE, //;Ch 188, 4940MHz
		0x0CAFF0, 0x8AAAA8, 0x47F8A2, 0x21EBFE, //;Ch 192, 4960MHz
		0x0CAFF0, 0x800000, 0x47F8A2, 0x21EBF6	//;Ch 196, 4980MHz
};


static	void	zd_test_setAL7230bRF(wlink_data *wp, int channel, int initChOnly)
{
	U32	channel_temp;

	zd_test_writeIndex = 0;
	Szd_lockPhyReg(wp);	
	Szd_writeReg(wp, DZD_CR240, 0x57);
	Szd_writeReg(wp, DZD_CR9, 0xE4);
	Szd_unlockPhyReg(wp);

	if ( !initChOnly ) {
		D_TEST_FILL_WRITE_REGISTER(DZD_CR10, 0x8B);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR15, 0x20);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR17, 0x2B);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR20, 0x10);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR23, 0x40);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR24, 0x20);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR26, 0x93);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR28, 0x3E);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR29, 0x00);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR33, 0x28);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR34, 0x30);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR35, 0x3E);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR41, 0x24);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR44, 0x32);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR46, 0x99);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR47, 0x1E);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR48, 0x00);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR49, 0x00);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR51, 0x01);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR52, 0x80);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR53, 0x7E);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR65, 0x00);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR66, 0x00);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR67, 0x00);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR68, 0x00);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR69, 0x28);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR79, 0x58);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR80, 0x30);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR81, 0x30);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR87, 0x0A);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR89, 0x04);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR90, 0x58);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR91, 0x00);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR92, 0x0A);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR98, 0x8D);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR99, 0x00);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR100, 0x02);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR101, 0x13);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR102, 0x27);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR106, 0x20);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR109, 0x13);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR112, 0x1F);
		if ( (wp->wd_RF_Mode & BIT_31) ) {	// PhyNewLayout.
			D_TEST_FILL_WRITE_REGISTER(DZD_CR107, 0x28);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR110, 0x1F);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR111, 0x1F);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR116, 0x2A);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR118, 0xFA);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR119, 0x12);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR121, 0x6C);
		} else {
			D_TEST_FILL_WRITE_REGISTER(DZD_CR107, 0x24);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR110, 0x13);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR111, 0x13);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR116, 0x24);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR118, 0xfC);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR119, 0x11);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR121, 0x6A);
		}
		D_TEST_FILL_WRITE_REGISTER(DZD_CR113, 0x27);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR114, 0x27);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR115, 0x24);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR117, 0xFA);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR120, 0x4F);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR122, 0xFC);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR123, 0x57);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR125, 0xAD);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR126, 0x6C);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR127, 0x03);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR130, 0x10);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR131, 0x00);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR137, 0x50);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR138, 0xA8);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR144, 0xAC);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR148, 0x40);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR149, 0x40);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR150, 0x1A);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR252, 0x34);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR253, 0x34);
		if ( wp->wd_macMode != DWLAN_MACMODE_80211A ) {
			D_TEST_FILL_WRITE_REGISTER(DZD_CR251, 0x2F);
			D_TEST_SET_IF_SYNTHESIZER(wp, TEST_AL7230BTB[channel * 2]);
			D_TEST_SET_IF_SYNTHESIZER(wp, TEST_AL7230BTB[channel * 2 + 1]);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x4FF821);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xC5FBFC);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x21EBFE);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xAFD401);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x6CF56A);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xE04073);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x190D76);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x9DD844);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x500007);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xD8C010);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x03C900);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xBFFFFF);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x700000);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xF15D58);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR251, 0x7F);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR128, 0x14);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR129, 0x12);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR130, 0x10);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR38, 0x38);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR136, 0xDF);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xF15D59);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xF15D5C);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xF15D58);
		} else {
			D_TEST_FILL_WRITE_REGISTER(DZD_CR251, 0x2F);
			if( (34 <= channel) && (channel <= 48) ) {
				channel_temp = (channel/2) - 13;
				D_TEST_SET_IF_SYNTHESIZER(wp, TEST_AL7230BTB_a[channel_temp*4]);
				D_TEST_SET_IF_SYNTHESIZER(wp, TEST_AL7230BTB_a[channel_temp*4+1]);
			} else {
				channel_temp = (channel/4) - 1;
				D_TEST_SET_IF_SYNTHESIZER(wp, TEST_AL7230BTB_a[channel_temp*4]);
				D_TEST_SET_IF_SYNTHESIZER(wp, TEST_AL7230BTB_a[channel_temp*4+1]);
			}
			D_TEST_SET_IF_SYNTHESIZER(wp, TEST_AL7230BTB_a[channel_temp*4+3]);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x47f8A2);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xC5FBFA);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xAFFCA1);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x6CF56A);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xE04073);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x190D36);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x9DD844);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x500607);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xD8C010);
			if ( (48 < channel) && (channel < 184) ) {
				D_TEST_SET_IF_SYNTHESIZER(wp, 0x3C2800);
			} else {
				D_TEST_SET_IF_SYNTHESIZER(wp, 0x3E2800);
			}
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xBFFFFF);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x700000);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xF35D48);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR251, 0x7F);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR128, 0x12);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR129, 0x10);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR130, 0x10);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR38, 0x7F);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR136, 0x5F);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xF15D59);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xF15D5C);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xF35D48);
		}
	} else {
		if ( wp->wd_macMode != DWLAN_MACMODE_80211A ) {
			D_TEST_FILL_WRITE_REGISTER(DZD_CR251, 0x2F);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x4FF821);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xC5FBFC);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x21EBFE);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xAFD401);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x6CF56A);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xE04073);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x190D76);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x9DD844);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x500007);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xD8C010);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x3C9000);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xF15D58);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR128, 0x14);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR129, 0x12);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR130, 0x10);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR38, 0x38);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR136, 0xDF);
			D_TEST_SET_IF_SYNTHESIZER(wp, TEST_AL7230BTB[channel*2]);
			D_TEST_SET_IF_SYNTHESIZER(wp, TEST_AL7230BTB[channel*2 + 1]);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x3C9000);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR251, 0x7F);
		} else {
			D_TEST_FILL_WRITE_REGISTER(DZD_CR251, 0x2f);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x190D36);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x47F8A2);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xC5FBFA);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xAFFCA1);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x6CF56A);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xE04073);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x9DD844);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x500607);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xD8C010);
			D_TEST_SET_IF_SYNTHESIZER(wp, 0xF35D48);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR128, 0x12);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR129, 0x10);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR130, 0x10);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR38, 0x7F);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR136, 0x5F);
			if ( (48 < channel) && (channel < 184) ) {
			       D_TEST_SET_IF_SYNTHESIZER(wp, 0x3C2800);
			} else {
			       D_TEST_SET_IF_SYNTHESIZER(wp, 0x3E2800);
			}
			if ( (34 <= channel) && (channel <= 48) ) {
				channel_temp = (channel/2) - 13;
				D_TEST_SET_IF_SYNTHESIZER(wp, TEST_AL7230BTB_a[channel_temp*4]);
				D_TEST_SET_IF_SYNTHESIZER(wp, TEST_AL7230BTB_a[channel_temp*4+1]);
				D_TEST_SET_IF_SYNTHESIZER(wp, TEST_AL7230BTB_a[channel_temp*4+3]);
			} else {
				channel_temp = (channel/4) - 1;
				D_TEST_SET_IF_SYNTHESIZER(wp, TEST_AL7230BTB_a[channel_temp*4]);
				D_TEST_SET_IF_SYNTHESIZER(wp, TEST_AL7230BTB_a[channel_temp*4+1]);
				D_TEST_SET_IF_SYNTHESIZER(wp, TEST_AL7230BTB_a[channel_temp*4+3]);
			}
			D_TEST_FILL_WRITE_REGISTER(DZD_CR251, 0x7F);
		}
	}
	if ( wp->wd_bssType == DWLAN_BSSTYPE_IBSS ) {
		D_TEST_FILL_WRITE_REGISTER(DZD_CR80, 0x0C);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR81, 0x0C);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR79, 0x16);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR12, 0x54);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR77, 0x1B);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR78, 0x58);
	} else {
		D_TEST_FILL_WRITE_REGISTER(DZD_CR80, 0x30);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR81, 0x30);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR79, 0x58);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR12, 0xF0);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR77, 0x1B);
		D_TEST_FILL_WRITE_REGISTER(DZD_CR78, 0x58);
	}
	if ( (wp->wd_RF_Mode & BIT_21) ) {	// HwFeature.
		if ( channel == 1 ) {
			D_TEST_FILL_WRITE_REGISTER(DZD_CR128, 0x0E);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR129, 0x10);
		} else if ( channel == 11 ) {
			D_TEST_FILL_WRITE_REGISTER(DZD_CR128, 0x10);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR129, 0x10);
		} else {
			D_TEST_FILL_WRITE_REGISTER(DZD_CR128, 0x14);
			D_TEST_FILL_WRITE_REGISTER(DZD_CR129, 0x12);
		}
	}
	D_TEST_FILL_WRITE_REGISTER(DZD_CR203, 0x04);
	// write those PHY registers to ZD1211B,
	Szd_lockPhyReg(wp);		
	Szd_writeMultiRegs(wp, zd_test_writeAddr, zd_test_writeData, &zd_test_writeIndex);
	Szd_unlockPhyReg(wp);

	Szd_lockPhyReg(wp);		
	Szd_writeReg(wp, DZD_CR240, 0x80);
	if ( (wp->wd_RF_Mode & BIT_31) )	// PhyNewLayout.		
		Szd_writeReg(wp, DZD_CR9, 0xE5);	
	Szd_writeReg(wp, DZD_CR203, 0x04);
	Szd_unlockPhyReg(wp);
}


static	void	zd_test_switchChannel(wlink_data *wp, int channel, int initChOnly)
{
	Szd_writeReg(wp, DZD_CONFIGPhilips, 0);
	// only support AL7230B.
	zd_test_setAL7230bRF(wp, channel, initChOnly);
}


static	void	zd_test_cont_tx(wlink_data *wp, int enable, int rate)
{
	u_int RateTmp = 0;
	u_long tmpvalue, nLoop;
	u_char tmpChr = 0;

	if (wp->wd_macMode != DWLAN_MACMODE_80211A)
	{
		if (rate > 3)
		{	
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x00093c);
		}	
		else
		{	
			D_TEST_SET_IF_SYNTHESIZER(wp, 0x000abc);	
		}	
	}		
	Szd_lockPhyReg(wp);
	Szd_writeMultiRegs(wp, zd_test_writeAddr, zd_test_writeData, &zd_test_writeIndex);
	Szd_unlockPhyReg(wp);	

	if (enable)
	{   
		Szd_lockPhyReg(wp);
		Szd_writeReg(wp, DZD_CR2, 0x3F);
		Szd_writeReg(wp, DZD_CR138, 0x28);
		Szd_writeReg(wp, DZD_CR33, 0x20);
		
		// Query CR60 until change to 0x04
		nLoop = 20;
		while (nLoop--)
		{
			sys_sleep_ms(10);	// sleep 10 ms
			tmpvalue = Szd_readReg(wp, DZD_CR60);
			if(tmpvalue == 0x04)
				break;
		}
		if(nLoop == 0) 
			dbg_sio_printf("nLoop count down to zero. But it still fails\r\n");			
		Szd_unlockPhyReg(wp);
		
		Szd_lockPhyReg(wp);
		/* 	In order to avoid the uninitial length problem,
   			force to set length to 0x20. */
		Szd_writeReg(wp, DZD_CR134, 0x20);
		switch (rate)
		{
			case 4:			//6M
				RateTmp = 0xB;
				break;
			case 5:			//9M
				RateTmp = 0xF;
				break;
			case 6:			//12M
				RateTmp = 0xA;
				break;
			case 7:			//18M
				RateTmp = 0xE;
				break;
			case 8:			//24M
				RateTmp = 0x9;
				break;
			case 9:			//36M
				RateTmp = 0xD;
				break;
			case 0xA:		//48M
				RateTmp = 0x8;
				break;
			case 0xB:		//54M
				RateTmp = 0xC;
				break;
			default:		//case 0: 1M, case 1: 2M, case 2: 5M, case 3: 11M  
				RateTmp = 0;
				break;
		}
		
		if (RateTmp)
		{
			Szd_writeReg(wp, DZD_CR132, RateTmp);
			tmpvalue = Szd_readReg(wp, DZD_CR159);
			tmpvalue &= ~(BIT_0 + BIT_1 );
			tmpvalue |= BIT_2;
			Szd_writeReg(wp, DZD_CR159, tmpvalue);
			Szd_unlockPhyReg(wp);
			
			Szd_writeReg(wp, DZD_RFCFG1, 7);		// OFDM
			tmpvalue = Szd_readReg(wp, DZD_AfterPNP);
			tmpvalue &= ~BIT_0;
			Szd_writeReg(wp, DZD_AfterPNP, tmpvalue);			
		}
		else
		{
			tmpChr |= BIT_5;		// SHORT_PREAMBLE
			if (wp->wd_RegionCode == DZD_REGION_USA)
				tmpChr &= ~BIT_6;	//USA
			if (wp->wd_RegionCode == DZD_REGION_Japan)
				tmpChr |= BIT_6;//japan	
				
			Szd_writeReg(wp, DZD_CR5, tmpChr);
			Szd_unlockPhyReg(wp);
			Szd_writeReg(wp, DZD_RFCFG1, 3);		// CCK
		}
	}
	else
	{		
		Szd_lockPhyReg(wp);
		Szd_writeReg(wp, DZD_CR2, 0x26);		
		Szd_writeReg(wp, DZD_CR138, 0xA8);		
		Szd_writeReg(wp, DZD_CR33, 0x08);		
		Szd_unlockPhyReg(wp);

		if (rate >= 4)
		{
			Szd_lockPhyReg(wp);		
			tmpvalue = Szd_readReg(wp, DZD_CR159);
			tmpvalue &= ~(BIT_0 + BIT_1 + BIT_2);		
			Szd_writeReg(wp, DZD_CR159, tmpvalue);			
			Szd_unlockPhyReg(wp);
		
			Szd_writeReg(wp, DZD_RFCFG1, 0);		
			tmpvalue = Szd_readReg(wp, DZD_AfterPNP);
			tmpvalue |= BIT_0;
			Szd_writeReg(wp, DZD_AfterPNP, tmpvalue);		
		}
		else
		{
			Szd_writeReg(wp, DZD_RFCFG1, 0);		
			tmpvalue = Szd_readReg(wp, DZD_AfterPNP);
			tmpvalue |= BIT_0;
			Szd_writeReg(wp, DZD_AfterPNP, tmpvalue);
		}
		
		// do device reset
		Szd_writeReg(wp, DZD_Pre_TBTT, 0x01);		
		tmpvalue = Szd_readReg(wp, DZD_PS_Ctrl);
		Szd_writeReg(wp, DZD_PS_Ctrl, tmpvalue | BIT_0);
		sys_sleep_ms(5);	// sleep 5 ms
	}
}


static	void	zd_test_cont_rx(wlink_data *wp, int enable)
{
	Szd_lockPhyReg(wp);
	if (enable)
		Szd_writeReg(wp, DZD_CR24, 0);			// continuous rx mode, set CCA to highest
	else
		Szd_writeReg(wp, DZD_CR24, 0x20);		// normal mode	
	Szd_unlockPhyReg(wp);
	
}


static	void	zd_test_adjust_power(wlink_data *wp, int power)
{
	Szd_lockPhyReg(wp);
	Szd_writeReg(wp, DZD_CR69, 0x28);	// 0x2a: auto power control, 0x28: manual power control
	Szd_writeReg(wp, DZD_CR31, power);
	Szd_unlockPhyReg(wp);
}


void zd_hw_test(void)
{
	wlink_data *wp;
	int idx, channel, cont_tx, power, cont_rx, rate;
			
	if ((wp = Szd_wlinkDataPtr(1)) == NULL)
	{
		dbg_sio_printf("Interface WLAN doesn't exist!\r\n");
		return;
	}	
	
	// channel
	idx = Scf_getWlanTestChannel();
	channel = atoi(Cwlan_hw_test_channel[idx]);	
	if (channel > 14)
		wp->wd_macMode = DWLAN_MACMODE_80211A;
	else
		wp->wd_macMode = DWLAN_MACMODE_MIXED;	
	zd_test_switchChannel(wp, channel, 1);
	
	// tx rate
	rate = Scf_getWlanTxRate();	
	
	// tx power
	power = Scf_getWlanTxPower();	
	zd_test_adjust_power(wp, power);
		
	// continuous Tx	
	cont_tx = Scf_getWlanContinuousTx();
	zd_test_cont_tx(wp, cont_tx, rate);
	
	// continuous rx
	cont_rx = Scf_getWlanContinuousRx();
	zd_test_cont_rx(wp, cont_rx);
}

#endif	/* WLAN_HW_TEST */