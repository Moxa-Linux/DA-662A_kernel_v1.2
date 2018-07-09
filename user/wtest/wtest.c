#include "wtest.h"
#include <linux/delay.h>

int gchannel;
unsigned char gpower;
int genable;
unsigned char grate;

unsigned long read_reg(unsigned long address)
{
    unsigned long ret;	
    int sock;
    struct ifreq req;
    struct zdap_ioctl zdreq;
    strcpy(req.ifr_name, "eth1");
    zdreq.addr = 0;
    zdreq.value = 0;
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock < 0) {
        printf("eth1: open socket error!\n");
	goto read_reg_fail;
    }
    zdreq.addr = address;
    zdreq.value = 0x00;
    zdreq.cmd = ZD_IOCTL_REG_READ;
    req.ifr_data = (char *)&zdreq;

    if (ioctl(sock, ZDAPIOCTL, &req) < 0) {
        printf("eth1: ioctl socket error!\n");
	goto read_reg_fail;
    }

   ret = zdreq.value;
   return ret; 
read_reg_fail:
   return -1;
}


int write_reg(unsigned long address,unsigned long value)
{
    int sock;
    struct ifreq req;
    struct zdap_ioctl zdreq;
    strcpy(req.ifr_name, "eth1");
    zdreq.addr = 0;
    zdreq.value = 0;
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock < 0) {
        printf("eth1: open socket error!\n");
	goto write_reg_fail;
    }

    zdreq.addr = address;
    zdreq.value = value;
    zdreq.cmd = ZD_IOCTL_REG_WRITE;
    req.ifr_data = (char *)&zdreq;

    if (ioctl(sock, ZDAPIOCTL, &req) < 0) {
        printf("eth1: ioctl socket error!\n");
	goto write_reg_fail;
    }
    
    return 0;
write_reg_fail:
    return -1;
}


static int setChannel(int channel,int macmode)
{
    int sock;
    struct ifreq req;
    struct zdap_ioctl zdreq;
    strcpy(req.ifr_name, "eth1");
    zdreq.addr = 0;
    zdreq.value = 0;
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock < 0) {
        printf("eth1: open socket error!\n");
	goto  setChannel_fail;
    }

    zdreq.addr = channel;
    zdreq.value = macmode;
    zdreq.cmd = ZD_IOCTL_SET_CHANNEL;
    req.ifr_data = (char *)&zdreq;

    if (ioctl(sock, ZDAPIOCTL, &req) < 0) {
        printf("eth1: ioctl socket error!\n");
	goto   setChannel_fail;
    }    
    return 0;
setChannel_fail:   
    return -1;

}

