#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdlib.h>

#define IOCTL_MAGIC 0xBF
#define IOCTL_APP_INFO _IOR(IOCTL_MAGIC, 1, int )
static int value = 0;

int fd;
FILE *temp_fd;

void signal_handler(int signo){
	if (signo == SIGINT){
		usleep(500000);
		printf("received SIGINT\n");
	}
	if (value == 0){
		value = 1;
	}else{
		value = 0;
	}
	temp_fd = fopen("/sys/class/gpio/gpio200/value", "w");
	fprintf(temp_fd, "%d", value);
	fclose(temp_fd);
	close(fd);
}

int main(){
	printf("PID: %d\n", getpid());
	/* Open driver */
	fd = open("/dev/button", O_RDONLY);
	if (fd < 0 ){
		printf("No /dev/button file found\n");
		exit(1);
	}

	/* Call IOCTL to register app with driver */
	int ret = ioctl(fd, IOCTL_APP_INFO, getpid());
	if (ret < 0){
		printf("Coud not call IOCTL %d\n", ret);
	}

	if (signal(SIGINT, signal_handler) == SIG_ERR)
		printf("\ncan't catch \n");

	while(1){
		sleep(1);
	}
	close(fd);
}
