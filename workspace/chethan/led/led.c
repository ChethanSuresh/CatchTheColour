#include <stdio.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

#include <linux/ioctl.h>
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

void led_read(int fd){ 

	void *buf[10]; 
	ssize_t read_size;

	read_size = read(fd, buf, 10);
	printf("Read: %p, size: %d\n", buf, read_size);
}

/*
void led_write(int fd,char o[]){
	ssize_t size_write;	
	size_write = write(fd, o, 10);
	printf("Read: %s, size: %d\n", o, size_write);
	close(fd);
}
*/

void help(void){
	printf("Error in args\n");
	printf("Usage:\n");
	printf("led <filename> <r|w [0|1]> \n");

}
int main(int argc, char *argv[]){

	int fd;
	if (argc < 3){
		help();
		exit(-1);
	}
	if (*argv[2] == 'r'){
		fd = led_open(argv[1]);
		led_read(fd);
		ioctl(fd,IOCTL_RED_LED_ON);
		ioctl(fd,IOCTL_RED_LED_ON_PARAM,1);
		close(fd);
	}
	
	/*
	else if (*argv[3] == 'w'){
		if (argc < 4) {
			help();
			exit(-1);
		}
		fd = led_open(argv[1]);
		char o[2];
		strcat(o, argv[4]);
		led_write(fd,o);
	}
	*/
	return 0;
}
