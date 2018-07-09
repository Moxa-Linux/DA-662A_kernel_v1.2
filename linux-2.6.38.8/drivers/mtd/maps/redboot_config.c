// Description: Get the software ID
//   Macro definition is defined in inux-2.6.x/include/asm/redboot_config.h
// 06-10-2004: Victor Yu. created
// 05-17-2007: Jared create for reading from redboot partition
// 05-10-2010: Get the software ID via checking the PCI device
// 07-25-2011: Porting to linux-2.6.38.8

#if 1 // Jared, 05-17-2007

#include	<linux/module.h>
#include	<linux/string.h>
#include	<linux/slab.h>
#include	<mach/redboot_config.h>
#include	<mach/hardware.h>
#define	REDCONFIG_MTD_NO 3

int	config_length(int type)
{
	switch ( type ) {
	case CONFIG_BOOL :
	case CONFIG_INT :
	case CONFIG_IP :
		return sizeof(unsigned long);
	case CONFIG_ESA :
		return 8;
	case CONFIG_NETPORT :
	case CONFIG_STRING :
		return MAX_STRING_LENGTH;
	case CONFIG_SCRIPT :
		return MAX_SCRIPT_LENGTH;
	default :
		return 0;
	}
	return 0;
}

#include <linux/mtd/mtd.h>

int	config_get(char *nkname, void *value)
{
	volatile u_char	*dp, *tmpbuf;
	volatile struct _config	*config;
	struct mtd_info		*mtd;
	size_t i, len;

	tmpbuf = kmalloc(MAX_CONFIG_DATA, GFP_KERNEL);
	if ( !tmpbuf ) {
		printk("%s[%d]:kmalloc() fail!\n",__FUNCTION__,__LINE__);
		return -ENOMEM;
	}
	mtd = get_mtd_device(NULL, REDCONFIG_MTD_NO);

	#if 0 // Turn on for debug only

	printk("%s[%d]mtd->type:%x\n", __FUNCTION__, __LINE__, mtd->type );
	printk("%s[%d]mtd->size:%x\n", __FUNCTION__, __LINE__, mtd->size );
	printk("%s[%d]mtd->erasesize:%x\n", __FUNCTION__, __LINE__, mtd->erasesize );
	//printk("%s[%d]mtd->ecctype:%x\n", __FUNCTION__, __LINE__, mtd->ecctype );
	//printk("%s[%d]mtd->oobavail:%x\n", __FUNCTION__, __LINE__, mtd->oobavail );
	//printk("%s[%d]mtd->eccsize:%x\n", __FUNCTION__, __LINE__, mtd->eccsize );
	printk("%s[%d]mtd->name:%s\n", __FUNCTION__, __LINE__, mtd->name );
	printk("%s[%d]mtd->index:%x\n", __FUNCTION__, __LINE__, mtd->index );
	//printk("%s[%d]mtd->oobinfo.useecc:%x\n", __FUNCTION__, __LINE__, mtd->oobinfo.useecc );
	//printk("%s[%d]mtd->oobinfo.eccbytes:%x\n", __FUNCTION__, __LINE__, mtd->oobinfo.eccbytes );
	//printk("%s[%d]mtd->oobinfo.eccpos:%d,%d,%d\n", __FUNCTION__, __LINE__, mtd->oobinfo.eccpos[0], mtd->oobinfo.eccpos[1], mtd->oobinfo.eccpos[2]);
	printk("%s[%d]mtd->numeraseregions:%x\n", __FUNCTION__, __LINE__, mtd->numeraseregions );

	//printk("%s[%d]mtd->oobblock:%x\n", __FUNCTION__, __LINE__, mtd->oobblock );
	printk("%s[%d]mtd->oobsize:%x\n", __FUNCTION__, __LINE__, mtd->oobsize );
	//printk("%s[%d]mtd->ecctype:%x\n", __FUNCTION__, __LINE__, mtd->ecctype );
	printk("%s[%d]mtd->oobavail:%x\n", __FUNCTION__, __LINE__, mtd->oobavail );
	//printk("%s[%d]mtd->eccsize:%x\n", __FUNCTION__, __LINE__, mtd->eccsize );
	#endif
	if ( mtd == NULL ) {
		kfree((void*)tmpbuf);
		printk("%s[%d]:get_mtd_device(NULL, REDCONFIG_MTD_NO) fail!\n",__FUNCTION__,__LINE__);
		return -ENODEV;
	}
	#if 0  // 08-13-2008 Comment out by Jared. 
	mtd->read(mtd, 0, mtd->size, &len, tmpbuf);
	#else  // Read from offset: 0x1f000
	mtd->read(mtd, 0x1f000, mtd->size, &len, (char*)tmpbuf);
	#endif
	#if 0 // Dump the contents of redboot partition.
	printk("%s[%d]mtd->size:%x, len:%x\n", __FUNCTION__, __LINE__, mtd->size, len);
	for( i=0; i < len; i++ ) {
		printk("%02x,", *(tmpbuf+i));
		if ( i%16 == 15 )
			printk("\n");
	}
	#endif
	
	config = (struct _config *)tmpbuf;
	dp = &config->config_data[0];
	while ( dp < &config->config_data[sizeof(config->config_data)] && CONFIG_OBJECT_TYPE(dp) != CONFIG_EMPTY ) {
		if ( strcmp(nkname, (char*) CONFIG_OBJECT_KEY(dp)) == 0 ) {
			switch ( CONFIG_OBJECT_TYPE(dp) ) {
				case CONFIG_BOOL :
				case CONFIG_INT :
				case CONFIG_IP :
					memcpy(value, (void*)CONFIG_OBJECT_VALUE(dp), sizeof(unsigned long));
					break;
				break;
				case CONFIG_ESA :
					memcpy(value, (void*)CONFIG_OBJECT_VALUE(dp), 6);
					break;
				break;
				case CONFIG_NETPORT :
				case CONFIG_STRING :
				case CONFIG_SCRIPT :
					strcpy(value, (void*)CONFIG_OBJECT_VALUE(dp));
					break;
				default:
					printk("%s[%d]:CONFIG_OBJECT_TYPE(dp):%d fail!\n", __FUNCTION__, __LINE__, CONFIG_OBJECT_TYPE(dp));
					put_mtd_device(mtd);
					kfree((void*)tmpbuf);
					return -1;
			}
			put_mtd_device(mtd);
			kfree((void*)tmpbuf);
			return 0;
		} // end of if
		dp +=  4 + CONFIG_OBJECT_KEYLEN(dp) + CONFIG_OBJECT_ENABLE_KEYLEN(dp) + config_length(CONFIG_OBJECT_TYPE(dp));
	} // end of while

	put_mtd_device(mtd);
	kfree((void*)tmpbuf);

	return 1; // not found
}
#endif // Jared, 05-17-2007

