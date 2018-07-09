/*
 *	This is an example of a mixer program for Linux
 *
 *	updated 1/1/93 to add stereo, level query, broken
 *      	devmask kludge - cmetz@thor.tjhsst.edu 
 *
 * (C) Craig Metz and Hannu Savolainen 1993.
 *
 * You may do anything you wish with this program.
 */

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/soundcard.h>

extern int errno;

char *names[SOUND_MIXER_NRDEVICES] = SOUND_DEVICE_NAMES;

int devmask = 0, recmask = 0, recsrc = 0;

void usage(void)
{
	int i, n = 0;

#if 1 //Jared, 05-07-2010
	printf("Usage: mixer vol <value>\n");
#else
	printf("Usage: mixer { ");
	for (i = 0; i < SOUND_MIXER_NRDEVICES; i++) 
		if ((1 << i) & devmask)  {
			if (n)
				putchar('|');
			printf(names[i]);
			n = 1;
		} 
	printf(" } <value>\n  or   mixer { +rec|-rec } <devicename>\n");
	printf("  or   mixer ?rec (prints the current recording source)\n");
#endif
	exit(1);
}

void print_recsrc(void)
{
	int i, n = 0;
	fprintf(stderr, "Recording source: ");

	for (i = 0; i < SOUND_MIXER_NRDEVICES; i++)
		if ((1 << i) & recsrc) {
			if (n)
				fprintf(stderr, ", ");
			fprintf(stderr, names[i]);
			n = 1;
		}
	fprintf(stderr, "\n");
}

int 
main(int argc, char *argv[])
{
	int ctrl, mode, arg, mixerfd, dev, left, right;

	char name[30] = "/dev/mixer";

	if (!strcmp(argv[0], "mixer0"))
	   strcpy(name, "/dev/mixer0");
	else
	  if (!strcmp(argv[0], "mixer1"))
	     strcpy(name, "/dev/mixer1");
	else
	  if (!strcmp(argv[0], "mixer2"))
	     strcpy(name, "/dev/mixer2");
	else
	  if (!strcmp(argv[0], "mixer3"))
	     strcpy(name, "/dev/mixer3");
	else
	  if (!strcmp(argv[0], "mixer4"))
	     strcpy(name, "/dev/mixer4");
	else
	  if (!strcmp(argv[0], "mixer5"))
	     strcpy(name, "/dev/mixer5");

	if ((mixerfd = open(name, O_RDWR)) < 0) {
		perror(name);
		exit(1);
	}
	if (ioctl(mixerfd, SOUND_MIXER_READ_DEVMASK, &devmask) == -1) {
		perror("SOUND_MIXER_READ_DEVMASK");
		exit(-1);
	}
	if (ioctl(mixerfd, SOUND_MIXER_READ_RECMASK, &recmask) == -1) {
		perror("SOUND_MIXER_READ_RECMASK");
		exit(-1);
	}
#if 1 // Jared, 05-07-2010, set the default recording device as line in
	recsrc=(1<<SOUND_MIXER_LINE);
	if (ioctl(mixerfd, SOUND_MIXER_WRITE_RECSRC, &recsrc) == -1) {
				perror("SOUND_MIXER_WRITE_RECSRC");
				exit(-1);
			}
#else
	if (ioctl(mixerfd, SOUND_MIXER_READ_RECSRC, &recsrc) == -1) {
		perror("SOUND_MIXER_READ_RECSRC");
		exit(-1);
	}
#endif

	switch (argc) {
		case 3:
			mode = 1;
			break;
		case 2:
			mode = 0;
			break;
		default:
			usage();
	}

	for (ctrl = 0; ctrl < SOUND_MIXER_NRDEVICES && strcmp(names[ctrl], argv[1]); ctrl++);

 	if ((ctrl >= SOUND_MIXER_NRDEVICES) || (!strcmp("?rec",argv[1]))) { 

#if 0 //Jared, 05-07-2010
		if (!strcmp("+rec", argv[1]) || !strcmp("-rec", argv[1])) {
			for (dev = 0; dev < SOUND_MIXER_NRDEVICES && strcmp(names[dev], argv[2]); dev++);
			if (dev >= SOUND_MIXER_NRDEVICES)
				usage();

			if (!((1 << dev) & recmask)) {
				fprintf(stderr, "Invalid recording source %s\n", argv[2]);
				exit(-1);
			}
			if (argv[1][0] == '+')
				recsrc |= (1 << dev);
			else
				recsrc &= ~(1 << dev);

			if (ioctl(mixerfd, SOUND_MIXER_WRITE_RECSRC, &recsrc) == -1) {
				perror("SOUND_MIXER_WRITE_RECSRC");
				exit(-1);
			}
			print_recsrc();
		} else if(!strcmp("?rec",argv[1])) {
#else
		if(strcmp("?rec",argv[1])) {
#endif
			print_recsrc();
		} else
			usage();
	} else {
		if (mode) {
			if (strchr(argv[2], ':') == NULL) {
				sscanf(argv[2], "%d", &left);
				right = left; /* Tandem setting */
			} else 
				sscanf(argv[2], "%d:%d", &left, &right);
	
			if (left < 0)
				left = 0;
			if (right < 0)
				right = 0;
			if (left > 100)
				left = 100;
			if (right > 100)
				right = 100;


                        arg = left | (right << 8);
			if (ioctl(mixerfd, MIXER_WRITE(ctrl), &arg) == -1)
			{
			   if (errno == EINVAL)
			      fprintf(stderr, "Invalid mixer control %s\n",
					names[ctrl]);
			   else
			      perror("MIXER_WRITE");
			   exit(-1);
			}
			printf("Setting the mixer %s to %d:%d.\n", 
				names[ctrl], left, right);
			exit (0);
		} else {
			if (ioctl(mixerfd, MIXER_READ(ctrl),&arg)== -1)
			{
			   if (errno == EINVAL)
			      fprintf(stderr, "Invalid mixer control %s\n",
					names[ctrl]);
			   else
			      perror("MIXER_READ");
			   exit(-1);
			}
			printf("The mixer %s is currently set to %d:%d.\n", 
				names[ctrl], arg & 0xff, (arg >> 8) & 0xff);
		}
	}

	close(mixerfd);
}
