#include <stdio.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#define IOCTL_MAGIC 0xBF
#define IOCTL_RED_LED_ON _IO(IOCTL_MAGIC, 1)
#define IOCTL_RED_LED_ON_WRITE_PARAM _IOW(IOCTL_MAGIC, 2, int)
#define IOCTL_RED_LED_ON_READ_PARAM _IOR(IOCTL_MAGIC, 3, int)
#define IOCTL_RED_LED_ON_RW_PARAM _IOWR(IOCTL_MAGIC, 4, int)


int led_open(char *filename){

	int fd;
	fd  = open(filename, O_RDONLY|O_APPEND);
	if (fd < -1){
		printf("Error opening file");
		exit(-1);
	}
	return fd;
}

int main(void){

	int fd;
	fd = led_open("/dev/chethan");
	ioctl(fd,IOCTL_RED_LED_ON_WRITE_PARAM,1);
	sleep(1);
	ioctl(fd,IOCTL_RED_LED_ON_WRITE_PARAM,0);
	return 0;
}
