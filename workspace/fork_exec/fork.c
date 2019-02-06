#include <unistd.h>
#include <stdio.h>

int main(void){
	pid_t pid;
	//char * argv_list[] = {"ls","-lart","/home",NULL};
	char *argv[] = {"sleep", "1000"};
	pid = fork();

	printf("pid = %d\n", pid);
	if (pid == 0){
		execv("/bin/sleep", argv);
	}
	sleep(1000);
	return 0;
}
