#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

//#define LED "/sys/class/gpio/gpio489"
#define SWITCH "/sys/class/gpio/gpio91"

int main(void){
	char value_str[3];
	int	state;
	int fd;
	while(1){	
		fd = open(SWITCH "/value", O_RDONLY);
		read(fd, value_str, 3);
		state = atoi(value_str);
		usleep(190000);
		if (state == 0){
			printf("Value: %d\n", state);	
		}
		close(fd);
	}
	return 0;
}