static	void  test_cont_tx(int enable, unsigned char rate)
{
    unsigned long tmpvalue;  
    unsigned int RateTmp; 
    unsigned char tmpChr;
    int nLoop;

	if (enable)
	{  
		write_reg( ZD_CR2,0x3f);
		write_reg( ZD_CR138,0x28);
		write_reg( ZD_CR33,0x20);

		// Query CR60 until change to 0x04
		nLoop = 20;
		while (nLoop--)
		{
			usleep(10000);	// sleep 10 ms
			tmpvalue = read_reg(ZD_CR60);
			if(tmpvalue == 0x04)
				break;
		}

		/*
		if(nLoop == 0) 
			dbg_printf("nLoop count down to zero. But it still fails\r\n");			
                */			
		
		/* 	In order to avoid the uninitial length problem,
   			force to set length to 0x20. */
		write_reg( ZD_CR134,0x20);
		
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
			write_reg( ZD_CR132,RateTmp);
			tmpvalue = read_reg(ZD_CR159);
			tmpvalue &= ~( ( 1 << 0 ) + ( 1 << 1 ) );
			tmpvalue |= ( 1 << 2 );

			write_reg( ZD_CR159, tmpvalue);
			write_reg( RFCFG1 , 0x0007);
			
			
			tmpvalue = read_reg(ZD_AfterPNP);
			tmpvalue &= ~( 1 << 0);
			write_reg( ZD_AfterPNP, tmpvalue);
		}
		else
		{
			tmpChr |= ( 1 << 5);		// SHORT_PREAMBLE
			if ( gchannel == 12 ||  gchannel == 13 || gchannel == 14 )
				tmpChr |= ( 1 << 6);
			else
				tmpChr &= ~( 1 << 6);
			
			write_reg( ZD_CR5, tmpChr);
			write_reg( RFCFG1, 0x0003);

		}
	}
	else
	{	
		write_reg( ZD_CR2, 0x0026);
		write_reg( ZD_CR138, 0x00a8);
		write_reg( ZD_CR33, 0x0008);

		if (rate >= 4)
		{

			tmpvalue = read_reg(ZD_CR159);
			tmpvalue &= ~( ( 1 << 0 ) + ( 1 << 1 ) + ( 1 << 2 ));		
			write_reg( ZD_CR159,tmpvalue);
			write_reg( RFCFG1,0x0000);

			tmpvalue = read_reg(ZD_AfterPNP);
			tmpvalue |= ( 1 << 0 );
			write_reg(ZD_AfterPNP,tmpvalue);
		}
		else
		{
			write_reg( RFCFG1,0x0000);
			tmpvalue = read_reg(ZD_AfterPNP);
			tmpvalue |= ( 1 << 0);
			write_reg(ZD_AfterPNP,tmpvalue);

		}
		
		// do device reset
		write_reg( ZD_Pre_TBTT,0x0001);
		tmpvalue = read_reg(ZD_PS_Ctrl);
		tmpvalue |= ( 1 << 0 );
		write_reg( ZD_PS_Ctrl,tmpvalue);
		usleep(5000);	// sleep 5 ms
	}
}


static	void	test_cont_rx(int enable)
{
	if (enable)
		write_reg( ZD_CR24,0x0000);
	else
		write_reg( ZD_CR24,0x0020);
}


static void switch_txpower(unsigned char txpower)
{
	write_reg( ZD_CR69,0x0028);
	write_reg( ZD_CR31,txpower);
}


static void switch_channel(int channel)
{
    char sysstr[30];	

    write_reg( ZD_CONFIGPhilips ,0x0000);

    if ( channel > 14 )
	setChannel(channel,0x04);
    else 
	setChannel(channel,0x02);
}

int check11Achannel(int mChannel)
{
    int ret = -1;
    int count;
    for (  count = 0 ; count < 35 ; count++ )
    {
       if ( mChannel ==  dot11A_Channel[count] )  
       {	       
	  ret = 1;     
	  break;
       }
    }

    return ret;
}


int checkopt(int argc,char** argv)
{
    int ret = -1;
    if ( argc != 5 )
    {
	    ret  = -1;
	    goto endcheckopt;
    }
    else
    {
    	sscanf(argv[1], "%d", &gchannel );

		if ( ( gchannel > 14 ) && ( check11Achannel(gchannel) == -1 ) )
		{
                   ret = 0;
		   printf("11A had no this channel!\n");
 		   goto endcheckopt;
		}	
 
 	sscanf(argv[2], "%x", &gpower );
 	sscanf(argv[3], "%d", &genable );
 	sscanf(argv[4], "%x", &grate );
	ret = 1;
    }
endcheckopt:
	return ret;   
}

static void usage()
{
    printf("wtest usage:\n");
    printf("wtest channel txpower enable/disable rate\n");
    printf("wtest     1      0x7f       1         4  \n");
}



int main(int argc,char** argv)
{
    int errno;	
    errno = checkopt(argc,argv);
        switch ( errno )
	{
		case -1:
			usage();
			break;
		case 1:
			system("echo 8 > /proc/sys/kernel/printk");
		// channel
			switch_channel(gchannel);
		// tx power
			switch_txpower(gpower);
		// continuous Tx	
			test_cont_tx(genable, grate);
		// continuous rx
			test_cont_rx(genable);
			break;		  
		case 0:
		   	break;		
	   	
	}

	exit(0);
}