#if defined(__SOFTWARE_ID_PCI_IDENTIFICATION__)  // Jared, 05-10-2010
#include <linux/pci.h>
/*  Description: count the number of the device by vendor/device ID */
int pci_device_count(unsigned int vendor, unsigned int device) {
	int device_count = 0;
	struct pci_dev *pdev = NULL;
	
	do {
		pdev=pci_get_device(vendor, device, pdev);
		if( pdev ) {
			//printk("%s[%d]\n",__FUNCTION__,__LINE__);
			device_count++;
		}
	}
	while ( pdev!=NULL );

	return device_count;
}

int	config_get_target_swid(void *value)
{
		int nRTL8139, nMU860, nCAN, nSM502, nATH9K;
		unsigned int *software_id;

		software_id = value;

		nRTL8139 = pci_device_count(0x10ec, 0x8139);
		nMU860 = pci_device_count(0x1393, 0x0001);
		nCAN = pci_device_count(0x1393, 0x0100);
		nSM502 = pci_device_count(0x126f, 0x0501);
		nATH9K = pci_device_count(0x168c, 0x0029);

		if ( nRTL8139 == 1 && nMU860 ==1 && nCAN == 0 && nSM502 ==0 )
			*software_id = 0x80008410;
		else if ( nRTL8139 == 0 && nMU860 ==1 && nCAN == 0 && nSM502 ==0 )
			*software_id = 0x80008411;
		else if ( nRTL8139 == 2 && nMU860 ==1 && nCAN == 0 && nSM502 ==0 )
	#if defined(CONFIG_MACH_MOXA_DCU8620) // Jared 10-04-2012, UC-8416 and DCU-8620 has the same RTL8139 and MU860, but each should has a distinct ID
			*software_id = 0x80008620;
	#else
			*software_id = 0x80008416;
	#endif
		else if ( nRTL8139 == 1 && nMU860 ==1 && nCAN == 1 && nSM502 ==0 )
			*software_id = 0x80008418;
		else if ( nRTL8139 == 1 && nMU860 ==1 && nCAN == 0 && nSM502 ==1 )
			*software_id = 0x80008430;
		else if ( nRTL8139 == 0 && nMU860 ==1 && nCAN == 0 && nATH9K==1 )
			*software_id = 0x80008481;
		else if ( nRTL8139 == 0 && nMU860 ==1 && nCAN == 0 && nATH9K==2 )
			*software_id = 0x80008482;
		else if ( nRTL8139 == 0 && nMU860 ==1 && nCAN == 0 && nATH9K==3 )
			*software_id = 0x80008483;
		else {
			*software_id = 0x80008410;
			return 1; // Not found
		}

		return 0;
}
EXPORT_SYMBOL(config_get_target_swid);
#endif // Masked by Jared 05-10-2010

EXPORT_SYMBOL(config_get);
EXPORT_SYMBOL(config_length);
