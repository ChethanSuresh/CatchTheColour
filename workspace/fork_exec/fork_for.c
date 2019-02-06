#include <unistd.h>
#include <stdio.h>

int count = 0;
int main(void){
	pid_t pid;
	//char *argv[] = {"sleep", "1"};
	pid = fork();

	printf("pid = %d\n", pid);
	if (pid == 0){
		//execv("/bin/sleep", argv);
		for (;;){
			count++;
		}
	}
	sleep(1);
	printf("Count: %d\n", count);
	return 0;
}
