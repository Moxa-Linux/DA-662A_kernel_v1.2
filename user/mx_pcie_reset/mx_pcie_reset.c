#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define MX_USB_GPIO_MINOR	108
#define IOCTL_CELLULAR_POWER	_IOW(MX_USB_GPIO_MINOR,0,int)
#define IOCTL_CELLULAR_IGT	_IOW(MX_USB_GPIO_MINOR,1,int)
#define IOCTL_PCIE_RESET	_IOW(MX_USB_GPIO_MINOR,2,int)
#define IOCTL_PCIE_WDISABLE	_IOW(MX_USB_GPIO_MINOR,3,int)
#define MIN_SLOT	1	/* slot-number is 1 ~ 3 */
#define MAX_SLOT	3

#define PCIE_DEV "/dev/moxa_usb_gpio"

struct slot_info {
	int     num;
	int     value;  // 0: for active; else: idle
};

extern int optind, opterr, optopt;
extern char *optarg, **environ;

int main(int argc, char *argv[]) {
	int	fd, ret, i, c ;
	int	bPCIE_Wireless_disable=0;
	int	bPCIE_reset=0;
	char	optstring[] = "rds:";
	struct	slot_info slot;

	if ( argc < 2 ) {
usage:
		printf("Reset the PH8 module/PCIe slot utility\n");
		printf("Usage: %s -s slot-number\n", argv[0]);
		printf("	slot-number	- 1 ~ 3\n");
		exit(0);
	}
	
	while ((c = getopt(argc, argv, optstring)) != -1)
		switch (c) {
		case 's':
			if ( optarg == NULL )
				goto usage;
			slot.num = atoi(optarg); 
			if ( slot.num < MIN_SLOT || slot.num > MAX_SLOT )
				goto usage;
			break;
		case 'd':
			bPCIE_Wireless_disable=1;
			break;
		case 'r':
			bPCIE_reset=1; 
			break;
		case '?':
			printf("Invalid option\n");
			goto usage;
		default:
			goto usage;
		}

	fd = open(PCIE_DEV, O_RDONLY);
	if ( fd < 0 ) {
		printf("Open file %s fail !\n", PCIE_DEV);
		exit(1);
	}

	if ( bPCIE_Wireless_disable == 1 ) {
		printf("Disable the mini PCIe slot[%d]\n", slot.num);
		slot.value=0;	// Disable Wireless module
		ret = ioctl(fd, IOCTL_PCIE_WDISABLE, &slot);
		sleep(1);
		slot.value=1;	// Entering idle state
		ret = ioctl(fd, IOCTL_PCIE_WDISABLE, &slot);
	}
	else if ( bPCIE_reset == 1 ) {
		printf("Reset the mini PCIe slot[%d]\n", slot.num);
		slot.value=0;	// Reset Wireless module
		ret = ioctl(fd, IOCTL_PCIE_RESET, &slot);
		sleep(1);
		slot.value=1;	// Entering idle state
		ret = ioctl(fd, IOCTL_PCIE_RESET, &slot);
	}
	else {
		/* Power off, 3 seconds latter, power on */
		printf("Power off the PH8[%d] module\n", slot.num);

		slot.value=0;	/* Power off*/
		ret = ioctl(fd, IOCTL_CELLULAR_POWER, &slot);
		sleep(3);
		slot.value=1;	/* idle*/
		ret = ioctl(fd, IOCTL_CELLULAR_POWER, &slot);

		sleep(1);

		printf("Initialize the PH8[%d] module\n", slot.num);
		/* IGT on, 1 seconds latter, idle */
		slot.value=0;	/* IGT on*/
		ret = ioctl(fd, IOCTL_CELLULAR_IGT, &slot);
		sleep(1);
		slot.value=1;	/* idle*/
		ret = ioctl(fd, IOCTL_CELLULAR_IGT, &slot);

		printf("Complete the PH8[%d] reset\n", slot.num);
	}

	close(fd);
}
